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

#include <ctype.h>
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

void ooTrace (char *file, long line, OOUINT32 traceLevel, const char * fmtspec, ...)
{
   char *filename;
   va_list arglist;
   if (traceLevel > gs_traceLevel) return;

   logDateTime();

   if (gs_printTime) {
      filename = strrchr(file, '/');

      if (filename) {
         filename++;
      } else {
         filename = file;
      }

      fprintf (gH323ep.fptraceFile, "%s:%-4ld ", filename, line);
   }

   va_start (arglist, fmtspec);


   vfprintf (gH323ep.fptraceFile, fmtspec, arglist);

   va_end (arglist);

   fflush (gH323ep.fptraceFile);

   gs_printTime = (OOBOOL)(0 != strchr(fmtspec, '\n'));
}

int ooLogAsn1Error(int stat, const char * fname, int lno)
{
   OOTRACEERR4("Asn1Error: %d at %s:%d\n", stat, fname, lno);
   return stat;
}

static char* strTrim (char *s)
{
   size_t i = strlen(s) - 1;
   while (s[i] == ' ') i--;
   s[i+1] = '\0';
   return (s);
}

static void hexDumpToFileEx
(FILE* fp, const OOUCHAR* data, OOUINT32 numocts, int bytesPerUnit)
{
   OOUINT32 i;
   OOUINT32 val;
   OOUCHAR  b, dumpstr = 1;
   char     hexstr[49], ascstr[17], buf[20], *pbuf;
   int      k, unitsPerLine = 16 / bytesPerUnit, ai, shift;

   memset (hexstr, ' ', 48); hexstr[48] = '\0';
   memset (ascstr, ' ', 16); ascstr[16] = '\0';
   ai = 0;

   if (bytesPerUnit > 4) bytesPerUnit = 4;

   for (i = 0; i < numocts / bytesPerUnit; i++)
   {
      pbuf = (char*)buf;
      buf [bytesPerUnit * 2] = 0;
      if (bytesPerUnit == 1) {
         val = *data++;
         shift = 0;
      }
      else if (bytesPerUnit == 2) {
         val = *((OOUINT16*)data);
         data += sizeof (OOUINT16);
         shift = 8;
      }
      else { /* greater than 2 */
         val = *((OOUINT32*)data);
         data += sizeof (OOUINT32);
         shift = (sizeof (OOUINT32) - 1) * 8;
      }
      for (k = 0; k < bytesPerUnit; k++, pbuf += 2, shift -= 8) {
         b = (OOUCHAR)((val >> shift) & 0xFF);
         sprintf (pbuf, "%02x", b);
         ascstr[ai++] = (char) (isprint(b) ? b : '.');
      }
      *pbuf = 0;

      k = i % unitsPerLine * (bytesPerUnit * 2 + 1);
      memcpy (&hexstr[k], buf, bytesPerUnit * 2);

      if ((dumpstr = (OOUCHAR) ((i + 1) % unitsPerLine == 0)) != 0)
      {
         fprintf (fp, "%48s %16s\n", hexstr, ascstr);
         if (i < (numocts - 1)) {
            memset (hexstr, ' ', 48);
            memset (ascstr, ' ', 16);
            ai = 0;
         }
      }
   }

   if (!dumpstr) fprintf (fp, "%48s %s\n", hexstr, strTrim(ascstr));
}

static void hexDumpToFile (FILE* fp, const OOUCHAR* data, OOUINT32 numocts)
{
   hexDumpToFileEx (fp, data, numocts, 1);
}

void ooTraceHexDump
(OOUINT32 traceLevel, const OOUCHAR* data, OOUINT32 numocts)
{
   if (traceLevel <= gs_traceLevel) {
      hexDumpToFile (gH323ep.fptraceFile, data, numocts);
      fflush (gH323ep.fptraceFile);
   }
}

