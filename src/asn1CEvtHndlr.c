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

/* asn1CEvtHndlr.c - this file contains implementations of the static
 methods necessary to register new event handlers..
*/
#include "asn1CEvtHndlr.h"
/**
 * addEventHandler : This method adds an event handler object to
 * the current list within the specified context.
 * @param pCtxt  Context to which event handler has to be added
 * @param pHandler Pointer to event handler.
 *
 * @return None
 */
void rtAddEventHandler (OOCTXT* pCtxt,
                        Asn1NamedCEventHandler* pHandler)
{
   struct _SListNode * temp=NULL;
   if (!sListFind (&pCtxt->evtHndlrList, pHandler))
   {
      sListInit(&pCtxt->evtHndlrList);
      temp = sListAppend (&pCtxt->evtHndlrList, pHandler);
   }
}

/**
 * removeEventHandler : This static method removes an event handler object
 * from the current list within the specified context.
 * @param pCtxt    Context from which event handler has to be removed.
 * @param pHandler Pointer to event handler.
 *
 * @return None
 */
void rtRemoveEventHandler
   (OOCTXT* pCtxt, Asn1NamedCEventHandler* pHandler)
{
   sListRemove (&pCtxt->evtHndlrList, pHandler);
}

/* invoke methods: These methods loop through the list of event handler
 objects and invoke the appropriate method in each object..
*/
void rtInvokeStartElement (OOCTXT* pCtxt, const char* name, int index)
{
   Asn1NamedCEventHandler* pHandler;
   SListNode* pListNode;
   pListNode = pCtxt->evtHndlrList.head;
  
   while (pListNode) {
         
      pHandler = (Asn1NamedCEventHandler*) pListNode->data;
          pHandler->startElement (name, index);
          pListNode = pListNode->next;
   }
}

void rtInvokeEndElement (OOCTXT* pCtxt, const char* name, int index)
{
   Asn1NamedCEventHandler* pHandler;
   SListNode* pListNode = pCtxt->evtHndlrList.head;

   while (pListNode) {
      pHandler = (Asn1NamedCEventHandler*) pListNode->data;
      pHandler->endElement (name, index);
      pListNode = pListNode->next;
   }
}

void rtInvokeBoolValue (OOCTXT* pCtxt, ASN1BOOL value)
{
   Asn1NamedCEventHandler* pHandler;
   SListNode* pListNode = pCtxt->evtHndlrList.head;

   while (pListNode) {
      pHandler = (Asn1NamedCEventHandler*) pListNode->data;
      pHandler->boolValue (value);
      pListNode = pListNode->next;
   }
}

void rtInvokeIntValue (OOCTXT* pCtxt, ASN1INT value)
{
   Asn1NamedCEventHandler* pHandler;
   SListNode* pListNode = pCtxt->evtHndlrList.head;

   while (pListNode) {
      pHandler = (Asn1NamedCEventHandler*) pListNode->data;
      pHandler->intValue (value);
      pListNode = pListNode->next;
   }
}

void rtInvokeUIntValue (OOCTXT* pCtxt, ASN1UINT value)
{
   Asn1NamedCEventHandler* pHandler;
   SListNode* pListNode = pCtxt->evtHndlrList.head;

   while (pListNode) {
      pHandler = (Asn1NamedCEventHandler*) pListNode->data;
      pHandler->uIntValue (value);
      pListNode = pListNode->next;
   }
}

void rtInvokeBitStrValue (OOCTXT* pCtxt, ASN1UINT numbits,
                          const ASN1OCTET* data)
{
   Asn1NamedCEventHandler* pHandler;
   SListNode* pListNode = pCtxt->evtHndlrList.head;

   while (pListNode) {
      pHandler = (Asn1NamedCEventHandler*) pListNode->data;
      pHandler->bitStrValue (numbits, data);
      pListNode = pListNode->next;
   }
}

