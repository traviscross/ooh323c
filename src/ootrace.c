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

#include "ootypes.h"
#include "ootrace.h"
#include "ooCommon.h"
#include "ooCapability.h"
#include "ooq931.h"
#include "ooh245.h"
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

/** Global endpoint structure */
extern ooEndPoint gH323ep;

static OOUINT32 gs_traceLevel = TRACELVL;

#define OONUMBEROF(items) (sizeof(items)/sizeof(items[0]))

/**
 * Reasons for ending call
 */
static const char* reasonCodeText[] = {
   "Reason unknown",
   "Remote endpoint closed H.225 TCP connection",
   "Remote endpoint closed H.245 TCP connection",
   "Remote Ended call",
   "Local host ended call",
   "Normal End of Call"
};

static const char* callStateText[] = {
   "OO_CALL_CREATED",
   "OO_CALL_WAITING_ADMISSION",
   "OO_CALL_CONNECTING",
   "OO_CALL_CONNECTED",
   "OO_CALL_CLEAR",
   "OO_CALL_CLEAR_RELEASERECVD",
   "OO_CALL_CLEAR_RELEASESENT",
   "OO_CALL_CLEARED"
};

static const char *msgTypeText[]={
   "OOQ931MSG",
   "OOH245MSG",
   "OOSetup",
   "OOCallProceeding",
   "OOAlert",
   "OOConnect",
   "OOReleaseComplete",
   "OOFacility",
   "OOMasterSlaveDetermination",
   "OOMasterSlaveAck",
   "OOMasterSlaveReject",
   "OOMasterSlaveRelease",
   "OOTerminalCapabilitySet",
   "OOTerminalCapabilitySetAck",
   "OOTerminalCapabilitySetReject",
   "OOTerminalCapabilitySetRelease",
   "OOOpenLogicalChannel",
   "OOOpenLogicalChannelAck",
   "OOOpenLogicalChannelReject",
   "OOOpenLogicalChannelRelease",
   "OOOpenLogicalChannelConfirm",
   "OOCloseLogicalChannel",
   "OOCloseLogicalChannelAck",
   "OORequestChannelClose",
   "OORequestChannelCloseAck",
   "OORequestChannelCloseReject",
   "OORequestChannelCloseRelease",
   "OOEndSessionCommand"
};

static const char *capTypes[]={
   "OO_G711ULAW64K",
   "OO_G711ULAW56K",
   "OO_G711ALAW64K",
   "OO_G711ALAW56K",
   "OO_GSMFULLRATE",
   "OO_GSMHALFRATE",
   "OO_GSMENHANCEDFULLRATE"
};

/*DTMF capabilities*/
static const char* dtmfCaps []={
   "OO_CAP_DTMF_RFC2833",
   "OO_CAP_DTMF_Q931",
   "OO_CAP_DTMF_H245"
};

static const char* ooGetText (int idx, const char** table, size_t tabsiz)
{
   return (idx < (int)tabsiz) ? table[idx] : "?";
}

const char* ooGetReasonCodeText (int code)
{
   return ooGetText (code, reasonCodeText, OONUMBEROF(reasonCodeText));
}

const char* ooGetCallStateText (int callState)
{
   return ooGetText (callState, callStateText, OONUMBEROF(callStateText));
}

const char* ooGetMsgTypeText (int msgType)
{
   int idx = msgType - OO_MSGTYPE_MIN;
   return ooGetText (idx, msgTypeText, OONUMBEROF(msgTypeText));
}

void ooSetTraceThreshold(OOUINT32 traceLevel)
{
   gs_traceLevel = traceLevel;
}

void ooTrace(OOUINT32 traceLevel, const char * fmtspec, ...)
{
   va_list arglist;
   char logMessage[MAXLOGMSGLEN];
   if(traceLevel > gs_traceLevel) return;
   va_start (arglist, fmtspec);
   /*   memset(logMessage, 0, MAXLOGMSGLEN);*/
   vsprintf(logMessage, fmtspec, arglist);  
   va_end(arglist);
   ooTraceLogMessage(logMessage);
}

void ooTraceLogMessage(const char * logMessage)
{
   char timeString[100];
   char currtime[3];
   static int lasttime=25;
   int printDate =0;
   static int printTime=1;
  
#ifdef _WIN32
  
   SYSTEMTIME systemTime;
   GetLocalTime(&systemTime);
   GetTimeFormat(LOCALE_SYSTEM_DEFAULT,0, &systemTime, "HH':'mm':'ss", timeString, 100);
   GetTimeFormat(LOCALE_SYSTEM_DEFAULT,0, &systemTime, "H", currtime, 3);
   if(lasttime> atoi(currtime))
      printDate=1;
   lasttime = atoi(currtime);
  
#else
   struct tm *ptime;
   struct timeval systemTime;
   char dateString[10];
   gettimeofday(&systemTime, NULL);
   ptime = (struct tm*)localtime(&systemTime.tv_sec);
   strftime(timeString, 100, "%H:%M:%S", ptime);
   strftime(currtime, 3, "%H", ptime);
   if(lasttime>atoi(currtime))
       printDate = 1;
   lasttime = atoi(currtime);  
#endif

     
#ifdef _WIN32
   if(printDate)
   {
      printDate = 0;
      fprintf(gH323ep.fptraceFile, "---------Date %d/%d/%d---------\n",
                      systemTime.wMonth, systemTime.wDay, systemTime.wYear);
   }
   if(printTime)
     fprintf(gH323ep.fptraceFile, "%s:%03d  %s", timeString,
             systemTime.wMilliseconds, logMessage);
   else
      fprintf(gH323ep.fptraceFile, "%s", logMessage);
  
   fflush(gH323ep.fptraceFile);
#else
   if(printDate)
   {
      printDate = 0;
      strftime(dateString, 10, "%D", ptime);
      fprintf(gH323ep.fptraceFile, "---------Date %s---------\n",
                  dateString);
   }
   if(printTime)
      fprintf(gH323ep.fptraceFile, "%s:%03d  %s", timeString,
                  systemTime.tv_usec/1000, logMessage);
   else
      fprintf(gH323ep.fptraceFile, "%s", logMessage);

   fflush(gH323ep.fptraceFile);
#endif
  
   if(strchr(logMessage, '\n'))
      printTime = 1;
   else
      printTime = 0;

}

int ooLogAsn1Error(int stat, const char * fname, int lno)
{
   OOTRACEERR4("Asn1Error: %d at %s:%d\n", stat, fname, lno);
   return stat;
}

