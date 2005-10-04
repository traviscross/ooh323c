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
#include "ooMutex.h"

/** Global endpoint structure */
extern OOH323EndPoint gH323ep;


extern DList gStkCmdList;

extern OOCTXT gCmdCtxt;



int ooMakeCall
   (const char* dest, char* callToken, size_t bufsiz, ooCallOptions *opts)
{
   OOStackCommand *cmd;
   int ret;

   ooMutexAcquireCmdMutex(); /* Acquire lock */

   cmd = (OOStackCommand*) memAlloc (&gCmdCtxt, sizeof(OOStackCommand));
   if(!cmd)
   {
      OOTRACEERR1("Error:Memory - ooMakeCall - cmd\n");
      ooMutexReleaseCmdMutex(); /* Release lock */
      return OO_FAILED;
   }
   cmd->type = OO_CMD_MAKECALL;
   cmd->param1 = (void*) memAlloc(&gCmdCtxt, strlen(dest)+1);
   if(!cmd->param1)
   {
      OOTRACEERR1("ERROR:Memory - ooMakeCall - param1\n");
      memFreePtr(&gCmdCtxt, cmd); /* free memory */
      ooMutexReleaseCmdMutex();  /* Release lock */
      return OO_FAILED;
   }
   strcpy((char*)cmd->param1, dest);

   /* Generate call token*/
   if(!callToken)
   {
      OOTRACEERR1("ERROR:Invalid 'callToken' parameter to 'ooMakeCall'\n");
      memFreePtr(&gCmdCtxt, cmd->param1); /* free memory */
      memFreePtr(&gCmdCtxt, cmd);
      ooMutexReleaseCmdMutex(); /* Release lock */
      return OO_FAILED;
   }

   if ((ret = ooGenerateCallToken (callToken, bufsiz)) != 0){
      memFreePtr(&gCmdCtxt, cmd->param1); /* free memory */
      memFreePtr(&gCmdCtxt, cmd);
      ooMutexReleaseCmdMutex(); /* Release lock */
      return ret;
   }

   cmd->param2 = (void*) memAlloc(&gCmdCtxt, strlen(callToken)+1);
   if(!cmd->param2)
   {
      OOTRACEERR1("ERROR:Memory - ooMakeCall - param2\n");
      memFreePtr(&gCmdCtxt, cmd->param1);
      memFreePtr(&gCmdCtxt, cmd);
      ooMutexReleaseCmdMutex();
      return OO_FAILED;
   }
  
   strcpy((char*)cmd->param2, callToken);

   if(!opts)
   {
      cmd->param3 = 0;
   }else {
      cmd->param3 = (void*) memAlloc(&gCmdCtxt, sizeof(ooCallOptions));
      if(!cmd->param3)
      {
         OOTRACEERR1("ERROR:Memory - ooMakeCall - param3\n");
         memFreePtr(&gCmdCtxt, cmd->param1);
         memFreePtr(&gCmdCtxt, cmd->param2);
         memFreePtr(&gCmdCtxt, cmd);
         ooMutexReleaseCmdMutex();
         return OO_FAILED;
      }
      memcpy((void*)cmd->param3, opts, sizeof(ooCallOptions));
   }


   dListAppend (&gCmdCtxt, &gStkCmdList, cmd);
#ifdef HAVE_PIPE
   if(write(gH323ep.cmdPipe[1], "c", 1)<0)
   {
      OOTRACEERR1("ERROR:Failed to write to command pipe\n");
   }
#endif

   ooMutexReleaseCmdMutex();

   return OO_OK;
}


int ooManualRingback(const char *callToken)
{
   OOStackCommand *cmd;
   if(!callToken)
   {
      OOTRACEERR1("Error: Invalid callToken passed to ooManulRingback\n");
      return OO_FAILED;
   }

   ooMutexAcquireCmdMutex();

   cmd = (OOStackCommand*)memAlloc(&gCmdCtxt, sizeof(OOStackCommand));
   if(!cmd)
   {
      OOTRACEERR1("Error:Memory - ooManualRingback - cmd\n");
      ooMutexReleaseCmdMutex();
      return OO_FAILED;
   }
   memset(cmd, 0, sizeof(OOStackCommand));
   cmd->type = OO_CMD_MANUALRINGBACK;
   cmd->param1 = (void*) memAlloc(&gCmdCtxt, strlen(callToken)+1);
   if(!cmd->param1)
   {
      OOTRACEERR1("Error:Memory - ooManualRingback - cmd->param1\n");
      memFreePtr(&gCmdCtxt, cmd);
      ooMutexReleaseCmdMutex();
      return OO_FAILED;
   }
   strcpy((char*)cmd->param1, callToken);
  
   dListAppend( &gCmdCtxt, &gStkCmdList, cmd);
  
#ifdef HAVE_PIPE
   if(write(gH323ep.cmdPipe[1], "c", 1)<0)
   {
      OOTRACEERR1("ERROR:Failed to write to command pipe\n");
   }
#endif

   ooMutexReleaseCmdMutex();

   return OO_OK;
}

