/*
 * Copyright (C) 2004-2005 by Objective Systems, Inc.
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

#include "ooports.h"
#include "oochannels.h"
#include "oo.h"
#include "ooq931.h"
#include "ooh245.h"
#include "ooh323.h"
#include "ooCalls.h"
#include "printHandler.h"

#include "stdio.h"



int ooCreateH245Listener(ooCallData *call)
{
   int ret=0;
   OOSOCKET channelSocket=0;
   OOTRACEINFO1("Creating H245 listener\n");
   if((ret=ooSocketCreate (&channelSocket))!=ASN_OK)
   {
      OOTRACEERR3("ERROR: Failed to create socket for H245 listener "
                  "(%s, %s)\n", call->callType, call->callToken);
      return OO_FAILED;
   }
   ret = ooBindPort(&gH323ep, OOTCP, channelSocket);
   if(ret == OO_FAILED)
   {
      OOTRACEERR3("Error:Unable to bind to a TCP port - H245 listener creation"
                  " (%s, %s)\n", call->callType, call->callToken);
      return OO_FAILED;
   }
   call->h245listenport = (int*) ASN1MALLOC(call->pctxt, sizeof(int));
   *(call->h245listenport) = ret;
   call->h245listener = (OOSOCKET*)ASN1MALLOC(call->pctxt, sizeof(OOSOCKET));
   *(call->h245listener) = channelSocket;
   ooSocketListen(*(call->h245listener), 5);
   OOTRACEINFO4("H245 listener creation - successful(port %d) (%s, %s)\n",
                *(call->h245listenport),call->callType, call->callToken);
   return OO_OK;
}

int ooCreateH245Connection(ooCallData *call)
{
   int ret=0;
   OOSOCKET channelSocket=0;
   OOTRACEINFO1("Creating H245 Connection\n");
   if((ret=ooSocketCreate (&channelSocket))!=ASN_OK)
   {
      OOTRACEERR3("ERROR:Failed to create socket for H245 connection "
                  "(%s, %s)\n", call->callType, call->callToken);
      return OO_FAILED;
   }
   else
   {
      /*
         bind socket to a port before connecting. Thus avoiding
         implicit bind done by a connect call.
      */
      ret = ooBindPort(&gH323ep, OOTCP, channelSocket);
      if(ret == OO_FAILED)
      {
         OOTRACEERR3("Error:Unable to bind to a TCP port - h245 connection "
                     "(%s, %s)\n", call->callType, call->callToken);
         return OO_FAILED;
      }
      call->h245ChanPort = (int*) ASN1MALLOC(call->pctxt, sizeof(int));
      *(call->h245ChanPort) = ret;
      OOTRACEDBGC4("Local H.245 port is %d (%s, %s)\n", *(call->h245ChanPort),
                   call->callType, call->callToken);
      OOTRACEINFO5("Trying to connect to remote endpoint to setup H245 "
                   "connection %s:%d(%s, %s)\n", call->remoteIP,
                    call->remoteH245Port, call->callType, call->callToken);
             
      if((ret=ooSocketConnect(channelSocket, call->remoteIP,
                              call->remoteH245Port))==ASN_OK)
      {
         call->h245Channel = (OOSOCKET*) ASN1MALLOC(call->pctxt,
                                                        sizeof(OOSOCKET));
         *(call->h245Channel) = channelSocket;
         call->h245SessionState = OO_H245SESSION_ACTIVE;
         OOTRACEINFO3("H245 connection creation succesful (%s, %s)\n",
                      call->callType, call->callToken);
         /* Start terminal capability exchange and master slave determination */
         ret = ooSendTermCapMsg(call);
         if(ret != OO_OK)
         {
            OOTRACEERR3("ERROR:Sending Terminal capability message (%s, %s)\n",
                         call->callType, call->callToken);
            return ret;
         }
         ret = ooSendMasterSlaveDetermination(call);
         if(ret != OO_OK)
         {
            OOTRACEERR3("ERROR:Sending Master-slave determination message (%s, %s)\n",
                         call->callType, call->callToken);
            return ret;
         }
        
      }
      else
      {
         OOTRACEINFO3("ERROR:Failed to connect to remote destination for H245 "
                     "connection (%s, %s)\n", call->callType, call->callToken);
         return OO_FAILED;
      }
   }
  
   return OO_OK;
}

int ooSendH245Msg(ooCallData *call, H245Message *msg)
{
   if(!call)
      return OO_FAILED;

   if(msg)
   {
      dListAppend(call->pctxt, &call->outH245Queue,
                  msg);
      call->sendH245++;
   }
   OOTRACEDBGC4("Queued H245 messages %d. (%s, %s)\n", call->sendH245,
               call->callType, call->callToken);  
   return OO_OK;
}


