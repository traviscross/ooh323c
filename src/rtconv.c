/*
 * Copyright (C) 1997-2005 by Objective Systems, Inc.
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

/* Run-time converting utility functions */

#include <stdarg.h>
#include <stdlib.h>
#include "ooasn1.h"
#include "rtctype.h"

/* 'toString' functions: these provide a stringified representation in  */
/* ASN.1 value notation format for each of the primitive types..        */

const char* rtBoolToString (ASN1BOOL value)
{
   return value ? "TRUE" : "FALSE";
}

const char* rtIntToString (ASN1INT value, char* buffer, size_t bufsiz)
{
   char lbuf[32];
   sprintf (lbuf, "%d", value);
   bufsiz = (sizeof (lbuf) < bufsiz) ? sizeof (lbuf) : bufsiz;
   strncpy (buffer, lbuf, bufsiz);
   buffer[bufsiz-1] = '\0';
   return buffer;
}

const char* rtUIntToString
   (ASN1UINT value, char* buffer, size_t bufsiz)
{
   char lbuf[32];
   sprintf (lbuf, "%u", value);
   bufsiz = (sizeof (lbuf) < bufsiz) ? sizeof (lbuf) : bufsiz;
   strncpy (buffer, lbuf, bufsiz);
   buffer[bufsiz-1] = '\0';
   return buffer;
}
/*
const char* rtUInt64ToString
   (ASN1UINT64 value, char* buffer, size_t bufsiz)
{
   char lbuf[32];
   sprintf (lbuf, ASN1UINT64FMT, value);
   bufsiz = (sizeof (lbuf) < bufsiz) ? sizeof (lbuf) : bufsiz;
   strncpy (buffer, lbuf, bufsiz);
   buffer[bufsiz-1] = '\0';
   return buffer;
}
*/
const char* rtBitStrToString (ASN1UINT numbits, const ASN1OCTET* data,
                                   char* buffer, size_t bufsiz)
{
   size_t i;
   unsigned char mask = 0x80;

   if (bufsiz > 0) {
      buffer[0] = '\'';
      for (i = 0; i < numbits; i++) {
         if (i < bufsiz - 1) {
            buffer[i+1] = (char) (((data[i/8] & mask) != 0) ? '1' : '0');
            mask >>= 1;
            if (0 == mask) mask = 0x80;
         }
         else break;
      }
     i++;
      if (i < bufsiz - 1) buffer[i++] = '\'';
      if (i < bufsiz - 1) buffer[i++] = 'B';
      if (i < bufsiz - 1) buffer[i] = '\0';
      else buffer[bufsiz - 1] = '\0';
   }

   return buffer;
}

const char* rtOctStrToString (ASN1UINT numocts, const ASN1OCTET* data,
                                   char* buffer, size_t bufsiz)
{
   size_t i;
   char lbuf[4];

   if (bufsiz > 0) {
      buffer[0] = '\'';
      if (bufsiz > 1) buffer[1] = '\0';
      for (i = 0; i < numocts; i++) {
         if (i < bufsiz - 1) {
            sprintf (lbuf, "%02x", data[i]);
            strcat (&buffer[(i*2)+1], lbuf);
         }
         else break;
      }
     i = i*2 + 1;
      if (i < bufsiz - 1) buffer[i++] = '\'';
      if (i < bufsiz - 1) buffer[i++] = 'H';
      if (i < bufsiz - 1) buffer[i] = '\0';
      else buffer[bufsiz - 1] = '\0';
   }

   return buffer;
}