int ooAnswerCall(const char *callToken)
{
   OOStackCommand *cmd;

   if(!callToken)
   {
      OOTRACEERR1("ERROR: Invalid callToken passed to ooAnswerCall\n");
      return OO_FAILED;
   }

   ooMutexAcquireCmdMutex();

   cmd = (OOStackCommand*)memAlloc(&gCmdCtxt, sizeof(OOStackCommand));
   if(!cmd)
   {
      OOTRACEERR1("Error:Memory - ooAnswerCall - cmd\n");
      ooMutexReleaseCmdMutex();
      return OO_FAILED;
   }
   memset(cmd, 0, sizeof(OOStackCommand));
   cmd->type = OO_CMD_ANSCALL;

   cmd->param1 = (void*) memAlloc(&gCmdCtxt, strlen(callToken)+1);
   if(!cmd->param1)
   {
      OOTRACEERR1("ERROR:Memory - ooAnswerCall - param1\n");
      memFreePtr(&gCmdCtxt, cmd);
      ooMutexReleaseCmdMutex();
      return OO_FAILED;
   }
   strcpy((char*)cmd->param1, callToken);
  
   dListAppend(&gCmdCtxt, &gStkCmdList, cmd);
  
#ifdef HAVE_PIPE
   if(write(gH323ep.cmdPipe[1], "c", 1)<0)
   {
      OOTRACEERR1("ERROR:Failed to write to command pipe\n");
   }
#endif

   ooMutexReleaseCmdMutex();

   return OO_OK;
}

int ooForwardCall(const char* callToken, char *dest)
{
   OOStackCommand *cmd;

   if(!callToken || !dest)
   {
      OOTRACEERR1("ERROR: Invalid callToken/dest passed to ooForwardCall\n");
      return OO_FAILED;
   }

   ooMutexAcquireCmdMutex();

   cmd = (OOStackCommand*)memAlloc(&gCmdCtxt, sizeof(OOStackCommand));
   if(!cmd)
   {
      OOTRACEERR1("Error:Memory - ooForwardCall - cmd\n");
      ooMutexReleaseCmdMutex(); /* Release lock */
      return OO_FAILED;
   }
   memset(cmd, 0, sizeof(OOStackCommand));
   cmd->type = OO_CMD_FWDCALL;

   cmd->param1 = (void*) memAlloc(&gCmdCtxt, strlen(callToken)+1);
   cmd->param2 = (void*) memAlloc(&gCmdCtxt, strlen(dest)+1);
   if(!cmd->param1 || !cmd->param2)
   {
      OOTRACEERR1("ERROR:Memory - ooForwardCall - param1/param2\n");
      if(cmd->param1)   memFreePtr(&gCmdCtxt, cmd->param1);  /* Release memory */
      if(cmd->param2)   memFreePtr(&gCmdCtxt, cmd->param2);
      memFreePtr(&gCmdCtxt, cmd);
      ooMutexReleaseCmdMutex();   /* Release lock */
      return OO_FAILED;
   }
   strcpy((char*)cmd->param1, callToken);
   strcpy((char*)cmd->param2, dest);
   dListAppend(&gCmdCtxt, &gStkCmdList, cmd);
  
#ifdef HAVE_PIPE
   if(write(gH323ep.cmdPipe[1], "c", 1)<0)
   {
      OOTRACEERR1("ERROR:Failed to write to command pipe\n");
   }
#endif

   ooMutexReleaseCmdMutex(); /* Release lock */

   return OO_OK;
}