int ooSendH225Msg(ooCallData *call, Q931Message *msg)
{
   if(!call)
      return OO_FAILED;

   if(msg)
   {
      dListAppend(call->pctxt, &call->outH225Queue,
                  msg);
      call->sendH225++;
   }
   return OO_OK;
}

int ooCreateH225Connection(ooCallData *call)
{
   int ret=0;
   OOSOCKET channelSocket=0;

   if((ret=ooSocketCreate (&channelSocket))!=ASN_OK)
   {
      OOTRACEERR3("Failed to create socket for transmit H2250 channel (%s, %s)"
                  "\n", call->callType, call->callToken);
      return OO_FAILED;
   }
   else
   {
      /*
         bind socket to a port before connecting. Thus avoiding
         implicit bind done by a connect call. Avoided on windows as
         windows sockets have problem in reusing the addresses even after
         setting SO_REUSEADDR, hence in windows we just allow os to bind
         to any random port.
      */
#ifndef _WIN32
      ret = ooBindPort(&gH323ep, OOTCP,channelSocket);
#else
      ret = ooBindOSAllocatedPort(channelSocket);
#endif
     
      if(ret == OO_FAILED)
      {
         OOTRACEERR3("Error:Unable to bind to a TCP port (%s, %s)\n",
         call->callType, call->callToken);
         return OO_FAILED;
      }
      call->h225ChanPort = (int*)ASN1MALLOC(call->pctxt, sizeof(int));

      *(call->h225ChanPort) = ret;

      OOTRACEINFO5("Trying to connect to remote endpoint(%s:%d) to setup H2250"                                    "channel (%s, %s)\n", call->remoteIP, call->remotePort,
                    call->callType, call->callToken);
      if((ret=ooSocketConnect(channelSocket, call->remoteIP,
                              call->remotePort))==ASN_OK)
      {
         call->h225Channel = (OOSOCKET*) ASN1MALLOC(call->pctxt,
                                                            sizeof(OOSOCKET));
         *(call->h225Channel) = channelSocket;
        
         OOTRACEINFO3("H2250 transmiter channel creation - succesful "
                      "(%s, %s)\n", call->callType, call->callToken);
         return OO_OK;
      }
      else
      {
         OOTRACEERR3("ERROR:Failed to connect to remote destination for transmit "
                     "H2250 channel\n", call->callType, call->callToken);
         return OO_FAILED;
      }
      return OO_FAILED;
   }
}

int ooCloseH225Connection(ooCallData *call)
{
   if(call->h225Channel)
   {
      ooSocketClose(*(call->h225Channel));
      ASN1MEMFREEPTR(call->pctxt, call->h225Channel);
      call->h225Channel = NULL;
      if(call->outH225Queue.count > 0)
      {
         dListFreeAll(call->pctxt, &(call->outH225Queue));
         call->sendH225 = 0;
      }
   }
   return OO_OK;
}

int ooCreateH323Listener()
{
   int ret=0;
   OOSOCKET channelSocket=0;
   OOIPADDR ipaddrs;
   char localip[20];
   ret = ooGetLocalIPAddress(localip);
   if(ret != ASN_OK)
   {
      OOTRACEERR1("ERROR:Failed to retrieve local ip address for creating "
                  "listener\n");
      return OO_FAILED;
   }

    /* Create socket */
   if((ret=ooSocketCreate (&channelSocket))!=ASN_OK)
   {
      OOTRACEERR1("Failed to create socket for H323 Listener\n");
      return OO_FAILED;
   }
   ret= ooSocketStrToAddr (localip, &ipaddrs);
   if((ret=ooSocketBind (channelSocket, ipaddrs,
                         gH323ep.listenPort))==ASN_OK)
   {
      gH323ep.listener = (OOSOCKET*)ASN1MALLOC(&gH323ep.ctxt,sizeof(OOSOCKET));
      *(gH323ep.listener) = channelSocket;
         
      ooSocketListen(channelSocket, 5); /*listen on socket*/
      OOTRACEINFO1("H323 listener creation - successful\n");
      return OO_OK;
   }
   else
   {
      OOTRACEERR1("ERROR:Failed to create H323 listener\n");
      return OO_FAILED;
   }
}