const char* rtOIDToString (ASN1UINT numids, ASN1UINT* data,
                                char* buffer, size_t bufsiz)
{
   size_t ui, cnt;
   char lbuf[16];

   if (bufsiz < 6) return 0;
   strcpy (buffer, "{ ");
   cnt = 2;

   for (ui = 0; ui < numids; ui++) {
      sprintf (lbuf, "%u ", data[ui]);
      if ((strlen(lbuf) + cnt) < bufsiz) {
         strcat (buffer, lbuf);
         cnt += strlen(lbuf);
      }
      else return buffer;
   }

   if ((cnt + 1) < bufsiz) strcat (buffer, "}");

   return buffer;
}
/*
const char* rtOID64ToString (ASN1UINT numids, ASN1UINT64* data,
                                  char* buffer, size_t bufsiz)
{
   size_t ui, cnt;
   char lbuf[32];

   if (bufsiz < 6) return 0;
   strcpy (buffer, "{ ");
   cnt = 2;

   for (ui = 0; ui < numids; ui++) {
      sprintf (lbuf, ASN1UINT64FMT" ", data[ui]);
      if ((strlen(lbuf) + cnt) < bufsiz) {
         strcat (buffer, lbuf);
         cnt += strlen(lbuf);
      }
      else return buffer;
   }

   if ((cnt + 1) < bufsiz) strcat (buffer, "}");

   return buffer;
}
*/
const char* rtTagToString (ASN1TAG tag, char* buffer, size_t bufsiz)
{
   ASN1UINT idmask = 0xFFFFFFFF >> (((sizeof(ASN1TAG) - 2) * 8) + 3);
   ASN1UINT idcode = ((ASN1UINT)tag) & idmask;
   ASN1TAG  tclass = tag & TM_PRIV;
   char lbuf[32];

   switch (tclass) {
   case TM_UNIV: sprintf (lbuf, "[UNIVERSAL %d]", idcode); break;
   case TM_APPL: sprintf (lbuf, "[APPLICATION %d]", idcode); break;
   case TM_CTXT: sprintf (lbuf, "[%d]", idcode); break;
   case TM_PRIV: sprintf (lbuf, "[PRIVATE %d]", idcode); break;
   default:      sprintf (lbuf, "[??? %d]", idcode); break;
   }

   bufsiz = (sizeof (lbuf) < bufsiz) ? sizeof (lbuf) : bufsiz;
   strncpy (buffer, lbuf, bufsiz);
   buffer[bufsiz-1] = '\0';

   return buffer;
}

/* Returns number of octets are occupied by BCD string in the bcdStr. */
size_t rtStringToBCD (const char* str, ASN1OCTET* bcdStr, size_t bufsiz)
{
   ASN1OCTET         elem = 0, sym;
   register size_t   i, maxDigits = bufsiz * 2;
   int               isDigit;
   const char*  pstr = str;
   ASN1OCTET         *pbcdStr = bcdStr;

   if (str == NULL || bcdStr == NULL || !bufsiz) return 0;
   memset (bcdStr, 0xFF, bufsiz);
   for (i = 0; *pstr != 0 && i < maxDigits; pstr++, i++) {
      sym = (ASN1OCTET) OS_TOUPPER (*pstr);
      isDigit = OS_ISDIGIT(sym);
      if (!isDigit && (sym < 'A' || sym >= 'F'))
         break;
     
      if (i & 1) {  /* non-parity */
         elem |= (((ASN1OCTET) (isDigit ? sym - '0' : sym - 'A' + 10)) << 4);
         *pbcdStr++ = elem;
      }
      else {
         elem = (ASN1OCTET) (isDigit ? sym - '0' : sym - 'A' + 10);
      }
   }
   if (i & 1) { /* length is non-parity - write last octet */
      *pbcdStr = (ASN1OCTET) (elem | 0xF0);
   }

   return (i + 1) / 2;
}

const char* rtBCDToString (ASN1UINT numocts, const ASN1OCTET* data,
                                char* buffer, size_t bufsiz)
{
   size_t dstByteIndex = 0, srcByteIndex = 0, maxDigits = numocts * 2;
   register ASN1UINT off;

   if (!data || !buffer || !numocts || !bufsiz) return NULL;
   for (off = 0; dstByteIndex < bufsiz && off < maxDigits; off++) {
      ASN1UINT b;
      if (!(off & 1)) { /* check parity */
         b = data[srcByteIndex] & 0xF;
      }
      else {
         b = data[srcByteIndex++] >> 4;
      }
      if (b == 0xF) /* filler? end of BCD. */
         break;
      buffer[dstByteIndex++] = (char) ((b < 0xA) ? b + '0': b + 'A' - 10);
   }
   if (dstByteIndex < bufsiz)
      buffer[dstByteIndex] = 0;
   return buffer;
}
