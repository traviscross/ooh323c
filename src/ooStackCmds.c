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

#include "ooStackCmds.h"
#include "ootrace.h"
#include "ooq931.h"
#include "ooh245.h"
#include "ooh323ep.h"
#include "oochannels.h"
#include "ooCalls.h"

/** Global endpoint structure */
extern OOH323EndPoint gH323ep;

extern OO_MUTEX gCmdMutex;

int ooMakeCall
   (const char* dest, char* callToken, size_t bufsiz, ooCallOptions *opts)
{
   OOStackCommand *cmd;
   int ret;
#ifdef _WIN32
   EnterCriticalSection(&gCmdMutex);
#else
   pthread_mutex_lock(&gCmdMutex);
#endif
   cmd = (OOStackCommand*) memAlloc (&gH323ep.ctxt, sizeof(OOStackCommand));
   if(!cmd)
   {
      OOTRACEERR1("Error:Memory - ooMakeCall - cmd\n");
      return OO_FAILED;
   }
   cmd->type = OO_CMD_MAKECALL;
   cmd->param1 = (void*) memAlloc(&gH323ep.ctxt, strlen(dest)+1);
   if(!cmd->param1)
   {
      OOTRACEERR1("ERROR:Memory - ooMakeCall - param1\n");
      memFreePtr(&gH323ep.ctxt, cmd);
      return OO_FAILED;
   }
   strcpy((char*)cmd->param1, dest);

   /* Generate call token*/
   if(!callToken)
   {
      OOTRACEERR1("ERROR:Invalid 'callToken' parameter to 'ooMakeCall'\n");
      memFreePtr(&gH323ep.ctxt, cmd->param1);
      memFreePtr(&gH323ep.ctxt, cmd);
      return OO_FAILED;
   }

   if ((ret = ooGenerateCallToken (callToken, bufsiz)) != 0)
      return ret;

   cmd->param2 = (void*) memAlloc(&gH323ep.ctxt, strlen(callToken)+1);
   if(!cmd->param2)
   {
      OOTRACEERR1("ERROR:Memory - ooMakeCall - param2\n");
      memFreePtr(&gH323ep.ctxt, cmd->param1);
      memFreePtr(&gH323ep.ctxt, cmd);
      return OO_FAILED;
   }
  
   strcpy((char*)cmd->param2, callToken);

   if(!opts)
   {
      cmd->param3 = 0;
   }else {
      cmd->param3 = (void*) memAlloc(&gH323ep.ctxt, sizeof(ooCallOptions));
      if(!cmd->param3)
      {
         OOTRACEERR1("ERROR:Memory - ooMakeCall - param3\n");
         memFreePtr(&gH323ep.ctxt, cmd->param1);
         memFreePtr(&gH323ep.ctxt, cmd->param2);
         memFreePtr(&gH323ep.ctxt, cmd);
         return OO_FAILED;
      }
      memcpy((void*)cmd->param3, opts, sizeof(ooCallOptions));
   }


   dListAppend (&gH323ep.ctxt, &gH323ep.stkCmdList, cmd);
#ifdef HAVE_PIPE
   if(write(gH323ep.cmdPipe[1], "c", 1)<0)
   {
      OOTRACEERR1("ERROR:Failed to write to command pipe\n");
   }
#endif

#ifdef _WIN32
   LeaveCriticalSection(&gCmdMutex);
#else
   pthread_mutex_unlock(&gCmdMutex);
#endif

   return OO_OK;
}

