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

#include "ooasn1.h"

void dListInit (DList* pList)
{
   if (pList) {
      pList->count = 0;
      pList->head = (DListNode*) 0;
      pList->tail = (DListNode*) 0;
   }
}

DListNode* dListAppend (OOCTXT* pctxt, DList* pList, void* pData)
{
   DListNode* pListNode = (DListNode*)
      ASN1MALLOC (pctxt, sizeof(DListNode));

   if (0 != pListNode) {
      pListNode->data = pData;
      pListNode->next = (DListNode*) 0;
      if (0 != pList->tail) {
         pList->tail->next = pListNode;
         pListNode->prev = pList->tail;
      }
      if (0 == pList->head) {
         pList->head = pListNode;
         pListNode->prev = (DListNode*) 0;
      }
      pList->tail = pListNode;
      pList->count++;
   }

   return pListNode;
}

DListNode* dListAppendNode (OOCTXT* pctxt, DList* pList, void* pData)
{
   DListNode* pListNode =
      (DListNode*) (((char*)pData) - sizeof(DListNode));

   if (0 != pListNode) {
      pListNode->data = pData;
      pListNode->next = (DListNode*) 0;
      if (0 != pList->tail) {
         pList->tail->next = pListNode;
         pListNode->prev = pList->tail;
      }
      if (0 == pList->head) {
         pList->head = pListNode;
         pListNode->prev = (DListNode*) 0;
      }
      pList->tail = pListNode;
      pList->count++;
   }

   return pListNode;
}

/* Free all nodes, but not the data */
void dListFreeNodes (OOCTXT* pctxt, DList* pList)
{
   DListNode* pNode, *pNextNode;

   for (pNode = pList->head; pNode != 0; pNode = pNextNode) {
      pNextNode = pNode->next;
      ASN1MEMFREEPTR (pctxt, pNode);
   }
   pList->count = 0;
   pList->head = pList->tail = 0;
}

/* Free all nodes and their data */
void dListFreeAll (OOCTXT* pctxt, DList* pList)
{
   DListNode* pNode, *pNextNode;

   for (pNode = pList->head; pNode != 0; pNode = pNextNode) {
      pNextNode = pNode->next;
     
      ASN1MEMFREEPTR (pctxt, pNode->data);
      ASN1MEMFREEPTR (pctxt, pNode);
   }
   pList->count = 0;
   pList->head = pList->tail = 0;
}

/* Remove node from list. Node is not freeing */
void dListRemove (DList* pList, DListNode* node)
{
   if(node->next != 0) {
      node->next->prev = node->prev;
   }
   else { /* tail */
      pList->tail = node->prev;
   }
   if(node->prev != 0) {
      node->prev->next = node->next;
   }
   else { /* head */
      pList->head = node->next;
   }
   pList->count--;
}

DListNode* dListFindByIndex (DList* pList, int index)
{
   DListNode* curNode;
   int i;

   if(index >= (int)pList->count) return 0;
   for(i = 0, curNode = pList->head; i < index && curNode != 0; i++) {
      curNode = curNode->next;
   }
   return curNode;
}