int ooHangCall(const char* callToken, OOCallClearReason reason)
{
   OOStackCommand *cmd;

   if(!callToken)
   {
      OOTRACEERR1("ERROR: Invalid callToken passed to ooHangCall\n");
      return OO_FAILED;
   }

   ooMutexAcquireCmdMutex(); /* acquire lock */

   cmd = (OOStackCommand*)memAlloc(&gCmdCtxt, sizeof(OOStackCommand));
   if(!cmd)
   {
      OOTRACEERR1("Error:Allocating memory for command structure - HangCall\n");
      ooMutexReleaseCmdMutex(); /* Release lock */
      return OO_FAILED;
   }

   memset(cmd, 0, sizeof(OOStackCommand));
   cmd->type = OO_CMD_HANGCALL;
   cmd->param1 = (void*) memAlloc(&gCmdCtxt, strlen(callToken)+1);
   cmd->param2 = (void*)memAlloc(&gCmdCtxt, sizeof(OOCallClearReason));
   if(!cmd->param1 || !cmd->param2)
   {
      OOTRACEERR1("ERROR:Allocating memory for cmd param1/param2- HangCall\n");
      if(cmd->param1)   memFreePtr(&gCmdCtxt, cmd->param1); /* Release memory */
      if(cmd->param2)   memFreePtr(&gCmdCtxt, cmd->param2);
      memFreePtr(&gCmdCtxt, cmd);
      ooMutexReleaseCmdMutex();  /* Release lock */
      return OO_FAILED;
   }
   strcpy((char*)cmd->param1, callToken);
   *((OOCallClearReason*)cmd->param2) = reason;
  
   dListAppend(&gCmdCtxt, &gStkCmdList, cmd);
  
#ifdef HAVE_PIPE
   if(write(gH323ep.cmdPipe[1], "c", 1)<0)
   {
      OOTRACEERR1("ERROR:Failed to write to command pipe\n");
   }
#endif
   ooMutexReleaseCmdMutex(); /* Release lock */

   return OO_OK;
}


int ooStopMonitor()
{
   OOStackCommand *cmd;

   ooMutexAcquireCmdMutex(); /* Acquire lock */

   cmd = (OOStackCommand*)memAlloc(&gCmdCtxt, sizeof(OOStackCommand));
   if(!cmd)
   {
      OOTRACEERR1("Error:Allocating memory for command structure - StopMonitor\n");
      ooMutexReleaseCmdMutex(); /* Release lock */

      return OO_FAILED;
   }
   memset(cmd, 0, sizeof(OOStackCommand));
   cmd->type = OO_CMD_STOPMONITOR;
  
   dListAppend (&gCmdCtxt, &gStkCmdList, cmd);

#ifdef HAVE_PIPE
   if(write(gH323ep.cmdPipe[1], "c", 1)<0)
   {
      OOTRACEERR1("ERROR:Failed to write to command pipe\n");
   }
#endif
  
   ooMutexReleaseCmdMutex(); /* Release lock */

   return OO_OK;
}

int ooSendDTMFDigit(const char *callToken, const char* dtmf)
{
   OOStackCommand *cmd = NULL;

   if(!callToken)
   {
      OOTRACEERR1("ERROR: Invalid callToken passed to ooSendDTMFDigit\n");
      return OO_FAILED;
   }

   ooMutexAcquireCmdMutex(); /* Acquire lock */

   cmd = (OOStackCommand*)memAlloc(&gCmdCtxt, sizeof(OOStackCommand));
   if(!cmd)
   {
      OOTRACEERR1("Error:Memory - ooSendDTMFDigit - cmd\n");
      ooMutexReleaseCmdMutex(); /* Release lock */
      return OO_FAILED;
   }
   memset(cmd, 0, sizeof(OOStackCommand));
   cmd->type = OO_CMD_SENDDIGIT;

   cmd->param1 = (void*) memAlloc(&gCmdCtxt, strlen(callToken)+1);
   cmd->param2 = (void*) memAlloc(&gCmdCtxt, strlen(dtmf)+1);
   if(!cmd->param1 || !cmd->param2)
   {
      OOTRACEERR1("ERROR:Memory - ooSendDTMFDigit - param1/param2\n");
      if(cmd->param1)   memFreePtr(&gCmdCtxt, cmd->param1); /* Release memory */
      if(cmd->param2)   memFreePtr(&gCmdCtxt, cmd->param2);
      memFreePtr(&gCmdCtxt, cmd);
      ooMutexReleaseCmdMutex(); /* Release lock */
      return OO_FAILED;
   }
   strcpy((char*)cmd->param1, callToken);
   strcpy((char*)cmd->param2, dtmf);
  
   dListAppend(&gCmdCtxt, &gStkCmdList, cmd);
  
#ifdef HAVE_PIPE
   if(write(gH323ep.cmdPipe[1], "c", 1)<0)
   {
      OOTRACEERR1("ERROR:Failed to write to command pipe\n");
   }
#endif

   ooMutexReleaseCmdMutex(); /* Release lock */

   return OO_OK;
}


