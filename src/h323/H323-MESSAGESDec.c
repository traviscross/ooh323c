/*
 * Copyright (C) 2004-2005 by Objective Systems, Inc.
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
#include "H323-MESSAGES.h"
#include "asn1CEvtHndlr.h"

/**************************************************************/
/*                                                            */
/*  ScreeningIndicator                                        */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225ScreeningIndicator (OOCTXT* pctxt, H225ScreeningIndicator* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT ui;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   if (extbit) {
      stat = decodeSmallNonNegWholeNumber (pctxt, &ui);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      *pvalue = ui;
   }
   else {
      stat = decodeConsUnsigned (pctxt, &ui, 0, 3);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      switch (ui) {
         case 0: *pvalue = userProvidedNotScreened; break;
         case 1: *pvalue = userProvidedVerifiedAndPassed; break;
         case 2: *pvalue = userProvidedVerifiedAndFailed; break;
         case 3: *pvalue = networkProvided; break;
         default: return LOG_ASN1ERR (pctxt, ASN_E_INVENUM);
      }
   }
   rtInvokeUIntValue (pctxt, *pvalue);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  NumberDigits                                              */
/*                                                            */
/**************************************************************/

extern EXTERN const char* gs_H323_MESSAGES_NumberDigits_CharSet;

EXTERN int asn1PD_H225NumberDigits (OOCTXT* pctxt, H225NumberDigits* pvalue)
{
   static Asn1SizeCnst lsize1 = { 0, 1, 128, 0 };
   int stat = ASN_OK;

   stat = addSizeConstraint (pctxt, &lsize1);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   stat = decodeConstrainedStringEx (pctxt, pvalue, gs_H323_MESSAGES_NumberDigits_CharSet, 4, 4, 7);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   rtInvokeCharStrValue (pctxt, *pvalue);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  TBCD_STRING                                               */
/*                                                            */
/**************************************************************/

extern EXTERN const char* gs_H323_MESSAGES_TBCD_STRING_CharSet;

EXTERN int asn1PD_H225TBCD_STRING (OOCTXT* pctxt, H225TBCD_STRING* pvalue)
{
   int stat = ASN_OK;

   stat = decodeConstrainedStringEx (pctxt, pvalue, gs_H323_MESSAGES_TBCD_STRING_CharSet, 4, 4, 7);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   rtInvokeCharStrValue (pctxt, *pvalue);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  GloballyUniqueID                                          */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225GloballyUniqueID (OOCTXT* pctxt, H225GloballyUniqueID* pvalue)
{
   static Asn1SizeCnst lsize1 = { 0, 16, 16, 0 };
   int stat = ASN_OK;

   stat = addSizeConstraint (pctxt, &lsize1);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   stat = decodeOctetString (pctxt,
                          &pvalue->numocts,
                          pvalue->data,
                          sizeof(pvalue->data));
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   rtInvokeOctStrValue (pctxt, pvalue->numocts, pvalue->data);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  ConferenceIdentifier                                      */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225ConferenceIdentifier (OOCTXT* pctxt, H225ConferenceIdentifier* pvalue)
{
   int stat = ASN_OK;

   stat = asn1PD_H225GloballyUniqueID (pctxt, pvalue);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  RequestSeqNum                                             */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225RequestSeqNum (OOCTXT* pctxt, H225RequestSeqNum* pvalue)
{
   int stat = ASN_OK;

   stat = decodeConsUInt16 (pctxt, pvalue, 1U, 65535U);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   rtInvokeUIntValue (pctxt, *pvalue);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  GatekeeperIdentifier                                      */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225GatekeeperIdentifier (OOCTXT* pctxt, H225GatekeeperIdentifier* pvalue)
{
   static Asn1SizeCnst lsize1 = { 0, 1, 128, 0 };
   int stat = ASN_OK;

   stat = addSizeConstraint (pctxt, &lsize1);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   stat = decodeBMPString (pctxt, pvalue, 0);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   rtInvokeCharStr16BitValue (pctxt, pvalue->nchars, pvalue->data);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  BandWidth                                                 */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225BandWidth (OOCTXT* pctxt, H225BandWidth* pvalue)
{
   int stat = ASN_OK;

   stat = decodeConsUnsigned (pctxt, pvalue, 0U, ASN1UINT_MAX);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   rtInvokeUIntValue (pctxt, *pvalue);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  CallReferenceValue                                        */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225CallReferenceValue (OOCTXT* pctxt, H225CallReferenceValue* pvalue)
{
   int stat = ASN_OK;

   stat = decodeConsUInt16 (pctxt, pvalue, 0U, 65535U);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   rtInvokeUIntValue (pctxt, *pvalue);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  EndpointIdentifier                                        */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225EndpointIdentifier (OOCTXT* pctxt, H225EndpointIdentifier* pvalue)
{
   static Asn1SizeCnst lsize1 = { 0, 1, 128, 0 };
   int stat = ASN_OK;

   stat = addSizeConstraint (pctxt, &lsize1);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   stat = decodeBMPString (pctxt, pvalue, 0);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   rtInvokeCharStr16BitValue (pctxt, pvalue->nchars, pvalue->data);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  ProtocolIdentifier                                        */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225ProtocolIdentifier (OOCTXT* pctxt, H225ProtocolIdentifier* pvalue)
{
   int stat = ASN_OK;

   stat = decodeObjectIdentifier (pctxt, pvalue);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   rtInvokeOidValue (pctxt, pvalue->numids, pvalue->subid);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  TimeToLive                                                */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225TimeToLive (OOCTXT* pctxt, H225TimeToLive* pvalue)
{
   int stat = ASN_OK;

   stat = decodeConsUnsigned (pctxt, pvalue, 1U, ASN1UINT_MAX);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   rtInvokeUIntValue (pctxt, *pvalue);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  H248PackagesDescriptor                                    */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225H248PackagesDescriptor (OOCTXT* pctxt, H225H248PackagesDescriptor* pvalue)
{
   int stat = ASN_OK;

   stat = decodeDynOctetString (pctxt, (ASN1DynOctStr*)pvalue);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   rtInvokeOctStrValue (pctxt, pvalue->numocts, pvalue->data);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  H248SignalsDescriptor                                     */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225H248SignalsDescriptor (OOCTXT* pctxt, H225H248SignalsDescriptor* pvalue)
{
   int stat = ASN_OK;

   stat = decodeDynOctetString (pctxt, (ASN1DynOctStr*)pvalue);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   rtInvokeOctStrValue (pctxt, pvalue->numocts, pvalue->data);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  GenericIdentifier                                         */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225GenericIdentifier (OOCTXT* pctxt, H225GenericIdentifier* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT ui;
   ASN1OpenType openType;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   if (!extbit) {
      stat = decodeConsUnsigned (pctxt, &ui, 0, 2);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      else pvalue->t = ui + 1;

      switch (ui) {
         /* standard */
         case 0:
            rtInvokeStartElement (pctxt, "standard", -1);

            /* extension bit */

            stat = DECODEBIT (pctxt, &extbit);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            if (extbit == 0) {
               stat = decodeConsInteger (pctxt, &pvalue->u.standard, 0, 16383);
               if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
            }
            else {
               stat = decodeUnconsInteger (pctxt, &pvalue->u.standard);
               if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
            }
            rtInvokeIntValue (pctxt, pvalue->u.standard);

            rtInvokeEndElement (pctxt, "standard", -1);

            break;

         /* oid */
         case 1:
            rtInvokeStartElement (pctxt, "oid", -1);

            pvalue->u.oid = ALLOC_ASN1ELEM (pctxt, ASN1OBJID);
            if (pvalue->u.oid == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = decodeObjectIdentifier (pctxt, pvalue->u.oid);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
            rtInvokeOidValue (pctxt, pvalue->u.oid->numids, pvalue->u.oid->subid);

            rtInvokeEndElement (pctxt, "oid", -1);

            break;

         /* nonStandard */
         case 2:
            rtInvokeStartElement (pctxt, "nonStandard", -1);

            pvalue->u.nonStandard = ALLOC_ASN1ELEM (pctxt, H225GloballyUniqueID);
            if (pvalue->u.nonStandard == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225GloballyUniqueID (pctxt, pvalue->u.nonStandard);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "nonStandard", -1);

            break;

         default:
            return LOG_ASN1ERR (pctxt, ASN_E_INVOPT);
      }
   }
   else {
      stat = decodeSmallNonNegWholeNumber (pctxt, &ui);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      else pvalue->t = ui + 4;

      stat = decodeByteAlign (pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      pvalue->u.extElem1 = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
      if (pvalue->u.extElem1 == NULL)
         return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

      pvalue->u.extElem1->data = openType.data;
      pvalue->u.extElem1->numocts = openType.numocts;

   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  TransportAddress_ipAddress_ip                             */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225TransportAddress_ipAddress_ip (OOCTXT* pctxt, H225TransportAddress_ipAddress_ip* pvalue)
{
   static Asn1SizeCnst lsize1 = { 0, 4, 4, 0 };
   int stat = ASN_OK;

   stat = addSizeConstraint (pctxt, &lsize1);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   stat = decodeOctetString (pctxt,
                          &pvalue->numocts,
                          pvalue->data,
                          sizeof(pvalue->data));
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   rtInvokeOctStrValue (pctxt, pvalue->numocts, pvalue->data);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  TransportAddress_ipAddress                                */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225TransportAddress_ipAddress (OOCTXT* pctxt, H225TransportAddress_ipAddress* pvalue)
{
   int stat = ASN_OK;

   /* decode ip */

   rtInvokeStartElement (pctxt, "ip", -1);

   stat = asn1PD_H225TransportAddress_ipAddress_ip (pctxt, &pvalue->ip);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "ip", -1);

   /* decode port */

   rtInvokeStartElement (pctxt, "port", -1);

   stat = decodeConsUInt16 (pctxt, &pvalue->port, 0U, 65535U);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   rtInvokeUIntValue (pctxt, pvalue->port);

   rtInvokeEndElement (pctxt, "port", -1);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  TransportAddress_ipSourceRoute_ip                         */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225TransportAddress_ipSourceRoute_ip (OOCTXT* pctxt, H225TransportAddress_ipSourceRoute_ip* pvalue)
{
   static Asn1SizeCnst lsize1 = { 0, 4, 4, 0 };
   int stat = ASN_OK;

   stat = addSizeConstraint (pctxt, &lsize1);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   stat = decodeOctetString (pctxt,
                          &pvalue->numocts,
                          pvalue->data,
                          sizeof(pvalue->data));
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   rtInvokeOctStrValue (pctxt, pvalue->numocts, pvalue->data);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  TransportAddress_ipSourceRoute_route_element              */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225TransportAddress_ipSourceRoute_route_element (OOCTXT* pctxt, H225TransportAddress_ipSourceRoute_route_element* pvalue)
{
   static Asn1SizeCnst lsize1 = { 0, 4, 4, 0 };
   int stat = ASN_OK;

   stat = addSizeConstraint (pctxt, &lsize1);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   stat = decodeOctetString (pctxt,
                          &pvalue->numocts,
                          pvalue->data,
                          sizeof(pvalue->data));
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   rtInvokeOctStrValue (pctxt, pvalue->numocts, pvalue->data);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  _SeqOfH225TransportAddress_ipSourceRoute_route_element    */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225_SeqOfH225TransportAddress_ipSourceRoute_route_element (OOCTXT* pctxt, H225_SeqOfH225TransportAddress_ipSourceRoute_route_element* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT xx1;

   /* decode length determinant */

   stat = decodeLength (pctxt, &pvalue->n);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* decode elements */

   ALLOC_ASN1ARRAY (pctxt, pvalue, H225TransportAddress_ipSourceRoute_route_element);

   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      rtInvokeStartElement (pctxt, "elem", xx1);

      stat = asn1PD_H225TransportAddress_ipSourceRoute_route_element (pctxt, &pvalue->elem[xx1]);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      rtInvokeEndElement (pctxt, "elem", xx1);

   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  TransportAddress_ipSourceRoute_routing                    */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225TransportAddress_ipSourceRoute_routing (OOCTXT* pctxt, H225TransportAddress_ipSourceRoute_routing* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT ui;
   ASN1OpenType openType;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   if (!extbit) {
      stat = decodeConsUnsigned (pctxt, &ui, 0, 1);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      else pvalue->t = ui + 1;

      switch (ui) {
         /* strict */
         case 0:
            rtInvokeStartElement (pctxt, "strict", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "strict", -1);

            break;

         /* loose */
         case 1:
            rtInvokeStartElement (pctxt, "loose", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "loose", -1);

            break;

         default:
            return LOG_ASN1ERR (pctxt, ASN_E_INVOPT);
      }
   }
   else {
      stat = decodeSmallNonNegWholeNumber (pctxt, &ui);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      else pvalue->t = ui + 3;

      stat = decodeByteAlign (pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      pvalue->u.extElem1 = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
      if (pvalue->u.extElem1 == NULL)
         return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

      pvalue->u.extElem1->data = openType.data;
      pvalue->u.extElem1->numocts = openType.numocts;

   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  TransportAddress_ipSourceRoute                            */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225TransportAddress_ipSourceRoute (OOCTXT* pctxt, H225TransportAddress_ipSourceRoute* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   dListInit (&pvalue->extElem1);

   /* decode ip */

   rtInvokeStartElement (pctxt, "ip", -1);

   stat = asn1PD_H225TransportAddress_ipSourceRoute_ip (pctxt, &pvalue->ip);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "ip", -1);

   /* decode port */

   rtInvokeStartElement (pctxt, "port", -1);

   stat = decodeConsUInt16 (pctxt, &pvalue->port, 0U, 65535U);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   rtInvokeUIntValue (pctxt, pvalue->port);

   rtInvokeEndElement (pctxt, "port", -1);

   /* decode route */

   rtInvokeStartElement (pctxt, "route", -1);

   stat = asn1PD_H225_SeqOfH225TransportAddress_ipSourceRoute_route_element (pctxt, &pvalue->route);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "route", -1);

   /* decode routing */

   rtInvokeStartElement (pctxt, "routing", -1);

   stat = asn1PD_H225TransportAddress_ipSourceRoute_routing (pctxt, &pvalue->routing);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "routing", -1);

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
            pOpenType->numocts = openType.numocts;
            pOpenType->data = openType.data;
            dListAppend (pctxt, &pvalue->extElem1, pOpenType);
         }
         else {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  TransportAddress_ipxAddress_node                          */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225TransportAddress_ipxAddress_node (OOCTXT* pctxt, H225TransportAddress_ipxAddress_node* pvalue)
{
   static Asn1SizeCnst lsize1 = { 0, 6, 6, 0 };
   int stat = ASN_OK;

   stat = addSizeConstraint (pctxt, &lsize1);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   stat = decodeOctetString (pctxt,
                          &pvalue->numocts,
                          pvalue->data,
                          sizeof(pvalue->data));
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   rtInvokeOctStrValue (pctxt, pvalue->numocts, pvalue->data);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  TransportAddress_ipxAddress_netnum                        */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225TransportAddress_ipxAddress_netnum (OOCTXT* pctxt, H225TransportAddress_ipxAddress_netnum* pvalue)
{
   static Asn1SizeCnst lsize1 = { 0, 4, 4, 0 };
   int stat = ASN_OK;

   stat = addSizeConstraint (pctxt, &lsize1);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   stat = decodeOctetString (pctxt,
                          &pvalue->numocts,
                          pvalue->data,
                          sizeof(pvalue->data));
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   rtInvokeOctStrValue (pctxt, pvalue->numocts, pvalue->data);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  TransportAddress_ipxAddress_port                          */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225TransportAddress_ipxAddress_port (OOCTXT* pctxt, H225TransportAddress_ipxAddress_port* pvalue)
{
   static Asn1SizeCnst lsize1 = { 0, 2, 2, 0 };
   int stat = ASN_OK;

   stat = addSizeConstraint (pctxt, &lsize1);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   stat = decodeOctetString (pctxt,
                          &pvalue->numocts,
                          pvalue->data,
                          sizeof(pvalue->data));
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   rtInvokeOctStrValue (pctxt, pvalue->numocts, pvalue->data);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  TransportAddress_ipxAddress                               */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225TransportAddress_ipxAddress (OOCTXT* pctxt, H225TransportAddress_ipxAddress* pvalue)
{
   int stat = ASN_OK;

   /* decode node */

   rtInvokeStartElement (pctxt, "node", -1);

   stat = asn1PD_H225TransportAddress_ipxAddress_node (pctxt, &pvalue->node);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "node", -1);

   /* decode netnum */

   rtInvokeStartElement (pctxt, "netnum", -1);

   stat = asn1PD_H225TransportAddress_ipxAddress_netnum (pctxt, &pvalue->netnum);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "netnum", -1);

   /* decode port */

   rtInvokeStartElement (pctxt, "port", -1);

   stat = asn1PD_H225TransportAddress_ipxAddress_port (pctxt, &pvalue->port);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "port", -1);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  TransportAddress_ip6Address_ip                            */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225TransportAddress_ip6Address_ip (OOCTXT* pctxt, H225TransportAddress_ip6Address_ip* pvalue)
{
   static Asn1SizeCnst lsize1 = { 0, 16, 16, 0 };
   int stat = ASN_OK;

   stat = addSizeConstraint (pctxt, &lsize1);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   stat = decodeOctetString (pctxt,
                          &pvalue->numocts,
                          pvalue->data,
                          sizeof(pvalue->data));
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   rtInvokeOctStrValue (pctxt, pvalue->numocts, pvalue->data);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  TransportAddress_ip6Address                               */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225TransportAddress_ip6Address (OOCTXT* pctxt, H225TransportAddress_ip6Address* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   dListInit (&pvalue->extElem1);

   /* decode ip */

   rtInvokeStartElement (pctxt, "ip", -1);

   stat = asn1PD_H225TransportAddress_ip6Address_ip (pctxt, &pvalue->ip);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "ip", -1);

   /* decode port */

   rtInvokeStartElement (pctxt, "port", -1);

   stat = decodeConsUInt16 (pctxt, &pvalue->port, 0U, 65535U);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   rtInvokeUIntValue (pctxt, pvalue->port);

   rtInvokeEndElement (pctxt, "port", -1);

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
            pOpenType->numocts = openType.numocts;
            pOpenType->data = openType.data;
            dListAppend (pctxt, &pvalue->extElem1, pOpenType);
         }
         else {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  TransportAddress_netBios                                  */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225TransportAddress_netBios (OOCTXT* pctxt, H225TransportAddress_netBios* pvalue)
{
   static Asn1SizeCnst lsize1 = { 0, 16, 16, 0 };
   int stat = ASN_OK;

   stat = addSizeConstraint (pctxt, &lsize1);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   stat = decodeOctetString (pctxt,
                          &pvalue->numocts,
                          pvalue->data,
                          sizeof(pvalue->data));
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   rtInvokeOctStrValue (pctxt, pvalue->numocts, pvalue->data);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  TransportAddress_nsap                                     */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225TransportAddress_nsap (OOCTXT* pctxt, H225TransportAddress_nsap* pvalue)
{
   static Asn1SizeCnst lsize1 = { 0, 1, 20, 0 };
   int stat = ASN_OK;

   stat = addSizeConstraint (pctxt, &lsize1);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   stat = decodeOctetString (pctxt,
                          &pvalue->numocts,
                          pvalue->data,
                          sizeof(pvalue->data));
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   rtInvokeOctStrValue (pctxt, pvalue->numocts, pvalue->data);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  H221NonStandard                                           */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225H221NonStandard (OOCTXT* pctxt, H225H221NonStandard* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   dListInit (&pvalue->extElem1);

   /* decode t35CountryCode */

   rtInvokeStartElement (pctxt, "t35CountryCode", -1);

   stat = decodeConsUInt8 (pctxt, &pvalue->t35CountryCode, 0U, 255U);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   rtInvokeUIntValue (pctxt, pvalue->t35CountryCode);

   rtInvokeEndElement (pctxt, "t35CountryCode", -1);

   /* decode t35Extension */

   rtInvokeStartElement (pctxt, "t35Extension", -1);

   stat = decodeConsUInt8 (pctxt, &pvalue->t35Extension, 0U, 255U);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   rtInvokeUIntValue (pctxt, pvalue->t35Extension);

   rtInvokeEndElement (pctxt, "t35Extension", -1);

   /* decode manufacturerCode */

   rtInvokeStartElement (pctxt, "manufacturerCode", -1);

   stat = decodeConsUInt16 (pctxt, &pvalue->manufacturerCode, 0U, 65535U);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   rtInvokeUIntValue (pctxt, pvalue->manufacturerCode);

   rtInvokeEndElement (pctxt, "manufacturerCode", -1);

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
            pOpenType->numocts = openType.numocts;
            pOpenType->data = openType.data;
            dListAppend (pctxt, &pvalue->extElem1, pOpenType);
         }
         else {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  NonStandardIdentifier                                     */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225NonStandardIdentifier (OOCTXT* pctxt, H225NonStandardIdentifier* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT ui;
   ASN1OpenType openType;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   if (!extbit) {
      stat = decodeConsUnsigned (pctxt, &ui, 0, 1);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      else pvalue->t = ui + 1;

      switch (ui) {
         /* object */
         case 0:
            rtInvokeStartElement (pctxt, "object", -1);

            pvalue->u.object = ALLOC_ASN1ELEM (pctxt, ASN1OBJID);
            if (pvalue->u.object == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = decodeObjectIdentifier (pctxt, pvalue->u.object);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
            rtInvokeOidValue (pctxt, pvalue->u.object->numids, pvalue->u.object->subid);

            rtInvokeEndElement (pctxt, "object", -1);

            break;

         /* h221NonStandard */
         case 1:
            rtInvokeStartElement (pctxt, "h221NonStandard", -1);

            pvalue->u.h221NonStandard = ALLOC_ASN1ELEM (pctxt, H225H221NonStandard);
            if (pvalue->u.h221NonStandard == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225H221NonStandard (pctxt, pvalue->u.h221NonStandard);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "h221NonStandard", -1);

            break;

         default:
            return LOG_ASN1ERR (pctxt, ASN_E_INVOPT);
      }
   }
   else {
      stat = decodeSmallNonNegWholeNumber (pctxt, &ui);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      else pvalue->t = ui + 3;

      stat = decodeByteAlign (pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      pvalue->u.extElem1 = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
      if (pvalue->u.extElem1 == NULL)
         return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

      pvalue->u.extElem1->data = openType.data;
      pvalue->u.extElem1->numocts = openType.numocts;

   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  NonStandardParameter                                      */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225NonStandardParameter (OOCTXT* pctxt, H225NonStandardParameter* pvalue)
{
   int stat = ASN_OK;

   /* decode nonStandardIdentifier */

   rtInvokeStartElement (pctxt, "nonStandardIdentifier", -1);

   stat = asn1PD_H225NonStandardIdentifier (pctxt, &pvalue->nonStandardIdentifier);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "nonStandardIdentifier", -1);

   /* decode data */

   rtInvokeStartElement (pctxt, "data", -1);

   stat = decodeDynOctetString (pctxt, (ASN1DynOctStr*)&pvalue->data);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   rtInvokeOctStrValue (pctxt, pvalue->data.numocts, pvalue->data.data);

   rtInvokeEndElement (pctxt, "data", -1);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  TransportAddress                                          */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225TransportAddress (OOCTXT* pctxt, H225TransportAddress* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT ui;
   ASN1OpenType openType;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   if (!extbit) {
      stat = decodeConsUnsigned (pctxt, &ui, 0, 6);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      else pvalue->t = ui + 1;

      switch (ui) {
         /* ipAddress */
         case 0:
            rtInvokeStartElement (pctxt, "ipAddress", -1);

            pvalue->u.ipAddress = ALLOC_ASN1ELEM (pctxt, H225TransportAddress_ipAddress);
            if (pvalue->u.ipAddress == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225TransportAddress_ipAddress (pctxt, pvalue->u.ipAddress);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "ipAddress", -1);

            break;

         /* ipSourceRoute */
         case 1:
            rtInvokeStartElement (pctxt, "ipSourceRoute", -1);

            pvalue->u.ipSourceRoute = ALLOC_ASN1ELEM (pctxt, H225TransportAddress_ipSourceRoute);
            if (pvalue->u.ipSourceRoute == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225TransportAddress_ipSourceRoute (pctxt, pvalue->u.ipSourceRoute);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "ipSourceRoute", -1);

            break;

         /* ipxAddress */
         case 2:
            rtInvokeStartElement (pctxt, "ipxAddress", -1);

            pvalue->u.ipxAddress = ALLOC_ASN1ELEM (pctxt, H225TransportAddress_ipxAddress);
            if (pvalue->u.ipxAddress == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225TransportAddress_ipxAddress (pctxt, pvalue->u.ipxAddress);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "ipxAddress", -1);

            break;

         /* ip6Address */
         case 3:
            rtInvokeStartElement (pctxt, "ip6Address", -1);

            pvalue->u.ip6Address = ALLOC_ASN1ELEM (pctxt, H225TransportAddress_ip6Address);
            if (pvalue->u.ip6Address == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225TransportAddress_ip6Address (pctxt, pvalue->u.ip6Address);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "ip6Address", -1);

            break;

         /* netBios */
         case 4:
            rtInvokeStartElement (pctxt, "netBios", -1);

            pvalue->u.netBios = ALLOC_ASN1ELEM (pctxt, H225TransportAddress_netBios);
            if (pvalue->u.netBios == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225TransportAddress_netBios (pctxt, pvalue->u.netBios);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "netBios", -1);

            break;

         /* nsap */
         case 5:
            rtInvokeStartElement (pctxt, "nsap", -1);

            pvalue->u.nsap = ALLOC_ASN1ELEM (pctxt, H225TransportAddress_nsap);
            if (pvalue->u.nsap == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225TransportAddress_nsap (pctxt, pvalue->u.nsap);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "nsap", -1);

            break;

         /* nonStandardAddress */
         case 6:
            rtInvokeStartElement (pctxt, "nonStandardAddress", -1);

            pvalue->u.nonStandardAddress = ALLOC_ASN1ELEM (pctxt, H225NonStandardParameter);
            if (pvalue->u.nonStandardAddress == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225NonStandardParameter (pctxt, pvalue->u.nonStandardAddress);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "nonStandardAddress", -1);

            break;

         default:
            return LOG_ASN1ERR (pctxt, ASN_E_INVOPT);
      }
   }
   else {
      stat = decodeSmallNonNegWholeNumber (pctxt, &ui);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      else pvalue->t = ui + 8;

      stat = decodeByteAlign (pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      pvalue->u.extElem1 = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
      if (pvalue->u.extElem1 == NULL)
         return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

      pvalue->u.extElem1->data = openType.data;
      pvalue->u.extElem1->numocts = openType.numocts;

   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  PublicTypeOfNumber                                        */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225PublicTypeOfNumber (OOCTXT* pctxt, H225PublicTypeOfNumber* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT ui;
   ASN1OpenType openType;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   if (!extbit) {
      stat = decodeConsUnsigned (pctxt, &ui, 0, 5);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      else pvalue->t = ui + 1;

      switch (ui) {
         /* unknown */
         case 0:
            rtInvokeStartElement (pctxt, "unknown", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "unknown", -1);

            break;

         /* internationalNumber */
         case 1:
            rtInvokeStartElement (pctxt, "internationalNumber", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "internationalNumber", -1);

            break;

         /* nationalNumber */
         case 2:
            rtInvokeStartElement (pctxt, "nationalNumber", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "nationalNumber", -1);

            break;

         /* networkSpecificNumber */
         case 3:
            rtInvokeStartElement (pctxt, "networkSpecificNumber", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "networkSpecificNumber", -1);

            break;

         /* subscriberNumber */
         case 4:
            rtInvokeStartElement (pctxt, "subscriberNumber", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "subscriberNumber", -1);

            break;

         /* abbreviatedNumber */
         case 5:
            rtInvokeStartElement (pctxt, "abbreviatedNumber", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "abbreviatedNumber", -1);

            break;

         default:
            return LOG_ASN1ERR (pctxt, ASN_E_INVOPT);
      }
   }
   else {
      stat = decodeSmallNonNegWholeNumber (pctxt, &ui);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      else pvalue->t = ui + 7;

      stat = decodeByteAlign (pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      pvalue->u.extElem1 = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
      if (pvalue->u.extElem1 == NULL)
         return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

      pvalue->u.extElem1->data = openType.data;
      pvalue->u.extElem1->numocts = openType.numocts;

   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  PublicPartyNumber                                         */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225PublicPartyNumber (OOCTXT* pctxt, H225PublicPartyNumber* pvalue)
{
   int stat = ASN_OK;

   /* decode publicTypeOfNumber */

   rtInvokeStartElement (pctxt, "publicTypeOfNumber", -1);

   stat = asn1PD_H225PublicTypeOfNumber (pctxt, &pvalue->publicTypeOfNumber);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "publicTypeOfNumber", -1);

   /* decode publicNumberDigits */

   rtInvokeStartElement (pctxt, "publicNumberDigits", -1);

   stat = asn1PD_H225NumberDigits (pctxt, &pvalue->publicNumberDigits);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "publicNumberDigits", -1);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  PrivateTypeOfNumber                                       */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225PrivateTypeOfNumber (OOCTXT* pctxt, H225PrivateTypeOfNumber* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT ui;
   ASN1OpenType openType;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   if (!extbit) {
      stat = decodeConsUnsigned (pctxt, &ui, 0, 5);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      else pvalue->t = ui + 1;

      switch (ui) {
         /* unknown */
         case 0:
            rtInvokeStartElement (pctxt, "unknown", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "unknown", -1);

            break;

         /* level2RegionalNumber */
         case 1:
            rtInvokeStartElement (pctxt, "level2RegionalNumber", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "level2RegionalNumber", -1);

            break;

         /* level1RegionalNumber */
         case 2:
            rtInvokeStartElement (pctxt, "level1RegionalNumber", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "level1RegionalNumber", -1);

            break;

         /* pISNSpecificNumber */
         case 3:
            rtInvokeStartElement (pctxt, "pISNSpecificNumber", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "pISNSpecificNumber", -1);

            break;

         /* localNumber */
         case 4:
            rtInvokeStartElement (pctxt, "localNumber", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "localNumber", -1);

            break;

         /* abbreviatedNumber */
         case 5:
            rtInvokeStartElement (pctxt, "abbreviatedNumber", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "abbreviatedNumber", -1);

            break;

         default:
            return LOG_ASN1ERR (pctxt, ASN_E_INVOPT);
      }
   }
   else {
      stat = decodeSmallNonNegWholeNumber (pctxt, &ui);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      else pvalue->t = ui + 7;

      stat = decodeByteAlign (pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      pvalue->u.extElem1 = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
      if (pvalue->u.extElem1 == NULL)
         return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

      pvalue->u.extElem1->data = openType.data;
      pvalue->u.extElem1->numocts = openType.numocts;

   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  PrivatePartyNumber                                        */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225PrivatePartyNumber (OOCTXT* pctxt, H225PrivatePartyNumber* pvalue)
{
   int stat = ASN_OK;

   /* decode privateTypeOfNumber */

   rtInvokeStartElement (pctxt, "privateTypeOfNumber", -1);

   stat = asn1PD_H225PrivateTypeOfNumber (pctxt, &pvalue->privateTypeOfNumber);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "privateTypeOfNumber", -1);

   /* decode privateNumberDigits */

   rtInvokeStartElement (pctxt, "privateNumberDigits", -1);

   stat = asn1PD_H225NumberDigits (pctxt, &pvalue->privateNumberDigits);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "privateNumberDigits", -1);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  PartyNumber                                               */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225PartyNumber (OOCTXT* pctxt, H225PartyNumber* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT ui;
   ASN1OpenType openType;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   if (!extbit) {
      stat = decodeConsUnsigned (pctxt, &ui, 0, 4);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      else pvalue->t = ui + 1;

      switch (ui) {
         /* e164Number */
         case 0:
            rtInvokeStartElement (pctxt, "e164Number", -1);

            pvalue->u.e164Number = ALLOC_ASN1ELEM (pctxt, H225PublicPartyNumber);
            if (pvalue->u.e164Number == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225PublicPartyNumber (pctxt, pvalue->u.e164Number);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "e164Number", -1);

            break;

         /* dataPartyNumber */
         case 1:
            rtInvokeStartElement (pctxt, "dataPartyNumber", -1);

            stat = asn1PD_H225NumberDigits (pctxt, &pvalue->u.dataPartyNumber);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "dataPartyNumber", -1);

            break;

         /* telexPartyNumber */
         case 2:
            rtInvokeStartElement (pctxt, "telexPartyNumber", -1);

            stat = asn1PD_H225NumberDigits (pctxt, &pvalue->u.telexPartyNumber);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "telexPartyNumber", -1);

            break;

         /* privateNumber */
         case 3:
            rtInvokeStartElement (pctxt, "privateNumber", -1);

            pvalue->u.privateNumber = ALLOC_ASN1ELEM (pctxt, H225PrivatePartyNumber);
            if (pvalue->u.privateNumber == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225PrivatePartyNumber (pctxt, pvalue->u.privateNumber);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "privateNumber", -1);

            break;

         /* nationalStandardPartyNumber */
         case 4:
            rtInvokeStartElement (pctxt, "nationalStandardPartyNumber", -1);

            stat = asn1PD_H225NumberDigits (pctxt, &pvalue->u.nationalStandardPartyNumber);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "nationalStandardPartyNumber", -1);

            break;

         default:
            return LOG_ASN1ERR (pctxt, ASN_E_INVOPT);
      }
   }
   else {
      stat = decodeSmallNonNegWholeNumber (pctxt, &ui);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      else pvalue->t = ui + 6;

      stat = decodeByteAlign (pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      pvalue->u.extElem1 = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
      if (pvalue->u.extElem1 == NULL)
         return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

      pvalue->u.extElem1->data = openType.data;
      pvalue->u.extElem1->numocts = openType.numocts;

   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  ANSI_41_UIM_system_id                                     */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225ANSI_41_UIM_system_id (OOCTXT* pctxt, H225ANSI_41_UIM_system_id* pvalue)
{
   static Asn1SizeCnst sid_lsize1 = { 0, 1, 4, 0 };
   static Asn1SizeCnst mid_lsize1 = { 0, 1, 4, 0 };
   int stat = ASN_OK;
   ASN1UINT ui;
   ASN1OpenType openType;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   if (!extbit) {
      stat = decodeConsUnsigned (pctxt, &ui, 0, 1);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      else pvalue->t = ui + 1;

      switch (ui) {
         /* sid */
         case 0:
            rtInvokeStartElement (pctxt, "sid", -1);

            stat = addSizeConstraint (pctxt, &sid_lsize1);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            stat = asn1PD_H225TBCD_STRING (pctxt, &pvalue->u.sid);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "sid", -1);

            break;

         /* mid */
         case 1:
            rtInvokeStartElement (pctxt, "mid", -1);

            stat = addSizeConstraint (pctxt, &mid_lsize1);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            stat = asn1PD_H225TBCD_STRING (pctxt, &pvalue->u.mid);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "mid", -1);

            break;

         default:
            return LOG_ASN1ERR (pctxt, ASN_E_INVOPT);
      }
   }
   else {
      stat = decodeSmallNonNegWholeNumber (pctxt, &ui);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      else pvalue->t = ui + 3;

      stat = decodeByteAlign (pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      pvalue->u.extElem1 = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
      if (pvalue->u.extElem1 == NULL)
         return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

      pvalue->u.extElem1->data = openType.data;
      pvalue->u.extElem1->numocts = openType.numocts;

   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  ANSI_41_UIM_systemMyTypeCode                              */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225ANSI_41_UIM_systemMyTypeCode (OOCTXT* pctxt, H225ANSI_41_UIM_systemMyTypeCode* pvalue)
{
   static Asn1SizeCnst lsize1 = { 0, 1, 1, 0 };
   int stat = ASN_OK;

   stat = addSizeConstraint (pctxt, &lsize1);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   stat = decodeOctetString (pctxt,
                          &pvalue->numocts,
                          pvalue->data,
                          sizeof(pvalue->data));
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   rtInvokeOctStrValue (pctxt, pvalue->numocts, pvalue->data);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  ANSI_41_UIM_systemAccessType                              */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225ANSI_41_UIM_systemAccessType (OOCTXT* pctxt, H225ANSI_41_UIM_systemAccessType* pvalue)
{
   static Asn1SizeCnst lsize1 = { 0, 1, 1, 0 };
   int stat = ASN_OK;

   stat = addSizeConstraint (pctxt, &lsize1);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   stat = decodeOctetString (pctxt,
                          &pvalue->numocts,
                          pvalue->data,
                          sizeof(pvalue->data));
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   rtInvokeOctStrValue (pctxt, pvalue->numocts, pvalue->data);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  ANSI_41_UIM_qualificationInformationCode                  */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225ANSI_41_UIM_qualificationInformationCode (OOCTXT* pctxt, H225ANSI_41_UIM_qualificationInformationCode* pvalue)
{
   static Asn1SizeCnst lsize1 = { 0, 1, 1, 0 };
   int stat = ASN_OK;

   stat = addSizeConstraint (pctxt, &lsize1);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   stat = decodeOctetString (pctxt,
                          &pvalue->numocts,
                          pvalue->data,
                          sizeof(pvalue->data));
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   rtInvokeOctStrValue (pctxt, pvalue->numocts, pvalue->data);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  ANSI_41_UIM                                               */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225ANSI_41_UIM (OOCTXT* pctxt, H225ANSI_41_UIM* pvalue)
{
   static Asn1SizeCnst imsi_lsize1 = { 0, 3, 16, 0 };
   static Asn1SizeCnst min_lsize1 = { 0, 3, 16, 0 };
   static Asn1SizeCnst mdn_lsize1 = { 0, 3, 16, 0 };
   static Asn1SizeCnst msisdn_lsize1 = { 0, 3, 16, 0 };
   static Asn1SizeCnst esn_lsize1 = { 0, 16, 16, 0 };
   static Asn1SizeCnst mscid_lsize1 = { 0, 3, 16, 0 };
   static Asn1SizeCnst sesn_lsize1 = { 0, 16, 16, 0 };
   static Asn1SizeCnst soc_lsize1 = { 0, 3, 16, 0 };
   int stat = ASN_OK;
   OOCTXT lctxt;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.imsiPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.minPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.mdnPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.msisdnPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.esnPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.mscidPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.systemMyTypeCodePresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.systemAccessTypePresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.qualificationInformationCodePresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.sesnPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.socPresent = optbit;

   /* decode imsi */

   if (pvalue->m.imsiPresent) {
      rtInvokeStartElement (pctxt, "imsi", -1);

      stat = addSizeConstraint (pctxt, &imsi_lsize1);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = asn1PD_H225TBCD_STRING (pctxt, &pvalue->imsi);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "imsi", -1);
   }

   /* decode min */

   if (pvalue->m.minPresent) {
      rtInvokeStartElement (pctxt, "min", -1);

      stat = addSizeConstraint (pctxt, &min_lsize1);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = asn1PD_H225TBCD_STRING (pctxt, &pvalue->min);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "min", -1);
   }

   /* decode mdn */

   if (pvalue->m.mdnPresent) {
      rtInvokeStartElement (pctxt, "mdn", -1);

      stat = addSizeConstraint (pctxt, &mdn_lsize1);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = asn1PD_H225TBCD_STRING (pctxt, &pvalue->mdn);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "mdn", -1);
   }

   /* decode msisdn */

   if (pvalue->m.msisdnPresent) {
      rtInvokeStartElement (pctxt, "msisdn", -1);

      stat = addSizeConstraint (pctxt, &msisdn_lsize1);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = asn1PD_H225TBCD_STRING (pctxt, &pvalue->msisdn);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "msisdn", -1);
   }

   /* decode esn */

   if (pvalue->m.esnPresent) {
      rtInvokeStartElement (pctxt, "esn", -1);

      stat = addSizeConstraint (pctxt, &esn_lsize1);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = asn1PD_H225TBCD_STRING (pctxt, &pvalue->esn);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "esn", -1);
   }

   /* decode mscid */

   if (pvalue->m.mscidPresent) {
      rtInvokeStartElement (pctxt, "mscid", -1);

      stat = addSizeConstraint (pctxt, &mscid_lsize1);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = asn1PD_H225TBCD_STRING (pctxt, &pvalue->mscid);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "mscid", -1);
   }

   /* decode system_id */

   rtInvokeStartElement (pctxt, "system_id", -1);

   stat = asn1PD_H225ANSI_41_UIM_system_id (pctxt, &pvalue->system_id);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "system_id", -1);

   /* decode systemMyTypeCode */

   if (pvalue->m.systemMyTypeCodePresent) {
      rtInvokeStartElement (pctxt, "systemMyTypeCode", -1);

      stat = asn1PD_H225ANSI_41_UIM_systemMyTypeCode (pctxt, &pvalue->systemMyTypeCode);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "systemMyTypeCode", -1);
   }

   /* decode systemAccessType */

   if (pvalue->m.systemAccessTypePresent) {
      rtInvokeStartElement (pctxt, "systemAccessType", -1);

      stat = asn1PD_H225ANSI_41_UIM_systemAccessType (pctxt, &pvalue->systemAccessType);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "systemAccessType", -1);
   }

   /* decode qualificationInformationCode */

   if (pvalue->m.qualificationInformationCodePresent) {
      rtInvokeStartElement (pctxt, "qualificationInformationCode", -1);

      stat = asn1PD_H225ANSI_41_UIM_qualificationInformationCode (pctxt, &pvalue->qualificationInformationCode);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "qualificationInformationCode", -1);
   }

   /* decode sesn */

   if (pvalue->m.sesnPresent) {
      rtInvokeStartElement (pctxt, "sesn", -1);

      stat = addSizeConstraint (pctxt, &sesn_lsize1);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = asn1PD_H225TBCD_STRING (pctxt, &pvalue->sesn);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "sesn", -1);
   }

   /* decode soc */

   if (pvalue->m.socPresent) {
      rtInvokeStartElement (pctxt, "soc", -1);

      stat = addSizeConstraint (pctxt, &soc_lsize1);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = asn1PD_H225TBCD_STRING (pctxt, &pvalue->soc);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "soc", -1);
   }

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
            pOpenType->numocts = openType.numocts;
            pOpenType->data = openType.data;
            dListAppend (pctxt, &pvalue->extElem1, pOpenType);
         }
         else {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  GSM_UIM_tmsi                                              */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225GSM_UIM_tmsi (OOCTXT* pctxt, H225GSM_UIM_tmsi* pvalue)
{
   static Asn1SizeCnst lsize1 = { 0, 1, 4, 0 };
   int stat = ASN_OK;

   stat = addSizeConstraint (pctxt, &lsize1);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   stat = decodeOctetString (pctxt,
                          &pvalue->numocts,
                          pvalue->data,
                          sizeof(pvalue->data));
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   rtInvokeOctStrValue (pctxt, pvalue->numocts, pvalue->data);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  GSM_UIM                                                   */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225GSM_UIM (OOCTXT* pctxt, H225GSM_UIM* pvalue)
{
   static Asn1SizeCnst imsi_lsize1 = { 0, 3, 16, 0 };
   static Asn1SizeCnst msisdn_lsize1 = { 0, 3, 16, 0 };
   static Asn1SizeCnst imei_lsize1 = { 0, 15, 16, 0 };
   static Asn1SizeCnst hplmn_lsize1 = { 0, 1, 4, 0 };
   static Asn1SizeCnst vplmn_lsize1 = { 0, 1, 4, 0 };
   int stat = ASN_OK;
   OOCTXT lctxt;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.imsiPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.tmsiPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.msisdnPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.imeiPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.hplmnPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.vplmnPresent = optbit;

   /* decode imsi */

   if (pvalue->m.imsiPresent) {
      rtInvokeStartElement (pctxt, "imsi", -1);

      stat = addSizeConstraint (pctxt, &imsi_lsize1);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = asn1PD_H225TBCD_STRING (pctxt, &pvalue->imsi);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "imsi", -1);
   }

   /* decode tmsi */

   if (pvalue->m.tmsiPresent) {
      rtInvokeStartElement (pctxt, "tmsi", -1);

      stat = asn1PD_H225GSM_UIM_tmsi (pctxt, &pvalue->tmsi);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "tmsi", -1);
   }

   /* decode msisdn */

   if (pvalue->m.msisdnPresent) {
      rtInvokeStartElement (pctxt, "msisdn", -1);

      stat = addSizeConstraint (pctxt, &msisdn_lsize1);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = asn1PD_H225TBCD_STRING (pctxt, &pvalue->msisdn);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "msisdn", -1);
   }

   /* decode imei */

   if (pvalue->m.imeiPresent) {
      rtInvokeStartElement (pctxt, "imei", -1);

      stat = addSizeConstraint (pctxt, &imei_lsize1);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = asn1PD_H225TBCD_STRING (pctxt, &pvalue->imei);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "imei", -1);
   }

   /* decode hplmn */

   if (pvalue->m.hplmnPresent) {
      rtInvokeStartElement (pctxt, "hplmn", -1);

      stat = addSizeConstraint (pctxt, &hplmn_lsize1);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = asn1PD_H225TBCD_STRING (pctxt, &pvalue->hplmn);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "hplmn", -1);
   }

   /* decode vplmn */

   if (pvalue->m.vplmnPresent) {
      rtInvokeStartElement (pctxt, "vplmn", -1);

      stat = addSizeConstraint (pctxt, &vplmn_lsize1);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = asn1PD_H225TBCD_STRING (pctxt, &pvalue->vplmn);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "vplmn", -1);
   }

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
            pOpenType->numocts = openType.numocts;
            pOpenType->data = openType.data;
            dListAppend (pctxt, &pvalue->extElem1, pOpenType);
         }
         else {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  MobileUIM                                                 */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225MobileUIM (OOCTXT* pctxt, H225MobileUIM* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT ui;
   ASN1OpenType openType;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   if (!extbit) {
      stat = decodeConsUnsigned (pctxt, &ui, 0, 1);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      else pvalue->t = ui + 1;

      switch (ui) {
         /* ansi_41_uim */
         case 0:
            rtInvokeStartElement (pctxt, "ansi_41_uim", -1);

            pvalue->u.ansi_41_uim = ALLOC_ASN1ELEM (pctxt, H225ANSI_41_UIM);
            if (pvalue->u.ansi_41_uim == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225ANSI_41_UIM (pctxt, pvalue->u.ansi_41_uim);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "ansi_41_uim", -1);

            break;

         /* gsm_uim */
         case 1:
            rtInvokeStartElement (pctxt, "gsm_uim", -1);

            pvalue->u.gsm_uim = ALLOC_ASN1ELEM (pctxt, H225GSM_UIM);
            if (pvalue->u.gsm_uim == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225GSM_UIM (pctxt, pvalue->u.gsm_uim);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "gsm_uim", -1);

            break;

         default:
            return LOG_ASN1ERR (pctxt, ASN_E_INVOPT);
      }
   }
   else {
      stat = decodeSmallNonNegWholeNumber (pctxt, &ui);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      else pvalue->t = ui + 3;

      stat = decodeByteAlign (pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      pvalue->u.extElem1 = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
      if (pvalue->u.extElem1 == NULL)
         return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

      pvalue->u.extElem1->data = openType.data;
      pvalue->u.extElem1->numocts = openType.numocts;

   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  AliasAddress                                              */
/*                                                            */
/**************************************************************/

extern EXTERN const char* gs_H323_MESSAGES_AliasAddress_dialedDigits_CharSet;

EXTERN int asn1PD_H225AliasAddress (OOCTXT* pctxt, H225AliasAddress* pvalue)
{
   static Asn1SizeCnst dialedDigits_lsize1 = { 0, 1, 128, 0 };
   static Asn1SizeCnst h323_ID_lsize1 = { 0, 1, 256, 0 };
   static Asn1SizeCnst url_ID_lsize1 = { 0, 1, 512, 0 };
   static Asn1SizeCnst email_ID_lsize1 = { 0, 1, 512, 0 };
   int stat = ASN_OK;
   ASN1UINT ui;
   ASN1OpenType openType;
   ASN1BOOL extbit;
   OOCTXT lctxt;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   if (!extbit) {
      stat = decodeConsUnsigned (pctxt, &ui, 0, 1);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      else pvalue->t = ui + 1;

      switch (ui) {
         /* dialedDigits */
         case 0:
            rtInvokeStartElement (pctxt, "dialedDigits", -1);

            stat = addSizeConstraint (pctxt, &dialedDigits_lsize1);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            stat = decodeConstrainedStringEx (pctxt, &pvalue->u.dialedDigits, gs_H323_MESSAGES_AliasAddress_dialedDigits_CharSet, 4, 4, 7);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
            rtInvokeCharStrValue (pctxt, pvalue->u.dialedDigits);

            rtInvokeEndElement (pctxt, "dialedDigits", -1);

            break;

         /* h323_ID */
         case 1:
            rtInvokeStartElement (pctxt, "h323_ID", -1);

            stat = addSizeConstraint (pctxt, &h323_ID_lsize1);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            stat = decodeBMPString (pctxt, &pvalue->u.h323_ID, 0);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
            rtInvokeCharStr16BitValue (pctxt, pvalue->u.h323_ID.nchars, pvalue->u.h323_ID.data);

            rtInvokeEndElement (pctxt, "h323_ID", -1);

            break;

         default:
            return LOG_ASN1ERR (pctxt, ASN_E_INVOPT);
      }
   }
   else {
      stat = decodeSmallNonNegWholeNumber (pctxt, &ui);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      else pvalue->t = ui + 3;

      stat = decodeByteAlign (pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      copyContext (&lctxt, pctxt);
      initContextBuffer (pctxt, openType.data, openType.numocts);

      switch (pvalue->t) {
         /* url_ID */
         case 3:
            rtInvokeStartElement (pctxt, "url_ID", -1);

            stat = addSizeConstraint (pctxt, &url_ID_lsize1);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            stat = decodeConstrainedStringEx (pctxt, &pvalue->u.url_ID, 0, 8, 7, 7);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
            rtInvokeCharStrValue (pctxt, pvalue->u.url_ID);

            rtInvokeEndElement (pctxt, "url_ID", -1);

            break;

         /* transportID */
         case 4:
            rtInvokeStartElement (pctxt, "transportID", -1);

            pvalue->u.transportID = ALLOC_ASN1ELEM (pctxt, H225TransportAddress);
            if (pvalue->u.transportID == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225TransportAddress (pctxt, pvalue->u.transportID);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "transportID", -1);

            break;

         /* email_ID */
         case 5:
            rtInvokeStartElement (pctxt, "email_ID", -1);

            stat = addSizeConstraint (pctxt, &email_ID_lsize1);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            stat = decodeConstrainedStringEx (pctxt, &pvalue->u.email_ID, 0, 8, 7, 7);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
            rtInvokeCharStrValue (pctxt, pvalue->u.email_ID);

            rtInvokeEndElement (pctxt, "email_ID", -1);

            break;

         /* partyNumber */
         case 6:
            rtInvokeStartElement (pctxt, "partyNumber", -1);

            pvalue->u.partyNumber = ALLOC_ASN1ELEM (pctxt, H225PartyNumber);
            if (pvalue->u.partyNumber == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225PartyNumber (pctxt, pvalue->u.partyNumber);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "partyNumber", -1);

            break;

         /* mobileUIM */
         case 7:
            rtInvokeStartElement (pctxt, "mobileUIM", -1);

            pvalue->u.mobileUIM = ALLOC_ASN1ELEM (pctxt, H225MobileUIM);
            if (pvalue->u.mobileUIM == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225MobileUIM (pctxt, pvalue->u.mobileUIM);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "mobileUIM", -1);

            break;

         default:
            pvalue->u.extElem1 = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
            if (pvalue->u.extElem1 == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            pvalue->u.extElem1->data = openType.data;
            pvalue->u.extElem1->numocts = openType.numocts;

      }

      copyContext (pctxt, &lctxt);
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  Content_compound                                          */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225Content_compound (OOCTXT* pctxt, H225Content_compound* pvalue)
{
   static Asn1SizeCnst lsize1 = { 0, 1, 512, 0 };
   int stat = ASN_OK;
   H225EnumeratedParameter* pdata;
   ASN1UINT count = 0;
   ASN1UINT xx1;

   /* decode length determinant */

   stat = addSizeConstraint (pctxt, &lsize1);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   stat = decodeLength (pctxt, &count);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* decode elements */

   dListInit (pvalue);

   for (xx1 = 0; xx1 < count; xx1++) {
      rtInvokeStartElement (pctxt, "elem", xx1);

      pdata = ALLOC_ASN1ELEMDNODE (pctxt, H225EnumeratedParameter);
      if (pdata == NULL)
         return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

      stat = asn1PD_H225EnumeratedParameter (pctxt, (H225EnumeratedParameter*)pdata);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      rtInvokeEndElement (pctxt, "elem", xx1);

      dListAppendNode (pctxt, pvalue, pdata);
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  Content_nested                                            */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225Content_nested (OOCTXT* pctxt, H225Content_nested* pvalue)
{
   static Asn1SizeCnst lsize1 = { 0, 1, 16, 0 };
   int stat = ASN_OK;
   H225GenericData* pdata;
   ASN1UINT count = 0;
   ASN1UINT xx1;

   /* decode length determinant */

   stat = addSizeConstraint (pctxt, &lsize1);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   stat = decodeLength (pctxt, &count);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* decode elements */

   dListInit (pvalue);

   for (xx1 = 0; xx1 < count; xx1++) {
      rtInvokeStartElement (pctxt, "elem", xx1);

      pdata = ALLOC_ASN1ELEMDNODE (pctxt, H225GenericData);
      if (pdata == NULL)
         return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

      stat = asn1PD_H225GenericData (pctxt, (H225GenericData*)pdata);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      rtInvokeEndElement (pctxt, "elem", xx1);

      dListAppendNode (pctxt, pvalue, pdata);
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  Content                                                   */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225Content (OOCTXT* pctxt, H225Content* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT ui;
   ASN1OpenType openType;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   if (!extbit) {
      stat = decodeConsUnsigned (pctxt, &ui, 0, 11);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      else pvalue->t = ui + 1;

      switch (ui) {
         /* raw */
         case 0:
            rtInvokeStartElement (pctxt, "raw", -1);

            pvalue->u.raw = ALLOC_ASN1ELEM (pctxt, ASN1DynOctStr);
            if (pvalue->u.raw == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = decodeDynOctetString (pctxt, (ASN1DynOctStr*)pvalue->u.raw);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
            rtInvokeOctStrValue (pctxt, pvalue->u.raw->numocts, pvalue->u.raw->data);

            rtInvokeEndElement (pctxt, "raw", -1);

            break;

         /* text */
         case 1:
            rtInvokeStartElement (pctxt, "text", -1);

            stat = decodeConstrainedStringEx (pctxt, &pvalue->u.text, 0, 8, 7, 7);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
            rtInvokeCharStrValue (pctxt, pvalue->u.text);

            rtInvokeEndElement (pctxt, "text", -1);

            break;

         /* unicode */
         case 2:
            rtInvokeStartElement (pctxt, "unicode", -1);

            stat = decodeBMPString (pctxt, &pvalue->u.unicode, 0);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
            rtInvokeCharStr16BitValue (pctxt, pvalue->u.unicode.nchars, pvalue->u.unicode.data);

            rtInvokeEndElement (pctxt, "unicode", -1);

            break;

         /* bool_ */
         case 3:
            rtInvokeStartElement (pctxt, "bool_", -1);

            stat = DECODEBIT (pctxt, &pvalue->u.bool_);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
            rtInvokeBoolValue (pctxt, pvalue->u.bool_);

            rtInvokeEndElement (pctxt, "bool_", -1);

            break;

         /* number8 */
         case 4:
            rtInvokeStartElement (pctxt, "number8", -1);

            stat = decodeConsUInt8 (pctxt, &pvalue->u.number8, 0U, 255U);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
            rtInvokeUIntValue (pctxt, pvalue->u.number8);

            rtInvokeEndElement (pctxt, "number8", -1);

            break;

         /* number16 */
         case 5:
            rtInvokeStartElement (pctxt, "number16", -1);

            stat = decodeConsUInt16 (pctxt, &pvalue->u.number16, 0U, 65535U);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
            rtInvokeUIntValue (pctxt, pvalue->u.number16);

            rtInvokeEndElement (pctxt, "number16", -1);

            break;

         /* number32 */
         case 6:
            rtInvokeStartElement (pctxt, "number32", -1);

            stat = decodeConsUnsigned (pctxt, &pvalue->u.number32, 0U, ASN1UINT_MAX);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
            rtInvokeUIntValue (pctxt, pvalue->u.number32);

            rtInvokeEndElement (pctxt, "number32", -1);

            break;

         /* id */
         case 7:
            rtInvokeStartElement (pctxt, "id", -1);

            pvalue->u.id = ALLOC_ASN1ELEM (pctxt, H225GenericIdentifier);
            if (pvalue->u.id == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225GenericIdentifier (pctxt, pvalue->u.id);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "id", -1);

            break;

         /* alias */
         case 8:
            rtInvokeStartElement (pctxt, "alias", -1);

            pvalue->u.alias = ALLOC_ASN1ELEM (pctxt, H225AliasAddress);
            if (pvalue->u.alias == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225AliasAddress (pctxt, pvalue->u.alias);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "alias", -1);

            break;

         /* transport */
         case 9:
            rtInvokeStartElement (pctxt, "transport", -1);

            pvalue->u.transport = ALLOC_ASN1ELEM (pctxt, H225TransportAddress);
            if (pvalue->u.transport == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225TransportAddress (pctxt, pvalue->u.transport);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "transport", -1);

            break;

         /* compound */
         case 10:
            rtInvokeStartElement (pctxt, "compound", -1);

            pvalue->u.compound = ALLOC_ASN1ELEM (pctxt, H225Content_compound);
            if (pvalue->u.compound == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225Content_compound (pctxt, pvalue->u.compound);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "compound", -1);

            break;

         /* nested */
         case 11:
            rtInvokeStartElement (pctxt, "nested", -1);

            pvalue->u.nested = ALLOC_ASN1ELEM (pctxt, H225Content_nested);
            if (pvalue->u.nested == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225Content_nested (pctxt, pvalue->u.nested);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "nested", -1);

            break;

         default:
            return LOG_ASN1ERR (pctxt, ASN_E_INVOPT);
      }
   }
   else {
      stat = decodeSmallNonNegWholeNumber (pctxt, &ui);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      else pvalue->t = ui + 13;

      stat = decodeByteAlign (pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      pvalue->u.extElem1 = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
      if (pvalue->u.extElem1 == NULL)
         return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

      pvalue->u.extElem1->data = openType.data;
      pvalue->u.extElem1->numocts = openType.numocts;

   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  EnumeratedParameter                                       */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225EnumeratedParameter (OOCTXT* pctxt, H225EnumeratedParameter* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.contentPresent = optbit;

   /* decode id */

   rtInvokeStartElement (pctxt, "id", -1);

   stat = asn1PD_H225GenericIdentifier (pctxt, &pvalue->id);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "id", -1);

   /* decode content */

   if (pvalue->m.contentPresent) {
      rtInvokeStartElement (pctxt, "content", -1);

      stat = asn1PD_H225Content (pctxt, &pvalue->content);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "content", -1);
   }

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
            pOpenType->numocts = openType.numocts;
            pOpenType->data = openType.data;
            dListAppend (pctxt, &pvalue->extElem1, pOpenType);
         }
         else {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  GenericData_parameters                                    */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225GenericData_parameters (OOCTXT* pctxt, H225GenericData_parameters* pvalue)
{
   static Asn1SizeCnst lsize1 = { 0, 1, 512, 0 };
   int stat = ASN_OK;
   H225EnumeratedParameter* pdata;
   ASN1UINT count = 0;
   ASN1UINT xx1;

   /* decode length determinant */

   stat = addSizeConstraint (pctxt, &lsize1);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   stat = decodeLength (pctxt, &count);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* decode elements */

   dListInit (pvalue);

   for (xx1 = 0; xx1 < count; xx1++) {
      rtInvokeStartElement (pctxt, "elem", xx1);

      pdata = ALLOC_ASN1ELEMDNODE (pctxt, H225EnumeratedParameter);
      if (pdata == NULL)
         return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

      stat = asn1PD_H225EnumeratedParameter (pctxt, pdata);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      rtInvokeEndElement (pctxt, "elem", xx1);

      dListAppendNode (pctxt, pvalue, pdata);
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  GenericData                                               */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225GenericData (OOCTXT* pctxt, H225GenericData* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.parametersPresent = optbit;

   /* decode id */

   rtInvokeStartElement (pctxt, "id", -1);

   stat = asn1PD_H225GenericIdentifier (pctxt, &pvalue->id);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "id", -1);

   /* decode parameters */

   if (pvalue->m.parametersPresent) {
      rtInvokeStartElement (pctxt, "parameters", -1);

      stat = asn1PD_H225GenericData_parameters (pctxt, &pvalue->parameters);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "parameters", -1);
   }

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
            pOpenType->numocts = openType.numocts;
            pOpenType->data = openType.data;
            dListAppend (pctxt, &pvalue->extElem1, pOpenType);
         }
         else {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  FeatureDescriptor                                         */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225FeatureDescriptor (OOCTXT* pctxt, H225FeatureDescriptor* pvalue)
{
   int stat = ASN_OK;

   stat = asn1PD_H225GenericData (pctxt, pvalue);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  FastStartToken                                            */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225FastStartToken (OOCTXT* pctxt, H225FastStartToken* pvalue)
{
   int stat = ASN_OK;

   stat = asn1PD_H235ClearToken (pctxt, pvalue);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  EncodedFastStartToken                                     */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225EncodedFastStartToken (OOCTXT* pctxt, H225EncodedFastStartToken* pvalue)
{
   int stat = ASN_OK;

   stat = decodeOpenType (pctxt, &pvalue->data, &pvalue->numocts);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   rtInvokeOpenTypeValue
      (pctxt, pvalue->numocts, pvalue->data);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  H323_UserInformation_user_data_user_information           */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225H323_UserInformation_user_data_user_information (OOCTXT* pctxt, H225H323_UserInformation_user_data_user_information* pvalue)
{
   static Asn1SizeCnst lsize1 = { 0, 1, 131, 0 };
   int stat = ASN_OK;

   stat = addSizeConstraint (pctxt, &lsize1);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   stat = decodeOctetString (pctxt,
                          &pvalue->numocts,
                          pvalue->data,
                          sizeof(pvalue->data));
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   rtInvokeOctStrValue (pctxt, pvalue->numocts, pvalue->data);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  EndpointType_set                                          */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225EndpointType_set (OOCTXT* pctxt, H225EndpointType_set* pvalue)
{
   static Asn1SizeCnst lsize1 = { 0, 32, 32, 0 };
   int stat = ASN_OK;

   stat = addSizeConstraint (pctxt, &lsize1);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   stat = decodeBitString (pctxt,
                        &pvalue->numbits,
                        pvalue->data,
                        sizeof(pvalue->data));
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeBitStrValue (pctxt, pvalue->numbits, pvalue->data);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  VendorIdentifier_productId                                */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225VendorIdentifier_productId (OOCTXT* pctxt, H225VendorIdentifier_productId* pvalue)
{
   static Asn1SizeCnst lsize1 = { 0, 1, 256, 0 };
   int stat = ASN_OK;

   stat = addSizeConstraint (pctxt, &lsize1);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   stat = decodeOctetString (pctxt,
                          &pvalue->numocts,
                          pvalue->data,
                          sizeof(pvalue->data));
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   rtInvokeOctStrValue (pctxt, pvalue->numocts, pvalue->data);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  VendorIdentifier_versionId                                */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225VendorIdentifier_versionId (OOCTXT* pctxt, H225VendorIdentifier_versionId* pvalue)
{
   static Asn1SizeCnst lsize1 = { 0, 1, 256, 0 };
   int stat = ASN_OK;

   stat = addSizeConstraint (pctxt, &lsize1);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   stat = decodeOctetString (pctxt,
                          &pvalue->numocts,
                          pvalue->data,
                          sizeof(pvalue->data));
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   rtInvokeOctStrValue (pctxt, pvalue->numocts, pvalue->data);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  CicInfo_cic_element                                       */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225CicInfo_cic_element (OOCTXT* pctxt, H225CicInfo_cic_element* pvalue)
{
   static Asn1SizeCnst lsize1 = { 0, 2, 4, 0 };
   int stat = ASN_OK;

   stat = addSizeConstraint (pctxt, &lsize1);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   stat = decodeOctetString (pctxt,
                          &pvalue->numocts,
                          pvalue->data,
                          sizeof(pvalue->data));
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   rtInvokeOctStrValue (pctxt, pvalue->numocts, pvalue->data);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  CicInfo_pointCode                                         */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225CicInfo_pointCode (OOCTXT* pctxt, H225CicInfo_pointCode* pvalue)
{
   static Asn1SizeCnst lsize1 = { 0, 2, 5, 0 };
   int stat = ASN_OK;

   stat = addSizeConstraint (pctxt, &lsize1);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   stat = decodeOctetString (pctxt,
                          &pvalue->numocts,
                          pvalue->data,
                          sizeof(pvalue->data));
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   rtInvokeOctStrValue (pctxt, pvalue->numocts, pvalue->data);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  CarrierInfo_carrierIdentificationCode                     */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225CarrierInfo_carrierIdentificationCode (OOCTXT* pctxt, H225CarrierInfo_carrierIdentificationCode* pvalue)
{
   static Asn1SizeCnst lsize1 = { 0, 3, 4, 0 };
   int stat = ASN_OK;

   stat = addSizeConstraint (pctxt, &lsize1);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   stat = decodeOctetString (pctxt,
                          &pvalue->numocts,
                          pvalue->data,
                          sizeof(pvalue->data));
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   rtInvokeOctStrValue (pctxt, pvalue->numocts, pvalue->data);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  CallTerminationCause_releaseCompleteCauseIE               */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225CallTerminationCause_releaseCompleteCauseIE (OOCTXT* pctxt, H225CallTerminationCause_releaseCompleteCauseIE* pvalue)
{
   static Asn1SizeCnst lsize1 = { 0, 2, 32, 0 };
   int stat = ASN_OK;

   stat = addSizeConstraint (pctxt, &lsize1);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   stat = decodeOctetString (pctxt,
                          &pvalue->numocts,
                          pvalue->data,
                          sizeof(pvalue->data));
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   rtInvokeOctStrValue (pctxt, pvalue->numocts, pvalue->data);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  _SeqOfH225AliasAddress                                    */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225_SeqOfH225AliasAddress (OOCTXT* pctxt, H225_SeqOfH225AliasAddress* pvalue)
{
   int stat = ASN_OK;
   H225AliasAddress* pdata;
   ASN1UINT count = 0;
   ASN1UINT xx1;
   int lstat;

   dListInit (pvalue);

   for (;;) {
      /* decode length determinant */

      lstat = decodeLength (pctxt, &count);
      if (lstat != ASN_OK && lstat != ASN_OK_FRAG) {
         return LOG_ASN1ERR (pctxt, lstat);
      }

      /* decode elements */

      for (xx1 = 0; xx1 < count; xx1++) {
         rtInvokeStartElement (pctxt, "elem", xx1);

         pdata = ALLOC_ASN1ELEMDNODE (pctxt, H225AliasAddress);
         if (pdata == NULL)
            return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

         stat = asn1PD_H225AliasAddress (pctxt, pdata);
         if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
         rtInvokeEndElement (pctxt, "elem", xx1);

         dListAppendNode (pctxt, pvalue, pdata);
      }

      if(lstat == ASN_OK) break;
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  VendorIdentifier                                          */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225VendorIdentifier (OOCTXT* pctxt, H225VendorIdentifier* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   OOCTXT lctxt2;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.productIdPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.versionIdPresent = optbit;

   /* decode vendor */

   rtInvokeStartElement (pctxt, "vendor", -1);

   stat = asn1PD_H225H221NonStandard (pctxt, &pvalue->vendor);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "vendor", -1);

   /* decode productId */

   if (pvalue->m.productIdPresent) {
      rtInvokeStartElement (pctxt, "productId", -1);

      stat = asn1PD_H225VendorIdentifier_productId (pctxt, &pvalue->productId);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "productId", -1);
   }

   /* decode versionId */

   if (pvalue->m.versionIdPresent) {
      rtInvokeStartElement (pctxt, "versionId", -1);

      stat = asn1PD_H225VendorIdentifier_versionId (pctxt, &pvalue->versionId);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "versionId", -1);
   }

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            if (i < 1 && openType.numocts > 0) {  /* known element */
               copyContext (&lctxt2, pctxt);
               initContextBuffer (pctxt, openType.data, openType.numocts);

               switch (i) {
                  case 0:
                     pvalue->m.enterpriseNumberPresent = 1;

                     rtInvokeStartElement (pctxt, "enterpriseNumber", -1);

                     stat = decodeObjectIdentifier (pctxt, &pvalue->enterpriseNumber);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
                     rtInvokeOidValue (pctxt, pvalue->enterpriseNumber.numids, pvalue->enterpriseNumber.subid);

                     rtInvokeEndElement (pctxt, "enterpriseNumber", -1);
                     break;

                  default:
                     pctxt->buffer.byteIndex += openType.numocts;
               }
               copyContext (pctxt, &lctxt2);
            }
            else {  /* unknown element */
               pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
               pOpenType->numocts = openType.numocts;
               pOpenType->data = openType.data;
               dListAppend (pctxt, &pvalue->extElem1, pOpenType);
            }
         }
         else if (i >= 1) {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  GatekeeperInfo                                            */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225GatekeeperInfo (OOCTXT* pctxt, H225GatekeeperInfo* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.nonStandardDataPresent = optbit;

   /* decode nonStandardData */

   if (pvalue->m.nonStandardDataPresent) {
      rtInvokeStartElement (pctxt, "nonStandardData", -1);

      stat = asn1PD_H225NonStandardParameter (pctxt, &pvalue->nonStandardData);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "nonStandardData", -1);
   }

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
            pOpenType->numocts = openType.numocts;
            pOpenType->data = openType.data;
            dListAppend (pctxt, &pvalue->extElem1, pOpenType);
         }
         else {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  DataRate                                                  */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225DataRate (OOCTXT* pctxt, H225DataRate* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.nonStandardDataPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.channelMultiplierPresent = optbit;

   /* decode nonStandardData */

   if (pvalue->m.nonStandardDataPresent) {
      rtInvokeStartElement (pctxt, "nonStandardData", -1);

      stat = asn1PD_H225NonStandardParameter (pctxt, &pvalue->nonStandardData);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "nonStandardData", -1);
   }

   /* decode channelRate */

   rtInvokeStartElement (pctxt, "channelRate", -1);

   stat = asn1PD_H225BandWidth (pctxt, &pvalue->channelRate);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "channelRate", -1);

   /* decode channelMultiplier */

   if (pvalue->m.channelMultiplierPresent) {
      rtInvokeStartElement (pctxt, "channelMultiplier", -1);

      stat = decodeConsUInt16 (pctxt, &pvalue->channelMultiplier, 1U, 256U);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      rtInvokeUIntValue (pctxt, pvalue->channelMultiplier);

      rtInvokeEndElement (pctxt, "channelMultiplier", -1);
   }

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
            pOpenType->numocts = openType.numocts;
            pOpenType->data = openType.data;
            dListAppend (pctxt, &pvalue->extElem1, pOpenType);
         }
         else {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  _SeqOfH225DataRate                                        */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225_SeqOfH225DataRate (OOCTXT* pctxt, H225_SeqOfH225DataRate* pvalue)
{
   int stat = ASN_OK;
   H225DataRate* pdata;
   ASN1UINT count = 0;
   ASN1UINT xx1;
   int lstat;

   dListInit (pvalue);

   for (;;) {
      /* decode length determinant */

      lstat = decodeLength (pctxt, &count);
      if (lstat != ASN_OK && lstat != ASN_OK_FRAG) {
         return LOG_ASN1ERR (pctxt, lstat);
      }

      /* decode elements */

      for (xx1 = 0; xx1 < count; xx1++) {
         rtInvokeStartElement (pctxt, "elem", xx1);

         pdata = ALLOC_ASN1ELEMDNODE (pctxt, H225DataRate);
         if (pdata == NULL)
            return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

         stat = asn1PD_H225DataRate (pctxt, pdata);
         if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
         rtInvokeEndElement (pctxt, "elem", xx1);

         dListAppendNode (pctxt, pvalue, pdata);
      }

      if(lstat == ASN_OK) break;
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  SupportedPrefix                                           */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225SupportedPrefix (OOCTXT* pctxt, H225SupportedPrefix* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.nonStandardDataPresent = optbit;

   /* decode nonStandardData */

   if (pvalue->m.nonStandardDataPresent) {
      rtInvokeStartElement (pctxt, "nonStandardData", -1);

      stat = asn1PD_H225NonStandardParameter (pctxt, &pvalue->nonStandardData);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "nonStandardData", -1);
   }

   /* decode prefix */

   rtInvokeStartElement (pctxt, "prefix", -1);

   stat = asn1PD_H225AliasAddress (pctxt, &pvalue->prefix);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "prefix", -1);

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
            pOpenType->numocts = openType.numocts;
            pOpenType->data = openType.data;
            dListAppend (pctxt, &pvalue->extElem1, pOpenType);
         }
         else {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  _SeqOfH225SupportedPrefix                                 */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225_SeqOfH225SupportedPrefix (OOCTXT* pctxt, H225_SeqOfH225SupportedPrefix* pvalue)
{
   int stat = ASN_OK;
   H225SupportedPrefix* pdata;
   ASN1UINT count = 0;
   ASN1UINT xx1;
   int lstat;

   dListInit (pvalue);

   for (;;) {
      /* decode length determinant */

      lstat = decodeLength (pctxt, &count);
      if (lstat != ASN_OK && lstat != ASN_OK_FRAG) {
         return LOG_ASN1ERR (pctxt, lstat);
      }

      /* decode elements */

      for (xx1 = 0; xx1 < count; xx1++) {
         rtInvokeStartElement (pctxt, "elem", xx1);

         pdata = ALLOC_ASN1ELEMDNODE (pctxt, H225SupportedPrefix);
         if (pdata == NULL)
            return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

         stat = asn1PD_H225SupportedPrefix (pctxt, pdata);
         if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
         rtInvokeEndElement (pctxt, "elem", xx1);

         dListAppendNode (pctxt, pvalue, pdata);
      }

      if(lstat == ASN_OK) break;
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  H310Caps                                                  */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225H310Caps (OOCTXT* pctxt, H225H310Caps* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   OOCTXT lctxt2;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.nonStandardDataPresent = optbit;

   /* decode nonStandardData */

   if (pvalue->m.nonStandardDataPresent) {
      rtInvokeStartElement (pctxt, "nonStandardData", -1);

      stat = asn1PD_H225NonStandardParameter (pctxt, &pvalue->nonStandardData);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "nonStandardData", -1);
   }

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            if (i < 2 && openType.numocts > 0) {  /* known element */
               copyContext (&lctxt2, pctxt);
               initContextBuffer (pctxt, openType.data, openType.numocts);

               switch (i) {
                  case 0:
                     pvalue->m.dataRatesSupportedPresent = 1;

                     rtInvokeStartElement (pctxt, "dataRatesSupported", -1);

                     stat = asn1PD_H225_SeqOfH225DataRate (pctxt, &pvalue->dataRatesSupported);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "dataRatesSupported", -1);
                     break;

                  case 1:
                     pvalue->m.supportedPrefixesPresent = 1;

                     rtInvokeStartElement (pctxt, "supportedPrefixes", -1);

                     stat = asn1PD_H225_SeqOfH225SupportedPrefix (pctxt, &pvalue->supportedPrefixes);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "supportedPrefixes", -1);
                     break;

                  default:
                     pctxt->buffer.byteIndex += openType.numocts;
               }
               copyContext (pctxt, &lctxt2);
            }
            else {  /* unknown element */
               pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
               pOpenType->numocts = openType.numocts;
               pOpenType->data = openType.data;
               dListAppend (pctxt, &pvalue->extElem1, pOpenType);
            }
         }
         else if (i >= 2) {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  H320Caps                                                  */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225H320Caps (OOCTXT* pctxt, H225H320Caps* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   OOCTXT lctxt2;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.nonStandardDataPresent = optbit;

   /* decode nonStandardData */

   if (pvalue->m.nonStandardDataPresent) {
      rtInvokeStartElement (pctxt, "nonStandardData", -1);

      stat = asn1PD_H225NonStandardParameter (pctxt, &pvalue->nonStandardData);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "nonStandardData", -1);
   }

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            if (i < 2 && openType.numocts > 0) {  /* known element */
               copyContext (&lctxt2, pctxt);
               initContextBuffer (pctxt, openType.data, openType.numocts);

               switch (i) {
                  case 0:
                     pvalue->m.dataRatesSupportedPresent = 1;

                     rtInvokeStartElement (pctxt, "dataRatesSupported", -1);

                     stat = asn1PD_H225_SeqOfH225DataRate (pctxt, &pvalue->dataRatesSupported);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "dataRatesSupported", -1);
                     break;

                  case 1:
                     pvalue->m.supportedPrefixesPresent = 1;

                     rtInvokeStartElement (pctxt, "supportedPrefixes", -1);

                     stat = asn1PD_H225_SeqOfH225SupportedPrefix (pctxt, &pvalue->supportedPrefixes);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "supportedPrefixes", -1);
                     break;

                  default:
                     pctxt->buffer.byteIndex += openType.numocts;
               }
               copyContext (pctxt, &lctxt2);
            }
            else {  /* unknown element */
               pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
               pOpenType->numocts = openType.numocts;
               pOpenType->data = openType.data;
               dListAppend (pctxt, &pvalue->extElem1, pOpenType);
            }
         }
         else if (i >= 2) {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  H321Caps                                                  */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225H321Caps (OOCTXT* pctxt, H225H321Caps* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   OOCTXT lctxt2;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.nonStandardDataPresent = optbit;

   /* decode nonStandardData */

   if (pvalue->m.nonStandardDataPresent) {
      rtInvokeStartElement (pctxt, "nonStandardData", -1);

      stat = asn1PD_H225NonStandardParameter (pctxt, &pvalue->nonStandardData);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "nonStandardData", -1);
   }

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            if (i < 2 && openType.numocts > 0) {  /* known element */
               copyContext (&lctxt2, pctxt);
               initContextBuffer (pctxt, openType.data, openType.numocts);

               switch (i) {
                  case 0:
                     pvalue->m.dataRatesSupportedPresent = 1;

                     rtInvokeStartElement (pctxt, "dataRatesSupported", -1);

                     stat = asn1PD_H225_SeqOfH225DataRate (pctxt, &pvalue->dataRatesSupported);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "dataRatesSupported", -1);
                     break;

                  case 1:
                     pvalue->m.supportedPrefixesPresent = 1;

                     rtInvokeStartElement (pctxt, "supportedPrefixes", -1);

                     stat = asn1PD_H225_SeqOfH225SupportedPrefix (pctxt, &pvalue->supportedPrefixes);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "supportedPrefixes", -1);
                     break;

                  default:
                     pctxt->buffer.byteIndex += openType.numocts;
               }
               copyContext (pctxt, &lctxt2);
            }
            else {  /* unknown element */
               pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
               pOpenType->numocts = openType.numocts;
               pOpenType->data = openType.data;
               dListAppend (pctxt, &pvalue->extElem1, pOpenType);
            }
         }
         else if (i >= 2) {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  H322Caps                                                  */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225H322Caps (OOCTXT* pctxt, H225H322Caps* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   OOCTXT lctxt2;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.nonStandardDataPresent = optbit;

   /* decode nonStandardData */

   if (pvalue->m.nonStandardDataPresent) {
      rtInvokeStartElement (pctxt, "nonStandardData", -1);

      stat = asn1PD_H225NonStandardParameter (pctxt, &pvalue->nonStandardData);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "nonStandardData", -1);
   }

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            if (i < 2 && openType.numocts > 0) {  /* known element */
               copyContext (&lctxt2, pctxt);
               initContextBuffer (pctxt, openType.data, openType.numocts);

               switch (i) {
                  case 0:
                     pvalue->m.dataRatesSupportedPresent = 1;

                     rtInvokeStartElement (pctxt, "dataRatesSupported", -1);

                     stat = asn1PD_H225_SeqOfH225DataRate (pctxt, &pvalue->dataRatesSupported);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "dataRatesSupported", -1);
                     break;

                  case 1:
                     pvalue->m.supportedPrefixesPresent = 1;

                     rtInvokeStartElement (pctxt, "supportedPrefixes", -1);

                     stat = asn1PD_H225_SeqOfH225SupportedPrefix (pctxt, &pvalue->supportedPrefixes);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "supportedPrefixes", -1);
                     break;

                  default:
                     pctxt->buffer.byteIndex += openType.numocts;
               }
               copyContext (pctxt, &lctxt2);
            }
            else {  /* unknown element */
               pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
               pOpenType->numocts = openType.numocts;
               pOpenType->data = openType.data;
               dListAppend (pctxt, &pvalue->extElem1, pOpenType);
            }
         }
         else if (i >= 2) {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  H323Caps                                                  */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225H323Caps (OOCTXT* pctxt, H225H323Caps* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   OOCTXT lctxt2;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.nonStandardDataPresent = optbit;

   /* decode nonStandardData */

   if (pvalue->m.nonStandardDataPresent) {
      rtInvokeStartElement (pctxt, "nonStandardData", -1);

      stat = asn1PD_H225NonStandardParameter (pctxt, &pvalue->nonStandardData);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "nonStandardData", -1);
   }

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            if (i < 2 && openType.numocts > 0) {  /* known element */
               copyContext (&lctxt2, pctxt);
               initContextBuffer (pctxt, openType.data, openType.numocts);

               switch (i) {
                  case 0:
                     pvalue->m.dataRatesSupportedPresent = 1;

                     rtInvokeStartElement (pctxt, "dataRatesSupported", -1);

                     stat = asn1PD_H225_SeqOfH225DataRate (pctxt, &pvalue->dataRatesSupported);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "dataRatesSupported", -1);
                     break;

                  case 1:
                     pvalue->m.supportedPrefixesPresent = 1;

                     rtInvokeStartElement (pctxt, "supportedPrefixes", -1);

                     stat = asn1PD_H225_SeqOfH225SupportedPrefix (pctxt, &pvalue->supportedPrefixes);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "supportedPrefixes", -1);
                     break;

                  default:
                     pctxt->buffer.byteIndex += openType.numocts;
               }
               copyContext (pctxt, &lctxt2);
            }
            else {  /* unknown element */
               pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
               pOpenType->numocts = openType.numocts;
               pOpenType->data = openType.data;
               dListAppend (pctxt, &pvalue->extElem1, pOpenType);
            }
         }
         else if (i >= 2) {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  H324Caps                                                  */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225H324Caps (OOCTXT* pctxt, H225H324Caps* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   OOCTXT lctxt2;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.nonStandardDataPresent = optbit;

   /* decode nonStandardData */

   if (pvalue->m.nonStandardDataPresent) {
      rtInvokeStartElement (pctxt, "nonStandardData", -1);

      stat = asn1PD_H225NonStandardParameter (pctxt, &pvalue->nonStandardData);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "nonStandardData", -1);
   }

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            if (i < 2 && openType.numocts > 0) {  /* known element */
               copyContext (&lctxt2, pctxt);
               initContextBuffer (pctxt, openType.data, openType.numocts);

               switch (i) {
                  case 0:
                     pvalue->m.dataRatesSupportedPresent = 1;

                     rtInvokeStartElement (pctxt, "dataRatesSupported", -1);

                     stat = asn1PD_H225_SeqOfH225DataRate (pctxt, &pvalue->dataRatesSupported);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "dataRatesSupported", -1);
                     break;

                  case 1:
                     pvalue->m.supportedPrefixesPresent = 1;

                     rtInvokeStartElement (pctxt, "supportedPrefixes", -1);

                     stat = asn1PD_H225_SeqOfH225SupportedPrefix (pctxt, &pvalue->supportedPrefixes);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "supportedPrefixes", -1);
                     break;

                  default:
                     pctxt->buffer.byteIndex += openType.numocts;
               }
               copyContext (pctxt, &lctxt2);
            }
            else {  /* unknown element */
               pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
               pOpenType->numocts = openType.numocts;
               pOpenType->data = openType.data;
               dListAppend (pctxt, &pvalue->extElem1, pOpenType);
            }
         }
         else if (i >= 2) {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  VoiceCaps                                                 */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225VoiceCaps (OOCTXT* pctxt, H225VoiceCaps* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   OOCTXT lctxt2;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.nonStandardDataPresent = optbit;

   /* decode nonStandardData */

   if (pvalue->m.nonStandardDataPresent) {
      rtInvokeStartElement (pctxt, "nonStandardData", -1);

      stat = asn1PD_H225NonStandardParameter (pctxt, &pvalue->nonStandardData);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "nonStandardData", -1);
   }

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            if (i < 2 && openType.numocts > 0) {  /* known element */
               copyContext (&lctxt2, pctxt);
               initContextBuffer (pctxt, openType.data, openType.numocts);

               switch (i) {
                  case 0:
                     pvalue->m.dataRatesSupportedPresent = 1;

                     rtInvokeStartElement (pctxt, "dataRatesSupported", -1);

                     stat = asn1PD_H225_SeqOfH225DataRate (pctxt, &pvalue->dataRatesSupported);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "dataRatesSupported", -1);
                     break;

                  case 1:
                     pvalue->m.supportedPrefixesPresent = 1;

                     rtInvokeStartElement (pctxt, "supportedPrefixes", -1);

                     stat = asn1PD_H225_SeqOfH225SupportedPrefix (pctxt, &pvalue->supportedPrefixes);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "supportedPrefixes", -1);
                     break;

                  default:
                     pctxt->buffer.byteIndex += openType.numocts;
               }
               copyContext (pctxt, &lctxt2);
            }
            else {  /* unknown element */
               pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
               pOpenType->numocts = openType.numocts;
               pOpenType->data = openType.data;
               dListAppend (pctxt, &pvalue->extElem1, pOpenType);
            }
         }
         else if (i >= 2) {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  T120OnlyCaps                                              */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225T120OnlyCaps (OOCTXT* pctxt, H225T120OnlyCaps* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   OOCTXT lctxt2;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.nonStandardDataPresent = optbit;

   /* decode nonStandardData */

   if (pvalue->m.nonStandardDataPresent) {
      rtInvokeStartElement (pctxt, "nonStandardData", -1);

      stat = asn1PD_H225NonStandardParameter (pctxt, &pvalue->nonStandardData);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "nonStandardData", -1);
   }

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            if (i < 2 && openType.numocts > 0) {  /* known element */
               copyContext (&lctxt2, pctxt);
               initContextBuffer (pctxt, openType.data, openType.numocts);

               switch (i) {
                  case 0:
                     pvalue->m.dataRatesSupportedPresent = 1;

                     rtInvokeStartElement (pctxt, "dataRatesSupported", -1);

                     stat = asn1PD_H225_SeqOfH225DataRate (pctxt, &pvalue->dataRatesSupported);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "dataRatesSupported", -1);
                     break;

                  case 1:
                     pvalue->m.supportedPrefixesPresent = 1;

                     rtInvokeStartElement (pctxt, "supportedPrefixes", -1);

                     stat = asn1PD_H225_SeqOfH225SupportedPrefix (pctxt, &pvalue->supportedPrefixes);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "supportedPrefixes", -1);
                     break;

                  default:
                     pctxt->buffer.byteIndex += openType.numocts;
               }
               copyContext (pctxt, &lctxt2);
            }
            else {  /* unknown element */
               pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
               pOpenType->numocts = openType.numocts;
               pOpenType->data = openType.data;
               dListAppend (pctxt, &pvalue->extElem1, pOpenType);
            }
         }
         else if (i >= 2) {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  NonStandardProtocol                                       */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225NonStandardProtocol (OOCTXT* pctxt, H225NonStandardProtocol* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.nonStandardDataPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.dataRatesSupportedPresent = optbit;

   /* decode nonStandardData */

   if (pvalue->m.nonStandardDataPresent) {
      rtInvokeStartElement (pctxt, "nonStandardData", -1);

      stat = asn1PD_H225NonStandardParameter (pctxt, &pvalue->nonStandardData);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "nonStandardData", -1);
   }

   /* decode dataRatesSupported */

   if (pvalue->m.dataRatesSupportedPresent) {
      rtInvokeStartElement (pctxt, "dataRatesSupported", -1);

      stat = asn1PD_H225_SeqOfH225DataRate (pctxt, &pvalue->dataRatesSupported);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "dataRatesSupported", -1);
   }

   /* decode supportedPrefixes */

   rtInvokeStartElement (pctxt, "supportedPrefixes", -1);

   stat = asn1PD_H225_SeqOfH225SupportedPrefix (pctxt, &pvalue->supportedPrefixes);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "supportedPrefixes", -1);

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
            pOpenType->numocts = openType.numocts;
            pOpenType->data = openType.data;
            dListAppend (pctxt, &pvalue->extElem1, pOpenType);
         }
         else {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  T38FaxAnnexbOnlyCaps                                      */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225T38FaxAnnexbOnlyCaps (OOCTXT* pctxt, H225T38FaxAnnexbOnlyCaps* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.nonStandardDataPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.dataRatesSupportedPresent = optbit;

   /* decode nonStandardData */

   if (pvalue->m.nonStandardDataPresent) {
      rtInvokeStartElement (pctxt, "nonStandardData", -1);

      stat = asn1PD_H225NonStandardParameter (pctxt, &pvalue->nonStandardData);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "nonStandardData", -1);
   }

   /* decode dataRatesSupported */

   if (pvalue->m.dataRatesSupportedPresent) {
      rtInvokeStartElement (pctxt, "dataRatesSupported", -1);

      stat = asn1PD_H225_SeqOfH225DataRate (pctxt, &pvalue->dataRatesSupported);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "dataRatesSupported", -1);
   }

   /* decode supportedPrefixes */

   rtInvokeStartElement (pctxt, "supportedPrefixes", -1);

   stat = asn1PD_H225_SeqOfH225SupportedPrefix (pctxt, &pvalue->supportedPrefixes);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "supportedPrefixes", -1);

   /* decode t38FaxProtocol */

   rtInvokeStartElement (pctxt, "t38FaxProtocol", -1);

   stat = asn1PD_H245DataProtocolCapability (pctxt, &pvalue->t38FaxProtocol);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "t38FaxProtocol", -1);

   /* decode t38FaxProfile */

   rtInvokeStartElement (pctxt, "t38FaxProfile", -1);

   stat = asn1PD_H245T38FaxProfile (pctxt, &pvalue->t38FaxProfile);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "t38FaxProfile", -1);

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
            pOpenType->numocts = openType.numocts;
            pOpenType->data = openType.data;
            dListAppend (pctxt, &pvalue->extElem1, pOpenType);
         }
         else {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  SIPCaps                                                   */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225SIPCaps (OOCTXT* pctxt, H225SIPCaps* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.nonStandardDataPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.dataRatesSupportedPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.supportedPrefixesPresent = optbit;

   /* decode nonStandardData */

   if (pvalue->m.nonStandardDataPresent) {
      rtInvokeStartElement (pctxt, "nonStandardData", -1);

      stat = asn1PD_H225NonStandardParameter (pctxt, &pvalue->nonStandardData);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "nonStandardData", -1);
   }

   /* decode dataRatesSupported */

   if (pvalue->m.dataRatesSupportedPresent) {
      rtInvokeStartElement (pctxt, "dataRatesSupported", -1);

      stat = asn1PD_H225_SeqOfH225DataRate (pctxt, &pvalue->dataRatesSupported);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "dataRatesSupported", -1);
   }

   /* decode supportedPrefixes */

   if (pvalue->m.supportedPrefixesPresent) {
      rtInvokeStartElement (pctxt, "supportedPrefixes", -1);

      stat = asn1PD_H225_SeqOfH225SupportedPrefix (pctxt, &pvalue->supportedPrefixes);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "supportedPrefixes", -1);
   }

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
            pOpenType->numocts = openType.numocts;
            pOpenType->data = openType.data;
            dListAppend (pctxt, &pvalue->extElem1, pOpenType);
         }
         else {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  SupportedProtocols                                        */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225SupportedProtocols (OOCTXT* pctxt, H225SupportedProtocols* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT ui;
   ASN1OpenType openType;
   ASN1BOOL extbit;
   OOCTXT lctxt;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   if (!extbit) {
      stat = decodeConsUnsigned (pctxt, &ui, 0, 8);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      else pvalue->t = ui + 1;

      switch (ui) {
         /* nonStandardData */
         case 0:
            rtInvokeStartElement (pctxt, "nonStandardData", -1);

            pvalue->u.nonStandardData = ALLOC_ASN1ELEM (pctxt, H225NonStandardParameter);
            if (pvalue->u.nonStandardData == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225NonStandardParameter (pctxt, pvalue->u.nonStandardData);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "nonStandardData", -1);

            break;

         /* h310 */
         case 1:
            rtInvokeStartElement (pctxt, "h310", -1);

            pvalue->u.h310 = ALLOC_ASN1ELEM (pctxt, H225H310Caps);
            if (pvalue->u.h310 == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225H310Caps (pctxt, pvalue->u.h310);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "h310", -1);

            break;

         /* h320 */
         case 2:
            rtInvokeStartElement (pctxt, "h320", -1);

            pvalue->u.h320 = ALLOC_ASN1ELEM (pctxt, H225H320Caps);
            if (pvalue->u.h320 == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225H320Caps (pctxt, pvalue->u.h320);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "h320", -1);

            break;

         /* h321 */
         case 3:
            rtInvokeStartElement (pctxt, "h321", -1);

            pvalue->u.h321 = ALLOC_ASN1ELEM (pctxt, H225H321Caps);
            if (pvalue->u.h321 == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225H321Caps (pctxt, pvalue->u.h321);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "h321", -1);

            break;

         /* h322 */
         case 4:
            rtInvokeStartElement (pctxt, "h322", -1);

            pvalue->u.h322 = ALLOC_ASN1ELEM (pctxt, H225H322Caps);
            if (pvalue->u.h322 == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225H322Caps (pctxt, pvalue->u.h322);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "h322", -1);

            break;

         /* h323 */
         case 5:
            rtInvokeStartElement (pctxt, "h323", -1);

            pvalue->u.h323 = ALLOC_ASN1ELEM (pctxt, H225H323Caps);
            if (pvalue->u.h323 == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225H323Caps (pctxt, pvalue->u.h323);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "h323", -1);

            break;

         /* h324 */
         case 6:
            rtInvokeStartElement (pctxt, "h324", -1);

            pvalue->u.h324 = ALLOC_ASN1ELEM (pctxt, H225H324Caps);
            if (pvalue->u.h324 == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225H324Caps (pctxt, pvalue->u.h324);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "h324", -1);

            break;

         /* voice */
         case 7:
            rtInvokeStartElement (pctxt, "voice", -1);

            pvalue->u.voice = ALLOC_ASN1ELEM (pctxt, H225VoiceCaps);
            if (pvalue->u.voice == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225VoiceCaps (pctxt, pvalue->u.voice);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "voice", -1);

            break;

         /* t120_only */
         case 8:
            rtInvokeStartElement (pctxt, "t120_only", -1);

            pvalue->u.t120_only = ALLOC_ASN1ELEM (pctxt, H225T120OnlyCaps);
            if (pvalue->u.t120_only == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225T120OnlyCaps (pctxt, pvalue->u.t120_only);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "t120_only", -1);

            break;

         default:
            return LOG_ASN1ERR (pctxt, ASN_E_INVOPT);
      }
   }
   else {
      stat = decodeSmallNonNegWholeNumber (pctxt, &ui);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      else pvalue->t = ui + 10;

      stat = decodeByteAlign (pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      copyContext (&lctxt, pctxt);
      initContextBuffer (pctxt, openType.data, openType.numocts);

      switch (pvalue->t) {
         /* nonStandardProtocol */
         case 10:
            rtInvokeStartElement (pctxt, "nonStandardProtocol", -1);

            pvalue->u.nonStandardProtocol = ALLOC_ASN1ELEM (pctxt, H225NonStandardProtocol);
            if (pvalue->u.nonStandardProtocol == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225NonStandardProtocol (pctxt, pvalue->u.nonStandardProtocol);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "nonStandardProtocol", -1);

            break;

         /* t38FaxAnnexbOnly */
         case 11:
            rtInvokeStartElement (pctxt, "t38FaxAnnexbOnly", -1);

            pvalue->u.t38FaxAnnexbOnly = ALLOC_ASN1ELEM (pctxt, H225T38FaxAnnexbOnlyCaps);
            if (pvalue->u.t38FaxAnnexbOnly == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225T38FaxAnnexbOnlyCaps (pctxt, pvalue->u.t38FaxAnnexbOnly);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "t38FaxAnnexbOnly", -1);

            break;

         /* sip */
         case 12:
            rtInvokeStartElement (pctxt, "sip", -1);

            pvalue->u.sip = ALLOC_ASN1ELEM (pctxt, H225SIPCaps);
            if (pvalue->u.sip == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225SIPCaps (pctxt, pvalue->u.sip);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "sip", -1);

            break;

         default:
            pvalue->u.extElem1 = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
            if (pvalue->u.extElem1 == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            pvalue->u.extElem1->data = openType.data;
            pvalue->u.extElem1->numocts = openType.numocts;

      }

      copyContext (pctxt, &lctxt);
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  _SeqOfH225SupportedProtocols                              */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225_SeqOfH225SupportedProtocols (OOCTXT* pctxt, H225_SeqOfH225SupportedProtocols* pvalue)
{
   int stat = ASN_OK;
   H225SupportedProtocols* pdata;
   ASN1UINT count = 0;
   ASN1UINT xx1;
   int lstat;

   dListInit (pvalue);

   for (;;) {
      /* decode length determinant */

      lstat = decodeLength (pctxt, &count);
      if (lstat != ASN_OK && lstat != ASN_OK_FRAG) {
         return LOG_ASN1ERR (pctxt, lstat);
      }

      /* decode elements */

      for (xx1 = 0; xx1 < count; xx1++) {
         rtInvokeStartElement (pctxt, "elem", xx1);

         pdata = ALLOC_ASN1ELEMDNODE (pctxt, H225SupportedProtocols);
         if (pdata == NULL)
            return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

         stat = asn1PD_H225SupportedProtocols (pctxt, pdata);
         if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
         rtInvokeEndElement (pctxt, "elem", xx1);

         dListAppendNode (pctxt, pvalue, pdata);
      }

      if(lstat == ASN_OK) break;
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  GatewayInfo                                               */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225GatewayInfo (OOCTXT* pctxt, H225GatewayInfo* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.protocolPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.nonStandardDataPresent = optbit;

   /* decode protocol */

   if (pvalue->m.protocolPresent) {
      rtInvokeStartElement (pctxt, "protocol", -1);

      stat = asn1PD_H225_SeqOfH225SupportedProtocols (pctxt, &pvalue->protocol);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "protocol", -1);
   }

   /* decode nonStandardData */

   if (pvalue->m.nonStandardDataPresent) {
      rtInvokeStartElement (pctxt, "nonStandardData", -1);

      stat = asn1PD_H225NonStandardParameter (pctxt, &pvalue->nonStandardData);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "nonStandardData", -1);
   }

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
            pOpenType->numocts = openType.numocts;
            pOpenType->data = openType.data;
            dListAppend (pctxt, &pvalue->extElem1, pOpenType);
         }
         else {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  McuInfo                                                   */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225McuInfo (OOCTXT* pctxt, H225McuInfo* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   OOCTXT lctxt2;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.nonStandardDataPresent = optbit;

   /* decode nonStandardData */

   if (pvalue->m.nonStandardDataPresent) {
      rtInvokeStartElement (pctxt, "nonStandardData", -1);

      stat = asn1PD_H225NonStandardParameter (pctxt, &pvalue->nonStandardData);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "nonStandardData", -1);
   }

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            if (i < 1 && openType.numocts > 0) {  /* known element */
               copyContext (&lctxt2, pctxt);
               initContextBuffer (pctxt, openType.data, openType.numocts);

               switch (i) {
                  case 0:
                     pvalue->m.protocolPresent = 1;

                     rtInvokeStartElement (pctxt, "protocol", -1);

                     stat = asn1PD_H225_SeqOfH225SupportedProtocols (pctxt, &pvalue->protocol);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "protocol", -1);
                     break;

                  default:
                     pctxt->buffer.byteIndex += openType.numocts;
               }
               copyContext (pctxt, &lctxt2);
            }
            else {  /* unknown element */
               pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
               pOpenType->numocts = openType.numocts;
               pOpenType->data = openType.data;
               dListAppend (pctxt, &pvalue->extElem1, pOpenType);
            }
         }
         else if (i >= 1) {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  TerminalInfo                                              */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225TerminalInfo (OOCTXT* pctxt, H225TerminalInfo* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.nonStandardDataPresent = optbit;

   /* decode nonStandardData */

   if (pvalue->m.nonStandardDataPresent) {
      rtInvokeStartElement (pctxt, "nonStandardData", -1);

      stat = asn1PD_H225NonStandardParameter (pctxt, &pvalue->nonStandardData);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "nonStandardData", -1);
   }

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
            pOpenType->numocts = openType.numocts;
            pOpenType->data = openType.data;
            dListAppend (pctxt, &pvalue->extElem1, pOpenType);
         }
         else {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  TunnelledProtocolAlternateIdentifier                      */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225TunnelledProtocolAlternateIdentifier (OOCTXT* pctxt, H225TunnelledProtocolAlternateIdentifier* pvalue)
{
   static Asn1SizeCnst protocolType_lsize1 = { 0, 1, 64, 0 };
   static Asn1SizeCnst protocolVariant_lsize1 = { 0, 1, 64, 0 };
   int stat = ASN_OK;
   OOCTXT lctxt;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.protocolVariantPresent = optbit;

   /* decode protocolType */

   rtInvokeStartElement (pctxt, "protocolType", -1);

   stat = addSizeConstraint (pctxt, &protocolType_lsize1);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   stat = decodeConstrainedStringEx (pctxt, &pvalue->protocolType, 0, 8, 7, 7);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   rtInvokeCharStrValue (pctxt, pvalue->protocolType);

   rtInvokeEndElement (pctxt, "protocolType", -1);

   /* decode protocolVariant */

   if (pvalue->m.protocolVariantPresent) {
      rtInvokeStartElement (pctxt, "protocolVariant", -1);

      stat = addSizeConstraint (pctxt, &protocolVariant_lsize1);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = decodeConstrainedStringEx (pctxt, &pvalue->protocolVariant, 0, 8, 7, 7);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      rtInvokeCharStrValue (pctxt, pvalue->protocolVariant);

      rtInvokeEndElement (pctxt, "protocolVariant", -1);
   }

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
            pOpenType->numocts = openType.numocts;
            pOpenType->data = openType.data;
            dListAppend (pctxt, &pvalue->extElem1, pOpenType);
         }
         else {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  TunnelledProtocol_id                                      */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225TunnelledProtocol_id (OOCTXT* pctxt, H225TunnelledProtocol_id* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT ui;
   ASN1OpenType openType;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   if (!extbit) {
      stat = decodeConsUnsigned (pctxt, &ui, 0, 1);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      else pvalue->t = ui + 1;

      switch (ui) {
         /* tunnelledProtocolObjectID */
         case 0:
            rtInvokeStartElement (pctxt, "tunnelledProtocolObjectID", -1);

            pvalue->u.tunnelledProtocolObjectID = ALLOC_ASN1ELEM (pctxt, ASN1OBJID);
            if (pvalue->u.tunnelledProtocolObjectID == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = decodeObjectIdentifier (pctxt, pvalue->u.tunnelledProtocolObjectID);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
            rtInvokeOidValue (pctxt, pvalue->u.tunnelledProtocolObjectID->numids, pvalue->u.tunnelledProtocolObjectID->subid);

            rtInvokeEndElement (pctxt, "tunnelledProtocolObjectID", -1);

            break;

         /* tunnelledProtocolAlternateID */
         case 1:
            rtInvokeStartElement (pctxt, "tunnelledProtocolAlternateID", -1);

            pvalue->u.tunnelledProtocolAlternateID = ALLOC_ASN1ELEM (pctxt, H225TunnelledProtocolAlternateIdentifier);
            if (pvalue->u.tunnelledProtocolAlternateID == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225TunnelledProtocolAlternateIdentifier (pctxt, pvalue->u.tunnelledProtocolAlternateID);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "tunnelledProtocolAlternateID", -1);

            break;

         default:
            return LOG_ASN1ERR (pctxt, ASN_E_INVOPT);
      }
   }
   else {
      stat = decodeSmallNonNegWholeNumber (pctxt, &ui);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      else pvalue->t = ui + 3;

      stat = decodeByteAlign (pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      pvalue->u.extElem1 = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
      if (pvalue->u.extElem1 == NULL)
         return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

      pvalue->u.extElem1->data = openType.data;
      pvalue->u.extElem1->numocts = openType.numocts;

   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  TunnelledProtocol                                         */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225TunnelledProtocol (OOCTXT* pctxt, H225TunnelledProtocol* pvalue)
{
   static Asn1SizeCnst subIdentifier_lsize1 = { 0, 1, 64, 0 };
   int stat = ASN_OK;
   OOCTXT lctxt;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.subIdentifierPresent = optbit;

   /* decode id */

   rtInvokeStartElement (pctxt, "id", -1);

   stat = asn1PD_H225TunnelledProtocol_id (pctxt, &pvalue->id);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "id", -1);

   /* decode subIdentifier */

   if (pvalue->m.subIdentifierPresent) {
      rtInvokeStartElement (pctxt, "subIdentifier", -1);

      stat = addSizeConstraint (pctxt, &subIdentifier_lsize1);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = decodeConstrainedStringEx (pctxt, &pvalue->subIdentifier, 0, 8, 7, 7);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      rtInvokeCharStrValue (pctxt, pvalue->subIdentifier);

      rtInvokeEndElement (pctxt, "subIdentifier", -1);
   }

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
            pOpenType->numocts = openType.numocts;
            pOpenType->data = openType.data;
            dListAppend (pctxt, &pvalue->extElem1, pOpenType);
         }
         else {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  _SeqOfH225TunnelledProtocol                               */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225_SeqOfH225TunnelledProtocol (OOCTXT* pctxt, H225_SeqOfH225TunnelledProtocol* pvalue)
{
   int stat = ASN_OK;
   H225TunnelledProtocol* pdata;
   ASN1UINT count = 0;
   ASN1UINT xx1;
   int lstat;

   dListInit (pvalue);

   for (;;) {
      /* decode length determinant */

      lstat = decodeLength (pctxt, &count);
      if (lstat != ASN_OK && lstat != ASN_OK_FRAG) {
         return LOG_ASN1ERR (pctxt, lstat);
      }

      /* decode elements */

      for (xx1 = 0; xx1 < count; xx1++) {
         rtInvokeStartElement (pctxt, "elem", xx1);

         pdata = ALLOC_ASN1ELEMDNODE (pctxt, H225TunnelledProtocol);
         if (pdata == NULL)
            return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

         stat = asn1PD_H225TunnelledProtocol (pctxt, pdata);
         if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
         rtInvokeEndElement (pctxt, "elem", xx1);

         dListAppendNode (pctxt, pvalue, pdata);
      }

      if(lstat == ASN_OK) break;
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  EndpointType                                              */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225EndpointType (OOCTXT* pctxt, H225EndpointType* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   OOCTXT lctxt2;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.nonStandardDataPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.vendorPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.gatekeeperPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.gatewayPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.mcuPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.terminalPresent = optbit;

   /* decode nonStandardData */

   if (pvalue->m.nonStandardDataPresent) {
      rtInvokeStartElement (pctxt, "nonStandardData", -1);

      stat = asn1PD_H225NonStandardParameter (pctxt, &pvalue->nonStandardData);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "nonStandardData", -1);
   }

   /* decode vendor */

   if (pvalue->m.vendorPresent) {
      rtInvokeStartElement (pctxt, "vendor", -1);

      stat = asn1PD_H225VendorIdentifier (pctxt, &pvalue->vendor);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "vendor", -1);
   }

   /* decode gatekeeper */

   if (pvalue->m.gatekeeperPresent) {
      rtInvokeStartElement (pctxt, "gatekeeper", -1);

      stat = asn1PD_H225GatekeeperInfo (pctxt, &pvalue->gatekeeper);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "gatekeeper", -1);
   }

   /* decode gateway */

   if (pvalue->m.gatewayPresent) {
      rtInvokeStartElement (pctxt, "gateway", -1);

      stat = asn1PD_H225GatewayInfo (pctxt, &pvalue->gateway);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "gateway", -1);
   }

   /* decode mcu */

   if (pvalue->m.mcuPresent) {
      rtInvokeStartElement (pctxt, "mcu", -1);

      stat = asn1PD_H225McuInfo (pctxt, &pvalue->mcu);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "mcu", -1);
   }

   /* decode terminal */

   if (pvalue->m.terminalPresent) {
      rtInvokeStartElement (pctxt, "terminal", -1);

      stat = asn1PD_H225TerminalInfo (pctxt, &pvalue->terminal);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "terminal", -1);
   }

   /* decode mc */

   rtInvokeStartElement (pctxt, "mc", -1);

   stat = DECODEBIT (pctxt, &pvalue->mc);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   rtInvokeBoolValue (pctxt, pvalue->mc);

   rtInvokeEndElement (pctxt, "mc", -1);

   /* decode undefinedNode */

   rtInvokeStartElement (pctxt, "undefinedNode", -1);

   stat = DECODEBIT (pctxt, &pvalue->undefinedNode);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   rtInvokeBoolValue (pctxt, pvalue->undefinedNode);

   rtInvokeEndElement (pctxt, "undefinedNode", -1);

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            if (i < 2 && openType.numocts > 0) {  /* known element */
               copyContext (&lctxt2, pctxt);
               initContextBuffer (pctxt, openType.data, openType.numocts);

               switch (i) {
                  case 0:
                     pvalue->m.setPresent = 1;

                     rtInvokeStartElement (pctxt, "set", -1);

                     stat = asn1PD_H225EndpointType_set (pctxt, &pvalue->set);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "set", -1);
                     break;

                  case 1:
                     pvalue->m.supportedTunnelledProtocolsPresent = 1;

                     rtInvokeStartElement (pctxt, "supportedTunnelledProtocols", -1);

                     stat = asn1PD_H225_SeqOfH225TunnelledProtocol (pctxt, &pvalue->supportedTunnelledProtocols);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "supportedTunnelledProtocols", -1);
                     break;

                  default:
                     pctxt->buffer.byteIndex += openType.numocts;
               }
               copyContext (pctxt, &lctxt2);
            }
            else {  /* unknown element */
               pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
               pOpenType->numocts = openType.numocts;
               pOpenType->data = openType.data;
               dListAppend (pctxt, &pvalue->extElem1, pOpenType);
            }
         }
         else if (i >= 2) {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  _SeqOfH225CallReferenceValue                              */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225_SeqOfH225CallReferenceValue (OOCTXT* pctxt, H225_SeqOfH225CallReferenceValue* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT xx1;

   /* decode length determinant */

   stat = decodeLength (pctxt, &pvalue->n);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* decode elements */

   ALLOC_ASN1ARRAY (pctxt, pvalue, H225CallReferenceValue);

   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      rtInvokeStartElement (pctxt, "elem", xx1);

      stat = asn1PD_H225CallReferenceValue (pctxt, &pvalue->elem[xx1]);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      rtInvokeEndElement (pctxt, "elem", xx1);

   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  Setup_UUIE_conferenceGoal                                 */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225Setup_UUIE_conferenceGoal (OOCTXT* pctxt, H225Setup_UUIE_conferenceGoal* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT ui;
   ASN1OpenType openType;
   ASN1BOOL extbit;
   OOCTXT lctxt;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   if (!extbit) {
      stat = decodeConsUnsigned (pctxt, &ui, 0, 2);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      else pvalue->t = ui + 1;

      switch (ui) {
         /* create */
         case 0:
            rtInvokeStartElement (pctxt, "create", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "create", -1);

            break;

         /* join */
         case 1:
            rtInvokeStartElement (pctxt, "join", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "join", -1);

            break;

         /* invite */
         case 2:
            rtInvokeStartElement (pctxt, "invite", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "invite", -1);

            break;

         default:
            return LOG_ASN1ERR (pctxt, ASN_E_INVOPT);
      }
   }
   else {
      stat = decodeSmallNonNegWholeNumber (pctxt, &ui);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      else pvalue->t = ui + 4;

      stat = decodeByteAlign (pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      copyContext (&lctxt, pctxt);
      initContextBuffer (pctxt, openType.data, openType.numocts);

      switch (pvalue->t) {
         /* capability_negotiation */
         case 4:
            rtInvokeStartElement (pctxt, "capability_negotiation", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "capability_negotiation", -1);

            break;

         /* callIndependentSupplementaryService */
         case 5:
            rtInvokeStartElement (pctxt, "callIndependentSupplementaryService", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "callIndependentSupplementaryService", -1);

            break;

         default:
            pvalue->u.extElem1 = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
            if (pvalue->u.extElem1 == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            pvalue->u.extElem1->data = openType.data;
            pvalue->u.extElem1->numocts = openType.numocts;

      }

      copyContext (pctxt, &lctxt);
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  Q954Details                                               */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225Q954Details (OOCTXT* pctxt, H225Q954Details* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   dListInit (&pvalue->extElem1);

   /* decode conferenceCalling */

   rtInvokeStartElement (pctxt, "conferenceCalling", -1);

   stat = DECODEBIT (pctxt, &pvalue->conferenceCalling);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   rtInvokeBoolValue (pctxt, pvalue->conferenceCalling);

   rtInvokeEndElement (pctxt, "conferenceCalling", -1);

   /* decode threePartyService */

   rtInvokeStartElement (pctxt, "threePartyService", -1);

   stat = DECODEBIT (pctxt, &pvalue->threePartyService);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   rtInvokeBoolValue (pctxt, pvalue->threePartyService);

   rtInvokeEndElement (pctxt, "threePartyService", -1);

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
            pOpenType->numocts = openType.numocts;
            pOpenType->data = openType.data;
            dListAppend (pctxt, &pvalue->extElem1, pOpenType);
         }
         else {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  QseriesOptions                                            */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225QseriesOptions (OOCTXT* pctxt, H225QseriesOptions* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   dListInit (&pvalue->extElem1);

   /* decode q932Full */

   rtInvokeStartElement (pctxt, "q932Full", -1);

   stat = DECODEBIT (pctxt, &pvalue->q932Full);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   rtInvokeBoolValue (pctxt, pvalue->q932Full);

   rtInvokeEndElement (pctxt, "q932Full", -1);

   /* decode q951Full */

   rtInvokeStartElement (pctxt, "q951Full", -1);

   stat = DECODEBIT (pctxt, &pvalue->q951Full);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   rtInvokeBoolValue (pctxt, pvalue->q951Full);

   rtInvokeEndElement (pctxt, "q951Full", -1);

   /* decode q952Full */

   rtInvokeStartElement (pctxt, "q952Full", -1);

   stat = DECODEBIT (pctxt, &pvalue->q952Full);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   rtInvokeBoolValue (pctxt, pvalue->q952Full);

   rtInvokeEndElement (pctxt, "q952Full", -1);

   /* decode q953Full */

   rtInvokeStartElement (pctxt, "q953Full", -1);

   stat = DECODEBIT (pctxt, &pvalue->q953Full);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   rtInvokeBoolValue (pctxt, pvalue->q953Full);

   rtInvokeEndElement (pctxt, "q953Full", -1);

   /* decode q955Full */

   rtInvokeStartElement (pctxt, "q955Full", -1);

   stat = DECODEBIT (pctxt, &pvalue->q955Full);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   rtInvokeBoolValue (pctxt, pvalue->q955Full);

   rtInvokeEndElement (pctxt, "q955Full", -1);

   /* decode q956Full */

   rtInvokeStartElement (pctxt, "q956Full", -1);

   stat = DECODEBIT (pctxt, &pvalue->q956Full);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   rtInvokeBoolValue (pctxt, pvalue->q956Full);

   rtInvokeEndElement (pctxt, "q956Full", -1);

   /* decode q957Full */

   rtInvokeStartElement (pctxt, "q957Full", -1);

   stat = DECODEBIT (pctxt, &pvalue->q957Full);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   rtInvokeBoolValue (pctxt, pvalue->q957Full);

   rtInvokeEndElement (pctxt, "q957Full", -1);

   /* decode q954Info */

   rtInvokeStartElement (pctxt, "q954Info", -1);

   stat = asn1PD_H225Q954Details (pctxt, &pvalue->q954Info);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "q954Info", -1);

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
            pOpenType->numocts = openType.numocts;
            pOpenType->data = openType.data;
            dListAppend (pctxt, &pvalue->extElem1, pOpenType);
         }
         else {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  CallType                                                  */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225CallType (OOCTXT* pctxt, H225CallType* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT ui;
   ASN1OpenType openType;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   if (!extbit) {
      stat = decodeConsUnsigned (pctxt, &ui, 0, 3);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      else pvalue->t = ui + 1;

      switch (ui) {
         /* pointToPoint */
         case 0:
            rtInvokeStartElement (pctxt, "pointToPoint", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "pointToPoint", -1);

            break;

         /* oneToN */
         case 1:
            rtInvokeStartElement (pctxt, "oneToN", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "oneToN", -1);

            break;

         /* nToOne */
         case 2:
            rtInvokeStartElement (pctxt, "nToOne", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "nToOne", -1);

            break;

         /* nToN */
         case 3:
            rtInvokeStartElement (pctxt, "nToN", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "nToN", -1);

            break;

         default:
            return LOG_ASN1ERR (pctxt, ASN_E_INVOPT);
      }
   }
   else {
      stat = decodeSmallNonNegWholeNumber (pctxt, &ui);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      else pvalue->t = ui + 5;

      stat = decodeByteAlign (pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      pvalue->u.extElem1 = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
      if (pvalue->u.extElem1 == NULL)
         return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

      pvalue->u.extElem1->data = openType.data;
      pvalue->u.extElem1->numocts = openType.numocts;

   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  CallIdentifier                                            */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225CallIdentifier (OOCTXT* pctxt, H225CallIdentifier* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   dListInit (&pvalue->extElem1);

   /* decode guid */

   rtInvokeStartElement (pctxt, "guid", -1);

   stat = asn1PD_H225GloballyUniqueID (pctxt, &pvalue->guid);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "guid", -1);

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
            pOpenType->numocts = openType.numocts;
            pOpenType->data = openType.data;
            dListAppend (pctxt, &pvalue->extElem1, pOpenType);
         }
         else {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  SecurityServiceMode                                       */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225SecurityServiceMode (OOCTXT* pctxt, H225SecurityServiceMode* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT ui;
   ASN1OpenType openType;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   if (!extbit) {
      stat = decodeConsUnsigned (pctxt, &ui, 0, 2);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      else pvalue->t = ui + 1;

      switch (ui) {
         /* nonStandard */
         case 0:
            rtInvokeStartElement (pctxt, "nonStandard", -1);

            pvalue->u.nonStandard = ALLOC_ASN1ELEM (pctxt, H225NonStandardParameter);
            if (pvalue->u.nonStandard == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225NonStandardParameter (pctxt, pvalue->u.nonStandard);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "nonStandard", -1);

            break;

         /* none */
         case 1:
            rtInvokeStartElement (pctxt, "none", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "none", -1);

            break;

         /* default_ */
         case 2:
            rtInvokeStartElement (pctxt, "default_", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "default_", -1);

            break;

         default:
            return LOG_ASN1ERR (pctxt, ASN_E_INVOPT);
      }
   }
   else {
      stat = decodeSmallNonNegWholeNumber (pctxt, &ui);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      else pvalue->t = ui + 4;

      stat = decodeByteAlign (pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      pvalue->u.extElem1 = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
      if (pvalue->u.extElem1 == NULL)
         return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

      pvalue->u.extElem1->data = openType.data;
      pvalue->u.extElem1->numocts = openType.numocts;

   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  SecurityCapabilities                                      */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225SecurityCapabilities (OOCTXT* pctxt, H225SecurityCapabilities* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.nonStandardPresent = optbit;

   /* decode nonStandard */

   if (pvalue->m.nonStandardPresent) {
      rtInvokeStartElement (pctxt, "nonStandard", -1);

      stat = asn1PD_H225NonStandardParameter (pctxt, &pvalue->nonStandard);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "nonStandard", -1);
   }

   /* decode encryption */

   rtInvokeStartElement (pctxt, "encryption", -1);

   stat = asn1PD_H225SecurityServiceMode (pctxt, &pvalue->encryption);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "encryption", -1);

   /* decode authenticaton */

   rtInvokeStartElement (pctxt, "authenticaton", -1);

   stat = asn1PD_H225SecurityServiceMode (pctxt, &pvalue->authenticaton);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "authenticaton", -1);

   /* decode integrity */

   rtInvokeStartElement (pctxt, "integrity", -1);

   stat = asn1PD_H225SecurityServiceMode (pctxt, &pvalue->integrity);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "integrity", -1);

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
            pOpenType->numocts = openType.numocts;
            pOpenType->data = openType.data;
            dListAppend (pctxt, &pvalue->extElem1, pOpenType);
         }
         else {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  H245Security                                              */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225H245Security (OOCTXT* pctxt, H225H245Security* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT ui;
   ASN1OpenType openType;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   if (!extbit) {
      stat = decodeConsUnsigned (pctxt, &ui, 0, 3);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      else pvalue->t = ui + 1;

      switch (ui) {
         /* nonStandard */
         case 0:
            rtInvokeStartElement (pctxt, "nonStandard", -1);

            pvalue->u.nonStandard = ALLOC_ASN1ELEM (pctxt, H225NonStandardParameter);
            if (pvalue->u.nonStandard == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225NonStandardParameter (pctxt, pvalue->u.nonStandard);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "nonStandard", -1);

            break;

         /* noSecurity */
         case 1:
            rtInvokeStartElement (pctxt, "noSecurity", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "noSecurity", -1);

            break;

         /* tls */
         case 2:
            rtInvokeStartElement (pctxt, "tls", -1);

            pvalue->u.tls = ALLOC_ASN1ELEM (pctxt, H225SecurityCapabilities);
            if (pvalue->u.tls == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225SecurityCapabilities (pctxt, pvalue->u.tls);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "tls", -1);

            break;

         /* ipsec */
         case 3:
            rtInvokeStartElement (pctxt, "ipsec", -1);

            pvalue->u.ipsec = ALLOC_ASN1ELEM (pctxt, H225SecurityCapabilities);
            if (pvalue->u.ipsec == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225SecurityCapabilities (pctxt, pvalue->u.ipsec);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "ipsec", -1);

            break;

         default:
            return LOG_ASN1ERR (pctxt, ASN_E_INVOPT);
      }
   }
   else {
      stat = decodeSmallNonNegWholeNumber (pctxt, &ui);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      else pvalue->t = ui + 5;

      stat = decodeByteAlign (pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      pvalue->u.extElem1 = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
      if (pvalue->u.extElem1 == NULL)
         return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

      pvalue->u.extElem1->data = openType.data;
      pvalue->u.extElem1->numocts = openType.numocts;

   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  _SeqOfH225H245Security                                    */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225_SeqOfH225H245Security (OOCTXT* pctxt, H225_SeqOfH225H245Security* pvalue)
{
   int stat = ASN_OK;
   H225H245Security* pdata;
   ASN1UINT count = 0;
   ASN1UINT xx1;
   int lstat;

   dListInit (pvalue);

   for (;;) {
      /* decode length determinant */

      lstat = decodeLength (pctxt, &count);
      if (lstat != ASN_OK && lstat != ASN_OK_FRAG) {
         return LOG_ASN1ERR (pctxt, lstat);
      }

      /* decode elements */

      for (xx1 = 0; xx1 < count; xx1++) {
         rtInvokeStartElement (pctxt, "elem", xx1);

         pdata = ALLOC_ASN1ELEMDNODE (pctxt, H225H245Security);
         if (pdata == NULL)
            return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

         stat = asn1PD_H225H245Security (pctxt, pdata);
         if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
         rtInvokeEndElement (pctxt, "elem", xx1);

         dListAppendNode (pctxt, pvalue, pdata);
      }

      if(lstat == ASN_OK) break;
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  _SeqOfH225ClearToken                                      */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225_SeqOfH225ClearToken (OOCTXT* pctxt, H225_SeqOfH225ClearToken* pvalue)
{
   int stat = ASN_OK;
   H235ClearToken* pdata;
   ASN1UINT count = 0;
   ASN1UINT xx1;
   int lstat;

   dListInit (pvalue);

   for (;;) {
      /* decode length determinant */

      lstat = decodeLength (pctxt, &count);
      if (lstat != ASN_OK && lstat != ASN_OK_FRAG) {
         return LOG_ASN1ERR (pctxt, lstat);
      }

      /* decode elements */

      for (xx1 = 0; xx1 < count; xx1++) {
         rtInvokeStartElement (pctxt, "elem", xx1);

         pdata = ALLOC_ASN1ELEMDNODE (pctxt, H235ClearToken);
         if (pdata == NULL)
            return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

         stat = asn1PD_H235ClearToken (pctxt, pdata);
         if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
         rtInvokeEndElement (pctxt, "elem", xx1);

         dListAppendNode (pctxt, pvalue, pdata);
      }

      if(lstat == ASN_OK) break;
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  CryptoH323Token_cryptoEPPwdHash                           */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225CryptoH323Token_cryptoEPPwdHash (OOCTXT* pctxt, H225CryptoH323Token_cryptoEPPwdHash* pvalue)
{
   int stat = ASN_OK;

   /* decode alias */

   rtInvokeStartElement (pctxt, "alias", -1);

   stat = asn1PD_H225AliasAddress (pctxt, &pvalue->alias);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "alias", -1);

   /* decode timeStamp */

   rtInvokeStartElement (pctxt, "timeStamp", -1);

   stat = asn1PD_H235TimeStamp (pctxt, &pvalue->timeStamp);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "timeStamp", -1);

   /* decode token */

   rtInvokeStartElement (pctxt, "token", -1);

   stat = asn1PD_H235HASHED (pctxt, &pvalue->token);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "token", -1);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  CryptoH323Token_cryptoGKPwdHash                           */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225CryptoH323Token_cryptoGKPwdHash (OOCTXT* pctxt, H225CryptoH323Token_cryptoGKPwdHash* pvalue)
{
   int stat = ASN_OK;

   /* decode gatekeeperId */

   rtInvokeStartElement (pctxt, "gatekeeperId", -1);

   stat = asn1PD_H225GatekeeperIdentifier (pctxt, &pvalue->gatekeeperId);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "gatekeeperId", -1);

   /* decode timeStamp */

   rtInvokeStartElement (pctxt, "timeStamp", -1);

   stat = asn1PD_H235TimeStamp (pctxt, &pvalue->timeStamp);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "timeStamp", -1);

   /* decode token */

   rtInvokeStartElement (pctxt, "token", -1);

   stat = asn1PD_H235HASHED (pctxt, &pvalue->token);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "token", -1);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  CryptoH323Token_cryptoEPCert                              */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225CryptoH323Token_cryptoEPCert (OOCTXT* pctxt, H225CryptoH323Token_cryptoEPCert* pvalue)
{
   int stat = ASN_OK;

   /* decode toBeSigned */

   rtInvokeStartElement (pctxt, "toBeSigned", -1);

   stat = asn1PD_H235EncodedPwdCertToken (pctxt, &pvalue->toBeSigned);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "toBeSigned", -1);

   /* decode algorithmOID */

   rtInvokeStartElement (pctxt, "algorithmOID", -1);

   stat = decodeObjectIdentifier (pctxt, &pvalue->algorithmOID);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   rtInvokeOidValue (pctxt, pvalue->algorithmOID.numids, pvalue->algorithmOID.subid);

   rtInvokeEndElement (pctxt, "algorithmOID", -1);

   /* decode paramS */

   rtInvokeStartElement (pctxt, "paramS", -1);

   stat = asn1PD_H235Params (pctxt, &pvalue->paramS);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "paramS", -1);

   /* decode signature */

   rtInvokeStartElement (pctxt, "signature", -1);

   stat = decodeDynBitString (pctxt, (ASN1DynBitStr*)&pvalue->signature);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeBitStrValue (pctxt, pvalue->signature.numbits, pvalue->signature.data);

   rtInvokeEndElement (pctxt, "signature", -1);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  CryptoH323Token_cryptoGKCert                              */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225CryptoH323Token_cryptoGKCert (OOCTXT* pctxt, H225CryptoH323Token_cryptoGKCert* pvalue)
{
   int stat = ASN_OK;

   /* decode toBeSigned */

   rtInvokeStartElement (pctxt, "toBeSigned", -1);

   stat = asn1PD_H235EncodedPwdCertToken (pctxt, &pvalue->toBeSigned);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "toBeSigned", -1);

   /* decode algorithmOID */

   rtInvokeStartElement (pctxt, "algorithmOID", -1);

   stat = decodeObjectIdentifier (pctxt, &pvalue->algorithmOID);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   rtInvokeOidValue (pctxt, pvalue->algorithmOID.numids, pvalue->algorithmOID.subid);

   rtInvokeEndElement (pctxt, "algorithmOID", -1);

   /* decode paramS */

   rtInvokeStartElement (pctxt, "paramS", -1);

   stat = asn1PD_H235Params (pctxt, &pvalue->paramS);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "paramS", -1);

   /* decode signature */

   rtInvokeStartElement (pctxt, "signature", -1);

   stat = decodeDynBitString (pctxt, (ASN1DynBitStr*)&pvalue->signature);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeBitStrValue (pctxt, pvalue->signature.numbits, pvalue->signature.data);

   rtInvokeEndElement (pctxt, "signature", -1);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  CryptoH323Token_cryptoFastStart                           */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225CryptoH323Token_cryptoFastStart (OOCTXT* pctxt, H225CryptoH323Token_cryptoFastStart* pvalue)
{
   int stat = ASN_OK;

   /* decode toBeSigned */

   rtInvokeStartElement (pctxt, "toBeSigned", -1);

   stat = asn1PD_H225EncodedFastStartToken (pctxt, &pvalue->toBeSigned);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "toBeSigned", -1);

   /* decode algorithmOID */

   rtInvokeStartElement (pctxt, "algorithmOID", -1);

   stat = decodeObjectIdentifier (pctxt, &pvalue->algorithmOID);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   rtInvokeOidValue (pctxt, pvalue->algorithmOID.numids, pvalue->algorithmOID.subid);

   rtInvokeEndElement (pctxt, "algorithmOID", -1);

   /* decode paramS */

   rtInvokeStartElement (pctxt, "paramS", -1);

   stat = asn1PD_H235Params (pctxt, &pvalue->paramS);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "paramS", -1);

   /* decode signature */

   rtInvokeStartElement (pctxt, "signature", -1);

   stat = decodeDynBitString (pctxt, (ASN1DynBitStr*)&pvalue->signature);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeBitStrValue (pctxt, pvalue->signature.numbits, pvalue->signature.data);

   rtInvokeEndElement (pctxt, "signature", -1);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  CryptoH323Token                                           */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225CryptoH323Token (OOCTXT* pctxt, H225CryptoH323Token* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT ui;
   ASN1OpenType openType;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   if (!extbit) {
      stat = decodeConsUnsigned (pctxt, &ui, 0, 7);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      else pvalue->t = ui + 1;

      switch (ui) {
         /* cryptoEPPwdHash */
         case 0:
            rtInvokeStartElement (pctxt, "cryptoEPPwdHash", -1);

            pvalue->u.cryptoEPPwdHash = ALLOC_ASN1ELEM (pctxt, H225CryptoH323Token_cryptoEPPwdHash);
            if (pvalue->u.cryptoEPPwdHash == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225CryptoH323Token_cryptoEPPwdHash (pctxt, pvalue->u.cryptoEPPwdHash);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "cryptoEPPwdHash", -1);

            break;

         /* cryptoGKPwdHash */
         case 1:
            rtInvokeStartElement (pctxt, "cryptoGKPwdHash", -1);

            pvalue->u.cryptoGKPwdHash = ALLOC_ASN1ELEM (pctxt, H225CryptoH323Token_cryptoGKPwdHash);
            if (pvalue->u.cryptoGKPwdHash == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225CryptoH323Token_cryptoGKPwdHash (pctxt, pvalue->u.cryptoGKPwdHash);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "cryptoGKPwdHash", -1);

            break;

         /* cryptoEPPwdEncr */
         case 2:
            rtInvokeStartElement (pctxt, "cryptoEPPwdEncr", -1);

            pvalue->u.cryptoEPPwdEncr = ALLOC_ASN1ELEM (pctxt, H235ENCRYPTED);
            if (pvalue->u.cryptoEPPwdEncr == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H235ENCRYPTED (pctxt, pvalue->u.cryptoEPPwdEncr);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "cryptoEPPwdEncr", -1);

            break;

         /* cryptoGKPwdEncr */
         case 3:
            rtInvokeStartElement (pctxt, "cryptoGKPwdEncr", -1);

            pvalue->u.cryptoGKPwdEncr = ALLOC_ASN1ELEM (pctxt, H235ENCRYPTED);
            if (pvalue->u.cryptoGKPwdEncr == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H235ENCRYPTED (pctxt, pvalue->u.cryptoGKPwdEncr);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "cryptoGKPwdEncr", -1);

            break;

         /* cryptoEPCert */
         case 4:
            rtInvokeStartElement (pctxt, "cryptoEPCert", -1);

            pvalue->u.cryptoEPCert = ALLOC_ASN1ELEM (pctxt, H225CryptoH323Token_cryptoEPCert);
            if (pvalue->u.cryptoEPCert == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225CryptoH323Token_cryptoEPCert (pctxt, pvalue->u.cryptoEPCert);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "cryptoEPCert", -1);

            break;

         /* cryptoGKCert */
         case 5:
            rtInvokeStartElement (pctxt, "cryptoGKCert", -1);

            pvalue->u.cryptoGKCert = ALLOC_ASN1ELEM (pctxt, H225CryptoH323Token_cryptoGKCert);
            if (pvalue->u.cryptoGKCert == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225CryptoH323Token_cryptoGKCert (pctxt, pvalue->u.cryptoGKCert);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "cryptoGKCert", -1);

            break;

         /* cryptoFastStart */
         case 6:
            rtInvokeStartElement (pctxt, "cryptoFastStart", -1);

            pvalue->u.cryptoFastStart = ALLOC_ASN1ELEM (pctxt, H225CryptoH323Token_cryptoFastStart);
            if (pvalue->u.cryptoFastStart == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225CryptoH323Token_cryptoFastStart (pctxt, pvalue->u.cryptoFastStart);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "cryptoFastStart", -1);

            break;

         /* nestedcryptoToken */
         case 7:
            rtInvokeStartElement (pctxt, "nestedcryptoToken", -1);

            pvalue->u.nestedcryptoToken = ALLOC_ASN1ELEM (pctxt, H235CryptoToken);
            if (pvalue->u.nestedcryptoToken == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H235CryptoToken (pctxt, pvalue->u.nestedcryptoToken);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "nestedcryptoToken", -1);

            break;

         default:
            return LOG_ASN1ERR (pctxt, ASN_E_INVOPT);
      }
   }
   else {
      stat = decodeSmallNonNegWholeNumber (pctxt, &ui);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      else pvalue->t = ui + 9;

      stat = decodeByteAlign (pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      pvalue->u.extElem1 = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
      if (pvalue->u.extElem1 == NULL)
         return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

      pvalue->u.extElem1->data = openType.data;
      pvalue->u.extElem1->numocts = openType.numocts;

   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  _SeqOfH225CryptoH323Token                                 */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225_SeqOfH225CryptoH323Token (OOCTXT* pctxt, H225_SeqOfH225CryptoH323Token* pvalue)
{
   int stat = ASN_OK;
   H225CryptoH323Token* pdata;
   ASN1UINT count = 0;
   ASN1UINT xx1;
   int lstat;

   dListInit (pvalue);

   for (;;) {
      /* decode length determinant */

      lstat = decodeLength (pctxt, &count);
      if (lstat != ASN_OK && lstat != ASN_OK_FRAG) {
         return LOG_ASN1ERR (pctxt, lstat);
      }

      /* decode elements */

      for (xx1 = 0; xx1 < count; xx1++) {
         rtInvokeStartElement (pctxt, "elem", xx1);

         pdata = ALLOC_ASN1ELEMDNODE (pctxt, H225CryptoH323Token);
         if (pdata == NULL)
            return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

         stat = asn1PD_H225CryptoH323Token (pctxt, pdata);
         if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
         rtInvokeEndElement (pctxt, "elem", xx1);

         dListAppendNode (pctxt, pvalue, pdata);
      }

      if(lstat == ASN_OK) break;
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  Setup_UUIE_fastStart                                      */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225Setup_UUIE_fastStart (OOCTXT* pctxt, H225Setup_UUIE_fastStart* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT xx1;

   /* decode length determinant */

   stat = decodeLength (pctxt, &pvalue->n);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* decode elements */

   ALLOC_ASN1ARRAY (pctxt, pvalue, ASN1DynOctStr);

   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      rtInvokeStartElement (pctxt, "elem", xx1);

      stat = decodeDynOctetString (pctxt, (ASN1DynOctStr*)&pvalue->elem[xx1]);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      rtInvokeOctStrValue (pctxt, pvalue->elem[xx1].numocts, pvalue->elem[xx1].data);
      rtInvokeEndElement (pctxt, "elem", xx1);

   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  ScnConnectionType                                         */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225ScnConnectionType (OOCTXT* pctxt, H225ScnConnectionType* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT ui;
   ASN1OpenType openType;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   if (!extbit) {
      stat = decodeConsUnsigned (pctxt, &ui, 0, 6);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      else pvalue->t = ui + 1;

      switch (ui) {
         /* unknown */
         case 0:
            rtInvokeStartElement (pctxt, "unknown", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "unknown", -1);

            break;

         /* bChannel */
         case 1:
            rtInvokeStartElement (pctxt, "bChannel", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "bChannel", -1);

            break;

         /* hybrid2x64 */
         case 2:
            rtInvokeStartElement (pctxt, "hybrid2x64", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "hybrid2x64", -1);

            break;

         /* hybrid384 */
         case 3:
            rtInvokeStartElement (pctxt, "hybrid384", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "hybrid384", -1);

            break;

         /* hybrid1536 */
         case 4:
            rtInvokeStartElement (pctxt, "hybrid1536", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "hybrid1536", -1);

            break;

         /* hybrid1920 */
         case 5:
            rtInvokeStartElement (pctxt, "hybrid1920", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "hybrid1920", -1);

            break;

         /* multirate */
         case 6:
            rtInvokeStartElement (pctxt, "multirate", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "multirate", -1);

            break;

         default:
            return LOG_ASN1ERR (pctxt, ASN_E_INVOPT);
      }
   }
   else {
      stat = decodeSmallNonNegWholeNumber (pctxt, &ui);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      else pvalue->t = ui + 8;

      stat = decodeByteAlign (pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      pvalue->u.extElem1 = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
      if (pvalue->u.extElem1 == NULL)
         return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

      pvalue->u.extElem1->data = openType.data;
      pvalue->u.extElem1->numocts = openType.numocts;

   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  ScnConnectionAggregation                                  */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225ScnConnectionAggregation (OOCTXT* pctxt, H225ScnConnectionAggregation* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT ui;
   ASN1OpenType openType;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   if (!extbit) {
      stat = decodeConsUnsigned (pctxt, &ui, 0, 5);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      else pvalue->t = ui + 1;

      switch (ui) {
         /* auto_ */
         case 0:
            rtInvokeStartElement (pctxt, "auto_", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "auto_", -1);

            break;

         /* none */
         case 1:
            rtInvokeStartElement (pctxt, "none", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "none", -1);

            break;

         /* h221 */
         case 2:
            rtInvokeStartElement (pctxt, "h221", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "h221", -1);

            break;

         /* bonded_mode1 */
         case 3:
            rtInvokeStartElement (pctxt, "bonded_mode1", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "bonded_mode1", -1);

            break;

         /* bonded_mode2 */
         case 4:
            rtInvokeStartElement (pctxt, "bonded_mode2", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "bonded_mode2", -1);

            break;

         /* bonded_mode3 */
         case 5:
            rtInvokeStartElement (pctxt, "bonded_mode3", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "bonded_mode3", -1);

            break;

         default:
            return LOG_ASN1ERR (pctxt, ASN_E_INVOPT);
      }
   }
   else {
      stat = decodeSmallNonNegWholeNumber (pctxt, &ui);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      else pvalue->t = ui + 7;

      stat = decodeByteAlign (pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      pvalue->u.extElem1 = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
      if (pvalue->u.extElem1 == NULL)
         return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

      pvalue->u.extElem1->data = openType.data;
      pvalue->u.extElem1->numocts = openType.numocts;

   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  Setup_UUIE_connectionParameters                           */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225Setup_UUIE_connectionParameters (OOCTXT* pctxt, H225Setup_UUIE_connectionParameters* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   dListInit (&pvalue->extElem1);

   /* decode connectionType */

   rtInvokeStartElement (pctxt, "connectionType", -1);

   stat = asn1PD_H225ScnConnectionType (pctxt, &pvalue->connectionType);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "connectionType", -1);

   /* decode numberOfScnConnections */

   rtInvokeStartElement (pctxt, "numberOfScnConnections", -1);

   stat = decodeConsUInt16 (pctxt, &pvalue->numberOfScnConnections, 0U, 65535U);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   rtInvokeUIntValue (pctxt, pvalue->numberOfScnConnections);

   rtInvokeEndElement (pctxt, "numberOfScnConnections", -1);

   /* decode connectionAggregation */

   rtInvokeStartElement (pctxt, "connectionAggregation", -1);

   stat = asn1PD_H225ScnConnectionAggregation (pctxt, &pvalue->connectionAggregation);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "connectionAggregation", -1);

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
            pOpenType->numocts = openType.numocts;
            pOpenType->data = openType.data;
            dListAppend (pctxt, &pvalue->extElem1, pOpenType);
         }
         else {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  Setup_UUIE_language                                       */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225Setup_UUIE_language (OOCTXT* pctxt, H225Setup_UUIE_language* pvalue)
{
   static Asn1SizeCnst element_lsize1 = { 0, 1, 32, 0 };
   int stat = ASN_OK;
   ASN1UINT xx1;

   /* decode length determinant */

   stat = decodeLength (pctxt, &pvalue->n);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* decode elements */

   ALLOC_ASN1ARRAY (pctxt, pvalue, ASN1IA5String);

   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      rtInvokeStartElement (pctxt, "elem", xx1);

      stat = addSizeConstraint (pctxt, &element_lsize1);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = decodeConstrainedStringEx (pctxt, &pvalue->elem[xx1], 0, 8, 7, 7);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      rtInvokeCharStrValue (pctxt, pvalue->elem[xx1]);
      rtInvokeEndElement (pctxt, "elem", xx1);

   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  PresentationIndicator                                     */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225PresentationIndicator (OOCTXT* pctxt, H225PresentationIndicator* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT ui;
   ASN1OpenType openType;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   if (!extbit) {
      stat = decodeConsUnsigned (pctxt, &ui, 0, 2);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      else pvalue->t = ui + 1;

      switch (ui) {
         /* presentationAllowed */
         case 0:
            rtInvokeStartElement (pctxt, "presentationAllowed", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "presentationAllowed", -1);

            break;

         /* presentationRestricted */
         case 1:
            rtInvokeStartElement (pctxt, "presentationRestricted", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "presentationRestricted", -1);

            break;

         /* addressNotAvailable */
         case 2:
            rtInvokeStartElement (pctxt, "addressNotAvailable", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "addressNotAvailable", -1);

            break;

         default:
            return LOG_ASN1ERR (pctxt, ASN_E_INVOPT);
      }
   }
   else {
      stat = decodeSmallNonNegWholeNumber (pctxt, &ui);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      else pvalue->t = ui + 4;

      stat = decodeByteAlign (pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      pvalue->u.extElem1 = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
      if (pvalue->u.extElem1 == NULL)
         return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

      pvalue->u.extElem1->data = openType.data;
      pvalue->u.extElem1->numocts = openType.numocts;

   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  CallCreditServiceControl_billingMode                      */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225CallCreditServiceControl_billingMode (OOCTXT* pctxt, H225CallCreditServiceControl_billingMode* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT ui;
   ASN1OpenType openType;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   if (!extbit) {
      stat = decodeConsUnsigned (pctxt, &ui, 0, 1);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      else pvalue->t = ui + 1;

      switch (ui) {
         /* credit */
         case 0:
            rtInvokeStartElement (pctxt, "credit", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "credit", -1);

            break;

         /* debit */
         case 1:
            rtInvokeStartElement (pctxt, "debit", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "debit", -1);

            break;

         default:
            return LOG_ASN1ERR (pctxt, ASN_E_INVOPT);
      }
   }
   else {
      stat = decodeSmallNonNegWholeNumber (pctxt, &ui);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      else pvalue->t = ui + 3;

      stat = decodeByteAlign (pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      pvalue->u.extElem1 = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
      if (pvalue->u.extElem1 == NULL)
         return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

      pvalue->u.extElem1->data = openType.data;
      pvalue->u.extElem1->numocts = openType.numocts;

   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  CallCreditServiceControl_callStartingPoint                */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225CallCreditServiceControl_callStartingPoint (OOCTXT* pctxt, H225CallCreditServiceControl_callStartingPoint* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT ui;
   ASN1OpenType openType;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   if (!extbit) {
      stat = decodeConsUnsigned (pctxt, &ui, 0, 1);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      else pvalue->t = ui + 1;

      switch (ui) {
         /* alerting */
         case 0:
            rtInvokeStartElement (pctxt, "alerting", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "alerting", -1);

            break;

         /* connect */
         case 1:
            rtInvokeStartElement (pctxt, "connect", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "connect", -1);

            break;

         default:
            return LOG_ASN1ERR (pctxt, ASN_E_INVOPT);
      }
   }
   else {
      stat = decodeSmallNonNegWholeNumber (pctxt, &ui);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      else pvalue->t = ui + 3;

      stat = decodeByteAlign (pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      pvalue->u.extElem1 = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
      if (pvalue->u.extElem1 == NULL)
         return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

      pvalue->u.extElem1->data = openType.data;
      pvalue->u.extElem1->numocts = openType.numocts;

   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  CallCreditServiceControl                                  */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225CallCreditServiceControl (OOCTXT* pctxt, H225CallCreditServiceControl* pvalue)
{
   static Asn1SizeCnst amountString_lsize1 = { 0, 1, 512, 0 };
   int stat = ASN_OK;
   OOCTXT lctxt;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.amountStringPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.billingModePresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.callDurationLimitPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.enforceCallDurationLimitPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.callStartingPointPresent = optbit;

   /* decode amountString */

   if (pvalue->m.amountStringPresent) {
      rtInvokeStartElement (pctxt, "amountString", -1);

      stat = addSizeConstraint (pctxt, &amountString_lsize1);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = decodeBMPString (pctxt, &pvalue->amountString, 0);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      rtInvokeCharStr16BitValue (pctxt, pvalue->amountString.nchars, pvalue->amountString.data);

      rtInvokeEndElement (pctxt, "amountString", -1);
   }

   /* decode billingMode */

   if (pvalue->m.billingModePresent) {
      rtInvokeStartElement (pctxt, "billingMode", -1);

      stat = asn1PD_H225CallCreditServiceControl_billingMode (pctxt, &pvalue->billingMode);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "billingMode", -1);
   }

   /* decode callDurationLimit */

   if (pvalue->m.callDurationLimitPresent) {
      rtInvokeStartElement (pctxt, "callDurationLimit", -1);

      stat = decodeConsUnsigned (pctxt, &pvalue->callDurationLimit, 1U, ASN1UINT_MAX);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      rtInvokeUIntValue (pctxt, pvalue->callDurationLimit);

      rtInvokeEndElement (pctxt, "callDurationLimit", -1);
   }

   /* decode enforceCallDurationLimit */

   if (pvalue->m.enforceCallDurationLimitPresent) {
      rtInvokeStartElement (pctxt, "enforceCallDurationLimit", -1);

      stat = DECODEBIT (pctxt, &pvalue->enforceCallDurationLimit);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      rtInvokeBoolValue (pctxt, pvalue->enforceCallDurationLimit);

      rtInvokeEndElement (pctxt, "enforceCallDurationLimit", -1);
   }

   /* decode callStartingPoint */

   if (pvalue->m.callStartingPointPresent) {
      rtInvokeStartElement (pctxt, "callStartingPoint", -1);

      stat = asn1PD_H225CallCreditServiceControl_callStartingPoint (pctxt, &pvalue->callStartingPoint);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "callStartingPoint", -1);
   }

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
            pOpenType->numocts = openType.numocts;
            pOpenType->data = openType.data;
            dListAppend (pctxt, &pvalue->extElem1, pOpenType);
         }
         else {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  ServiceControlDescriptor                                  */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225ServiceControlDescriptor (OOCTXT* pctxt, H225ServiceControlDescriptor* pvalue)
{
   static Asn1SizeCnst url_lsize1 = { 0, 0, 512, 0 };
   int stat = ASN_OK;
   ASN1UINT ui;
   ASN1OpenType openType;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   if (!extbit) {
      stat = decodeConsUnsigned (pctxt, &ui, 0, 3);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      else pvalue->t = ui + 1;

      switch (ui) {
         /* url */
         case 0:
            rtInvokeStartElement (pctxt, "url", -1);

            stat = addSizeConstraint (pctxt, &url_lsize1);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            stat = decodeConstrainedStringEx (pctxt, &pvalue->u.url, 0, 8, 7, 7);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
            rtInvokeCharStrValue (pctxt, pvalue->u.url);

            rtInvokeEndElement (pctxt, "url", -1);

            break;

         /* signal */
         case 1:
            rtInvokeStartElement (pctxt, "signal", -1);

            pvalue->u.signal = ALLOC_ASN1ELEM (pctxt, H225H248SignalsDescriptor);
            if (pvalue->u.signal == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225H248SignalsDescriptor (pctxt, pvalue->u.signal);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "signal", -1);

            break;

         /* nonStandard */
         case 2:
            rtInvokeStartElement (pctxt, "nonStandard", -1);

            pvalue->u.nonStandard = ALLOC_ASN1ELEM (pctxt, H225NonStandardParameter);
            if (pvalue->u.nonStandard == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225NonStandardParameter (pctxt, pvalue->u.nonStandard);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "nonStandard", -1);

            break;

         /* callCreditServiceControl */
         case 3:
            rtInvokeStartElement (pctxt, "callCreditServiceControl", -1);

            pvalue->u.callCreditServiceControl = ALLOC_ASN1ELEM (pctxt, H225CallCreditServiceControl);
            if (pvalue->u.callCreditServiceControl == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225CallCreditServiceControl (pctxt, pvalue->u.callCreditServiceControl);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "callCreditServiceControl", -1);

            break;

         default:
            return LOG_ASN1ERR (pctxt, ASN_E_INVOPT);
      }
   }
   else {
      stat = decodeSmallNonNegWholeNumber (pctxt, &ui);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      else pvalue->t = ui + 5;

      stat = decodeByteAlign (pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      pvalue->u.extElem1 = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
      if (pvalue->u.extElem1 == NULL)
         return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

      pvalue->u.extElem1->data = openType.data;
      pvalue->u.extElem1->numocts = openType.numocts;

   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  ServiceControlSession_reason                              */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225ServiceControlSession_reason (OOCTXT* pctxt, H225ServiceControlSession_reason* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT ui;
   ASN1OpenType openType;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   if (!extbit) {
      stat = decodeConsUnsigned (pctxt, &ui, 0, 2);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      else pvalue->t = ui + 1;

      switch (ui) {
         /* open */
         case 0:
            rtInvokeStartElement (pctxt, "open", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "open", -1);

            break;

         /* refresh */
         case 1:
            rtInvokeStartElement (pctxt, "refresh", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "refresh", -1);

            break;

         /* close */
         case 2:
            rtInvokeStartElement (pctxt, "close", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "close", -1);

            break;

         default:
            return LOG_ASN1ERR (pctxt, ASN_E_INVOPT);
      }
   }
   else {
      stat = decodeSmallNonNegWholeNumber (pctxt, &ui);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      else pvalue->t = ui + 4;

      stat = decodeByteAlign (pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      pvalue->u.extElem1 = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
      if (pvalue->u.extElem1 == NULL)
         return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

      pvalue->u.extElem1->data = openType.data;
      pvalue->u.extElem1->numocts = openType.numocts;

   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  ServiceControlSession                                     */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225ServiceControlSession (OOCTXT* pctxt, H225ServiceControlSession* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.contentsPresent = optbit;

   /* decode sessionId */

   rtInvokeStartElement (pctxt, "sessionId", -1);

   stat = decodeConsUInt8 (pctxt, &pvalue->sessionId, 0U, 255U);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   rtInvokeUIntValue (pctxt, pvalue->sessionId);

   rtInvokeEndElement (pctxt, "sessionId", -1);

   /* decode contents */

   if (pvalue->m.contentsPresent) {
      rtInvokeStartElement (pctxt, "contents", -1);

      stat = asn1PD_H225ServiceControlDescriptor (pctxt, &pvalue->contents);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "contents", -1);
   }

   /* decode reason */

   rtInvokeStartElement (pctxt, "reason", -1);

   stat = asn1PD_H225ServiceControlSession_reason (pctxt, &pvalue->reason);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "reason", -1);

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
            pOpenType->numocts = openType.numocts;
            pOpenType->data = openType.data;
            dListAppend (pctxt, &pvalue->extElem1, pOpenType);
         }
         else {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  _SeqOfH225ServiceControlSession                           */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225_SeqOfH225ServiceControlSession (OOCTXT* pctxt, H225_SeqOfH225ServiceControlSession* pvalue)
{
   int stat = ASN_OK;
   H225ServiceControlSession* pdata;
   ASN1UINT count = 0;
   ASN1UINT xx1;
   int lstat;

   dListInit (pvalue);

   for (;;) {
      /* decode length determinant */

      lstat = decodeLength (pctxt, &count);
      if (lstat != ASN_OK && lstat != ASN_OK_FRAG) {
         return LOG_ASN1ERR (pctxt, lstat);
      }

      /* decode elements */

      for (xx1 = 0; xx1 < count; xx1++) {
         rtInvokeStartElement (pctxt, "elem", xx1);

         pdata = ALLOC_ASN1ELEMDNODE (pctxt, H225ServiceControlSession);
         if (pdata == NULL)
            return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

         stat = asn1PD_H225ServiceControlSession (pctxt, pdata);
         if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
         rtInvokeEndElement (pctxt, "elem", xx1);

         dListAppendNode (pctxt, pvalue, pdata);
      }

      if(lstat == ASN_OK) break;
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  CarrierInfo                                               */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225CarrierInfo (OOCTXT* pctxt, H225CarrierInfo* pvalue)
{
   static Asn1SizeCnst carrierName_lsize1 = { 0, 1, 128, 0 };
   int stat = ASN_OK;
   OOCTXT lctxt;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.carrierIdentificationCodePresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.carrierNamePresent = optbit;

   /* decode carrierIdentificationCode */

   if (pvalue->m.carrierIdentificationCodePresent) {
      rtInvokeStartElement (pctxt, "carrierIdentificationCode", -1);

      stat = asn1PD_H225CarrierInfo_carrierIdentificationCode (pctxt, &pvalue->carrierIdentificationCode);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "carrierIdentificationCode", -1);
   }

   /* decode carrierName */

   if (pvalue->m.carrierNamePresent) {
      rtInvokeStartElement (pctxt, "carrierName", -1);

      stat = addSizeConstraint (pctxt, &carrierName_lsize1);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = decodeConstrainedStringEx (pctxt, &pvalue->carrierName, 0, 8, 7, 7);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      rtInvokeCharStrValue (pctxt, pvalue->carrierName);

      rtInvokeEndElement (pctxt, "carrierName", -1);
   }

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
            pOpenType->numocts = openType.numocts;
            pOpenType->data = openType.data;
            dListAppend (pctxt, &pvalue->extElem1, pOpenType);
         }
         else {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  CallsAvailable                                            */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225CallsAvailable (OOCTXT* pctxt, H225CallsAvailable* pvalue)
{
   static Asn1SizeCnst group_lsize1 = { 0, 1, 128, 0 };
   int stat = ASN_OK;
   OOCTXT lctxt;
   OOCTXT lctxt2;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.groupPresent = optbit;

   /* decode calls */

   rtInvokeStartElement (pctxt, "calls", -1);

   stat = decodeConsUnsigned (pctxt, &pvalue->calls, 0U, ASN1UINT_MAX);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   rtInvokeUIntValue (pctxt, pvalue->calls);

   rtInvokeEndElement (pctxt, "calls", -1);

   /* decode group */

   if (pvalue->m.groupPresent) {
      rtInvokeStartElement (pctxt, "group", -1);

      stat = addSizeConstraint (pctxt, &group_lsize1);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = decodeConstrainedStringEx (pctxt, &pvalue->group, 0, 8, 7, 7);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      rtInvokeCharStrValue (pctxt, pvalue->group);

      rtInvokeEndElement (pctxt, "group", -1);
   }

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            if (i < 1 && openType.numocts > 0) {  /* known element */
               copyContext (&lctxt2, pctxt);
               initContextBuffer (pctxt, openType.data, openType.numocts);

               switch (i) {
                  case 0:
                     pvalue->m.carrierPresent = 1;

                     rtInvokeStartElement (pctxt, "carrier", -1);

                     stat = asn1PD_H225CarrierInfo (pctxt, &pvalue->carrier);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "carrier", -1);
                     break;

                  default:
                     pctxt->buffer.byteIndex += openType.numocts;
               }
               copyContext (pctxt, &lctxt2);
            }
            else {  /* unknown element */
               pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
               pOpenType->numocts = openType.numocts;
               pOpenType->data = openType.data;
               dListAppend (pctxt, &pvalue->extElem1, pOpenType);
            }
         }
         else if (i >= 1) {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  _SeqOfH225CallsAvailable                                  */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225_SeqOfH225CallsAvailable (OOCTXT* pctxt, H225_SeqOfH225CallsAvailable* pvalue)
{
   int stat = ASN_OK;
   H225CallsAvailable* pdata;
   ASN1UINT count = 0;
   ASN1UINT xx1;
   int lstat;

   dListInit (pvalue);

   for (;;) {
      /* decode length determinant */

      lstat = decodeLength (pctxt, &count);
      if (lstat != ASN_OK && lstat != ASN_OK_FRAG) {
         return LOG_ASN1ERR (pctxt, lstat);
      }

      /* decode elements */

      for (xx1 = 0; xx1 < count; xx1++) {
         rtInvokeStartElement (pctxt, "elem", xx1);

         pdata = ALLOC_ASN1ELEMDNODE (pctxt, H225CallsAvailable);
         if (pdata == NULL)
            return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

         stat = asn1PD_H225CallsAvailable (pctxt, pdata);
         if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
         rtInvokeEndElement (pctxt, "elem", xx1);

         dListAppendNode (pctxt, pvalue, pdata);
      }

      if(lstat == ASN_OK) break;
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  CallCapacityInfo                                          */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225CallCapacityInfo (OOCTXT* pctxt, H225CallCapacityInfo* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   OOCTXT lctxt2;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.voiceGwCallsAvailablePresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.h310GwCallsAvailablePresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.h320GwCallsAvailablePresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.h321GwCallsAvailablePresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.h322GwCallsAvailablePresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.h323GwCallsAvailablePresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.h324GwCallsAvailablePresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.t120OnlyGwCallsAvailablePresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.t38FaxAnnexbOnlyGwCallsAvailablePresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.terminalCallsAvailablePresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.mcuCallsAvailablePresent = optbit;

   /* decode voiceGwCallsAvailable */

   if (pvalue->m.voiceGwCallsAvailablePresent) {
      rtInvokeStartElement (pctxt, "voiceGwCallsAvailable", -1);

      stat = asn1PD_H225_SeqOfH225CallsAvailable (pctxt, &pvalue->voiceGwCallsAvailable);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "voiceGwCallsAvailable", -1);
   }

   /* decode h310GwCallsAvailable */

   if (pvalue->m.h310GwCallsAvailablePresent) {
      rtInvokeStartElement (pctxt, "h310GwCallsAvailable", -1);

      stat = asn1PD_H225_SeqOfH225CallsAvailable (pctxt, &pvalue->h310GwCallsAvailable);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "h310GwCallsAvailable", -1);
   }

   /* decode h320GwCallsAvailable */

   if (pvalue->m.h320GwCallsAvailablePresent) {
      rtInvokeStartElement (pctxt, "h320GwCallsAvailable", -1);

      stat = asn1PD_H225_SeqOfH225CallsAvailable (pctxt, &pvalue->h320GwCallsAvailable);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "h320GwCallsAvailable", -1);
   }

   /* decode h321GwCallsAvailable */

   if (pvalue->m.h321GwCallsAvailablePresent) {
      rtInvokeStartElement (pctxt, "h321GwCallsAvailable", -1);

      stat = asn1PD_H225_SeqOfH225CallsAvailable (pctxt, &pvalue->h321GwCallsAvailable);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "h321GwCallsAvailable", -1);
   }

   /* decode h322GwCallsAvailable */

   if (pvalue->m.h322GwCallsAvailablePresent) {
      rtInvokeStartElement (pctxt, "h322GwCallsAvailable", -1);

      stat = asn1PD_H225_SeqOfH225CallsAvailable (pctxt, &pvalue->h322GwCallsAvailable);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "h322GwCallsAvailable", -1);
   }

   /* decode h323GwCallsAvailable */

   if (pvalue->m.h323GwCallsAvailablePresent) {
      rtInvokeStartElement (pctxt, "h323GwCallsAvailable", -1);

      stat = asn1PD_H225_SeqOfH225CallsAvailable (pctxt, &pvalue->h323GwCallsAvailable);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "h323GwCallsAvailable", -1);
   }

   /* decode h324GwCallsAvailable */

   if (pvalue->m.h324GwCallsAvailablePresent) {
      rtInvokeStartElement (pctxt, "h324GwCallsAvailable", -1);

      stat = asn1PD_H225_SeqOfH225CallsAvailable (pctxt, &pvalue->h324GwCallsAvailable);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "h324GwCallsAvailable", -1);
   }

   /* decode t120OnlyGwCallsAvailable */

   if (pvalue->m.t120OnlyGwCallsAvailablePresent) {
      rtInvokeStartElement (pctxt, "t120OnlyGwCallsAvailable", -1);

      stat = asn1PD_H225_SeqOfH225CallsAvailable (pctxt, &pvalue->t120OnlyGwCallsAvailable);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "t120OnlyGwCallsAvailable", -1);
   }

   /* decode t38FaxAnnexbOnlyGwCallsAvailable */

   if (pvalue->m.t38FaxAnnexbOnlyGwCallsAvailablePresent) {
      rtInvokeStartElement (pctxt, "t38FaxAnnexbOnlyGwCallsAvailable", -1);

      stat = asn1PD_H225_SeqOfH225CallsAvailable (pctxt, &pvalue->t38FaxAnnexbOnlyGwCallsAvailable);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "t38FaxAnnexbOnlyGwCallsAvailable", -1);
   }

   /* decode terminalCallsAvailable */

   if (pvalue->m.terminalCallsAvailablePresent) {
      rtInvokeStartElement (pctxt, "terminalCallsAvailable", -1);

      stat = asn1PD_H225_SeqOfH225CallsAvailable (pctxt, &pvalue->terminalCallsAvailable);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "terminalCallsAvailable", -1);
   }

   /* decode mcuCallsAvailable */

   if (pvalue->m.mcuCallsAvailablePresent) {
      rtInvokeStartElement (pctxt, "mcuCallsAvailable", -1);

      stat = asn1PD_H225_SeqOfH225CallsAvailable (pctxt, &pvalue->mcuCallsAvailable);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "mcuCallsAvailable", -1);
   }

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            if (i < 1 && openType.numocts > 0) {  /* known element */
               copyContext (&lctxt2, pctxt);
               initContextBuffer (pctxt, openType.data, openType.numocts);

               switch (i) {
                  case 0:
                     pvalue->m.sipGwCallsAvailablePresent = 1;

                     rtInvokeStartElement (pctxt, "sipGwCallsAvailable", -1);

                     stat = asn1PD_H225_SeqOfH225CallsAvailable (pctxt, &pvalue->sipGwCallsAvailable);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "sipGwCallsAvailable", -1);
                     break;

                  default:
                     pctxt->buffer.byteIndex += openType.numocts;
               }
               copyContext (pctxt, &lctxt2);
            }
            else {  /* unknown element */
               pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
               pOpenType->numocts = openType.numocts;
               pOpenType->data = openType.data;
               dListAppend (pctxt, &pvalue->extElem1, pOpenType);
            }
         }
         else if (i >= 1) {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  CallCapacity                                              */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225CallCapacity (OOCTXT* pctxt, H225CallCapacity* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.maximumCallCapacityPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.currentCallCapacityPresent = optbit;

   /* decode maximumCallCapacity */

   if (pvalue->m.maximumCallCapacityPresent) {
      rtInvokeStartElement (pctxt, "maximumCallCapacity", -1);

      stat = asn1PD_H225CallCapacityInfo (pctxt, &pvalue->maximumCallCapacity);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "maximumCallCapacity", -1);
   }

   /* decode currentCallCapacity */

   if (pvalue->m.currentCallCapacityPresent) {
      rtInvokeStartElement (pctxt, "currentCallCapacity", -1);

      stat = asn1PD_H225CallCapacityInfo (pctxt, &pvalue->currentCallCapacity);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "currentCallCapacity", -1);
   }

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
            pOpenType->numocts = openType.numocts;
            pOpenType->data = openType.data;
            dListAppend (pctxt, &pvalue->extElem1, pOpenType);
         }
         else {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  _SeqOfH225CicInfo_cic_element                             */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225_SeqOfH225CicInfo_cic_element (OOCTXT* pctxt, H225_SeqOfH225CicInfo_cic_element* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT xx1;

   /* decode length determinant */

   stat = decodeLength (pctxt, &pvalue->n);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* decode elements */

   ALLOC_ASN1ARRAY (pctxt, pvalue, H225CicInfo_cic_element);

   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      rtInvokeStartElement (pctxt, "elem", xx1);

      stat = asn1PD_H225CicInfo_cic_element (pctxt, &pvalue->elem[xx1]);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      rtInvokeEndElement (pctxt, "elem", xx1);

   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  CicInfo                                                   */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225CicInfo (OOCTXT* pctxt, H225CicInfo* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   dListInit (&pvalue->extElem1);

   /* decode cic */

   rtInvokeStartElement (pctxt, "cic", -1);

   stat = asn1PD_H225_SeqOfH225CicInfo_cic_element (pctxt, &pvalue->cic);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "cic", -1);

   /* decode pointCode */

   rtInvokeStartElement (pctxt, "pointCode", -1);

   stat = asn1PD_H225CicInfo_pointCode (pctxt, &pvalue->pointCode);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "pointCode", -1);

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
            pOpenType->numocts = openType.numocts;
            pOpenType->data = openType.data;
            dListAppend (pctxt, &pvalue->extElem1, pOpenType);
         }
         else {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  GroupID_member                                            */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225GroupID_member (OOCTXT* pctxt, H225GroupID_member* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT xx1;

   /* decode length determinant */

   stat = decodeLength (pctxt, &pvalue->n);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* decode elements */

   ALLOC_ASN1ARRAY (pctxt, pvalue, ASN1USINT);

   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      rtInvokeStartElement (pctxt, "elem", xx1);

      stat = decodeConsUInt16 (pctxt, &pvalue->elem[xx1], 0U, 65535U);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      rtInvokeUIntValue (pctxt, pvalue->elem[xx1]);
      rtInvokeEndElement (pctxt, "elem", xx1);

   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  GroupID                                                   */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225GroupID (OOCTXT* pctxt, H225GroupID* pvalue)
{
   static Asn1SizeCnst group_lsize1 = { 0, 1, 128, 0 };
   int stat = ASN_OK;
   OOCTXT lctxt;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.memberPresent = optbit;

   /* decode member */

   if (pvalue->m.memberPresent) {
      rtInvokeStartElement (pctxt, "member", -1);

      stat = asn1PD_H225GroupID_member (pctxt, &pvalue->member);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "member", -1);
   }

   /* decode group */

   rtInvokeStartElement (pctxt, "group", -1);

   stat = addSizeConstraint (pctxt, &group_lsize1);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   stat = decodeConstrainedStringEx (pctxt, &pvalue->group, 0, 8, 7, 7);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   rtInvokeCharStrValue (pctxt, pvalue->group);

   rtInvokeEndElement (pctxt, "group", -1);

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
            pOpenType->numocts = openType.numocts;
            pOpenType->data = openType.data;
            dListAppend (pctxt, &pvalue->extElem1, pOpenType);
         }
         else {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  CircuitIdentifier                                         */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225CircuitIdentifier (OOCTXT* pctxt, H225CircuitIdentifier* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   OOCTXT lctxt2;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.cicPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.groupPresent = optbit;

   /* decode cic */

   if (pvalue->m.cicPresent) {
      rtInvokeStartElement (pctxt, "cic", -1);

      stat = asn1PD_H225CicInfo (pctxt, &pvalue->cic);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "cic", -1);
   }

   /* decode group */

   if (pvalue->m.groupPresent) {
      rtInvokeStartElement (pctxt, "group", -1);

      stat = asn1PD_H225GroupID (pctxt, &pvalue->group);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "group", -1);
   }

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            if (i < 1 && openType.numocts > 0) {  /* known element */
               copyContext (&lctxt2, pctxt);
               initContextBuffer (pctxt, openType.data, openType.numocts);

               switch (i) {
                  case 0:
                     pvalue->m.carrierPresent = 1;

                     rtInvokeStartElement (pctxt, "carrier", -1);

                     stat = asn1PD_H225CarrierInfo (pctxt, &pvalue->carrier);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "carrier", -1);
                     break;

                  default:
                     pctxt->buffer.byteIndex += openType.numocts;
               }
               copyContext (pctxt, &lctxt2);
            }
            else {  /* unknown element */
               pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
               pOpenType->numocts = openType.numocts;
               pOpenType->data = openType.data;
               dListAppend (pctxt, &pvalue->extElem1, pOpenType);
            }
         }
         else if (i >= 1) {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  _SeqOfH225GenericData                                     */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225_SeqOfH225GenericData (OOCTXT* pctxt, H225_SeqOfH225GenericData* pvalue)
{
   int stat = ASN_OK;
   H225GenericData* pdata;
   ASN1UINT count = 0;
   ASN1UINT xx1;
   int lstat;

   dListInit (pvalue);

   for (;;) {
      /* decode length determinant */

      lstat = decodeLength (pctxt, &count);
      if (lstat != ASN_OK && lstat != ASN_OK_FRAG) {
         return LOG_ASN1ERR (pctxt, lstat);
      }

      /* decode elements */

      for (xx1 = 0; xx1 < count; xx1++) {
         rtInvokeStartElement (pctxt, "elem", xx1);

         pdata = ALLOC_ASN1ELEMDNODE (pctxt, H225GenericData);
         if (pdata == NULL)
            return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

         stat = asn1PD_H225GenericData (pctxt, pdata);
         if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
         rtInvokeEndElement (pctxt, "elem", xx1);

         dListAppendNode (pctxt, pvalue, pdata);
      }

      if(lstat == ASN_OK) break;
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  CircuitInfo                                               */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225CircuitInfo (OOCTXT* pctxt, H225CircuitInfo* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.sourceCircuitIDPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.destinationCircuitIDPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.genericDataPresent = optbit;

   /* decode sourceCircuitID */

   if (pvalue->m.sourceCircuitIDPresent) {
      rtInvokeStartElement (pctxt, "sourceCircuitID", -1);

      stat = asn1PD_H225CircuitIdentifier (pctxt, &pvalue->sourceCircuitID);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "sourceCircuitID", -1);
   }

   /* decode destinationCircuitID */

   if (pvalue->m.destinationCircuitIDPresent) {
      rtInvokeStartElement (pctxt, "destinationCircuitID", -1);

      stat = asn1PD_H225CircuitIdentifier (pctxt, &pvalue->destinationCircuitID);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "destinationCircuitID", -1);
   }

   /* decode genericData */

   if (pvalue->m.genericDataPresent) {
      rtInvokeStartElement (pctxt, "genericData", -1);

      stat = asn1PD_H225_SeqOfH225GenericData (pctxt, &pvalue->genericData);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "genericData", -1);
   }

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
            pOpenType->numocts = openType.numocts;
            pOpenType->data = openType.data;
            dListAppend (pctxt, &pvalue->extElem1, pOpenType);
         }
         else {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  _SeqOfH225FeatureDescriptor                               */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225_SeqOfH225FeatureDescriptor (OOCTXT* pctxt, H225_SeqOfH225FeatureDescriptor* pvalue)
{
   int stat = ASN_OK;
   H225FeatureDescriptor* pdata;
   ASN1UINT count = 0;
   ASN1UINT xx1;
   int lstat;

   dListInit (pvalue);

   for (;;) {
      /* decode length determinant */

      lstat = decodeLength (pctxt, &count);
      if (lstat != ASN_OK && lstat != ASN_OK_FRAG) {
         return LOG_ASN1ERR (pctxt, lstat);
      }

      /* decode elements */

      for (xx1 = 0; xx1 < count; xx1++) {
         rtInvokeStartElement (pctxt, "elem", xx1);

         pdata = ALLOC_ASN1ELEMDNODE (pctxt, H225FeatureDescriptor);
         if (pdata == NULL)
            return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

         stat = asn1PD_H225FeatureDescriptor (pctxt, pdata);
         if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
         rtInvokeEndElement (pctxt, "elem", xx1);

         dListAppendNode (pctxt, pvalue, pdata);
      }

      if(lstat == ASN_OK) break;
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  Setup_UUIE_parallelH245Control                            */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225Setup_UUIE_parallelH245Control (OOCTXT* pctxt, H225Setup_UUIE_parallelH245Control* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT xx1;

   /* decode length determinant */

   stat = decodeLength (pctxt, &pvalue->n);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* decode elements */

   ALLOC_ASN1ARRAY (pctxt, pvalue, ASN1DynOctStr);

   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      rtInvokeStartElement (pctxt, "elem", xx1);

      stat = decodeDynOctetString (pctxt, (ASN1DynOctStr*)&pvalue->elem[xx1]);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      rtInvokeOctStrValue (pctxt, pvalue->elem[xx1].numocts, pvalue->elem[xx1].data);
      rtInvokeEndElement (pctxt, "elem", xx1);

   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  ExtendedAliasAddress                                      */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225ExtendedAliasAddress (OOCTXT* pctxt, H225ExtendedAliasAddress* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.presentationIndicatorPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.screeningIndicatorPresent = optbit;

   /* decode address */

   rtInvokeStartElement (pctxt, "address", -1);

   stat = asn1PD_H225AliasAddress (pctxt, &pvalue->address);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "address", -1);

   /* decode presentationIndicator */

   if (pvalue->m.presentationIndicatorPresent) {
      rtInvokeStartElement (pctxt, "presentationIndicator", -1);

      stat = asn1PD_H225PresentationIndicator (pctxt, &pvalue->presentationIndicator);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "presentationIndicator", -1);
   }

   /* decode screeningIndicator */

   if (pvalue->m.screeningIndicatorPresent) {
      rtInvokeStartElement (pctxt, "screeningIndicator", -1);

      stat = asn1PD_H225ScreeningIndicator (pctxt, &pvalue->screeningIndicator);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "screeningIndicator", -1);
   }

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
            pOpenType->numocts = openType.numocts;
            pOpenType->data = openType.data;
            dListAppend (pctxt, &pvalue->extElem1, pOpenType);
         }
         else {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  _SeqOfH225ExtendedAliasAddress                            */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225_SeqOfH225ExtendedAliasAddress (OOCTXT* pctxt, H225_SeqOfH225ExtendedAliasAddress* pvalue)
{
   int stat = ASN_OK;
   H225ExtendedAliasAddress* pdata;
   ASN1UINT count = 0;
   ASN1UINT xx1;
   int lstat;

   dListInit (pvalue);

   for (;;) {
      /* decode length determinant */

      lstat = decodeLength (pctxt, &count);
      if (lstat != ASN_OK && lstat != ASN_OK_FRAG) {
         return LOG_ASN1ERR (pctxt, lstat);
      }

      /* decode elements */

      for (xx1 = 0; xx1 < count; xx1++) {
         rtInvokeStartElement (pctxt, "elem", xx1);

         pdata = ALLOC_ASN1ELEMDNODE (pctxt, H225ExtendedAliasAddress);
         if (pdata == NULL)
            return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

         stat = asn1PD_H225ExtendedAliasAddress (pctxt, pdata);
         if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
         rtInvokeEndElement (pctxt, "elem", xx1);

         dListAppendNode (pctxt, pvalue, pdata);
      }

      if(lstat == ASN_OK) break;
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  Setup_UUIE                                                */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225Setup_UUIE (OOCTXT* pctxt, H225Setup_UUIE* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   OOCTXT lctxt2;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.h245AddressPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.sourceAddressPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.destinationAddressPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.destCallSignalAddressPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.destExtraCallInfoPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.destExtraCRVPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.callServicesPresent = optbit;

   /* decode protocolIdentifier */

   rtInvokeStartElement (pctxt, "protocolIdentifier", -1);

   stat = asn1PD_H225ProtocolIdentifier (pctxt, &pvalue->protocolIdentifier);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "protocolIdentifier", -1);

   /* decode h245Address */

   if (pvalue->m.h245AddressPresent) {
      rtInvokeStartElement (pctxt, "h245Address", -1);

      stat = asn1PD_H225TransportAddress (pctxt, &pvalue->h245Address);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "h245Address", -1);
   }

   /* decode sourceAddress */

   if (pvalue->m.sourceAddressPresent) {
      rtInvokeStartElement (pctxt, "sourceAddress", -1);

      stat = asn1PD_H225_SeqOfH225AliasAddress (pctxt, &pvalue->sourceAddress);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "sourceAddress", -1);
   }

   /* decode sourceInfo */

   rtInvokeStartElement (pctxt, "sourceInfo", -1);

   stat = asn1PD_H225EndpointType (pctxt, &pvalue->sourceInfo);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "sourceInfo", -1);

   /* decode destinationAddress */

   if (pvalue->m.destinationAddressPresent) {
      rtInvokeStartElement (pctxt, "destinationAddress", -1);

      stat = asn1PD_H225_SeqOfH225AliasAddress (pctxt, &pvalue->destinationAddress);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "destinationAddress", -1);
   }

   /* decode destCallSignalAddress */

   if (pvalue->m.destCallSignalAddressPresent) {
      rtInvokeStartElement (pctxt, "destCallSignalAddress", -1);

      stat = asn1PD_H225TransportAddress (pctxt, &pvalue->destCallSignalAddress);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "destCallSignalAddress", -1);
   }

   /* decode destExtraCallInfo */

   if (pvalue->m.destExtraCallInfoPresent) {
      rtInvokeStartElement (pctxt, "destExtraCallInfo", -1);

      stat = asn1PD_H225_SeqOfH225AliasAddress (pctxt, &pvalue->destExtraCallInfo);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "destExtraCallInfo", -1);
   }

   /* decode destExtraCRV */

   if (pvalue->m.destExtraCRVPresent) {
      rtInvokeStartElement (pctxt, "destExtraCRV", -1);

      stat = asn1PD_H225_SeqOfH225CallReferenceValue (pctxt, &pvalue->destExtraCRV);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "destExtraCRV", -1);
   }

   /* decode activeMC */

   rtInvokeStartElement (pctxt, "activeMC", -1);

   stat = DECODEBIT (pctxt, &pvalue->activeMC);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   rtInvokeBoolValue (pctxt, pvalue->activeMC);

   rtInvokeEndElement (pctxt, "activeMC", -1);

   /* decode conferenceID */

   rtInvokeStartElement (pctxt, "conferenceID", -1);

   stat = asn1PD_H225ConferenceIdentifier (pctxt, &pvalue->conferenceID);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "conferenceID", -1);

   /* decode conferenceGoal */

   rtInvokeStartElement (pctxt, "conferenceGoal", -1);

   stat = asn1PD_H225Setup_UUIE_conferenceGoal (pctxt, &pvalue->conferenceGoal);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "conferenceGoal", -1);

   /* decode callServices */

   if (pvalue->m.callServicesPresent) {
      rtInvokeStartElement (pctxt, "callServices", -1);

      stat = asn1PD_H225QseriesOptions (pctxt, &pvalue->callServices);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "callServices", -1);
   }

   /* decode callType */

   rtInvokeStartElement (pctxt, "callType", -1);

   stat = asn1PD_H225CallType (pctxt, &pvalue->callType);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "callType", -1);

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            if (i < 27 && openType.numocts > 0) {  /* known element */
               copyContext (&lctxt2, pctxt);
               initContextBuffer (pctxt, openType.data, openType.numocts);

               switch (i) {
                  case 0:
                     pvalue->m.sourceCallSignalAddressPresent = 1;

                     rtInvokeStartElement (pctxt, "sourceCallSignalAddress", -1);

                     stat = asn1PD_H225TransportAddress (pctxt, &pvalue->sourceCallSignalAddress);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "sourceCallSignalAddress", -1);
                     break;

                  case 1:
                     pvalue->m.remoteExtensionAddressPresent = 1;

                     rtInvokeStartElement (pctxt, "remoteExtensionAddress", -1);

                     stat = asn1PD_H225AliasAddress (pctxt, &pvalue->remoteExtensionAddress);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "remoteExtensionAddress", -1);
                     break;

                  case 2:
                     pvalue->m.callIdentifierPresent = 1;

                     rtInvokeStartElement (pctxt, "callIdentifier", -1);

                     stat = asn1PD_H225CallIdentifier (pctxt, &pvalue->callIdentifier);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "callIdentifier", -1);
                     break;

                  case 3:
                     pvalue->m.h245SecurityCapabilityPresent = 1;

                     rtInvokeStartElement (pctxt, "h245SecurityCapability", -1);

                     stat = asn1PD_H225_SeqOfH225H245Security (pctxt, &pvalue->h245SecurityCapability);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "h245SecurityCapability", -1);
                     break;

                  case 4:
                     pvalue->m.tokensPresent = 1;

                     rtInvokeStartElement (pctxt, "tokens", -1);

                     stat = asn1PD_H225_SeqOfH225ClearToken (pctxt, &pvalue->tokens);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "tokens", -1);
                     break;

                  case 5:
                     pvalue->m.cryptoTokensPresent = 1;

                     rtInvokeStartElement (pctxt, "cryptoTokens", -1);

                     stat = asn1PD_H225_SeqOfH225CryptoH323Token (pctxt, &pvalue->cryptoTokens);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "cryptoTokens", -1);
                     break;

                  case 6:
                     pvalue->m.fastStartPresent = 1;

                     rtInvokeStartElement (pctxt, "fastStart", -1);

                     stat = asn1PD_H225Setup_UUIE_fastStart (pctxt, &pvalue->fastStart);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "fastStart", -1);
                     break;

                  case 7:
                     pvalue->m.mediaWaitForConnectPresent = 1;

                     rtInvokeStartElement (pctxt, "mediaWaitForConnect", -1);

                     stat = DECODEBIT (pctxt, &pvalue->mediaWaitForConnect);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
                     rtInvokeBoolValue (pctxt, pvalue->mediaWaitForConnect);

                     rtInvokeEndElement (pctxt, "mediaWaitForConnect", -1);
                     break;

                  case 8:
                     pvalue->m.canOverlapSendPresent = 1;

                     rtInvokeStartElement (pctxt, "canOverlapSend", -1);

                     stat = DECODEBIT (pctxt, &pvalue->canOverlapSend);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
                     rtInvokeBoolValue (pctxt, pvalue->canOverlapSend);

                     rtInvokeEndElement (pctxt, "canOverlapSend", -1);
                     break;

                  case 9:
                     pvalue->m.endpointIdentifierPresent = 1;

                     rtInvokeStartElement (pctxt, "endpointIdentifier", -1);

                     stat = asn1PD_H225EndpointIdentifier (pctxt, &pvalue->endpointIdentifier);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "endpointIdentifier", -1);
                     break;

                  case 10:
                     pvalue->m.multipleCallsPresent = 1;

                     rtInvokeStartElement (pctxt, "multipleCalls", -1);

                     stat = DECODEBIT (pctxt, &pvalue->multipleCalls);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
                     rtInvokeBoolValue (pctxt, pvalue->multipleCalls);

                     rtInvokeEndElement (pctxt, "multipleCalls", -1);
                     break;

                  case 11:
                     pvalue->m.maintainConnectionPresent = 1;

                     rtInvokeStartElement (pctxt, "maintainConnection", -1);

                     stat = DECODEBIT (pctxt, &pvalue->maintainConnection);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
                     rtInvokeBoolValue (pctxt, pvalue->maintainConnection);

                     rtInvokeEndElement (pctxt, "maintainConnection", -1);
                     break;

                  case 12:
                     pvalue->m.connectionParametersPresent = 1;

                     rtInvokeStartElement (pctxt, "connectionParameters", -1);

                     stat = asn1PD_H225Setup_UUIE_connectionParameters (pctxt, &pvalue->connectionParameters);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "connectionParameters", -1);
                     break;

                  case 13:
                     pvalue->m.languagePresent = 1;

                     rtInvokeStartElement (pctxt, "language", -1);

                     stat = asn1PD_H225Setup_UUIE_language (pctxt, &pvalue->language);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "language", -1);
                     break;

                  case 14:
                     pvalue->m.presentationIndicatorPresent = 1;

                     rtInvokeStartElement (pctxt, "presentationIndicator", -1);

                     stat = asn1PD_H225PresentationIndicator (pctxt, &pvalue->presentationIndicator);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "presentationIndicator", -1);
                     break;

                  case 15:
                     pvalue->m.screeningIndicatorPresent = 1;

                     rtInvokeStartElement (pctxt, "screeningIndicator", -1);

                     stat = asn1PD_H225ScreeningIndicator (pctxt, &pvalue->screeningIndicator);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "screeningIndicator", -1);
                     break;

                  case 16:
                     pvalue->m.serviceControlPresent = 1;

                     rtInvokeStartElement (pctxt, "serviceControl", -1);

                     stat = asn1PD_H225_SeqOfH225ServiceControlSession (pctxt, &pvalue->serviceControl);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "serviceControl", -1);
                     break;

                  case 17:
                     pvalue->m.symmetricOperationRequiredPresent = 1;

                     rtInvokeStartElement (pctxt, "symmetricOperationRequired", -1);

                     /* NULL */
                     rtInvokeNullValue (pctxt);

                     rtInvokeEndElement (pctxt, "symmetricOperationRequired", -1);
                     break;

                  case 18:
                     pvalue->m.capacityPresent = 1;

                     rtInvokeStartElement (pctxt, "capacity", -1);

                     stat = asn1PD_H225CallCapacity (pctxt, &pvalue->capacity);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "capacity", -1);
                     break;

                  case 19:
                     pvalue->m.circuitInfoPresent = 1;

                     rtInvokeStartElement (pctxt, "circuitInfo", -1);

                     stat = asn1PD_H225CircuitInfo (pctxt, &pvalue->circuitInfo);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "circuitInfo", -1);
                     break;

                  case 20:
                     pvalue->m.desiredProtocolsPresent = 1;

                     rtInvokeStartElement (pctxt, "desiredProtocols", -1);

                     stat = asn1PD_H225_SeqOfH225SupportedProtocols (pctxt, &pvalue->desiredProtocols);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "desiredProtocols", -1);
                     break;

                  case 21:
                     pvalue->m.neededFeaturesPresent = 1;

                     rtInvokeStartElement (pctxt, "neededFeatures", -1);

                     stat = asn1PD_H225_SeqOfH225FeatureDescriptor (pctxt, &pvalue->neededFeatures);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "neededFeatures", -1);
                     break;

                  case 22:
                     pvalue->m.desiredFeaturesPresent = 1;

                     rtInvokeStartElement (pctxt, "desiredFeatures", -1);

                     stat = asn1PD_H225_SeqOfH225FeatureDescriptor (pctxt, &pvalue->desiredFeatures);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "desiredFeatures", -1);
                     break;

                  case 23:
                     pvalue->m.supportedFeaturesPresent = 1;

                     rtInvokeStartElement (pctxt, "supportedFeatures", -1);

                     stat = asn1PD_H225_SeqOfH225FeatureDescriptor (pctxt, &pvalue->supportedFeatures);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "supportedFeatures", -1);
                     break;

                  case 24:
                     pvalue->m.parallelH245ControlPresent = 1;

                     rtInvokeStartElement (pctxt, "parallelH245Control", -1);

                     stat = asn1PD_H225Setup_UUIE_parallelH245Control (pctxt, &pvalue->parallelH245Control);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "parallelH245Control", -1);
                     break;

                  case 25:
                     pvalue->m.additionalSourceAddressesPresent = 1;

                     rtInvokeStartElement (pctxt, "additionalSourceAddresses", -1);

                     stat = asn1PD_H225_SeqOfH225ExtendedAliasAddress (pctxt, &pvalue->additionalSourceAddresses);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "additionalSourceAddresses", -1);
                     break;

                  case 26:
                     pvalue->m.hopCountPresent = 1;

                     rtInvokeStartElement (pctxt, "hopCount", -1);

                     stat = decodeConsUInt8 (pctxt, &pvalue->hopCount, 1U, 31U);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
                     rtInvokeUIntValue (pctxt, pvalue->hopCount);

                     rtInvokeEndElement (pctxt, "hopCount", -1);
                     break;

                  default:
                     pctxt->buffer.byteIndex += openType.numocts;
               }
               copyContext (pctxt, &lctxt2);
            }
            else {  /* unknown element */
               pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
               pOpenType->numocts = openType.numocts;
               pOpenType->data = openType.data;
               dListAppend (pctxt, &pvalue->extElem1, pOpenType);
            }
         }
         else if (i >= 27) {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  CallProceeding_UUIE_fastStart                             */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225CallProceeding_UUIE_fastStart (OOCTXT* pctxt, H225CallProceeding_UUIE_fastStart* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT xx1;

   /* decode length determinant */

   stat = decodeLength (pctxt, &pvalue->n);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* decode elements */

   ALLOC_ASN1ARRAY (pctxt, pvalue, ASN1DynOctStr);

   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      rtInvokeStartElement (pctxt, "elem", xx1);

      stat = decodeDynOctetString (pctxt, (ASN1DynOctStr*)&pvalue->elem[xx1]);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      rtInvokeOctStrValue (pctxt, pvalue->elem[xx1].numocts, pvalue->elem[xx1].data);
      rtInvokeEndElement (pctxt, "elem", xx1);

   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  FeatureSet                                                */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225FeatureSet (OOCTXT* pctxt, H225FeatureSet* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.neededFeaturesPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.desiredFeaturesPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.supportedFeaturesPresent = optbit;

   /* decode replacementFeatureSet */

   rtInvokeStartElement (pctxt, "replacementFeatureSet", -1);

   stat = DECODEBIT (pctxt, &pvalue->replacementFeatureSet);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   rtInvokeBoolValue (pctxt, pvalue->replacementFeatureSet);

   rtInvokeEndElement (pctxt, "replacementFeatureSet", -1);

   /* decode neededFeatures */

   if (pvalue->m.neededFeaturesPresent) {
      rtInvokeStartElement (pctxt, "neededFeatures", -1);

      stat = asn1PD_H225_SeqOfH225FeatureDescriptor (pctxt, &pvalue->neededFeatures);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "neededFeatures", -1);
   }

   /* decode desiredFeatures */

   if (pvalue->m.desiredFeaturesPresent) {
      rtInvokeStartElement (pctxt, "desiredFeatures", -1);

      stat = asn1PD_H225_SeqOfH225FeatureDescriptor (pctxt, &pvalue->desiredFeatures);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "desiredFeatures", -1);
   }

   /* decode supportedFeatures */

   if (pvalue->m.supportedFeaturesPresent) {
      rtInvokeStartElement (pctxt, "supportedFeatures", -1);

      stat = asn1PD_H225_SeqOfH225FeatureDescriptor (pctxt, &pvalue->supportedFeatures);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "supportedFeatures", -1);
   }

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
            pOpenType->numocts = openType.numocts;
            pOpenType->data = openType.data;
            dListAppend (pctxt, &pvalue->extElem1, pOpenType);
         }
         else {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  CallProceeding_UUIE                                       */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225CallProceeding_UUIE (OOCTXT* pctxt, H225CallProceeding_UUIE* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   OOCTXT lctxt2;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.h245AddressPresent = optbit;

   /* decode protocolIdentifier */

   rtInvokeStartElement (pctxt, "protocolIdentifier", -1);

   stat = asn1PD_H225ProtocolIdentifier (pctxt, &pvalue->protocolIdentifier);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "protocolIdentifier", -1);

   /* decode destinationInfo */

   rtInvokeStartElement (pctxt, "destinationInfo", -1);

   stat = asn1PD_H225EndpointType (pctxt, &pvalue->destinationInfo);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "destinationInfo", -1);

   /* decode h245Address */

   if (pvalue->m.h245AddressPresent) {
      rtInvokeStartElement (pctxt, "h245Address", -1);

      stat = asn1PD_H225TransportAddress (pctxt, &pvalue->h245Address);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "h245Address", -1);
   }

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            if (i < 9 && openType.numocts > 0) {  /* known element */
               copyContext (&lctxt2, pctxt);
               initContextBuffer (pctxt, openType.data, openType.numocts);

               switch (i) {
                  case 0:
                     pvalue->m.callIdentifierPresent = 1;

                     rtInvokeStartElement (pctxt, "callIdentifier", -1);

                     stat = asn1PD_H225CallIdentifier (pctxt, &pvalue->callIdentifier);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "callIdentifier", -1);
                     break;

                  case 1:
                     pvalue->m.h245SecurityModePresent = 1;

                     rtInvokeStartElement (pctxt, "h245SecurityMode", -1);

                     stat = asn1PD_H225H245Security (pctxt, &pvalue->h245SecurityMode);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "h245SecurityMode", -1);
                     break;

                  case 2:
                     pvalue->m.tokensPresent = 1;

                     rtInvokeStartElement (pctxt, "tokens", -1);

                     stat = asn1PD_H225_SeqOfH225ClearToken (pctxt, &pvalue->tokens);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "tokens", -1);
                     break;

                  case 3:
                     pvalue->m.cryptoTokensPresent = 1;

                     rtInvokeStartElement (pctxt, "cryptoTokens", -1);

                     stat = asn1PD_H225_SeqOfH225CryptoH323Token (pctxt, &pvalue->cryptoTokens);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "cryptoTokens", -1);
                     break;

                  case 4:
                     pvalue->m.fastStartPresent = 1;

                     rtInvokeStartElement (pctxt, "fastStart", -1);

                     stat = asn1PD_H225CallProceeding_UUIE_fastStart (pctxt, &pvalue->fastStart);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "fastStart", -1);
                     break;

                  case 5:
                     pvalue->m.multipleCallsPresent = 1;

                     rtInvokeStartElement (pctxt, "multipleCalls", -1);

                     stat = DECODEBIT (pctxt, &pvalue->multipleCalls);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
                     rtInvokeBoolValue (pctxt, pvalue->multipleCalls);

                     rtInvokeEndElement (pctxt, "multipleCalls", -1);
                     break;

                  case 6:
                     pvalue->m.maintainConnectionPresent = 1;

                     rtInvokeStartElement (pctxt, "maintainConnection", -1);

                     stat = DECODEBIT (pctxt, &pvalue->maintainConnection);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
                     rtInvokeBoolValue (pctxt, pvalue->maintainConnection);

                     rtInvokeEndElement (pctxt, "maintainConnection", -1);
                     break;

                  case 7:
                     pvalue->m.fastConnectRefusedPresent = 1;

                     rtInvokeStartElement (pctxt, "fastConnectRefused", -1);

                     /* NULL */
                     rtInvokeNullValue (pctxt);

                     rtInvokeEndElement (pctxt, "fastConnectRefused", -1);
                     break;

                  case 8:
                     pvalue->m.featureSetPresent = 1;

                     rtInvokeStartElement (pctxt, "featureSet", -1);

                     stat = asn1PD_H225FeatureSet (pctxt, &pvalue->featureSet);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "featureSet", -1);
                     break;

                  default:
                     pctxt->buffer.byteIndex += openType.numocts;
               }
               copyContext (pctxt, &lctxt2);
            }
            else {  /* unknown element */
               pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
               pOpenType->numocts = openType.numocts;
               pOpenType->data = openType.data;
               dListAppend (pctxt, &pvalue->extElem1, pOpenType);
            }
         }
         else if (i >= 9) {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  Connect_UUIE_fastStart                                    */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225Connect_UUIE_fastStart (OOCTXT* pctxt, H225Connect_UUIE_fastStart* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT xx1;

   /* decode length determinant */

   stat = decodeLength (pctxt, &pvalue->n);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* decode elements */

   ALLOC_ASN1ARRAY (pctxt, pvalue, ASN1DynOctStr);

   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      rtInvokeStartElement (pctxt, "elem", xx1);

      stat = decodeDynOctetString (pctxt, (ASN1DynOctStr*)&pvalue->elem[xx1]);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      rtInvokeOctStrValue (pctxt, pvalue->elem[xx1].numocts, pvalue->elem[xx1].data);
      rtInvokeEndElement (pctxt, "elem", xx1);

   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  Connect_UUIE_language                                     */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225Connect_UUIE_language (OOCTXT* pctxt, H225Connect_UUIE_language* pvalue)
{
   static Asn1SizeCnst element_lsize1 = { 0, 1, 32, 0 };
   int stat = ASN_OK;
   ASN1UINT xx1;

   /* decode length determinant */

   stat = decodeLength (pctxt, &pvalue->n);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* decode elements */

   ALLOC_ASN1ARRAY (pctxt, pvalue, ASN1IA5String);

   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      rtInvokeStartElement (pctxt, "elem", xx1);

      stat = addSizeConstraint (pctxt, &element_lsize1);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = decodeConstrainedStringEx (pctxt, &pvalue->elem[xx1], 0, 8, 7, 7);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      rtInvokeCharStrValue (pctxt, pvalue->elem[xx1]);
      rtInvokeEndElement (pctxt, "elem", xx1);

   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  Connect_UUIE                                              */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225Connect_UUIE (OOCTXT* pctxt, H225Connect_UUIE* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   OOCTXT lctxt2;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.h245AddressPresent = optbit;

   /* decode protocolIdentifier */

   rtInvokeStartElement (pctxt, "protocolIdentifier", -1);

   stat = asn1PD_H225ProtocolIdentifier (pctxt, &pvalue->protocolIdentifier);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "protocolIdentifier", -1);

   /* decode h245Address */

   if (pvalue->m.h245AddressPresent) {
      rtInvokeStartElement (pctxt, "h245Address", -1);

      stat = asn1PD_H225TransportAddress (pctxt, &pvalue->h245Address);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "h245Address", -1);
   }

   /* decode destinationInfo */

   rtInvokeStartElement (pctxt, "destinationInfo", -1);

   stat = asn1PD_H225EndpointType (pctxt, &pvalue->destinationInfo);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "destinationInfo", -1);

   /* decode conferenceID */

   rtInvokeStartElement (pctxt, "conferenceID", -1);

   stat = asn1PD_H225ConferenceIdentifier (pctxt, &pvalue->conferenceID);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "conferenceID", -1);

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            if (i < 15 && openType.numocts > 0) {  /* known element */
               copyContext (&lctxt2, pctxt);
               initContextBuffer (pctxt, openType.data, openType.numocts);

               switch (i) {
                  case 0:
                     pvalue->m.callIdentifierPresent = 1;

                     rtInvokeStartElement (pctxt, "callIdentifier", -1);

                     stat = asn1PD_H225CallIdentifier (pctxt, &pvalue->callIdentifier);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "callIdentifier", -1);
                     break;

                  case 1:
                     pvalue->m.h245SecurityModePresent = 1;

                     rtInvokeStartElement (pctxt, "h245SecurityMode", -1);

                     stat = asn1PD_H225H245Security (pctxt, &pvalue->h245SecurityMode);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "h245SecurityMode", -1);
                     break;

                  case 2:
                     pvalue->m.tokensPresent = 1;

                     rtInvokeStartElement (pctxt, "tokens", -1);

                     stat = asn1PD_H225_SeqOfH225ClearToken (pctxt, &pvalue->tokens);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "tokens", -1);
                     break;

                  case 3:
                     pvalue->m.cryptoTokensPresent = 1;

                     rtInvokeStartElement (pctxt, "cryptoTokens", -1);

                     stat = asn1PD_H225_SeqOfH225CryptoH323Token (pctxt, &pvalue->cryptoTokens);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "cryptoTokens", -1);
                     break;

                  case 4:
                     pvalue->m.fastStartPresent = 1;

                     rtInvokeStartElement (pctxt, "fastStart", -1);

                     stat = asn1PD_H225Connect_UUIE_fastStart (pctxt, &pvalue->fastStart);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "fastStart", -1);
                     break;

                  case 5:
                     pvalue->m.multipleCallsPresent = 1;

                     rtInvokeStartElement (pctxt, "multipleCalls", -1);

                     stat = DECODEBIT (pctxt, &pvalue->multipleCalls);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
                     rtInvokeBoolValue (pctxt, pvalue->multipleCalls);

                     rtInvokeEndElement (pctxt, "multipleCalls", -1);
                     break;

                  case 6:
                     pvalue->m.maintainConnectionPresent = 1;

                     rtInvokeStartElement (pctxt, "maintainConnection", -1);

                     stat = DECODEBIT (pctxt, &pvalue->maintainConnection);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
                     rtInvokeBoolValue (pctxt, pvalue->maintainConnection);

                     rtInvokeEndElement (pctxt, "maintainConnection", -1);
                     break;

                  case 7:
                     pvalue->m.languagePresent = 1;

                     rtInvokeStartElement (pctxt, "language", -1);

                     stat = asn1PD_H225Connect_UUIE_language (pctxt, &pvalue->language);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "language", -1);
                     break;

                  case 8:
                     pvalue->m.connectedAddressPresent = 1;

                     rtInvokeStartElement (pctxt, "connectedAddress", -1);

                     stat = asn1PD_H225_SeqOfH225AliasAddress (pctxt, &pvalue->connectedAddress);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "connectedAddress", -1);
                     break;

                  case 9:
                     pvalue->m.presentationIndicatorPresent = 1;

                     rtInvokeStartElement (pctxt, "presentationIndicator", -1);

                     stat = asn1PD_H225PresentationIndicator (pctxt, &pvalue->presentationIndicator);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "presentationIndicator", -1);
                     break;

                  case 10:
                     pvalue->m.screeningIndicatorPresent = 1;

                     rtInvokeStartElement (pctxt, "screeningIndicator", -1);

                     stat = asn1PD_H225ScreeningIndicator (pctxt, &pvalue->screeningIndicator);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "screeningIndicator", -1);
                     break;

                  case 11:
                     pvalue->m.fastConnectRefusedPresent = 1;

                     rtInvokeStartElement (pctxt, "fastConnectRefused", -1);

                     /* NULL */
                     rtInvokeNullValue (pctxt);

                     rtInvokeEndElement (pctxt, "fastConnectRefused", -1);
                     break;

                  case 12:
                     pvalue->m.serviceControlPresent = 1;

                     rtInvokeStartElement (pctxt, "serviceControl", -1);

                     stat = asn1PD_H225_SeqOfH225ServiceControlSession (pctxt, &pvalue->serviceControl);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "serviceControl", -1);
                     break;

                  case 13:
                     pvalue->m.capacityPresent = 1;

                     rtInvokeStartElement (pctxt, "capacity", -1);

                     stat = asn1PD_H225CallCapacity (pctxt, &pvalue->capacity);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "capacity", -1);
                     break;

                  case 14:
                     pvalue->m.featureSetPresent = 1;

                     rtInvokeStartElement (pctxt, "featureSet", -1);

                     stat = asn1PD_H225FeatureSet (pctxt, &pvalue->featureSet);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "featureSet", -1);
                     break;

                  default:
                     pctxt->buffer.byteIndex += openType.numocts;
               }
               copyContext (pctxt, &lctxt2);
            }
            else {  /* unknown element */
               pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
               pOpenType->numocts = openType.numocts;
               pOpenType->data = openType.data;
               dListAppend (pctxt, &pvalue->extElem1, pOpenType);
            }
         }
         else if (i >= 15) {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  Alerting_UUIE_fastStart                                   */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225Alerting_UUIE_fastStart (OOCTXT* pctxt, H225Alerting_UUIE_fastStart* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT xx1;

   /* decode length determinant */

   stat = decodeLength (pctxt, &pvalue->n);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* decode elements */

   ALLOC_ASN1ARRAY (pctxt, pvalue, ASN1DynOctStr);

   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      rtInvokeStartElement (pctxt, "elem", xx1);

      stat = decodeDynOctetString (pctxt, (ASN1DynOctStr*)&pvalue->elem[xx1]);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      rtInvokeOctStrValue (pctxt, pvalue->elem[xx1].numocts, pvalue->elem[xx1].data);
      rtInvokeEndElement (pctxt, "elem", xx1);

   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  Alerting_UUIE                                             */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225Alerting_UUIE (OOCTXT* pctxt, H225Alerting_UUIE* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   OOCTXT lctxt2;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.h245AddressPresent = optbit;

   /* decode protocolIdentifier */

   rtInvokeStartElement (pctxt, "protocolIdentifier", -1);

   stat = asn1PD_H225ProtocolIdentifier (pctxt, &pvalue->protocolIdentifier);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "protocolIdentifier", -1);

   /* decode destinationInfo */

   rtInvokeStartElement (pctxt, "destinationInfo", -1);

   stat = asn1PD_H225EndpointType (pctxt, &pvalue->destinationInfo);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "destinationInfo", -1);

   /* decode h245Address */

   if (pvalue->m.h245AddressPresent) {
      rtInvokeStartElement (pctxt, "h245Address", -1);

      stat = asn1PD_H225TransportAddress (pctxt, &pvalue->h245Address);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "h245Address", -1);
   }

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            if (i < 14 && openType.numocts > 0) {  /* known element */
               copyContext (&lctxt2, pctxt);
               initContextBuffer (pctxt, openType.data, openType.numocts);

               switch (i) {
                  case 0:
                     pvalue->m.callIdentifierPresent = 1;

                     rtInvokeStartElement (pctxt, "callIdentifier", -1);

                     stat = asn1PD_H225CallIdentifier (pctxt, &pvalue->callIdentifier);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "callIdentifier", -1);
                     break;

                  case 1:
                     pvalue->m.h245SecurityModePresent = 1;

                     rtInvokeStartElement (pctxt, "h245SecurityMode", -1);

                     stat = asn1PD_H225H245Security (pctxt, &pvalue->h245SecurityMode);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "h245SecurityMode", -1);
                     break;

                  case 2:
                     pvalue->m.tokensPresent = 1;

                     rtInvokeStartElement (pctxt, "tokens", -1);

                     stat = asn1PD_H225_SeqOfH225ClearToken (pctxt, &pvalue->tokens);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "tokens", -1);
                     break;

                  case 3:
                     pvalue->m.cryptoTokensPresent = 1;

                     rtInvokeStartElement (pctxt, "cryptoTokens", -1);

                     stat = asn1PD_H225_SeqOfH225CryptoH323Token (pctxt, &pvalue->cryptoTokens);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "cryptoTokens", -1);
                     break;

                  case 4:
                     pvalue->m.fastStartPresent = 1;

                     rtInvokeStartElement (pctxt, "fastStart", -1);

                     stat = asn1PD_H225Alerting_UUIE_fastStart (pctxt, &pvalue->fastStart);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "fastStart", -1);
                     break;

                  case 5:
                     pvalue->m.multipleCallsPresent = 1;

                     rtInvokeStartElement (pctxt, "multipleCalls", -1);

                     stat = DECODEBIT (pctxt, &pvalue->multipleCalls);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
                     rtInvokeBoolValue (pctxt, pvalue->multipleCalls);

                     rtInvokeEndElement (pctxt, "multipleCalls", -1);
                     break;

                  case 6:
                     pvalue->m.maintainConnectionPresent = 1;

                     rtInvokeStartElement (pctxt, "maintainConnection", -1);

                     stat = DECODEBIT (pctxt, &pvalue->maintainConnection);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
                     rtInvokeBoolValue (pctxt, pvalue->maintainConnection);

                     rtInvokeEndElement (pctxt, "maintainConnection", -1);
                     break;

                  case 7:
                     pvalue->m.alertingAddressPresent = 1;

                     rtInvokeStartElement (pctxt, "alertingAddress", -1);

                     stat = asn1PD_H225_SeqOfH225AliasAddress (pctxt, &pvalue->alertingAddress);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "alertingAddress", -1);
                     break;

                  case 8:
                     pvalue->m.presentationIndicatorPresent = 1;

                     rtInvokeStartElement (pctxt, "presentationIndicator", -1);

                     stat = asn1PD_H225PresentationIndicator (pctxt, &pvalue->presentationIndicator);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "presentationIndicator", -1);
                     break;

                  case 9:
                     pvalue->m.screeningIndicatorPresent = 1;

                     rtInvokeStartElement (pctxt, "screeningIndicator", -1);

                     stat = asn1PD_H225ScreeningIndicator (pctxt, &pvalue->screeningIndicator);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "screeningIndicator", -1);
                     break;

                  case 10:
                     pvalue->m.fastConnectRefusedPresent = 1;

                     rtInvokeStartElement (pctxt, "fastConnectRefused", -1);

                     /* NULL */
                     rtInvokeNullValue (pctxt);

                     rtInvokeEndElement (pctxt, "fastConnectRefused", -1);
                     break;

                  case 11:
                     pvalue->m.serviceControlPresent = 1;

                     rtInvokeStartElement (pctxt, "serviceControl", -1);

                     stat = asn1PD_H225_SeqOfH225ServiceControlSession (pctxt, &pvalue->serviceControl);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "serviceControl", -1);
                     break;

                  case 12:
                     pvalue->m.capacityPresent = 1;

                     rtInvokeStartElement (pctxt, "capacity", -1);

                     stat = asn1PD_H225CallCapacity (pctxt, &pvalue->capacity);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "capacity", -1);
                     break;

                  case 13:
                     pvalue->m.featureSetPresent = 1;

                     rtInvokeStartElement (pctxt, "featureSet", -1);

                     stat = asn1PD_H225FeatureSet (pctxt, &pvalue->featureSet);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "featureSet", -1);
                     break;

                  default:
                     pctxt->buffer.byteIndex += openType.numocts;
               }
               copyContext (pctxt, &lctxt2);
            }
            else {  /* unknown element */
               pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
               pOpenType->numocts = openType.numocts;
               pOpenType->data = openType.data;
               dListAppend (pctxt, &pvalue->extElem1, pOpenType);
            }
         }
         else if (i >= 14) {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  Information_UUIE_fastStart                                */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225Information_UUIE_fastStart (OOCTXT* pctxt, H225Information_UUIE_fastStart* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT xx1;

   /* decode length determinant */

   stat = decodeLength (pctxt, &pvalue->n);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* decode elements */

   ALLOC_ASN1ARRAY (pctxt, pvalue, ASN1DynOctStr);

   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      rtInvokeStartElement (pctxt, "elem", xx1);

      stat = decodeDynOctetString (pctxt, (ASN1DynOctStr*)&pvalue->elem[xx1]);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      rtInvokeOctStrValue (pctxt, pvalue->elem[xx1].numocts, pvalue->elem[xx1].data);
      rtInvokeEndElement (pctxt, "elem", xx1);

   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  Information_UUIE                                          */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225Information_UUIE (OOCTXT* pctxt, H225Information_UUIE* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   OOCTXT lctxt2;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   /* decode protocolIdentifier */

   rtInvokeStartElement (pctxt, "protocolIdentifier", -1);

   stat = asn1PD_H225ProtocolIdentifier (pctxt, &pvalue->protocolIdentifier);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "protocolIdentifier", -1);

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            if (i < 6 && openType.numocts > 0) {  /* known element */
               copyContext (&lctxt2, pctxt);
               initContextBuffer (pctxt, openType.data, openType.numocts);

               switch (i) {
                  case 0:
                     pvalue->m.callIdentifierPresent = 1;

                     rtInvokeStartElement (pctxt, "callIdentifier", -1);

                     stat = asn1PD_H225CallIdentifier (pctxt, &pvalue->callIdentifier);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "callIdentifier", -1);
                     break;

                  case 1:
                     pvalue->m.tokensPresent = 1;

                     rtInvokeStartElement (pctxt, "tokens", -1);

                     stat = asn1PD_H225_SeqOfH225ClearToken (pctxt, &pvalue->tokens);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "tokens", -1);
                     break;

                  case 2:
                     pvalue->m.cryptoTokensPresent = 1;

                     rtInvokeStartElement (pctxt, "cryptoTokens", -1);

                     stat = asn1PD_H225_SeqOfH225CryptoH323Token (pctxt, &pvalue->cryptoTokens);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "cryptoTokens", -1);
                     break;

                  case 3:
                     pvalue->m.fastStartPresent = 1;

                     rtInvokeStartElement (pctxt, "fastStart", -1);

                     stat = asn1PD_H225Information_UUIE_fastStart (pctxt, &pvalue->fastStart);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "fastStart", -1);
                     break;

                  case 4:
                     pvalue->m.fastConnectRefusedPresent = 1;

                     rtInvokeStartElement (pctxt, "fastConnectRefused", -1);

                     /* NULL */
                     rtInvokeNullValue (pctxt);

                     rtInvokeEndElement (pctxt, "fastConnectRefused", -1);
                     break;

                  case 5:
                     pvalue->m.circuitInfoPresent = 1;

                     rtInvokeStartElement (pctxt, "circuitInfo", -1);

                     stat = asn1PD_H225CircuitInfo (pctxt, &pvalue->circuitInfo);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "circuitInfo", -1);
                     break;

                  default:
                     pctxt->buffer.byteIndex += openType.numocts;
               }
               copyContext (pctxt, &lctxt2);
            }
            else {  /* unknown element */
               pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
               pOpenType->numocts = openType.numocts;
               pOpenType->data = openType.data;
               dListAppend (pctxt, &pvalue->extElem1, pOpenType);
            }
         }
         else if (i >= 6) {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  SecurityErrors                                            */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225SecurityErrors (OOCTXT* pctxt, H225SecurityErrors* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT ui;
   ASN1OpenType openType;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   if (!extbit) {
      stat = decodeConsUnsigned (pctxt, &ui, 0, 15);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      else pvalue->t = ui + 1;

      switch (ui) {
         /* securityWrongSyncTime */
         case 0:
            rtInvokeStartElement (pctxt, "securityWrongSyncTime", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "securityWrongSyncTime", -1);

            break;

         /* securityReplay */
         case 1:
            rtInvokeStartElement (pctxt, "securityReplay", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "securityReplay", -1);

            break;

         /* securityWrongGeneralID */
         case 2:
            rtInvokeStartElement (pctxt, "securityWrongGeneralID", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "securityWrongGeneralID", -1);

            break;

         /* securityWrongSendersID */
         case 3:
            rtInvokeStartElement (pctxt, "securityWrongSendersID", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "securityWrongSendersID", -1);

            break;

         /* securityIntegrityFailed */
         case 4:
            rtInvokeStartElement (pctxt, "securityIntegrityFailed", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "securityIntegrityFailed", -1);

            break;

         /* securityWrongOID */
         case 5:
            rtInvokeStartElement (pctxt, "securityWrongOID", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "securityWrongOID", -1);

            break;

         /* securityDHmismatch */
         case 6:
            rtInvokeStartElement (pctxt, "securityDHmismatch", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "securityDHmismatch", -1);

            break;

         /* securityCertificateExpired */
         case 7:
            rtInvokeStartElement (pctxt, "securityCertificateExpired", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "securityCertificateExpired", -1);

            break;

         /* securityCertificateDateInvalid */
         case 8:
            rtInvokeStartElement (pctxt, "securityCertificateDateInvalid", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "securityCertificateDateInvalid", -1);

            break;

         /* securityCertificateRevoked */
         case 9:
            rtInvokeStartElement (pctxt, "securityCertificateRevoked", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "securityCertificateRevoked", -1);

            break;

         /* securityCertificateNotReadable */
         case 10:
            rtInvokeStartElement (pctxt, "securityCertificateNotReadable", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "securityCertificateNotReadable", -1);

            break;

         /* securityCertificateSignatureInvalid */
         case 11:
            rtInvokeStartElement (pctxt, "securityCertificateSignatureInvalid", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "securityCertificateSignatureInvalid", -1);

            break;

         /* securityCertificateMissing */
         case 12:
            rtInvokeStartElement (pctxt, "securityCertificateMissing", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "securityCertificateMissing", -1);

            break;

         /* securityCertificateIncomplete */
         case 13:
            rtInvokeStartElement (pctxt, "securityCertificateIncomplete", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "securityCertificateIncomplete", -1);

            break;

         /* securityUnsupportedCertificateAlgOID */
         case 14:
            rtInvokeStartElement (pctxt, "securityUnsupportedCertificateAlgOID", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "securityUnsupportedCertificateAlgOID", -1);

            break;

         /* securityUnknownCA */
         case 15:
            rtInvokeStartElement (pctxt, "securityUnknownCA", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "securityUnknownCA", -1);

            break;

         default:
            return LOG_ASN1ERR (pctxt, ASN_E_INVOPT);
      }
   }
   else {
      stat = decodeSmallNonNegWholeNumber (pctxt, &ui);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      else pvalue->t = ui + 17;

      stat = decodeByteAlign (pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      pvalue->u.extElem1 = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
      if (pvalue->u.extElem1 == NULL)
         return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

      pvalue->u.extElem1->data = openType.data;
      pvalue->u.extElem1->numocts = openType.numocts;

   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  ReleaseCompleteReason                                     */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225ReleaseCompleteReason (OOCTXT* pctxt, H225ReleaseCompleteReason* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT ui;
   ASN1OpenType openType;
   ASN1BOOL extbit;
   OOCTXT lctxt;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   if (!extbit) {
      stat = decodeConsUnsigned (pctxt, &ui, 0, 11);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      else pvalue->t = ui + 1;

      switch (ui) {
         /* noBandwidth */
         case 0:
            rtInvokeStartElement (pctxt, "noBandwidth", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "noBandwidth", -1);

            break;

         /* gatekeeperResources */
         case 1:
            rtInvokeStartElement (pctxt, "gatekeeperResources", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "gatekeeperResources", -1);

            break;

         /* unreachableDestination */
         case 2:
            rtInvokeStartElement (pctxt, "unreachableDestination", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "unreachableDestination", -1);

            break;

         /* destinationRejection */
         case 3:
            rtInvokeStartElement (pctxt, "destinationRejection", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "destinationRejection", -1);

            break;

         /* invalidRevision */
         case 4:
            rtInvokeStartElement (pctxt, "invalidRevision", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "invalidRevision", -1);

            break;

         /* noPermission */
         case 5:
            rtInvokeStartElement (pctxt, "noPermission", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "noPermission", -1);

            break;

         /* unreachableGatekeeper */
         case 6:
            rtInvokeStartElement (pctxt, "unreachableGatekeeper", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "unreachableGatekeeper", -1);

            break;

         /* gatewayResources */
         case 7:
            rtInvokeStartElement (pctxt, "gatewayResources", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "gatewayResources", -1);

            break;

         /* badFormatAddress */
         case 8:
            rtInvokeStartElement (pctxt, "badFormatAddress", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "badFormatAddress", -1);

            break;

         /* adaptiveBusy */
         case 9:
            rtInvokeStartElement (pctxt, "adaptiveBusy", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "adaptiveBusy", -1);

            break;

         /* inConf */
         case 10:
            rtInvokeStartElement (pctxt, "inConf", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "inConf", -1);

            break;

         /* undefinedReason */
         case 11:
            rtInvokeStartElement (pctxt, "undefinedReason", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "undefinedReason", -1);

            break;

         default:
            return LOG_ASN1ERR (pctxt, ASN_E_INVOPT);
      }
   }
   else {
      stat = decodeSmallNonNegWholeNumber (pctxt, &ui);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      else pvalue->t = ui + 13;

      stat = decodeByteAlign (pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      copyContext (&lctxt, pctxt);
      initContextBuffer (pctxt, openType.data, openType.numocts);

      switch (pvalue->t) {
         /* facilityCallDeflection */
         case 13:
            rtInvokeStartElement (pctxt, "facilityCallDeflection", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "facilityCallDeflection", -1);

            break;

         /* securityDenied */
         case 14:
            rtInvokeStartElement (pctxt, "securityDenied", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "securityDenied", -1);

            break;

         /* calledPartyNotRegistered */
         case 15:
            rtInvokeStartElement (pctxt, "calledPartyNotRegistered", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "calledPartyNotRegistered", -1);

            break;

         /* callerNotRegistered */
         case 16:
            rtInvokeStartElement (pctxt, "callerNotRegistered", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "callerNotRegistered", -1);

            break;

         /* newConnectionNeeded */
         case 17:
            rtInvokeStartElement (pctxt, "newConnectionNeeded", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "newConnectionNeeded", -1);

            break;

         /* nonStandardReason */
         case 18:
            rtInvokeStartElement (pctxt, "nonStandardReason", -1);

            pvalue->u.nonStandardReason = ALLOC_ASN1ELEM (pctxt, H225NonStandardParameter);
            if (pvalue->u.nonStandardReason == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225NonStandardParameter (pctxt, pvalue->u.nonStandardReason);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "nonStandardReason", -1);

            break;

         /* replaceWithConferenceInvite */
         case 19:
            rtInvokeStartElement (pctxt, "replaceWithConferenceInvite", -1);

            pvalue->u.replaceWithConferenceInvite = ALLOC_ASN1ELEM (pctxt, H225ConferenceIdentifier);
            if (pvalue->u.replaceWithConferenceInvite == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225ConferenceIdentifier (pctxt, pvalue->u.replaceWithConferenceInvite);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "replaceWithConferenceInvite", -1);

            break;

         /* genericDataReason */
         case 20:
            rtInvokeStartElement (pctxt, "genericDataReason", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "genericDataReason", -1);

            break;

         /* neededFeatureNotSupported */
         case 21:
            rtInvokeStartElement (pctxt, "neededFeatureNotSupported", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "neededFeatureNotSupported", -1);

            break;

         /* tunnelledSignallingRejected */
         case 22:
            rtInvokeStartElement (pctxt, "tunnelledSignallingRejected", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "tunnelledSignallingRejected", -1);

            break;

         /* invalidCID */
         case 23:
            rtInvokeStartElement (pctxt, "invalidCID", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "invalidCID", -1);

            break;

         /* securityError */
         case 24:
            rtInvokeStartElement (pctxt, "securityError", -1);

            pvalue->u.securityError = ALLOC_ASN1ELEM (pctxt, H225SecurityErrors);
            if (pvalue->u.securityError == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225SecurityErrors (pctxt, pvalue->u.securityError);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "securityError", -1);

            break;

         /* hopCountExceeded */
         case 25:
            rtInvokeStartElement (pctxt, "hopCountExceeded", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "hopCountExceeded", -1);

            break;

         default:
            pvalue->u.extElem1 = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
            if (pvalue->u.extElem1 == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            pvalue->u.extElem1->data = openType.data;
            pvalue->u.extElem1->numocts = openType.numocts;

      }

      copyContext (pctxt, &lctxt);
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  ReleaseComplete_UUIE                                      */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225ReleaseComplete_UUIE (OOCTXT* pctxt, H225ReleaseComplete_UUIE* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   OOCTXT lctxt2;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.reasonPresent = optbit;

   /* decode protocolIdentifier */

   rtInvokeStartElement (pctxt, "protocolIdentifier", -1);

   stat = asn1PD_H225ProtocolIdentifier (pctxt, &pvalue->protocolIdentifier);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "protocolIdentifier", -1);

   /* decode reason */

   if (pvalue->m.reasonPresent) {
      rtInvokeStartElement (pctxt, "reason", -1);

      stat = asn1PD_H225ReleaseCompleteReason (pctxt, &pvalue->reason);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "reason", -1);
   }

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            if (i < 9 && openType.numocts > 0) {  /* known element */
               copyContext (&lctxt2, pctxt);
               initContextBuffer (pctxt, openType.data, openType.numocts);

               switch (i) {
                  case 0:
                     pvalue->m.callIdentifierPresent = 1;

                     rtInvokeStartElement (pctxt, "callIdentifier", -1);

                     stat = asn1PD_H225CallIdentifier (pctxt, &pvalue->callIdentifier);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "callIdentifier", -1);
                     break;

                  case 1:
                     pvalue->m.tokensPresent = 1;

                     rtInvokeStartElement (pctxt, "tokens", -1);

                     stat = asn1PD_H225_SeqOfH225ClearToken (pctxt, &pvalue->tokens);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "tokens", -1);
                     break;

                  case 2:
                     pvalue->m.cryptoTokensPresent = 1;

                     rtInvokeStartElement (pctxt, "cryptoTokens", -1);

                     stat = asn1PD_H225_SeqOfH225CryptoH323Token (pctxt, &pvalue->cryptoTokens);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "cryptoTokens", -1);
                     break;

                  case 3:
                     pvalue->m.busyAddressPresent = 1;

                     rtInvokeStartElement (pctxt, "busyAddress", -1);

                     stat = asn1PD_H225_SeqOfH225AliasAddress (pctxt, &pvalue->busyAddress);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "busyAddress", -1);
                     break;

                  case 4:
                     pvalue->m.presentationIndicatorPresent = 1;

                     rtInvokeStartElement (pctxt, "presentationIndicator", -1);

                     stat = asn1PD_H225PresentationIndicator (pctxt, &pvalue->presentationIndicator);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "presentationIndicator", -1);
                     break;

                  case 5:
                     pvalue->m.screeningIndicatorPresent = 1;

                     rtInvokeStartElement (pctxt, "screeningIndicator", -1);

                     stat = asn1PD_H225ScreeningIndicator (pctxt, &pvalue->screeningIndicator);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "screeningIndicator", -1);
                     break;

                  case 6:
                     pvalue->m.capacityPresent = 1;

                     rtInvokeStartElement (pctxt, "capacity", -1);

                     stat = asn1PD_H225CallCapacity (pctxt, &pvalue->capacity);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "capacity", -1);
                     break;

                  case 7:
                     pvalue->m.serviceControlPresent = 1;

                     rtInvokeStartElement (pctxt, "serviceControl", -1);

                     stat = asn1PD_H225_SeqOfH225ServiceControlSession (pctxt, &pvalue->serviceControl);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "serviceControl", -1);
                     break;

                  case 8:
                     pvalue->m.featureSetPresent = 1;

                     rtInvokeStartElement (pctxt, "featureSet", -1);

                     stat = asn1PD_H225FeatureSet (pctxt, &pvalue->featureSet);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "featureSet", -1);
                     break;

                  default:
                     pctxt->buffer.byteIndex += openType.numocts;
               }
               copyContext (pctxt, &lctxt2);
            }
            else {  /* unknown element */
               pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
               pOpenType->numocts = openType.numocts;
               pOpenType->data = openType.data;
               dListAppend (pctxt, &pvalue->extElem1, pOpenType);
            }
         }
         else if (i >= 9) {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  FacilityReason                                            */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225FacilityReason (OOCTXT* pctxt, H225FacilityReason* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT ui;
   ASN1OpenType openType;
   ASN1BOOL extbit;
   OOCTXT lctxt;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   if (!extbit) {
      stat = decodeConsUnsigned (pctxt, &ui, 0, 3);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      else pvalue->t = ui + 1;

      switch (ui) {
         /* routeCallToGatekeeper */
         case 0:
            rtInvokeStartElement (pctxt, "routeCallToGatekeeper", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "routeCallToGatekeeper", -1);

            break;

         /* callForwarded */
         case 1:
            rtInvokeStartElement (pctxt, "callForwarded", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "callForwarded", -1);

            break;

         /* routeCallToMC */
         case 2:
            rtInvokeStartElement (pctxt, "routeCallToMC", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "routeCallToMC", -1);

            break;

         /* undefinedReason */
         case 3:
            rtInvokeStartElement (pctxt, "undefinedReason", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "undefinedReason", -1);

            break;

         default:
            return LOG_ASN1ERR (pctxt, ASN_E_INVOPT);
      }
   }
   else {
      stat = decodeSmallNonNegWholeNumber (pctxt, &ui);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      else pvalue->t = ui + 5;

      stat = decodeByteAlign (pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      copyContext (&lctxt, pctxt);
      initContextBuffer (pctxt, openType.data, openType.numocts);

      switch (pvalue->t) {
         /* conferenceListChoice */
         case 5:
            rtInvokeStartElement (pctxt, "conferenceListChoice", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "conferenceListChoice", -1);

            break;

         /* startH245 */
         case 6:
            rtInvokeStartElement (pctxt, "startH245", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "startH245", -1);

            break;

         /* noH245 */
         case 7:
            rtInvokeStartElement (pctxt, "noH245", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "noH245", -1);

            break;

         /* newTokens */
         case 8:
            rtInvokeStartElement (pctxt, "newTokens", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "newTokens", -1);

            break;

         /* featureSetUpdate */
         case 9:
            rtInvokeStartElement (pctxt, "featureSetUpdate", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "featureSetUpdate", -1);

            break;

         /* forwardedElements */
         case 10:
            rtInvokeStartElement (pctxt, "forwardedElements", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "forwardedElements", -1);

            break;

         /* transportedInformation */
         case 11:
            rtInvokeStartElement (pctxt, "transportedInformation", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "transportedInformation", -1);

            break;

         default:
            pvalue->u.extElem1 = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
            if (pvalue->u.extElem1 == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            pvalue->u.extElem1->data = openType.data;
            pvalue->u.extElem1->numocts = openType.numocts;

      }

      copyContext (pctxt, &lctxt);
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  ConferenceList                                            */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225ConferenceList (OOCTXT* pctxt, H225ConferenceList* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.conferenceIDPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.conferenceAliasPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.nonStandardDataPresent = optbit;

   /* decode conferenceID */

   if (pvalue->m.conferenceIDPresent) {
      rtInvokeStartElement (pctxt, "conferenceID", -1);

      stat = asn1PD_H225ConferenceIdentifier (pctxt, &pvalue->conferenceID);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "conferenceID", -1);
   }

   /* decode conferenceAlias */

   if (pvalue->m.conferenceAliasPresent) {
      rtInvokeStartElement (pctxt, "conferenceAlias", -1);

      stat = asn1PD_H225AliasAddress (pctxt, &pvalue->conferenceAlias);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "conferenceAlias", -1);
   }

   /* decode nonStandardData */

   if (pvalue->m.nonStandardDataPresent) {
      rtInvokeStartElement (pctxt, "nonStandardData", -1);

      stat = asn1PD_H225NonStandardParameter (pctxt, &pvalue->nonStandardData);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "nonStandardData", -1);
   }

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
            pOpenType->numocts = openType.numocts;
            pOpenType->data = openType.data;
            dListAppend (pctxt, &pvalue->extElem1, pOpenType);
         }
         else {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  _SeqOfH225ConferenceList                                  */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225_SeqOfH225ConferenceList (OOCTXT* pctxt, H225_SeqOfH225ConferenceList* pvalue)
{
   int stat = ASN_OK;
   H225ConferenceList* pdata;
   ASN1UINT count = 0;
   ASN1UINT xx1;
   int lstat;

   dListInit (pvalue);

   for (;;) {
      /* decode length determinant */

      lstat = decodeLength (pctxt, &count);
      if (lstat != ASN_OK && lstat != ASN_OK_FRAG) {
         return LOG_ASN1ERR (pctxt, lstat);
      }

      /* decode elements */

      for (xx1 = 0; xx1 < count; xx1++) {
         rtInvokeStartElement (pctxt, "elem", xx1);

         pdata = ALLOC_ASN1ELEMDNODE (pctxt, H225ConferenceList);
         if (pdata == NULL)
            return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

         stat = asn1PD_H225ConferenceList (pctxt, pdata);
         if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
         rtInvokeEndElement (pctxt, "elem", xx1);

         dListAppendNode (pctxt, pvalue, pdata);
      }

      if(lstat == ASN_OK) break;
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  Facility_UUIE_fastStart                                   */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225Facility_UUIE_fastStart (OOCTXT* pctxt, H225Facility_UUIE_fastStart* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT xx1;

   /* decode length determinant */

   stat = decodeLength (pctxt, &pvalue->n);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* decode elements */

   ALLOC_ASN1ARRAY (pctxt, pvalue, ASN1DynOctStr);

   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      rtInvokeStartElement (pctxt, "elem", xx1);

      stat = decodeDynOctetString (pctxt, (ASN1DynOctStr*)&pvalue->elem[xx1]);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      rtInvokeOctStrValue (pctxt, pvalue->elem[xx1].numocts, pvalue->elem[xx1].data);
      rtInvokeEndElement (pctxt, "elem", xx1);

   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  Facility_UUIE                                             */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225Facility_UUIE (OOCTXT* pctxt, H225Facility_UUIE* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   OOCTXT lctxt2;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.alternativeAddressPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.alternativeAliasAddressPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.conferenceIDPresent = optbit;

   /* decode protocolIdentifier */

   rtInvokeStartElement (pctxt, "protocolIdentifier", -1);

   stat = asn1PD_H225ProtocolIdentifier (pctxt, &pvalue->protocolIdentifier);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "protocolIdentifier", -1);

   /* decode alternativeAddress */

   if (pvalue->m.alternativeAddressPresent) {
      rtInvokeStartElement (pctxt, "alternativeAddress", -1);

      stat = asn1PD_H225TransportAddress (pctxt, &pvalue->alternativeAddress);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "alternativeAddress", -1);
   }

   /* decode alternativeAliasAddress */

   if (pvalue->m.alternativeAliasAddressPresent) {
      rtInvokeStartElement (pctxt, "alternativeAliasAddress", -1);

      stat = asn1PD_H225_SeqOfH225AliasAddress (pctxt, &pvalue->alternativeAliasAddress);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "alternativeAliasAddress", -1);
   }

   /* decode conferenceID */

   if (pvalue->m.conferenceIDPresent) {
      rtInvokeStartElement (pctxt, "conferenceID", -1);

      stat = asn1PD_H225ConferenceIdentifier (pctxt, &pvalue->conferenceID);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "conferenceID", -1);
   }

   /* decode reason */

   rtInvokeStartElement (pctxt, "reason", -1);

   stat = asn1PD_H225FacilityReason (pctxt, &pvalue->reason);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "reason", -1);

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            if (i < 16 && openType.numocts > 0) {  /* known element */
               copyContext (&lctxt2, pctxt);
               initContextBuffer (pctxt, openType.data, openType.numocts);

               switch (i) {
                  case 0:
                     pvalue->m.callIdentifierPresent = 1;

                     rtInvokeStartElement (pctxt, "callIdentifier", -1);

                     stat = asn1PD_H225CallIdentifier (pctxt, &pvalue->callIdentifier);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "callIdentifier", -1);
                     break;

                  case 1:
                     pvalue->m.destExtraCallInfoPresent = 1;

                     rtInvokeStartElement (pctxt, "destExtraCallInfo", -1);

                     stat = asn1PD_H225_SeqOfH225AliasAddress (pctxt, &pvalue->destExtraCallInfo);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "destExtraCallInfo", -1);
                     break;

                  case 2:
                     pvalue->m.remoteExtensionAddressPresent = 1;

                     rtInvokeStartElement (pctxt, "remoteExtensionAddress", -1);

                     stat = asn1PD_H225AliasAddress (pctxt, &pvalue->remoteExtensionAddress);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "remoteExtensionAddress", -1);
                     break;

                  case 3:
                     pvalue->m.tokensPresent = 1;

                     rtInvokeStartElement (pctxt, "tokens", -1);

                     stat = asn1PD_H225_SeqOfH225ClearToken (pctxt, &pvalue->tokens);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "tokens", -1);
                     break;

                  case 4:
                     pvalue->m.cryptoTokensPresent = 1;

                     rtInvokeStartElement (pctxt, "cryptoTokens", -1);

                     stat = asn1PD_H225_SeqOfH225CryptoH323Token (pctxt, &pvalue->cryptoTokens);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "cryptoTokens", -1);
                     break;

                  case 5:
                     pvalue->m.conferencesPresent = 1;

                     rtInvokeStartElement (pctxt, "conferences", -1);

                     stat = asn1PD_H225_SeqOfH225ConferenceList (pctxt, &pvalue->conferences);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "conferences", -1);
                     break;

                  case 6:
                     pvalue->m.h245AddressPresent = 1;

                     rtInvokeStartElement (pctxt, "h245Address", -1);

                     stat = asn1PD_H225TransportAddress (pctxt, &pvalue->h245Address);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "h245Address", -1);
                     break;

                  case 7:
                     pvalue->m.fastStartPresent = 1;

                     rtInvokeStartElement (pctxt, "fastStart", -1);

                     stat = asn1PD_H225Facility_UUIE_fastStart (pctxt, &pvalue->fastStart);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "fastStart", -1);
                     break;

                  case 8:
                     pvalue->m.multipleCallsPresent = 1;

                     rtInvokeStartElement (pctxt, "multipleCalls", -1);

                     stat = DECODEBIT (pctxt, &pvalue->multipleCalls);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
                     rtInvokeBoolValue (pctxt, pvalue->multipleCalls);

                     rtInvokeEndElement (pctxt, "multipleCalls", -1);
                     break;

                  case 9:
                     pvalue->m.maintainConnectionPresent = 1;

                     rtInvokeStartElement (pctxt, "maintainConnection", -1);

                     stat = DECODEBIT (pctxt, &pvalue->maintainConnection);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
                     rtInvokeBoolValue (pctxt, pvalue->maintainConnection);

                     rtInvokeEndElement (pctxt, "maintainConnection", -1);
                     break;

                  case 10:
                     pvalue->m.fastConnectRefusedPresent = 1;

                     rtInvokeStartElement (pctxt, "fastConnectRefused", -1);

                     /* NULL */
                     rtInvokeNullValue (pctxt);

                     rtInvokeEndElement (pctxt, "fastConnectRefused", -1);
                     break;

                  case 11:
                     pvalue->m.serviceControlPresent = 1;

                     rtInvokeStartElement (pctxt, "serviceControl", -1);

                     stat = asn1PD_H225_SeqOfH225ServiceControlSession (pctxt, &pvalue->serviceControl);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "serviceControl", -1);
                     break;

                  case 12:
                     pvalue->m.circuitInfoPresent = 1;

                     rtInvokeStartElement (pctxt, "circuitInfo", -1);

                     stat = asn1PD_H225CircuitInfo (pctxt, &pvalue->circuitInfo);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "circuitInfo", -1);
                     break;

                  case 13:
                     pvalue->m.featureSetPresent = 1;

                     rtInvokeStartElement (pctxt, "featureSet", -1);

                     stat = asn1PD_H225FeatureSet (pctxt, &pvalue->featureSet);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "featureSet", -1);
                     break;

                  case 14:
                     pvalue->m.destinationInfoPresent = 1;

                     rtInvokeStartElement (pctxt, "destinationInfo", -1);

                     stat = asn1PD_H225EndpointType (pctxt, &pvalue->destinationInfo);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "destinationInfo", -1);
                     break;

                  case 15:
                     pvalue->m.h245SecurityModePresent = 1;

                     rtInvokeStartElement (pctxt, "h245SecurityMode", -1);

                     stat = asn1PD_H225H245Security (pctxt, &pvalue->h245SecurityMode);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "h245SecurityMode", -1);
                     break;

                  default:
                     pctxt->buffer.byteIndex += openType.numocts;
               }
               copyContext (pctxt, &lctxt2);
            }
            else {  /* unknown element */
               pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
               pOpenType->numocts = openType.numocts;
               pOpenType->data = openType.data;
               dListAppend (pctxt, &pvalue->extElem1, pOpenType);
            }
         }
         else if (i >= 16) {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  Progress_UUIE_fastStart                                   */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225Progress_UUIE_fastStart (OOCTXT* pctxt, H225Progress_UUIE_fastStart* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT xx1;

   /* decode length determinant */

   stat = decodeLength (pctxt, &pvalue->n);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* decode elements */

   ALLOC_ASN1ARRAY (pctxt, pvalue, ASN1DynOctStr);

   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      rtInvokeStartElement (pctxt, "elem", xx1);

      stat = decodeDynOctetString (pctxt, (ASN1DynOctStr*)&pvalue->elem[xx1]);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      rtInvokeOctStrValue (pctxt, pvalue->elem[xx1].numocts, pvalue->elem[xx1].data);
      rtInvokeEndElement (pctxt, "elem", xx1);

   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  Progress_UUIE                                             */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225Progress_UUIE (OOCTXT* pctxt, H225Progress_UUIE* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   OOCTXT lctxt2;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.h245AddressPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.h245SecurityModePresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.tokensPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.cryptoTokensPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.fastStartPresent = optbit;

   /* decode protocolIdentifier */

   rtInvokeStartElement (pctxt, "protocolIdentifier", -1);

   stat = asn1PD_H225ProtocolIdentifier (pctxt, &pvalue->protocolIdentifier);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "protocolIdentifier", -1);

   /* decode destinationInfo */

   rtInvokeStartElement (pctxt, "destinationInfo", -1);

   stat = asn1PD_H225EndpointType (pctxt, &pvalue->destinationInfo);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "destinationInfo", -1);

   /* decode h245Address */

   if (pvalue->m.h245AddressPresent) {
      rtInvokeStartElement (pctxt, "h245Address", -1);

      stat = asn1PD_H225TransportAddress (pctxt, &pvalue->h245Address);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "h245Address", -1);
   }

   /* decode callIdentifier */

   rtInvokeStartElement (pctxt, "callIdentifier", -1);

   stat = asn1PD_H225CallIdentifier (pctxt, &pvalue->callIdentifier);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "callIdentifier", -1);

   /* decode h245SecurityMode */

   if (pvalue->m.h245SecurityModePresent) {
      rtInvokeStartElement (pctxt, "h245SecurityMode", -1);

      stat = asn1PD_H225H245Security (pctxt, &pvalue->h245SecurityMode);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "h245SecurityMode", -1);
   }

   /* decode tokens */

   if (pvalue->m.tokensPresent) {
      rtInvokeStartElement (pctxt, "tokens", -1);

      stat = asn1PD_H225_SeqOfH225ClearToken (pctxt, &pvalue->tokens);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "tokens", -1);
   }

   /* decode cryptoTokens */

   if (pvalue->m.cryptoTokensPresent) {
      rtInvokeStartElement (pctxt, "cryptoTokens", -1);

      stat = asn1PD_H225_SeqOfH225CryptoH323Token (pctxt, &pvalue->cryptoTokens);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "cryptoTokens", -1);
   }

   /* decode fastStart */

   if (pvalue->m.fastStartPresent) {
      rtInvokeStartElement (pctxt, "fastStart", -1);

      stat = asn1PD_H225Progress_UUIE_fastStart (pctxt, &pvalue->fastStart);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "fastStart", -1);
   }

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            if (i < 3 && openType.numocts > 0) {  /* known element */
               copyContext (&lctxt2, pctxt);
               initContextBuffer (pctxt, openType.data, openType.numocts);

               switch (i) {
                  case 0:
                     pvalue->m.multipleCallsPresent = 1;

                     rtInvokeStartElement (pctxt, "multipleCalls", -1);

                     stat = DECODEBIT (pctxt, &pvalue->multipleCalls);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
                     rtInvokeBoolValue (pctxt, pvalue->multipleCalls);

                     rtInvokeEndElement (pctxt, "multipleCalls", -1);
                     break;

                  case 1:
                     pvalue->m.maintainConnectionPresent = 1;

                     rtInvokeStartElement (pctxt, "maintainConnection", -1);

                     stat = DECODEBIT (pctxt, &pvalue->maintainConnection);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
                     rtInvokeBoolValue (pctxt, pvalue->maintainConnection);

                     rtInvokeEndElement (pctxt, "maintainConnection", -1);
                     break;

                  case 2:
                     pvalue->m.fastConnectRefusedPresent = 1;

                     rtInvokeStartElement (pctxt, "fastConnectRefused", -1);

                     /* NULL */
                     rtInvokeNullValue (pctxt);

                     rtInvokeEndElement (pctxt, "fastConnectRefused", -1);
                     break;

                  default:
                     pctxt->buffer.byteIndex += openType.numocts;
               }
               copyContext (pctxt, &lctxt2);
            }
            else {  /* unknown element */
               pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
               pOpenType->numocts = openType.numocts;
               pOpenType->data = openType.data;
               dListAppend (pctxt, &pvalue->extElem1, pOpenType);
            }
         }
         else if (i >= 3) {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  Status_UUIE                                               */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225Status_UUIE (OOCTXT* pctxt, H225Status_UUIE* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.tokensPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.cryptoTokensPresent = optbit;

   /* decode protocolIdentifier */

   rtInvokeStartElement (pctxt, "protocolIdentifier", -1);

   stat = asn1PD_H225ProtocolIdentifier (pctxt, &pvalue->protocolIdentifier);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "protocolIdentifier", -1);

   /* decode callIdentifier */

   rtInvokeStartElement (pctxt, "callIdentifier", -1);

   stat = asn1PD_H225CallIdentifier (pctxt, &pvalue->callIdentifier);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "callIdentifier", -1);

   /* decode tokens */

   if (pvalue->m.tokensPresent) {
      rtInvokeStartElement (pctxt, "tokens", -1);

      stat = asn1PD_H225_SeqOfH225ClearToken (pctxt, &pvalue->tokens);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "tokens", -1);
   }

   /* decode cryptoTokens */

   if (pvalue->m.cryptoTokensPresent) {
      rtInvokeStartElement (pctxt, "cryptoTokens", -1);

      stat = asn1PD_H225_SeqOfH225CryptoH323Token (pctxt, &pvalue->cryptoTokens);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "cryptoTokens", -1);
   }

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
            pOpenType->numocts = openType.numocts;
            pOpenType->data = openType.data;
            dListAppend (pctxt, &pvalue->extElem1, pOpenType);
         }
         else {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  StatusInquiry_UUIE                                        */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225StatusInquiry_UUIE (OOCTXT* pctxt, H225StatusInquiry_UUIE* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.tokensPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.cryptoTokensPresent = optbit;

   /* decode protocolIdentifier */

   rtInvokeStartElement (pctxt, "protocolIdentifier", -1);

   stat = asn1PD_H225ProtocolIdentifier (pctxt, &pvalue->protocolIdentifier);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "protocolIdentifier", -1);

   /* decode callIdentifier */

   rtInvokeStartElement (pctxt, "callIdentifier", -1);

   stat = asn1PD_H225CallIdentifier (pctxt, &pvalue->callIdentifier);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "callIdentifier", -1);

   /* decode tokens */

   if (pvalue->m.tokensPresent) {
      rtInvokeStartElement (pctxt, "tokens", -1);

      stat = asn1PD_H225_SeqOfH225ClearToken (pctxt, &pvalue->tokens);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "tokens", -1);
   }

   /* decode cryptoTokens */

   if (pvalue->m.cryptoTokensPresent) {
      rtInvokeStartElement (pctxt, "cryptoTokens", -1);

      stat = asn1PD_H225_SeqOfH225CryptoH323Token (pctxt, &pvalue->cryptoTokens);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "cryptoTokens", -1);
   }

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
            pOpenType->numocts = openType.numocts;
            pOpenType->data = openType.data;
            dListAppend (pctxt, &pvalue->extElem1, pOpenType);
         }
         else {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  SetupAcknowledge_UUIE                                     */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225SetupAcknowledge_UUIE (OOCTXT* pctxt, H225SetupAcknowledge_UUIE* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.tokensPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.cryptoTokensPresent = optbit;

   /* decode protocolIdentifier */

   rtInvokeStartElement (pctxt, "protocolIdentifier", -1);

   stat = asn1PD_H225ProtocolIdentifier (pctxt, &pvalue->protocolIdentifier);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "protocolIdentifier", -1);

   /* decode callIdentifier */

   rtInvokeStartElement (pctxt, "callIdentifier", -1);

   stat = asn1PD_H225CallIdentifier (pctxt, &pvalue->callIdentifier);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "callIdentifier", -1);

   /* decode tokens */

   if (pvalue->m.tokensPresent) {
      rtInvokeStartElement (pctxt, "tokens", -1);

      stat = asn1PD_H225_SeqOfH225ClearToken (pctxt, &pvalue->tokens);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "tokens", -1);
   }

   /* decode cryptoTokens */

   if (pvalue->m.cryptoTokensPresent) {
      rtInvokeStartElement (pctxt, "cryptoTokens", -1);

      stat = asn1PD_H225_SeqOfH225CryptoH323Token (pctxt, &pvalue->cryptoTokens);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "cryptoTokens", -1);
   }

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
            pOpenType->numocts = openType.numocts;
            pOpenType->data = openType.data;
            dListAppend (pctxt, &pvalue->extElem1, pOpenType);
         }
         else {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  Notify_UUIE                                               */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225Notify_UUIE (OOCTXT* pctxt, H225Notify_UUIE* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.tokensPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.cryptoTokensPresent = optbit;

   /* decode protocolIdentifier */

   rtInvokeStartElement (pctxt, "protocolIdentifier", -1);

   stat = asn1PD_H225ProtocolIdentifier (pctxt, &pvalue->protocolIdentifier);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "protocolIdentifier", -1);

   /* decode callIdentifier */

   rtInvokeStartElement (pctxt, "callIdentifier", -1);

   stat = asn1PD_H225CallIdentifier (pctxt, &pvalue->callIdentifier);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "callIdentifier", -1);

   /* decode tokens */

   if (pvalue->m.tokensPresent) {
      rtInvokeStartElement (pctxt, "tokens", -1);

      stat = asn1PD_H225_SeqOfH225ClearToken (pctxt, &pvalue->tokens);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "tokens", -1);
   }

   /* decode cryptoTokens */

   if (pvalue->m.cryptoTokensPresent) {
      rtInvokeStartElement (pctxt, "cryptoTokens", -1);

      stat = asn1PD_H225_SeqOfH225CryptoH323Token (pctxt, &pvalue->cryptoTokens);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "cryptoTokens", -1);
   }

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
            pOpenType->numocts = openType.numocts;
            pOpenType->data = openType.data;
            dListAppend (pctxt, &pvalue->extElem1, pOpenType);
         }
         else {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  H323_UU_PDU_h323_message_body                             */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225H323_UU_PDU_h323_message_body (OOCTXT* pctxt, H225H323_UU_PDU_h323_message_body* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT ui;
   ASN1OpenType openType;
   ASN1BOOL extbit;
   OOCTXT lctxt;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   if (!extbit) {
      stat = decodeConsUnsigned (pctxt, &ui, 0, 6);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      else pvalue->t = ui + 1;

      switch (ui) {
         /* setup */
         case 0:
            rtInvokeStartElement (pctxt, "setup", -1);

            pvalue->u.setup = ALLOC_ASN1ELEM (pctxt, H225Setup_UUIE);
            if (pvalue->u.setup == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225Setup_UUIE (pctxt, pvalue->u.setup);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "setup", -1);

            break;

         /* callProceeding */
         case 1:
            rtInvokeStartElement (pctxt, "callProceeding", -1);

            pvalue->u.callProceeding = ALLOC_ASN1ELEM (pctxt, H225CallProceeding_UUIE);
            if (pvalue->u.callProceeding == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225CallProceeding_UUIE (pctxt, pvalue->u.callProceeding);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "callProceeding", -1);

            break;

         /* connect */
         case 2:
            rtInvokeStartElement (pctxt, "connect", -1);

            pvalue->u.connect = ALLOC_ASN1ELEM (pctxt, H225Connect_UUIE);
            if (pvalue->u.connect == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225Connect_UUIE (pctxt, pvalue->u.connect);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "connect", -1);

            break;

         /* alerting */
         case 3:
            rtInvokeStartElement (pctxt, "alerting", -1);

            pvalue->u.alerting = ALLOC_ASN1ELEM (pctxt, H225Alerting_UUIE);
            if (pvalue->u.alerting == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225Alerting_UUIE (pctxt, pvalue->u.alerting);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "alerting", -1);

            break;

         /* information */
         case 4:
            rtInvokeStartElement (pctxt, "information", -1);

            pvalue->u.information = ALLOC_ASN1ELEM (pctxt, H225Information_UUIE);
            if (pvalue->u.information == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225Information_UUIE (pctxt, pvalue->u.information);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "information", -1);

            break;

         /* releaseComplete */
         case 5:
            rtInvokeStartElement (pctxt, "releaseComplete", -1);

            pvalue->u.releaseComplete = ALLOC_ASN1ELEM (pctxt, H225ReleaseComplete_UUIE);
            if (pvalue->u.releaseComplete == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225ReleaseComplete_UUIE (pctxt, pvalue->u.releaseComplete);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "releaseComplete", -1);

            break;

         /* facility */
         case 6:
            rtInvokeStartElement (pctxt, "facility", -1);

            pvalue->u.facility = ALLOC_ASN1ELEM (pctxt, H225Facility_UUIE);
            if (pvalue->u.facility == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225Facility_UUIE (pctxt, pvalue->u.facility);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "facility", -1);

            break;

         default:
            return LOG_ASN1ERR (pctxt, ASN_E_INVOPT);
      }
   }
   else {
      stat = decodeSmallNonNegWholeNumber (pctxt, &ui);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      else pvalue->t = ui + 8;

      stat = decodeByteAlign (pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      copyContext (&lctxt, pctxt);
      initContextBuffer (pctxt, openType.data, openType.numocts);

      switch (pvalue->t) {
         /* progress */
         case 8:
            rtInvokeStartElement (pctxt, "progress", -1);

            pvalue->u.progress = ALLOC_ASN1ELEM (pctxt, H225Progress_UUIE);
            if (pvalue->u.progress == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225Progress_UUIE (pctxt, pvalue->u.progress);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "progress", -1);

            break;

         /* empty */
         case 9:
            rtInvokeStartElement (pctxt, "empty", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "empty", -1);

            break;

         /* status */
         case 10:
            rtInvokeStartElement (pctxt, "status", -1);

            pvalue->u.status = ALLOC_ASN1ELEM (pctxt, H225Status_UUIE);
            if (pvalue->u.status == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225Status_UUIE (pctxt, pvalue->u.status);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "status", -1);

            break;

         /* statusInquiry */
         case 11:
            rtInvokeStartElement (pctxt, "statusInquiry", -1);

            pvalue->u.statusInquiry = ALLOC_ASN1ELEM (pctxt, H225StatusInquiry_UUIE);
            if (pvalue->u.statusInquiry == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225StatusInquiry_UUIE (pctxt, pvalue->u.statusInquiry);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "statusInquiry", -1);

            break;

         /* setupAcknowledge */
         case 12:
            rtInvokeStartElement (pctxt, "setupAcknowledge", -1);

            pvalue->u.setupAcknowledge = ALLOC_ASN1ELEM (pctxt, H225SetupAcknowledge_UUIE);
            if (pvalue->u.setupAcknowledge == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225SetupAcknowledge_UUIE (pctxt, pvalue->u.setupAcknowledge);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "setupAcknowledge", -1);

            break;

         /* notify */
         case 13:
            rtInvokeStartElement (pctxt, "notify", -1);

            pvalue->u.notify = ALLOC_ASN1ELEM (pctxt, H225Notify_UUIE);
            if (pvalue->u.notify == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225Notify_UUIE (pctxt, pvalue->u.notify);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "notify", -1);

            break;

         default:
            pvalue->u.extElem1 = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
            if (pvalue->u.extElem1 == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            pvalue->u.extElem1->data = openType.data;
            pvalue->u.extElem1->numocts = openType.numocts;

      }

      copyContext (pctxt, &lctxt);
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  H323_UU_PDU_h4501SupplementaryService                     */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225H323_UU_PDU_h4501SupplementaryService (OOCTXT* pctxt, H225H323_UU_PDU_h4501SupplementaryService* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT xx1;

   /* decode length determinant */

   stat = decodeLength (pctxt, &pvalue->n);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* decode elements */

   ALLOC_ASN1ARRAY (pctxt, pvalue, ASN1DynOctStr);

   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      rtInvokeStartElement (pctxt, "elem", xx1);

      stat = decodeDynOctetString (pctxt, (ASN1DynOctStr*)&pvalue->elem[xx1]);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      rtInvokeOctStrValue (pctxt, pvalue->elem[xx1].numocts, pvalue->elem[xx1].data);
      rtInvokeEndElement (pctxt, "elem", xx1);

   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  H323_UU_PDU_h245Control                                   */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225H323_UU_PDU_h245Control (OOCTXT* pctxt, H225H323_UU_PDU_h245Control* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT xx1;

   /* decode length determinant */

   stat = decodeLength (pctxt, &pvalue->n);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* decode elements */

   ALLOC_ASN1ARRAY (pctxt, pvalue, ASN1DynOctStr);

   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      rtInvokeStartElement (pctxt, "elem", xx1);

      stat = decodeDynOctetString (pctxt, (ASN1DynOctStr*)&pvalue->elem[xx1]);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      rtInvokeOctStrValue (pctxt, pvalue->elem[xx1].numocts, pvalue->elem[xx1].data);
      rtInvokeEndElement (pctxt, "elem", xx1);

   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  _SeqOfH225NonStandardParameter                            */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225_SeqOfH225NonStandardParameter (OOCTXT* pctxt, H225_SeqOfH225NonStandardParameter* pvalue)
{
   int stat = ASN_OK;
   H225NonStandardParameter* pdata;
   ASN1UINT count = 0;
   ASN1UINT xx1;
   int lstat;

   dListInit (pvalue);

   for (;;) {
      /* decode length determinant */

      lstat = decodeLength (pctxt, &count);
      if (lstat != ASN_OK && lstat != ASN_OK_FRAG) {
         return LOG_ASN1ERR (pctxt, lstat);
      }

      /* decode elements */

      for (xx1 = 0; xx1 < count; xx1++) {
         rtInvokeStartElement (pctxt, "elem", xx1);

         pdata = ALLOC_ASN1ELEMDNODE (pctxt, H225NonStandardParameter);
         if (pdata == NULL)
            return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

         stat = asn1PD_H225NonStandardParameter (pctxt, pdata);
         if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
         rtInvokeEndElement (pctxt, "elem", xx1);

         dListAppendNode (pctxt, pvalue, pdata);
      }

      if(lstat == ASN_OK) break;
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  CallLinkage                                               */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225CallLinkage (OOCTXT* pctxt, H225CallLinkage* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.globalCallIdPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.threadIdPresent = optbit;

   /* decode globalCallId */

   if (pvalue->m.globalCallIdPresent) {
      rtInvokeStartElement (pctxt, "globalCallId", -1);

      stat = asn1PD_H225GloballyUniqueID (pctxt, &pvalue->globalCallId);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "globalCallId", -1);
   }

   /* decode threadId */

   if (pvalue->m.threadIdPresent) {
      rtInvokeStartElement (pctxt, "threadId", -1);

      stat = asn1PD_H225GloballyUniqueID (pctxt, &pvalue->threadId);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "threadId", -1);
   }

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
            pOpenType->numocts = openType.numocts;
            pOpenType->data = openType.data;
            dListAppend (pctxt, &pvalue->extElem1, pOpenType);
         }
         else {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  H323_UU_PDU_tunnelledSignallingMessage_messageContent     */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225H323_UU_PDU_tunnelledSignallingMessage_messageContent (OOCTXT* pctxt, H225H323_UU_PDU_tunnelledSignallingMessage_messageContent* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT xx1;

   /* decode length determinant */

   stat = decodeLength (pctxt, &pvalue->n);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* decode elements */

   ALLOC_ASN1ARRAY (pctxt, pvalue, ASN1DynOctStr);

   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      rtInvokeStartElement (pctxt, "elem", xx1);

      stat = decodeDynOctetString (pctxt, (ASN1DynOctStr*)&pvalue->elem[xx1]);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      rtInvokeOctStrValue (pctxt, pvalue->elem[xx1].numocts, pvalue->elem[xx1].data);
      rtInvokeEndElement (pctxt, "elem", xx1);

   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  H323_UU_PDU_tunnelledSignallingMessage                    */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225H323_UU_PDU_tunnelledSignallingMessage (OOCTXT* pctxt, H225H323_UU_PDU_tunnelledSignallingMessage* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.tunnellingRequiredPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.nonStandardDataPresent = optbit;

   /* decode tunnelledProtocolID */

   rtInvokeStartElement (pctxt, "tunnelledProtocolID", -1);

   stat = asn1PD_H225TunnelledProtocol (pctxt, &pvalue->tunnelledProtocolID);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "tunnelledProtocolID", -1);

   /* decode messageContent */

   rtInvokeStartElement (pctxt, "messageContent", -1);

   stat = asn1PD_H225H323_UU_PDU_tunnelledSignallingMessage_messageContent (pctxt, &pvalue->messageContent);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "messageContent", -1);

   /* decode tunnellingRequired */

   if (pvalue->m.tunnellingRequiredPresent) {
      rtInvokeStartElement (pctxt, "tunnellingRequired", -1);

      /* NULL */
      rtInvokeNullValue (pctxt);

      rtInvokeEndElement (pctxt, "tunnellingRequired", -1);
   }

   /* decode nonStandardData */

   if (pvalue->m.nonStandardDataPresent) {
      rtInvokeStartElement (pctxt, "nonStandardData", -1);

      stat = asn1PD_H225NonStandardParameter (pctxt, &pvalue->nonStandardData);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "nonStandardData", -1);
   }

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
            pOpenType->numocts = openType.numocts;
            pOpenType->data = openType.data;
            dListAppend (pctxt, &pvalue->extElem1, pOpenType);
         }
         else {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  StimulusControl                                           */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225StimulusControl (OOCTXT* pctxt, H225StimulusControl* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.nonStandardPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.isTextPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.h248MessagePresent = optbit;

   /* decode nonStandard */

   if (pvalue->m.nonStandardPresent) {
      rtInvokeStartElement (pctxt, "nonStandard", -1);

      stat = asn1PD_H225NonStandardParameter (pctxt, &pvalue->nonStandard);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "nonStandard", -1);
   }

   /* decode isText */

   if (pvalue->m.isTextPresent) {
      rtInvokeStartElement (pctxt, "isText", -1);

      /* NULL */
      rtInvokeNullValue (pctxt);

      rtInvokeEndElement (pctxt, "isText", -1);
   }

   /* decode h248Message */

   if (pvalue->m.h248MessagePresent) {
      rtInvokeStartElement (pctxt, "h248Message", -1);

      stat = decodeDynOctetString (pctxt, (ASN1DynOctStr*)&pvalue->h248Message);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      rtInvokeOctStrValue (pctxt, pvalue->h248Message.numocts, pvalue->h248Message.data);

      rtInvokeEndElement (pctxt, "h248Message", -1);
   }

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
            pOpenType->numocts = openType.numocts;
            pOpenType->data = openType.data;
            dListAppend (pctxt, &pvalue->extElem1, pOpenType);
         }
         else {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  H323_UU_PDU                                               */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225H323_UU_PDU (OOCTXT* pctxt, H225H323_UU_PDU* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   OOCTXT lctxt2;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.nonStandardDataPresent = optbit;

   /* decode h323_message_body */

   rtInvokeStartElement (pctxt, "h323_message_body", -1);

   stat = asn1PD_H225H323_UU_PDU_h323_message_body (pctxt, &pvalue->h323_message_body);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "h323_message_body", -1);

   /* decode nonStandardData */

   if (pvalue->m.nonStandardDataPresent) {
      rtInvokeStartElement (pctxt, "nonStandardData", -1);

      stat = asn1PD_H225NonStandardParameter (pctxt, &pvalue->nonStandardData);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "nonStandardData", -1);
   }

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            if (i < 9 && openType.numocts > 0) {  /* known element */
               copyContext (&lctxt2, pctxt);
               initContextBuffer (pctxt, openType.data, openType.numocts);

               switch (i) {
                  case 0:
                     pvalue->m.h4501SupplementaryServicePresent = 1;

                     rtInvokeStartElement (pctxt, "h4501SupplementaryService", -1);

                     stat = asn1PD_H225H323_UU_PDU_h4501SupplementaryService (pctxt, &pvalue->h4501SupplementaryService);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "h4501SupplementaryService", -1);
                     break;

                  case 1:
                     pvalue->m.h245TunnelingPresent = 1;

                     rtInvokeStartElement (pctxt, "h245Tunneling", -1);

                     stat = DECODEBIT (pctxt, &pvalue->h245Tunneling);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
                     rtInvokeBoolValue (pctxt, pvalue->h245Tunneling);

                     rtInvokeEndElement (pctxt, "h245Tunneling", -1);
                     break;

                  case 2:
                     pvalue->m.h245ControlPresent = 1;

                     rtInvokeStartElement (pctxt, "h245Control", -1);

                     stat = asn1PD_H225H323_UU_PDU_h245Control (pctxt, &pvalue->h245Control);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "h245Control", -1);
                     break;

                  case 3:
                     pvalue->m.nonStandardControlPresent = 1;

                     rtInvokeStartElement (pctxt, "nonStandardControl", -1);

                     stat = asn1PD_H225_SeqOfH225NonStandardParameter (pctxt, &pvalue->nonStandardControl);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "nonStandardControl", -1);
                     break;

                  case 4:
                     pvalue->m.callLinkagePresent = 1;

                     rtInvokeStartElement (pctxt, "callLinkage", -1);

                     stat = asn1PD_H225CallLinkage (pctxt, &pvalue->callLinkage);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "callLinkage", -1);
                     break;

                  case 5:
                     pvalue->m.tunnelledSignallingMessagePresent = 1;

                     rtInvokeStartElement (pctxt, "tunnelledSignallingMessage", -1);

                     stat = asn1PD_H225H323_UU_PDU_tunnelledSignallingMessage (pctxt, &pvalue->tunnelledSignallingMessage);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "tunnelledSignallingMessage", -1);
                     break;

                  case 6:
                     pvalue->m.provisionalRespToH245TunnelingPresent = 1;

                     rtInvokeStartElement (pctxt, "provisionalRespToH245Tunneling", -1);

                     /* NULL */
                     rtInvokeNullValue (pctxt);

                     rtInvokeEndElement (pctxt, "provisionalRespToH245Tunneling", -1);
                     break;

                  case 7:
                     pvalue->m.stimulusControlPresent = 1;

                     rtInvokeStartElement (pctxt, "stimulusControl", -1);

                     stat = asn1PD_H225StimulusControl (pctxt, &pvalue->stimulusControl);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "stimulusControl", -1);
                     break;

                  case 8:
                     pvalue->m.genericDataPresent = 1;

                     rtInvokeStartElement (pctxt, "genericData", -1);

                     stat = asn1PD_H225_SeqOfH225GenericData (pctxt, &pvalue->genericData);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "genericData", -1);
                     break;

                  default:
                     pctxt->buffer.byteIndex += openType.numocts;
               }
               copyContext (pctxt, &lctxt2);
            }
            else {  /* unknown element */
               pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
               pOpenType->numocts = openType.numocts;
               pOpenType->data = openType.data;
               dListAppend (pctxt, &pvalue->extElem1, pOpenType);
            }
         }
         else if (i >= 9) {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  H323_UserInformation_user_data                            */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225H323_UserInformation_user_data (OOCTXT* pctxt, H225H323_UserInformation_user_data* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   dListInit (&pvalue->extElem1);

   /* decode protocol_discriminator */

   rtInvokeStartElement (pctxt, "protocol_discriminator", -1);

   stat = decodeConsUInt8 (pctxt, &pvalue->protocol_discriminator, 0U, 255U);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   rtInvokeUIntValue (pctxt, pvalue->protocol_discriminator);

   rtInvokeEndElement (pctxt, "protocol_discriminator", -1);

   /* decode user_information */

   rtInvokeStartElement (pctxt, "user_information", -1);

   stat = asn1PD_H225H323_UserInformation_user_data_user_information (pctxt, &pvalue->user_information);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "user_information", -1);

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
            pOpenType->numocts = openType.numocts;
            pOpenType->data = openType.data;
            dListAppend (pctxt, &pvalue->extElem1, pOpenType);
         }
         else {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  H323_UserInformation                                      */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225H323_UserInformation (OOCTXT* pctxt, H225H323_UserInformation* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.user_dataPresent = optbit;

   /* decode h323_uu_pdu */

   rtInvokeStartElement (pctxt, "h323_uu_pdu", -1);

   stat = asn1PD_H225H323_UU_PDU (pctxt, &pvalue->h323_uu_pdu);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "h323_uu_pdu", -1);

   /* decode user_data */

   if (pvalue->m.user_dataPresent) {
      rtInvokeStartElement (pctxt, "user_data", -1);

      stat = asn1PD_H225H323_UserInformation_user_data (pctxt, &pvalue->user_data);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "user_data", -1);
   }

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
            pOpenType->numocts = openType.numocts;
            pOpenType->data = openType.data;
            dListAppend (pctxt, &pvalue->extElem1, pOpenType);
         }
         else {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  AddressPattern_range                                      */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225AddressPattern_range (OOCTXT* pctxt, H225AddressPattern_range* pvalue)
{
   int stat = ASN_OK;

   /* decode startOfRange */

   rtInvokeStartElement (pctxt, "startOfRange", -1);

   stat = asn1PD_H225PartyNumber (pctxt, &pvalue->startOfRange);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "startOfRange", -1);

   /* decode endOfRange */

   rtInvokeStartElement (pctxt, "endOfRange", -1);

   stat = asn1PD_H225PartyNumber (pctxt, &pvalue->endOfRange);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "endOfRange", -1);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  AddressPattern                                            */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225AddressPattern (OOCTXT* pctxt, H225AddressPattern* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT ui;
   ASN1OpenType openType;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   if (!extbit) {
      stat = decodeConsUnsigned (pctxt, &ui, 0, 1);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      else pvalue->t = ui + 1;

      switch (ui) {
         /* wildcard */
         case 0:
            rtInvokeStartElement (pctxt, "wildcard", -1);

            pvalue->u.wildcard = ALLOC_ASN1ELEM (pctxt, H225AliasAddress);
            if (pvalue->u.wildcard == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225AliasAddress (pctxt, pvalue->u.wildcard);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "wildcard", -1);

            break;

         /* range */
         case 1:
            rtInvokeStartElement (pctxt, "range", -1);

            pvalue->u.range = ALLOC_ASN1ELEM (pctxt, H225AddressPattern_range);
            if (pvalue->u.range == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225AddressPattern_range (pctxt, pvalue->u.range);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "range", -1);

            break;

         default:
            return LOG_ASN1ERR (pctxt, ASN_E_INVOPT);
      }
   }
   else {
      stat = decodeSmallNonNegWholeNumber (pctxt, &ui);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      else pvalue->t = ui + 3;

      stat = decodeByteAlign (pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      pvalue->u.extElem1 = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
      if (pvalue->u.extElem1 == NULL)
         return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

      pvalue->u.extElem1->data = openType.data;
      pvalue->u.extElem1->numocts = openType.numocts;

   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  _SeqOfH225TransportAddress                                */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225_SeqOfH225TransportAddress (OOCTXT* pctxt, H225_SeqOfH225TransportAddress* pvalue)
{
   int stat = ASN_OK;
   H225TransportAddress* pdata;
   ASN1UINT count = 0;
   ASN1UINT xx1;
   int lstat;

   dListInit (pvalue);

   for (;;) {
      /* decode length determinant */

      lstat = decodeLength (pctxt, &count);
      if (lstat != ASN_OK && lstat != ASN_OK_FRAG) {
         return LOG_ASN1ERR (pctxt, lstat);
      }

      /* decode elements */

      for (xx1 = 0; xx1 < count; xx1++) {
         rtInvokeStartElement (pctxt, "elem", xx1);

         pdata = ALLOC_ASN1ELEMDNODE (pctxt, H225TransportAddress);
         if (pdata == NULL)
            return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

         stat = asn1PD_H225TransportAddress (pctxt, pdata);
         if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
         rtInvokeEndElement (pctxt, "elem", xx1);

         dListAppendNode (pctxt, pvalue, pdata);
      }

      if(lstat == ASN_OK) break;
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  AlternateTransportAddresses                               */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225AlternateTransportAddresses (OOCTXT* pctxt, H225AlternateTransportAddresses* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   OOCTXT lctxt2;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.annexEPresent = optbit;

   /* decode annexE */

   if (pvalue->m.annexEPresent) {
      rtInvokeStartElement (pctxt, "annexE", -1);

      stat = asn1PD_H225_SeqOfH225TransportAddress (pctxt, &pvalue->annexE);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "annexE", -1);
   }

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            if (i < 1 && openType.numocts > 0) {  /* known element */
               copyContext (&lctxt2, pctxt);
               initContextBuffer (pctxt, openType.data, openType.numocts);

               switch (i) {
                  case 0:
                     pvalue->m.sctpPresent = 1;

                     rtInvokeStartElement (pctxt, "sctp", -1);

                     stat = asn1PD_H225_SeqOfH225TransportAddress (pctxt, &pvalue->sctp);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "sctp", -1);
                     break;

                  default:
                     pctxt->buffer.byteIndex += openType.numocts;
               }
               copyContext (pctxt, &lctxt2);
            }
            else {  /* unknown element */
               pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
               pOpenType->numocts = openType.numocts;
               pOpenType->data = openType.data;
               dListAppend (pctxt, &pvalue->extElem1, pOpenType);
            }
         }
         else if (i >= 1) {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  Endpoint                                                  */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225Endpoint (OOCTXT* pctxt, H225Endpoint* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   OOCTXT lctxt2;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.nonStandardDataPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.aliasAddressPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.callSignalAddressPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.rasAddressPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.endpointTypePresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.tokensPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.cryptoTokensPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.priorityPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.remoteExtensionAddressPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.destExtraCallInfoPresent = optbit;

   /* decode nonStandardData */

   if (pvalue->m.nonStandardDataPresent) {
      rtInvokeStartElement (pctxt, "nonStandardData", -1);

      stat = asn1PD_H225NonStandardParameter (pctxt, &pvalue->nonStandardData);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "nonStandardData", -1);
   }

   /* decode aliasAddress */

   if (pvalue->m.aliasAddressPresent) {
      rtInvokeStartElement (pctxt, "aliasAddress", -1);

      stat = asn1PD_H225_SeqOfH225AliasAddress (pctxt, &pvalue->aliasAddress);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "aliasAddress", -1);
   }

   /* decode callSignalAddress */

   if (pvalue->m.callSignalAddressPresent) {
      rtInvokeStartElement (pctxt, "callSignalAddress", -1);

      stat = asn1PD_H225_SeqOfH225TransportAddress (pctxt, &pvalue->callSignalAddress);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "callSignalAddress", -1);
   }

   /* decode rasAddress */

   if (pvalue->m.rasAddressPresent) {
      rtInvokeStartElement (pctxt, "rasAddress", -1);

      stat = asn1PD_H225_SeqOfH225TransportAddress (pctxt, &pvalue->rasAddress);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "rasAddress", -1);
   }

   /* decode endpointType */

   if (pvalue->m.endpointTypePresent) {
      rtInvokeStartElement (pctxt, "endpointType", -1);

      stat = asn1PD_H225EndpointType (pctxt, &pvalue->endpointType);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "endpointType", -1);
   }

   /* decode tokens */

   if (pvalue->m.tokensPresent) {
      rtInvokeStartElement (pctxt, "tokens", -1);

      stat = asn1PD_H225_SeqOfH225ClearToken (pctxt, &pvalue->tokens);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "tokens", -1);
   }

   /* decode cryptoTokens */

   if (pvalue->m.cryptoTokensPresent) {
      rtInvokeStartElement (pctxt, "cryptoTokens", -1);

      stat = asn1PD_H225_SeqOfH225CryptoH323Token (pctxt, &pvalue->cryptoTokens);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "cryptoTokens", -1);
   }

   /* decode priority */

   if (pvalue->m.priorityPresent) {
      rtInvokeStartElement (pctxt, "priority", -1);

      stat = decodeConsUInt8 (pctxt, &pvalue->priority, 0U, 127U);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      rtInvokeUIntValue (pctxt, pvalue->priority);

      rtInvokeEndElement (pctxt, "priority", -1);
   }

   /* decode remoteExtensionAddress */

   if (pvalue->m.remoteExtensionAddressPresent) {
      rtInvokeStartElement (pctxt, "remoteExtensionAddress", -1);

      stat = asn1PD_H225_SeqOfH225AliasAddress (pctxt, &pvalue->remoteExtensionAddress);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "remoteExtensionAddress", -1);
   }

   /* decode destExtraCallInfo */

   if (pvalue->m.destExtraCallInfoPresent) {
      rtInvokeStartElement (pctxt, "destExtraCallInfo", -1);

      stat = asn1PD_H225_SeqOfH225AliasAddress (pctxt, &pvalue->destExtraCallInfo);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "destExtraCallInfo", -1);
   }

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            if (i < 3 && openType.numocts > 0) {  /* known element */
               copyContext (&lctxt2, pctxt);
               initContextBuffer (pctxt, openType.data, openType.numocts);

               switch (i) {
                  case 0:
                     pvalue->m.alternateTransportAddressesPresent = 1;

                     rtInvokeStartElement (pctxt, "alternateTransportAddresses", -1);

                     stat = asn1PD_H225AlternateTransportAddresses (pctxt, &pvalue->alternateTransportAddresses);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "alternateTransportAddresses", -1);
                     break;

                  case 1:
                     pvalue->m.circuitInfoPresent = 1;

                     rtInvokeStartElement (pctxt, "circuitInfo", -1);

                     stat = asn1PD_H225CircuitInfo (pctxt, &pvalue->circuitInfo);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "circuitInfo", -1);
                     break;

                  case 2:
                     pvalue->m.featureSetPresent = 1;

                     rtInvokeStartElement (pctxt, "featureSet", -1);

                     stat = asn1PD_H225FeatureSet (pctxt, &pvalue->featureSet);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "featureSet", -1);
                     break;

                  default:
                     pctxt->buffer.byteIndex += openType.numocts;
               }
               copyContext (pctxt, &lctxt2);
            }
            else {  /* unknown element */
               pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
               pOpenType->numocts = openType.numocts;
               pOpenType->data = openType.data;
               dListAppend (pctxt, &pvalue->extElem1, pOpenType);
            }
         }
         else if (i >= 3) {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  UseSpecifiedTransport                                     */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225UseSpecifiedTransport (OOCTXT* pctxt, H225UseSpecifiedTransport* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT ui;
   ASN1OpenType openType;
   ASN1BOOL extbit;
   OOCTXT lctxt;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   if (!extbit) {
      stat = decodeConsUnsigned (pctxt, &ui, 0, 1);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      else pvalue->t = ui + 1;

      switch (ui) {
         /* tcp */
         case 0:
            rtInvokeStartElement (pctxt, "tcp", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "tcp", -1);

            break;

         /* annexE */
         case 1:
            rtInvokeStartElement (pctxt, "annexE", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "annexE", -1);

            break;

         default:
            return LOG_ASN1ERR (pctxt, ASN_E_INVOPT);
      }
   }
   else {
      stat = decodeSmallNonNegWholeNumber (pctxt, &ui);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      else pvalue->t = ui + 3;

      stat = decodeByteAlign (pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      copyContext (&lctxt, pctxt);
      initContextBuffer (pctxt, openType.data, openType.numocts);

      switch (pvalue->t) {
         /* sctp */
         case 3:
            rtInvokeStartElement (pctxt, "sctp", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "sctp", -1);

            break;

         default:
            pvalue->u.extElem1 = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
            if (pvalue->u.extElem1 == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            pvalue->u.extElem1->data = openType.data;
            pvalue->u.extElem1->numocts = openType.numocts;

      }

      copyContext (pctxt, &lctxt);
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  AlternateGK                                               */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225AlternateGK (OOCTXT* pctxt, H225AlternateGK* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.gatekeeperIdentifierPresent = optbit;

   /* decode rasAddress */

   rtInvokeStartElement (pctxt, "rasAddress", -1);

   stat = asn1PD_H225TransportAddress (pctxt, &pvalue->rasAddress);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "rasAddress", -1);

   /* decode gatekeeperIdentifier */

   if (pvalue->m.gatekeeperIdentifierPresent) {
      rtInvokeStartElement (pctxt, "gatekeeperIdentifier", -1);

      stat = asn1PD_H225GatekeeperIdentifier (pctxt, &pvalue->gatekeeperIdentifier);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "gatekeeperIdentifier", -1);
   }

   /* decode needToRegister */

   rtInvokeStartElement (pctxt, "needToRegister", -1);

   stat = DECODEBIT (pctxt, &pvalue->needToRegister);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   rtInvokeBoolValue (pctxt, pvalue->needToRegister);

   rtInvokeEndElement (pctxt, "needToRegister", -1);

   /* decode priority */

   rtInvokeStartElement (pctxt, "priority", -1);

   stat = decodeConsUInt8 (pctxt, &pvalue->priority, 0U, 127U);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   rtInvokeUIntValue (pctxt, pvalue->priority);

   rtInvokeEndElement (pctxt, "priority", -1);

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
            pOpenType->numocts = openType.numocts;
            pOpenType->data = openType.data;
            dListAppend (pctxt, &pvalue->extElem1, pOpenType);
         }
         else {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  _SeqOfH225AlternateGK                                     */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225_SeqOfH225AlternateGK (OOCTXT* pctxt, H225_SeqOfH225AlternateGK* pvalue)
{
   int stat = ASN_OK;
   H225AlternateGK* pdata;
   ASN1UINT count = 0;
   ASN1UINT xx1;
   int lstat;

   dListInit (pvalue);

   for (;;) {
      /* decode length determinant */

      lstat = decodeLength (pctxt, &count);
      if (lstat != ASN_OK && lstat != ASN_OK_FRAG) {
         return LOG_ASN1ERR (pctxt, lstat);
      }

      /* decode elements */

      for (xx1 = 0; xx1 < count; xx1++) {
         rtInvokeStartElement (pctxt, "elem", xx1);

         pdata = ALLOC_ASN1ELEMDNODE (pctxt, H225AlternateGK);
         if (pdata == NULL)
            return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

         stat = asn1PD_H225AlternateGK (pctxt, pdata);
         if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
         rtInvokeEndElement (pctxt, "elem", xx1);

         dListAppendNode (pctxt, pvalue, pdata);
      }

      if(lstat == ASN_OK) break;
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  AltGKInfo                                                 */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225AltGKInfo (OOCTXT* pctxt, H225AltGKInfo* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   dListInit (&pvalue->extElem1);

   /* decode alternateGatekeeper */

   rtInvokeStartElement (pctxt, "alternateGatekeeper", -1);

   stat = asn1PD_H225_SeqOfH225AlternateGK (pctxt, &pvalue->alternateGatekeeper);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "alternateGatekeeper", -1);

   /* decode altGKisPermanent */

   rtInvokeStartElement (pctxt, "altGKisPermanent", -1);

   stat = DECODEBIT (pctxt, &pvalue->altGKisPermanent);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   rtInvokeBoolValue (pctxt, pvalue->altGKisPermanent);

   rtInvokeEndElement (pctxt, "altGKisPermanent", -1);

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
            pOpenType->numocts = openType.numocts;
            pOpenType->data = openType.data;
            dListAppend (pctxt, &pvalue->extElem1, pOpenType);
         }
         else {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  SecurityErrors2                                           */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225SecurityErrors2 (OOCTXT* pctxt, H225SecurityErrors2* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT ui;
   ASN1OpenType openType;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   if (!extbit) {
      stat = decodeConsUnsigned (pctxt, &ui, 0, 5);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      else pvalue->t = ui + 1;

      switch (ui) {
         /* securityWrongSyncTime */
         case 0:
            rtInvokeStartElement (pctxt, "securityWrongSyncTime", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "securityWrongSyncTime", -1);

            break;

         /* securityReplay */
         case 1:
            rtInvokeStartElement (pctxt, "securityReplay", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "securityReplay", -1);

            break;

         /* securityWrongGeneralID */
         case 2:
            rtInvokeStartElement (pctxt, "securityWrongGeneralID", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "securityWrongGeneralID", -1);

            break;

         /* securityWrongSendersID */
         case 3:
            rtInvokeStartElement (pctxt, "securityWrongSendersID", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "securityWrongSendersID", -1);

            break;

         /* securityIntegrityFailed */
         case 4:
            rtInvokeStartElement (pctxt, "securityIntegrityFailed", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "securityIntegrityFailed", -1);

            break;

         /* securityWrongOID */
         case 5:
            rtInvokeStartElement (pctxt, "securityWrongOID", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "securityWrongOID", -1);

            break;

         default:
            return LOG_ASN1ERR (pctxt, ASN_E_INVOPT);
      }
   }
   else {
      stat = decodeSmallNonNegWholeNumber (pctxt, &ui);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      else pvalue->t = ui + 7;

      stat = decodeByteAlign (pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      pvalue->u.extElem1 = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
      if (pvalue->u.extElem1 == NULL)
         return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

      pvalue->u.extElem1->data = openType.data;
      pvalue->u.extElem1->numocts = openType.numocts;

   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  EncryptIntAlg                                             */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225EncryptIntAlg (OOCTXT* pctxt, H225EncryptIntAlg* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT ui;
   ASN1OpenType openType;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   if (!extbit) {
      stat = decodeConsUnsigned (pctxt, &ui, 0, 1);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      else pvalue->t = ui + 1;

      switch (ui) {
         /* nonStandard */
         case 0:
            rtInvokeStartElement (pctxt, "nonStandard", -1);

            pvalue->u.nonStandard = ALLOC_ASN1ELEM (pctxt, H225NonStandardParameter);
            if (pvalue->u.nonStandard == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225NonStandardParameter (pctxt, pvalue->u.nonStandard);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "nonStandard", -1);

            break;

         /* isoAlgorithm */
         case 1:
            rtInvokeStartElement (pctxt, "isoAlgorithm", -1);

            pvalue->u.isoAlgorithm = ALLOC_ASN1ELEM (pctxt, ASN1OBJID);
            if (pvalue->u.isoAlgorithm == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = decodeObjectIdentifier (pctxt, pvalue->u.isoAlgorithm);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
            rtInvokeOidValue (pctxt, pvalue->u.isoAlgorithm->numids, pvalue->u.isoAlgorithm->subid);

            rtInvokeEndElement (pctxt, "isoAlgorithm", -1);

            break;

         default:
            return LOG_ASN1ERR (pctxt, ASN_E_INVOPT);
      }
   }
   else {
      stat = decodeSmallNonNegWholeNumber (pctxt, &ui);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      else pvalue->t = ui + 3;

      stat = decodeByteAlign (pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      pvalue->u.extElem1 = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
      if (pvalue->u.extElem1 == NULL)
         return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

      pvalue->u.extElem1->data = openType.data;
      pvalue->u.extElem1->numocts = openType.numocts;

   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  NonIsoIntegrityMechanism                                  */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225NonIsoIntegrityMechanism (OOCTXT* pctxt, H225NonIsoIntegrityMechanism* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT ui;
   ASN1OpenType openType;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   if (!extbit) {
      stat = decodeConsUnsigned (pctxt, &ui, 0, 3);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      else pvalue->t = ui + 1;

      switch (ui) {
         /* hMAC_MD5 */
         case 0:
            rtInvokeStartElement (pctxt, "hMAC_MD5", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "hMAC_MD5", -1);

            break;

         /* hMAC_iso10118_2_s */
         case 1:
            rtInvokeStartElement (pctxt, "hMAC_iso10118_2_s", -1);

            pvalue->u.hMAC_iso10118_2_s = ALLOC_ASN1ELEM (pctxt, H225EncryptIntAlg);
            if (pvalue->u.hMAC_iso10118_2_s == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225EncryptIntAlg (pctxt, pvalue->u.hMAC_iso10118_2_s);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "hMAC_iso10118_2_s", -1);

            break;

         /* hMAC_iso10118_2_l */
         case 2:
            rtInvokeStartElement (pctxt, "hMAC_iso10118_2_l", -1);

            pvalue->u.hMAC_iso10118_2_l = ALLOC_ASN1ELEM (pctxt, H225EncryptIntAlg);
            if (pvalue->u.hMAC_iso10118_2_l == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225EncryptIntAlg (pctxt, pvalue->u.hMAC_iso10118_2_l);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "hMAC_iso10118_2_l", -1);

            break;

         /* hMAC_iso10118_3 */
         case 3:
            rtInvokeStartElement (pctxt, "hMAC_iso10118_3", -1);

            pvalue->u.hMAC_iso10118_3 = ALLOC_ASN1ELEM (pctxt, ASN1OBJID);
            if (pvalue->u.hMAC_iso10118_3 == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = decodeObjectIdentifier (pctxt, pvalue->u.hMAC_iso10118_3);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
            rtInvokeOidValue (pctxt, pvalue->u.hMAC_iso10118_3->numids, pvalue->u.hMAC_iso10118_3->subid);

            rtInvokeEndElement (pctxt, "hMAC_iso10118_3", -1);

            break;

         default:
            return LOG_ASN1ERR (pctxt, ASN_E_INVOPT);
      }
   }
   else {
      stat = decodeSmallNonNegWholeNumber (pctxt, &ui);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      else pvalue->t = ui + 5;

      stat = decodeByteAlign (pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      pvalue->u.extElem1 = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
      if (pvalue->u.extElem1 == NULL)
         return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

      pvalue->u.extElem1->data = openType.data;
      pvalue->u.extElem1->numocts = openType.numocts;

   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  IntegrityMechanism                                        */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225IntegrityMechanism (OOCTXT* pctxt, H225IntegrityMechanism* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT ui;
   ASN1OpenType openType;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   if (!extbit) {
      stat = decodeConsUnsigned (pctxt, &ui, 0, 3);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      else pvalue->t = ui + 1;

      switch (ui) {
         /* nonStandard */
         case 0:
            rtInvokeStartElement (pctxt, "nonStandard", -1);

            pvalue->u.nonStandard = ALLOC_ASN1ELEM (pctxt, H225NonStandardParameter);
            if (pvalue->u.nonStandard == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225NonStandardParameter (pctxt, pvalue->u.nonStandard);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "nonStandard", -1);

            break;

         /* digSig */
         case 1:
            rtInvokeStartElement (pctxt, "digSig", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "digSig", -1);

            break;

         /* iso9797 */
         case 2:
            rtInvokeStartElement (pctxt, "iso9797", -1);

            pvalue->u.iso9797 = ALLOC_ASN1ELEM (pctxt, ASN1OBJID);
            if (pvalue->u.iso9797 == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = decodeObjectIdentifier (pctxt, pvalue->u.iso9797);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
            rtInvokeOidValue (pctxt, pvalue->u.iso9797->numids, pvalue->u.iso9797->subid);

            rtInvokeEndElement (pctxt, "iso9797", -1);

            break;

         /* nonIsoIM */
         case 3:
            rtInvokeStartElement (pctxt, "nonIsoIM", -1);

            pvalue->u.nonIsoIM = ALLOC_ASN1ELEM (pctxt, H225NonIsoIntegrityMechanism);
            if (pvalue->u.nonIsoIM == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225NonIsoIntegrityMechanism (pctxt, pvalue->u.nonIsoIM);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "nonIsoIM", -1);

            break;

         default:
            return LOG_ASN1ERR (pctxt, ASN_E_INVOPT);
      }
   }
   else {
      stat = decodeSmallNonNegWholeNumber (pctxt, &ui);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      else pvalue->t = ui + 5;

      stat = decodeByteAlign (pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      pvalue->u.extElem1 = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
      if (pvalue->u.extElem1 == NULL)
         return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

      pvalue->u.extElem1->data = openType.data;
      pvalue->u.extElem1->numocts = openType.numocts;

   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  ICV                                                       */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225ICV (OOCTXT* pctxt, H225ICV* pvalue)
{
   int stat = ASN_OK;

   /* decode algorithmOID */

   rtInvokeStartElement (pctxt, "algorithmOID", -1);

   stat = decodeObjectIdentifier (pctxt, &pvalue->algorithmOID);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   rtInvokeOidValue (pctxt, pvalue->algorithmOID.numids, pvalue->algorithmOID.subid);

   rtInvokeEndElement (pctxt, "algorithmOID", -1);

   /* decode icv */

   rtInvokeStartElement (pctxt, "icv", -1);

   stat = decodeDynBitString (pctxt, (ASN1DynBitStr*)&pvalue->icv);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeBitStrValue (pctxt, pvalue->icv.numbits, pvalue->icv.data);

   rtInvokeEndElement (pctxt, "icv", -1);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  CapacityReportingCapability                               */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225CapacityReportingCapability (OOCTXT* pctxt, H225CapacityReportingCapability* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   dListInit (&pvalue->extElem1);

   /* decode canReportCallCapacity */

   rtInvokeStartElement (pctxt, "canReportCallCapacity", -1);

   stat = DECODEBIT (pctxt, &pvalue->canReportCallCapacity);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   rtInvokeBoolValue (pctxt, pvalue->canReportCallCapacity);

   rtInvokeEndElement (pctxt, "canReportCallCapacity", -1);

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
            pOpenType->numocts = openType.numocts;
            pOpenType->data = openType.data;
            dListAppend (pctxt, &pvalue->extElem1, pOpenType);
         }
         else {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  CapacityReportingSpecification_when                       */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225CapacityReportingSpecification_when (OOCTXT* pctxt, H225CapacityReportingSpecification_when* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.callStartPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.callEndPresent = optbit;

   /* decode callStart */

   if (pvalue->m.callStartPresent) {
      rtInvokeStartElement (pctxt, "callStart", -1);

      /* NULL */
      rtInvokeNullValue (pctxt);

      rtInvokeEndElement (pctxt, "callStart", -1);
   }

   /* decode callEnd */

   if (pvalue->m.callEndPresent) {
      rtInvokeStartElement (pctxt, "callEnd", -1);

      /* NULL */
      rtInvokeNullValue (pctxt);

      rtInvokeEndElement (pctxt, "callEnd", -1);
   }

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
            pOpenType->numocts = openType.numocts;
            pOpenType->data = openType.data;
            dListAppend (pctxt, &pvalue->extElem1, pOpenType);
         }
         else {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  CapacityReportingSpecification                            */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225CapacityReportingSpecification (OOCTXT* pctxt, H225CapacityReportingSpecification* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   dListInit (&pvalue->extElem1);

   /* decode when */

   rtInvokeStartElement (pctxt, "when", -1);

   stat = asn1PD_H225CapacityReportingSpecification_when (pctxt, &pvalue->when);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "when", -1);

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
            pOpenType->numocts = openType.numocts;
            pOpenType->data = openType.data;
            dListAppend (pctxt, &pvalue->extElem1, pOpenType);
         }
         else {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  RasUsageInfoTypes                                         */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225RasUsageInfoTypes (OOCTXT* pctxt, H225RasUsageInfoTypes* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.startTimePresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.endTimePresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.terminationCausePresent = optbit;

   /* decode nonStandardUsageTypes */

   rtInvokeStartElement (pctxt, "nonStandardUsageTypes", -1);

   stat = asn1PD_H225_SeqOfH225NonStandardParameter (pctxt, &pvalue->nonStandardUsageTypes);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "nonStandardUsageTypes", -1);

   /* decode startTime */

   if (pvalue->m.startTimePresent) {
      rtInvokeStartElement (pctxt, "startTime", -1);

      /* NULL */
      rtInvokeNullValue (pctxt);

      rtInvokeEndElement (pctxt, "startTime", -1);
   }

   /* decode endTime */

   if (pvalue->m.endTimePresent) {
      rtInvokeStartElement (pctxt, "endTime", -1);

      /* NULL */
      rtInvokeNullValue (pctxt);

      rtInvokeEndElement (pctxt, "endTime", -1);
   }

   /* decode terminationCause */

   if (pvalue->m.terminationCausePresent) {
      rtInvokeStartElement (pctxt, "terminationCause", -1);

      /* NULL */
      rtInvokeNullValue (pctxt);

      rtInvokeEndElement (pctxt, "terminationCause", -1);
   }

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
            pOpenType->numocts = openType.numocts;
            pOpenType->data = openType.data;
            dListAppend (pctxt, &pvalue->extElem1, pOpenType);
         }
         else {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  RasUsageSpecification_when                                */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225RasUsageSpecification_when (OOCTXT* pctxt, H225RasUsageSpecification_when* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.startPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.endPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.inIrrPresent = optbit;

   /* decode start */

   if (pvalue->m.startPresent) {
      rtInvokeStartElement (pctxt, "start", -1);

      /* NULL */
      rtInvokeNullValue (pctxt);

      rtInvokeEndElement (pctxt, "start", -1);
   }

   /* decode end */

   if (pvalue->m.endPresent) {
      rtInvokeStartElement (pctxt, "end", -1);

      /* NULL */
      rtInvokeNullValue (pctxt);

      rtInvokeEndElement (pctxt, "end", -1);
   }

   /* decode inIrr */

   if (pvalue->m.inIrrPresent) {
      rtInvokeStartElement (pctxt, "inIrr", -1);

      /* NULL */
      rtInvokeNullValue (pctxt);

      rtInvokeEndElement (pctxt, "inIrr", -1);
   }

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
            pOpenType->numocts = openType.numocts;
            pOpenType->data = openType.data;
            dListAppend (pctxt, &pvalue->extElem1, pOpenType);
         }
         else {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  RasUsageSpecification_callStartingPoint                   */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225RasUsageSpecification_callStartingPoint (OOCTXT* pctxt, H225RasUsageSpecification_callStartingPoint* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.alertingPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.connectPresent = optbit;

   /* decode alerting */

   if (pvalue->m.alertingPresent) {
      rtInvokeStartElement (pctxt, "alerting", -1);

      /* NULL */
      rtInvokeNullValue (pctxt);

      rtInvokeEndElement (pctxt, "alerting", -1);
   }

   /* decode connect */

   if (pvalue->m.connectPresent) {
      rtInvokeStartElement (pctxt, "connect", -1);

      /* NULL */
      rtInvokeNullValue (pctxt);

      rtInvokeEndElement (pctxt, "connect", -1);
   }

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
            pOpenType->numocts = openType.numocts;
            pOpenType->data = openType.data;
            dListAppend (pctxt, &pvalue->extElem1, pOpenType);
         }
         else {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  RasUsageSpecification                                     */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225RasUsageSpecification (OOCTXT* pctxt, H225RasUsageSpecification* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.callStartingPointPresent = optbit;

   /* decode when */

   rtInvokeStartElement (pctxt, "when", -1);

   stat = asn1PD_H225RasUsageSpecification_when (pctxt, &pvalue->when);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "when", -1);

   /* decode callStartingPoint */

   if (pvalue->m.callStartingPointPresent) {
      rtInvokeStartElement (pctxt, "callStartingPoint", -1);

      stat = asn1PD_H225RasUsageSpecification_callStartingPoint (pctxt, &pvalue->callStartingPoint);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "callStartingPoint", -1);
   }

   /* decode required */

   rtInvokeStartElement (pctxt, "required", -1);

   stat = asn1PD_H225RasUsageInfoTypes (pctxt, &pvalue->required);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "required", -1);

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
            pOpenType->numocts = openType.numocts;
            pOpenType->data = openType.data;
            dListAppend (pctxt, &pvalue->extElem1, pOpenType);
         }
         else {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  RasUsageInformation                                       */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225RasUsageInformation (OOCTXT* pctxt, H225RasUsageInformation* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.alertingTimePresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.connectTimePresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.endTimePresent = optbit;

   /* decode nonStandardUsageFields */

   rtInvokeStartElement (pctxt, "nonStandardUsageFields", -1);

   stat = asn1PD_H225_SeqOfH225NonStandardParameter (pctxt, &pvalue->nonStandardUsageFields);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "nonStandardUsageFields", -1);

   /* decode alertingTime */

   if (pvalue->m.alertingTimePresent) {
      rtInvokeStartElement (pctxt, "alertingTime", -1);

      stat = asn1PD_H235TimeStamp (pctxt, &pvalue->alertingTime);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "alertingTime", -1);
   }

   /* decode connectTime */

   if (pvalue->m.connectTimePresent) {
      rtInvokeStartElement (pctxt, "connectTime", -1);

      stat = asn1PD_H235TimeStamp (pctxt, &pvalue->connectTime);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "connectTime", -1);
   }

   /* decode endTime */

   if (pvalue->m.endTimePresent) {
      rtInvokeStartElement (pctxt, "endTime", -1);

      stat = asn1PD_H235TimeStamp (pctxt, &pvalue->endTime);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "endTime", -1);
   }

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
            pOpenType->numocts = openType.numocts;
            pOpenType->data = openType.data;
            dListAppend (pctxt, &pvalue->extElem1, pOpenType);
         }
         else {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  CallTerminationCause                                      */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225CallTerminationCause (OOCTXT* pctxt, H225CallTerminationCause* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT ui;
   ASN1OpenType openType;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   if (!extbit) {
      stat = decodeConsUnsigned (pctxt, &ui, 0, 1);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      else pvalue->t = ui + 1;

      switch (ui) {
         /* releaseCompleteReason */
         case 0:
            rtInvokeStartElement (pctxt, "releaseCompleteReason", -1);

            pvalue->u.releaseCompleteReason = ALLOC_ASN1ELEM (pctxt, H225ReleaseCompleteReason);
            if (pvalue->u.releaseCompleteReason == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225ReleaseCompleteReason (pctxt, pvalue->u.releaseCompleteReason);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "releaseCompleteReason", -1);

            break;

         /* releaseCompleteCauseIE */
         case 1:
            rtInvokeStartElement (pctxt, "releaseCompleteCauseIE", -1);

            pvalue->u.releaseCompleteCauseIE = ALLOC_ASN1ELEM (pctxt, H225CallTerminationCause_releaseCompleteCauseIE);
            if (pvalue->u.releaseCompleteCauseIE == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225CallTerminationCause_releaseCompleteCauseIE (pctxt, pvalue->u.releaseCompleteCauseIE);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "releaseCompleteCauseIE", -1);

            break;

         default:
            return LOG_ASN1ERR (pctxt, ASN_E_INVOPT);
      }
   }
   else {
      stat = decodeSmallNonNegWholeNumber (pctxt, &ui);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      else pvalue->t = ui + 3;

      stat = decodeByteAlign (pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      pvalue->u.extElem1 = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
      if (pvalue->u.extElem1 == NULL)
         return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

      pvalue->u.extElem1->data = openType.data;
      pvalue->u.extElem1->numocts = openType.numocts;

   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  TransportChannelInfo                                      */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225TransportChannelInfo (OOCTXT* pctxt, H225TransportChannelInfo* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.sendAddressPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.recvAddressPresent = optbit;

   /* decode sendAddress */

   if (pvalue->m.sendAddressPresent) {
      rtInvokeStartElement (pctxt, "sendAddress", -1);

      stat = asn1PD_H225TransportAddress (pctxt, &pvalue->sendAddress);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "sendAddress", -1);
   }

   /* decode recvAddress */

   if (pvalue->m.recvAddressPresent) {
      rtInvokeStartElement (pctxt, "recvAddress", -1);

      stat = asn1PD_H225TransportAddress (pctxt, &pvalue->recvAddress);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "recvAddress", -1);
   }

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
            pOpenType->numocts = openType.numocts;
            pOpenType->data = openType.data;
            dListAppend (pctxt, &pvalue->extElem1, pOpenType);
         }
         else {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  BandwidthDetails                                          */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225BandwidthDetails (OOCTXT* pctxt, H225BandwidthDetails* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   dListInit (&pvalue->extElem1);

   /* decode sender */

   rtInvokeStartElement (pctxt, "sender", -1);

   stat = DECODEBIT (pctxt, &pvalue->sender);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   rtInvokeBoolValue (pctxt, pvalue->sender);

   rtInvokeEndElement (pctxt, "sender", -1);

   /* decode multicast */

   rtInvokeStartElement (pctxt, "multicast", -1);

   stat = DECODEBIT (pctxt, &pvalue->multicast);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   rtInvokeBoolValue (pctxt, pvalue->multicast);

   rtInvokeEndElement (pctxt, "multicast", -1);

   /* decode bandwidth */

   rtInvokeStartElement (pctxt, "bandwidth", -1);

   stat = asn1PD_H225BandWidth (pctxt, &pvalue->bandwidth);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "bandwidth", -1);

   /* decode rtcpAddresses */

   rtInvokeStartElement (pctxt, "rtcpAddresses", -1);

   stat = asn1PD_H225TransportChannelInfo (pctxt, &pvalue->rtcpAddresses);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "rtcpAddresses", -1);

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
            pOpenType->numocts = openType.numocts;
            pOpenType->data = openType.data;
            dListAppend (pctxt, &pvalue->extElem1, pOpenType);
         }
         else {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  CallCreditCapability                                      */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225CallCreditCapability (OOCTXT* pctxt, H225CallCreditCapability* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.canDisplayAmountStringPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.canEnforceDurationLimitPresent = optbit;

   /* decode canDisplayAmountString */

   if (pvalue->m.canDisplayAmountStringPresent) {
      rtInvokeStartElement (pctxt, "canDisplayAmountString", -1);

      stat = DECODEBIT (pctxt, &pvalue->canDisplayAmountString);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      rtInvokeBoolValue (pctxt, pvalue->canDisplayAmountString);

      rtInvokeEndElement (pctxt, "canDisplayAmountString", -1);
   }

   /* decode canEnforceDurationLimit */

   if (pvalue->m.canEnforceDurationLimitPresent) {
      rtInvokeStartElement (pctxt, "canEnforceDurationLimit", -1);

      stat = DECODEBIT (pctxt, &pvalue->canEnforceDurationLimit);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      rtInvokeBoolValue (pctxt, pvalue->canEnforceDurationLimit);

      rtInvokeEndElement (pctxt, "canEnforceDurationLimit", -1);
   }

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
            pOpenType->numocts = openType.numocts;
            pOpenType->data = openType.data;
            dListAppend (pctxt, &pvalue->extElem1, pOpenType);
         }
         else {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  RTPSession_associatedSessionIds                           */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225RTPSession_associatedSessionIds (OOCTXT* pctxt, H225RTPSession_associatedSessionIds* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT xx1;

   /* decode length determinant */

   stat = decodeLength (pctxt, &pvalue->n);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* decode elements */

   ALLOC_ASN1ARRAY (pctxt, pvalue, ASN1UINT8);

   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      rtInvokeStartElement (pctxt, "elem", xx1);

      stat = decodeConsUInt8 (pctxt, &pvalue->elem[xx1], 1U, 255U);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      rtInvokeUIntValue (pctxt, pvalue->elem[xx1]);
      rtInvokeEndElement (pctxt, "elem", xx1);

   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  RTPSession                                                */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225RTPSession (OOCTXT* pctxt, H225RTPSession* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   OOCTXT lctxt2;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   /* decode rtpAddress */

   rtInvokeStartElement (pctxt, "rtpAddress", -1);

   stat = asn1PD_H225TransportChannelInfo (pctxt, &pvalue->rtpAddress);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "rtpAddress", -1);

   /* decode rtcpAddress */

   rtInvokeStartElement (pctxt, "rtcpAddress", -1);

   stat = asn1PD_H225TransportChannelInfo (pctxt, &pvalue->rtcpAddress);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "rtcpAddress", -1);

   /* decode cname */

   rtInvokeStartElement (pctxt, "cname", -1);

   stat = decodeConstrainedStringEx (pctxt, &pvalue->cname, 0, 8, 7, 7);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   rtInvokeCharStrValue (pctxt, pvalue->cname);

   rtInvokeEndElement (pctxt, "cname", -1);

   /* decode ssrc */

   rtInvokeStartElement (pctxt, "ssrc", -1);

   stat = decodeConsUnsigned (pctxt, &pvalue->ssrc, 1U, ASN1UINT_MAX);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   rtInvokeUIntValue (pctxt, pvalue->ssrc);

   rtInvokeEndElement (pctxt, "ssrc", -1);

   /* decode sessionId */

   rtInvokeStartElement (pctxt, "sessionId", -1);

   stat = decodeConsUInt8 (pctxt, &pvalue->sessionId, 1U, 255U);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   rtInvokeUIntValue (pctxt, pvalue->sessionId);

   rtInvokeEndElement (pctxt, "sessionId", -1);

   /* decode associatedSessionIds */

   rtInvokeStartElement (pctxt, "associatedSessionIds", -1);

   stat = asn1PD_H225RTPSession_associatedSessionIds (pctxt, &pvalue->associatedSessionIds);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "associatedSessionIds", -1);

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            if (i < 2 && openType.numocts > 0) {  /* known element */
               copyContext (&lctxt2, pctxt);
               initContextBuffer (pctxt, openType.data, openType.numocts);

               switch (i) {
                  case 0:
                     pvalue->m.multicastPresent = 1;

                     rtInvokeStartElement (pctxt, "multicast", -1);

                     /* NULL */
                     rtInvokeNullValue (pctxt);

                     rtInvokeEndElement (pctxt, "multicast", -1);
                     break;

                  case 1:
                     pvalue->m.bandwidthPresent = 1;

                     rtInvokeStartElement (pctxt, "bandwidth", -1);

                     stat = asn1PD_H225BandWidth (pctxt, &pvalue->bandwidth);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "bandwidth", -1);
                     break;

                  default:
                     pctxt->buffer.byteIndex += openType.numocts;
               }
               copyContext (pctxt, &lctxt2);
            }
            else {  /* unknown element */
               pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
               pOpenType->numocts = openType.numocts;
               pOpenType->data = openType.data;
               dListAppend (pctxt, &pvalue->extElem1, pOpenType);
            }
         }
         else if (i >= 2) {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  _SeqOfH225Endpoint                                        */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225_SeqOfH225Endpoint (OOCTXT* pctxt, H225_SeqOfH225Endpoint* pvalue)
{
   int stat = ASN_OK;
   H225Endpoint* pdata;
   ASN1UINT count = 0;
   ASN1UINT xx1;
   int lstat;

   dListInit (pvalue);

   for (;;) {
      /* decode length determinant */

      lstat = decodeLength (pctxt, &count);
      if (lstat != ASN_OK && lstat != ASN_OK_FRAG) {
         return LOG_ASN1ERR (pctxt, lstat);
      }

      /* decode elements */

      for (xx1 = 0; xx1 < count; xx1++) {
         rtInvokeStartElement (pctxt, "elem", xx1);

         pdata = ALLOC_ASN1ELEMDNODE (pctxt, H225Endpoint);
         if (pdata == NULL)
            return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

         stat = asn1PD_H225Endpoint (pctxt, pdata);
         if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
         rtInvokeEndElement (pctxt, "elem", xx1);

         dListAppendNode (pctxt, pvalue, pdata);
      }

      if(lstat == ASN_OK) break;
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  _SeqOfH225AuthenticationMechanism                         */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225_SeqOfH225AuthenticationMechanism (OOCTXT* pctxt, H225_SeqOfH225AuthenticationMechanism* pvalue)
{
   int stat = ASN_OK;
   H235AuthenticationMechanism* pdata;
   ASN1UINT count = 0;
   ASN1UINT xx1;
   int lstat;

   dListInit (pvalue);

   for (;;) {
      /* decode length determinant */

      lstat = decodeLength (pctxt, &count);
      if (lstat != ASN_OK && lstat != ASN_OK_FRAG) {
         return LOG_ASN1ERR (pctxt, lstat);
      }

      /* decode elements */

      for (xx1 = 0; xx1 < count; xx1++) {
         rtInvokeStartElement (pctxt, "elem", xx1);

         pdata = ALLOC_ASN1ELEMDNODE (pctxt, H235AuthenticationMechanism);
         if (pdata == NULL)
            return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

         stat = asn1PD_H235AuthenticationMechanism (pctxt, pdata);
         if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
         rtInvokeEndElement (pctxt, "elem", xx1);

         dListAppendNode (pctxt, pvalue, pdata);
      }

      if(lstat == ASN_OK) break;
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  GatekeeperRequest_algorithmOIDs                           */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225GatekeeperRequest_algorithmOIDs (OOCTXT* pctxt, H225GatekeeperRequest_algorithmOIDs* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT xx1;

   /* decode length determinant */

   stat = decodeLength (pctxt, &pvalue->n);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* decode elements */

   ALLOC_ASN1ARRAY (pctxt, pvalue, ASN1OBJID);

   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      rtInvokeStartElement (pctxt, "elem", xx1);

      stat = decodeObjectIdentifier (pctxt, &pvalue->elem[xx1]);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      rtInvokeOidValue (pctxt, pvalue->elem[xx1].numids, pvalue->elem[xx1].subid);
      rtInvokeEndElement (pctxt, "elem", xx1);

   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  _SeqOfH225IntegrityMechanism                              */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225_SeqOfH225IntegrityMechanism (OOCTXT* pctxt, H225_SeqOfH225IntegrityMechanism* pvalue)
{
   int stat = ASN_OK;
   H225IntegrityMechanism* pdata;
   ASN1UINT count = 0;
   ASN1UINT xx1;
   int lstat;

   dListInit (pvalue);

   for (;;) {
      /* decode length determinant */

      lstat = decodeLength (pctxt, &count);
      if (lstat != ASN_OK && lstat != ASN_OK_FRAG) {
         return LOG_ASN1ERR (pctxt, lstat);
      }

      /* decode elements */

      for (xx1 = 0; xx1 < count; xx1++) {
         rtInvokeStartElement (pctxt, "elem", xx1);

         pdata = ALLOC_ASN1ELEMDNODE (pctxt, H225IntegrityMechanism);
         if (pdata == NULL)
            return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

         stat = asn1PD_H225IntegrityMechanism (pctxt, pdata);
         if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
         rtInvokeEndElement (pctxt, "elem", xx1);

         dListAppendNode (pctxt, pvalue, pdata);
      }

      if(lstat == ASN_OK) break;
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  GatekeeperRequest                                         */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225GatekeeperRequest (OOCTXT* pctxt, H225GatekeeperRequest* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   OOCTXT lctxt2;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.nonStandardDataPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.gatekeeperIdentifierPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.callServicesPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.endpointAliasPresent = optbit;

   /* decode requestSeqNum */

   rtInvokeStartElement (pctxt, "requestSeqNum", -1);

   stat = asn1PD_H225RequestSeqNum (pctxt, &pvalue->requestSeqNum);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "requestSeqNum", -1);

   /* decode protocolIdentifier */

   rtInvokeStartElement (pctxt, "protocolIdentifier", -1);

   stat = asn1PD_H225ProtocolIdentifier (pctxt, &pvalue->protocolIdentifier);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "protocolIdentifier", -1);

   /* decode nonStandardData */

   if (pvalue->m.nonStandardDataPresent) {
      rtInvokeStartElement (pctxt, "nonStandardData", -1);

      stat = asn1PD_H225NonStandardParameter (pctxt, &pvalue->nonStandardData);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "nonStandardData", -1);
   }

   /* decode rasAddress */

   rtInvokeStartElement (pctxt, "rasAddress", -1);

   stat = asn1PD_H225TransportAddress (pctxt, &pvalue->rasAddress);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "rasAddress", -1);

   /* decode endpointType */

   rtInvokeStartElement (pctxt, "endpointType", -1);

   stat = asn1PD_H225EndpointType (pctxt, &pvalue->endpointType);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "endpointType", -1);

   /* decode gatekeeperIdentifier */

   if (pvalue->m.gatekeeperIdentifierPresent) {
      rtInvokeStartElement (pctxt, "gatekeeperIdentifier", -1);

      stat = asn1PD_H225GatekeeperIdentifier (pctxt, &pvalue->gatekeeperIdentifier);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "gatekeeperIdentifier", -1);
   }

   /* decode callServices */

   if (pvalue->m.callServicesPresent) {
      rtInvokeStartElement (pctxt, "callServices", -1);

      stat = asn1PD_H225QseriesOptions (pctxt, &pvalue->callServices);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "callServices", -1);
   }

   /* decode endpointAlias */

   if (pvalue->m.endpointAliasPresent) {
      rtInvokeStartElement (pctxt, "endpointAlias", -1);

      stat = asn1PD_H225_SeqOfH225AliasAddress (pctxt, &pvalue->endpointAlias);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "endpointAlias", -1);
   }

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            if (i < 10 && openType.numocts > 0) {  /* known element */
               copyContext (&lctxt2, pctxt);
               initContextBuffer (pctxt, openType.data, openType.numocts);

               switch (i) {
                  case 0:
                     pvalue->m.alternateEndpointsPresent = 1;

                     rtInvokeStartElement (pctxt, "alternateEndpoints", -1);

                     stat = asn1PD_H225_SeqOfH225Endpoint (pctxt, &pvalue->alternateEndpoints);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "alternateEndpoints", -1);
                     break;

                  case 1:
                     pvalue->m.tokensPresent = 1;

                     rtInvokeStartElement (pctxt, "tokens", -1);

                     stat = asn1PD_H225_SeqOfH225ClearToken (pctxt, &pvalue->tokens);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "tokens", -1);
                     break;

                  case 2:
                     pvalue->m.cryptoTokensPresent = 1;

                     rtInvokeStartElement (pctxt, "cryptoTokens", -1);

                     stat = asn1PD_H225_SeqOfH225CryptoH323Token (pctxt, &pvalue->cryptoTokens);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "cryptoTokens", -1);
                     break;

                  case 3:
                     pvalue->m.authenticationCapabilityPresent = 1;

                     rtInvokeStartElement (pctxt, "authenticationCapability", -1);

                     stat = asn1PD_H225_SeqOfH225AuthenticationMechanism (pctxt, &pvalue->authenticationCapability);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "authenticationCapability", -1);
                     break;

                  case 4:
                     pvalue->m.algorithmOIDsPresent = 1;

                     rtInvokeStartElement (pctxt, "algorithmOIDs", -1);

                     stat = asn1PD_H225GatekeeperRequest_algorithmOIDs (pctxt, &pvalue->algorithmOIDs);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "algorithmOIDs", -1);
                     break;

                  case 5:
                     pvalue->m.integrityPresent = 1;

                     rtInvokeStartElement (pctxt, "integrity", -1);

                     stat = asn1PD_H225_SeqOfH225IntegrityMechanism (pctxt, &pvalue->integrity);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "integrity", -1);
                     break;

                  case 6:
                     pvalue->m.integrityCheckValuePresent = 1;

                     rtInvokeStartElement (pctxt, "integrityCheckValue", -1);

                     stat = asn1PD_H225ICV (pctxt, &pvalue->integrityCheckValue);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "integrityCheckValue", -1);
                     break;

                  case 7:
                     pvalue->m.supportsAltGKPresent = 1;

                     rtInvokeStartElement (pctxt, "supportsAltGK", -1);

                     /* NULL */
                     rtInvokeNullValue (pctxt);

                     rtInvokeEndElement (pctxt, "supportsAltGK", -1);
                     break;

                  case 8:
                     pvalue->m.featureSetPresent = 1;

                     rtInvokeStartElement (pctxt, "featureSet", -1);

                     stat = asn1PD_H225FeatureSet (pctxt, &pvalue->featureSet);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "featureSet", -1);
                     break;

                  case 9:
                     pvalue->m.genericDataPresent = 1;

                     rtInvokeStartElement (pctxt, "genericData", -1);

                     stat = asn1PD_H225_SeqOfH225GenericData (pctxt, &pvalue->genericData);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "genericData", -1);
                     break;

                  default:
                     pctxt->buffer.byteIndex += openType.numocts;
               }
               copyContext (pctxt, &lctxt2);
            }
            else {  /* unknown element */
               pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
               pOpenType->numocts = openType.numocts;
               pOpenType->data = openType.data;
               dListAppend (pctxt, &pvalue->extElem1, pOpenType);
            }
         }
         else if (i >= 10) {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  GatekeeperConfirm                                         */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225GatekeeperConfirm (OOCTXT* pctxt, H225GatekeeperConfirm* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   OOCTXT lctxt2;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.nonStandardDataPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.gatekeeperIdentifierPresent = optbit;

   /* decode requestSeqNum */

   rtInvokeStartElement (pctxt, "requestSeqNum", -1);

   stat = asn1PD_H225RequestSeqNum (pctxt, &pvalue->requestSeqNum);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "requestSeqNum", -1);

   /* decode protocolIdentifier */

   rtInvokeStartElement (pctxt, "protocolIdentifier", -1);

   stat = asn1PD_H225ProtocolIdentifier (pctxt, &pvalue->protocolIdentifier);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "protocolIdentifier", -1);

   /* decode nonStandardData */

   if (pvalue->m.nonStandardDataPresent) {
      rtInvokeStartElement (pctxt, "nonStandardData", -1);

      stat = asn1PD_H225NonStandardParameter (pctxt, &pvalue->nonStandardData);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "nonStandardData", -1);
   }

   /* decode gatekeeperIdentifier */

   if (pvalue->m.gatekeeperIdentifierPresent) {
      rtInvokeStartElement (pctxt, "gatekeeperIdentifier", -1);

      stat = asn1PD_H225GatekeeperIdentifier (pctxt, &pvalue->gatekeeperIdentifier);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "gatekeeperIdentifier", -1);
   }

   /* decode rasAddress */

   rtInvokeStartElement (pctxt, "rasAddress", -1);

   stat = asn1PD_H225TransportAddress (pctxt, &pvalue->rasAddress);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "rasAddress", -1);

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            if (i < 9 && openType.numocts > 0) {  /* known element */
               copyContext (&lctxt2, pctxt);
               initContextBuffer (pctxt, openType.data, openType.numocts);

               switch (i) {
                  case 0:
                     pvalue->m.alternateGatekeeperPresent = 1;

                     rtInvokeStartElement (pctxt, "alternateGatekeeper", -1);

                     stat = asn1PD_H225_SeqOfH225AlternateGK (pctxt, &pvalue->alternateGatekeeper);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "alternateGatekeeper", -1);
                     break;

                  case 1:
                     pvalue->m.authenticationModePresent = 1;

                     rtInvokeStartElement (pctxt, "authenticationMode", -1);

                     stat = asn1PD_H235AuthenticationMechanism (pctxt, &pvalue->authenticationMode);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "authenticationMode", -1);
                     break;

                  case 2:
                     pvalue->m.tokensPresent = 1;

                     rtInvokeStartElement (pctxt, "tokens", -1);

                     stat = asn1PD_H225_SeqOfH225ClearToken (pctxt, &pvalue->tokens);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "tokens", -1);
                     break;

                  case 3:
                     pvalue->m.cryptoTokensPresent = 1;

                     rtInvokeStartElement (pctxt, "cryptoTokens", -1);

                     stat = asn1PD_H225_SeqOfH225CryptoH323Token (pctxt, &pvalue->cryptoTokens);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "cryptoTokens", -1);
                     break;

                  case 4:
                     pvalue->m.algorithmOIDPresent = 1;

                     rtInvokeStartElement (pctxt, "algorithmOID", -1);

                     stat = decodeObjectIdentifier (pctxt, &pvalue->algorithmOID);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
                     rtInvokeOidValue (pctxt, pvalue->algorithmOID.numids, pvalue->algorithmOID.subid);

                     rtInvokeEndElement (pctxt, "algorithmOID", -1);
                     break;

                  case 5:
                     pvalue->m.integrityPresent = 1;

                     rtInvokeStartElement (pctxt, "integrity", -1);

                     stat = asn1PD_H225_SeqOfH225IntegrityMechanism (pctxt, &pvalue->integrity);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "integrity", -1);
                     break;

                  case 6:
                     pvalue->m.integrityCheckValuePresent = 1;

                     rtInvokeStartElement (pctxt, "integrityCheckValue", -1);

                     stat = asn1PD_H225ICV (pctxt, &pvalue->integrityCheckValue);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "integrityCheckValue", -1);
                     break;

                  case 7:
                     pvalue->m.featureSetPresent = 1;

                     rtInvokeStartElement (pctxt, "featureSet", -1);

                     stat = asn1PD_H225FeatureSet (pctxt, &pvalue->featureSet);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "featureSet", -1);
                     break;

                  case 8:
                     pvalue->m.genericDataPresent = 1;

                     rtInvokeStartElement (pctxt, "genericData", -1);

                     stat = asn1PD_H225_SeqOfH225GenericData (pctxt, &pvalue->genericData);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "genericData", -1);
                     break;

                  default:
                     pctxt->buffer.byteIndex += openType.numocts;
               }
               copyContext (pctxt, &lctxt2);
            }
            else {  /* unknown element */
               pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
               pOpenType->numocts = openType.numocts;
               pOpenType->data = openType.data;
               dListAppend (pctxt, &pvalue->extElem1, pOpenType);
            }
         }
         else if (i >= 9) {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  GatekeeperRejectReason                                    */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225GatekeeperRejectReason (OOCTXT* pctxt, H225GatekeeperRejectReason* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT ui;
   ASN1OpenType openType;
   ASN1BOOL extbit;
   OOCTXT lctxt;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   if (!extbit) {
      stat = decodeConsUnsigned (pctxt, &ui, 0, 3);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      else pvalue->t = ui + 1;

      switch (ui) {
         /* resourceUnavailable */
         case 0:
            rtInvokeStartElement (pctxt, "resourceUnavailable", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "resourceUnavailable", -1);

            break;

         /* terminalExcluded */
         case 1:
            rtInvokeStartElement (pctxt, "terminalExcluded", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "terminalExcluded", -1);

            break;

         /* invalidRevision */
         case 2:
            rtInvokeStartElement (pctxt, "invalidRevision", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "invalidRevision", -1);

            break;

         /* undefinedReason */
         case 3:
            rtInvokeStartElement (pctxt, "undefinedReason", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "undefinedReason", -1);

            break;

         default:
            return LOG_ASN1ERR (pctxt, ASN_E_INVOPT);
      }
   }
   else {
      stat = decodeSmallNonNegWholeNumber (pctxt, &ui);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      else pvalue->t = ui + 5;

      stat = decodeByteAlign (pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      copyContext (&lctxt, pctxt);
      initContextBuffer (pctxt, openType.data, openType.numocts);

      switch (pvalue->t) {
         /* securityDenial */
         case 5:
            rtInvokeStartElement (pctxt, "securityDenial", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "securityDenial", -1);

            break;

         /* genericDataReason */
         case 6:
            rtInvokeStartElement (pctxt, "genericDataReason", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "genericDataReason", -1);

            break;

         /* neededFeatureNotSupported */
         case 7:
            rtInvokeStartElement (pctxt, "neededFeatureNotSupported", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "neededFeatureNotSupported", -1);

            break;

         /* securityError */
         case 8:
            rtInvokeStartElement (pctxt, "securityError", -1);

            pvalue->u.securityError = ALLOC_ASN1ELEM (pctxt, H225SecurityErrors);
            if (pvalue->u.securityError == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225SecurityErrors (pctxt, pvalue->u.securityError);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "securityError", -1);

            break;

         default:
            pvalue->u.extElem1 = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
            if (pvalue->u.extElem1 == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            pvalue->u.extElem1->data = openType.data;
            pvalue->u.extElem1->numocts = openType.numocts;

      }

      copyContext (pctxt, &lctxt);
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  GatekeeperReject                                          */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225GatekeeperReject (OOCTXT* pctxt, H225GatekeeperReject* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   OOCTXT lctxt2;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.nonStandardDataPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.gatekeeperIdentifierPresent = optbit;

   /* decode requestSeqNum */

   rtInvokeStartElement (pctxt, "requestSeqNum", -1);

   stat = asn1PD_H225RequestSeqNum (pctxt, &pvalue->requestSeqNum);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "requestSeqNum", -1);

   /* decode protocolIdentifier */

   rtInvokeStartElement (pctxt, "protocolIdentifier", -1);

   stat = asn1PD_H225ProtocolIdentifier (pctxt, &pvalue->protocolIdentifier);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "protocolIdentifier", -1);

   /* decode nonStandardData */

   if (pvalue->m.nonStandardDataPresent) {
      rtInvokeStartElement (pctxt, "nonStandardData", -1);

      stat = asn1PD_H225NonStandardParameter (pctxt, &pvalue->nonStandardData);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "nonStandardData", -1);
   }

   /* decode gatekeeperIdentifier */

   if (pvalue->m.gatekeeperIdentifierPresent) {
      rtInvokeStartElement (pctxt, "gatekeeperIdentifier", -1);

      stat = asn1PD_H225GatekeeperIdentifier (pctxt, &pvalue->gatekeeperIdentifier);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "gatekeeperIdentifier", -1);
   }

   /* decode rejectReason */

   rtInvokeStartElement (pctxt, "rejectReason", -1);

   stat = asn1PD_H225GatekeeperRejectReason (pctxt, &pvalue->rejectReason);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "rejectReason", -1);

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            if (i < 6 && openType.numocts > 0) {  /* known element */
               copyContext (&lctxt2, pctxt);
               initContextBuffer (pctxt, openType.data, openType.numocts);

               switch (i) {
                  case 0:
                     pvalue->m.altGKInfoPresent = 1;

                     rtInvokeStartElement (pctxt, "altGKInfo", -1);

                     stat = asn1PD_H225AltGKInfo (pctxt, &pvalue->altGKInfo);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "altGKInfo", -1);
                     break;

                  case 1:
                     pvalue->m.tokensPresent = 1;

                     rtInvokeStartElement (pctxt, "tokens", -1);

                     stat = asn1PD_H225_SeqOfH225ClearToken (pctxt, &pvalue->tokens);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "tokens", -1);
                     break;

                  case 2:
                     pvalue->m.cryptoTokensPresent = 1;

                     rtInvokeStartElement (pctxt, "cryptoTokens", -1);

                     stat = asn1PD_H225_SeqOfH225CryptoH323Token (pctxt, &pvalue->cryptoTokens);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "cryptoTokens", -1);
                     break;

                  case 3:
                     pvalue->m.integrityCheckValuePresent = 1;

                     rtInvokeStartElement (pctxt, "integrityCheckValue", -1);

                     stat = asn1PD_H225ICV (pctxt, &pvalue->integrityCheckValue);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "integrityCheckValue", -1);
                     break;

                  case 4:
                     pvalue->m.featureSetPresent = 1;

                     rtInvokeStartElement (pctxt, "featureSet", -1);

                     stat = asn1PD_H225FeatureSet (pctxt, &pvalue->featureSet);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "featureSet", -1);
                     break;

                  case 5:
                     pvalue->m.genericDataPresent = 1;

                     rtInvokeStartElement (pctxt, "genericData", -1);

                     stat = asn1PD_H225_SeqOfH225GenericData (pctxt, &pvalue->genericData);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "genericData", -1);
                     break;

                  default:
                     pctxt->buffer.byteIndex += openType.numocts;
               }
               copyContext (pctxt, &lctxt2);
            }
            else {  /* unknown element */
               pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
               pOpenType->numocts = openType.numocts;
               pOpenType->data = openType.data;
               dListAppend (pctxt, &pvalue->extElem1, pOpenType);
            }
         }
         else if (i >= 6) {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  _SeqOfH225AddressPattern                                  */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225_SeqOfH225AddressPattern (OOCTXT* pctxt, H225_SeqOfH225AddressPattern* pvalue)
{
   int stat = ASN_OK;
   H225AddressPattern* pdata;
   ASN1UINT count = 0;
   ASN1UINT xx1;
   int lstat;

   dListInit (pvalue);

   for (;;) {
      /* decode length determinant */

      lstat = decodeLength (pctxt, &count);
      if (lstat != ASN_OK && lstat != ASN_OK_FRAG) {
         return LOG_ASN1ERR (pctxt, lstat);
      }

      /* decode elements */

      for (xx1 = 0; xx1 < count; xx1++) {
         rtInvokeStartElement (pctxt, "elem", xx1);

         pdata = ALLOC_ASN1ELEMDNODE (pctxt, H225AddressPattern);
         if (pdata == NULL)
            return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

         stat = asn1PD_H225AddressPattern (pctxt, pdata);
         if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
         rtInvokeEndElement (pctxt, "elem", xx1);

         dListAppendNode (pctxt, pvalue, pdata);
      }

      if(lstat == ASN_OK) break;
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  _SeqOfH225H248PackagesDescriptor                          */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225_SeqOfH225H248PackagesDescriptor (OOCTXT* pctxt, H225_SeqOfH225H248PackagesDescriptor* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT xx1;

   /* decode length determinant */

   stat = decodeLength (pctxt, &pvalue->n);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* decode elements */

   ALLOC_ASN1ARRAY (pctxt, pvalue, H225H248PackagesDescriptor);

   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      rtInvokeStartElement (pctxt, "elem", xx1);

      stat = asn1PD_H225H248PackagesDescriptor (pctxt, &pvalue->elem[xx1]);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      rtInvokeEndElement (pctxt, "elem", xx1);

   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  RegistrationRequest                                       */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225RegistrationRequest (OOCTXT* pctxt, H225RegistrationRequest* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   OOCTXT lctxt2;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.nonStandardDataPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.terminalAliasPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.gatekeeperIdentifierPresent = optbit;

   /* decode requestSeqNum */

   rtInvokeStartElement (pctxt, "requestSeqNum", -1);

   stat = asn1PD_H225RequestSeqNum (pctxt, &pvalue->requestSeqNum);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "requestSeqNum", -1);

   /* decode protocolIdentifier */

   rtInvokeStartElement (pctxt, "protocolIdentifier", -1);

   stat = asn1PD_H225ProtocolIdentifier (pctxt, &pvalue->protocolIdentifier);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "protocolIdentifier", -1);

   /* decode nonStandardData */

   if (pvalue->m.nonStandardDataPresent) {
      rtInvokeStartElement (pctxt, "nonStandardData", -1);

      stat = asn1PD_H225NonStandardParameter (pctxt, &pvalue->nonStandardData);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "nonStandardData", -1);
   }

   /* decode discoveryComplete */

   rtInvokeStartElement (pctxt, "discoveryComplete", -1);

   stat = DECODEBIT (pctxt, &pvalue->discoveryComplete);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   rtInvokeBoolValue (pctxt, pvalue->discoveryComplete);

   rtInvokeEndElement (pctxt, "discoveryComplete", -1);

   /* decode callSignalAddress */

   rtInvokeStartElement (pctxt, "callSignalAddress", -1);

   stat = asn1PD_H225_SeqOfH225TransportAddress (pctxt, &pvalue->callSignalAddress);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "callSignalAddress", -1);

   /* decode rasAddress */

   rtInvokeStartElement (pctxt, "rasAddress", -1);

   stat = asn1PD_H225_SeqOfH225TransportAddress (pctxt, &pvalue->rasAddress);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "rasAddress", -1);

   /* decode terminalType */

   rtInvokeStartElement (pctxt, "terminalType", -1);

   stat = asn1PD_H225EndpointType (pctxt, &pvalue->terminalType);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "terminalType", -1);

   /* decode terminalAlias */

   if (pvalue->m.terminalAliasPresent) {
      rtInvokeStartElement (pctxt, "terminalAlias", -1);

      stat = asn1PD_H225_SeqOfH225AliasAddress (pctxt, &pvalue->terminalAlias);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "terminalAlias", -1);
   }

   /* decode gatekeeperIdentifier */

   if (pvalue->m.gatekeeperIdentifierPresent) {
      rtInvokeStartElement (pctxt, "gatekeeperIdentifier", -1);

      stat = asn1PD_H225GatekeeperIdentifier (pctxt, &pvalue->gatekeeperIdentifier);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "gatekeeperIdentifier", -1);
   }

   /* decode endpointVendor */

   rtInvokeStartElement (pctxt, "endpointVendor", -1);

   stat = asn1PD_H225VendorIdentifier (pctxt, &pvalue->endpointVendor);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "endpointVendor", -1);

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            if (i < 23 && openType.numocts > 0) {  /* known element */
               copyContext (&lctxt2, pctxt);
               initContextBuffer (pctxt, openType.data, openType.numocts);

               switch (i) {
                  case 0:
                     pvalue->m.alternateEndpointsPresent = 1;

                     rtInvokeStartElement (pctxt, "alternateEndpoints", -1);

                     stat = asn1PD_H225_SeqOfH225Endpoint (pctxt, &pvalue->alternateEndpoints);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "alternateEndpoints", -1);
                     break;

                  case 1:
                     pvalue->m.timeToLivePresent = 1;

                     rtInvokeStartElement (pctxt, "timeToLive", -1);

                     stat = asn1PD_H225TimeToLive (pctxt, &pvalue->timeToLive);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "timeToLive", -1);
                     break;

                  case 2:
                     pvalue->m.tokensPresent = 1;

                     rtInvokeStartElement (pctxt, "tokens", -1);

                     stat = asn1PD_H225_SeqOfH225ClearToken (pctxt, &pvalue->tokens);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "tokens", -1);
                     break;

                  case 3:
                     pvalue->m.cryptoTokensPresent = 1;

                     rtInvokeStartElement (pctxt, "cryptoTokens", -1);

                     stat = asn1PD_H225_SeqOfH225CryptoH323Token (pctxt, &pvalue->cryptoTokens);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "cryptoTokens", -1);
                     break;

                  case 4:
                     pvalue->m.integrityCheckValuePresent = 1;

                     rtInvokeStartElement (pctxt, "integrityCheckValue", -1);

                     stat = asn1PD_H225ICV (pctxt, &pvalue->integrityCheckValue);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "integrityCheckValue", -1);
                     break;

                  case 5:
                     pvalue->m.keepAlivePresent = 1;

                     rtInvokeStartElement (pctxt, "keepAlive", -1);

                     stat = DECODEBIT (pctxt, &pvalue->keepAlive);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
                     rtInvokeBoolValue (pctxt, pvalue->keepAlive);

                     rtInvokeEndElement (pctxt, "keepAlive", -1);
                     break;

                  case 6:
                     pvalue->m.endpointIdentifierPresent = 1;

                     rtInvokeStartElement (pctxt, "endpointIdentifier", -1);

                     stat = asn1PD_H225EndpointIdentifier (pctxt, &pvalue->endpointIdentifier);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "endpointIdentifier", -1);
                     break;

                  case 7:
                     pvalue->m.willSupplyUUIEsPresent = 1;

                     rtInvokeStartElement (pctxt, "willSupplyUUIEs", -1);

                     stat = DECODEBIT (pctxt, &pvalue->willSupplyUUIEs);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
                     rtInvokeBoolValue (pctxt, pvalue->willSupplyUUIEs);

                     rtInvokeEndElement (pctxt, "willSupplyUUIEs", -1);
                     break;

                  case 8:
                     pvalue->m.maintainConnectionPresent = 1;

                     rtInvokeStartElement (pctxt, "maintainConnection", -1);

                     stat = DECODEBIT (pctxt, &pvalue->maintainConnection);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
                     rtInvokeBoolValue (pctxt, pvalue->maintainConnection);

                     rtInvokeEndElement (pctxt, "maintainConnection", -1);
                     break;

                  case 9:
                     pvalue->m.alternateTransportAddressesPresent = 1;

                     rtInvokeStartElement (pctxt, "alternateTransportAddresses", -1);

                     stat = asn1PD_H225AlternateTransportAddresses (pctxt, &pvalue->alternateTransportAddresses);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "alternateTransportAddresses", -1);
                     break;

                  case 10:
                     pvalue->m.additiveRegistrationPresent = 1;

                     rtInvokeStartElement (pctxt, "additiveRegistration", -1);

                     /* NULL */
                     rtInvokeNullValue (pctxt);

                     rtInvokeEndElement (pctxt, "additiveRegistration", -1);
                     break;

                  case 11:
                     pvalue->m.terminalAliasPatternPresent = 1;

                     rtInvokeStartElement (pctxt, "terminalAliasPattern", -1);

                     stat = asn1PD_H225_SeqOfH225AddressPattern (pctxt, &pvalue->terminalAliasPattern);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "terminalAliasPattern", -1);
                     break;

                  case 12:
                     pvalue->m.supportsAltGKPresent = 1;

                     rtInvokeStartElement (pctxt, "supportsAltGK", -1);

                     /* NULL */
                     rtInvokeNullValue (pctxt);

                     rtInvokeEndElement (pctxt, "supportsAltGK", -1);
                     break;

                  case 13:
                     pvalue->m.usageReportingCapabilityPresent = 1;

                     rtInvokeStartElement (pctxt, "usageReportingCapability", -1);

                     stat = asn1PD_H225RasUsageInfoTypes (pctxt, &pvalue->usageReportingCapability);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "usageReportingCapability", -1);
                     break;

                  case 14:
                     pvalue->m.multipleCallsPresent = 1;

                     rtInvokeStartElement (pctxt, "multipleCalls", -1);

                     stat = DECODEBIT (pctxt, &pvalue->multipleCalls);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
                     rtInvokeBoolValue (pctxt, pvalue->multipleCalls);

                     rtInvokeEndElement (pctxt, "multipleCalls", -1);
                     break;

                  case 15:
                     pvalue->m.supportedH248PackagesPresent = 1;

                     rtInvokeStartElement (pctxt, "supportedH248Packages", -1);

                     stat = asn1PD_H225_SeqOfH225H248PackagesDescriptor (pctxt, &pvalue->supportedH248Packages);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "supportedH248Packages", -1);
                     break;

                  case 16:
                     pvalue->m.callCreditCapabilityPresent = 1;

                     rtInvokeStartElement (pctxt, "callCreditCapability", -1);

                     stat = asn1PD_H225CallCreditCapability (pctxt, &pvalue->callCreditCapability);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "callCreditCapability", -1);
                     break;

                  case 17:
                     pvalue->m.capacityReportingCapabilityPresent = 1;

                     rtInvokeStartElement (pctxt, "capacityReportingCapability", -1);

                     stat = asn1PD_H225CapacityReportingCapability (pctxt, &pvalue->capacityReportingCapability);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "capacityReportingCapability", -1);
                     break;

                  case 18:
                     pvalue->m.capacityPresent = 1;

                     rtInvokeStartElement (pctxt, "capacity", -1);

                     stat = asn1PD_H225CallCapacity (pctxt, &pvalue->capacity);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "capacity", -1);
                     break;

                  case 19:
                     pvalue->m.featureSetPresent = 1;

                     rtInvokeStartElement (pctxt, "featureSet", -1);

                     stat = asn1PD_H225FeatureSet (pctxt, &pvalue->featureSet);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "featureSet", -1);
                     break;

                  case 20:
                     pvalue->m.genericDataPresent = 1;

                     rtInvokeStartElement (pctxt, "genericData", -1);

                     stat = asn1PD_H225_SeqOfH225GenericData (pctxt, &pvalue->genericData);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "genericData", -1);
                     break;

                  case 21:
                     pvalue->m.restartPresent = 1;

                     rtInvokeStartElement (pctxt, "restart", -1);

                     /* NULL */
                     rtInvokeNullValue (pctxt);

                     rtInvokeEndElement (pctxt, "restart", -1);
                     break;

                  case 22:
                     pvalue->m.supportsACFSequencesPresent = 1;

                     rtInvokeStartElement (pctxt, "supportsACFSequences", -1);

                     /* NULL */
                     rtInvokeNullValue (pctxt);

                     rtInvokeEndElement (pctxt, "supportsACFSequences", -1);
                     break;

                  default:
                     pctxt->buffer.byteIndex += openType.numocts;
               }
               copyContext (pctxt, &lctxt2);
            }
            else {  /* unknown element */
               pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
               pOpenType->numocts = openType.numocts;
               pOpenType->data = openType.data;
               dListAppend (pctxt, &pvalue->extElem1, pOpenType);
            }
         }
         else if (i >= 23) {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  RegistrationConfirm_preGrantedARQ                         */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225RegistrationConfirm_preGrantedARQ (OOCTXT* pctxt, H225RegistrationConfirm_preGrantedARQ* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   OOCTXT lctxt2;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   /* decode makeCall */

   rtInvokeStartElement (pctxt, "makeCall", -1);

   stat = DECODEBIT (pctxt, &pvalue->makeCall);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   rtInvokeBoolValue (pctxt, pvalue->makeCall);

   rtInvokeEndElement (pctxt, "makeCall", -1);

   /* decode useGKCallSignalAddressToMakeCall */

   rtInvokeStartElement (pctxt, "useGKCallSignalAddressToMakeCall", -1);

   stat = DECODEBIT (pctxt, &pvalue->useGKCallSignalAddressToMakeCall);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   rtInvokeBoolValue (pctxt, pvalue->useGKCallSignalAddressToMakeCall);

   rtInvokeEndElement (pctxt, "useGKCallSignalAddressToMakeCall", -1);

   /* decode answerCall */

   rtInvokeStartElement (pctxt, "answerCall", -1);

   stat = DECODEBIT (pctxt, &pvalue->answerCall);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   rtInvokeBoolValue (pctxt, pvalue->answerCall);

   rtInvokeEndElement (pctxt, "answerCall", -1);

   /* decode useGKCallSignalAddressToAnswer */

   rtInvokeStartElement (pctxt, "useGKCallSignalAddressToAnswer", -1);

   stat = DECODEBIT (pctxt, &pvalue->useGKCallSignalAddressToAnswer);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   rtInvokeBoolValue (pctxt, pvalue->useGKCallSignalAddressToAnswer);

   rtInvokeEndElement (pctxt, "useGKCallSignalAddressToAnswer", -1);

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            if (i < 4 && openType.numocts > 0) {  /* known element */
               copyContext (&lctxt2, pctxt);
               initContextBuffer (pctxt, openType.data, openType.numocts);

               switch (i) {
                  case 0:
                     pvalue->m.irrFrequencyInCallPresent = 1;

                     rtInvokeStartElement (pctxt, "irrFrequencyInCall", -1);

                     stat = decodeConsUInt16 (pctxt, &pvalue->irrFrequencyInCall, 1U, 65535U);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
                     rtInvokeUIntValue (pctxt, pvalue->irrFrequencyInCall);

                     rtInvokeEndElement (pctxt, "irrFrequencyInCall", -1);
                     break;

                  case 1:
                     pvalue->m.totalBandwidthRestrictionPresent = 1;

                     rtInvokeStartElement (pctxt, "totalBandwidthRestriction", -1);

                     stat = asn1PD_H225BandWidth (pctxt, &pvalue->totalBandwidthRestriction);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "totalBandwidthRestriction", -1);
                     break;

                  case 2:
                     pvalue->m.alternateTransportAddressesPresent = 1;

                     rtInvokeStartElement (pctxt, "alternateTransportAddresses", -1);

                     stat = asn1PD_H225AlternateTransportAddresses (pctxt, &pvalue->alternateTransportAddresses);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "alternateTransportAddresses", -1);
                     break;

                  case 3:
                     pvalue->m.useSpecifiedTransportPresent = 1;

                     rtInvokeStartElement (pctxt, "useSpecifiedTransport", -1);

                     stat = asn1PD_H225UseSpecifiedTransport (pctxt, &pvalue->useSpecifiedTransport);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "useSpecifiedTransport", -1);
                     break;

                  default:
                     pctxt->buffer.byteIndex += openType.numocts;
               }
               copyContext (pctxt, &lctxt2);
            }
            else {  /* unknown element */
               pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
               pOpenType->numocts = openType.numocts;
               pOpenType->data = openType.data;
               dListAppend (pctxt, &pvalue->extElem1, pOpenType);
            }
         }
         else if (i >= 4) {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  _SeqOfH225RasUsageSpecification                           */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225_SeqOfH225RasUsageSpecification (OOCTXT* pctxt, H225_SeqOfH225RasUsageSpecification* pvalue)
{
   int stat = ASN_OK;
   H225RasUsageSpecification* pdata;
   ASN1UINT count = 0;
   ASN1UINT xx1;
   int lstat;

   dListInit (pvalue);

   for (;;) {
      /* decode length determinant */

      lstat = decodeLength (pctxt, &count);
      if (lstat != ASN_OK && lstat != ASN_OK_FRAG) {
         return LOG_ASN1ERR (pctxt, lstat);
      }

      /* decode elements */

      for (xx1 = 0; xx1 < count; xx1++) {
         rtInvokeStartElement (pctxt, "elem", xx1);

         pdata = ALLOC_ASN1ELEMDNODE (pctxt, H225RasUsageSpecification);
         if (pdata == NULL)
            return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

         stat = asn1PD_H225RasUsageSpecification (pctxt, pdata);
         if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
         rtInvokeEndElement (pctxt, "elem", xx1);

         dListAppendNode (pctxt, pvalue, pdata);
      }

      if(lstat == ASN_OK) break;
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  RegistrationConfirm                                       */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225RegistrationConfirm (OOCTXT* pctxt, H225RegistrationConfirm* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   OOCTXT lctxt2;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.nonStandardDataPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.terminalAliasPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.gatekeeperIdentifierPresent = optbit;

   /* decode requestSeqNum */

   rtInvokeStartElement (pctxt, "requestSeqNum", -1);

   stat = asn1PD_H225RequestSeqNum (pctxt, &pvalue->requestSeqNum);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "requestSeqNum", -1);

   /* decode protocolIdentifier */

   rtInvokeStartElement (pctxt, "protocolIdentifier", -1);

   stat = asn1PD_H225ProtocolIdentifier (pctxt, &pvalue->protocolIdentifier);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "protocolIdentifier", -1);

   /* decode nonStandardData */

   if (pvalue->m.nonStandardDataPresent) {
      rtInvokeStartElement (pctxt, "nonStandardData", -1);

      stat = asn1PD_H225NonStandardParameter (pctxt, &pvalue->nonStandardData);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "nonStandardData", -1);
   }

   /* decode callSignalAddress */

   rtInvokeStartElement (pctxt, "callSignalAddress", -1);

   stat = asn1PD_H225_SeqOfH225TransportAddress (pctxt, &pvalue->callSignalAddress);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "callSignalAddress", -1);

   /* decode terminalAlias */

   if (pvalue->m.terminalAliasPresent) {
      rtInvokeStartElement (pctxt, "terminalAlias", -1);

      stat = asn1PD_H225_SeqOfH225AliasAddress (pctxt, &pvalue->terminalAlias);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "terminalAlias", -1);
   }

   /* decode gatekeeperIdentifier */

   if (pvalue->m.gatekeeperIdentifierPresent) {
      rtInvokeStartElement (pctxt, "gatekeeperIdentifier", -1);

      stat = asn1PD_H225GatekeeperIdentifier (pctxt, &pvalue->gatekeeperIdentifier);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "gatekeeperIdentifier", -1);
   }

   /* decode endpointIdentifier */

   rtInvokeStartElement (pctxt, "endpointIdentifier", -1);

   stat = asn1PD_H225EndpointIdentifier (pctxt, &pvalue->endpointIdentifier);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "endpointIdentifier", -1);

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            if (i < 17 && openType.numocts > 0) {  /* known element */
               copyContext (&lctxt2, pctxt);
               initContextBuffer (pctxt, openType.data, openType.numocts);

               switch (i) {
                  case 0:
                     pvalue->m.alternateGatekeeperPresent = 1;

                     rtInvokeStartElement (pctxt, "alternateGatekeeper", -1);

                     stat = asn1PD_H225_SeqOfH225AlternateGK (pctxt, &pvalue->alternateGatekeeper);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "alternateGatekeeper", -1);
                     break;

                  case 1:
                     pvalue->m.timeToLivePresent = 1;

                     rtInvokeStartElement (pctxt, "timeToLive", -1);

                     stat = asn1PD_H225TimeToLive (pctxt, &pvalue->timeToLive);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "timeToLive", -1);
                     break;

                  case 2:
                     pvalue->m.tokensPresent = 1;

                     rtInvokeStartElement (pctxt, "tokens", -1);

                     stat = asn1PD_H225_SeqOfH225ClearToken (pctxt, &pvalue->tokens);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "tokens", -1);
                     break;

                  case 3:
                     pvalue->m.cryptoTokensPresent = 1;

                     rtInvokeStartElement (pctxt, "cryptoTokens", -1);

                     stat = asn1PD_H225_SeqOfH225CryptoH323Token (pctxt, &pvalue->cryptoTokens);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "cryptoTokens", -1);
                     break;

                  case 4:
                     pvalue->m.integrityCheckValuePresent = 1;

                     rtInvokeStartElement (pctxt, "integrityCheckValue", -1);

                     stat = asn1PD_H225ICV (pctxt, &pvalue->integrityCheckValue);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "integrityCheckValue", -1);
                     break;

                  case 5:
                     pvalue->m.willRespondToIRRPresent = 1;

                     rtInvokeStartElement (pctxt, "willRespondToIRR", -1);

                     stat = DECODEBIT (pctxt, &pvalue->willRespondToIRR);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
                     rtInvokeBoolValue (pctxt, pvalue->willRespondToIRR);

                     rtInvokeEndElement (pctxt, "willRespondToIRR", -1);
                     break;

                  case 6:
                     pvalue->m.preGrantedARQPresent = 1;

                     rtInvokeStartElement (pctxt, "preGrantedARQ", -1);

                     stat = asn1PD_H225RegistrationConfirm_preGrantedARQ (pctxt, &pvalue->preGrantedARQ);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "preGrantedARQ", -1);
                     break;

                  case 7:
                     pvalue->m.maintainConnectionPresent = 1;

                     rtInvokeStartElement (pctxt, "maintainConnection", -1);

                     stat = DECODEBIT (pctxt, &pvalue->maintainConnection);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
                     rtInvokeBoolValue (pctxt, pvalue->maintainConnection);

                     rtInvokeEndElement (pctxt, "maintainConnection", -1);
                     break;

                  case 8:
                     pvalue->m.serviceControlPresent = 1;

                     rtInvokeStartElement (pctxt, "serviceControl", -1);

                     stat = asn1PD_H225_SeqOfH225ServiceControlSession (pctxt, &pvalue->serviceControl);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "serviceControl", -1);
                     break;

                  case 9:
                     pvalue->m.supportsAdditiveRegistrationPresent = 1;

                     rtInvokeStartElement (pctxt, "supportsAdditiveRegistration", -1);

                     /* NULL */
                     rtInvokeNullValue (pctxt);

                     rtInvokeEndElement (pctxt, "supportsAdditiveRegistration", -1);
                     break;

                  case 10:
                     pvalue->m.terminalAliasPatternPresent = 1;

                     rtInvokeStartElement (pctxt, "terminalAliasPattern", -1);

                     stat = asn1PD_H225_SeqOfH225AddressPattern (pctxt, &pvalue->terminalAliasPattern);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "terminalAliasPattern", -1);
                     break;

                  case 11:
                     pvalue->m.supportedPrefixesPresent = 1;

                     rtInvokeStartElement (pctxt, "supportedPrefixes", -1);

                     stat = asn1PD_H225_SeqOfH225SupportedPrefix (pctxt, &pvalue->supportedPrefixes);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "supportedPrefixes", -1);
                     break;

                  case 12:
                     pvalue->m.usageSpecPresent = 1;

                     rtInvokeStartElement (pctxt, "usageSpec", -1);

                     stat = asn1PD_H225_SeqOfH225RasUsageSpecification (pctxt, &pvalue->usageSpec);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "usageSpec", -1);
                     break;

                  case 13:
                     pvalue->m.featureServerAliasPresent = 1;

                     rtInvokeStartElement (pctxt, "featureServerAlias", -1);

                     stat = asn1PD_H225AliasAddress (pctxt, &pvalue->featureServerAlias);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "featureServerAlias", -1);
                     break;

                  case 14:
                     pvalue->m.capacityReportingSpecPresent = 1;

                     rtInvokeStartElement (pctxt, "capacityReportingSpec", -1);

                     stat = asn1PD_H225CapacityReportingSpecification (pctxt, &pvalue->capacityReportingSpec);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "capacityReportingSpec", -1);
                     break;

                  case 15:
                     pvalue->m.featureSetPresent = 1;

                     rtInvokeStartElement (pctxt, "featureSet", -1);

                     stat = asn1PD_H225FeatureSet (pctxt, &pvalue->featureSet);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "featureSet", -1);
                     break;

                  case 16:
                     pvalue->m.genericDataPresent = 1;

                     rtInvokeStartElement (pctxt, "genericData", -1);

                     stat = asn1PD_H225_SeqOfH225GenericData (pctxt, &pvalue->genericData);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "genericData", -1);
                     break;

                  default:
                     pctxt->buffer.byteIndex += openType.numocts;
               }
               copyContext (pctxt, &lctxt2);
            }
            else {  /* unknown element */
               pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
               pOpenType->numocts = openType.numocts;
               pOpenType->data = openType.data;
               dListAppend (pctxt, &pvalue->extElem1, pOpenType);
            }
         }
         else if (i >= 17) {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  RegistrationRejectReason_invalidTerminalAliases           */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225RegistrationRejectReason_invalidTerminalAliases (OOCTXT* pctxt, H225RegistrationRejectReason_invalidTerminalAliases* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.terminalAliasPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.terminalAliasPatternPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.supportedPrefixesPresent = optbit;

   /* decode terminalAlias */

   if (pvalue->m.terminalAliasPresent) {
      rtInvokeStartElement (pctxt, "terminalAlias", -1);

      stat = asn1PD_H225_SeqOfH225AliasAddress (pctxt, &pvalue->terminalAlias);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "terminalAlias", -1);
   }

   /* decode terminalAliasPattern */

   if (pvalue->m.terminalAliasPatternPresent) {
      rtInvokeStartElement (pctxt, "terminalAliasPattern", -1);

      stat = asn1PD_H225_SeqOfH225AddressPattern (pctxt, &pvalue->terminalAliasPattern);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "terminalAliasPattern", -1);
   }

   /* decode supportedPrefixes */

   if (pvalue->m.supportedPrefixesPresent) {
      rtInvokeStartElement (pctxt, "supportedPrefixes", -1);

      stat = asn1PD_H225_SeqOfH225SupportedPrefix (pctxt, &pvalue->supportedPrefixes);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "supportedPrefixes", -1);
   }

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
            pOpenType->numocts = openType.numocts;
            pOpenType->data = openType.data;
            dListAppend (pctxt, &pvalue->extElem1, pOpenType);
         }
         else {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  RegistrationRejectReason                                  */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225RegistrationRejectReason (OOCTXT* pctxt, H225RegistrationRejectReason* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT ui;
   ASN1OpenType openType;
   ASN1BOOL extbit;
   OOCTXT lctxt;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   if (!extbit) {
      stat = decodeConsUnsigned (pctxt, &ui, 0, 7);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      else pvalue->t = ui + 1;

      switch (ui) {
         /* discoveryRequired */
         case 0:
            rtInvokeStartElement (pctxt, "discoveryRequired", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "discoveryRequired", -1);

            break;

         /* invalidRevision */
         case 1:
            rtInvokeStartElement (pctxt, "invalidRevision", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "invalidRevision", -1);

            break;

         /* invalidCallSignalAddress */
         case 2:
            rtInvokeStartElement (pctxt, "invalidCallSignalAddress", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "invalidCallSignalAddress", -1);

            break;

         /* invalidRASAddress */
         case 3:
            rtInvokeStartElement (pctxt, "invalidRASAddress", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "invalidRASAddress", -1);

            break;

         /* duplicateAlias */
         case 4:
            rtInvokeStartElement (pctxt, "duplicateAlias", -1);

            pvalue->u.duplicateAlias = ALLOC_ASN1ELEM (pctxt, H225_SeqOfH225AliasAddress);
            if (pvalue->u.duplicateAlias == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225_SeqOfH225AliasAddress (pctxt, pvalue->u.duplicateAlias);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "duplicateAlias", -1);

            break;

         /* invalidTerminalType */
         case 5:
            rtInvokeStartElement (pctxt, "invalidTerminalType", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "invalidTerminalType", -1);

            break;

         /* undefinedReason */
         case 6:
            rtInvokeStartElement (pctxt, "undefinedReason", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "undefinedReason", -1);

            break;

         /* transportNotSupported */
         case 7:
            rtInvokeStartElement (pctxt, "transportNotSupported", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "transportNotSupported", -1);

            break;

         default:
            return LOG_ASN1ERR (pctxt, ASN_E_INVOPT);
      }
   }
   else {
      stat = decodeSmallNonNegWholeNumber (pctxt, &ui);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      else pvalue->t = ui + 9;

      stat = decodeByteAlign (pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      copyContext (&lctxt, pctxt);
      initContextBuffer (pctxt, openType.data, openType.numocts);

      switch (pvalue->t) {
         /* transportQOSNotSupported */
         case 9:
            rtInvokeStartElement (pctxt, "transportQOSNotSupported", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "transportQOSNotSupported", -1);

            break;

         /* resourceUnavailable */
         case 10:
            rtInvokeStartElement (pctxt, "resourceUnavailable", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "resourceUnavailable", -1);

            break;

         /* invalidAlias */
         case 11:
            rtInvokeStartElement (pctxt, "invalidAlias", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "invalidAlias", -1);

            break;

         /* securityDenial */
         case 12:
            rtInvokeStartElement (pctxt, "securityDenial", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "securityDenial", -1);

            break;

         /* fullRegistrationRequired */
         case 13:
            rtInvokeStartElement (pctxt, "fullRegistrationRequired", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "fullRegistrationRequired", -1);

            break;

         /* additiveRegistrationNotSupported */
         case 14:
            rtInvokeStartElement (pctxt, "additiveRegistrationNotSupported", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "additiveRegistrationNotSupported", -1);

            break;

         /* invalidTerminalAliases */
         case 15:
            rtInvokeStartElement (pctxt, "invalidTerminalAliases", -1);

            pvalue->u.invalidTerminalAliases = ALLOC_ASN1ELEM (pctxt, H225RegistrationRejectReason_invalidTerminalAliases);
            if (pvalue->u.invalidTerminalAliases == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225RegistrationRejectReason_invalidTerminalAliases (pctxt, pvalue->u.invalidTerminalAliases);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "invalidTerminalAliases", -1);

            break;

         /* genericDataReason */
         case 16:
            rtInvokeStartElement (pctxt, "genericDataReason", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "genericDataReason", -1);

            break;

         /* neededFeatureNotSupported */
         case 17:
            rtInvokeStartElement (pctxt, "neededFeatureNotSupported", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "neededFeatureNotSupported", -1);

            break;

         /* securityError */
         case 18:
            rtInvokeStartElement (pctxt, "securityError", -1);

            pvalue->u.securityError = ALLOC_ASN1ELEM (pctxt, H225SecurityErrors);
            if (pvalue->u.securityError == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225SecurityErrors (pctxt, pvalue->u.securityError);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "securityError", -1);

            break;

         default:
            pvalue->u.extElem1 = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
            if (pvalue->u.extElem1 == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            pvalue->u.extElem1->data = openType.data;
            pvalue->u.extElem1->numocts = openType.numocts;

      }

      copyContext (pctxt, &lctxt);
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  RegistrationReject                                        */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225RegistrationReject (OOCTXT* pctxt, H225RegistrationReject* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   OOCTXT lctxt2;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.nonStandardDataPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.gatekeeperIdentifierPresent = optbit;

   /* decode requestSeqNum */

   rtInvokeStartElement (pctxt, "requestSeqNum", -1);

   stat = asn1PD_H225RequestSeqNum (pctxt, &pvalue->requestSeqNum);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "requestSeqNum", -1);

   /* decode protocolIdentifier */

   rtInvokeStartElement (pctxt, "protocolIdentifier", -1);

   stat = asn1PD_H225ProtocolIdentifier (pctxt, &pvalue->protocolIdentifier);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "protocolIdentifier", -1);

   /* decode nonStandardData */

   if (pvalue->m.nonStandardDataPresent) {
      rtInvokeStartElement (pctxt, "nonStandardData", -1);

      stat = asn1PD_H225NonStandardParameter (pctxt, &pvalue->nonStandardData);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "nonStandardData", -1);
   }

   /* decode rejectReason */

   rtInvokeStartElement (pctxt, "rejectReason", -1);

   stat = asn1PD_H225RegistrationRejectReason (pctxt, &pvalue->rejectReason);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "rejectReason", -1);

   /* decode gatekeeperIdentifier */

   if (pvalue->m.gatekeeperIdentifierPresent) {
      rtInvokeStartElement (pctxt, "gatekeeperIdentifier", -1);

      stat = asn1PD_H225GatekeeperIdentifier (pctxt, &pvalue->gatekeeperIdentifier);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "gatekeeperIdentifier", -1);
   }

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            if (i < 6 && openType.numocts > 0) {  /* known element */
               copyContext (&lctxt2, pctxt);
               initContextBuffer (pctxt, openType.data, openType.numocts);

               switch (i) {
                  case 0:
                     pvalue->m.altGKInfoPresent = 1;

                     rtInvokeStartElement (pctxt, "altGKInfo", -1);

                     stat = asn1PD_H225AltGKInfo (pctxt, &pvalue->altGKInfo);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "altGKInfo", -1);
                     break;

                  case 1:
                     pvalue->m.tokensPresent = 1;

                     rtInvokeStartElement (pctxt, "tokens", -1);

                     stat = asn1PD_H225_SeqOfH225ClearToken (pctxt, &pvalue->tokens);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "tokens", -1);
                     break;

                  case 2:
                     pvalue->m.cryptoTokensPresent = 1;

                     rtInvokeStartElement (pctxt, "cryptoTokens", -1);

                     stat = asn1PD_H225_SeqOfH225CryptoH323Token (pctxt, &pvalue->cryptoTokens);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "cryptoTokens", -1);
                     break;

                  case 3:
                     pvalue->m.integrityCheckValuePresent = 1;

                     rtInvokeStartElement (pctxt, "integrityCheckValue", -1);

                     stat = asn1PD_H225ICV (pctxt, &pvalue->integrityCheckValue);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "integrityCheckValue", -1);
                     break;

                  case 4:
                     pvalue->m.featureSetPresent = 1;

                     rtInvokeStartElement (pctxt, "featureSet", -1);

                     stat = asn1PD_H225FeatureSet (pctxt, &pvalue->featureSet);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "featureSet", -1);
                     break;

                  case 5:
                     pvalue->m.genericDataPresent = 1;

                     rtInvokeStartElement (pctxt, "genericData", -1);

                     stat = asn1PD_H225_SeqOfH225GenericData (pctxt, &pvalue->genericData);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "genericData", -1);
                     break;

                  default:
                     pctxt->buffer.byteIndex += openType.numocts;
               }
               copyContext (pctxt, &lctxt2);
            }
            else {  /* unknown element */
               pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
               pOpenType->numocts = openType.numocts;
               pOpenType->data = openType.data;
               dListAppend (pctxt, &pvalue->extElem1, pOpenType);
            }
         }
         else if (i >= 6) {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  UnregRequestReason                                        */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225UnregRequestReason (OOCTXT* pctxt, H225UnregRequestReason* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT ui;
   ASN1OpenType openType;
   ASN1BOOL extbit;
   OOCTXT lctxt;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   if (!extbit) {
      stat = decodeConsUnsigned (pctxt, &ui, 0, 3);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      else pvalue->t = ui + 1;

      switch (ui) {
         /* reregistrationRequired */
         case 0:
            rtInvokeStartElement (pctxt, "reregistrationRequired", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "reregistrationRequired", -1);

            break;

         /* ttlExpired */
         case 1:
            rtInvokeStartElement (pctxt, "ttlExpired", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "ttlExpired", -1);

            break;

         /* securityDenial */
         case 2:
            rtInvokeStartElement (pctxt, "securityDenial", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "securityDenial", -1);

            break;

         /* undefinedReason */
         case 3:
            rtInvokeStartElement (pctxt, "undefinedReason", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "undefinedReason", -1);

            break;

         default:
            return LOG_ASN1ERR (pctxt, ASN_E_INVOPT);
      }
   }
   else {
      stat = decodeSmallNonNegWholeNumber (pctxt, &ui);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      else pvalue->t = ui + 5;

      stat = decodeByteAlign (pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      copyContext (&lctxt, pctxt);
      initContextBuffer (pctxt, openType.data, openType.numocts);

      switch (pvalue->t) {
         /* maintenance */
         case 5:
            rtInvokeStartElement (pctxt, "maintenance", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "maintenance", -1);

            break;

         /* securityError */
         case 6:
            rtInvokeStartElement (pctxt, "securityError", -1);

            pvalue->u.securityError = ALLOC_ASN1ELEM (pctxt, H225SecurityErrors2);
            if (pvalue->u.securityError == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225SecurityErrors2 (pctxt, pvalue->u.securityError);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "securityError", -1);

            break;

         default:
            pvalue->u.extElem1 = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
            if (pvalue->u.extElem1 == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            pvalue->u.extElem1->data = openType.data;
            pvalue->u.extElem1->numocts = openType.numocts;

      }

      copyContext (pctxt, &lctxt);
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  UnregistrationRequest                                     */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225UnregistrationRequest (OOCTXT* pctxt, H225UnregistrationRequest* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   OOCTXT lctxt2;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.endpointAliasPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.nonStandardDataPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.endpointIdentifierPresent = optbit;

   /* decode requestSeqNum */

   rtInvokeStartElement (pctxt, "requestSeqNum", -1);

   stat = asn1PD_H225RequestSeqNum (pctxt, &pvalue->requestSeqNum);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "requestSeqNum", -1);

   /* decode callSignalAddress */

   rtInvokeStartElement (pctxt, "callSignalAddress", -1);

   stat = asn1PD_H225_SeqOfH225TransportAddress (pctxt, &pvalue->callSignalAddress);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "callSignalAddress", -1);

   /* decode endpointAlias */

   if (pvalue->m.endpointAliasPresent) {
      rtInvokeStartElement (pctxt, "endpointAlias", -1);

      stat = asn1PD_H225_SeqOfH225AliasAddress (pctxt, &pvalue->endpointAlias);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "endpointAlias", -1);
   }

   /* decode nonStandardData */

   if (pvalue->m.nonStandardDataPresent) {
      rtInvokeStartElement (pctxt, "nonStandardData", -1);

      stat = asn1PD_H225NonStandardParameter (pctxt, &pvalue->nonStandardData);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "nonStandardData", -1);
   }

   /* decode endpointIdentifier */

   if (pvalue->m.endpointIdentifierPresent) {
      rtInvokeStartElement (pctxt, "endpointIdentifier", -1);

      stat = asn1PD_H225EndpointIdentifier (pctxt, &pvalue->endpointIdentifier);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "endpointIdentifier", -1);
   }

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            if (i < 10 && openType.numocts > 0) {  /* known element */
               copyContext (&lctxt2, pctxt);
               initContextBuffer (pctxt, openType.data, openType.numocts);

               switch (i) {
                  case 0:
                     pvalue->m.alternateEndpointsPresent = 1;

                     rtInvokeStartElement (pctxt, "alternateEndpoints", -1);

                     stat = asn1PD_H225_SeqOfH225Endpoint (pctxt, &pvalue->alternateEndpoints);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "alternateEndpoints", -1);
                     break;

                  case 1:
                     pvalue->m.gatekeeperIdentifierPresent = 1;

                     rtInvokeStartElement (pctxt, "gatekeeperIdentifier", -1);

                     stat = asn1PD_H225GatekeeperIdentifier (pctxt, &pvalue->gatekeeperIdentifier);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "gatekeeperIdentifier", -1);
                     break;

                  case 2:
                     pvalue->m.tokensPresent = 1;

                     rtInvokeStartElement (pctxt, "tokens", -1);

                     stat = asn1PD_H225_SeqOfH225ClearToken (pctxt, &pvalue->tokens);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "tokens", -1);
                     break;

                  case 3:
                     pvalue->m.cryptoTokensPresent = 1;

                     rtInvokeStartElement (pctxt, "cryptoTokens", -1);

                     stat = asn1PD_H225_SeqOfH225CryptoH323Token (pctxt, &pvalue->cryptoTokens);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "cryptoTokens", -1);
                     break;

                  case 4:
                     pvalue->m.integrityCheckValuePresent = 1;

                     rtInvokeStartElement (pctxt, "integrityCheckValue", -1);

                     stat = asn1PD_H225ICV (pctxt, &pvalue->integrityCheckValue);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "integrityCheckValue", -1);
                     break;

                  case 5:
                     pvalue->m.reasonPresent = 1;

                     rtInvokeStartElement (pctxt, "reason", -1);

                     stat = asn1PD_H225UnregRequestReason (pctxt, &pvalue->reason);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "reason", -1);
                     break;

                  case 6:
                     pvalue->m.endpointAliasPatternPresent = 1;

                     rtInvokeStartElement (pctxt, "endpointAliasPattern", -1);

                     stat = asn1PD_H225_SeqOfH225AddressPattern (pctxt, &pvalue->endpointAliasPattern);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "endpointAliasPattern", -1);
                     break;

                  case 7:
                     pvalue->m.supportedPrefixesPresent = 1;

                     rtInvokeStartElement (pctxt, "supportedPrefixes", -1);

                     stat = asn1PD_H225_SeqOfH225SupportedPrefix (pctxt, &pvalue->supportedPrefixes);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "supportedPrefixes", -1);
                     break;

                  case 8:
                     pvalue->m.alternateGatekeeperPresent = 1;

                     rtInvokeStartElement (pctxt, "alternateGatekeeper", -1);

                     stat = asn1PD_H225_SeqOfH225AlternateGK (pctxt, &pvalue->alternateGatekeeper);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "alternateGatekeeper", -1);
                     break;

                  case 9:
                     pvalue->m.genericDataPresent = 1;

                     rtInvokeStartElement (pctxt, "genericData", -1);

                     stat = asn1PD_H225_SeqOfH225GenericData (pctxt, &pvalue->genericData);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "genericData", -1);
                     break;

                  default:
                     pctxt->buffer.byteIndex += openType.numocts;
               }
               copyContext (pctxt, &lctxt2);
            }
            else {  /* unknown element */
               pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
               pOpenType->numocts = openType.numocts;
               pOpenType->data = openType.data;
               dListAppend (pctxt, &pvalue->extElem1, pOpenType);
            }
         }
         else if (i >= 10) {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  UnregistrationConfirm                                     */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225UnregistrationConfirm (OOCTXT* pctxt, H225UnregistrationConfirm* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   OOCTXT lctxt2;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.nonStandardDataPresent = optbit;

   /* decode requestSeqNum */

   rtInvokeStartElement (pctxt, "requestSeqNum", -1);

   stat = asn1PD_H225RequestSeqNum (pctxt, &pvalue->requestSeqNum);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "requestSeqNum", -1);

   /* decode nonStandardData */

   if (pvalue->m.nonStandardDataPresent) {
      rtInvokeStartElement (pctxt, "nonStandardData", -1);

      stat = asn1PD_H225NonStandardParameter (pctxt, &pvalue->nonStandardData);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "nonStandardData", -1);
   }

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            if (i < 4 && openType.numocts > 0) {  /* known element */
               copyContext (&lctxt2, pctxt);
               initContextBuffer (pctxt, openType.data, openType.numocts);

               switch (i) {
                  case 0:
                     pvalue->m.tokensPresent = 1;

                     rtInvokeStartElement (pctxt, "tokens", -1);

                     stat = asn1PD_H225_SeqOfH225ClearToken (pctxt, &pvalue->tokens);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "tokens", -1);
                     break;

                  case 1:
                     pvalue->m.cryptoTokensPresent = 1;

                     rtInvokeStartElement (pctxt, "cryptoTokens", -1);

                     stat = asn1PD_H225_SeqOfH225CryptoH323Token (pctxt, &pvalue->cryptoTokens);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "cryptoTokens", -1);
                     break;

                  case 2:
                     pvalue->m.integrityCheckValuePresent = 1;

                     rtInvokeStartElement (pctxt, "integrityCheckValue", -1);

                     stat = asn1PD_H225ICV (pctxt, &pvalue->integrityCheckValue);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "integrityCheckValue", -1);
                     break;

                  case 3:
                     pvalue->m.genericDataPresent = 1;

                     rtInvokeStartElement (pctxt, "genericData", -1);

                     stat = asn1PD_H225_SeqOfH225GenericData (pctxt, &pvalue->genericData);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "genericData", -1);
                     break;

                  default:
                     pctxt->buffer.byteIndex += openType.numocts;
               }
               copyContext (pctxt, &lctxt2);
            }
            else {  /* unknown element */
               pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
               pOpenType->numocts = openType.numocts;
               pOpenType->data = openType.data;
               dListAppend (pctxt, &pvalue->extElem1, pOpenType);
            }
         }
         else if (i >= 4) {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  UnregRejectReason                                         */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225UnregRejectReason (OOCTXT* pctxt, H225UnregRejectReason* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT ui;
   ASN1OpenType openType;
   ASN1BOOL extbit;
   OOCTXT lctxt;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   if (!extbit) {
      stat = decodeConsUnsigned (pctxt, &ui, 0, 2);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      else pvalue->t = ui + 1;

      switch (ui) {
         /* notCurrentlyRegistered */
         case 0:
            rtInvokeStartElement (pctxt, "notCurrentlyRegistered", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "notCurrentlyRegistered", -1);

            break;

         /* callInProgress */
         case 1:
            rtInvokeStartElement (pctxt, "callInProgress", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "callInProgress", -1);

            break;

         /* undefinedReason */
         case 2:
            rtInvokeStartElement (pctxt, "undefinedReason", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "undefinedReason", -1);

            break;

         default:
            return LOG_ASN1ERR (pctxt, ASN_E_INVOPT);
      }
   }
   else {
      stat = decodeSmallNonNegWholeNumber (pctxt, &ui);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      else pvalue->t = ui + 4;

      stat = decodeByteAlign (pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      copyContext (&lctxt, pctxt);
      initContextBuffer (pctxt, openType.data, openType.numocts);

      switch (pvalue->t) {
         /* permissionDenied */
         case 4:
            rtInvokeStartElement (pctxt, "permissionDenied", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "permissionDenied", -1);

            break;

         /* securityDenial */
         case 5:
            rtInvokeStartElement (pctxt, "securityDenial", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "securityDenial", -1);

            break;

         /* securityError */
         case 6:
            rtInvokeStartElement (pctxt, "securityError", -1);

            pvalue->u.securityError = ALLOC_ASN1ELEM (pctxt, H225SecurityErrors2);
            if (pvalue->u.securityError == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225SecurityErrors2 (pctxt, pvalue->u.securityError);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "securityError", -1);

            break;

         default:
            pvalue->u.extElem1 = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
            if (pvalue->u.extElem1 == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            pvalue->u.extElem1->data = openType.data;
            pvalue->u.extElem1->numocts = openType.numocts;

      }

      copyContext (pctxt, &lctxt);
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  UnregistrationReject                                      */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225UnregistrationReject (OOCTXT* pctxt, H225UnregistrationReject* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   OOCTXT lctxt2;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.nonStandardDataPresent = optbit;

   /* decode requestSeqNum */

   rtInvokeStartElement (pctxt, "requestSeqNum", -1);

   stat = asn1PD_H225RequestSeqNum (pctxt, &pvalue->requestSeqNum);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "requestSeqNum", -1);

   /* decode rejectReason */

   rtInvokeStartElement (pctxt, "rejectReason", -1);

   stat = asn1PD_H225UnregRejectReason (pctxt, &pvalue->rejectReason);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "rejectReason", -1);

   /* decode nonStandardData */

   if (pvalue->m.nonStandardDataPresent) {
      rtInvokeStartElement (pctxt, "nonStandardData", -1);

      stat = asn1PD_H225NonStandardParameter (pctxt, &pvalue->nonStandardData);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "nonStandardData", -1);
   }

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            if (i < 5 && openType.numocts > 0) {  /* known element */
               copyContext (&lctxt2, pctxt);
               initContextBuffer (pctxt, openType.data, openType.numocts);

               switch (i) {
                  case 0:
                     pvalue->m.altGKInfoPresent = 1;

                     rtInvokeStartElement (pctxt, "altGKInfo", -1);

                     stat = asn1PD_H225AltGKInfo (pctxt, &pvalue->altGKInfo);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "altGKInfo", -1);
                     break;

                  case 1:
                     pvalue->m.tokensPresent = 1;

                     rtInvokeStartElement (pctxt, "tokens", -1);

                     stat = asn1PD_H225_SeqOfH225ClearToken (pctxt, &pvalue->tokens);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "tokens", -1);
                     break;

                  case 2:
                     pvalue->m.cryptoTokensPresent = 1;

                     rtInvokeStartElement (pctxt, "cryptoTokens", -1);

                     stat = asn1PD_H225_SeqOfH225CryptoH323Token (pctxt, &pvalue->cryptoTokens);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "cryptoTokens", -1);
                     break;

                  case 3:
                     pvalue->m.integrityCheckValuePresent = 1;

                     rtInvokeStartElement (pctxt, "integrityCheckValue", -1);

                     stat = asn1PD_H225ICV (pctxt, &pvalue->integrityCheckValue);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "integrityCheckValue", -1);
                     break;

                  case 4:
                     pvalue->m.genericDataPresent = 1;

                     rtInvokeStartElement (pctxt, "genericData", -1);

                     stat = asn1PD_H225_SeqOfH225GenericData (pctxt, &pvalue->genericData);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "genericData", -1);
                     break;

                  default:
                     pctxt->buffer.byteIndex += openType.numocts;
               }
               copyContext (pctxt, &lctxt2);
            }
            else {  /* unknown element */
               pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
               pOpenType->numocts = openType.numocts;
               pOpenType->data = openType.data;
               dListAppend (pctxt, &pvalue->extElem1, pOpenType);
            }
         }
         else if (i >= 5) {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  CallModel                                                 */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225CallModel (OOCTXT* pctxt, H225CallModel* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT ui;
   ASN1OpenType openType;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   if (!extbit) {
      stat = decodeConsUnsigned (pctxt, &ui, 0, 1);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      else pvalue->t = ui + 1;

      switch (ui) {
         /* direct */
         case 0:
            rtInvokeStartElement (pctxt, "direct", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "direct", -1);

            break;

         /* gatekeeperRouted */
         case 1:
            rtInvokeStartElement (pctxt, "gatekeeperRouted", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "gatekeeperRouted", -1);

            break;

         default:
            return LOG_ASN1ERR (pctxt, ASN_E_INVOPT);
      }
   }
   else {
      stat = decodeSmallNonNegWholeNumber (pctxt, &ui);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      else pvalue->t = ui + 3;

      stat = decodeByteAlign (pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      pvalue->u.extElem1 = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
      if (pvalue->u.extElem1 == NULL)
         return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

      pvalue->u.extElem1->data = openType.data;
      pvalue->u.extElem1->numocts = openType.numocts;

   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  TransportQOS                                              */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225TransportQOS (OOCTXT* pctxt, H225TransportQOS* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT ui;
   ASN1OpenType openType;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   if (!extbit) {
      stat = decodeConsUnsigned (pctxt, &ui, 0, 2);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      else pvalue->t = ui + 1;

      switch (ui) {
         /* endpointControlled */
         case 0:
            rtInvokeStartElement (pctxt, "endpointControlled", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "endpointControlled", -1);

            break;

         /* gatekeeperControlled */
         case 1:
            rtInvokeStartElement (pctxt, "gatekeeperControlled", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "gatekeeperControlled", -1);

            break;

         /* noControl */
         case 2:
            rtInvokeStartElement (pctxt, "noControl", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "noControl", -1);

            break;

         default:
            return LOG_ASN1ERR (pctxt, ASN_E_INVOPT);
      }
   }
   else {
      stat = decodeSmallNonNegWholeNumber (pctxt, &ui);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      else pvalue->t = ui + 4;

      stat = decodeByteAlign (pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      pvalue->u.extElem1 = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
      if (pvalue->u.extElem1 == NULL)
         return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

      pvalue->u.extElem1->data = openType.data;
      pvalue->u.extElem1->numocts = openType.numocts;

   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  AdmissionRequest                                          */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225AdmissionRequest (OOCTXT* pctxt, H225AdmissionRequest* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   OOCTXT lctxt2;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.callModelPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.destinationInfoPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.destCallSignalAddressPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.destExtraCallInfoPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.srcCallSignalAddressPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.nonStandardDataPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.callServicesPresent = optbit;

   /* decode requestSeqNum */

   rtInvokeStartElement (pctxt, "requestSeqNum", -1);

   stat = asn1PD_H225RequestSeqNum (pctxt, &pvalue->requestSeqNum);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "requestSeqNum", -1);

   /* decode callType */

   rtInvokeStartElement (pctxt, "callType", -1);

   stat = asn1PD_H225CallType (pctxt, &pvalue->callType);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "callType", -1);

   /* decode callModel */

   if (pvalue->m.callModelPresent) {
      rtInvokeStartElement (pctxt, "callModel", -1);

      stat = asn1PD_H225CallModel (pctxt, &pvalue->callModel);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "callModel", -1);
   }

   /* decode endpointIdentifier */

   rtInvokeStartElement (pctxt, "endpointIdentifier", -1);

   stat = asn1PD_H225EndpointIdentifier (pctxt, &pvalue->endpointIdentifier);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "endpointIdentifier", -1);

   /* decode destinationInfo */

   if (pvalue->m.destinationInfoPresent) {
      rtInvokeStartElement (pctxt, "destinationInfo", -1);

      stat = asn1PD_H225_SeqOfH225AliasAddress (pctxt, &pvalue->destinationInfo);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "destinationInfo", -1);
   }

   /* decode destCallSignalAddress */

   if (pvalue->m.destCallSignalAddressPresent) {
      rtInvokeStartElement (pctxt, "destCallSignalAddress", -1);

      stat = asn1PD_H225TransportAddress (pctxt, &pvalue->destCallSignalAddress);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "destCallSignalAddress", -1);
   }

   /* decode destExtraCallInfo */

   if (pvalue->m.destExtraCallInfoPresent) {
      rtInvokeStartElement (pctxt, "destExtraCallInfo", -1);

      stat = asn1PD_H225_SeqOfH225AliasAddress (pctxt, &pvalue->destExtraCallInfo);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "destExtraCallInfo", -1);
   }

   /* decode srcInfo */

   rtInvokeStartElement (pctxt, "srcInfo", -1);

   stat = asn1PD_H225_SeqOfH225AliasAddress (pctxt, &pvalue->srcInfo);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "srcInfo", -1);

   /* decode srcCallSignalAddress */

   if (pvalue->m.srcCallSignalAddressPresent) {
      rtInvokeStartElement (pctxt, "srcCallSignalAddress", -1);

      stat = asn1PD_H225TransportAddress (pctxt, &pvalue->srcCallSignalAddress);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "srcCallSignalAddress", -1);
   }

   /* decode bandWidth */

   rtInvokeStartElement (pctxt, "bandWidth", -1);

   stat = asn1PD_H225BandWidth (pctxt, &pvalue->bandWidth);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "bandWidth", -1);

   /* decode callReferenceValue */

   rtInvokeStartElement (pctxt, "callReferenceValue", -1);

   stat = asn1PD_H225CallReferenceValue (pctxt, &pvalue->callReferenceValue);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "callReferenceValue", -1);

   /* decode nonStandardData */

   if (pvalue->m.nonStandardDataPresent) {
      rtInvokeStartElement (pctxt, "nonStandardData", -1);

      stat = asn1PD_H225NonStandardParameter (pctxt, &pvalue->nonStandardData);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "nonStandardData", -1);
   }

   /* decode callServices */

   if (pvalue->m.callServicesPresent) {
      rtInvokeStartElement (pctxt, "callServices", -1);

      stat = asn1PD_H225QseriesOptions (pctxt, &pvalue->callServices);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "callServices", -1);
   }

   /* decode conferenceID */

   rtInvokeStartElement (pctxt, "conferenceID", -1);

   stat = asn1PD_H225ConferenceIdentifier (pctxt, &pvalue->conferenceID);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "conferenceID", -1);

   /* decode activeMC */

   rtInvokeStartElement (pctxt, "activeMC", -1);

   stat = DECODEBIT (pctxt, &pvalue->activeMC);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   rtInvokeBoolValue (pctxt, pvalue->activeMC);

   rtInvokeEndElement (pctxt, "activeMC", -1);

   /* decode answerCall */

   rtInvokeStartElement (pctxt, "answerCall", -1);

   stat = DECODEBIT (pctxt, &pvalue->answerCall);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   rtInvokeBoolValue (pctxt, pvalue->answerCall);

   rtInvokeEndElement (pctxt, "answerCall", -1);

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            if (i < 19 && openType.numocts > 0) {  /* known element */
               copyContext (&lctxt2, pctxt);
               initContextBuffer (pctxt, openType.data, openType.numocts);

               switch (i) {
                  case 0:
                     pvalue->m.canMapAliasPresent = 1;

                     rtInvokeStartElement (pctxt, "canMapAlias", -1);

                     stat = DECODEBIT (pctxt, &pvalue->canMapAlias);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
                     rtInvokeBoolValue (pctxt, pvalue->canMapAlias);

                     rtInvokeEndElement (pctxt, "canMapAlias", -1);
                     break;

                  case 1:
                     pvalue->m.callIdentifierPresent = 1;

                     rtInvokeStartElement (pctxt, "callIdentifier", -1);

                     stat = asn1PD_H225CallIdentifier (pctxt, &pvalue->callIdentifier);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "callIdentifier", -1);
                     break;

                  case 2:
                     pvalue->m.srcAlternativesPresent = 1;

                     rtInvokeStartElement (pctxt, "srcAlternatives", -1);

                     stat = asn1PD_H225_SeqOfH225Endpoint (pctxt, &pvalue->srcAlternatives);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "srcAlternatives", -1);
                     break;

                  case 3:
                     pvalue->m.destAlternativesPresent = 1;

                     rtInvokeStartElement (pctxt, "destAlternatives", -1);

                     stat = asn1PD_H225_SeqOfH225Endpoint (pctxt, &pvalue->destAlternatives);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "destAlternatives", -1);
                     break;

                  case 4:
                     pvalue->m.gatekeeperIdentifierPresent = 1;

                     rtInvokeStartElement (pctxt, "gatekeeperIdentifier", -1);

                     stat = asn1PD_H225GatekeeperIdentifier (pctxt, &pvalue->gatekeeperIdentifier);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "gatekeeperIdentifier", -1);
                     break;

                  case 5:
                     pvalue->m.tokensPresent = 1;

                     rtInvokeStartElement (pctxt, "tokens", -1);

                     stat = asn1PD_H225_SeqOfH225ClearToken (pctxt, &pvalue->tokens);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "tokens", -1);
                     break;

                  case 6:
                     pvalue->m.cryptoTokensPresent = 1;

                     rtInvokeStartElement (pctxt, "cryptoTokens", -1);

                     stat = asn1PD_H225_SeqOfH225CryptoH323Token (pctxt, &pvalue->cryptoTokens);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "cryptoTokens", -1);
                     break;

                  case 7:
                     pvalue->m.integrityCheckValuePresent = 1;

                     rtInvokeStartElement (pctxt, "integrityCheckValue", -1);

                     stat = asn1PD_H225ICV (pctxt, &pvalue->integrityCheckValue);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "integrityCheckValue", -1);
                     break;

                  case 8:
                     pvalue->m.transportQOSPresent = 1;

                     rtInvokeStartElement (pctxt, "transportQOS", -1);

                     stat = asn1PD_H225TransportQOS (pctxt, &pvalue->transportQOS);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "transportQOS", -1);
                     break;

                  case 9:
                     pvalue->m.willSupplyUUIEsPresent = 1;

                     rtInvokeStartElement (pctxt, "willSupplyUUIEs", -1);

                     stat = DECODEBIT (pctxt, &pvalue->willSupplyUUIEs);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
                     rtInvokeBoolValue (pctxt, pvalue->willSupplyUUIEs);

                     rtInvokeEndElement (pctxt, "willSupplyUUIEs", -1);
                     break;

                  case 10:
                     pvalue->m.callLinkagePresent = 1;

                     rtInvokeStartElement (pctxt, "callLinkage", -1);

                     stat = asn1PD_H225CallLinkage (pctxt, &pvalue->callLinkage);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "callLinkage", -1);
                     break;

                  case 11:
                     pvalue->m.gatewayDataRatePresent = 1;

                     rtInvokeStartElement (pctxt, "gatewayDataRate", -1);

                     stat = asn1PD_H225DataRate (pctxt, &pvalue->gatewayDataRate);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "gatewayDataRate", -1);
                     break;

                  case 12:
                     pvalue->m.capacityPresent = 1;

                     rtInvokeStartElement (pctxt, "capacity", -1);

                     stat = asn1PD_H225CallCapacity (pctxt, &pvalue->capacity);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "capacity", -1);
                     break;

                  case 13:
                     pvalue->m.circuitInfoPresent = 1;

                     rtInvokeStartElement (pctxt, "circuitInfo", -1);

                     stat = asn1PD_H225CircuitInfo (pctxt, &pvalue->circuitInfo);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "circuitInfo", -1);
                     break;

                  case 14:
                     pvalue->m.desiredProtocolsPresent = 1;

                     rtInvokeStartElement (pctxt, "desiredProtocols", -1);

                     stat = asn1PD_H225_SeqOfH225SupportedProtocols (pctxt, &pvalue->desiredProtocols);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "desiredProtocols", -1);
                     break;

                  case 15:
                     pvalue->m.desiredTunnelledProtocolPresent = 1;

                     rtInvokeStartElement (pctxt, "desiredTunnelledProtocol", -1);

                     stat = asn1PD_H225TunnelledProtocol (pctxt, &pvalue->desiredTunnelledProtocol);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "desiredTunnelledProtocol", -1);
                     break;

                  case 16:
                     pvalue->m.featureSetPresent = 1;

                     rtInvokeStartElement (pctxt, "featureSet", -1);

                     stat = asn1PD_H225FeatureSet (pctxt, &pvalue->featureSet);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "featureSet", -1);
                     break;

                  case 17:
                     pvalue->m.genericDataPresent = 1;

                     rtInvokeStartElement (pctxt, "genericData", -1);

                     stat = asn1PD_H225_SeqOfH225GenericData (pctxt, &pvalue->genericData);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "genericData", -1);
                     break;

                  case 18:
                     pvalue->m.canMapSrcAliasPresent = 1;

                     rtInvokeStartElement (pctxt, "canMapSrcAlias", -1);

                     stat = DECODEBIT (pctxt, &pvalue->canMapSrcAlias);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
                     rtInvokeBoolValue (pctxt, pvalue->canMapSrcAlias);

                     rtInvokeEndElement (pctxt, "canMapSrcAlias", -1);
                     break;

                  default:
                     pctxt->buffer.byteIndex += openType.numocts;
               }
               copyContext (pctxt, &lctxt2);
            }
            else {  /* unknown element */
               pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
               pOpenType->numocts = openType.numocts;
               pOpenType->data = openType.data;
               dListAppend (pctxt, &pvalue->extElem1, pOpenType);
            }
         }
         else if (i >= 19) {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  UUIEsRequested                                            */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225UUIEsRequested (OOCTXT* pctxt, H225UUIEsRequested* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   OOCTXT lctxt2;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   /* decode setup */

   rtInvokeStartElement (pctxt, "setup", -1);

   stat = DECODEBIT (pctxt, &pvalue->setup);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   rtInvokeBoolValue (pctxt, pvalue->setup);

   rtInvokeEndElement (pctxt, "setup", -1);

   /* decode callProceeding */

   rtInvokeStartElement (pctxt, "callProceeding", -1);

   stat = DECODEBIT (pctxt, &pvalue->callProceeding);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   rtInvokeBoolValue (pctxt, pvalue->callProceeding);

   rtInvokeEndElement (pctxt, "callProceeding", -1);

   /* decode connect */

   rtInvokeStartElement (pctxt, "connect", -1);

   stat = DECODEBIT (pctxt, &pvalue->connect);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   rtInvokeBoolValue (pctxt, pvalue->connect);

   rtInvokeEndElement (pctxt, "connect", -1);

   /* decode alerting */

   rtInvokeStartElement (pctxt, "alerting", -1);

   stat = DECODEBIT (pctxt, &pvalue->alerting);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   rtInvokeBoolValue (pctxt, pvalue->alerting);

   rtInvokeEndElement (pctxt, "alerting", -1);

   /* decode information */

   rtInvokeStartElement (pctxt, "information", -1);

   stat = DECODEBIT (pctxt, &pvalue->information);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   rtInvokeBoolValue (pctxt, pvalue->information);

   rtInvokeEndElement (pctxt, "information", -1);

   /* decode releaseComplete */

   rtInvokeStartElement (pctxt, "releaseComplete", -1);

   stat = DECODEBIT (pctxt, &pvalue->releaseComplete);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   rtInvokeBoolValue (pctxt, pvalue->releaseComplete);

   rtInvokeEndElement (pctxt, "releaseComplete", -1);

   /* decode facility */

   rtInvokeStartElement (pctxt, "facility", -1);

   stat = DECODEBIT (pctxt, &pvalue->facility);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   rtInvokeBoolValue (pctxt, pvalue->facility);

   rtInvokeEndElement (pctxt, "facility", -1);

   /* decode progress */

   rtInvokeStartElement (pctxt, "progress", -1);

   stat = DECODEBIT (pctxt, &pvalue->progress);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   rtInvokeBoolValue (pctxt, pvalue->progress);

   rtInvokeEndElement (pctxt, "progress", -1);

   /* decode empty */

   rtInvokeStartElement (pctxt, "empty", -1);

   stat = DECODEBIT (pctxt, &pvalue->empty);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   rtInvokeBoolValue (pctxt, pvalue->empty);

   rtInvokeEndElement (pctxt, "empty", -1);

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            if (i < 4 && openType.numocts > 0) {  /* known element */
               copyContext (&lctxt2, pctxt);
               initContextBuffer (pctxt, openType.data, openType.numocts);

               switch (i) {
                  case 0:
                     pvalue->m.statusPresent = 1;

                     rtInvokeStartElement (pctxt, "status", -1);

                     stat = DECODEBIT (pctxt, &pvalue->status);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
                     rtInvokeBoolValue (pctxt, pvalue->status);

                     rtInvokeEndElement (pctxt, "status", -1);
                     break;

                  case 1:
                     pvalue->m.statusInquiryPresent = 1;

                     rtInvokeStartElement (pctxt, "statusInquiry", -1);

                     stat = DECODEBIT (pctxt, &pvalue->statusInquiry);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
                     rtInvokeBoolValue (pctxt, pvalue->statusInquiry);

                     rtInvokeEndElement (pctxt, "statusInquiry", -1);
                     break;

                  case 2:
                     pvalue->m.setupAcknowledgePresent = 1;

                     rtInvokeStartElement (pctxt, "setupAcknowledge", -1);

                     stat = DECODEBIT (pctxt, &pvalue->setupAcknowledge);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
                     rtInvokeBoolValue (pctxt, pvalue->setupAcknowledge);

                     rtInvokeEndElement (pctxt, "setupAcknowledge", -1);
                     break;

                  case 3:
                     pvalue->m.notifyPresent = 1;

                     rtInvokeStartElement (pctxt, "notify", -1);

                     stat = DECODEBIT (pctxt, &pvalue->notify);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
                     rtInvokeBoolValue (pctxt, pvalue->notify);

                     rtInvokeEndElement (pctxt, "notify", -1);
                     break;

                  default:
                     pctxt->buffer.byteIndex += openType.numocts;
               }
               copyContext (pctxt, &lctxt2);
            }
            else {  /* unknown element */
               pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
               pOpenType->numocts = openType.numocts;
               pOpenType->data = openType.data;
               dListAppend (pctxt, &pvalue->extElem1, pOpenType);
            }
         }
         else if (i >= 4) {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  AdmissionConfirm_language                                 */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225AdmissionConfirm_language (OOCTXT* pctxt, H225AdmissionConfirm_language* pvalue)
{
   static Asn1SizeCnst element_lsize1 = { 0, 1, 32, 0 };
   int stat = ASN_OK;
   ASN1UINT xx1;

   /* decode length determinant */

   stat = decodeLength (pctxt, &pvalue->n);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* decode elements */

   ALLOC_ASN1ARRAY (pctxt, pvalue, ASN1IA5String);

   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      rtInvokeStartElement (pctxt, "elem", xx1);

      stat = addSizeConstraint (pctxt, &element_lsize1);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = decodeConstrainedStringEx (pctxt, &pvalue->elem[xx1], 0, 8, 7, 7);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      rtInvokeCharStrValue (pctxt, pvalue->elem[xx1]);
      rtInvokeEndElement (pctxt, "elem", xx1);

   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  AdmissionConfirm                                          */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225AdmissionConfirm (OOCTXT* pctxt, H225AdmissionConfirm* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   OOCTXT lctxt2;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.irrFrequencyPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.nonStandardDataPresent = optbit;

   /* decode requestSeqNum */

   rtInvokeStartElement (pctxt, "requestSeqNum", -1);

   stat = asn1PD_H225RequestSeqNum (pctxt, &pvalue->requestSeqNum);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "requestSeqNum", -1);

   /* decode bandWidth */

   rtInvokeStartElement (pctxt, "bandWidth", -1);

   stat = asn1PD_H225BandWidth (pctxt, &pvalue->bandWidth);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "bandWidth", -1);

   /* decode callModel */

   rtInvokeStartElement (pctxt, "callModel", -1);

   stat = asn1PD_H225CallModel (pctxt, &pvalue->callModel);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "callModel", -1);

   /* decode destCallSignalAddress */

   rtInvokeStartElement (pctxt, "destCallSignalAddress", -1);

   stat = asn1PD_H225TransportAddress (pctxt, &pvalue->destCallSignalAddress);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "destCallSignalAddress", -1);

   /* decode irrFrequency */

   if (pvalue->m.irrFrequencyPresent) {
      rtInvokeStartElement (pctxt, "irrFrequency", -1);

      stat = decodeConsUInt16 (pctxt, &pvalue->irrFrequency, 1U, 65535U);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      rtInvokeUIntValue (pctxt, pvalue->irrFrequency);

      rtInvokeEndElement (pctxt, "irrFrequency", -1);
   }

   /* decode nonStandardData */

   if (pvalue->m.nonStandardDataPresent) {
      rtInvokeStartElement (pctxt, "nonStandardData", -1);

      stat = asn1PD_H225NonStandardParameter (pctxt, &pvalue->nonStandardData);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "nonStandardData", -1);
   }

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            if (i < 22 && openType.numocts > 0) {  /* known element */
               copyContext (&lctxt2, pctxt);
               initContextBuffer (pctxt, openType.data, openType.numocts);

               switch (i) {
                  case 0:
                     pvalue->m.destinationInfoPresent = 1;

                     rtInvokeStartElement (pctxt, "destinationInfo", -1);

                     stat = asn1PD_H225_SeqOfH225AliasAddress (pctxt, &pvalue->destinationInfo);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "destinationInfo", -1);
                     break;

                  case 1:
                     pvalue->m.destExtraCallInfoPresent = 1;

                     rtInvokeStartElement (pctxt, "destExtraCallInfo", -1);

                     stat = asn1PD_H225_SeqOfH225AliasAddress (pctxt, &pvalue->destExtraCallInfo);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "destExtraCallInfo", -1);
                     break;

                  case 2:
                     pvalue->m.destinationTypePresent = 1;

                     rtInvokeStartElement (pctxt, "destinationType", -1);

                     stat = asn1PD_H225EndpointType (pctxt, &pvalue->destinationType);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "destinationType", -1);
                     break;

                  case 3:
                     pvalue->m.remoteExtensionAddressPresent = 1;

                     rtInvokeStartElement (pctxt, "remoteExtensionAddress", -1);

                     stat = asn1PD_H225_SeqOfH225AliasAddress (pctxt, &pvalue->remoteExtensionAddress);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "remoteExtensionAddress", -1);
                     break;

                  case 4:
                     pvalue->m.alternateEndpointsPresent = 1;

                     rtInvokeStartElement (pctxt, "alternateEndpoints", -1);

                     stat = asn1PD_H225_SeqOfH225Endpoint (pctxt, &pvalue->alternateEndpoints);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "alternateEndpoints", -1);
                     break;

                  case 5:
                     pvalue->m.tokensPresent = 1;

                     rtInvokeStartElement (pctxt, "tokens", -1);

                     stat = asn1PD_H225_SeqOfH225ClearToken (pctxt, &pvalue->tokens);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "tokens", -1);
                     break;

                  case 6:
                     pvalue->m.cryptoTokensPresent = 1;

                     rtInvokeStartElement (pctxt, "cryptoTokens", -1);

                     stat = asn1PD_H225_SeqOfH225CryptoH323Token (pctxt, &pvalue->cryptoTokens);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "cryptoTokens", -1);
                     break;

                  case 7:
                     pvalue->m.integrityCheckValuePresent = 1;

                     rtInvokeStartElement (pctxt, "integrityCheckValue", -1);

                     stat = asn1PD_H225ICV (pctxt, &pvalue->integrityCheckValue);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "integrityCheckValue", -1);
                     break;

                  case 8:
                     pvalue->m.transportQOSPresent = 1;

                     rtInvokeStartElement (pctxt, "transportQOS", -1);

                     stat = asn1PD_H225TransportQOS (pctxt, &pvalue->transportQOS);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "transportQOS", -1);
                     break;

                  case 9:
                     pvalue->m.willRespondToIRRPresent = 1;

                     rtInvokeStartElement (pctxt, "willRespondToIRR", -1);

                     stat = DECODEBIT (pctxt, &pvalue->willRespondToIRR);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
                     rtInvokeBoolValue (pctxt, pvalue->willRespondToIRR);

                     rtInvokeEndElement (pctxt, "willRespondToIRR", -1);
                     break;

                  case 10:
                     pvalue->m.uuiesRequestedPresent = 1;

                     rtInvokeStartElement (pctxt, "uuiesRequested", -1);

                     stat = asn1PD_H225UUIEsRequested (pctxt, &pvalue->uuiesRequested);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "uuiesRequested", -1);
                     break;

                  case 11:
                     pvalue->m.languagePresent = 1;

                     rtInvokeStartElement (pctxt, "language", -1);

                     stat = asn1PD_H225AdmissionConfirm_language (pctxt, &pvalue->language);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "language", -1);
                     break;

                  case 12:
                     pvalue->m.alternateTransportAddressesPresent = 1;

                     rtInvokeStartElement (pctxt, "alternateTransportAddresses", -1);

                     stat = asn1PD_H225AlternateTransportAddresses (pctxt, &pvalue->alternateTransportAddresses);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "alternateTransportAddresses", -1);
                     break;

                  case 13:
                     pvalue->m.useSpecifiedTransportPresent = 1;

                     rtInvokeStartElement (pctxt, "useSpecifiedTransport", -1);

                     stat = asn1PD_H225UseSpecifiedTransport (pctxt, &pvalue->useSpecifiedTransport);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "useSpecifiedTransport", -1);
                     break;

                  case 14:
                     pvalue->m.circuitInfoPresent = 1;

                     rtInvokeStartElement (pctxt, "circuitInfo", -1);

                     stat = asn1PD_H225CircuitInfo (pctxt, &pvalue->circuitInfo);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "circuitInfo", -1);
                     break;

                  case 15:
                     pvalue->m.usageSpecPresent = 1;

                     rtInvokeStartElement (pctxt, "usageSpec", -1);

                     stat = asn1PD_H225_SeqOfH225RasUsageSpecification (pctxt, &pvalue->usageSpec);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "usageSpec", -1);
                     break;

                  case 16:
                     pvalue->m.supportedProtocolsPresent = 1;

                     rtInvokeStartElement (pctxt, "supportedProtocols", -1);

                     stat = asn1PD_H225_SeqOfH225SupportedProtocols (pctxt, &pvalue->supportedProtocols);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "supportedProtocols", -1);
                     break;

                  case 17:
                     pvalue->m.serviceControlPresent = 1;

                     rtInvokeStartElement (pctxt, "serviceControl", -1);

                     stat = asn1PD_H225_SeqOfH225ServiceControlSession (pctxt, &pvalue->serviceControl);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "serviceControl", -1);
                     break;

                  case 18:
                     pvalue->m.multipleCallsPresent = 1;

                     rtInvokeStartElement (pctxt, "multipleCalls", -1);

                     stat = DECODEBIT (pctxt, &pvalue->multipleCalls);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
                     rtInvokeBoolValue (pctxt, pvalue->multipleCalls);

                     rtInvokeEndElement (pctxt, "multipleCalls", -1);
                     break;

                  case 19:
                     pvalue->m.featureSetPresent = 1;

                     rtInvokeStartElement (pctxt, "featureSet", -1);

                     stat = asn1PD_H225FeatureSet (pctxt, &pvalue->featureSet);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "featureSet", -1);
                     break;

                  case 20:
                     pvalue->m.genericDataPresent = 1;

                     rtInvokeStartElement (pctxt, "genericData", -1);

                     stat = asn1PD_H225_SeqOfH225GenericData (pctxt, &pvalue->genericData);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "genericData", -1);
                     break;

                  case 21:
                     pvalue->m.modifiedSrcInfoPresent = 1;

                     rtInvokeStartElement (pctxt, "modifiedSrcInfo", -1);

                     stat = asn1PD_H225_SeqOfH225AliasAddress (pctxt, &pvalue->modifiedSrcInfo);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "modifiedSrcInfo", -1);
                     break;

                  default:
                     pctxt->buffer.byteIndex += openType.numocts;
               }
               copyContext (pctxt, &lctxt2);
            }
            else {  /* unknown element */
               pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
               pOpenType->numocts = openType.numocts;
               pOpenType->data = openType.data;
               dListAppend (pctxt, &pvalue->extElem1, pOpenType);
            }
         }
         else if (i >= 22) {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  _SeqOfH225PartyNumber                                     */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225_SeqOfH225PartyNumber (OOCTXT* pctxt, H225_SeqOfH225PartyNumber* pvalue)
{
   int stat = ASN_OK;
   H225PartyNumber* pdata;
   ASN1UINT count = 0;
   ASN1UINT xx1;
   int lstat;

   dListInit (pvalue);

   for (;;) {
      /* decode length determinant */

      lstat = decodeLength (pctxt, &count);
      if (lstat != ASN_OK && lstat != ASN_OK_FRAG) {
         return LOG_ASN1ERR (pctxt, lstat);
      }

      /* decode elements */

      for (xx1 = 0; xx1 < count; xx1++) {
         rtInvokeStartElement (pctxt, "elem", xx1);

         pdata = ALLOC_ASN1ELEMDNODE (pctxt, H225PartyNumber);
         if (pdata == NULL)
            return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

         stat = asn1PD_H225PartyNumber (pctxt, pdata);
         if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
         rtInvokeEndElement (pctxt, "elem", xx1);

         dListAppendNode (pctxt, pvalue, pdata);
      }

      if(lstat == ASN_OK) break;
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  AdmissionRejectReason                                     */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225AdmissionRejectReason (OOCTXT* pctxt, H225AdmissionRejectReason* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT ui;
   ASN1OpenType openType;
   ASN1BOOL extbit;
   OOCTXT lctxt;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   if (!extbit) {
      stat = decodeConsUnsigned (pctxt, &ui, 0, 7);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      else pvalue->t = ui + 1;

      switch (ui) {
         /* calledPartyNotRegistered */
         case 0:
            rtInvokeStartElement (pctxt, "calledPartyNotRegistered", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "calledPartyNotRegistered", -1);

            break;

         /* invalidPermission */
         case 1:
            rtInvokeStartElement (pctxt, "invalidPermission", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "invalidPermission", -1);

            break;

         /* requestDenied */
         case 2:
            rtInvokeStartElement (pctxt, "requestDenied", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "requestDenied", -1);

            break;

         /* undefinedReason */
         case 3:
            rtInvokeStartElement (pctxt, "undefinedReason", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "undefinedReason", -1);

            break;

         /* callerNotRegistered */
         case 4:
            rtInvokeStartElement (pctxt, "callerNotRegistered", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "callerNotRegistered", -1);

            break;

         /* routeCallToGatekeeper */
         case 5:
            rtInvokeStartElement (pctxt, "routeCallToGatekeeper", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "routeCallToGatekeeper", -1);

            break;

         /* invalidEndpointIdentifier */
         case 6:
            rtInvokeStartElement (pctxt, "invalidEndpointIdentifier", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "invalidEndpointIdentifier", -1);

            break;

         /* resourceUnavailable */
         case 7:
            rtInvokeStartElement (pctxt, "resourceUnavailable", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "resourceUnavailable", -1);

            break;

         default:
            return LOG_ASN1ERR (pctxt, ASN_E_INVOPT);
      }
   }
   else {
      stat = decodeSmallNonNegWholeNumber (pctxt, &ui);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      else pvalue->t = ui + 9;

      stat = decodeByteAlign (pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      copyContext (&lctxt, pctxt);
      initContextBuffer (pctxt, openType.data, openType.numocts);

      switch (pvalue->t) {
         /* securityDenial */
         case 9:
            rtInvokeStartElement (pctxt, "securityDenial", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "securityDenial", -1);

            break;

         /* qosControlNotSupported */
         case 10:
            rtInvokeStartElement (pctxt, "qosControlNotSupported", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "qosControlNotSupported", -1);

            break;

         /* incompleteAddress */
         case 11:
            rtInvokeStartElement (pctxt, "incompleteAddress", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "incompleteAddress", -1);

            break;

         /* aliasesInconsistent */
         case 12:
            rtInvokeStartElement (pctxt, "aliasesInconsistent", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "aliasesInconsistent", -1);

            break;

         /* routeCallToSCN */
         case 13:
            rtInvokeStartElement (pctxt, "routeCallToSCN", -1);

            pvalue->u.routeCallToSCN = ALLOC_ASN1ELEM (pctxt, H225_SeqOfH225PartyNumber);
            if (pvalue->u.routeCallToSCN == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225_SeqOfH225PartyNumber (pctxt, pvalue->u.routeCallToSCN);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "routeCallToSCN", -1);

            break;

         /* exceedsCallCapacity */
         case 14:
            rtInvokeStartElement (pctxt, "exceedsCallCapacity", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "exceedsCallCapacity", -1);

            break;

         /* collectDestination */
         case 15:
            rtInvokeStartElement (pctxt, "collectDestination", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "collectDestination", -1);

            break;

         /* collectPIN */
         case 16:
            rtInvokeStartElement (pctxt, "collectPIN", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "collectPIN", -1);

            break;

         /* genericDataReason */
         case 17:
            rtInvokeStartElement (pctxt, "genericDataReason", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "genericDataReason", -1);

            break;

         /* neededFeatureNotSupported */
         case 18:
            rtInvokeStartElement (pctxt, "neededFeatureNotSupported", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "neededFeatureNotSupported", -1);

            break;

         /* securityErrors */
         case 19:
            rtInvokeStartElement (pctxt, "securityErrors", -1);

            pvalue->u.securityErrors = ALLOC_ASN1ELEM (pctxt, H225SecurityErrors2);
            if (pvalue->u.securityErrors == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225SecurityErrors2 (pctxt, pvalue->u.securityErrors);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "securityErrors", -1);

            break;

         /* securityDHmismatch */
         case 20:
            rtInvokeStartElement (pctxt, "securityDHmismatch", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "securityDHmismatch", -1);

            break;

         /* noRouteToDestination */
         case 21:
            rtInvokeStartElement (pctxt, "noRouteToDestination", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "noRouteToDestination", -1);

            break;

         /* unallocatedNumber */
         case 22:
            rtInvokeStartElement (pctxt, "unallocatedNumber", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "unallocatedNumber", -1);

            break;

         default:
            pvalue->u.extElem1 = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
            if (pvalue->u.extElem1 == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            pvalue->u.extElem1->data = openType.data;
            pvalue->u.extElem1->numocts = openType.numocts;

      }

      copyContext (pctxt, &lctxt);
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  AdmissionReject                                           */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225AdmissionReject (OOCTXT* pctxt, H225AdmissionReject* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   OOCTXT lctxt2;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.nonStandardDataPresent = optbit;

   /* decode requestSeqNum */

   rtInvokeStartElement (pctxt, "requestSeqNum", -1);

   stat = asn1PD_H225RequestSeqNum (pctxt, &pvalue->requestSeqNum);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "requestSeqNum", -1);

   /* decode rejectReason */

   rtInvokeStartElement (pctxt, "rejectReason", -1);

   stat = asn1PD_H225AdmissionRejectReason (pctxt, &pvalue->rejectReason);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "rejectReason", -1);

   /* decode nonStandardData */

   if (pvalue->m.nonStandardDataPresent) {
      rtInvokeStartElement (pctxt, "nonStandardData", -1);

      stat = asn1PD_H225NonStandardParameter (pctxt, &pvalue->nonStandardData);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "nonStandardData", -1);
   }

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            if (i < 8 && openType.numocts > 0) {  /* known element */
               copyContext (&lctxt2, pctxt);
               initContextBuffer (pctxt, openType.data, openType.numocts);

               switch (i) {
                  case 0:
                     pvalue->m.altGKInfoPresent = 1;

                     rtInvokeStartElement (pctxt, "altGKInfo", -1);

                     stat = asn1PD_H225AltGKInfo (pctxt, &pvalue->altGKInfo);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "altGKInfo", -1);
                     break;

                  case 1:
                     pvalue->m.tokensPresent = 1;

                     rtInvokeStartElement (pctxt, "tokens", -1);

                     stat = asn1PD_H225_SeqOfH225ClearToken (pctxt, &pvalue->tokens);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "tokens", -1);
                     break;

                  case 2:
                     pvalue->m.cryptoTokensPresent = 1;

                     rtInvokeStartElement (pctxt, "cryptoTokens", -1);

                     stat = asn1PD_H225_SeqOfH225CryptoH323Token (pctxt, &pvalue->cryptoTokens);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "cryptoTokens", -1);
                     break;

                  case 3:
                     pvalue->m.callSignalAddressPresent = 1;

                     rtInvokeStartElement (pctxt, "callSignalAddress", -1);

                     stat = asn1PD_H225_SeqOfH225TransportAddress (pctxt, &pvalue->callSignalAddress);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "callSignalAddress", -1);
                     break;

                  case 4:
                     pvalue->m.integrityCheckValuePresent = 1;

                     rtInvokeStartElement (pctxt, "integrityCheckValue", -1);

                     stat = asn1PD_H225ICV (pctxt, &pvalue->integrityCheckValue);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "integrityCheckValue", -1);
                     break;

                  case 5:
                     pvalue->m.serviceControlPresent = 1;

                     rtInvokeStartElement (pctxt, "serviceControl", -1);

                     stat = asn1PD_H225_SeqOfH225ServiceControlSession (pctxt, &pvalue->serviceControl);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "serviceControl", -1);
                     break;

                  case 6:
                     pvalue->m.featureSetPresent = 1;

                     rtInvokeStartElement (pctxt, "featureSet", -1);

                     stat = asn1PD_H225FeatureSet (pctxt, &pvalue->featureSet);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "featureSet", -1);
                     break;

                  case 7:
                     pvalue->m.genericDataPresent = 1;

                     rtInvokeStartElement (pctxt, "genericData", -1);

                     stat = asn1PD_H225_SeqOfH225GenericData (pctxt, &pvalue->genericData);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "genericData", -1);
                     break;

                  default:
                     pctxt->buffer.byteIndex += openType.numocts;
               }
               copyContext (pctxt, &lctxt2);
            }
            else {  /* unknown element */
               pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
               pOpenType->numocts = openType.numocts;
               pOpenType->data = openType.data;
               dListAppend (pctxt, &pvalue->extElem1, pOpenType);
            }
         }
         else if (i >= 8) {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  _SeqOfH225BandwidthDetails                                */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225_SeqOfH225BandwidthDetails (OOCTXT* pctxt, H225_SeqOfH225BandwidthDetails* pvalue)
{
   int stat = ASN_OK;
   H225BandwidthDetails* pdata;
   ASN1UINT count = 0;
   ASN1UINT xx1;
   int lstat;

   dListInit (pvalue);

   for (;;) {
      /* decode length determinant */

      lstat = decodeLength (pctxt, &count);
      if (lstat != ASN_OK && lstat != ASN_OK_FRAG) {
         return LOG_ASN1ERR (pctxt, lstat);
      }

      /* decode elements */

      for (xx1 = 0; xx1 < count; xx1++) {
         rtInvokeStartElement (pctxt, "elem", xx1);

         pdata = ALLOC_ASN1ELEMDNODE (pctxt, H225BandwidthDetails);
         if (pdata == NULL)
            return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

         stat = asn1PD_H225BandwidthDetails (pctxt, pdata);
         if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
         rtInvokeEndElement (pctxt, "elem", xx1);

         dListAppendNode (pctxt, pvalue, pdata);
      }

      if(lstat == ASN_OK) break;
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  BandwidthRequest                                          */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225BandwidthRequest (OOCTXT* pctxt, H225BandwidthRequest* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   OOCTXT lctxt2;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.callTypePresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.nonStandardDataPresent = optbit;

   /* decode requestSeqNum */

   rtInvokeStartElement (pctxt, "requestSeqNum", -1);

   stat = asn1PD_H225RequestSeqNum (pctxt, &pvalue->requestSeqNum);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "requestSeqNum", -1);

   /* decode endpointIdentifier */

   rtInvokeStartElement (pctxt, "endpointIdentifier", -1);

   stat = asn1PD_H225EndpointIdentifier (pctxt, &pvalue->endpointIdentifier);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "endpointIdentifier", -1);

   /* decode conferenceID */

   rtInvokeStartElement (pctxt, "conferenceID", -1);

   stat = asn1PD_H225ConferenceIdentifier (pctxt, &pvalue->conferenceID);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "conferenceID", -1);

   /* decode callReferenceValue */

   rtInvokeStartElement (pctxt, "callReferenceValue", -1);

   stat = asn1PD_H225CallReferenceValue (pctxt, &pvalue->callReferenceValue);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "callReferenceValue", -1);

   /* decode callType */

   if (pvalue->m.callTypePresent) {
      rtInvokeStartElement (pctxt, "callType", -1);

      stat = asn1PD_H225CallType (pctxt, &pvalue->callType);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "callType", -1);
   }

   /* decode bandWidth */

   rtInvokeStartElement (pctxt, "bandWidth", -1);

   stat = asn1PD_H225BandWidth (pctxt, &pvalue->bandWidth);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "bandWidth", -1);

   /* decode nonStandardData */

   if (pvalue->m.nonStandardDataPresent) {
      rtInvokeStartElement (pctxt, "nonStandardData", -1);

      stat = asn1PD_H225NonStandardParameter (pctxt, &pvalue->nonStandardData);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "nonStandardData", -1);
   }

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            if (i < 11 && openType.numocts > 0) {  /* known element */
               copyContext (&lctxt2, pctxt);
               initContextBuffer (pctxt, openType.data, openType.numocts);

               switch (i) {
                  case 0:
                     pvalue->m.callIdentifierPresent = 1;

                     rtInvokeStartElement (pctxt, "callIdentifier", -1);

                     stat = asn1PD_H225CallIdentifier (pctxt, &pvalue->callIdentifier);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "callIdentifier", -1);
                     break;

                  case 1:
                     pvalue->m.gatekeeperIdentifierPresent = 1;

                     rtInvokeStartElement (pctxt, "gatekeeperIdentifier", -1);

                     stat = asn1PD_H225GatekeeperIdentifier (pctxt, &pvalue->gatekeeperIdentifier);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "gatekeeperIdentifier", -1);
                     break;

                  case 2:
                     pvalue->m.tokensPresent = 1;

                     rtInvokeStartElement (pctxt, "tokens", -1);

                     stat = asn1PD_H225_SeqOfH225ClearToken (pctxt, &pvalue->tokens);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "tokens", -1);
                     break;

                  case 3:
                     pvalue->m.cryptoTokensPresent = 1;

                     rtInvokeStartElement (pctxt, "cryptoTokens", -1);

                     stat = asn1PD_H225_SeqOfH225CryptoH323Token (pctxt, &pvalue->cryptoTokens);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "cryptoTokens", -1);
                     break;

                  case 4:
                     pvalue->m.integrityCheckValuePresent = 1;

                     rtInvokeStartElement (pctxt, "integrityCheckValue", -1);

                     stat = asn1PD_H225ICV (pctxt, &pvalue->integrityCheckValue);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "integrityCheckValue", -1);
                     break;

                  case 5:
                     pvalue->m.answeredCallPresent = 1;

                     rtInvokeStartElement (pctxt, "answeredCall", -1);

                     stat = DECODEBIT (pctxt, &pvalue->answeredCall);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
                     rtInvokeBoolValue (pctxt, pvalue->answeredCall);

                     rtInvokeEndElement (pctxt, "answeredCall", -1);
                     break;

                  case 6:
                     pvalue->m.callLinkagePresent = 1;

                     rtInvokeStartElement (pctxt, "callLinkage", -1);

                     stat = asn1PD_H225CallLinkage (pctxt, &pvalue->callLinkage);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "callLinkage", -1);
                     break;

                  case 7:
                     pvalue->m.capacityPresent = 1;

                     rtInvokeStartElement (pctxt, "capacity", -1);

                     stat = asn1PD_H225CallCapacity (pctxt, &pvalue->capacity);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "capacity", -1);
                     break;

                  case 8:
                     pvalue->m.usageInformationPresent = 1;

                     rtInvokeStartElement (pctxt, "usageInformation", -1);

                     stat = asn1PD_H225RasUsageInformation (pctxt, &pvalue->usageInformation);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "usageInformation", -1);
                     break;

                  case 9:
                     pvalue->m.bandwidthDetailsPresent = 1;

                     rtInvokeStartElement (pctxt, "bandwidthDetails", -1);

                     stat = asn1PD_H225_SeqOfH225BandwidthDetails (pctxt, &pvalue->bandwidthDetails);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "bandwidthDetails", -1);
                     break;

                  case 10:
                     pvalue->m.genericDataPresent = 1;

                     rtInvokeStartElement (pctxt, "genericData", -1);

                     stat = asn1PD_H225_SeqOfH225GenericData (pctxt, &pvalue->genericData);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "genericData", -1);
                     break;

                  default:
                     pctxt->buffer.byteIndex += openType.numocts;
               }
               copyContext (pctxt, &lctxt2);
            }
            else {  /* unknown element */
               pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
               pOpenType->numocts = openType.numocts;
               pOpenType->data = openType.data;
               dListAppend (pctxt, &pvalue->extElem1, pOpenType);
            }
         }
         else if (i >= 11) {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  BandwidthConfirm                                          */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225BandwidthConfirm (OOCTXT* pctxt, H225BandwidthConfirm* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   OOCTXT lctxt2;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.nonStandardDataPresent = optbit;

   /* decode requestSeqNum */

   rtInvokeStartElement (pctxt, "requestSeqNum", -1);

   stat = asn1PD_H225RequestSeqNum (pctxt, &pvalue->requestSeqNum);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "requestSeqNum", -1);

   /* decode bandWidth */

   rtInvokeStartElement (pctxt, "bandWidth", -1);

   stat = asn1PD_H225BandWidth (pctxt, &pvalue->bandWidth);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "bandWidth", -1);

   /* decode nonStandardData */

   if (pvalue->m.nonStandardDataPresent) {
      rtInvokeStartElement (pctxt, "nonStandardData", -1);

      stat = asn1PD_H225NonStandardParameter (pctxt, &pvalue->nonStandardData);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "nonStandardData", -1);
   }

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            if (i < 5 && openType.numocts > 0) {  /* known element */
               copyContext (&lctxt2, pctxt);
               initContextBuffer (pctxt, openType.data, openType.numocts);

               switch (i) {
                  case 0:
                     pvalue->m.tokensPresent = 1;

                     rtInvokeStartElement (pctxt, "tokens", -1);

                     stat = asn1PD_H225_SeqOfH225ClearToken (pctxt, &pvalue->tokens);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "tokens", -1);
                     break;

                  case 1:
                     pvalue->m.cryptoTokensPresent = 1;

                     rtInvokeStartElement (pctxt, "cryptoTokens", -1);

                     stat = asn1PD_H225_SeqOfH225CryptoH323Token (pctxt, &pvalue->cryptoTokens);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "cryptoTokens", -1);
                     break;

                  case 2:
                     pvalue->m.integrityCheckValuePresent = 1;

                     rtInvokeStartElement (pctxt, "integrityCheckValue", -1);

                     stat = asn1PD_H225ICV (pctxt, &pvalue->integrityCheckValue);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "integrityCheckValue", -1);
                     break;

                  case 3:
                     pvalue->m.capacityPresent = 1;

                     rtInvokeStartElement (pctxt, "capacity", -1);

                     stat = asn1PD_H225CallCapacity (pctxt, &pvalue->capacity);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "capacity", -1);
                     break;

                  case 4:
                     pvalue->m.genericDataPresent = 1;

                     rtInvokeStartElement (pctxt, "genericData", -1);

                     stat = asn1PD_H225_SeqOfH225GenericData (pctxt, &pvalue->genericData);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "genericData", -1);
                     break;

                  default:
                     pctxt->buffer.byteIndex += openType.numocts;
               }
               copyContext (pctxt, &lctxt2);
            }
            else {  /* unknown element */
               pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
               pOpenType->numocts = openType.numocts;
               pOpenType->data = openType.data;
               dListAppend (pctxt, &pvalue->extElem1, pOpenType);
            }
         }
         else if (i >= 5) {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  BandRejectReason                                          */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225BandRejectReason (OOCTXT* pctxt, H225BandRejectReason* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT ui;
   ASN1OpenType openType;
   ASN1BOOL extbit;
   OOCTXT lctxt;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   if (!extbit) {
      stat = decodeConsUnsigned (pctxt, &ui, 0, 5);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      else pvalue->t = ui + 1;

      switch (ui) {
         /* notBound */
         case 0:
            rtInvokeStartElement (pctxt, "notBound", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "notBound", -1);

            break;

         /* invalidConferenceID */
         case 1:
            rtInvokeStartElement (pctxt, "invalidConferenceID", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "invalidConferenceID", -1);

            break;

         /* invalidPermission */
         case 2:
            rtInvokeStartElement (pctxt, "invalidPermission", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "invalidPermission", -1);

            break;

         /* insufficientResources */
         case 3:
            rtInvokeStartElement (pctxt, "insufficientResources", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "insufficientResources", -1);

            break;

         /* invalidRevision */
         case 4:
            rtInvokeStartElement (pctxt, "invalidRevision", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "invalidRevision", -1);

            break;

         /* undefinedReason */
         case 5:
            rtInvokeStartElement (pctxt, "undefinedReason", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "undefinedReason", -1);

            break;

         default:
            return LOG_ASN1ERR (pctxt, ASN_E_INVOPT);
      }
   }
   else {
      stat = decodeSmallNonNegWholeNumber (pctxt, &ui);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      else pvalue->t = ui + 7;

      stat = decodeByteAlign (pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      copyContext (&lctxt, pctxt);
      initContextBuffer (pctxt, openType.data, openType.numocts);

      switch (pvalue->t) {
         /* securityDenial */
         case 7:
            rtInvokeStartElement (pctxt, "securityDenial", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "securityDenial", -1);

            break;

         /* securityError */
         case 8:
            rtInvokeStartElement (pctxt, "securityError", -1);

            pvalue->u.securityError = ALLOC_ASN1ELEM (pctxt, H225SecurityErrors2);
            if (pvalue->u.securityError == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225SecurityErrors2 (pctxt, pvalue->u.securityError);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "securityError", -1);

            break;

         default:
            pvalue->u.extElem1 = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
            if (pvalue->u.extElem1 == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            pvalue->u.extElem1->data = openType.data;
            pvalue->u.extElem1->numocts = openType.numocts;

      }

      copyContext (pctxt, &lctxt);
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  BandwidthReject                                           */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225BandwidthReject (OOCTXT* pctxt, H225BandwidthReject* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   OOCTXT lctxt2;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.nonStandardDataPresent = optbit;

   /* decode requestSeqNum */

   rtInvokeStartElement (pctxt, "requestSeqNum", -1);

   stat = asn1PD_H225RequestSeqNum (pctxt, &pvalue->requestSeqNum);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "requestSeqNum", -1);

   /* decode rejectReason */

   rtInvokeStartElement (pctxt, "rejectReason", -1);

   stat = asn1PD_H225BandRejectReason (pctxt, &pvalue->rejectReason);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "rejectReason", -1);

   /* decode allowedBandWidth */

   rtInvokeStartElement (pctxt, "allowedBandWidth", -1);

   stat = asn1PD_H225BandWidth (pctxt, &pvalue->allowedBandWidth);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "allowedBandWidth", -1);

   /* decode nonStandardData */

   if (pvalue->m.nonStandardDataPresent) {
      rtInvokeStartElement (pctxt, "nonStandardData", -1);

      stat = asn1PD_H225NonStandardParameter (pctxt, &pvalue->nonStandardData);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "nonStandardData", -1);
   }

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            if (i < 5 && openType.numocts > 0) {  /* known element */
               copyContext (&lctxt2, pctxt);
               initContextBuffer (pctxt, openType.data, openType.numocts);

               switch (i) {
                  case 0:
                     pvalue->m.altGKInfoPresent = 1;

                     rtInvokeStartElement (pctxt, "altGKInfo", -1);

                     stat = asn1PD_H225AltGKInfo (pctxt, &pvalue->altGKInfo);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "altGKInfo", -1);
                     break;

                  case 1:
                     pvalue->m.tokensPresent = 1;

                     rtInvokeStartElement (pctxt, "tokens", -1);

                     stat = asn1PD_H225_SeqOfH225ClearToken (pctxt, &pvalue->tokens);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "tokens", -1);
                     break;

                  case 2:
                     pvalue->m.cryptoTokensPresent = 1;

                     rtInvokeStartElement (pctxt, "cryptoTokens", -1);

                     stat = asn1PD_H225_SeqOfH225CryptoH323Token (pctxt, &pvalue->cryptoTokens);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "cryptoTokens", -1);
                     break;

                  case 3:
                     pvalue->m.integrityCheckValuePresent = 1;

                     rtInvokeStartElement (pctxt, "integrityCheckValue", -1);

                     stat = asn1PD_H225ICV (pctxt, &pvalue->integrityCheckValue);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "integrityCheckValue", -1);
                     break;

                  case 4:
                     pvalue->m.genericDataPresent = 1;

                     rtInvokeStartElement (pctxt, "genericData", -1);

                     stat = asn1PD_H225_SeqOfH225GenericData (pctxt, &pvalue->genericData);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "genericData", -1);
                     break;

                  default:
                     pctxt->buffer.byteIndex += openType.numocts;
               }
               copyContext (pctxt, &lctxt2);
            }
            else {  /* unknown element */
               pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
               pOpenType->numocts = openType.numocts;
               pOpenType->data = openType.data;
               dListAppend (pctxt, &pvalue->extElem1, pOpenType);
            }
         }
         else if (i >= 5) {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  DisengageReason                                           */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225DisengageReason (OOCTXT* pctxt, H225DisengageReason* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT ui;
   ASN1OpenType openType;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   if (!extbit) {
      stat = decodeConsUnsigned (pctxt, &ui, 0, 2);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      else pvalue->t = ui + 1;

      switch (ui) {
         /* forcedDrop */
         case 0:
            rtInvokeStartElement (pctxt, "forcedDrop", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "forcedDrop", -1);

            break;

         /* normalDrop */
         case 1:
            rtInvokeStartElement (pctxt, "normalDrop", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "normalDrop", -1);

            break;

         /* undefinedReason */
         case 2:
            rtInvokeStartElement (pctxt, "undefinedReason", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "undefinedReason", -1);

            break;

         default:
            return LOG_ASN1ERR (pctxt, ASN_E_INVOPT);
      }
   }
   else {
      stat = decodeSmallNonNegWholeNumber (pctxt, &ui);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      else pvalue->t = ui + 4;

      stat = decodeByteAlign (pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      pvalue->u.extElem1 = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
      if (pvalue->u.extElem1 == NULL)
         return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

      pvalue->u.extElem1->data = openType.data;
      pvalue->u.extElem1->numocts = openType.numocts;

   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  DisengageRequest                                          */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225DisengageRequest (OOCTXT* pctxt, H225DisengageRequest* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   OOCTXT lctxt2;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.nonStandardDataPresent = optbit;

   /* decode requestSeqNum */

   rtInvokeStartElement (pctxt, "requestSeqNum", -1);

   stat = asn1PD_H225RequestSeqNum (pctxt, &pvalue->requestSeqNum);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "requestSeqNum", -1);

   /* decode endpointIdentifier */

   rtInvokeStartElement (pctxt, "endpointIdentifier", -1);

   stat = asn1PD_H225EndpointIdentifier (pctxt, &pvalue->endpointIdentifier);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "endpointIdentifier", -1);

   /* decode conferenceID */

   rtInvokeStartElement (pctxt, "conferenceID", -1);

   stat = asn1PD_H225ConferenceIdentifier (pctxt, &pvalue->conferenceID);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "conferenceID", -1);

   /* decode callReferenceValue */

   rtInvokeStartElement (pctxt, "callReferenceValue", -1);

   stat = asn1PD_H225CallReferenceValue (pctxt, &pvalue->callReferenceValue);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "callReferenceValue", -1);

   /* decode disengageReason */

   rtInvokeStartElement (pctxt, "disengageReason", -1);

   stat = asn1PD_H225DisengageReason (pctxt, &pvalue->disengageReason);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "disengageReason", -1);

   /* decode nonStandardData */

   if (pvalue->m.nonStandardDataPresent) {
      rtInvokeStartElement (pctxt, "nonStandardData", -1);

      stat = asn1PD_H225NonStandardParameter (pctxt, &pvalue->nonStandardData);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "nonStandardData", -1);
   }

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            if (i < 13 && openType.numocts > 0) {  /* known element */
               copyContext (&lctxt2, pctxt);
               initContextBuffer (pctxt, openType.data, openType.numocts);

               switch (i) {
                  case 0:
                     pvalue->m.callIdentifierPresent = 1;

                     rtInvokeStartElement (pctxt, "callIdentifier", -1);

                     stat = asn1PD_H225CallIdentifier (pctxt, &pvalue->callIdentifier);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "callIdentifier", -1);
                     break;

                  case 1:
                     pvalue->m.gatekeeperIdentifierPresent = 1;

                     rtInvokeStartElement (pctxt, "gatekeeperIdentifier", -1);

                     stat = asn1PD_H225GatekeeperIdentifier (pctxt, &pvalue->gatekeeperIdentifier);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "gatekeeperIdentifier", -1);
                     break;

                  case 2:
                     pvalue->m.tokensPresent = 1;

                     rtInvokeStartElement (pctxt, "tokens", -1);

                     stat = asn1PD_H225_SeqOfH225ClearToken (pctxt, &pvalue->tokens);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "tokens", -1);
                     break;

                  case 3:
                     pvalue->m.cryptoTokensPresent = 1;

                     rtInvokeStartElement (pctxt, "cryptoTokens", -1);

                     stat = asn1PD_H225_SeqOfH225CryptoH323Token (pctxt, &pvalue->cryptoTokens);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "cryptoTokens", -1);
                     break;

                  case 4:
                     pvalue->m.integrityCheckValuePresent = 1;

                     rtInvokeStartElement (pctxt, "integrityCheckValue", -1);

                     stat = asn1PD_H225ICV (pctxt, &pvalue->integrityCheckValue);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "integrityCheckValue", -1);
                     break;

                  case 5:
                     pvalue->m.answeredCallPresent = 1;

                     rtInvokeStartElement (pctxt, "answeredCall", -1);

                     stat = DECODEBIT (pctxt, &pvalue->answeredCall);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
                     rtInvokeBoolValue (pctxt, pvalue->answeredCall);

                     rtInvokeEndElement (pctxt, "answeredCall", -1);
                     break;

                  case 6:
                     pvalue->m.callLinkagePresent = 1;

                     rtInvokeStartElement (pctxt, "callLinkage", -1);

                     stat = asn1PD_H225CallLinkage (pctxt, &pvalue->callLinkage);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "callLinkage", -1);
                     break;

                  case 7:
                     pvalue->m.capacityPresent = 1;

                     rtInvokeStartElement (pctxt, "capacity", -1);

                     stat = asn1PD_H225CallCapacity (pctxt, &pvalue->capacity);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "capacity", -1);
                     break;

                  case 8:
                     pvalue->m.circuitInfoPresent = 1;

                     rtInvokeStartElement (pctxt, "circuitInfo", -1);

                     stat = asn1PD_H225CircuitInfo (pctxt, &pvalue->circuitInfo);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "circuitInfo", -1);
                     break;

                  case 9:
                     pvalue->m.usageInformationPresent = 1;

                     rtInvokeStartElement (pctxt, "usageInformation", -1);

                     stat = asn1PD_H225RasUsageInformation (pctxt, &pvalue->usageInformation);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "usageInformation", -1);
                     break;

                  case 10:
                     pvalue->m.terminationCausePresent = 1;

                     rtInvokeStartElement (pctxt, "terminationCause", -1);

                     stat = asn1PD_H225CallTerminationCause (pctxt, &pvalue->terminationCause);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "terminationCause", -1);
                     break;

                  case 11:
                     pvalue->m.serviceControlPresent = 1;

                     rtInvokeStartElement (pctxt, "serviceControl", -1);

                     stat = asn1PD_H225_SeqOfH225ServiceControlSession (pctxt, &pvalue->serviceControl);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "serviceControl", -1);
                     break;

                  case 12:
                     pvalue->m.genericDataPresent = 1;

                     rtInvokeStartElement (pctxt, "genericData", -1);

                     stat = asn1PD_H225_SeqOfH225GenericData (pctxt, &pvalue->genericData);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "genericData", -1);
                     break;

                  default:
                     pctxt->buffer.byteIndex += openType.numocts;
               }
               copyContext (pctxt, &lctxt2);
            }
            else {  /* unknown element */
               pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
               pOpenType->numocts = openType.numocts;
               pOpenType->data = openType.data;
               dListAppend (pctxt, &pvalue->extElem1, pOpenType);
            }
         }
         else if (i >= 13) {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  DisengageConfirm                                          */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225DisengageConfirm (OOCTXT* pctxt, H225DisengageConfirm* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   OOCTXT lctxt2;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.nonStandardDataPresent = optbit;

   /* decode requestSeqNum */

   rtInvokeStartElement (pctxt, "requestSeqNum", -1);

   stat = asn1PD_H225RequestSeqNum (pctxt, &pvalue->requestSeqNum);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "requestSeqNum", -1);

   /* decode nonStandardData */

   if (pvalue->m.nonStandardDataPresent) {
      rtInvokeStartElement (pctxt, "nonStandardData", -1);

      stat = asn1PD_H225NonStandardParameter (pctxt, &pvalue->nonStandardData);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "nonStandardData", -1);
   }

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            if (i < 7 && openType.numocts > 0) {  /* known element */
               copyContext (&lctxt2, pctxt);
               initContextBuffer (pctxt, openType.data, openType.numocts);

               switch (i) {
                  case 0:
                     pvalue->m.tokensPresent = 1;

                     rtInvokeStartElement (pctxt, "tokens", -1);

                     stat = asn1PD_H225_SeqOfH225ClearToken (pctxt, &pvalue->tokens);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "tokens", -1);
                     break;

                  case 1:
                     pvalue->m.cryptoTokensPresent = 1;

                     rtInvokeStartElement (pctxt, "cryptoTokens", -1);

                     stat = asn1PD_H225_SeqOfH225CryptoH323Token (pctxt, &pvalue->cryptoTokens);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "cryptoTokens", -1);
                     break;

                  case 2:
                     pvalue->m.integrityCheckValuePresent = 1;

                     rtInvokeStartElement (pctxt, "integrityCheckValue", -1);

                     stat = asn1PD_H225ICV (pctxt, &pvalue->integrityCheckValue);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "integrityCheckValue", -1);
                     break;

                  case 3:
                     pvalue->m.capacityPresent = 1;

                     rtInvokeStartElement (pctxt, "capacity", -1);

                     stat = asn1PD_H225CallCapacity (pctxt, &pvalue->capacity);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "capacity", -1);
                     break;

                  case 4:
                     pvalue->m.circuitInfoPresent = 1;

                     rtInvokeStartElement (pctxt, "circuitInfo", -1);

                     stat = asn1PD_H225CircuitInfo (pctxt, &pvalue->circuitInfo);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "circuitInfo", -1);
                     break;

                  case 5:
                     pvalue->m.usageInformationPresent = 1;

                     rtInvokeStartElement (pctxt, "usageInformation", -1);

                     stat = asn1PD_H225RasUsageInformation (pctxt, &pvalue->usageInformation);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "usageInformation", -1);
                     break;

                  case 6:
                     pvalue->m.genericDataPresent = 1;

                     rtInvokeStartElement (pctxt, "genericData", -1);

                     stat = asn1PD_H225_SeqOfH225GenericData (pctxt, &pvalue->genericData);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "genericData", -1);
                     break;

                  default:
                     pctxt->buffer.byteIndex += openType.numocts;
               }
               copyContext (pctxt, &lctxt2);
            }
            else {  /* unknown element */
               pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
               pOpenType->numocts = openType.numocts;
               pOpenType->data = openType.data;
               dListAppend (pctxt, &pvalue->extElem1, pOpenType);
            }
         }
         else if (i >= 7) {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  DisengageRejectReason                                     */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225DisengageRejectReason (OOCTXT* pctxt, H225DisengageRejectReason* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT ui;
   ASN1OpenType openType;
   ASN1BOOL extbit;
   OOCTXT lctxt;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   if (!extbit) {
      stat = decodeConsUnsigned (pctxt, &ui, 0, 1);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      else pvalue->t = ui + 1;

      switch (ui) {
         /* notRegistered */
         case 0:
            rtInvokeStartElement (pctxt, "notRegistered", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "notRegistered", -1);

            break;

         /* requestToDropOther */
         case 1:
            rtInvokeStartElement (pctxt, "requestToDropOther", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "requestToDropOther", -1);

            break;

         default:
            return LOG_ASN1ERR (pctxt, ASN_E_INVOPT);
      }
   }
   else {
      stat = decodeSmallNonNegWholeNumber (pctxt, &ui);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      else pvalue->t = ui + 3;

      stat = decodeByteAlign (pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      copyContext (&lctxt, pctxt);
      initContextBuffer (pctxt, openType.data, openType.numocts);

      switch (pvalue->t) {
         /* securityDenial */
         case 3:
            rtInvokeStartElement (pctxt, "securityDenial", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "securityDenial", -1);

            break;

         /* securityError */
         case 4:
            rtInvokeStartElement (pctxt, "securityError", -1);

            pvalue->u.securityError = ALLOC_ASN1ELEM (pctxt, H225SecurityErrors2);
            if (pvalue->u.securityError == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225SecurityErrors2 (pctxt, pvalue->u.securityError);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "securityError", -1);

            break;

         default:
            pvalue->u.extElem1 = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
            if (pvalue->u.extElem1 == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            pvalue->u.extElem1->data = openType.data;
            pvalue->u.extElem1->numocts = openType.numocts;

      }

      copyContext (pctxt, &lctxt);
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  DisengageReject                                           */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225DisengageReject (OOCTXT* pctxt, H225DisengageReject* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   OOCTXT lctxt2;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.nonStandardDataPresent = optbit;

   /* decode requestSeqNum */

   rtInvokeStartElement (pctxt, "requestSeqNum", -1);

   stat = asn1PD_H225RequestSeqNum (pctxt, &pvalue->requestSeqNum);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "requestSeqNum", -1);

   /* decode rejectReason */

   rtInvokeStartElement (pctxt, "rejectReason", -1);

   stat = asn1PD_H225DisengageRejectReason (pctxt, &pvalue->rejectReason);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "rejectReason", -1);

   /* decode nonStandardData */

   if (pvalue->m.nonStandardDataPresent) {
      rtInvokeStartElement (pctxt, "nonStandardData", -1);

      stat = asn1PD_H225NonStandardParameter (pctxt, &pvalue->nonStandardData);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "nonStandardData", -1);
   }

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            if (i < 5 && openType.numocts > 0) {  /* known element */
               copyContext (&lctxt2, pctxt);
               initContextBuffer (pctxt, openType.data, openType.numocts);

               switch (i) {
                  case 0:
                     pvalue->m.altGKInfoPresent = 1;

                     rtInvokeStartElement (pctxt, "altGKInfo", -1);

                     stat = asn1PD_H225AltGKInfo (pctxt, &pvalue->altGKInfo);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "altGKInfo", -1);
                     break;

                  case 1:
                     pvalue->m.tokensPresent = 1;

                     rtInvokeStartElement (pctxt, "tokens", -1);

                     stat = asn1PD_H225_SeqOfH225ClearToken (pctxt, &pvalue->tokens);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "tokens", -1);
                     break;

                  case 2:
                     pvalue->m.cryptoTokensPresent = 1;

                     rtInvokeStartElement (pctxt, "cryptoTokens", -1);

                     stat = asn1PD_H225_SeqOfH225CryptoH323Token (pctxt, &pvalue->cryptoTokens);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "cryptoTokens", -1);
                     break;

                  case 3:
                     pvalue->m.integrityCheckValuePresent = 1;

                     rtInvokeStartElement (pctxt, "integrityCheckValue", -1);

                     stat = asn1PD_H225ICV (pctxt, &pvalue->integrityCheckValue);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "integrityCheckValue", -1);
                     break;

                  case 4:
                     pvalue->m.genericDataPresent = 1;

                     rtInvokeStartElement (pctxt, "genericData", -1);

                     stat = asn1PD_H225_SeqOfH225GenericData (pctxt, &pvalue->genericData);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "genericData", -1);
                     break;

                  default:
                     pctxt->buffer.byteIndex += openType.numocts;
               }
               copyContext (pctxt, &lctxt2);
            }
            else {  /* unknown element */
               pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
               pOpenType->numocts = openType.numocts;
               pOpenType->data = openType.data;
               dListAppend (pctxt, &pvalue->extElem1, pOpenType);
            }
         }
         else if (i >= 5) {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  LocationRequest                                           */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225LocationRequest (OOCTXT* pctxt, H225LocationRequest* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   OOCTXT lctxt2;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.endpointIdentifierPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.nonStandardDataPresent = optbit;

   /* decode requestSeqNum */

   rtInvokeStartElement (pctxt, "requestSeqNum", -1);

   stat = asn1PD_H225RequestSeqNum (pctxt, &pvalue->requestSeqNum);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "requestSeqNum", -1);

   /* decode endpointIdentifier */

   if (pvalue->m.endpointIdentifierPresent) {
      rtInvokeStartElement (pctxt, "endpointIdentifier", -1);

      stat = asn1PD_H225EndpointIdentifier (pctxt, &pvalue->endpointIdentifier);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "endpointIdentifier", -1);
   }

   /* decode destinationInfo */

   rtInvokeStartElement (pctxt, "destinationInfo", -1);

   stat = asn1PD_H225_SeqOfH225AliasAddress (pctxt, &pvalue->destinationInfo);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "destinationInfo", -1);

   /* decode nonStandardData */

   if (pvalue->m.nonStandardDataPresent) {
      rtInvokeStartElement (pctxt, "nonStandardData", -1);

      stat = asn1PD_H225NonStandardParameter (pctxt, &pvalue->nonStandardData);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "nonStandardData", -1);
   }

   /* decode replyAddress */

   rtInvokeStartElement (pctxt, "replyAddress", -1);

   stat = asn1PD_H225TransportAddress (pctxt, &pvalue->replyAddress);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "replyAddress", -1);

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            if (i < 16 && openType.numocts > 0) {  /* known element */
               copyContext (&lctxt2, pctxt);
               initContextBuffer (pctxt, openType.data, openType.numocts);

               switch (i) {
                  case 0:
                     pvalue->m.sourceInfoPresent = 1;

                     rtInvokeStartElement (pctxt, "sourceInfo", -1);

                     stat = asn1PD_H225_SeqOfH225AliasAddress (pctxt, &pvalue->sourceInfo);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "sourceInfo", -1);
                     break;

                  case 1:
                     pvalue->m.canMapAliasPresent = 1;

                     rtInvokeStartElement (pctxt, "canMapAlias", -1);

                     stat = DECODEBIT (pctxt, &pvalue->canMapAlias);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
                     rtInvokeBoolValue (pctxt, pvalue->canMapAlias);

                     rtInvokeEndElement (pctxt, "canMapAlias", -1);
                     break;

                  case 2:
                     pvalue->m.gatekeeperIdentifierPresent = 1;

                     rtInvokeStartElement (pctxt, "gatekeeperIdentifier", -1);

                     stat = asn1PD_H225GatekeeperIdentifier (pctxt, &pvalue->gatekeeperIdentifier);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "gatekeeperIdentifier", -1);
                     break;

                  case 3:
                     pvalue->m.tokensPresent = 1;

                     rtInvokeStartElement (pctxt, "tokens", -1);

                     stat = asn1PD_H225_SeqOfH225ClearToken (pctxt, &pvalue->tokens);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "tokens", -1);
                     break;

                  case 4:
                     pvalue->m.cryptoTokensPresent = 1;

                     rtInvokeStartElement (pctxt, "cryptoTokens", -1);

                     stat = asn1PD_H225_SeqOfH225CryptoH323Token (pctxt, &pvalue->cryptoTokens);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "cryptoTokens", -1);
                     break;

                  case 5:
                     pvalue->m.integrityCheckValuePresent = 1;

                     rtInvokeStartElement (pctxt, "integrityCheckValue", -1);

                     stat = asn1PD_H225ICV (pctxt, &pvalue->integrityCheckValue);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "integrityCheckValue", -1);
                     break;

                  case 6:
                     pvalue->m.desiredProtocolsPresent = 1;

                     rtInvokeStartElement (pctxt, "desiredProtocols", -1);

                     stat = asn1PD_H225_SeqOfH225SupportedProtocols (pctxt, &pvalue->desiredProtocols);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "desiredProtocols", -1);
                     break;

                  case 7:
                     pvalue->m.desiredTunnelledProtocolPresent = 1;

                     rtInvokeStartElement (pctxt, "desiredTunnelledProtocol", -1);

                     stat = asn1PD_H225TunnelledProtocol (pctxt, &pvalue->desiredTunnelledProtocol);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "desiredTunnelledProtocol", -1);
                     break;

                  case 8:
                     pvalue->m.featureSetPresent = 1;

                     rtInvokeStartElement (pctxt, "featureSet", -1);

                     stat = asn1PD_H225FeatureSet (pctxt, &pvalue->featureSet);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "featureSet", -1);
                     break;

                  case 9:
                     pvalue->m.genericDataPresent = 1;

                     rtInvokeStartElement (pctxt, "genericData", -1);

                     stat = asn1PD_H225_SeqOfH225GenericData (pctxt, &pvalue->genericData);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "genericData", -1);
                     break;

                  case 10:
                     pvalue->m.hopCountPresent = 1;

                     rtInvokeStartElement (pctxt, "hopCount", -1);

                     stat = decodeConsUInt8 (pctxt, &pvalue->hopCount, 1U, 255U);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
                     rtInvokeUIntValue (pctxt, pvalue->hopCount);

                     rtInvokeEndElement (pctxt, "hopCount", -1);
                     break;

                  case 11:
                     pvalue->m.circuitInfoPresent = 1;

                     rtInvokeStartElement (pctxt, "circuitInfo", -1);

                     stat = asn1PD_H225CircuitInfo (pctxt, &pvalue->circuitInfo);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "circuitInfo", -1);
                     break;

                  case 12:
                     pvalue->m.callIdentifierPresent = 1;

                     rtInvokeStartElement (pctxt, "callIdentifier", -1);

                     stat = asn1PD_H225CallIdentifier (pctxt, &pvalue->callIdentifier);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "callIdentifier", -1);
                     break;

                  case 13:
                     pvalue->m.bandWidthPresent = 1;

                     rtInvokeStartElement (pctxt, "bandWidth", -1);

                     stat = asn1PD_H225BandWidth (pctxt, &pvalue->bandWidth);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "bandWidth", -1);
                     break;

                  case 14:
                     pvalue->m.sourceEndpointInfoPresent = 1;

                     rtInvokeStartElement (pctxt, "sourceEndpointInfo", -1);

                     stat = asn1PD_H225_SeqOfH225AliasAddress (pctxt, &pvalue->sourceEndpointInfo);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "sourceEndpointInfo", -1);
                     break;

                  case 15:
                     pvalue->m.canMapSrcAliasPresent = 1;

                     rtInvokeStartElement (pctxt, "canMapSrcAlias", -1);

                     stat = DECODEBIT (pctxt, &pvalue->canMapSrcAlias);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
                     rtInvokeBoolValue (pctxt, pvalue->canMapSrcAlias);

                     rtInvokeEndElement (pctxt, "canMapSrcAlias", -1);
                     break;

                  default:
                     pctxt->buffer.byteIndex += openType.numocts;
               }
               copyContext (pctxt, &lctxt2);
            }
            else {  /* unknown element */
               pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
               pOpenType->numocts = openType.numocts;
               pOpenType->data = openType.data;
               dListAppend (pctxt, &pvalue->extElem1, pOpenType);
            }
         }
         else if (i >= 16) {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  LocationConfirm                                           */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225LocationConfirm (OOCTXT* pctxt, H225LocationConfirm* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   OOCTXT lctxt2;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.nonStandardDataPresent = optbit;

   /* decode requestSeqNum */

   rtInvokeStartElement (pctxt, "requestSeqNum", -1);

   stat = asn1PD_H225RequestSeqNum (pctxt, &pvalue->requestSeqNum);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "requestSeqNum", -1);

   /* decode callSignalAddress */

   rtInvokeStartElement (pctxt, "callSignalAddress", -1);

   stat = asn1PD_H225TransportAddress (pctxt, &pvalue->callSignalAddress);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "callSignalAddress", -1);

   /* decode rasAddress */

   rtInvokeStartElement (pctxt, "rasAddress", -1);

   stat = asn1PD_H225TransportAddress (pctxt, &pvalue->rasAddress);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "rasAddress", -1);

   /* decode nonStandardData */

   if (pvalue->m.nonStandardDataPresent) {
      rtInvokeStartElement (pctxt, "nonStandardData", -1);

      stat = asn1PD_H225NonStandardParameter (pctxt, &pvalue->nonStandardData);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "nonStandardData", -1);
   }

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            if (i < 17 && openType.numocts > 0) {  /* known element */
               copyContext (&lctxt2, pctxt);
               initContextBuffer (pctxt, openType.data, openType.numocts);

               switch (i) {
                  case 0:
                     pvalue->m.destinationInfoPresent = 1;

                     rtInvokeStartElement (pctxt, "destinationInfo", -1);

                     stat = asn1PD_H225_SeqOfH225AliasAddress (pctxt, &pvalue->destinationInfo);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "destinationInfo", -1);
                     break;

                  case 1:
                     pvalue->m.destExtraCallInfoPresent = 1;

                     rtInvokeStartElement (pctxt, "destExtraCallInfo", -1);

                     stat = asn1PD_H225_SeqOfH225AliasAddress (pctxt, &pvalue->destExtraCallInfo);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "destExtraCallInfo", -1);
                     break;

                  case 2:
                     pvalue->m.destinationTypePresent = 1;

                     rtInvokeStartElement (pctxt, "destinationType", -1);

                     stat = asn1PD_H225EndpointType (pctxt, &pvalue->destinationType);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "destinationType", -1);
                     break;

                  case 3:
                     pvalue->m.remoteExtensionAddressPresent = 1;

                     rtInvokeStartElement (pctxt, "remoteExtensionAddress", -1);

                     stat = asn1PD_H225_SeqOfH225AliasAddress (pctxt, &pvalue->remoteExtensionAddress);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "remoteExtensionAddress", -1);
                     break;

                  case 4:
                     pvalue->m.alternateEndpointsPresent = 1;

                     rtInvokeStartElement (pctxt, "alternateEndpoints", -1);

                     stat = asn1PD_H225_SeqOfH225Endpoint (pctxt, &pvalue->alternateEndpoints);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "alternateEndpoints", -1);
                     break;

                  case 5:
                     pvalue->m.tokensPresent = 1;

                     rtInvokeStartElement (pctxt, "tokens", -1);

                     stat = asn1PD_H225_SeqOfH225ClearToken (pctxt, &pvalue->tokens);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "tokens", -1);
                     break;

                  case 6:
                     pvalue->m.cryptoTokensPresent = 1;

                     rtInvokeStartElement (pctxt, "cryptoTokens", -1);

                     stat = asn1PD_H225_SeqOfH225CryptoH323Token (pctxt, &pvalue->cryptoTokens);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "cryptoTokens", -1);
                     break;

                  case 7:
                     pvalue->m.integrityCheckValuePresent = 1;

                     rtInvokeStartElement (pctxt, "integrityCheckValue", -1);

                     stat = asn1PD_H225ICV (pctxt, &pvalue->integrityCheckValue);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "integrityCheckValue", -1);
                     break;

                  case 8:
                     pvalue->m.alternateTransportAddressesPresent = 1;

                     rtInvokeStartElement (pctxt, "alternateTransportAddresses", -1);

                     stat = asn1PD_H225AlternateTransportAddresses (pctxt, &pvalue->alternateTransportAddresses);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "alternateTransportAddresses", -1);
                     break;

                  case 9:
                     pvalue->m.supportedProtocolsPresent = 1;

                     rtInvokeStartElement (pctxt, "supportedProtocols", -1);

                     stat = asn1PD_H225_SeqOfH225SupportedProtocols (pctxt, &pvalue->supportedProtocols);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "supportedProtocols", -1);
                     break;

                  case 10:
                     pvalue->m.multipleCallsPresent = 1;

                     rtInvokeStartElement (pctxt, "multipleCalls", -1);

                     stat = DECODEBIT (pctxt, &pvalue->multipleCalls);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
                     rtInvokeBoolValue (pctxt, pvalue->multipleCalls);

                     rtInvokeEndElement (pctxt, "multipleCalls", -1);
                     break;

                  case 11:
                     pvalue->m.featureSetPresent = 1;

                     rtInvokeStartElement (pctxt, "featureSet", -1);

                     stat = asn1PD_H225FeatureSet (pctxt, &pvalue->featureSet);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "featureSet", -1);
                     break;

                  case 12:
                     pvalue->m.genericDataPresent = 1;

                     rtInvokeStartElement (pctxt, "genericData", -1);

                     stat = asn1PD_H225_SeqOfH225GenericData (pctxt, &pvalue->genericData);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "genericData", -1);
                     break;

                  case 13:
                     pvalue->m.circuitInfoPresent = 1;

                     rtInvokeStartElement (pctxt, "circuitInfo", -1);

                     stat = asn1PD_H225CircuitInfo (pctxt, &pvalue->circuitInfo);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "circuitInfo", -1);
                     break;

                  case 14:
                     pvalue->m.serviceControlPresent = 1;

                     rtInvokeStartElement (pctxt, "serviceControl", -1);

                     stat = asn1PD_H225_SeqOfH225ServiceControlSession (pctxt, &pvalue->serviceControl);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "serviceControl", -1);
                     break;

                  case 15:
                     pvalue->m.modifiedSrcInfoPresent = 1;

                     rtInvokeStartElement (pctxt, "modifiedSrcInfo", -1);

                     stat = asn1PD_H225_SeqOfH225AliasAddress (pctxt, &pvalue->modifiedSrcInfo);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "modifiedSrcInfo", -1);
                     break;

                  case 16:
                     pvalue->m.bandWidthPresent = 1;

                     rtInvokeStartElement (pctxt, "bandWidth", -1);

                     stat = asn1PD_H225BandWidth (pctxt, &pvalue->bandWidth);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "bandWidth", -1);
                     break;

                  default:
                     pctxt->buffer.byteIndex += openType.numocts;
               }
               copyContext (pctxt, &lctxt2);
            }
            else {  /* unknown element */
               pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
               pOpenType->numocts = openType.numocts;
               pOpenType->data = openType.data;
               dListAppend (pctxt, &pvalue->extElem1, pOpenType);
            }
         }
         else if (i >= 17) {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  LocationRejectReason                                      */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225LocationRejectReason (OOCTXT* pctxt, H225LocationRejectReason* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT ui;
   ASN1OpenType openType;
   ASN1BOOL extbit;
   OOCTXT lctxt;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   if (!extbit) {
      stat = decodeConsUnsigned (pctxt, &ui, 0, 3);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      else pvalue->t = ui + 1;

      switch (ui) {
         /* notRegistered */
         case 0:
            rtInvokeStartElement (pctxt, "notRegistered", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "notRegistered", -1);

            break;

         /* invalidPermission */
         case 1:
            rtInvokeStartElement (pctxt, "invalidPermission", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "invalidPermission", -1);

            break;

         /* requestDenied */
         case 2:
            rtInvokeStartElement (pctxt, "requestDenied", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "requestDenied", -1);

            break;

         /* undefinedReason */
         case 3:
            rtInvokeStartElement (pctxt, "undefinedReason", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "undefinedReason", -1);

            break;

         default:
            return LOG_ASN1ERR (pctxt, ASN_E_INVOPT);
      }
   }
   else {
      stat = decodeSmallNonNegWholeNumber (pctxt, &ui);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      else pvalue->t = ui + 5;

      stat = decodeByteAlign (pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      copyContext (&lctxt, pctxt);
      initContextBuffer (pctxt, openType.data, openType.numocts);

      switch (pvalue->t) {
         /* securityDenial */
         case 5:
            rtInvokeStartElement (pctxt, "securityDenial", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "securityDenial", -1);

            break;

         /* aliasesInconsistent */
         case 6:
            rtInvokeStartElement (pctxt, "aliasesInconsistent", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "aliasesInconsistent", -1);

            break;

         /* routeCalltoSCN */
         case 7:
            rtInvokeStartElement (pctxt, "routeCalltoSCN", -1);

            pvalue->u.routeCalltoSCN = ALLOC_ASN1ELEM (pctxt, H225_SeqOfH225PartyNumber);
            if (pvalue->u.routeCalltoSCN == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225_SeqOfH225PartyNumber (pctxt, pvalue->u.routeCalltoSCN);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "routeCalltoSCN", -1);

            break;

         /* resourceUnavailable */
         case 8:
            rtInvokeStartElement (pctxt, "resourceUnavailable", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "resourceUnavailable", -1);

            break;

         /* genericDataReason */
         case 9:
            rtInvokeStartElement (pctxt, "genericDataReason", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "genericDataReason", -1);

            break;

         /* neededFeatureNotSupported */
         case 10:
            rtInvokeStartElement (pctxt, "neededFeatureNotSupported", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "neededFeatureNotSupported", -1);

            break;

         /* hopCountExceeded */
         case 11:
            rtInvokeStartElement (pctxt, "hopCountExceeded", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "hopCountExceeded", -1);

            break;

         /* incompleteAddress */
         case 12:
            rtInvokeStartElement (pctxt, "incompleteAddress", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "incompleteAddress", -1);

            break;

         /* securityError */
         case 13:
            rtInvokeStartElement (pctxt, "securityError", -1);

            pvalue->u.securityError = ALLOC_ASN1ELEM (pctxt, H225SecurityErrors2);
            if (pvalue->u.securityError == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225SecurityErrors2 (pctxt, pvalue->u.securityError);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "securityError", -1);

            break;

         /* securityDHmismatch */
         case 14:
            rtInvokeStartElement (pctxt, "securityDHmismatch", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "securityDHmismatch", -1);

            break;

         /* noRouteToDestination */
         case 15:
            rtInvokeStartElement (pctxt, "noRouteToDestination", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "noRouteToDestination", -1);

            break;

         /* unallocatedNumber */
         case 16:
            rtInvokeStartElement (pctxt, "unallocatedNumber", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "unallocatedNumber", -1);

            break;

         default:
            pvalue->u.extElem1 = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
            if (pvalue->u.extElem1 == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            pvalue->u.extElem1->data = openType.data;
            pvalue->u.extElem1->numocts = openType.numocts;

      }

      copyContext (pctxt, &lctxt);
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  LocationReject                                            */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225LocationReject (OOCTXT* pctxt, H225LocationReject* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   OOCTXT lctxt2;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.nonStandardDataPresent = optbit;

   /* decode requestSeqNum */

   rtInvokeStartElement (pctxt, "requestSeqNum", -1);

   stat = asn1PD_H225RequestSeqNum (pctxt, &pvalue->requestSeqNum);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "requestSeqNum", -1);

   /* decode rejectReason */

   rtInvokeStartElement (pctxt, "rejectReason", -1);

   stat = asn1PD_H225LocationRejectReason (pctxt, &pvalue->rejectReason);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "rejectReason", -1);

   /* decode nonStandardData */

   if (pvalue->m.nonStandardDataPresent) {
      rtInvokeStartElement (pctxt, "nonStandardData", -1);

      stat = asn1PD_H225NonStandardParameter (pctxt, &pvalue->nonStandardData);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "nonStandardData", -1);
   }

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            if (i < 7 && openType.numocts > 0) {  /* known element */
               copyContext (&lctxt2, pctxt);
               initContextBuffer (pctxt, openType.data, openType.numocts);

               switch (i) {
                  case 0:
                     pvalue->m.altGKInfoPresent = 1;

                     rtInvokeStartElement (pctxt, "altGKInfo", -1);

                     stat = asn1PD_H225AltGKInfo (pctxt, &pvalue->altGKInfo);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "altGKInfo", -1);
                     break;

                  case 1:
                     pvalue->m.tokensPresent = 1;

                     rtInvokeStartElement (pctxt, "tokens", -1);

                     stat = asn1PD_H225_SeqOfH225ClearToken (pctxt, &pvalue->tokens);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "tokens", -1);
                     break;

                  case 2:
                     pvalue->m.cryptoTokensPresent = 1;

                     rtInvokeStartElement (pctxt, "cryptoTokens", -1);

                     stat = asn1PD_H225_SeqOfH225CryptoH323Token (pctxt, &pvalue->cryptoTokens);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "cryptoTokens", -1);
                     break;

                  case 3:
                     pvalue->m.integrityCheckValuePresent = 1;

                     rtInvokeStartElement (pctxt, "integrityCheckValue", -1);

                     stat = asn1PD_H225ICV (pctxt, &pvalue->integrityCheckValue);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "integrityCheckValue", -1);
                     break;

                  case 4:
                     pvalue->m.featureSetPresent = 1;

                     rtInvokeStartElement (pctxt, "featureSet", -1);

                     stat = asn1PD_H225FeatureSet (pctxt, &pvalue->featureSet);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "featureSet", -1);
                     break;

                  case 5:
                     pvalue->m.genericDataPresent = 1;

                     rtInvokeStartElement (pctxt, "genericData", -1);

                     stat = asn1PD_H225_SeqOfH225GenericData (pctxt, &pvalue->genericData);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "genericData", -1);
                     break;

                  case 6:
                     pvalue->m.serviceControlPresent = 1;

                     rtInvokeStartElement (pctxt, "serviceControl", -1);

                     stat = asn1PD_H225_SeqOfH225ServiceControlSession (pctxt, &pvalue->serviceControl);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "serviceControl", -1);
                     break;

                  default:
                     pctxt->buffer.byteIndex += openType.numocts;
               }
               copyContext (pctxt, &lctxt2);
            }
            else {  /* unknown element */
               pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
               pOpenType->numocts = openType.numocts;
               pOpenType->data = openType.data;
               dListAppend (pctxt, &pvalue->extElem1, pOpenType);
            }
         }
         else if (i >= 7) {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  InfoRequest                                               */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225InfoRequest (OOCTXT* pctxt, H225InfoRequest* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   OOCTXT lctxt2;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.nonStandardDataPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.replyAddressPresent = optbit;

   /* decode requestSeqNum */

   rtInvokeStartElement (pctxt, "requestSeqNum", -1);

   stat = asn1PD_H225RequestSeqNum (pctxt, &pvalue->requestSeqNum);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "requestSeqNum", -1);

   /* decode callReferenceValue */

   rtInvokeStartElement (pctxt, "callReferenceValue", -1);

   stat = asn1PD_H225CallReferenceValue (pctxt, &pvalue->callReferenceValue);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "callReferenceValue", -1);

   /* decode nonStandardData */

   if (pvalue->m.nonStandardDataPresent) {
      rtInvokeStartElement (pctxt, "nonStandardData", -1);

      stat = asn1PD_H225NonStandardParameter (pctxt, &pvalue->nonStandardData);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "nonStandardData", -1);
   }

   /* decode replyAddress */

   if (pvalue->m.replyAddressPresent) {
      rtInvokeStartElement (pctxt, "replyAddress", -1);

      stat = asn1PD_H225TransportAddress (pctxt, &pvalue->replyAddress);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "replyAddress", -1);
   }

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            if (i < 11 && openType.numocts > 0) {  /* known element */
               copyContext (&lctxt2, pctxt);
               initContextBuffer (pctxt, openType.data, openType.numocts);

               switch (i) {
                  case 0:
                     pvalue->m.callIdentifierPresent = 1;

                     rtInvokeStartElement (pctxt, "callIdentifier", -1);

                     stat = asn1PD_H225CallIdentifier (pctxt, &pvalue->callIdentifier);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "callIdentifier", -1);
                     break;

                  case 1:
                     pvalue->m.tokensPresent = 1;

                     rtInvokeStartElement (pctxt, "tokens", -1);

                     stat = asn1PD_H225_SeqOfH225ClearToken (pctxt, &pvalue->tokens);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "tokens", -1);
                     break;

                  case 2:
                     pvalue->m.cryptoTokensPresent = 1;

                     rtInvokeStartElement (pctxt, "cryptoTokens", -1);

                     stat = asn1PD_H225_SeqOfH225CryptoH323Token (pctxt, &pvalue->cryptoTokens);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "cryptoTokens", -1);
                     break;

                  case 3:
                     pvalue->m.integrityCheckValuePresent = 1;

                     rtInvokeStartElement (pctxt, "integrityCheckValue", -1);

                     stat = asn1PD_H225ICV (pctxt, &pvalue->integrityCheckValue);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "integrityCheckValue", -1);
                     break;

                  case 4:
                     pvalue->m.uuiesRequestedPresent = 1;

                     rtInvokeStartElement (pctxt, "uuiesRequested", -1);

                     stat = asn1PD_H225UUIEsRequested (pctxt, &pvalue->uuiesRequested);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "uuiesRequested", -1);
                     break;

                  case 5:
                     pvalue->m.callLinkagePresent = 1;

                     rtInvokeStartElement (pctxt, "callLinkage", -1);

                     stat = asn1PD_H225CallLinkage (pctxt, &pvalue->callLinkage);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "callLinkage", -1);
                     break;

                  case 6:
                     pvalue->m.usageInfoRequestedPresent = 1;

                     rtInvokeStartElement (pctxt, "usageInfoRequested", -1);

                     stat = asn1PD_H225RasUsageInfoTypes (pctxt, &pvalue->usageInfoRequested);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "usageInfoRequested", -1);
                     break;

                  case 7:
                     pvalue->m.segmentedResponseSupportedPresent = 1;

                     rtInvokeStartElement (pctxt, "segmentedResponseSupported", -1);

                     /* NULL */
                     rtInvokeNullValue (pctxt);

                     rtInvokeEndElement (pctxt, "segmentedResponseSupported", -1);
                     break;

                  case 8:
                     pvalue->m.nextSegmentRequestedPresent = 1;

                     rtInvokeStartElement (pctxt, "nextSegmentRequested", -1);

                     stat = decodeConsUInt16 (pctxt, &pvalue->nextSegmentRequested, 0U, 65535U);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
                     rtInvokeUIntValue (pctxt, pvalue->nextSegmentRequested);

                     rtInvokeEndElement (pctxt, "nextSegmentRequested", -1);
                     break;

                  case 9:
                     pvalue->m.capacityInfoRequestedPresent = 1;

                     rtInvokeStartElement (pctxt, "capacityInfoRequested", -1);

                     /* NULL */
                     rtInvokeNullValue (pctxt);

                     rtInvokeEndElement (pctxt, "capacityInfoRequested", -1);
                     break;

                  case 10:
                     pvalue->m.genericDataPresent = 1;

                     rtInvokeStartElement (pctxt, "genericData", -1);

                     stat = asn1PD_H225_SeqOfH225GenericData (pctxt, &pvalue->genericData);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "genericData", -1);
                     break;

                  default:
                     pctxt->buffer.byteIndex += openType.numocts;
               }
               copyContext (pctxt, &lctxt2);
            }
            else {  /* unknown element */
               pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
               pOpenType->numocts = openType.numocts;
               pOpenType->data = openType.data;
               dListAppend (pctxt, &pvalue->extElem1, pOpenType);
            }
         }
         else if (i >= 11) {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  _SeqOfH225RTPSession                                      */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225_SeqOfH225RTPSession (OOCTXT* pctxt, H225_SeqOfH225RTPSession* pvalue)
{
   int stat = ASN_OK;
   H225RTPSession* pdata;
   ASN1UINT count = 0;
   ASN1UINT xx1;
   int lstat;

   dListInit (pvalue);

   for (;;) {
      /* decode length determinant */

      lstat = decodeLength (pctxt, &count);
      if (lstat != ASN_OK && lstat != ASN_OK_FRAG) {
         return LOG_ASN1ERR (pctxt, lstat);
      }

      /* decode elements */

      for (xx1 = 0; xx1 < count; xx1++) {
         rtInvokeStartElement (pctxt, "elem", xx1);

         pdata = ALLOC_ASN1ELEMDNODE (pctxt, H225RTPSession);
         if (pdata == NULL)
            return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

         stat = asn1PD_H225RTPSession (pctxt, pdata);
         if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
         rtInvokeEndElement (pctxt, "elem", xx1);

         dListAppendNode (pctxt, pvalue, pdata);
      }

      if(lstat == ASN_OK) break;
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  _SeqOfH225TransportChannelInfo                            */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225_SeqOfH225TransportChannelInfo (OOCTXT* pctxt, H225_SeqOfH225TransportChannelInfo* pvalue)
{
   int stat = ASN_OK;
   H225TransportChannelInfo* pdata;
   ASN1UINT count = 0;
   ASN1UINT xx1;
   int lstat;

   dListInit (pvalue);

   for (;;) {
      /* decode length determinant */

      lstat = decodeLength (pctxt, &count);
      if (lstat != ASN_OK && lstat != ASN_OK_FRAG) {
         return LOG_ASN1ERR (pctxt, lstat);
      }

      /* decode elements */

      for (xx1 = 0; xx1 < count; xx1++) {
         rtInvokeStartElement (pctxt, "elem", xx1);

         pdata = ALLOC_ASN1ELEMDNODE (pctxt, H225TransportChannelInfo);
         if (pdata == NULL)
            return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

         stat = asn1PD_H225TransportChannelInfo (pctxt, pdata);
         if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
         rtInvokeEndElement (pctxt, "elem", xx1);

         dListAppendNode (pctxt, pvalue, pdata);
      }

      if(lstat == ASN_OK) break;
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  _SeqOfH225ConferenceIdentifier                            */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225_SeqOfH225ConferenceIdentifier (OOCTXT* pctxt, H225_SeqOfH225ConferenceIdentifier* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT xx1;

   /* decode length determinant */

   stat = decodeLength (pctxt, &pvalue->n);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* decode elements */

   ALLOC_ASN1ARRAY (pctxt, pvalue, H225ConferenceIdentifier);

   for (xx1 = 0; xx1 < pvalue->n; xx1++) {
      rtInvokeStartElement (pctxt, "elem", xx1);

      stat = asn1PD_H225ConferenceIdentifier (pctxt, &pvalue->elem[xx1]);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      rtInvokeEndElement (pctxt, "elem", xx1);

   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  InfoRequestResponse_perCallInfo_element_pdu_element       */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225InfoRequestResponse_perCallInfo_element_pdu_element (OOCTXT* pctxt, H225InfoRequestResponse_perCallInfo_element_pdu_element* pvalue)
{
   int stat = ASN_OK;

   /* decode h323pdu */

   rtInvokeStartElement (pctxt, "h323pdu", -1);

   stat = asn1PD_H225H323_UU_PDU (pctxt, &pvalue->h323pdu);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "h323pdu", -1);

   /* decode sent */

   rtInvokeStartElement (pctxt, "sent", -1);

   stat = DECODEBIT (pctxt, &pvalue->sent);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   rtInvokeBoolValue (pctxt, pvalue->sent);

   rtInvokeEndElement (pctxt, "sent", -1);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  _SeqOfH225InfoRequestResponse_perCallInfo_element_pdu_el  */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225_SeqOfH225InfoRequestResponse_perCallInfo_element_pdu_element (OOCTXT* pctxt, H225_SeqOfH225InfoRequestResponse_perCallInfo_element_pdu_element* pvalue)
{
   int stat = ASN_OK;
   H225InfoRequestResponse_perCallInfo_element_pdu_element* pdata;
   ASN1UINT count = 0;
   ASN1UINT xx1;
   int lstat;

   dListInit (pvalue);

   for (;;) {
      /* decode length determinant */

      lstat = decodeLength (pctxt, &count);
      if (lstat != ASN_OK && lstat != ASN_OK_FRAG) {
         return LOG_ASN1ERR (pctxt, lstat);
      }

      /* decode elements */

      for (xx1 = 0; xx1 < count; xx1++) {
         rtInvokeStartElement (pctxt, "elem", xx1);

         pdata = ALLOC_ASN1ELEMDNODE (pctxt, H225InfoRequestResponse_perCallInfo_element_pdu_element);
         if (pdata == NULL)
            return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

         stat = asn1PD_H225InfoRequestResponse_perCallInfo_element_pdu_element (pctxt, pdata);
         if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
         rtInvokeEndElement (pctxt, "elem", xx1);

         dListAppendNode (pctxt, pvalue, pdata);
      }

      if(lstat == ASN_OK) break;
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  InfoRequestResponse_perCallInfo_element                   */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225InfoRequestResponse_perCallInfo_element (OOCTXT* pctxt, H225InfoRequestResponse_perCallInfo_element* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   OOCTXT lctxt2;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.nonStandardDataPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.originatorPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.audioPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.videoPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.dataPresent = optbit;

   /* decode nonStandardData */

   if (pvalue->m.nonStandardDataPresent) {
      rtInvokeStartElement (pctxt, "nonStandardData", -1);

      stat = asn1PD_H225NonStandardParameter (pctxt, &pvalue->nonStandardData);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "nonStandardData", -1);
   }

   /* decode callReferenceValue */

   rtInvokeStartElement (pctxt, "callReferenceValue", -1);

   stat = asn1PD_H225CallReferenceValue (pctxt, &pvalue->callReferenceValue);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "callReferenceValue", -1);

   /* decode conferenceID */

   rtInvokeStartElement (pctxt, "conferenceID", -1);

   stat = asn1PD_H225ConferenceIdentifier (pctxt, &pvalue->conferenceID);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "conferenceID", -1);

   /* decode originator */

   if (pvalue->m.originatorPresent) {
      rtInvokeStartElement (pctxt, "originator", -1);

      stat = DECODEBIT (pctxt, &pvalue->originator);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      rtInvokeBoolValue (pctxt, pvalue->originator);

      rtInvokeEndElement (pctxt, "originator", -1);
   }

   /* decode audio */

   if (pvalue->m.audioPresent) {
      rtInvokeStartElement (pctxt, "audio", -1);

      stat = asn1PD_H225_SeqOfH225RTPSession (pctxt, &pvalue->audio);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "audio", -1);
   }

   /* decode video */

   if (pvalue->m.videoPresent) {
      rtInvokeStartElement (pctxt, "video", -1);

      stat = asn1PD_H225_SeqOfH225RTPSession (pctxt, &pvalue->video);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "video", -1);
   }

   /* decode data */

   if (pvalue->m.dataPresent) {
      rtInvokeStartElement (pctxt, "data", -1);

      stat = asn1PD_H225_SeqOfH225TransportChannelInfo (pctxt, &pvalue->data);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "data", -1);
   }

   /* decode h245 */

   rtInvokeStartElement (pctxt, "h245", -1);

   stat = asn1PD_H225TransportChannelInfo (pctxt, &pvalue->h245);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "h245", -1);

   /* decode callSignaling */

   rtInvokeStartElement (pctxt, "callSignaling", -1);

   stat = asn1PD_H225TransportChannelInfo (pctxt, &pvalue->callSignaling);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "callSignaling", -1);

   /* decode callType */

   rtInvokeStartElement (pctxt, "callType", -1);

   stat = asn1PD_H225CallType (pctxt, &pvalue->callType);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "callType", -1);

   /* decode bandWidth */

   rtInvokeStartElement (pctxt, "bandWidth", -1);

   stat = asn1PD_H225BandWidth (pctxt, &pvalue->bandWidth);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "bandWidth", -1);

   /* decode callModel */

   rtInvokeStartElement (pctxt, "callModel", -1);

   stat = asn1PD_H225CallModel (pctxt, &pvalue->callModel);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "callModel", -1);

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            if (i < 8 && openType.numocts > 0) {  /* known element */
               copyContext (&lctxt2, pctxt);
               initContextBuffer (pctxt, openType.data, openType.numocts);

               switch (i) {
                  case 0:
                     pvalue->m.callIdentifierPresent = 1;

                     rtInvokeStartElement (pctxt, "callIdentifier", -1);

                     stat = asn1PD_H225CallIdentifier (pctxt, &pvalue->callIdentifier);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "callIdentifier", -1);
                     break;

                  case 1:
                     pvalue->m.tokensPresent = 1;

                     rtInvokeStartElement (pctxt, "tokens", -1);

                     stat = asn1PD_H225_SeqOfH225ClearToken (pctxt, &pvalue->tokens);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "tokens", -1);
                     break;

                  case 2:
                     pvalue->m.cryptoTokensPresent = 1;

                     rtInvokeStartElement (pctxt, "cryptoTokens", -1);

                     stat = asn1PD_H225_SeqOfH225CryptoH323Token (pctxt, &pvalue->cryptoTokens);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "cryptoTokens", -1);
                     break;

                  case 3:
                     pvalue->m.substituteConfIDsPresent = 1;

                     rtInvokeStartElement (pctxt, "substituteConfIDs", -1);

                     stat = asn1PD_H225_SeqOfH225ConferenceIdentifier (pctxt, &pvalue->substituteConfIDs);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "substituteConfIDs", -1);
                     break;

                  case 4:
                     pvalue->m.pduPresent = 1;

                     rtInvokeStartElement (pctxt, "pdu", -1);

                     stat = asn1PD_H225_SeqOfH225InfoRequestResponse_perCallInfo_element_pdu_element (pctxt, &pvalue->pdu);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "pdu", -1);
                     break;

                  case 5:
                     pvalue->m.callLinkagePresent = 1;

                     rtInvokeStartElement (pctxt, "callLinkage", -1);

                     stat = asn1PD_H225CallLinkage (pctxt, &pvalue->callLinkage);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "callLinkage", -1);
                     break;

                  case 6:
                     pvalue->m.usageInformationPresent = 1;

                     rtInvokeStartElement (pctxt, "usageInformation", -1);

                     stat = asn1PD_H225RasUsageInformation (pctxt, &pvalue->usageInformation);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "usageInformation", -1);
                     break;

                  case 7:
                     pvalue->m.circuitInfoPresent = 1;

                     rtInvokeStartElement (pctxt, "circuitInfo", -1);

                     stat = asn1PD_H225CircuitInfo (pctxt, &pvalue->circuitInfo);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "circuitInfo", -1);
                     break;

                  default:
                     pctxt->buffer.byteIndex += openType.numocts;
               }
               copyContext (pctxt, &lctxt2);
            }
            else {  /* unknown element */
               pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
               pOpenType->numocts = openType.numocts;
               pOpenType->data = openType.data;
               dListAppend (pctxt, &pvalue->extElem1, pOpenType);
            }
         }
         else if (i >= 8) {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  _SeqOfH225InfoRequestResponse_perCallInfo_element         */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225_SeqOfH225InfoRequestResponse_perCallInfo_element (OOCTXT* pctxt, H225_SeqOfH225InfoRequestResponse_perCallInfo_element* pvalue)
{
   int stat = ASN_OK;
   H225InfoRequestResponse_perCallInfo_element* pdata;
   ASN1UINT count = 0;
   ASN1UINT xx1;
   int lstat;

   dListInit (pvalue);

   for (;;) {
      /* decode length determinant */

      lstat = decodeLength (pctxt, &count);
      if (lstat != ASN_OK && lstat != ASN_OK_FRAG) {
         return LOG_ASN1ERR (pctxt, lstat);
      }

      /* decode elements */

      for (xx1 = 0; xx1 < count; xx1++) {
         rtInvokeStartElement (pctxt, "elem", xx1);

         pdata = ALLOC_ASN1ELEMDNODE (pctxt, H225InfoRequestResponse_perCallInfo_element);
         if (pdata == NULL)
            return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

         stat = asn1PD_H225InfoRequestResponse_perCallInfo_element (pctxt, pdata);
         if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
         rtInvokeEndElement (pctxt, "elem", xx1);

         dListAppendNode (pctxt, pvalue, pdata);
      }

      if(lstat == ASN_OK) break;
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  InfoRequestResponseStatus                                 */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225InfoRequestResponseStatus (OOCTXT* pctxt, H225InfoRequestResponseStatus* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT ui;
   ASN1OpenType openType;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   if (!extbit) {
      stat = decodeConsUnsigned (pctxt, &ui, 0, 3);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      else pvalue->t = ui + 1;

      switch (ui) {
         /* complete */
         case 0:
            rtInvokeStartElement (pctxt, "complete", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "complete", -1);

            break;

         /* incomplete */
         case 1:
            rtInvokeStartElement (pctxt, "incomplete", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "incomplete", -1);

            break;

         /* segment */
         case 2:
            rtInvokeStartElement (pctxt, "segment", -1);

            stat = decodeConsUInt16 (pctxt, &pvalue->u.segment, 0U, 65535U);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
            rtInvokeUIntValue (pctxt, pvalue->u.segment);

            rtInvokeEndElement (pctxt, "segment", -1);

            break;

         /* invalidCall */
         case 3:
            rtInvokeStartElement (pctxt, "invalidCall", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "invalidCall", -1);

            break;

         default:
            return LOG_ASN1ERR (pctxt, ASN_E_INVOPT);
      }
   }
   else {
      stat = decodeSmallNonNegWholeNumber (pctxt, &ui);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      else pvalue->t = ui + 5;

      stat = decodeByteAlign (pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      pvalue->u.extElem1 = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
      if (pvalue->u.extElem1 == NULL)
         return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

      pvalue->u.extElem1->data = openType.data;
      pvalue->u.extElem1->numocts = openType.numocts;

   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  InfoRequestResponse                                       */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225InfoRequestResponse (OOCTXT* pctxt, H225InfoRequestResponse* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   OOCTXT lctxt2;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.nonStandardDataPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.endpointAliasPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.perCallInfoPresent = optbit;

   /* decode nonStandardData */

   if (pvalue->m.nonStandardDataPresent) {
      rtInvokeStartElement (pctxt, "nonStandardData", -1);

      stat = asn1PD_H225NonStandardParameter (pctxt, &pvalue->nonStandardData);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "nonStandardData", -1);
   }

   /* decode requestSeqNum */

   rtInvokeStartElement (pctxt, "requestSeqNum", -1);

   stat = asn1PD_H225RequestSeqNum (pctxt, &pvalue->requestSeqNum);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "requestSeqNum", -1);

   /* decode endpointType */

   rtInvokeStartElement (pctxt, "endpointType", -1);

   stat = asn1PD_H225EndpointType (pctxt, &pvalue->endpointType);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "endpointType", -1);

   /* decode endpointIdentifier */

   rtInvokeStartElement (pctxt, "endpointIdentifier", -1);

   stat = asn1PD_H225EndpointIdentifier (pctxt, &pvalue->endpointIdentifier);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "endpointIdentifier", -1);

   /* decode rasAddress */

   rtInvokeStartElement (pctxt, "rasAddress", -1);

   stat = asn1PD_H225TransportAddress (pctxt, &pvalue->rasAddress);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "rasAddress", -1);

   /* decode callSignalAddress */

   rtInvokeStartElement (pctxt, "callSignalAddress", -1);

   stat = asn1PD_H225_SeqOfH225TransportAddress (pctxt, &pvalue->callSignalAddress);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "callSignalAddress", -1);

   /* decode endpointAlias */

   if (pvalue->m.endpointAliasPresent) {
      rtInvokeStartElement (pctxt, "endpointAlias", -1);

      stat = asn1PD_H225_SeqOfH225AliasAddress (pctxt, &pvalue->endpointAlias);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "endpointAlias", -1);
   }

   /* decode perCallInfo */

   if (pvalue->m.perCallInfoPresent) {
      rtInvokeStartElement (pctxt, "perCallInfo", -1);

      stat = asn1PD_H225_SeqOfH225InfoRequestResponse_perCallInfo_element (pctxt, &pvalue->perCallInfo);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "perCallInfo", -1);
   }

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            if (i < 8 && openType.numocts > 0) {  /* known element */
               copyContext (&lctxt2, pctxt);
               initContextBuffer (pctxt, openType.data, openType.numocts);

               switch (i) {
                  case 0:
                     pvalue->m.tokensPresent = 1;

                     rtInvokeStartElement (pctxt, "tokens", -1);

                     stat = asn1PD_H225_SeqOfH225ClearToken (pctxt, &pvalue->tokens);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "tokens", -1);
                     break;

                  case 1:
                     pvalue->m.cryptoTokensPresent = 1;

                     rtInvokeStartElement (pctxt, "cryptoTokens", -1);

                     stat = asn1PD_H225_SeqOfH225CryptoH323Token (pctxt, &pvalue->cryptoTokens);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "cryptoTokens", -1);
                     break;

                  case 2:
                     pvalue->m.integrityCheckValuePresent = 1;

                     rtInvokeStartElement (pctxt, "integrityCheckValue", -1);

                     stat = asn1PD_H225ICV (pctxt, &pvalue->integrityCheckValue);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "integrityCheckValue", -1);
                     break;

                  case 3:
                     pvalue->m.needResponsePresent = 1;

                     rtInvokeStartElement (pctxt, "needResponse", -1);

                     stat = DECODEBIT (pctxt, &pvalue->needResponse);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
                     rtInvokeBoolValue (pctxt, pvalue->needResponse);

                     rtInvokeEndElement (pctxt, "needResponse", -1);
                     break;

                  case 4:
                     pvalue->m.capacityPresent = 1;

                     rtInvokeStartElement (pctxt, "capacity", -1);

                     stat = asn1PD_H225CallCapacity (pctxt, &pvalue->capacity);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "capacity", -1);
                     break;

                  case 5:
                     pvalue->m.irrStatusPresent = 1;

                     rtInvokeStartElement (pctxt, "irrStatus", -1);

                     stat = asn1PD_H225InfoRequestResponseStatus (pctxt, &pvalue->irrStatus);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "irrStatus", -1);
                     break;

                  case 6:
                     pvalue->m.unsolicitedPresent = 1;

                     rtInvokeStartElement (pctxt, "unsolicited", -1);

                     stat = DECODEBIT (pctxt, &pvalue->unsolicited);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
                     rtInvokeBoolValue (pctxt, pvalue->unsolicited);

                     rtInvokeEndElement (pctxt, "unsolicited", -1);
                     break;

                  case 7:
                     pvalue->m.genericDataPresent = 1;

                     rtInvokeStartElement (pctxt, "genericData", -1);

                     stat = asn1PD_H225_SeqOfH225GenericData (pctxt, &pvalue->genericData);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "genericData", -1);
                     break;

                  default:
                     pctxt->buffer.byteIndex += openType.numocts;
               }
               copyContext (pctxt, &lctxt2);
            }
            else {  /* unknown element */
               pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
               pOpenType->numocts = openType.numocts;
               pOpenType->data = openType.data;
               dListAppend (pctxt, &pvalue->extElem1, pOpenType);
            }
         }
         else if (i >= 8) {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  NonStandardMessage                                        */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225NonStandardMessage (OOCTXT* pctxt, H225NonStandardMessage* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   OOCTXT lctxt2;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   /* decode requestSeqNum */

   rtInvokeStartElement (pctxt, "requestSeqNum", -1);

   stat = asn1PD_H225RequestSeqNum (pctxt, &pvalue->requestSeqNum);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "requestSeqNum", -1);

   /* decode nonStandardData */

   rtInvokeStartElement (pctxt, "nonStandardData", -1);

   stat = asn1PD_H225NonStandardParameter (pctxt, &pvalue->nonStandardData);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "nonStandardData", -1);

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            if (i < 5 && openType.numocts > 0) {  /* known element */
               copyContext (&lctxt2, pctxt);
               initContextBuffer (pctxt, openType.data, openType.numocts);

               switch (i) {
                  case 0:
                     pvalue->m.tokensPresent = 1;

                     rtInvokeStartElement (pctxt, "tokens", -1);

                     stat = asn1PD_H225_SeqOfH225ClearToken (pctxt, &pvalue->tokens);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "tokens", -1);
                     break;

                  case 1:
                     pvalue->m.cryptoTokensPresent = 1;

                     rtInvokeStartElement (pctxt, "cryptoTokens", -1);

                     stat = asn1PD_H225_SeqOfH225CryptoH323Token (pctxt, &pvalue->cryptoTokens);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "cryptoTokens", -1);
                     break;

                  case 2:
                     pvalue->m.integrityCheckValuePresent = 1;

                     rtInvokeStartElement (pctxt, "integrityCheckValue", -1);

                     stat = asn1PD_H225ICV (pctxt, &pvalue->integrityCheckValue);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "integrityCheckValue", -1);
                     break;

                  case 3:
                     pvalue->m.featureSetPresent = 1;

                     rtInvokeStartElement (pctxt, "featureSet", -1);

                     stat = asn1PD_H225FeatureSet (pctxt, &pvalue->featureSet);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "featureSet", -1);
                     break;

                  case 4:
                     pvalue->m.genericDataPresent = 1;

                     rtInvokeStartElement (pctxt, "genericData", -1);

                     stat = asn1PD_H225_SeqOfH225GenericData (pctxt, &pvalue->genericData);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "genericData", -1);
                     break;

                  default:
                     pctxt->buffer.byteIndex += openType.numocts;
               }
               copyContext (pctxt, &lctxt2);
            }
            else {  /* unknown element */
               pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
               pOpenType->numocts = openType.numocts;
               pOpenType->data = openType.data;
               dListAppend (pctxt, &pvalue->extElem1, pOpenType);
            }
         }
         else if (i >= 5) {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  UnknownMessageResponse                                    */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225UnknownMessageResponse (OOCTXT* pctxt, H225UnknownMessageResponse* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   OOCTXT lctxt2;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   /* decode requestSeqNum */

   rtInvokeStartElement (pctxt, "requestSeqNum", -1);

   stat = asn1PD_H225RequestSeqNum (pctxt, &pvalue->requestSeqNum);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "requestSeqNum", -1);

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            if (i < 4 && openType.numocts > 0) {  /* known element */
               copyContext (&lctxt2, pctxt);
               initContextBuffer (pctxt, openType.data, openType.numocts);

               switch (i) {
                  case 0:
                     pvalue->m.tokensPresent = 1;

                     rtInvokeStartElement (pctxt, "tokens", -1);

                     stat = asn1PD_H225_SeqOfH225ClearToken (pctxt, &pvalue->tokens);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "tokens", -1);
                     break;

                  case 1:
                     pvalue->m.cryptoTokensPresent = 1;

                     rtInvokeStartElement (pctxt, "cryptoTokens", -1);

                     stat = asn1PD_H225_SeqOfH225CryptoH323Token (pctxt, &pvalue->cryptoTokens);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "cryptoTokens", -1);
                     break;

                  case 2:
                     pvalue->m.integrityCheckValuePresent = 1;

                     rtInvokeStartElement (pctxt, "integrityCheckValue", -1);

                     stat = asn1PD_H225ICV (pctxt, &pvalue->integrityCheckValue);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "integrityCheckValue", -1);
                     break;

                  case 3:
                     pvalue->m.messageNotUnderstoodPresent = 1;

                     rtInvokeStartElement (pctxt, "messageNotUnderstood", -1);

                     stat = decodeDynOctetString (pctxt, (ASN1DynOctStr*)&pvalue->messageNotUnderstood);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
                     rtInvokeOctStrValue (pctxt, pvalue->messageNotUnderstood.numocts, pvalue->messageNotUnderstood.data);

                     rtInvokeEndElement (pctxt, "messageNotUnderstood", -1);
                     break;

                  default:
                     pctxt->buffer.byteIndex += openType.numocts;
               }
               copyContext (pctxt, &lctxt2);
            }
            else {  /* unknown element */
               pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
               pOpenType->numocts = openType.numocts;
               pOpenType->data = openType.data;
               dListAppend (pctxt, &pvalue->extElem1, pOpenType);
            }
         }
         else if (i >= 4) {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  RequestInProgress                                         */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225RequestInProgress (OOCTXT* pctxt, H225RequestInProgress* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.nonStandardDataPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.tokensPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.cryptoTokensPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.integrityCheckValuePresent = optbit;

   /* decode requestSeqNum */

   rtInvokeStartElement (pctxt, "requestSeqNum", -1);

   stat = asn1PD_H225RequestSeqNum (pctxt, &pvalue->requestSeqNum);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "requestSeqNum", -1);

   /* decode nonStandardData */

   if (pvalue->m.nonStandardDataPresent) {
      rtInvokeStartElement (pctxt, "nonStandardData", -1);

      stat = asn1PD_H225NonStandardParameter (pctxt, &pvalue->nonStandardData);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "nonStandardData", -1);
   }

   /* decode tokens */

   if (pvalue->m.tokensPresent) {
      rtInvokeStartElement (pctxt, "tokens", -1);

      stat = asn1PD_H225_SeqOfH225ClearToken (pctxt, &pvalue->tokens);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "tokens", -1);
   }

   /* decode cryptoTokens */

   if (pvalue->m.cryptoTokensPresent) {
      rtInvokeStartElement (pctxt, "cryptoTokens", -1);

      stat = asn1PD_H225_SeqOfH225CryptoH323Token (pctxt, &pvalue->cryptoTokens);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "cryptoTokens", -1);
   }

   /* decode integrityCheckValue */

   if (pvalue->m.integrityCheckValuePresent) {
      rtInvokeStartElement (pctxt, "integrityCheckValue", -1);

      stat = asn1PD_H225ICV (pctxt, &pvalue->integrityCheckValue);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "integrityCheckValue", -1);
   }

   /* decode delay */

   rtInvokeStartElement (pctxt, "delay", -1);

   stat = decodeConsUInt16 (pctxt, &pvalue->delay, 1U, 65535U);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   rtInvokeUIntValue (pctxt, pvalue->delay);

   rtInvokeEndElement (pctxt, "delay", -1);

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
            pOpenType->numocts = openType.numocts;
            pOpenType->data = openType.data;
            dListAppend (pctxt, &pvalue->extElem1, pOpenType);
         }
         else {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  ResourcesAvailableIndicate                                */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225ResourcesAvailableIndicate (OOCTXT* pctxt, H225ResourcesAvailableIndicate* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   OOCTXT lctxt2;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.nonStandardDataPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.tokensPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.cryptoTokensPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.integrityCheckValuePresent = optbit;

   /* decode requestSeqNum */

   rtInvokeStartElement (pctxt, "requestSeqNum", -1);

   stat = asn1PD_H225RequestSeqNum (pctxt, &pvalue->requestSeqNum);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "requestSeqNum", -1);

   /* decode protocolIdentifier */

   rtInvokeStartElement (pctxt, "protocolIdentifier", -1);

   stat = asn1PD_H225ProtocolIdentifier (pctxt, &pvalue->protocolIdentifier);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "protocolIdentifier", -1);

   /* decode nonStandardData */

   if (pvalue->m.nonStandardDataPresent) {
      rtInvokeStartElement (pctxt, "nonStandardData", -1);

      stat = asn1PD_H225NonStandardParameter (pctxt, &pvalue->nonStandardData);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "nonStandardData", -1);
   }

   /* decode endpointIdentifier */

   rtInvokeStartElement (pctxt, "endpointIdentifier", -1);

   stat = asn1PD_H225EndpointIdentifier (pctxt, &pvalue->endpointIdentifier);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "endpointIdentifier", -1);

   /* decode protocols */

   rtInvokeStartElement (pctxt, "protocols", -1);

   stat = asn1PD_H225_SeqOfH225SupportedProtocols (pctxt, &pvalue->protocols);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "protocols", -1);

   /* decode almostOutOfResources */

   rtInvokeStartElement (pctxt, "almostOutOfResources", -1);

   stat = DECODEBIT (pctxt, &pvalue->almostOutOfResources);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   rtInvokeBoolValue (pctxt, pvalue->almostOutOfResources);

   rtInvokeEndElement (pctxt, "almostOutOfResources", -1);

   /* decode tokens */

   if (pvalue->m.tokensPresent) {
      rtInvokeStartElement (pctxt, "tokens", -1);

      stat = asn1PD_H225_SeqOfH225ClearToken (pctxt, &pvalue->tokens);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "tokens", -1);
   }

   /* decode cryptoTokens */

   if (pvalue->m.cryptoTokensPresent) {
      rtInvokeStartElement (pctxt, "cryptoTokens", -1);

      stat = asn1PD_H225_SeqOfH225CryptoH323Token (pctxt, &pvalue->cryptoTokens);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "cryptoTokens", -1);
   }

   /* decode integrityCheckValue */

   if (pvalue->m.integrityCheckValuePresent) {
      rtInvokeStartElement (pctxt, "integrityCheckValue", -1);

      stat = asn1PD_H225ICV (pctxt, &pvalue->integrityCheckValue);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "integrityCheckValue", -1);
   }

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            if (i < 2 && openType.numocts > 0) {  /* known element */
               copyContext (&lctxt2, pctxt);
               initContextBuffer (pctxt, openType.data, openType.numocts);

               switch (i) {
                  case 0:
                     pvalue->m.capacityPresent = 1;

                     rtInvokeStartElement (pctxt, "capacity", -1);

                     stat = asn1PD_H225CallCapacity (pctxt, &pvalue->capacity);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "capacity", -1);
                     break;

                  case 1:
                     pvalue->m.genericDataPresent = 1;

                     rtInvokeStartElement (pctxt, "genericData", -1);

                     stat = asn1PD_H225_SeqOfH225GenericData (pctxt, &pvalue->genericData);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "genericData", -1);
                     break;

                  default:
                     pctxt->buffer.byteIndex += openType.numocts;
               }
               copyContext (pctxt, &lctxt2);
            }
            else {  /* unknown element */
               pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
               pOpenType->numocts = openType.numocts;
               pOpenType->data = openType.data;
               dListAppend (pctxt, &pvalue->extElem1, pOpenType);
            }
         }
         else if (i >= 2) {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  ResourcesAvailableConfirm                                 */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225ResourcesAvailableConfirm (OOCTXT* pctxt, H225ResourcesAvailableConfirm* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   OOCTXT lctxt2;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.nonStandardDataPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.tokensPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.cryptoTokensPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.integrityCheckValuePresent = optbit;

   /* decode requestSeqNum */

   rtInvokeStartElement (pctxt, "requestSeqNum", -1);

   stat = asn1PD_H225RequestSeqNum (pctxt, &pvalue->requestSeqNum);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "requestSeqNum", -1);

   /* decode protocolIdentifier */

   rtInvokeStartElement (pctxt, "protocolIdentifier", -1);

   stat = asn1PD_H225ProtocolIdentifier (pctxt, &pvalue->protocolIdentifier);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "protocolIdentifier", -1);

   /* decode nonStandardData */

   if (pvalue->m.nonStandardDataPresent) {
      rtInvokeStartElement (pctxt, "nonStandardData", -1);

      stat = asn1PD_H225NonStandardParameter (pctxt, &pvalue->nonStandardData);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "nonStandardData", -1);
   }

   /* decode tokens */

   if (pvalue->m.tokensPresent) {
      rtInvokeStartElement (pctxt, "tokens", -1);

      stat = asn1PD_H225_SeqOfH225ClearToken (pctxt, &pvalue->tokens);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "tokens", -1);
   }

   /* decode cryptoTokens */

   if (pvalue->m.cryptoTokensPresent) {
      rtInvokeStartElement (pctxt, "cryptoTokens", -1);

      stat = asn1PD_H225_SeqOfH225CryptoH323Token (pctxt, &pvalue->cryptoTokens);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "cryptoTokens", -1);
   }

   /* decode integrityCheckValue */

   if (pvalue->m.integrityCheckValuePresent) {
      rtInvokeStartElement (pctxt, "integrityCheckValue", -1);

      stat = asn1PD_H225ICV (pctxt, &pvalue->integrityCheckValue);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "integrityCheckValue", -1);
   }

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            if (i < 1 && openType.numocts > 0) {  /* known element */
               copyContext (&lctxt2, pctxt);
               initContextBuffer (pctxt, openType.data, openType.numocts);

               switch (i) {
                  case 0:
                     pvalue->m.genericDataPresent = 1;

                     rtInvokeStartElement (pctxt, "genericData", -1);

                     stat = asn1PD_H225_SeqOfH225GenericData (pctxt, &pvalue->genericData);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "genericData", -1);
                     break;

                  default:
                     pctxt->buffer.byteIndex += openType.numocts;
               }
               copyContext (pctxt, &lctxt2);
            }
            else {  /* unknown element */
               pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
               pOpenType->numocts = openType.numocts;
               pOpenType->data = openType.data;
               dListAppend (pctxt, &pvalue->extElem1, pOpenType);
            }
         }
         else if (i >= 1) {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  InfoRequestAck                                            */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225InfoRequestAck (OOCTXT* pctxt, H225InfoRequestAck* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.nonStandardDataPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.tokensPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.cryptoTokensPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.integrityCheckValuePresent = optbit;

   /* decode requestSeqNum */

   rtInvokeStartElement (pctxt, "requestSeqNum", -1);

   stat = asn1PD_H225RequestSeqNum (pctxt, &pvalue->requestSeqNum);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "requestSeqNum", -1);

   /* decode nonStandardData */

   if (pvalue->m.nonStandardDataPresent) {
      rtInvokeStartElement (pctxt, "nonStandardData", -1);

      stat = asn1PD_H225NonStandardParameter (pctxt, &pvalue->nonStandardData);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "nonStandardData", -1);
   }

   /* decode tokens */

   if (pvalue->m.tokensPresent) {
      rtInvokeStartElement (pctxt, "tokens", -1);

      stat = asn1PD_H225_SeqOfH225ClearToken (pctxt, &pvalue->tokens);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "tokens", -1);
   }

   /* decode cryptoTokens */

   if (pvalue->m.cryptoTokensPresent) {
      rtInvokeStartElement (pctxt, "cryptoTokens", -1);

      stat = asn1PD_H225_SeqOfH225CryptoH323Token (pctxt, &pvalue->cryptoTokens);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "cryptoTokens", -1);
   }

   /* decode integrityCheckValue */

   if (pvalue->m.integrityCheckValuePresent) {
      rtInvokeStartElement (pctxt, "integrityCheckValue", -1);

      stat = asn1PD_H225ICV (pctxt, &pvalue->integrityCheckValue);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "integrityCheckValue", -1);
   }

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
            pOpenType->numocts = openType.numocts;
            pOpenType->data = openType.data;
            dListAppend (pctxt, &pvalue->extElem1, pOpenType);
         }
         else {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  InfoRequestNakReason                                      */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225InfoRequestNakReason (OOCTXT* pctxt, H225InfoRequestNakReason* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT ui;
   ASN1OpenType openType;
   ASN1BOOL extbit;
   OOCTXT lctxt;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   if (!extbit) {
      stat = decodeConsUnsigned (pctxt, &ui, 0, 2);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      else pvalue->t = ui + 1;

      switch (ui) {
         /* notRegistered */
         case 0:
            rtInvokeStartElement (pctxt, "notRegistered", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "notRegistered", -1);

            break;

         /* securityDenial */
         case 1:
            rtInvokeStartElement (pctxt, "securityDenial", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "securityDenial", -1);

            break;

         /* undefinedReason */
         case 2:
            rtInvokeStartElement (pctxt, "undefinedReason", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "undefinedReason", -1);

            break;

         default:
            return LOG_ASN1ERR (pctxt, ASN_E_INVOPT);
      }
   }
   else {
      stat = decodeSmallNonNegWholeNumber (pctxt, &ui);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      else pvalue->t = ui + 4;

      stat = decodeByteAlign (pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      copyContext (&lctxt, pctxt);
      initContextBuffer (pctxt, openType.data, openType.numocts);

      switch (pvalue->t) {
         /* securityError */
         case 4:
            rtInvokeStartElement (pctxt, "securityError", -1);

            pvalue->u.securityError = ALLOC_ASN1ELEM (pctxt, H225SecurityErrors2);
            if (pvalue->u.securityError == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225SecurityErrors2 (pctxt, pvalue->u.securityError);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "securityError", -1);

            break;

         default:
            pvalue->u.extElem1 = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
            if (pvalue->u.extElem1 == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            pvalue->u.extElem1->data = openType.data;
            pvalue->u.extElem1->numocts = openType.numocts;

      }

      copyContext (pctxt, &lctxt);
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  InfoRequestNak                                            */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225InfoRequestNak (OOCTXT* pctxt, H225InfoRequestNak* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.nonStandardDataPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.altGKInfoPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.tokensPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.cryptoTokensPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.integrityCheckValuePresent = optbit;

   /* decode requestSeqNum */

   rtInvokeStartElement (pctxt, "requestSeqNum", -1);

   stat = asn1PD_H225RequestSeqNum (pctxt, &pvalue->requestSeqNum);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "requestSeqNum", -1);

   /* decode nonStandardData */

   if (pvalue->m.nonStandardDataPresent) {
      rtInvokeStartElement (pctxt, "nonStandardData", -1);

      stat = asn1PD_H225NonStandardParameter (pctxt, &pvalue->nonStandardData);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "nonStandardData", -1);
   }

   /* decode nakReason */

   rtInvokeStartElement (pctxt, "nakReason", -1);

   stat = asn1PD_H225InfoRequestNakReason (pctxt, &pvalue->nakReason);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "nakReason", -1);

   /* decode altGKInfo */

   if (pvalue->m.altGKInfoPresent) {
      rtInvokeStartElement (pctxt, "altGKInfo", -1);

      stat = asn1PD_H225AltGKInfo (pctxt, &pvalue->altGKInfo);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "altGKInfo", -1);
   }

   /* decode tokens */

   if (pvalue->m.tokensPresent) {
      rtInvokeStartElement (pctxt, "tokens", -1);

      stat = asn1PD_H225_SeqOfH225ClearToken (pctxt, &pvalue->tokens);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "tokens", -1);
   }

   /* decode cryptoTokens */

   if (pvalue->m.cryptoTokensPresent) {
      rtInvokeStartElement (pctxt, "cryptoTokens", -1);

      stat = asn1PD_H225_SeqOfH225CryptoH323Token (pctxt, &pvalue->cryptoTokens);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "cryptoTokens", -1);
   }

   /* decode integrityCheckValue */

   if (pvalue->m.integrityCheckValuePresent) {
      rtInvokeStartElement (pctxt, "integrityCheckValue", -1);

      stat = asn1PD_H225ICV (pctxt, &pvalue->integrityCheckValue);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "integrityCheckValue", -1);
   }

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
            pOpenType->numocts = openType.numocts;
            pOpenType->data = openType.data;
            dListAppend (pctxt, &pvalue->extElem1, pOpenType);
         }
         else {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  ServiceControlIndication_callSpecific                     */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225ServiceControlIndication_callSpecific (OOCTXT* pctxt, H225ServiceControlIndication_callSpecific* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   dListInit (&pvalue->extElem1);

   /* decode callIdentifier */

   rtInvokeStartElement (pctxt, "callIdentifier", -1);

   stat = asn1PD_H225CallIdentifier (pctxt, &pvalue->callIdentifier);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "callIdentifier", -1);

   /* decode conferenceID */

   rtInvokeStartElement (pctxt, "conferenceID", -1);

   stat = asn1PD_H225ConferenceIdentifier (pctxt, &pvalue->conferenceID);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "conferenceID", -1);

   /* decode answeredCall */

   rtInvokeStartElement (pctxt, "answeredCall", -1);

   stat = DECODEBIT (pctxt, &pvalue->answeredCall);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   rtInvokeBoolValue (pctxt, pvalue->answeredCall);

   rtInvokeEndElement (pctxt, "answeredCall", -1);

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
            pOpenType->numocts = openType.numocts;
            pOpenType->data = openType.data;
            dListAppend (pctxt, &pvalue->extElem1, pOpenType);
         }
         else {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  ServiceControlIndication                                  */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225ServiceControlIndication (OOCTXT* pctxt, H225ServiceControlIndication* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.nonStandardDataPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.endpointIdentifierPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.callSpecificPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.tokensPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.cryptoTokensPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.integrityCheckValuePresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.featureSetPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.genericDataPresent = optbit;

   /* decode requestSeqNum */

   rtInvokeStartElement (pctxt, "requestSeqNum", -1);

   stat = asn1PD_H225RequestSeqNum (pctxt, &pvalue->requestSeqNum);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "requestSeqNum", -1);

   /* decode nonStandardData */

   if (pvalue->m.nonStandardDataPresent) {
      rtInvokeStartElement (pctxt, "nonStandardData", -1);

      stat = asn1PD_H225NonStandardParameter (pctxt, &pvalue->nonStandardData);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "nonStandardData", -1);
   }

   /* decode serviceControl */

   rtInvokeStartElement (pctxt, "serviceControl", -1);

   stat = asn1PD_H225_SeqOfH225ServiceControlSession (pctxt, &pvalue->serviceControl);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "serviceControl", -1);

   /* decode endpointIdentifier */

   if (pvalue->m.endpointIdentifierPresent) {
      rtInvokeStartElement (pctxt, "endpointIdentifier", -1);

      stat = asn1PD_H225EndpointIdentifier (pctxt, &pvalue->endpointIdentifier);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "endpointIdentifier", -1);
   }

   /* decode callSpecific */

   if (pvalue->m.callSpecificPresent) {
      rtInvokeStartElement (pctxt, "callSpecific", -1);

      stat = asn1PD_H225ServiceControlIndication_callSpecific (pctxt, &pvalue->callSpecific);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "callSpecific", -1);
   }

   /* decode tokens */

   if (pvalue->m.tokensPresent) {
      rtInvokeStartElement (pctxt, "tokens", -1);

      stat = asn1PD_H225_SeqOfH225ClearToken (pctxt, &pvalue->tokens);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "tokens", -1);
   }

   /* decode cryptoTokens */

   if (pvalue->m.cryptoTokensPresent) {
      rtInvokeStartElement (pctxt, "cryptoTokens", -1);

      stat = asn1PD_H225_SeqOfH225CryptoH323Token (pctxt, &pvalue->cryptoTokens);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "cryptoTokens", -1);
   }

   /* decode integrityCheckValue */

   if (pvalue->m.integrityCheckValuePresent) {
      rtInvokeStartElement (pctxt, "integrityCheckValue", -1);

      stat = asn1PD_H225ICV (pctxt, &pvalue->integrityCheckValue);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "integrityCheckValue", -1);
   }

   /* decode featureSet */

   if (pvalue->m.featureSetPresent) {
      rtInvokeStartElement (pctxt, "featureSet", -1);

      stat = asn1PD_H225FeatureSet (pctxt, &pvalue->featureSet);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "featureSet", -1);
   }

   /* decode genericData */

   if (pvalue->m.genericDataPresent) {
      rtInvokeStartElement (pctxt, "genericData", -1);

      stat = asn1PD_H225_SeqOfH225GenericData (pctxt, &pvalue->genericData);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "genericData", -1);
   }

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
            pOpenType->numocts = openType.numocts;
            pOpenType->data = openType.data;
            dListAppend (pctxt, &pvalue->extElem1, pOpenType);
         }
         else {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  ServiceControlResponse_result                             */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225ServiceControlResponse_result (OOCTXT* pctxt, H225ServiceControlResponse_result* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT ui;
   ASN1OpenType openType;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   if (!extbit) {
      stat = decodeConsUnsigned (pctxt, &ui, 0, 4);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      else pvalue->t = ui + 1;

      switch (ui) {
         /* started */
         case 0:
            rtInvokeStartElement (pctxt, "started", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "started", -1);

            break;

         /* failed */
         case 1:
            rtInvokeStartElement (pctxt, "failed", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "failed", -1);

            break;

         /* stopped */
         case 2:
            rtInvokeStartElement (pctxt, "stopped", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "stopped", -1);

            break;

         /* notAvailable */
         case 3:
            rtInvokeStartElement (pctxt, "notAvailable", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "notAvailable", -1);

            break;

         /* neededFeatureNotSupported */
         case 4:
            rtInvokeStartElement (pctxt, "neededFeatureNotSupported", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "neededFeatureNotSupported", -1);

            break;

         default:
            return LOG_ASN1ERR (pctxt, ASN_E_INVOPT);
      }
   }
   else {
      stat = decodeSmallNonNegWholeNumber (pctxt, &ui);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      else pvalue->t = ui + 6;

      stat = decodeByteAlign (pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      pvalue->u.extElem1 = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
      if (pvalue->u.extElem1 == NULL)
         return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

      pvalue->u.extElem1->data = openType.data;
      pvalue->u.extElem1->numocts = openType.numocts;

   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  ServiceControlResponse                                    */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225ServiceControlResponse (OOCTXT* pctxt, H225ServiceControlResponse* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   ASN1OpenType openType;
   ASN1OpenType* pOpenType;
   ASN1UINT bitcnt;
   ASN1UINT i;
   ASN1BOOL optbit;
   ASN1BOOL extbit;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   dListInit (&pvalue->extElem1);

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.resultPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.nonStandardDataPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.tokensPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.cryptoTokensPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.integrityCheckValuePresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.featureSetPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.genericDataPresent = optbit;

   /* decode requestSeqNum */

   rtInvokeStartElement (pctxt, "requestSeqNum", -1);

   stat = asn1PD_H225RequestSeqNum (pctxt, &pvalue->requestSeqNum);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "requestSeqNum", -1);

   /* decode result */

   if (pvalue->m.resultPresent) {
      rtInvokeStartElement (pctxt, "result", -1);

      stat = asn1PD_H225ServiceControlResponse_result (pctxt, &pvalue->result);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "result", -1);
   }

   /* decode nonStandardData */

   if (pvalue->m.nonStandardDataPresent) {
      rtInvokeStartElement (pctxt, "nonStandardData", -1);

      stat = asn1PD_H225NonStandardParameter (pctxt, &pvalue->nonStandardData);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "nonStandardData", -1);
   }

   /* decode tokens */

   if (pvalue->m.tokensPresent) {
      rtInvokeStartElement (pctxt, "tokens", -1);

      stat = asn1PD_H225_SeqOfH225ClearToken (pctxt, &pvalue->tokens);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "tokens", -1);
   }

   /* decode cryptoTokens */

   if (pvalue->m.cryptoTokensPresent) {
      rtInvokeStartElement (pctxt, "cryptoTokens", -1);

      stat = asn1PD_H225_SeqOfH225CryptoH323Token (pctxt, &pvalue->cryptoTokens);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "cryptoTokens", -1);
   }

   /* decode integrityCheckValue */

   if (pvalue->m.integrityCheckValuePresent) {
      rtInvokeStartElement (pctxt, "integrityCheckValue", -1);

      stat = asn1PD_H225ICV (pctxt, &pvalue->integrityCheckValue);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "integrityCheckValue", -1);
   }

   /* decode featureSet */

   if (pvalue->m.featureSetPresent) {
      rtInvokeStartElement (pctxt, "featureSet", -1);

      stat = asn1PD_H225FeatureSet (pctxt, &pvalue->featureSet);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "featureSet", -1);
   }

   /* decode genericData */

   if (pvalue->m.genericDataPresent) {
      rtInvokeStartElement (pctxt, "genericData", -1);

      stat = asn1PD_H225_SeqOfH225GenericData (pctxt, &pvalue->genericData);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "genericData", -1);
   }

   if (extbit) {

      /* decode extension optional bits length */

      stat = decodeSmallNonNegWholeNumber (pctxt, &bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      bitcnt += 1;

      ZEROCONTEXT (&lctxt);
      stat = setPERBufferUsingCtxt (&lctxt, pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = moveBitCursor (pctxt, bitcnt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      for (i = 0; i < bitcnt; i++) {
         DECODEBIT (&lctxt, &optbit);

         if (optbit) {
            stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            pOpenType = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
            pOpenType->numocts = openType.numocts;
            pOpenType->data = openType.data;
            dListAppend (pctxt, &pvalue->extElem1, pOpenType);
         }
         else {  /* unknown element */
            dListAppend (pctxt, &pvalue->extElem1, 0);
         }
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  _SeqOfH225AdmissionConfirm                                */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225_SeqOfH225AdmissionConfirm (OOCTXT* pctxt, H225_SeqOfH225AdmissionConfirm* pvalue)
{
   int stat = ASN_OK;
   H225AdmissionConfirm* pdata;
   ASN1UINT count = 0;
   ASN1UINT xx1;
   int lstat;

   dListInit (pvalue);

   for (;;) {
      /* decode length determinant */

      lstat = decodeLength (pctxt, &count);
      if (lstat != ASN_OK && lstat != ASN_OK_FRAG) {
         return LOG_ASN1ERR (pctxt, lstat);
      }

      /* decode elements */

      for (xx1 = 0; xx1 < count; xx1++) {
         rtInvokeStartElement (pctxt, "elem", xx1);

         pdata = ALLOC_ASN1ELEMDNODE (pctxt, H225AdmissionConfirm);
         if (pdata == NULL)
            return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

         stat = asn1PD_H225AdmissionConfirm (pctxt, pdata);
         if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
         rtInvokeEndElement (pctxt, "elem", xx1);

         dListAppendNode (pctxt, pvalue, pdata);
      }

      if(lstat == ASN_OK) break;
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  RasMessage                                                */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H225RasMessage (OOCTXT* pctxt, H225RasMessage* pvalue)
{
   int stat = ASN_OK;
   ASN1UINT ui;
   ASN1OpenType openType;
   ASN1BOOL extbit;
   OOCTXT lctxt;

   /* extension bit */

   stat = DECODEBIT (pctxt, &extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   if (!extbit) {
      stat = decodeConsUnsigned (pctxt, &ui, 0, 24);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      else pvalue->t = ui + 1;

      switch (ui) {
         /* gatekeeperRequest */
         case 0:
            rtInvokeStartElement (pctxt, "gatekeeperRequest", -1);

            pvalue->u.gatekeeperRequest = ALLOC_ASN1ELEM (pctxt, H225GatekeeperRequest);
            if (pvalue->u.gatekeeperRequest == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225GatekeeperRequest (pctxt, pvalue->u.gatekeeperRequest);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "gatekeeperRequest", -1);

            break;

         /* gatekeeperConfirm */
         case 1:
            rtInvokeStartElement (pctxt, "gatekeeperConfirm", -1);

            pvalue->u.gatekeeperConfirm = ALLOC_ASN1ELEM (pctxt, H225GatekeeperConfirm);
            if (pvalue->u.gatekeeperConfirm == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225GatekeeperConfirm (pctxt, pvalue->u.gatekeeperConfirm);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "gatekeeperConfirm", -1);

            break;

         /* gatekeeperReject */
         case 2:
            rtInvokeStartElement (pctxt, "gatekeeperReject", -1);

            pvalue->u.gatekeeperReject = ALLOC_ASN1ELEM (pctxt, H225GatekeeperReject);
            if (pvalue->u.gatekeeperReject == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225GatekeeperReject (pctxt, pvalue->u.gatekeeperReject);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "gatekeeperReject", -1);

            break;

         /* registrationRequest */
         case 3:
            rtInvokeStartElement (pctxt, "registrationRequest", -1);

            pvalue->u.registrationRequest = ALLOC_ASN1ELEM (pctxt, H225RegistrationRequest);
            if (pvalue->u.registrationRequest == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225RegistrationRequest (pctxt, pvalue->u.registrationRequest);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "registrationRequest", -1);

            break;

         /* registrationConfirm */
         case 4:
            rtInvokeStartElement (pctxt, "registrationConfirm", -1);

            pvalue->u.registrationConfirm = ALLOC_ASN1ELEM (pctxt, H225RegistrationConfirm);
            if (pvalue->u.registrationConfirm == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225RegistrationConfirm (pctxt, pvalue->u.registrationConfirm);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "registrationConfirm", -1);

            break;

         /* registrationReject */
         case 5:
            rtInvokeStartElement (pctxt, "registrationReject", -1);

            pvalue->u.registrationReject = ALLOC_ASN1ELEM (pctxt, H225RegistrationReject);
            if (pvalue->u.registrationReject == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225RegistrationReject (pctxt, pvalue->u.registrationReject);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "registrationReject", -1);

            break;

         /* unregistrationRequest */
         case 6:
            rtInvokeStartElement (pctxt, "unregistrationRequest", -1);

            pvalue->u.unregistrationRequest = ALLOC_ASN1ELEM (pctxt, H225UnregistrationRequest);
            if (pvalue->u.unregistrationRequest == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225UnregistrationRequest (pctxt, pvalue->u.unregistrationRequest);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "unregistrationRequest", -1);

            break;

         /* unregistrationConfirm */
         case 7:
            rtInvokeStartElement (pctxt, "unregistrationConfirm", -1);

            pvalue->u.unregistrationConfirm = ALLOC_ASN1ELEM (pctxt, H225UnregistrationConfirm);
            if (pvalue->u.unregistrationConfirm == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225UnregistrationConfirm (pctxt, pvalue->u.unregistrationConfirm);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "unregistrationConfirm", -1);

            break;

         /* unregistrationReject */
         case 8:
            rtInvokeStartElement (pctxt, "unregistrationReject", -1);

            pvalue->u.unregistrationReject = ALLOC_ASN1ELEM (pctxt, H225UnregistrationReject);
            if (pvalue->u.unregistrationReject == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225UnregistrationReject (pctxt, pvalue->u.unregistrationReject);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "unregistrationReject", -1);

            break;

         /* admissionRequest */
         case 9:
            rtInvokeStartElement (pctxt, "admissionRequest", -1);

            pvalue->u.admissionRequest = ALLOC_ASN1ELEM (pctxt, H225AdmissionRequest);
            if (pvalue->u.admissionRequest == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225AdmissionRequest (pctxt, pvalue->u.admissionRequest);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "admissionRequest", -1);

            break;

         /* admissionConfirm */
         case 10:
            rtInvokeStartElement (pctxt, "admissionConfirm", -1);

            pvalue->u.admissionConfirm = ALLOC_ASN1ELEM (pctxt, H225AdmissionConfirm);
            if (pvalue->u.admissionConfirm == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225AdmissionConfirm (pctxt, pvalue->u.admissionConfirm);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "admissionConfirm", -1);

            break;

         /* admissionReject */
         case 11:
            rtInvokeStartElement (pctxt, "admissionReject", -1);

            pvalue->u.admissionReject = ALLOC_ASN1ELEM (pctxt, H225AdmissionReject);
            if (pvalue->u.admissionReject == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225AdmissionReject (pctxt, pvalue->u.admissionReject);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "admissionReject", -1);

            break;

         /* bandwidthRequest */
         case 12:
            rtInvokeStartElement (pctxt, "bandwidthRequest", -1);

            pvalue->u.bandwidthRequest = ALLOC_ASN1ELEM (pctxt, H225BandwidthRequest);
            if (pvalue->u.bandwidthRequest == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225BandwidthRequest (pctxt, pvalue->u.bandwidthRequest);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "bandwidthRequest", -1);

            break;

         /* bandwidthConfirm */
         case 13:
            rtInvokeStartElement (pctxt, "bandwidthConfirm", -1);

            pvalue->u.bandwidthConfirm = ALLOC_ASN1ELEM (pctxt, H225BandwidthConfirm);
            if (pvalue->u.bandwidthConfirm == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225BandwidthConfirm (pctxt, pvalue->u.bandwidthConfirm);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "bandwidthConfirm", -1);

            break;

         /* bandwidthReject */
         case 14:
            rtInvokeStartElement (pctxt, "bandwidthReject", -1);

            pvalue->u.bandwidthReject = ALLOC_ASN1ELEM (pctxt, H225BandwidthReject);
            if (pvalue->u.bandwidthReject == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225BandwidthReject (pctxt, pvalue->u.bandwidthReject);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "bandwidthReject", -1);

            break;

         /* disengageRequest */
         case 15:
            rtInvokeStartElement (pctxt, "disengageRequest", -1);

            pvalue->u.disengageRequest = ALLOC_ASN1ELEM (pctxt, H225DisengageRequest);
            if (pvalue->u.disengageRequest == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225DisengageRequest (pctxt, pvalue->u.disengageRequest);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "disengageRequest", -1);

            break;

         /* disengageConfirm */
         case 16:
            rtInvokeStartElement (pctxt, "disengageConfirm", -1);

            pvalue->u.disengageConfirm = ALLOC_ASN1ELEM (pctxt, H225DisengageConfirm);
            if (pvalue->u.disengageConfirm == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225DisengageConfirm (pctxt, pvalue->u.disengageConfirm);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "disengageConfirm", -1);

            break;

         /* disengageReject */
         case 17:
            rtInvokeStartElement (pctxt, "disengageReject", -1);

            pvalue->u.disengageReject = ALLOC_ASN1ELEM (pctxt, H225DisengageReject);
            if (pvalue->u.disengageReject == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225DisengageReject (pctxt, pvalue->u.disengageReject);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "disengageReject", -1);

            break;

         /* locationRequest */
         case 18:
            rtInvokeStartElement (pctxt, "locationRequest", -1);

            pvalue->u.locationRequest = ALLOC_ASN1ELEM (pctxt, H225LocationRequest);
            if (pvalue->u.locationRequest == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225LocationRequest (pctxt, pvalue->u.locationRequest);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "locationRequest", -1);

            break;

         /* locationConfirm */
         case 19:
            rtInvokeStartElement (pctxt, "locationConfirm", -1);

            pvalue->u.locationConfirm = ALLOC_ASN1ELEM (pctxt, H225LocationConfirm);
            if (pvalue->u.locationConfirm == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225LocationConfirm (pctxt, pvalue->u.locationConfirm);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "locationConfirm", -1);

            break;

         /* locationReject */
         case 20:
            rtInvokeStartElement (pctxt, "locationReject", -1);

            pvalue->u.locationReject = ALLOC_ASN1ELEM (pctxt, H225LocationReject);
            if (pvalue->u.locationReject == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225LocationReject (pctxt, pvalue->u.locationReject);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "locationReject", -1);

            break;

         /* infoRequest */
         case 21:
            rtInvokeStartElement (pctxt, "infoRequest", -1);

            pvalue->u.infoRequest = ALLOC_ASN1ELEM (pctxt, H225InfoRequest);
            if (pvalue->u.infoRequest == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225InfoRequest (pctxt, pvalue->u.infoRequest);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "infoRequest", -1);

            break;

         /* infoRequestResponse */
         case 22:
            rtInvokeStartElement (pctxt, "infoRequestResponse", -1);

            pvalue->u.infoRequestResponse = ALLOC_ASN1ELEM (pctxt, H225InfoRequestResponse);
            if (pvalue->u.infoRequestResponse == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225InfoRequestResponse (pctxt, pvalue->u.infoRequestResponse);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "infoRequestResponse", -1);

            break;

         /* nonStandardMessage */
         case 23:
            rtInvokeStartElement (pctxt, "nonStandardMessage", -1);

            pvalue->u.nonStandardMessage = ALLOC_ASN1ELEM (pctxt, H225NonStandardMessage);
            if (pvalue->u.nonStandardMessage == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225NonStandardMessage (pctxt, pvalue->u.nonStandardMessage);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "nonStandardMessage", -1);

            break;

         /* unknownMessageResponse */
         case 24:
            rtInvokeStartElement (pctxt, "unknownMessageResponse", -1);

            pvalue->u.unknownMessageResponse = ALLOC_ASN1ELEM (pctxt, H225UnknownMessageResponse);
            if (pvalue->u.unknownMessageResponse == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225UnknownMessageResponse (pctxt, pvalue->u.unknownMessageResponse);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "unknownMessageResponse", -1);

            break;

         default:
            return LOG_ASN1ERR (pctxt, ASN_E_INVOPT);
      }
   }
   else {
      stat = decodeSmallNonNegWholeNumber (pctxt, &ui);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      else pvalue->t = ui + 26;

      stat = decodeByteAlign (pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      copyContext (&lctxt, pctxt);
      initContextBuffer (pctxt, openType.data, openType.numocts);

      switch (pvalue->t) {
         /* requestInProgress */
         case 26:
            rtInvokeStartElement (pctxt, "requestInProgress", -1);

            pvalue->u.requestInProgress = ALLOC_ASN1ELEM (pctxt, H225RequestInProgress);
            if (pvalue->u.requestInProgress == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225RequestInProgress (pctxt, pvalue->u.requestInProgress);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "requestInProgress", -1);

            break;

         /* resourcesAvailableIndicate */
         case 27:
            rtInvokeStartElement (pctxt, "resourcesAvailableIndicate", -1);

            pvalue->u.resourcesAvailableIndicate = ALLOC_ASN1ELEM (pctxt, H225ResourcesAvailableIndicate);
            if (pvalue->u.resourcesAvailableIndicate == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225ResourcesAvailableIndicate (pctxt, pvalue->u.resourcesAvailableIndicate);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "resourcesAvailableIndicate", -1);

            break;

         /* resourcesAvailableConfirm */
         case 28:
            rtInvokeStartElement (pctxt, "resourcesAvailableConfirm", -1);

            pvalue->u.resourcesAvailableConfirm = ALLOC_ASN1ELEM (pctxt, H225ResourcesAvailableConfirm);
            if (pvalue->u.resourcesAvailableConfirm == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225ResourcesAvailableConfirm (pctxt, pvalue->u.resourcesAvailableConfirm);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "resourcesAvailableConfirm", -1);

            break;

         /* infoRequestAck */
         case 29:
            rtInvokeStartElement (pctxt, "infoRequestAck", -1);

            pvalue->u.infoRequestAck = ALLOC_ASN1ELEM (pctxt, H225InfoRequestAck);
            if (pvalue->u.infoRequestAck == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225InfoRequestAck (pctxt, pvalue->u.infoRequestAck);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "infoRequestAck", -1);

            break;

         /* infoRequestNak */
         case 30:
            rtInvokeStartElement (pctxt, "infoRequestNak", -1);

            pvalue->u.infoRequestNak = ALLOC_ASN1ELEM (pctxt, H225InfoRequestNak);
            if (pvalue->u.infoRequestNak == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225InfoRequestNak (pctxt, pvalue->u.infoRequestNak);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "infoRequestNak", -1);

            break;

         /* serviceControlIndication */
         case 31:
            rtInvokeStartElement (pctxt, "serviceControlIndication", -1);

            pvalue->u.serviceControlIndication = ALLOC_ASN1ELEM (pctxt, H225ServiceControlIndication);
            if (pvalue->u.serviceControlIndication == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225ServiceControlIndication (pctxt, pvalue->u.serviceControlIndication);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "serviceControlIndication", -1);

            break;

         /* serviceControlResponse */
         case 32:
            rtInvokeStartElement (pctxt, "serviceControlResponse", -1);

            pvalue->u.serviceControlResponse = ALLOC_ASN1ELEM (pctxt, H225ServiceControlResponse);
            if (pvalue->u.serviceControlResponse == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225ServiceControlResponse (pctxt, pvalue->u.serviceControlResponse);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "serviceControlResponse", -1);

            break;

         /* admissionConfirmSequence */
         case 33:
            rtInvokeStartElement (pctxt, "admissionConfirmSequence", -1);

            pvalue->u.admissionConfirmSequence = ALLOC_ASN1ELEM (pctxt, H225_SeqOfH225AdmissionConfirm);
            if (pvalue->u.admissionConfirmSequence == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H225_SeqOfH225AdmissionConfirm (pctxt, pvalue->u.admissionConfirmSequence);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "admissionConfirmSequence", -1);

            break;

         default:
            pvalue->u.extElem1 = ALLOC_ASN1ELEM (pctxt, ASN1OpenType);
            if (pvalue->u.extElem1 == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            pvalue->u.extElem1->data = openType.data;
            pvalue->u.extElem1->numocts = openType.numocts;

      }

      copyContext (pctxt, &lctxt);
   }

   return (stat);
}

