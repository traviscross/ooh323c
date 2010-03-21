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
#include <stdlib.h>
#include <string.h>
#include "ooUtils.h"
#include "ooCommon.h"

const char* ooUtilsGetText (OOUINT32 idx, const char** table, size_t tabsiz)
{
   return (idx < tabsiz) ? table[idx] : "?";
}

OOBOOL ooUtilsIsStrEmpty (const char* str)
{
   return (str == NULL || *str =='\0');
}


OOBOOL ooIsDialedDigit (const char* str)
{
   if(str == NULL || *str =='\0') { return FALSE; }
   while(*str != '\0')
   {
      if(!isdigit(*str) &&
         *str != '#' && *str != '*' && *str != ',') { return FALSE; }
      str++;
   }
   return TRUE;
}

OOINT32 lookupEnum
(const char* strValue, size_t strValueSize,
 const OOEnumItem enumTable[], OOUINT16 enumTableSize)
{
   size_t lower = 0;
   size_t upper = enumTableSize - 1;
   size_t middle;
   int    cmpRes;

   if (strValueSize == (size_t)-1) {
      strValueSize = strlen (strValue);
   }

   while (lower < upper && upper != (size_t)-1) {
      middle = (lower + upper)/2;

      cmpRes = strncmp (enumTable[middle].name, strValue, strValueSize);

      if (cmpRes == 0)
         cmpRes = (int)enumTable[middle].namelen - (int)strValueSize;

      if (cmpRes == 0) { /* equal */
         return (int)middle;
      }
      if (cmpRes < 0)
         lower = middle+1;
      else
         upper = middle-1;
   }

   if (lower == upper && (size_t)enumTable[lower].namelen == strValueSize &&
       strncmp (enumTable[lower].name, strValue, strValueSize) == 0) {
      return (int)lower;
   }

   return ASN_E_INVENUM;
}

int ooUtilsTextToBool (const char* str, OOBOOL* pbool)
{
   if (0 == pbool) return OO_FAILED;

   if (!strcasecmp (str, "true") ||
       !strcasecmp (str, "yes")  ||
       !strcasecmp (str, "1")) {
      *pbool = TRUE;
      return OO_OK;
   }
   else if (!strcasecmp (str, "false") ||
            !strcasecmp (str, "no")  ||
            !strcasecmp (str, "0")) {
      *pbool = FALSE;
      return OO_OK;
   }
   else {
      return OO_FAILED;
   }
}