int ooAcceptH225Connection()   
{
   ooCallData * call;
   int ret;
   char callToken[20];
   OOSOCKET h225Channel=0;
   ret = ooSocketAccept (*(gH323ep.listener), &h225Channel,
                         NULL, NULL);
   if(ret != ASN_OK)
   {
      OOTRACEERR1("Error:Accepting h225 connection\n");
      return OO_FAILED;
   }
#ifdef _WIN32
   EnterCriticalSection(&gCmdMutex);
#else
   pthread_mutex_lock(&gCmdMutex);
#endif
   sprintf(callToken, "ooh323c_%d", gCurCallToken++);
   /* Reset calltoken */
   if(gCurCallToken >= gCallTokenMax)
      gCurCallToken = gCallTokenBase;
#ifdef _WIN32
   LeaveCriticalSection(&gCmdMutex);
#else
   pthread_mutex_unlock(&gCmdMutex);
#endif

   call = ooCreateCall("incoming", callToken);
   if(!call)
   {
      OOTRACEERR1("ERROR:Failed to create an incoming call\n");
      return OO_FAILED;
   }
   call->h225Channel = (OOSOCKET*)ASN1MALLOC(call->pctxt, sizeof(OOSOCKET));
   *(call->h225Channel)=h225Channel;
 
   return OO_OK;
}

int ooAcceptH245Connection(ooCallData *call)
{
   int ret;
   OOSOCKET h245Channel=0;
   ret = ooSocketAccept (*(call->h245listener), &h245Channel,
                         NULL, NULL);
   if(ret != ASN_OK)
   {
      OOTRACEERR1("Error:Accepting h245 connection\n");
      return OO_FAILED;
   }
   call->h245Channel = (OOSOCKET*)ASN1MALLOC(call->pctxt, sizeof(OOSOCKET));
   *(call->h245Channel) = h245Channel;
   call->h245SessionState = OO_H245SESSION_ACTIVE;
   OOTRACEINFO3("H.245 connection established (%s, %s)\n",
                call->callType, call->callToken);
   /* Start terminal capability exchange and master slave determination */
   ret = ooSendTermCapMsg(call);
   if(ret != OO_OK)
   {
      OOTRACEERR3("ERROR:Sending Terminal capability message (%s, %s)\n",
                   call->callType, call->callToken);
      return ret;
   }
   ret = ooSendMasterSlaveDetermination(call);
   if(ret != OO_OK)
   {
      OOTRACEERR3("ERROR:Sending Master-slave determination message (%s, %s)\n",
                   call->callType, call->callToken);
      return ret;
   }  
   return OO_OK;
}

