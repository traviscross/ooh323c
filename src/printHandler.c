/*
 * Copyright (C) 2004 by Objective Systems, Inc.
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
/* This is an implementation of a simple print handler.  It outputs
   the fields of an encoded PER message to stdout in a structured output
   format..
*/
#include <stdlib.h>
#include <malloc.h>
#include "printHandler.h"
#include "ootypes.h"
#include "rtctype.h"
#include "oo.h"

void initializePrintHandler(Asn1NamedCEventHandler *printHandler, char * varname)
{
   printHandler->startElement = &printStartElement;
   printHandler->endElement = &printEndElement;
   printHandler->boolValue = &printBoolValue;
   printHandler->intValue = &printIntValue;
   printHandler->uIntValue = &printuIntValue ;
   printHandler->bitStrValue = &printBitStrValue;
   printHandler->octStrValue = &printOctStrValue;
   printHandler->charStrValue = &printCharStrValue;
   printHandler->charStrValue16Bit = &printCharStr16BitValue ;
   printHandler->charStrValue32Bit = &printCharStr32BitValue;
   printHandler->nullValue = &printNullValue;
   printHandler->oidValue = &printOidValue;
   printHandler->realValue = &printRealValue;
   printHandler->enumValue = &printEnumValue;
   printHandler->openTypeValue = &printOpenTypeValue;
   pVarName = varname;
   OOTRACEINFO2("%s = {\n", pVarName);
   gIndentSpaces += 3;
}

void finishPrint()
{
   OOTRACEINFO1 ("}\n");
   gIndentSpaces -= 3;
   if (gIndentSpaces != 0) {
      OOTRACEINFO1 ("ERROR: unbalanced structure\n");
   }
}

void indent ()
{
   int i=0;
   for (i = 0; i < gIndentSpaces; i++) OOTRACEINFO1 (" ");
}

void printStartElement (const char* name, int index)
{
   indent ();
   OOTRACEINFO1 (name);
   if (index >= 0) OOTRACEINFO2 ("[%d]", index);
   OOTRACEINFO1 (" = {\n");
   gIndentSpaces += 3;
}

void printEndElement (const char* name, int index)
{
   gIndentSpaces -= 3;
   indent ();
   OOTRACEINFO1 ("}\n");
}

void printBoolValue (ASN1BOOL value)
{
   const char* s = value ? "TRUE" : "FALSE";
   indent ();
   OOTRACEINFO2 ("%s\n", s);
}

void printIntValue (ASN1INT value)
{
   indent ();
   OOTRACEINFO2 ("%d\n", value);
}

void printuIntValue (ASN1UINT value)
{
   indent ();
   OOTRACEINFO2 ("%u\n", value);
}

void printBitStrValue (ASN1UINT numbits, const ASN1OCTET* data)
{
   char* s = (char*)malloc(numbits + 8);
   indent ();
   OOTRACEINFO2 ("%s\n", rtBitStrToString (numbits, data, s, numbits+8));
   free(s);
}

void printOctStrValue (ASN1UINT numocts, const ASN1OCTET* data)
{
   int bufsiz = (numocts * 2) + 8;
   char* s = (char*)malloc(bufsiz);
   indent ();
   OOTRACEINFO2 ("%s\n", rtOctStrToString (numocts, data, s, bufsiz));
   free(s);
}

void printCharStrValue (const char* value)
{
   indent ();
   OOTRACEINFO2 ("\"%s\"\n", value);
}

void printCharStr16BitValue (ASN1UINT nchars, ASN116BITCHAR* data)
{
   ASN1UINT ui;
   indent ();

   for (ui = 0; ui < nchars; ui++) {
      if (data[ui] >= 32 && data[ui] <= 127)
         OOTRACEINFO2 ("%c", (char)data[ui]);
      else
         OOTRACEINFO1 ("?");
   }

   OOTRACEINFO1 ("\n");
}

void printCharStr32BitValue (ASN1UINT nchars, ASN132BITCHAR* data)
{
   ASN1UINT ui;
   indent ();

   for ( ui = 0; ui < nchars; ui++) {
      if (data[ui] >= 32 && data[ui] <= 127)
         OOTRACEINFO2 ("%c", (char)data[ui]);
      else
         OOTRACEINFO2 ("\\%d", data[ui]);
   }

   OOTRACEINFO1 ("\n");
}

void printNullValue ()
{
   indent ();
   OOTRACEINFO1 ("NULL\n");
}

void ooPrintOIDValue (ASN1OBJID* pOID)
{
   ASN1UINT ui;
   OOTRACEINFO1 ("{ \n");
   for (ui = 0; ui < pOID->numids; ui++) {
      OOTRACEINFO2 ("%d ", pOID->subid[ui]);
   }
   OOTRACEINFO1 ("}\n");
}

void printOidValue (ASN1UINT numSubIds, ASN1UINT* pSubIds)
{
   ASN1UINT ui;
   ASN1OBJID oid;
   oid.numids = numSubIds;

   for ( ui = 0; ui < numSubIds; ui++)
      oid.subid[ui] = pSubIds[ui];

   indent ();
   ooPrintOIDValue (&oid);
}

void printRealValue (double value)
{
   indent ();
   OOTRACEINFO2 ("%f\n", value);
}

void printEnumValue (ASN1UINT value)
{
   indent ();
   OOTRACEINFO2 ("%u\n", value);
}

void printOpenTypeValue (ASN1UINT numocts, const ASN1OCTET* data)
{
   indent ();
   OOTRACEINFO1 ("< encoded data >\n");
}