int ooAnswerCall(char *callToken)
{
   OOStackCommand *cmd;

   if(!callToken)
   {
      OOTRACEERR1("ERROR: Invalid callToken passed to ooAnswerCall\n");
      return OO_FAILED;
   }

#ifdef _WIN32
   EnterCriticalSection(&gCmdMutex);
#else
   pthread_mutex_lock(&gCmdMutex);
#endif
   cmd = (OOStackCommand*)memAlloc(&gH323ep.ctxt, sizeof(OOStackCommand));
   if(!cmd)
   {
      OOTRACEERR1("Error:Memory - ooAnswerCall - cmd\n");
      return OO_FAILED;
   }
   memset(cmd, 0, sizeof(OOStackCommand));
   cmd->type = OO_CMD_ANSCALL;

   cmd->param1 = (void*) memAlloc(&gH323ep.ctxt, strlen(callToken)+1);
   if(!cmd->param1)
   {
      OOTRACEERR1("ERROR:Memory - ooAnswerCall - param1\n");
      return OO_FAILED;
   }
   strcpy((char*)cmd->param1, callToken);
  
   dListAppend(&gH323ep.ctxt, &gH323ep.stkCmdList, cmd);
  
#ifdef HAVE_PIPE
   if(write(gH323ep.cmdPipe[1], "c", 1)<0)
   {
      OOTRACEERR1("ERROR:Failed to write to command pipe\n");
   }
#endif

#ifdef _WIN32
   LeaveCriticalSection(&gCmdMutex);
#else
   pthread_mutex_unlock(&gCmdMutex);
#endif

   return OO_OK;
}

int ooForwardCall(char* callToken, char *dest)
{
   OOStackCommand *cmd;

   if(!callToken || !dest)
   {
      OOTRACEERR1("ERROR: Invalid callToken/dest passed to ooForwardCall\n");
      return OO_FAILED;
   }
#ifdef _WIN32
   EnterCriticalSection(&gCmdMutex);
#else
   pthread_mutex_lock(&gCmdMutex);
#endif
   cmd = (OOStackCommand*)memAlloc(&gH323ep.ctxt, sizeof(OOStackCommand));
   if(!cmd)
   {
      OOTRACEERR1("Error:Memory - ooForwardCall - cmd\n");
      return OO_FAILED;
   }
   memset(cmd, 0, sizeof(OOStackCommand));
   cmd->type = OO_CMD_FWDCALL;

   cmd->param1 = (void*) memAlloc(&gH323ep.ctxt, strlen(callToken)+1);
   cmd->param2 = (void*) memAlloc(&gH323ep.ctxt, strlen(dest)+1);
   if(!cmd->param1 || !cmd->param2)
   {
      OOTRACEERR1("ERROR:Memory - ooForwardCall - param1/param2\n");
      return OO_FAILED;
   }
   strcpy((char*)cmd->param1, callToken);
   strcpy((char*)cmd->param2, dest);
   dListAppend(&gH323ep.ctxt, &gH323ep.stkCmdList, cmd);
  
#ifdef HAVE_PIPE
   if(write(gH323ep.cmdPipe[1], "c", 1)<0)
   {
      OOTRACEERR1("ERROR:Failed to write to command pipe\n");
   }
#endif
#ifdef _WIN32
   LeaveCriticalSection(&gCmdMutex);
#else
   pthread_mutex_unlock(&gCmdMutex);
#endif

   return OO_OK;
}


int ooHangCall(char *callToken, OOCallClearReason reason)
{
   OOStackCommand *cmd;

   if(!callToken)
   {
      OOTRACEERR1("ERROR: Invalid callToken passed to ooHangCall\n");
      return OO_FAILED;
   }

#ifdef _WIN32
   EnterCriticalSection(&gCmdMutex);
#else
   pthread_mutex_lock(&gCmdMutex);
#endif
 
   cmd = (OOStackCommand*)memAlloc(&gH323ep.ctxt, sizeof(OOStackCommand));
   if(!cmd)
   {
      OOTRACEERR1("Error:Allocating memory for command structure - HangCall\n");
      return OO_FAILED;
   }

   memset(cmd, 0, sizeof(OOStackCommand));
   cmd->type = OO_CMD_HANGCALL;
   cmd->param1 = (void*) memAlloc(&gH323ep.ctxt, strlen(callToken)+1);
   cmd->param2 = (void*)memAlloc(&gH323ep.ctxt, sizeof(OOCallClearReason));
   if(!cmd->param1 || !cmd->param2)
   {
      OOTRACEERR1("ERROR:Allocating memory for cmd param1/param2- HangCall\n");
      return OO_FAILED;
   }
   strcpy((char*)cmd->param1, callToken);
   *((OOCallClearReason*)cmd->param2) = reason;
  
   dListAppend(&gH323ep.ctxt, &gH323ep.stkCmdList, cmd);
  
#ifdef HAVE_PIPE
   if(write(gH323ep.cmdPipe[1], "c", 1)<0)
   {
      OOTRACEERR1("ERROR:Failed to write to command pipe\n");
   }
#endif
#ifdef _WIN32
   LeaveCriticalSection(&gCmdMutex);
#else
   pthread_mutex_unlock(&gCmdMutex);
#endif

   return OO_OK;
}