int ooMonitorChannels()
{
   int ret=0, nfds=0;
   struct timeval timeout;
   fd_set readfds, writefds;
   ooCallData *call, *prev=NULL;
   DListNode *curNode;
   ooCommand *cmd;
   int i=0;  
   gMonitor = 1;
   while(1)
   {
      FD_ZERO(&readfds);
      FD_ZERO(&writefds);
      nfds = 0;
     
      if(gH323ep.listener)
      {
         FD_SET(*(gH323ep.listener), &readfds);
         if(nfds < (int)*(gH323ep.listener))
            nfds = *((int*)gH323ep.listener);
      }
     
      if(gH323ep.callList)
      {
         call = gH323ep.callList;
         while(call)
         {
            if(call->h225Channel)
            {
               FD_SET(*(call->h225Channel), &readfds);
               if(call->sendH225>0 ||
                  (call->isTunnelingActive && call->sendH245>0))
                  FD_SET(*(call->h225Channel), &writefds);
               if(nfds < (int)*(call->h225Channel))
                  nfds = *(call->h225Channel);
            }
           
            if(call->h245Channel)
            {
               FD_SET(*(call->h245Channel), &readfds);
               if(call->sendH245>0)
                  FD_SET(*(call->h245Channel), &writefds);
               if(nfds < (int)*(call->h245Channel))
                  nfds = *(call->h245Channel);
            }
            else if(call->h245listener)
            {
               OOTRACEINFO3("H.245 Listerner socket being monitored "
                            "(%s, %s)\n", call->callType, call->callToken);
                FD_SET(*(call->h245listener), &readfds);
                if(nfds < (int)*(call->h245listener))
                  nfds = *(call->h245listener);
             }
             call = call->next;
           
         }/* while(call) */
      }/*if(gH323ep.callList) */
               
      timeout.tv_sec = 3;
      timeout.tv_usec = 0;
      if(!gMonitor)
      {
         OOTRACEINFO1("Ending Monitor thread\n");
         break;
      }

      if(nfds != 0) nfds = nfds+1;
     
      if(nfds == 0)
#ifdef _WIN32
         Sleep(10);
#else
      {
         timeout.tv_sec = 0;
         timeout.tv_usec = 10000;
         ooSocketSelect(1, 0, 0, 0, &timeout);
      }
#endif
      else
         ret = ooSocketSelect(nfds, &readfds, &writefds,
                           NULL, &timeout);
     
      if(ret == -1)
      {
        
         OOTRACEERR1("Error in select ...exiting\n");
         exit(-1);
      }
#ifdef _WIN32
      EnterCriticalSection(&gCmdMutex);
#else
      pthread_mutex_lock(&gCmdMutex);
#endif
         
      if(gCmdList.count >0)
      {
         for(i=0; i< (int)gCmdList.count; i++)
         {
            curNode = dListFindByIndex (&gCmdList, i) ;
            cmd = (ooCommand*) curNode->data;
            switch(cmd->type)
            {
            case OO_CMD_MAKECALL:
               OOTRACEINFO2("Processing MakeCall command %s\n",
                             (char*)cmd->param3);
               ooH323MakeCall((char*)cmd->param1, *(int*)cmd->param2,
                             (char*)cmd->param3);
               break;
            case OO_CMD_ANSCALL:
               OOTRACEINFO2("Processing Answer Call command for %s\n",
                            (char*)cmd->param1);
               ooSendConnect(ooFindCallByToken((char*)cmd->param1));
               break;
            case OO_CMD_REJECTCALL:
               OOTRACEINFO2("Rejecting call %s\n", (char*)cmd->param1);
               ooEndCall(ooFindCallByToken((char*)cmd->param1));
               break;
            case OO_CMD_HANGCALL:
               OOTRACEINFO2("Processing Hang call command %s\n",
                             (char*)cmd->param1);
               ooH323HangCall((char*)cmd->param1);
               break;
            case OO_CMD_STOPMONITOR:
               OOTRACEINFO1("Processing StopMonitor command\n");
               ooStopMonitorCalls();
               break;
            default: printf("Unhandled command\n");
            }
           dListRemove (&gCmdList, curNode);
           ASN1MEMFREEPTR(&gCtxt, curNode);
           ASN1MEMFREEPTR(&gCtxt, cmd);
         }
      }
#ifdef _WIN32
   LeaveCriticalSection(&gCmdMutex);
#else
   pthread_mutex_unlock(&gCmdMutex);
#endif
      if(gH323ep.listener)
      {
         if(FD_ISSET(*(gH323ep.listener), &readfds))
         {
            OOTRACEDBGA1("New connection at H225 receiver\n");
            ooAcceptH225Connection();
         }     
      }
     

      if(gH323ep.callList)
      {
         call = gH323ep.callList;
         while(call)
         {
            if(call->h225Channel)
            {
               if(FD_ISSET(*(call->h225Channel), &readfds))
               {
                  ret = ooH2250Receive(call);
                  if(ret != OO_OK)
                  {
                     OOTRACEERR3("ERROR:Failed ooH2250Receive - Clearing call "
                                 "(%s, %s)\n", call->callType, call->callToken);
                     if(call->callState < OO_CALL_CLEAR)
                     {
                        call->callEndReason = OO_HOST_CLEARED;
                        call->callState = OO_CALL_CLEAR;
                      }
                  }
               }
            }

            if(call->h225Channel)
            {
               if(FD_ISSET(*(call->h225Channel), &writefds))
               {
                  if(call->sendH225>0)
                  {
                     OOTRACEDBGC3("Sending H225 message (%s, %s)\n",
                                 call->callType, call->callToken);
                     ooSendMsg(call, OOQ931MSG);
                  }
                  if(call->sendH245>0 && call->isTunnelingActive)
                  {
                     OOTRACEDBGC3("Tunneling H245 message (%s, %s)\n",
                                  call->callType, call->callToken);
                     ooSendMsg(call, OOH245MSG);
                  }
               }                               
            }

            if(call->h245Channel)
            {
               if(FD_ISSET(*(call->h245Channel), &readfds))
               {                          
                  ooH245Receive(call);
               }
            }

            if(call->h245Channel)
            {
               if(FD_ISSET(*(call->h245Channel), &writefds))
               {                          
                  ooSendMsg(call, OOH245MSG);
               }
            }
            else if(call->h245listener)
            {
               if(FD_ISSET(*(call->h245listener), &readfds))
               {
                  OOTRACEDBGC3("Incoming H.245 connection (%s, %s)\n",
                               call->callType, call->callToken);
                  ooAcceptH245Connection(call);
               }                          
            }
            prev = call;
            call = call->next;
            if(prev->callState >= OO_CALL_CLEAR)
               ooEndCall(prev);
         }/* while(call) */
      }/* if(gH323ep.callList) */
   }/* while(1)*/
   return OO_OK;
}