int ooProcStackCmds()
{
   OOH323CallData *call = NULL;
   if (gStkCmdList.count > 0)
   {
      OOStackCommand *cmd;
      DListNode *pNode = NULL;
      //while (0 !=(cmd=dListDeleteHead (&gH323ep.ctxt, &gH323ep.stkCmdList)))
      while (0 != (pNode = dListFindByIndex(&gStkCmdList, 0)))
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
               dListRemove(&gStkCmdList, pNode);
               memFreePtr(&gCmdCtxt, pNode);
               break;

            case OO_CMD_MANUALRINGBACK:
               if(gH323ep.gkClient &&
                  gH323ep.gkClient->state != GkClientRegistered)
               {
                  OOTRACEWARN1("New answer call cmd is waiting for "
                               "gatekeeper registration.\n");
                  continue;
               }
               if(OO_TESTFLAG(gH323ep.flags, OO_M_MANUALRINGBACK))
               {
                  ooSendAlerting(ooFindCallByToken((char*)cmd->param1));
                  if(OO_TESTFLAG(gH323ep.flags, OO_M_AUTOANSWER))
                     ooSendConnect(ooFindCallByToken((char*)cmd->param1));
               }
               dListRemove(&gStkCmdList, pNode);
               memFreePtr(&gCmdCtxt, pNode);
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
               dListRemove(&gStkCmdList, pNode);
               memFreePtr(&gCmdCtxt, pNode);
               break;

            case OO_CMD_FWDCALL:
               OOTRACEINFO3("Forwarding call %s to %s\n", (char*)cmd->param1,
                                                          (char*)cmd->param2);
               ooH323ForwardCall((char*)cmd->param1, (char*)cmd->param2);
               dListRemove(&gStkCmdList, pNode);
               memFreePtr(&gCmdCtxt, pNode);
               break;

            case OO_CMD_HANGCALL:
               OOTRACEINFO2("Processing Hang call command %s\n",
                             (char*)cmd->param1);
               ooH323HangCall((char*)cmd->param1,
                                       *(OOCallClearReason*)cmd->param2);
               dListRemove(&gStkCmdList, pNode);
               memFreePtr(&gCmdCtxt, pNode);
               break;
           
            case OO_CMD_SENDDIGIT:
               call = ooFindCallByToken((char*)cmd->param1);
               if(!call)
               {
                  OOTRACEERR2("Error:Invalid calltoken %s\n",
                                                         (char*)cmd->param1);
                  dListRemove(&gStkCmdList, pNode);
                  memFreePtr(&gCmdCtxt, pNode);
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

               dListRemove(&gStkCmdList, pNode);
               memFreePtr(&gCmdCtxt, pNode);
               break;

            case OO_CMD_STOPMONITOR:
               OOTRACEINFO1("Processing StopMonitor command\n");
               ooStopMonitorCalls();
               dListRemove(&gStkCmdList, pNode);
               memFreePtr(&gCmdCtxt, pNode);
               break;

            default: OOTRACEERR1("ERROR:Unknown command\n");
         }
         if(cmd->param1) memFreePtr(&gCmdCtxt, cmd->param1);
         if(cmd->param2) memFreePtr(&gCmdCtxt, cmd->param2);
         if(cmd->param3) memFreePtr(&gCmdCtxt, cmd->param3);
         memFreePtr (&gCmdCtxt, cmd);
      }
   }

   return OO_OK;
}