int ooProcStackCmds()
{
   OOH323CallData *call = NULL;
   if (gH323ep.stkCmdList.count > 0)
   {
      OOStackCommand *cmd;
      DListNode *pNode = NULL;
      //while (0 !=(cmd=dListDeleteHead (&gH323ep.ctxt, &gH323ep.stkCmdList)))
      while (0 != (pNode = dListFindByIndex(&gH323ep.stkCmdList, 0)))
      {
         cmd = (OOStackCommand*)pNode->data;
         switch(cmd->type) {
            case OO_CMD_MAKECALL:
               if(gH323ep.gkClient &&
                  gH323ep.gkClient->state != GkClientRegistered)
               {
                  OOTRACEWARN1("WARNING: New outgoing call cmd is waiting for "
                               "gatekeeper registration.\n");
                  continue;
               }

               OOTRACEINFO2("Processing MakeCall command %s\n",
                             (char*)cmd->param2);

               ooH323MakeCall ((char*)cmd->param1, (char*)cmd->param2,
                               (ooCallOptions*)cmd->param3);
               dListRemove(&gH323ep.stkCmdList, pNode);
               memFreePtr(&gH323ep.ctxt, pNode);
               break;

            case OO_CMD_ANSCALL:
               if(gH323ep.gkClient &&
                  gH323ep.gkClient->state != GkClientRegistered)
               {
                  OOTRACEWARN1("New answer call cmd is waiting for "
                               "gatekeeper registration.\n");
                  continue;
               }
               OOTRACEINFO2("Processing Answer Call command for %s\n",
                            (char*)cmd->param1);
               ooSendConnect(ooFindCallByToken((char*)cmd->param1));
               dListRemove(&gH323ep.stkCmdList, pNode);
               memFreePtr(&gH323ep.ctxt, pNode);
               break;

            case OO_CMD_FWDCALL:
               OOTRACEINFO3("Forwarding call %s to %s\n", (char*)cmd->param1,
                                                          (char*)cmd->param2);
               ooH323ForwardCall((char*)cmd->param1, (char*)cmd->param2);
               dListRemove(&gH323ep.stkCmdList, pNode);
               memFreePtr(&gH323ep.ctxt, pNode);
               break;

            case OO_CMD_HANGCALL:
               OOTRACEINFO2("Processing Hang call command %s\n",
                             (char*)cmd->param1);
               ooH323HangCall((char*)cmd->param1,
                                       *(OOCallClearReason*)cmd->param2);
               dListRemove(&gH323ep.stkCmdList, pNode);
               memFreePtr(&gH323ep.ctxt, pNode);
               break;
           
            case OO_CMD_SENDDIGIT:
               call = ooFindCallByToken((char*)cmd->param1);
               if(!call)
               {
                  OOTRACEERR2("Error:Invalid calltoken %s\n",
                                                         (char*)cmd->param1);
                  dListRemove(&gH323ep.stkCmdList, pNode);
                  memFreePtr(&gH323ep.ctxt, pNode);
                  break;
               }
               if(call->jointDtmfMode & OO_CAP_DTMF_H245_alphanumeric)
                  ooSendH245UserInputIndication_alphanumeric(call,
                                                    (const char*)cmd->param2);
               else if(call->jointDtmfMode & OO_CAP_DTMF_H245_signal)
                  ooSendH245UserInputIndication_signal(call,
                                                     (const char*)cmd->param2);
               else
                  ooQ931SendDTMFAsKeyPadIE(call, (const char*)cmd->param2);

               dListRemove(&gH323ep.stkCmdList, pNode);
               memFreePtr(&gH323ep.ctxt, pNode);
               break;

            case OO_CMD_STOPMONITOR:
               OOTRACEINFO1("Processing StopMonitor command\n");
               ooStopMonitorCalls();
               dListRemove(&gH323ep.stkCmdList, pNode);
               memFreePtr(&gH323ep.ctxt, pNode);
               break;

            default: OOTRACEERR1("ERROR:Unhandled command\n");
         }
         if(cmd->param1) memFreePtr(&gH323ep.ctxt, cmd->param1);
         if(cmd->param2) memFreePtr(&gH323ep.ctxt, cmd->param2);
         if(cmd->param3) memFreePtr(&gH323ep.ctxt, cmd->param3);
         memFreePtr (&gH323ep.ctxt, cmd);
      }
   }

   return OO_OK;
}

