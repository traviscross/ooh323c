/*
 * Copyright (C) 2004 by Objective Systems, Inc.
 *
 * This software is furnished under an open source license and may be
 * used and copied only in accordance with the terms of this license.
 * The text of the license may generally be found in the root
 * directory of this installation in the LICENSE.txt file.  It
 * can also be viewed online at the following URL:
 *
 *   http://www.obj-sys.com/open/license.html
 *
 * Any redistributions of this file including modified versions must
 * maintain this copyright notice.
 *
 *****************************************************************************/

#include "ooCommon.h"
#include "ooSock.h"

void ooLog(int level,const char * fmtspec, ...)
{
   va_list arglist;
   char timeString[100];
   char logMessage[MAXLOGMSGLEN];
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
   vsprintf(logMessage, fmtspec, arglist);

#ifdef _WIN32
   if(printDate)
   {
      printDate = 0;
      fprintf(fpLog, "---------Date %d/%d/%d---------\n",
              systemTime.wMonth, systemTime.wDay, systemTime.wYear);
   }
   fprintf(fpLog, "%s:%03d  %s\n", timeString,
           systemTime.wMilliseconds, logMessage);
#else
   if(printDate)
   {
      printDate = 0;
      strftime(dateString, 10, "%D", ptime);
      fprintf(fpLog, "---------Date %s---------\n", dateString);
   }
   fprintf(fpLog, "%s:%03d  %s\n", timeString,
           systemTime.tv_usec/1000, logMessage);
#endif
   fflush(fpLog);
   va_end(arglist);
}

void ooSleep(int milliseconds)
{
   struct timeval timeout;
   timeout.tv_sec = milliseconds/1000;
   timeout.tv_usec = (milliseconds%1000)*1000;
   ooSocketSelect(0, 0, 0, 0, &timeout);
   return;
}
