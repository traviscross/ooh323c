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

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "ootypes.h"
#include "ootrace.h"
#include "ooCommon.h"
#include "ooCapability.h"
#include "ooq931.h"
#include "ooh245.h"
#include "ooh323ep.h"

/** Global endpoint structure */
extern OOH323EndPoint gH323ep;

static OOUINT32 gs_traceLevel = TRACELVL;
static OOBOOL   gs_printTime = TRUE;

static void logDateTime ()
{
   char timeString[100];
   char currtime[3];
   static int lasttime=25;
   int printDate =0;

#ifdef _WIN32
   SYSTEMTIME systemTime;
   GetLocalTime(&systemTime);
   GetTimeFormat(LOCALE_SYSTEM_DEFAULT,0, &systemTime, "HH':'mm':'ss",
                                                              timeString, 100);
   GetTimeFormat(LOCALE_SYSTEM_DEFAULT,0, &systemTime, "H", currtime, 3);
   if(lasttime> atoi(currtime))
      printDate=1;
   lasttime = atoi(currtime);

   if (0 == gH323ep.fptraceFile) {
      gH323ep.fptraceFile = stdout;
   }
   if(printDate)
   {
      printDate = 0;
      fprintf(gH323ep.fptraceFile, "---------Date %d/%d/%d---------\n",
                      systemTime.wMonth, systemTime.wDay, systemTime.wYear);
   }
   if (gs_printTime) {
      fprintf (gH323ep.fptraceFile, "%s:%03d  ", timeString,
               systemTime.wMilliseconds);
   }
#else
   struct tm *ptime;
   char dateString[10];
   time_t t = time(NULL);
   ptime = localtime(&t);
   strftime(timeString, 100, "%H:%M:%S", ptime);
   strftime(currtime, 3, "%H", ptime);
   if(lasttime>atoi(currtime))
       printDate = 1;
   lasttime = atoi(currtime);

   if (0 == gH323ep.fptraceFile) {
      gH323ep.fptraceFile = stdout;
   }
   if (printDate)
   {
      printDate = 0;
      strftime(dateString, 10, "%D", ptime);
      fprintf(gH323ep.fptraceFile, "---------Date %s---------\n",
              dateString);
   }
   if (gs_printTime) {
      struct timeval systemTime;
      gettimeofday(&systemTime, NULL);
      fprintf (gH323ep.fptraceFile, "%s:%03ld  ", timeString,
               systemTime.tv_usec/1000);
   }
#endif
}

void ooSetTraceThreshold(OOUINT32 traceLevel)
{
   gs_traceLevel = traceLevel;
}

void ooTrace (OOUINT32 traceLevel, const char * fmtspec, ...)
{
   va_list arglist;
   if (traceLevel > gs_traceLevel) return;

   logDateTime();

   va_start (arglist, fmtspec);

   vfprintf (gH323ep.fptraceFile, fmtspec, arglist);

   va_end (arglist);

   fflush (gH323ep.fptraceFile);

   gs_printTime = (OOBOOL)(0 != strchr(fmtspec, '\n'));
}

void ooTraceLogMessage(const char * logMessage)
{
   logDateTime();

   fprintf (gH323ep.fptraceFile, "%s", logMessage);

   fflush (gH323ep.fptraceFile);

   gs_printTime = (OOBOOL)(0 != strchr(logMessage, '\n'));
}

int ooLogAsn1Error(int stat, const char * fname, int lno)
{
   OOTRACEERR4("Asn1Error: %d at %s:%d\n", stat, fname, lno);
   return stat;
}

