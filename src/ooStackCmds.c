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

/** Global endpoint structure */
extern ooEndPoint gH323ep;

int ooMakeCall(char* dest, char* callToken)
{
   ooCommand *cmd;
#ifdef _WIN32
   EnterCriticalSection(&gCmdMutex);
#else
   pthread_mutex_lock(&gCmdMutex);
#endif
   cmd = (ooCommand*)ASN1MALLOC(&gH323ep.ctxt, sizeof(ooCommand));
   if(!cmd)
   {
      OOTRACEERR1("Error:Allocating memory for command structure - "
                  "MakeCall\n");
      return OO_FAILED;
   }
   memset(cmd, 0, sizeof(ooCommand));
   cmd->type = OO_CMD_MAKECALL;
   cmd->param1 = (void*) ASN1MALLOC(&gH323ep.ctxt, strlen(dest)+1);
   if(!cmd->param1)
   {
      OOTRACEERR1("ERROR:Allocating memory for cmd param1 - MakeCall\n");
      return OO_FAILED;
   }
   memset(cmd->param1, 0, strlen(dest)+1);
   strcpy((char*)cmd->param1, dest);
   /* Generate call token*/
   if(!callToken)
   {
      OOTRACEERR1("ERROR:Invalid 'callToken' parameter to 'ooMakeCall'\n");
      return OO_FAILED;
   }

   sprintf(callToken, "ooh323c_%d", gCurCallToken++);
  
   if(gCurCallToken > gCallTokenMax)
      gCurCallToken = gCallTokenBase;

   cmd->param2 = (void*) ASN1MALLOC(&gH323ep.ctxt, strlen(callToken)+1);
   if(!cmd->param2)
   {
      OOTRACEERR1("ERROR:Allocating memory for cmd param2 - MakeCall\n");
      return OO_FAILED;
   }
  
   strcpy((char*)cmd->param2, callToken);

   dListAppend(&gH323ep.ctxt, &gH323ep.stkCmdList, cmd);

#ifdef _WIN32
   LeaveCriticalSection(&gCmdMutex);
#else
   pthread_mutex_unlock(&gCmdMutex);
#endif

   return OO_OK;
}

int ooMakeCall_3(char *dest, char*callToken, ASN1USINT * callRef)
{
  ooCommand *cmd;
#ifdef _WIN32
   EnterCriticalSection(&gCmdMutex);
#else
   pthread_mutex_lock(&gCmdMutex);
#endif
   cmd = (ooCommand*)ASN1MALLOC(&gH323ep.ctxt, sizeof(ooCommand));
   if(!cmd)
   {
      OOTRACEERR1("Error:Allocating memory for command structure - "
                  "MakeCall\n");
      return OO_FAILED;
   }
   cmd->type = OO_CMD_MAKECALL_3;
   cmd->param1 = (void*) ASN1MALLOC(&gH323ep.ctxt, strlen(dest)+1);
   if(!cmd->param1)
   {
      OOTRACEERR1("ERROR:Allocating memory for cmd param1 - MakeCall\n");
      return OO_FAILED;
   }
   memset(cmd->param1, 0, strlen(dest)+1);
   strcpy((char*)cmd->param1, dest);
   /* Generate call token*/
   if(!callToken)
   {
      OOTRACEERR1("ERROR:Invalid 'callToken' parameter to 'ooMakeCall'\n");
      return OO_FAILED;
   }

   sprintf(callToken, "ooh323c_%d", gCurCallToken++);
  
   if(gCurCallToken > gCallTokenMax)
      gCurCallToken = gCallTokenBase;

   cmd->param2 = (void*) ASN1MALLOC(&gH323ep.ctxt, strlen(callToken)+1);
   if(!cmd->param2)
   {
      OOTRACEERR1("ERROR:Allocating memory for cmd param2 - MakeCall\n");
      return OO_FAILED;
   }
  
   strcpy((char*)cmd->param2, callToken);

   *callRef = ooGenerateCallReference();
   cmd->param3 = (void*) ASN1MALLOC(&gH323ep.ctxt, sizeof(ASN1USINT));
   if(!cmd->param3)
   {
      OOTRACEERR1("ERROR:Allocating memory for cmd param3 - MakeCall\n");
      return OO_FAILED;
   }
   *((ASN1USINT*)cmd->param3) = *callRef;
   dListAppend(&gH323ep.ctxt, &gH323ep.stkCmdList, cmd);

#ifdef _WIN32
   LeaveCriticalSection(&gCmdMutex);
#else
   pthread_mutex_unlock(&gCmdMutex);
#endif

   return OO_OK;
}