int ooProcessStackCmds()
{
   OOStackCommand cmd;
   OOH323CallData *call = NULL;

   memset(&cmd, 0, sizeof(OOStackCommand));
   cmd.type = OO_CMD_NOOP;

   if(ooGetStackCmd(&cmd) != OO_OK)
   {
      OOTRACEERR1("Error:Failed to retrieve stack command\n");
      return OO_FAILED;
   }
  

   if(cmd.type == OO_CMD_NOOP)
      return OO_OK;


   switch(cmd.type) {
      case OO_CMD_MAKECALL:
         OOTRACEINFO2("Processing MakeCall command %s\n",
                              (char*)cmd.param2);

         ooH323MakeCall ((char*)cmd.param1, (char*)cmd.param2,
                               (ooCallOptions*)cmd.param3);
         break;

      case OO_CMD_MANUALRINGBACK:
         if(OO_TESTFLAG(gH323ep.flags, OO_M_MANUALRINGBACK))
         {
            ooSendAlerting(ooFindCallByToken((char*)cmd.param1));
            if(OO_TESTFLAG(gH323ep.flags, OO_M_AUTOANSWER))
               ooSendConnect(ooFindCallByToken((char*)cmd.param1));
         }
         break;

      case OO_CMD_ANSCALL:
         OOTRACEINFO2("Processing Answer Call command for %s\n",
                      (char*)cmd.param1);
         ooSendConnect(ooFindCallByToken((char*)cmd.param1));
         break;

      case OO_CMD_FWDCALL:
         OOTRACEINFO3("Forwarding call %s to %s\n", (char*)cmd.param1,
                                                          (char*)cmd.param2);
         ooH323ForwardCall((char*)cmd.param1, (char*)cmd.param2);
         break;

      case OO_CMD_HANGCALL:
         OOTRACEINFO2("Processing Hang call command %s\n",
                             (char*)cmd.param1);
         ooH323HangCall((char*)cmd.param1,
                                       *(OOCallClearReason*)cmd.param2);
         break;
         
      case OO_CMD_SENDDIGIT:
         call = ooFindCallByToken((char*)cmd.param1);
         if(!call)
         {
            OOTRACEERR2("Error:Invalid calltoken %s\n",
                                                   (char*)cmd.param1);
            break;
         }
         if(call->jointDtmfMode & OO_CAP_DTMF_H245_alphanumeric)
            ooSendH245UserInputIndication_alphanumeric(call,
                                                    (const char*)cmd.param2);
         else if(call->jointDtmfMode & OO_CAP_DTMF_H245_signal)
            ooSendH245UserInputIndication_signal(call,
                                               (const char*)cmd.param2);
         else
            ooQ931SendDTMFAsKeyPadIE(call, (const char*)cmd.param2);

         break;

      case OO_CMD_STOPMONITOR:
         OOTRACEINFO1("Processing StopMonitor command\n");
         ooStopMonitorCalls();
         break;

      default: OOTRACEERR1("ERROR:Unknown command\n");
   }
   if(cmd.param1) memFreePtr(&gH323ep.ctxt, cmd.param1);
   if(cmd.param2) memFreePtr(&gH323ep.ctxt, cmd.param2);
   if(cmd.param3) memFreePtr(&gH323ep.ctxt, cmd.param3);

   return OO_OK;
}