int ooStopMonitor()
{
   OOStackCommand *cmd;
#ifdef _WIN32
   EnterCriticalSection(&gCmdMutex);
#else
   pthread_mutex_lock(&gCmdMutex);
#endif
   cmd = (OOStackCommand*)memAlloc(&gH323ep.ctxt, sizeof(OOStackCommand));
   if(!cmd)
   {
      OOTRACEERR1("Error:Allocating memory for command structure - StopMonitor\n");
      return OO_FAILED;
   }
   memset(cmd, 0, sizeof(OOStackCommand));
   cmd->type = OO_CMD_STOPMONITOR;
  
   dListAppend (&gH323ep.ctxt, &gH323ep.stkCmdList, cmd);

#ifdef HAVE_PIPE
   if(write(gH323ep.cmdPipe[1], "c", 1)<0)
   {
      OOTRACEERR1("ERROR:Failed to write to command pipe\n");
   }
#endif
  
#ifdef _WIN32
   LeaveCriticalSection(&gCmdMutex);
#else
   pthread_mutex_unlock(&gCmdMutex);
#endif
   return OO_OK;
}

int ooSendDTMFDigit(char *callToken, char* dtmf)
{
   OOStackCommand *cmd = NULL;

   if(!callToken)
   {
      OOTRACEERR1("ERROR: Invalid callToken passed to ooSendDTMFDigit\n");
      return OO_FAILED;
   }

#ifdef _WIN32
   EnterCriticalSection(&gCmdMutex);
#else
   pthread_mutex_lock(&gCmdMutex);
#endif
   cmd = (OOStackCommand*)memAlloc(&gH323ep.ctxt, sizeof(OOStackCommand));
   if(!cmd)
   {
      OOTRACEERR1("Error:Memory - ooSendDTMFDigit - cmd\n");
      return OO_FAILED;
   }
   memset(cmd, 0, sizeof(OOStackCommand));
   cmd->type = OO_CMD_SENDDIGIT;

   cmd->param1 = (void*) memAlloc(&gH323ep.ctxt, strlen(callToken)+1);
   cmd->param2 = (void*) memAlloc(&gH323ep.ctxt, strlen(dtmf)+1);
   if(!cmd->param1 || !cmd->param2)
   {
      OOTRACEERR1("ERROR:Memory - ooSendDTMFDigit - param1/param2\n");
      return OO_FAILED;
   }
   strcpy((char*)cmd->param1, callToken);
   strcpy((char*)cmd->param2, dtmf);
  
   dListAppend(&gH323ep.ctxt, &gH323ep.stkCmdList, cmd);
  
#ifdef HAVE_PIPE
   if(write(gH323ep.cmdPipe[1], "c", 1)<0)
   {
      OOTRACEERR1("ERROR:Failed to write to command pipe\n");
   }
#endif

#ifdef _WIN32
   LeaveCriticalSection(&gCmdMutex);
#else
   pthread_mutex_unlock(&gCmdMutex);
#endif

   return OO_OK;
}