int ooH2250Receive(ooCallData *call)
{
   int  recvLen=0, total=0, ret=0;
   ASN1OCTET message[MAXMSGLEN], message1[MAXMSGLEN];
   int size =MAXMSGLEN, len;
   Q931Message *pmsg;
   OOCTXT *pctxt;
   struct timeval timeout;
   fd_set readfds;
   pctxt = (OOCTXT*)newContext();

   if(!pctxt)
   {
      OOTRACEERR3("ERROR: Failed to create OOCTXT for incoming H.2250 message"
                  " (%s, %s)\n", call->callType, call->callToken);
      return OO_FAILED;
   }
  
   pmsg = (Q931Message*)ASN1MALLOC(pctxt, sizeof(Q931Message));
   if(!pmsg)
   {
      OOTRACEERR3("ERROR:Failed to allocate memory for incoming H.2250 message"
                  " (%s, %s)\n", call->callType, call->callToken);
      return OO_FAILED;
   }
   pmsg->pctxt = pctxt;
  
   /* First read just TPKT header which is four bytes */
   recvLen = ooSocketRecv (*(call->h225Channel), message, 4);
   if(recvLen == 0)
   {
      OOTRACEWARN3("Warn:RemoteEndpoint closed connection (%s, %s)\n",
                   call->callType, call->callToken);
      if(call->callState < OO_CALL_CLEARED)
      {
         call->callEndReason = OO_REMOTE_CLOSED_CONNECTION;
         call->callState = OO_CALL_CLEARED;
        
      }
      return OO_OK;
   }
   OOTRACEDBGC3("Receiving H.2250 message (%s, %s)\n",
                call->callType, call->callToken);  
   /* Since we are working with TCP, need to determine the
      message boundary. Has to be done at channel level, as channels
      know the message formats and can determine boundaries
   */
   if(recvLen != 4)
   {
      OOTRACEERR4("Error: Reading TPKT header for H225 message "
                  "recvLen= %d (%s, %s)\n", recvLen, call->callType,
                  call->callToken);
      ooFreeQ931Message(pmsg);
      if(call->callState < OO_CALL_CLEAR)
      {
         call->callEndReason = OO_HOST_CLEARED;
         call->callState = OO_CALL_CLEAR;
      }
      return OO_FAILED;
   }
  
   len = message[2];
   len = len<<8;
   len = len + message[3];
   /* Remaining message length is length - tpkt length */
   len = len - 4;

   /* Now read actual Q931 message body. We should make sure that we
      receive complete message as indicated by len. If we don't then there
      is something wrong. The loop below receives message, then checks whether
      complete message is received. If not received, then uses select to peek
      for remaining bytes of the message. If message is not received in 3
      seconds, then we have a problem. Report an error and exit.
   */
   while(total < len)
   {
      recvLen = ooSocketRecv (*(call->h225Channel), message1, len-total);
      memcpy(message+total, message1, recvLen);
      total = total + recvLen;

      if(total == len) break; /* Complete message is received */
     
      FD_ZERO(&readfds);
      FD_SET(*(call->h225Channel), &readfds);
      timeout.tv_sec = 3;
      timeout.tv_usec = 0;
      ret = ooSocketSelect(*(call->h225Channel)+1, &readfds, NULL,
                           NULL, &timeout);
      if(ret == -1)
      {
         OOTRACEERR3("Error in select while receiving H.2250 message - "
                     "clearing call (%s, %s)\n", call->callType,
                     call->callToken);
         ooFreeQ931Message(pmsg);
         if(call->callState < OO_CALL_CLEAR)
         {
            call->callEndReason = OO_HOST_CLEARED;
            call->callState = OO_CALL_CLEAR;
         }
         return OO_FAILED;
      }
      /* If remaining part of the message is not received in 3 seconds
         exit */
      if(!FD_ISSET(*(call->h225Channel), &readfds))
      {
         OOTRACEERR3("Error: Incomplete H.2250 message received - clearing "
                     "call (%s, %s)\n", call->callType, call->callToken);
         ooFreeQ931Message(pmsg);
         if(call->callState < OO_CALL_CLEAR)
         {
            call->callEndReason = OO_HOST_CLEARED;
            call->callState = OO_CALL_CLEAR;
         }
         return OO_FAILED;
      }
   }
  
   OOTRACEDBGC3("Received H.2250 message: (%s, %s)\n",
                call->callType, call->callToken);
   initializePrintHandler(&printHandler, "Received H.323 Message");

   /* Add event handler to list */
   rtAddEventHandler (pctxt, &printHandler);
   ooQ931Decode (pmsg, len, message);
   ooHandleH2250Message(call, pmsg);
   return OO_OK;
}



