/*
 * Copyright (C) 2004-2009 by Objective Systems, Inc.
 *
 * This software is furnished under an open source license and may be
 * used and copied only in accordance with the terms of this license.
 * The text of the license may generally be found in the root
 * directory of this installation in the COPYING file.  It
 * can also be viewed online at the following URL:
 *
 *   http://www.obj-sys.com/open/license.html
 *
 * Any redistributions of this file including modified versions must
 * maintain this copyright notice.
 *
 *****************************************************************************/

#include "ooStackCmds.h"
#include "ootrace.h"
#include "ooq931.h"
#include "ooh245.h"
#include "ooh323ep.h"
#include "oochannels.h"
#include "ooCalls.h"
#include "ooCmdChannel.h"

/** Global endpoint structure */
extern OOH323EndPoint gH323ep;

OOSOCKET gCmdChan = 0;
#ifdef _WIN32
char gCmdIP[20];
int gCmdPort = OO_DEFAULT_CMDLISTENER_PORT;
#else
pthread_mutex_t gCmdChanLock;
#endif

#ifdef _WIN32
int ooCreateCmdListener()
{
   int ret=0;
   OOIPADDR ipaddrs;

   OOTRACEINFO3("Creating CMD listener at %s:%d\n",
                  gH323ep.signallingIP, gH323ep.cmdPort);
   if((ret=ooSocketCreate (&gH323ep.cmdListener))!=ASN_OK)
   {
      OOTRACEERR1("ERROR: Failed to create socket for CMD listener\n");
      return OO_FAILED;
   }
   ret= ooSocketStrToAddr (gH323ep.signallingIP, &ipaddrs);
   if((ret=ooSocketBind (gH323ep.cmdListener, ipaddrs,
                         gH323ep.cmdPort))==ASN_OK)
   {
      ooSocketListen(gH323ep.cmdListener,5); /*listen on socket*/
      OOTRACEINFO1("CMD listener creation - successful\n");
      return OO_OK;
   }
   else
   {
      OOTRACEERR1("ERROR:Failed to create CMD listener\n");
      return OO_FAILED;
   }

   return OO_OK;
}
#endif

int ooCreateCmdConnection()
{
   int ret=0;
#ifdef _WIN32
   if((ret=ooSocketCreate (&gCmdChan))!=ASN_OK)
   {
      return OO_FAILED;
   }
   else
   {

     //TODO:Need to add support for multihomed to work with channel driver

      /*
         bind socket to a port before connecting. Thus avoiding
         implicit bind done by a connect call. Avoided on windows as
         windows sockets have problem in reusing the addresses even after
         setting SO_REUSEADDR, hence in windows we just allow os to bind
         to any random port.
      */
      ret = ooBindOSAllocatedPort(gCmdChan, gCmdIP);

      if(ret == OO_FAILED)
      {
         return OO_FAILED;
      }


      if((ret=ooSocketConnect(gCmdChan, gCmdIP,
                           gCmdPort)) != ASN_OK)
         return OO_FAILED;

   }
#else /* Linux/UNIX - use pipe */
   int thePipe[2];

   if ((ret = pipe(thePipe)) == -1) {
      return OO_FAILED;
   }
   pthread_mutex_init(&gCmdChanLock, NULL);

   gH323ep.cmdSock = dup(thePipe[0]);
   close(thePipe[0]);
   gCmdChan = dup(thePipe[1]);
   close(thePipe[1]);
#endif

   return OO_OK;
}


int ooCloseCmdConnection()
{
#ifdef _WIN32
   ooSocketClose(gH323ep.cmdSock);
#else
   close (gH323ep.cmdSock);
   close (gCmdChan);
   gCmdChan = 0;
   pthread_mutex_destroy(&gCmdChanLock);
#endif
   gH323ep.cmdSock = 0;

   return OO_OK;
}

#ifdef _WIN32
int ooAcceptCmdConnection()
{
   int ret = ooSocketAccept
      (gH323ep.cmdListener, &gH323ep.cmdSock, NULL, NULL);

   if(ret != ASN_OK) {
      OOTRACEERR1 ("Error:Accepting CMD connection\n");
      return OO_FAILED;
   }
   OOTRACEINFO1("Cmd connection accepted\n");
   return OO_OK;
}
#endif

