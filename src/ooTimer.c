/*
 * Copyright (C) 2004-2005 by Objective Systems, Inc.
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

#include "ooDateTime.h"
#include "ooTimer.h"

#define USECS_IN_SECS 1000000
#define NSECS_IN_USECS 1000

#ifndef MICROSEC
#define MICROSEC USECS_IN_SECS
#endif

/*
 * Global timer list (list of OOTimer*)
 */
static DList gs_TimerList;

OOTimer* ooTimerCreate
(OOCTXT* pctxt, OOTimerCbFunc cb, OOUINT32 deltaSecs, void *data,
 OOBOOL reRegister)
{
   OOTimer* pTimer = (OOTimer*) memAlloc (pctxt, sizeof(OOTimer));
   if (0 == pTimer) return 0;

   memset (pTimer, 0, (sizeof(OOTimer)));
   pTimer->timeoutCB = cb;
   pTimer->cbData = data;
   pTimer->reRegister = reRegister;
   pTimer->timeout.tv_sec  = deltaSecs;
   pTimer->timeout.tv_usec = 0;

   /* Compute the absolute time at which this timer should expire */

   ooTimerComputeExpireTime (pTimer);

   /* Insert this timer into the complete list */

   ooTimerInsertEntry (pctxt, pTimer);

   return pTimer;
}

void ooTimerComputeExpireTime (OOTimer* pTimer)
{
   struct timeval tv;
   ooGetTimeOfDay (&tv, 0);

   /* Compute delta time to expiration */

   pTimer->expireTime.tv_usec = tv.tv_usec + pTimer->timeout.tv_usec;
   pTimer->expireTime.tv_sec  = tv.tv_sec  + pTimer->timeout.tv_sec;

   while (pTimer->expireTime.tv_usec >= MICROSEC) {
      pTimer->expireTime.tv_usec -= MICROSEC;
      pTimer->expireTime.tv_sec++;
   }
}

void ooTimerDelete (OOCTXT* pctxt, OOTimer* pTimer)
{
   dListRemove (&gs_TimerList, pTimer);
   memFreePtr (pctxt, pTimer);
}

OOBOOL ooTimerExpired (OOTimer* pTimer)
{
   struct timeval tvstr;
   ooGetTimeOfDay (&tvstr, 0);

   if (tvstr.tv_sec > pTimer->expireTime.tv_sec)
      return TRUE;

   if ((tvstr.tv_sec == pTimer->expireTime.tv_sec) &&
       (tvstr.tv_usec > pTimer->expireTime.tv_usec))
      return TRUE;

   return FALSE;
}

void ooTimerFireExpired (OOCTXT* pctxt)
{
   OOTimer* pTimer;
   int stat;

   while (gs_TimerList.count > 0) {
      pTimer = (OOTimer*) gs_TimerList.head->data;

      if (ooTimerExpired (pTimer)) {
         /*
          * Re-register before calling callback function in case it is
          * a long duration callback.                                  
          */
         if (pTimer->reRegister) ooTimerReset (pctxt, pTimer);

         stat = (*pTimer->timeoutCB)(pTimer, pTimer->cbData);

         if (0 != stat || !pTimer->reRegister) {
            ooTimerDelete (pctxt, pTimer);
         }
      }
      else break;
   }
}

int ooTimerInsertEntry (OOCTXT* pctxt, OOTimer* pTimer)
{
   DListNode* pNode;
   OOTimer* p;
   int i = 0;

   for (pNode = gs_TimerList.head; pNode != 0; pNode = pNode->next) {
      p = (OOTimer*) pNode->data;
      if (pTimer->expireTime.tv_sec  <  p->expireTime.tv_sec) break;
      if (pTimer->expireTime.tv_sec  == p->expireTime.tv_sec &&
          pTimer->expireTime.tv_usec <= p->expireTime.tv_usec) break;
      i++;
   }

   dListInsertBefore (pctxt, &gs_TimerList, pNode, pTimer);

   return i;
}

struct timeval* ooTimerNextTimeout (struct timeval* ptimeout)
{
   OOTimer* ptimer;
   struct timeval tvstr;

   if (gs_TimerList.count == 0) return 0;
   ptimer = (OOTimer*) gs_TimerList.head->data;

   ooGetTimeOfDay (&tvstr, 0);

   ptimeout->tv_sec =
      OOMAX ((int) 0, (int) (ptimer->expireTime.tv_sec - tvstr.tv_sec));  

   ptimeout->tv_usec = ptimer->expireTime.tv_usec - tvstr.tv_usec;

   while (ptimeout->tv_usec < 0) {
      ptimeout->tv_sec--;
      ptimeout->tv_usec += USECS_IN_SECS;
   }

   if (ptimeout->tv_sec < 0)
      ptimeout->tv_sec = ptimeout->tv_usec = 0;

   return (ptimeout);
}

/*
 * Reregister a timer entry.  This function is responsible for moving
 * the current pointer in the timer list to the next element to be
 * processed..
 */
void ooTimerReset (OOCTXT* pctxt, OOTimer* pTimer)
{
   if (pTimer->reRegister) {
      dListRemove (&gs_TimerList, pTimer);
      ooTimerComputeExpireTime (pTimer);
      ooTimerInsertEntry (pctxt, pTimer);
   }
   else
      ooTimerDelete (pctxt, pTimer);
}