int ooH245Receive(ooCallData *call)
{
   int  recvLen, ret, len, total=0;
   ASN1OCTET message[MAXMSGLEN], message1[MAXMSGLEN];
   int  size =MAXMSGLEN;
   ASN1BOOL aligned = TRUE, trace = FALSE;
   H245Message *pmsg;
   OOCTXT *pctxt;
   struct timeval timeout;
   fd_set readfds;
  
   pctxt = (OOCTXT*)newContext();
   if(pctxt == NULL)
   {
      OOTRACEERR3("ERROR: Failed to allocate ASN1 context for"
                  " incoming h245 message creation (%s, %s)\n",
                  call->callType, call->callToken);
      return OO_FAILED;
   }
  
   pmsg = (H245Message*)ASN1MALLOC(pctxt, sizeof(H245Message));
   pmsg->pctxt = pctxt;
   /* First read just TPKT header which is four bytes */
   recvLen = ooSocketRecv (*(call->h245Channel), message, 4);
   /* Since we are working with TCP, need to determine the
      message boundary. Has to be done at channel level, as channels
      know the message formats and can determine boundaries
   */
   if(recvLen==0)
   {
     
      OOTRACEINFO3("Closing H.245 channels as remote end point closed H.245"
                   " connection (%s, %s)\n", call->callType, call->callToken);
      ooCloseH245Connection(call);
      ooFreeH245Message(pmsg);
      if(call->callState < OO_CALL_CLEAR_ENDSESSION)
      {
         call->callEndReason = OO_REMOTE_CLOSED_H245_CONNECTION;
         call->callState = OO_CALL_CLEAR_ENDSESSION;
      }
      return OO_FAILED;
   }
   OOTRACEDBGC1("Receiving H245 message\n");
   if(recvLen != 4)
   {
      OOTRACEERR3("Error: Reading TPKT header for H245 message (%s, %s)\n",
                  call->callType, call->callToken);
      ooFreeH245Message(pmsg);
      if(call->callState < OO_CALL_CLEAR)
      {
         call->callEndReason = OO_HOST_CLEARED;
         call->callState = OO_CALL_CLEAR;
      }
      return OO_FAILED;
   }

   len = message[2];
   len = len<<8;
   len = (len | message[3]);
   /* Remaining message length is length - tpkt length */
   len = len - 4;
   /* Now read actual H245 message body. We should make sure that we
      receive complete message as indicated by len. If we don't then there
      is something wrong. The loop below receives message, then checks whether
      complete message is received. If not received, then uses select to peek
      for remaining bytes of the message. If message is not received in 3
      seconds, then we have a problem. Report an error and exit.
   */
   while(total < len)
   {
      recvLen = ooSocketRecv (*(call->h245Channel), message1, len-total);
      memcpy(message+total, message1, recvLen);
      total = total + recvLen;
      if(total == len) break; /* Complete message is received */
      FD_ZERO(&readfds);
      FD_SET(*(call->h245Channel), &readfds);
      timeout.tv_sec = 3;
      timeout.tv_usec = 0;
      ret = ooSocketSelect(*(call->h245Channel)+1, &readfds, NULL,
                           NULL, &timeout);
      if(ret == -1)
      {
         OOTRACEERR3("Error in select...H245 Receive-Clearing call (%s, %s)\n",
                     call->callType, call->callToken);
         ooFreeH245Message(pmsg);
         if(call->callState < OO_CALL_CLEAR)
         {
            call->callEndReason = OO_HOST_CLEARED;
            call->callState = OO_CALL_CLEAR;
         }
         return OO_FAILED;
      }
      /* If remaining part of the message is not received in 3 seconds
         exit */
      if(!FD_ISSET(*(call->h245Channel), &readfds))
      {
         OOTRACEERR3("Error: Incomplete h245 message received (%s, %s)\n",
                     call->callType, call->callToken);
         ooFreeH245Message(pmsg);
         if(call->callState < OO_CALL_CLEAR)
         {
            call->callEndReason = OO_HOST_CLEARED;
            call->callState = OO_CALL_CLEAR;
         }
         return OO_FAILED;
      }
   }

   OOTRACEDBGC3("Complete H245 message received (%s, %s)\n",
                 call->callType, call->callToken);
   setPERBuffer(pctxt, message, recvLen, aligned);
   initializePrintHandler(&printHandler, "Received H.245 Message");

   /* Add event handler to list */
   rtAddEventHandler (pctxt, &printHandler);
   ret = asn1PD_H245MultimediaSystemControlMessage(pctxt, &(pmsg->h245Msg));
   if(ret != ASN_OK)
   {
      OOTRACEERR3("Error decoding H245 message (%s, %s)\n",
                  call->callType, call->callToken);
      ooFreeH245Message(pmsg);
      return OO_FAILED;
   }
   
  ooHandleH245Message(call, pmsg);
  return OO_OK;

}