int ooWriteStackCommand (OOStackCommand *cmd)
{
   int stat;
   OOTRACEDBGA5 ("write stack cmd: t=%d, p1=%x, p2=%x, p3=%x\n",
                 cmd->type, cmd->param1, cmd->param2, cmd->param3);
#ifdef _WIN32
   stat = ooSocketSend
      (gCmdChan, (const ASN1OCTET*)cmd, sizeof(OOStackCommand));

   if (0 != stat) {
      OOTRACEERR2 ("ERROR: write stack command %d\n", stat);
      return OO_FAILED;
   }
#else
   /* lock and write to pipe */
   pthread_mutex_lock (&gCmdChanLock);

   stat = write (gCmdChan, (char*)cmd, sizeof(OOStackCommand));

   pthread_mutex_unlock (&gCmdChanLock);

   if (stat < 0) {
      OOTRACEERR2 ("ERROR: write stack command %d\n", stat);
      return OO_FAILED;
   }
#endif
   return OO_OK;
}

int ooProcessStackCommand (OOStackCommand* pcmd)
{
   OOH323CallData *pCall = NULL;

   if (pcmd->type == OO_CMD_NOOP) return 0;

   if (gH323ep.gkClient && gH323ep.gkClient->state != GkClientRegistered
       && pcmd->type != OO_CMD_STOPMONITOR)
   {
      OOTRACEINFO1
         ("Ignoring stack command as Gk Client is not registered yet\n");
      return 0;
   }

   switch (pcmd->type) {
   case OO_CMD_MAKECALL:
      OOTRACEINFO2("Processing MakeCall command %s\n", (char*)pcmd->param2);

      ooH323MakeCall ((char*)pcmd->param1, (char*)pcmd->param2,
                      (ooCallOptions*)pcmd->param3);

      break;

   case OO_CMD_MANUALRINGBACK:
      if(OO_TESTFLAG(gH323ep.flags, OO_M_MANUALRINGBACK))
      {
         pCall = ooFindCallByToken((char*)pcmd->param1);
         if(!pCall) {
            OOTRACEINFO2("Call \"%s\" does not exist\n", (char*)pcmd->param1);
            OOTRACEINFO1("Call may be cleared/closed\n");
         }
         else {
            ooSendAlerting (ooFindCallByToken((char*)pcmd->param1));
            if(OO_TESTFLAG(gH323ep.flags, OO_M_AUTOANSWER)) {
               ooSendConnect(ooFindCallByToken((char*)pcmd->param1));
            }
         }
      }
      break;

   case OO_CMD_ANSCALL:
      pCall = ooFindCallByToken((char*)pcmd->param1);
      if(!pCall) {
         OOTRACEINFO2("Call \"%s\" does not exist\n", (char*)pcmd->param1);
         OOTRACEINFO1("Call might be cleared/closed\n");
      }
      else {
         OOTRACEINFO2("Processing Answer Call command for %s\n",
                      (char*)pcmd->param1);
         ooSendConnect(pCall);
      }
      break;

   case OO_CMD_FWDCALL:
      OOTRACEINFO3("Forwarding call %s to %s\n",
                   (char*)pcmd->param1, (char*)pcmd->param2);

      ooH323ForwardCall((char*)pcmd->param1, (char*)pcmd->param2);

      break;

   case OO_CMD_HANGCALL:
      OOTRACEINFO2("Processing hang call command %s\n", (char*)pcmd->param1);

      ooH323HangCall((char*)pcmd->param1, *(OOCallClearReason*)pcmd->param2);

      break;

   case OO_CMD_SENDDIGIT:
      pCall = ooFindCallByToken((char*)pcmd->param1);
      if (!pCall) {
         OOTRACEERR2("ERROR:Invalid calltoken %s\n", (char*)pcmd->param1);
         break;
      }
      if (pCall->jointDtmfMode & OO_CAP_DTMF_H245_alphanumeric) {
         ooSendH245UserInputIndication_alphanumeric
            (pCall, (const char*)pcmd->param2);
      }
      else if (pCall->jointDtmfMode & OO_CAP_DTMF_H245_signal) {
         ooSendH245UserInputIndication_signal
            (pCall, (const char*)pcmd->param2);
      }
      else {
         ooQ931SendDTMFAsKeyPadIE(pCall, (const char*)pcmd->param2);
      }

      break;

   case OO_CMD_STOPMONITOR:
      OOTRACEINFO1 ("Processing StopMonitor command\n");
      ooStopMonitorCalls();
      break;

   default: OOTRACEERR2 ("ERROR: unknown command %d\n", pcmd->type);
   }

   // These items are freed elsewhere if an error occurs
   // if (0 != pcmd->param1) free (pcmd->param1);
   // if (0 != pcmd->param2) free (pcmd->param2);
   // if (0 != pcmd->param3) free (pcmd->param3);

   return OO_OK;
}