void rtInvokeOctStrValue (OOCTXT* pCtxt, ASN1UINT numocts,
                          const ASN1OCTET* data)
{
   Asn1NamedCEventHandler* pHandler;
   SListNode* pListNode = pCtxt->evtHndlrList.head;

   while (pListNode) {
      pHandler = (Asn1NamedCEventHandler*) pListNode->data;
      pHandler->octStrValue (numocts, data);
      pListNode = pListNode->next;
   }
}

void rtInvokeCharStrValue (OOCTXT* pCtxt, const char* value)
{
   Asn1NamedCEventHandler* pHandler;
   SListNode* pListNode = pCtxt->evtHndlrList.head;

   while (pListNode) {
      pHandler = (Asn1NamedCEventHandler*) pListNode->data;
      pHandler->charStrValue (value);
      pListNode = pListNode->next;
   }
}

void rtInvokeCharStr16BitValue (OOCTXT* pCtxt, ASN1UINT nchars,
                                ASN116BITCHAR* data)
{
   Asn1NamedCEventHandler* pHandler;
   SListNode* pListNode = pCtxt->evtHndlrList.head;
   while (pListNode) {
      pHandler = (Asn1NamedCEventHandler*) pListNode->data;
      pHandler->charStrValue16Bit (nchars, data);
      pListNode = pListNode->next;
   }
}

void rtInvokeCharStr32BitValue (OOCTXT* pCtxt, ASN1UINT nchars,
                                ASN132BITCHAR* data)
{
   Asn1NamedCEventHandler* pHandler;
   SListNode* pListNode = pCtxt->evtHndlrList.head;

   while (pListNode) {
      pHandler = (Asn1NamedCEventHandler*) pListNode->data;
      pHandler->charStrValue32Bit (nchars, data);
      pListNode = pListNode->next;
   }
}

void rtInvokeNullValue (OOCTXT* pCtxt)
{
   Asn1NamedCEventHandler* pHandler;
   SListNode* pListNode = pCtxt->evtHndlrList.head;

   while (pListNode) {
      pHandler = (Asn1NamedCEventHandler*) pListNode->data;
      pHandler->nullValue ();
      pListNode = pListNode->next;
   }
}

void rtInvokeOidValue (OOCTXT* pCtxt, ASN1UINT numSubIds, ASN1UINT* pSubIds)
{
   Asn1NamedCEventHandler* pHandler;
   SListNode* pListNode = pCtxt->evtHndlrList.head;

   while (pListNode) {
      pHandler = (Asn1NamedCEventHandler*) pListNode->data;
      pHandler->oidValue (numSubIds, pSubIds);
      pListNode = pListNode->next;
   }
}

void rtInvokeRealValue (OOCTXT* pCtxt, double value)
{
   Asn1NamedCEventHandler* pHandler;
   SListNode* pListNode = pCtxt->evtHndlrList.head;

   while (pListNode) {
      pHandler = (Asn1NamedCEventHandler*) pListNode->data;
      pHandler->realValue (value);
      pListNode = pListNode->next;
   }
}

void rtInvokeEnumValue (OOCTXT* pCtxt, ASN1UINT value)
{
   Asn1NamedCEventHandler* pHandler;
   SListNode* pListNode = pCtxt->evtHndlrList.head;

   while (pListNode) {
      pHandler = (Asn1NamedCEventHandler*) pListNode->data;
      pHandler->enumValue (value);
      pListNode = pListNode->next;
   }
}

void rtInvokeOpenTypeValue (OOCTXT* pCtxt, ASN1UINT numocts,
                            const ASN1OCTET* data)
{
   Asn1NamedCEventHandler* pHandler;
   SListNode* pListNode = pCtxt->evtHndlrList.head;

   while (pListNode) {
      pHandler = (Asn1NamedCEventHandler*) pListNode->data;
      pHandler->openTypeValue (numocts, data);
      pListNode = pListNode->next;
   }
}