/* Generic Send Message functionality. Based on type of message to be sent,
   it calls the corresponding function to retrieve the message buffer and
   then transmits on the associated channel
*/
int ooSendMsg(ooCallData *call, int type)
{
   ASN1OCTET msgbuf[MAXMSGLEN];
   int len=0, ret=0, msgType=0;
   int i =0;
   memset(msgbuf, 0, sizeof(msgbuf));
   if(type == OOQ931MSG)
   {
     
      if(ooGetOutgoingQ931Msgbuf(call, msgbuf, &len, &msgType)!=OO_OK)
      {
         OOTRACEERR3("ERROR:Could not retrieve message buffer for outgoing "
                     "q931 msg (%s, %s)\n", call->callType, call->callToken);
         call->sendH225--;
         if(call->callState < OO_CALL_CLEAR)
         {
            call->callEndReason = OO_HOST_CLEARED;
            call->callState = OO_CALL_CLEAR;
         }
         return OO_FAILED;
      }
      /* Send message out via TCP */
     
      ret = ooSocketSend(*(call->h225Channel), msgbuf, len);
      if(ret == ASN_OK)
      {
         OOTRACEDBGA3("H2250Q931 Message sent successfully (%s, %s)\n",
                      call->callType, call->callToken);
         call->sendH225--; /* decrement send count */
         ooOnSendMsg(call, msgType);
         return OO_OK;
      }
      else{
         OOTRACEERR3("H2250Q931 Message send failed (%s, %s)\n",
                     call->callType, call->callToken);
         call->sendH225--;
         if(call->callState < OO_CALL_CLEAR)
         {
            call->callEndReason = OO_HOST_CLEARED;
            call->callState = OO_CALL_CLEAR;
         }
         return OO_FAILED;
      }
   }/* end of type==OOQ931MSG */
   if(type == OOH245MSG)
   {
      if(ooGetOutgoingH245Msgbuf(call, msgbuf, &len, &msgType)!=OO_OK)
      {
         OOTRACEERR3("ERROR:Could not retrieve message buffer for outgoing "
                     "h245 message (%s, %s)\n", call->callType,
                     call->callToken);
         call->sendH245--;
         if(call->callState < OO_CALL_CLEAR)
         {
            call->callEndReason = OO_HOST_CLEARED;
            call->callState = OO_CALL_CLEAR;
         }
         return OO_FAILED;
      }
      call->sendH245--;
      /* Send message out */
      if(!call->h245Channel && !call->isTunnelingActive)
      {
         OOTRACEWARN3("Neither H.245 channel nor tunneling active "
                     "(%s, %s)\n", call->callType, call->callToken);
         /*ooCloseH245Session(call);*/
         if(call->callState < OO_CALL_CLEAR)
         {
            call->callEndReason = OO_HOST_CLEARED;
            call->callState = OO_CALL_CLEAR;
         }
         return OO_OK;
      }
     
      if(call->h245Channel)
      {
         ret = ooSocketSend(*(call->h245Channel), msgbuf, len);
         if(ret == ASN_OK)
                 {
            OOTRACEDBGA3("H245 Message sent successfully (%s, %s)\n",
                          call->callType, call->callToken);
            ooOnSendMsg(call, msgType);
            return OO_OK;
         }
         else{
            OOTRACEERR3("ERROR:H245 Message send failed (%s, %s)\n",
                        call->callType, call->callToken);
            if(call->callState < OO_CALL_CLEAR)
            {
               call->callEndReason = OO_HOST_CLEARED;
               call->callState = OO_CALL_CLEAR;
            }
            return OO_FAILED;
         }
      }else if(call->isTunnelingActive){
         /* Tunnel H.245 message */
         ret = ooSendAsTunneledMessage(call, msgbuf, len, msgType);
         if(ret != OO_OK)
         {
            OOTRACEERR3("ERROR:Failed to tunnel H.245 message (%s, %s)\n",
                         call->callType, call->callToken);
            if(call->callState < OO_CALL_CLEAR)
            {
               call->callEndReason = OO_HOST_CLEARED;
               call->callState = OO_CALL_CLEAR;
            }
            return OO_FAILED;
         }
                 return OO_OK;
      }
   }
   /* Need to add support for other messages such as T38 etc */
   OOTRACEWARN3("ERROR:Unknown message type - message not Sent (%s, %s)\n",
                call->callType, call->callToken);
   return OO_FAILED;
}              


int ooCloseH245Connection(ooCallData *call)
{
   OOTRACEINFO3("Closing H.245 connection (%s, %s)\n", call->callType,
                call->callToken);

   if(call->h245Channel)
   {
      ooSocketClose(*(call->h245Channel));
      ASN1MEMFREEPTR(call->pctxt, call->h245Channel);
      call->h245Channel = NULL;
   }
   if(call->h245listener)
   {
      ooSocketClose(*(call->h245listener));
      ASN1MEMFREEPTR(call->pctxt, call->h245listener);
      call->h245listener = NULL;
   }
  
   dListFreeAll(call->pctxt, &(call->outH245Queue));
   call->sendH245 = 0;
   call->h245SessionState = OO_H245SESSION_INACTIVE;
   return OO_OK;
}