int ooGetStackCmd(OOStackCommand *pCmd)
{
   OOStackCommand *cmd;
   DListNode *pNode = NULL;

   ooMutexAcquireCmdMutex(); /*Acquire lock */
   if (gStkCmdList.count > 0)
   {
      if(gH323ep.gkClient &&
                  gH323ep.gkClient->state != GkClientRegistered)
      {
         OOTRACEWARN1("Warning:Can not process stack command. Waiting for successful "
                     "gatekeeper registration.\n");
         ooMutexReleaseCmdMutex(); /* Release lock */
         return OO_OK;
      }

      if (0 != (pNode = dListFindByIndex(&gStkCmdList, 0)))
      {
         cmd = (OOStackCommand*)pNode->data;
         switch(cmd->type)
         {
            case OO_CMD_MAKECALL:
               OOTRACEDBGC1("Found MakeCall command\n");
               pCmd->type = cmd->type;
               pCmd->param1 = (void*)memAlloc(&gH323ep.ctxt, strlen((char*)cmd->param1)+1);
               pCmd->param2 = (void*)memAlloc(&gH323ep.ctxt, strlen((char*)cmd->param2)+1);
               if(cmd->param3) /* Third parameter is optional */
                  pCmd->param3 = (void*)memAlloc(&gH323ep.ctxt, sizeof(ooCallOptions));
               if(!pCmd->param1 || !pCmd->param2 || (cmd->param3 && !pCmd->param3))
               {
                  OOTRACEERR1("Error:Memory - ooGetStackCmd(MakeCall) - params\n");
                  if(pCmd->param1)   memFreePtr(&gH323ep.ctxt, pCmd->param1);
                  if(pCmd->param2)   memFreePtr(&gH323ep.ctxt, pCmd->param2);
                  if(pCmd->param3)   memFreePtr(&gH323ep.ctxt, pCmd->param3);
                  ooMutexReleaseCmdMutex(); /* Release lock */
                  return OO_FAILED;
               }

               strcpy((char*)pCmd->param1, (char*)cmd->param1);
               strcpy((char*)pCmd->param2, (char*)cmd->param2);
               if(pCmd->param3)
                  memcpy(pCmd->param3, cmd->param3, sizeof(ooCallOptions));
               break;

            case OO_CMD_MANUALRINGBACK:
            case OO_CMD_ANSCALL:
               pCmd->type = cmd->type;
               pCmd->param1 = (void*)memAlloc(&gH323ep.ctxt, strlen((char*)cmd->param1)+1);
               if(!pCmd->param1)
               {
                  OOTRACEERR1("Error:Memory - ooGetStackCmd(AnsCall/ManualRingback) - params\n");
                  ooMutexReleaseCmdMutex();
                  return OO_FAILED; /* Release lock */
               }
               strcpy((char*)pCmd->param1, (char*)cmd->param1);
               break;

            case OO_CMD_FWDCALL:
            case OO_CMD_SENDDIGIT:
               pCmd->type = cmd->type;
               pCmd->param1 = (void*)memAlloc(&gH323ep.ctxt, strlen((char*)cmd->param1)+1);
               pCmd->param2 = (void*)memAlloc(&gH323ep.ctxt, strlen((char*)cmd->param2)+1);
               if(!pCmd->param1 || !pCmd->param2)
               {
                  OOTRACEERR1("Error:Memory - ooGetStackCmd(FwdCall/SendDigit) - params\n");
                  if(pCmd->param1)   memFreePtr(&gH323ep.ctxt, pCmd->param1);
                  if(pCmd->param2)   memFreePtr(&gH323ep.ctxt, pCmd->param2);
                  ooMutexReleaseCmdMutex(); /* Release lock */
                  return OO_FAILED;
               }
               strcpy((char*)pCmd->param1, (char*)cmd->param1);
               strcpy((char*)pCmd->param2, (char*)cmd->param2);
               break;

            case OO_CMD_HANGCALL:
               pCmd->type = cmd->type;
               pCmd->param1 = (void*)memAlloc(&gH323ep.ctxt, strlen((char*)cmd->param1)+1);
               pCmd->param2 = (void*)memAlloc(&gH323ep.ctxt, sizeof(OOCallClearReason));
               if(!pCmd->param1 || !pCmd->param2)
               {
                  OOTRACEERR1("Error:Memory - ooGetStackCmd(HangCall) - params\n");
                  if(pCmd->param1)   memFreePtr(&gH323ep.ctxt, pCmd->param1);
                  if(pCmd->param2)   memFreePtr(&gH323ep.ctxt, pCmd->param2);
                  ooMutexReleaseCmdMutex(); /* Release lock */
                  return OO_FAILED;
               }
               strcpy((char*)pCmd->param1, (char*)cmd->param1);
               memcpy(pCmd->param2, cmd->param2, sizeof(OOCallClearReason));
               break;

            case OO_CMD_STOPMONITOR:
              pCmd->type = cmd->type;
              break;

            default:  
              OOTRACEERR1("Error: Unknown command type\n");
               
             
         }

         dListRemove(&gStkCmdList, pNode);
         memFreePtr(&gCmdCtxt, pNode); /* Release memory */
         if(cmd->param1) memFreePtr(&gCmdCtxt, cmd->param1);
         if(cmd->param2) memFreePtr(&gCmdCtxt, cmd->param2);
         if(cmd->param3) memFreePtr(&gCmdCtxt, cmd->param3);
         memFreePtr (&gCmdCtxt, cmd);
      }
   }
   ooMutexReleaseCmdMutex(); /* Release lock */

   return OO_OK;
}
