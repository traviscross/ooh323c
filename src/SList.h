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
/**
 * @file SList.h
 * Singly Linked list header file
 */

#ifndef _OO_SLIST_H_
#define _OO_SLIST_H_

#include "ooasn1.h"

#define OSMSGMALLOC(pctxt,nbytes) \
memHeapAlloc(&(pctxt)->pMsgMemHeap, nbytes)

#define OSMSGREALLOC(pctxt,pmem,nbytes) \
memHeapRealloc(&(pctxt)->pMsgMemHeap, pmem, nbytes)

#define OSMSGREALLOCARRAY(pctxt,pseqof,type) do {\
if (sizeof(type)*(pseqof)->n < (pseqof)->n) return ASN_E_NOMEM; \
if (((pseqof)->elem = (type*) memHeapRealloc \
(&(pctxt)->pMsgMemHeap, (pseqof)->elem, sizeof(type)*(pseqof)->n)) == 0) \
return ASN_E_NOMEM; \
} while (0)

#define OSMSGMEMFREE(pctxt) \
memHeapFreeAll(&(pctxt)->pMsgMemHeap)

#define OSMSGMEMFREEPTR(pctxt,pmem) \
memHeapFreePtr(&(pctxt)->pMsgMemHeap, pmem)

#define OSMSGMEMRESET(pctxt) \
memHeapReset(&(pctxt)->pMsgMemHeap)

#endif /* _OO_SLIST_H_ */