int ooOnSendMsg(ooCallData *call, int msgType)
{
   switch(msgType)
   {
   case OOSetup:
      OOTRACEINFO3("Sent Message - Setup (%s, %s)\n", call->callType,
                    call->callToken);
      gH323ep.onOutgoingCall(call);
      break;
   case OOCallProceeding:
      OOTRACEINFO3("Sent Message - CallProceeding (%s, %s)\n", call->callType,
                    call->callToken);
      break;
   case OOAlert:
      OOTRACEINFO3("Sent Message - Alerting (%s, %s)\n", call->callType,
                    call->callToken);
      break;
   case OOConnect:
      OOTRACEINFO3("Sent Message - Connect (%s, %s)\n", call->callType,
                    call->callToken);
      break;
   case OOReleaseComplete:
      OOTRACEINFO3("Sent Message - ReleaseComplete (%s, %s)\n", call->callType,
                    call->callToken);
      break;
   case OOFacility:
      OOTRACEINFO3("Sent Message - Facility (%s, %s)\n", call->callType,
                    call->callToken);
      break;
   case OOMasterSlaveDetermination:
      OOTRACEINFO3("Sent Message - MasterSlaveDetermination (%s, %s)\n",
                    call->callType, call->callToken);
      break;
   case OOMasterSlaveAck:
      OOTRACEINFO3("Sent Message - MasterSlaveDeterminationAck (%s, %s)\n",
                    call->callType, call->callToken);
      break;
   case OOMasterSlaveReject:
      OOTRACEINFO3("Sent Message - MasterSlaveDeterminationReject (%s, %s)\n",
                    call->callType, call->callToken);
      break;
   case OOMasterSlaveRelease:
      OOTRACEINFO3("Sent Message - MasterSlaveDeterminationRelease (%s, %s)\n",
                    call->callType, call->callToken);
      break;
   case OOTerminalCapabilitySet:
      OOTRACEINFO3("Sent Message - TerminalCapabilitySet (%s, %s)\n",
                    call->callType, call->callToken);
      break;
   case OOTerminalCapabilitySetAck:
      OOTRACEINFO3("Sent Message - TerminalCapabilitySetAck (%s, %s)\n",
                    call->callType, call->callToken);
      break;
   case OOTerminalCapabilitySetReject:
      OOTRACEINFO3("Sent Message - TerminalCapabilitySetReject (%s, %s)\n",
                    call->callType, call->callToken);
      break;
   case OOOpenLogicalChannel:
      OOTRACEINFO3("Sent Message - OpenLogicalChannel (%s, %s)\n",
                    call->callType, call->callToken);
      break;
   case OOOpenLogicalChannelAck:
      OOTRACEINFO3("Sent Message - OpenLogicalChannelAck (%s, %s)\n",
                    call->callType, call->callToken);
      break;
   case OOOpenLogicalChannelReject:
      OOTRACEINFO3("Sent Message - OpenLogicalChannelReject (%s, %s)\n",
                    call->callType, call->callToken);
      break;
   case OOOpenLogicalChannelRelease:
      OOTRACEINFO3("Sent Message - OpenLogicalChannelRelease (%s, %s)\n",
                    call->callType, call->callToken);
      break;
   case OOEndSessionCommand:
      OOTRACEINFO1("Sent Message - EndSessionCommand\n");
      break;
   case OOCloseLogicalChannel:
      OOTRACEINFO3("Sent Message - CloseLogicalChannel (%s, %s)\n",
                    call->callType, call->callToken);
      break;
   case OOCloseLogicalChannelAck:
      OOTRACEINFO3("Sent Message - CloseLogicalChannelAck (%s, %s)\n",
                    call->callType, call->callToken);
      break;
   case OORequestChannelClose:
      OOTRACEINFO3("Sent Message - RequestChannelClose (%s, %s)\n",
                    call->callType, call->callToken);
      break;
   case OORequestChannelCloseAck:
      OOTRACEINFO3("Sent Message - RequestChannelCloseAck (%s, %s)\n",
                    call->callType, call->callToken);
      break;
  
   default:
     ;
   }
   return OO_OK;
}


int ooStopMonitorCalls()
{
   ooCallData * call;
   OOTRACEINFO1("Doing ooStopMonitorCalls\n");
   if(gH323ep.callList)
   {
      OOTRACEWARN1("Warn:Abruptly ending calls as stack going down\n");
      call = gH323ep.callList;
      while(call)
      {
         OOTRACEWARN3("Clearing call (%s, %s)\n", call->callType,
                       call->callToken);
         call->callEndReason = OO_HOST_CLEARED;
         ooCleanCall(call);
         call = NULL;
         call = gH323ep.callList;
      }
   }
   OOTRACEINFO1("Stopping listener for incoming calls\n");  
   if(gH323ep.listener)
   {
      ooSocketClose(*(gH323ep.listener));
      ASN1MEMFREEPTR(&gH323ep.ctxt, gH323ep.listener);
   }

   gMonitor = 0;
   OOTRACEINFO1("Returning form ooStopMonitorCalls\n");
   return OO_OK;
}
     


