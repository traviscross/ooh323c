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
#include "oo.h"
#include "ooq931.h"
#include "ooh245.h"
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

void ooTrace( const char * fmtspec, ...)
{
   va_list arglist;
   char timeString[100];
   char logMessage[MAXLOGMSGLEN];
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

   va_start (arglist, fmtspec);
   memset(logMessage, 0, MAXLOGMSGLEN);
   vsprintf(logMessage, fmtspec, arglist);
  
     
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

   va_end(arglist);
}

int ooLogAsn1Error(int stat, const char * fname, int lno)
{
   OOTRACEERR4("Asn1Error: %d at %s:%d\n", stat, fname, lno);
   return stat;
}