int ooAnswerCall(char *callToken)
{
   ooCommand *cmd;
#ifdef _WIN32
   EnterCriticalSection(&gCmdMutex);
#else
   pthread_mutex_lock(&gCmdMutex);
#endif
   cmd = (ooCommand*)ASN1MALLOC(&gH323ep.ctxt, sizeof(ooCommand));
   if(!cmd)
   {
      OOTRACEERR1("Error:Allocating memory for command structure - AnswerCall\n");
      return OO_FAILED;
   }
   memset(cmd, 0, sizeof(ooCommand));
   cmd->type = OO_CMD_ANSCALL;

   cmd->param1 = (void*) ASN1MALLOC(&gH323ep.ctxt, strlen(callToken)+1);
   if(!cmd->param1)
   {
      OOTRACEERR1("ERROR:Allocating memory for cmd param1 - AnsCall\n");
      return OO_FAILED;
   }
   strcpy((char*)cmd->param1, callToken);
  
   dListAppend(&gH323ep.ctxt, &gH323ep.stkCmdList, cmd);
  

#ifdef _WIN32
   LeaveCriticalSection(&gCmdMutex);
#else
   pthread_mutex_unlock(&gCmdMutex);
#endif

   return OO_OK;
}

int ooRejectCall(char* callToken, int cause)
{
   ooCommand *cmd;
#ifdef _WIN32
   EnterCriticalSection(&gCmdMutex);
#else
   pthread_mutex_lock(&gCmdMutex);
#endif
   cmd = (ooCommand*)ASN1MALLOC(&gH323ep.ctxt, sizeof(ooCommand));
   if(!cmd)
   {
      OOTRACEERR1("Error:Allocating memory for command structure - RejectCall\n");
      return OO_FAILED;
   }
   memset(cmd, 0, sizeof(ooCommand));
   cmd->type = OO_CMD_REJECTCALL;

   cmd->param1 = (void*) ASN1MALLOC(&gH323ep.ctxt, strlen(callToken)+1);
   if(!cmd->param1)
   {
      OOTRACEERR1("ERROR:Allocating memory for cmd param1 - RejectCall\n");
      return OO_FAILED;
   }
   strcpy((char*)cmd->param1, callToken);
  
   dListAppend(&gH323ep.ctxt, &gH323ep.stkCmdList, cmd);
  

#ifdef _WIN32
   LeaveCriticalSection(&gCmdMutex);
#else
   pthread_mutex_unlock(&gCmdMutex);
#endif

   return OO_OK;
}


int ooHangCall(char * callToken)
{
   ooCommand *cmd;
#ifdef _WIN32
   EnterCriticalSection(&gCmdMutex);
#else
   pthread_mutex_lock(&gCmdMutex);
#endif
   cmd = (ooCommand*)ASN1MALLOC(&gH323ep.ctxt, sizeof(ooCommand));
   if(!cmd)
   {
      OOTRACEERR1("Error:Allocating memory for command structure - HangCall\n");
      return OO_FAILED;
   }

   memset(cmd, 0, sizeof(ooCommand));
   cmd->type = OO_CMD_HANGCALL;
   cmd->param1 = (void*) ASN1MALLOC(&gH323ep.ctxt, strlen(callToken)+1);
   if(!cmd->param1)
   {
      OOTRACEERR1("ERROR:Allocating memory for cmd param1 - HangCall\n");
      return OO_FAILED;
   }
   strcpy((char*)cmd->param1, callToken);
  
   dListAppend(&gH323ep.ctxt, &gH323ep.stkCmdList, cmd);
  

#ifdef _WIN32
   LeaveCriticalSection(&gCmdMutex);
#else
   pthread_mutex_unlock(&gCmdMutex);
#endif

   return OO_OK;
}

int ooStopMonitor()
{
   ooCommand *cmd;
#ifdef _WIN32
   EnterCriticalSection(&gCmdMutex);
#else
   pthread_mutex_lock(&gCmdMutex);
#endif
   cmd = (ooCommand*)ASN1MALLOC(&gH323ep.ctxt, sizeof(ooCommand));
   if(!cmd)
   {
      OOTRACEERR1("Error:Allocating memory for command structure - StopMonitor\n");
      return OO_FAILED;
   }
   memset(cmd, 0, sizeof(ooCommand));
   cmd->type = OO_CMD_STOPMONITOR;
  
   dListAppend (&gH323ep.ctxt, &gH323ep.stkCmdList, cmd);
  
#ifdef _WIN32
   LeaveCriticalSection(&gCmdMutex);
#else
   pthread_mutex_unlock(&gCmdMutex);
#endif
   return OO_OK;
}

