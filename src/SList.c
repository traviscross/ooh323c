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

#include <stdarg.h>
#include <stdlib.h>
#include "SList.h"

/* Singly-linked list */

void sListInit (SList* pList)
{
   if (pList) {
      pList->count = 0;
      pList->head = (SListNode*) 0;
      pList->tail = (SListNode*) 0;
      pList->pctxt = 0;
   }
}

void sListInitEx (OOCTXT* pctxt, SList* pList)
{
   if (pList) {
      pList->count = 0;
      pList->head = (SListNode*) 0;
      pList->tail = (SListNode*) 0;
      pList->pctxt = pctxt;
   }
}

void sListFree (SList* pList)
{
   if (pList) {
      SListNode *p1, *p2 = pList->head;
      while (p2) {
         p1 = p2;
         p2 = p1->next;
         if (pList->pctxt)
            OSMSGMEMFREEPTR (pList->pctxt, p1);
         else
            ASN1CRTFREE0 (p1);
      }
      sListInit (pList);
   }
}

SList* sListCreate ()
{
   SList* pList = (SList*) ASN1CRTMALLOC0 (sizeof(SList));
   if (pList == NULL) return NULL;
   sListInit (pList);
   return pList;
}

SList* sListCreateEx (OOCTXT* pctxt)
{
   SList* pList = (SList*) OSMSGMALLOC (pctxt, sizeof(SList));
   if (pList == NULL) return NULL;
   sListInitEx (pctxt, pList);
   return pList;
}

SListNode* sListAppend (SList* pList, void* pData)
{
   SListNode* pListNode =
      (SListNode*) ((pList->pctxt) ?
      OSMSGMALLOC (pList->pctxt, sizeof(SListNode)) :
      ASN1CRTMALLOC0 (sizeof(SListNode)));

   if (pListNode) {
      pListNode->data = pData;
      pListNode->next = (SListNode*) 0;
      if (pList->tail) {
         pList->tail->next = pListNode;
      }
      if (!pList->head) {
         pList->head = pListNode;
      }
      pList->tail = pListNode;
      pList->count++;
   }
   return pListNode;
}

ASN1BOOL sListFind (SList* pList, void* pData)
{
   SListNode* pNode = pList->head;
   while (0 != pNode) {
      if (pNode->data == pData) return TRUE;
      else pNode = pNode->next;
   }
   return FALSE;
}

void sListRemove (SList* pList, void* pData)
{
   SListNode* pNode = pList->head;
   SListNode* pPrev = 0;

   while (0 != pNode) {
      if (pNode->data == pData) break;
      else {
         pPrev = pNode;
         pNode = pNode->next;
      }
   }

   if (0 != pNode) {
      if (0 == pPrev) {
         /* head node */
         pList->head = pNode->next;
      }
      else {
         pPrev->next = pNode->next;
      }
      if (pList->tail == pNode) {
         pList->tail = pPrev;
      }
      pList->count--;
      if (pList->pctxt)
         OSMSGMEMFREEPTR (pList->pctxt, pNode);
      else
         ASN1CRTFREE0 (pNode);
   }
}

