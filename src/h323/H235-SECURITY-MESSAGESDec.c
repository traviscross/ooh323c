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
#include "H235-SECURITY-MESSAGES.h"
#include "asn1CEvtHndlr.h"

/**************************************************************/
/*                                                            */
/*  ChallengeString                                           */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H235ChallengeString (OOCTXT* pctxt, H235ChallengeString* pvalue)
{
   static Asn1SizeCnst lsize1 = { 0, 8, 128, 0 };
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
/*  TimeStamp                                                 */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H235TimeStamp (OOCTXT* pctxt, H235TimeStamp* pvalue)
{
   int stat = ASN_OK;

   stat = decodeConsUnsigned (pctxt, pvalue, 1U, ASN1UINT_MAX);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   rtInvokeUIntValue (pctxt, *pvalue);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  RandomVal                                                 */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H235RandomVal (OOCTXT* pctxt, H235RandomVal* pvalue)
{
   int stat = ASN_OK;

   stat = decodeUnconsInteger (pctxt, pvalue);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   rtInvokeIntValue (pctxt, *pvalue);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  Password                                                  */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H235Password (OOCTXT* pctxt, H235Password* pvalue)
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
/*  Identifier                                                */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H235Identifier (OOCTXT* pctxt, H235Identifier* pvalue)
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
/*  KeyMaterial                                               */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H235KeyMaterial (OOCTXT* pctxt, H235KeyMaterial* pvalue)
{
   static Asn1SizeCnst lsize1 = { 0, 1, 2048, 0 };
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
/*  IV8                                                       */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H235IV8 (OOCTXT* pctxt, H235IV8* pvalue)
{
   static Asn1SizeCnst lsize1 = { 0, 8, 8, 0 };
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
/*  IV16                                                      */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H235IV16 (OOCTXT* pctxt, H235IV16* pvalue)
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
/*  EncodedGeneralToken                                       */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H235EncodedGeneralToken (OOCTXT* pctxt, H235EncodedGeneralToken* pvalue)
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
/*  DHset_halfkey                                             */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H235DHset_halfkey (OOCTXT* pctxt, H235DHset_halfkey* pvalue)
{
   static Asn1SizeCnst lsize1 = { 0, 0, 2048, 0 };
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
/*  DHset_modSize                                             */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H235DHset_modSize (OOCTXT* pctxt, H235DHset_modSize* pvalue)
{
   static Asn1SizeCnst lsize1 = { 0, 0, 2048, 0 };
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
/*  DHset_generator                                           */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H235DHset_generator (OOCTXT* pctxt, H235DHset_generator* pvalue)
{
   static Asn1SizeCnst lsize1 = { 0, 0, 2048, 0 };
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
/*  DHset                                                     */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H235DHset (OOCTXT* pctxt, H235DHset* pvalue)
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

   /* decode halfkey */

   rtInvokeStartElement (pctxt, "halfkey", -1);

   stat = asn1PD_H235DHset_halfkey (pctxt, &pvalue->halfkey);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "halfkey", -1);

   /* decode modSize */

   rtInvokeStartElement (pctxt, "modSize", -1);

   stat = asn1PD_H235DHset_modSize (pctxt, &pvalue->modSize);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "modSize", -1);

   /* decode generator */

   rtInvokeStartElement (pctxt, "generator", -1);

   stat = asn1PD_H235DHset_generator (pctxt, &pvalue->generator);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "generator", -1);

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
/*  TypedCertificate                                          */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H235TypedCertificate (OOCTXT* pctxt, H235TypedCertificate* pvalue)
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

   /* decode type */

   rtInvokeStartElement (pctxt, "type", -1);

   stat = decodeObjectIdentifier (pctxt, &pvalue->type);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   rtInvokeOidValue (pctxt, pvalue->type.numids, pvalue->type.subid);

   rtInvokeEndElement (pctxt, "type", -1);

   /* decode certificate */

   rtInvokeStartElement (pctxt, "certificate", -1);

   stat = decodeDynOctetString (pctxt, (ASN1DynOctStr*)&pvalue->certificate);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   rtInvokeOctStrValue (pctxt, pvalue->certificate.numocts, pvalue->certificate.data);

   rtInvokeEndElement (pctxt, "certificate", -1);

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
/*  NonStandardParameter                                      */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H235NonStandardParameter (OOCTXT* pctxt, H235NonStandardParameter* pvalue)
{
   int stat = ASN_OK;

   /* decode nonStandardIdentifier */

   rtInvokeStartElement (pctxt, "nonStandardIdentifier", -1);

   stat = decodeObjectIdentifier (pctxt, &pvalue->nonStandardIdentifier);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   rtInvokeOidValue (pctxt, pvalue->nonStandardIdentifier.numids, pvalue->nonStandardIdentifier.subid);

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
/*  ECpoint_x                                                 */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H235ECpoint_x (OOCTXT* pctxt, H235ECpoint_x* pvalue)
{
   static Asn1SizeCnst lsize1 = { 0, 0, 511, 0 };
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
/*  ECpoint_y                                                 */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H235ECpoint_y (OOCTXT* pctxt, H235ECpoint_y* pvalue)
{
   static Asn1SizeCnst lsize1 = { 0, 0, 511, 0 };
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
/*  ECpoint                                                   */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H235ECpoint (OOCTXT* pctxt, H235ECpoint* pvalue)
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
   else pvalue->m.xPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.yPresent = optbit;

   /* decode x */

   if (pvalue->m.xPresent) {
      rtInvokeStartElement (pctxt, "x", -1);

      stat = asn1PD_H235ECpoint_x (pctxt, &pvalue->x);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "x", -1);
   }

   /* decode y */

   if (pvalue->m.yPresent) {
      rtInvokeStartElement (pctxt, "y", -1);

      stat = asn1PD_H235ECpoint_y (pctxt, &pvalue->y);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "y", -1);
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
/*  ECKASDH_eckasdhp_modulus                                  */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H235ECKASDH_eckasdhp_modulus (OOCTXT* pctxt, H235ECKASDH_eckasdhp_modulus* pvalue)
{
   static Asn1SizeCnst lsize1 = { 0, 0, 511, 0 };
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
/*  ECKASDH_eckasdhp_weierstrassA                             */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H235ECKASDH_eckasdhp_weierstrassA (OOCTXT* pctxt, H235ECKASDH_eckasdhp_weierstrassA* pvalue)
{
   static Asn1SizeCnst lsize1 = { 0, 0, 511, 0 };
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
/*  ECKASDH_eckasdhp_weierstrassB                             */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H235ECKASDH_eckasdhp_weierstrassB (OOCTXT* pctxt, H235ECKASDH_eckasdhp_weierstrassB* pvalue)
{
   static Asn1SizeCnst lsize1 = { 0, 0, 511, 0 };
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
/*  ECKASDH_eckasdhp                                          */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H235ECKASDH_eckasdhp (OOCTXT* pctxt, H235ECKASDH_eckasdhp* pvalue)
{
   int stat = ASN_OK;

   /* decode public_key */

   rtInvokeStartElement (pctxt, "public_key", -1);

   stat = asn1PD_H235ECpoint (pctxt, &pvalue->public_key);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "public_key", -1);

   /* decode modulus */

   rtInvokeStartElement (pctxt, "modulus", -1);

   stat = asn1PD_H235ECKASDH_eckasdhp_modulus (pctxt, &pvalue->modulus);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "modulus", -1);

   /* decode base */

   rtInvokeStartElement (pctxt, "base", -1);

   stat = asn1PD_H235ECpoint (pctxt, &pvalue->base);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "base", -1);

   /* decode weierstrassA */

   rtInvokeStartElement (pctxt, "weierstrassA", -1);

   stat = asn1PD_H235ECKASDH_eckasdhp_weierstrassA (pctxt, &pvalue->weierstrassA);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "weierstrassA", -1);

   /* decode weierstrassB */

   rtInvokeStartElement (pctxt, "weierstrassB", -1);

   stat = asn1PD_H235ECKASDH_eckasdhp_weierstrassB (pctxt, &pvalue->weierstrassB);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "weierstrassB", -1);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  ECKASDH_eckasdh2_fieldSize                                */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H235ECKASDH_eckasdh2_fieldSize (OOCTXT* pctxt, H235ECKASDH_eckasdh2_fieldSize* pvalue)
{
   static Asn1SizeCnst lsize1 = { 0, 0, 511, 0 };
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
/*  ECKASDH_eckasdh2_weierstrassA                             */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H235ECKASDH_eckasdh2_weierstrassA (OOCTXT* pctxt, H235ECKASDH_eckasdh2_weierstrassA* pvalue)
{
   static Asn1SizeCnst lsize1 = { 0, 0, 511, 0 };
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
/*  ECKASDH_eckasdh2_weierstrassB                             */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H235ECKASDH_eckasdh2_weierstrassB (OOCTXT* pctxt, H235ECKASDH_eckasdh2_weierstrassB* pvalue)
{
   static Asn1SizeCnst lsize1 = { 0, 0, 511, 0 };
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
/*  ECKASDH_eckasdh2                                          */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H235ECKASDH_eckasdh2 (OOCTXT* pctxt, H235ECKASDH_eckasdh2* pvalue)
{
   int stat = ASN_OK;

   /* decode public_key */

   rtInvokeStartElement (pctxt, "public_key", -1);

   stat = asn1PD_H235ECpoint (pctxt, &pvalue->public_key);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "public_key", -1);

   /* decode fieldSize */

   rtInvokeStartElement (pctxt, "fieldSize", -1);

   stat = asn1PD_H235ECKASDH_eckasdh2_fieldSize (pctxt, &pvalue->fieldSize);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "fieldSize", -1);

   /* decode base */

   rtInvokeStartElement (pctxt, "base", -1);

   stat = asn1PD_H235ECpoint (pctxt, &pvalue->base);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "base", -1);

   /* decode weierstrassA */

   rtInvokeStartElement (pctxt, "weierstrassA", -1);

   stat = asn1PD_H235ECKASDH_eckasdh2_weierstrassA (pctxt, &pvalue->weierstrassA);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "weierstrassA", -1);

   /* decode weierstrassB */

   rtInvokeStartElement (pctxt, "weierstrassB", -1);

   stat = asn1PD_H235ECKASDH_eckasdh2_weierstrassB (pctxt, &pvalue->weierstrassB);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "weierstrassB", -1);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  ECKASDH                                                   */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H235ECKASDH (OOCTXT* pctxt, H235ECKASDH* pvalue)
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
         /* eckasdhp */
         case 0:
            rtInvokeStartElement (pctxt, "eckasdhp", -1);

            pvalue->u.eckasdhp = ALLOC_ASN1ELEM (pctxt, H235ECKASDH_eckasdhp);
            if (pvalue->u.eckasdhp == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H235ECKASDH_eckasdhp (pctxt, pvalue->u.eckasdhp);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "eckasdhp", -1);

            break;

         /* eckasdh2 */
         case 1:
            rtInvokeStartElement (pctxt, "eckasdh2", -1);

            pvalue->u.eckasdh2 = ALLOC_ASN1ELEM (pctxt, H235ECKASDH_eckasdh2);
            if (pvalue->u.eckasdh2 == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H235ECKASDH_eckasdh2 (pctxt, pvalue->u.eckasdh2);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "eckasdh2", -1);

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
/*  ClearToken                                                */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H235ClearToken (OOCTXT* pctxt, H235ClearToken* pvalue)
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
   else pvalue->m.timeStampPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.passwordPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.dhkeyPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.challengePresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.randomPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.certificatePresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.generalIDPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.nonStandardPresent = optbit;

   /* decode tokenOID */

   rtInvokeStartElement (pctxt, "tokenOID", -1);

   stat = decodeObjectIdentifier (pctxt, &pvalue->tokenOID);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   rtInvokeOidValue (pctxt, pvalue->tokenOID.numids, pvalue->tokenOID.subid);

   rtInvokeEndElement (pctxt, "tokenOID", -1);

   /* decode timeStamp */

   if (pvalue->m.timeStampPresent) {
      rtInvokeStartElement (pctxt, "timeStamp", -1);

      stat = asn1PD_H235TimeStamp (pctxt, &pvalue->timeStamp);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "timeStamp", -1);
   }

   /* decode password */

   if (pvalue->m.passwordPresent) {
      rtInvokeStartElement (pctxt, "password", -1);

      stat = asn1PD_H235Password (pctxt, &pvalue->password);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "password", -1);
   }

   /* decode dhkey */

   if (pvalue->m.dhkeyPresent) {
      rtInvokeStartElement (pctxt, "dhkey", -1);

      stat = asn1PD_H235DHset (pctxt, &pvalue->dhkey);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "dhkey", -1);
   }

   /* decode challenge */

   if (pvalue->m.challengePresent) {
      rtInvokeStartElement (pctxt, "challenge", -1);

      stat = asn1PD_H235ChallengeString (pctxt, &pvalue->challenge);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "challenge", -1);
   }

   /* decode random */

   if (pvalue->m.randomPresent) {
      rtInvokeStartElement (pctxt, "random", -1);

      stat = asn1PD_H235RandomVal (pctxt, &pvalue->random);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "random", -1);
   }

   /* decode certificate */

   if (pvalue->m.certificatePresent) {
      rtInvokeStartElement (pctxt, "certificate", -1);

      stat = asn1PD_H235TypedCertificate (pctxt, &pvalue->certificate);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "certificate", -1);
   }

   /* decode generalID */

   if (pvalue->m.generalIDPresent) {
      rtInvokeStartElement (pctxt, "generalID", -1);

      stat = asn1PD_H235Identifier (pctxt, &pvalue->generalID);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "generalID", -1);
   }

   /* decode nonStandard */

   if (pvalue->m.nonStandardPresent) {
      rtInvokeStartElement (pctxt, "nonStandard", -1);

      stat = asn1PD_H235NonStandardParameter (pctxt, &pvalue->nonStandard);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "nonStandard", -1);
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
                     pvalue->m.eckasdhkeyPresent = 1;

                     rtInvokeStartElement (pctxt, "eckasdhkey", -1);

                     stat = asn1PD_H235ECKASDH (pctxt, &pvalue->eckasdhkey);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "eckasdhkey", -1);
                     break;

                  case 1:
                     pvalue->m.sendersIDPresent = 1;

                     rtInvokeStartElement (pctxt, "sendersID", -1);

                     stat = asn1PD_H235Identifier (pctxt, &pvalue->sendersID);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "sendersID", -1);
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
/*  PwdCertToken                                              */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H235PwdCertToken (OOCTXT* pctxt, H235PwdCertToken* pvalue)
{
   int stat = ASN_OK;

   stat = asn1PD_H235ClearToken (pctxt, pvalue);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  EncodedPwdCertToken                                       */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H235EncodedPwdCertToken (OOCTXT* pctxt, H235EncodedPwdCertToken* pvalue)
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
/*  EncodedKeySignedMaterial                                  */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H235EncodedKeySignedMaterial (OOCTXT* pctxt, H235EncodedKeySignedMaterial* pvalue)
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
/*  EncodedReturnSig                                          */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H235EncodedReturnSig (OOCTXT* pctxt, H235EncodedReturnSig* pvalue)
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
/*  EncodedKeySyncMaterial                                    */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H235EncodedKeySyncMaterial (OOCTXT* pctxt, H235EncodedKeySyncMaterial* pvalue)
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
/*  ECGDSASignature_r                                         */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H235ECGDSASignature_r (OOCTXT* pctxt, H235ECGDSASignature_r* pvalue)
{
   static Asn1SizeCnst lsize1 = { 0, 0, 511, 0 };
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
/*  ECGDSASignature_s                                         */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H235ECGDSASignature_s (OOCTXT* pctxt, H235ECGDSASignature_s* pvalue)
{
   static Asn1SizeCnst lsize1 = { 0, 0, 511, 0 };
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
/*  ECGDSASignature                                           */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H235ECGDSASignature (OOCTXT* pctxt, H235ECGDSASignature* pvalue)
{
   int stat = ASN_OK;

   /* decode r */

   rtInvokeStartElement (pctxt, "r", -1);

   stat = asn1PD_H235ECGDSASignature_r (pctxt, &pvalue->r);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "r", -1);

   /* decode s */

   rtInvokeStartElement (pctxt, "s", -1);

   stat = asn1PD_H235ECGDSASignature_s (pctxt, &pvalue->s);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "s", -1);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  AuthenticationBES                                         */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H235AuthenticationBES (OOCTXT* pctxt, H235AuthenticationBES* pvalue)
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
         /* default_ */
         case 0:
            rtInvokeStartElement (pctxt, "default_", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "default_", -1);

            break;

         /* radius */
         case 1:
            rtInvokeStartElement (pctxt, "radius", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "radius", -1);

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
/*  AuthenticationMechanism                                   */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H235AuthenticationMechanism (OOCTXT* pctxt, H235AuthenticationMechanism* pvalue)
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
         /* dhExch */
         case 0:
            rtInvokeStartElement (pctxt, "dhExch", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "dhExch", -1);

            break;

         /* pwdSymEnc */
         case 1:
            rtInvokeStartElement (pctxt, "pwdSymEnc", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "pwdSymEnc", -1);

            break;

         /* pwdHash */
         case 2:
            rtInvokeStartElement (pctxt, "pwdHash", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "pwdHash", -1);

            break;

         /* certSign */
         case 3:
            rtInvokeStartElement (pctxt, "certSign", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "certSign", -1);

            break;

         /* ipsec */
         case 4:
            rtInvokeStartElement (pctxt, "ipsec", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "ipsec", -1);

            break;

         /* tls */
         case 5:
            rtInvokeStartElement (pctxt, "tls", -1);

            /* NULL */
            rtInvokeNullValue (pctxt);

            rtInvokeEndElement (pctxt, "tls", -1);

            break;

         /* nonStandard */
         case 6:
            rtInvokeStartElement (pctxt, "nonStandard", -1);

            pvalue->u.nonStandard = ALLOC_ASN1ELEM (pctxt, H235NonStandardParameter);
            if (pvalue->u.nonStandard == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H235NonStandardParameter (pctxt, pvalue->u.nonStandard);
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
      else pvalue->t = ui + 8;

      stat = decodeByteAlign (pctxt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = decodeOpenType (pctxt, &openType.data, &openType.numocts);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      copyContext (&lctxt, pctxt);
      initContextBuffer (pctxt, openType.data, openType.numocts);

      switch (pvalue->t) {
         /* authenticationBES */
         case 8:
            rtInvokeStartElement (pctxt, "authenticationBES", -1);

            pvalue->u.authenticationBES = ALLOC_ASN1ELEM (pctxt, H235AuthenticationBES);
            if (pvalue->u.authenticationBES == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H235AuthenticationBES (pctxt, pvalue->u.authenticationBES);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "authenticationBES", -1);

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
/*  Params                                                    */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H235Params (OOCTXT* pctxt, H235Params* pvalue)
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
   else pvalue->m.ranIntPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.iv8Present = optbit;

   /* decode ranInt */

   if (pvalue->m.ranIntPresent) {
      rtInvokeStartElement (pctxt, "ranInt", -1);

      stat = decodeUnconsInteger (pctxt, &pvalue->ranInt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      rtInvokeIntValue (pctxt, pvalue->ranInt);

      rtInvokeEndElement (pctxt, "ranInt", -1);
   }

   /* decode iv8 */

   if (pvalue->m.iv8Present) {
      rtInvokeStartElement (pctxt, "iv8", -1);

      stat = asn1PD_H235IV8 (pctxt, &pvalue->iv8);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "iv8", -1);
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
                     pvalue->m.iv16Present = 1;

                     rtInvokeStartElement (pctxt, "iv16", -1);

                     stat = asn1PD_H235IV16 (pctxt, &pvalue->iv16);
                     if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

                     rtInvokeEndElement (pctxt, "iv16", -1);
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
/*  ENCRYPTED                                                 */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H235ENCRYPTED (OOCTXT* pctxt, H235ENCRYPTED* pvalue)
{
   int stat = ASN_OK;

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

   /* decode encryptedData */

   rtInvokeStartElement (pctxt, "encryptedData", -1);

   stat = decodeDynOctetString (pctxt, (ASN1DynOctStr*)&pvalue->encryptedData);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   rtInvokeOctStrValue (pctxt, pvalue->encryptedData.numocts, pvalue->encryptedData.data);

   rtInvokeEndElement (pctxt, "encryptedData", -1);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  CryptoToken_cryptoEncryptedToken                          */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H235CryptoToken_cryptoEncryptedToken (OOCTXT* pctxt, H235CryptoToken_cryptoEncryptedToken* pvalue)
{
   int stat = ASN_OK;

   /* decode tokenOID */

   rtInvokeStartElement (pctxt, "tokenOID", -1);

   stat = decodeObjectIdentifier (pctxt, &pvalue->tokenOID);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   rtInvokeOidValue (pctxt, pvalue->tokenOID.numids, pvalue->tokenOID.subid);

   rtInvokeEndElement (pctxt, "tokenOID", -1);

   /* decode token */

   rtInvokeStartElement (pctxt, "token", -1);

   stat = asn1PD_H235ENCRYPTED (pctxt, &pvalue->token);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "token", -1);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  CryptoToken_cryptoSignedToken_token                       */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H235CryptoToken_cryptoSignedToken_token (OOCTXT* pctxt, H235CryptoToken_cryptoSignedToken_token* pvalue)
{
   int stat = ASN_OK;

   /* decode toBeSigned */

   rtInvokeStartElement (pctxt, "toBeSigned", -1);

   stat = asn1PD_H235EncodedGeneralToken (pctxt, &pvalue->toBeSigned);
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
/*  CryptoToken_cryptoSignedToken                             */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H235CryptoToken_cryptoSignedToken (OOCTXT* pctxt, H235CryptoToken_cryptoSignedToken* pvalue)
{
   int stat = ASN_OK;

   /* decode tokenOID */

   rtInvokeStartElement (pctxt, "tokenOID", -1);

   stat = decodeObjectIdentifier (pctxt, &pvalue->tokenOID);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   rtInvokeOidValue (pctxt, pvalue->tokenOID.numids, pvalue->tokenOID.subid);

   rtInvokeEndElement (pctxt, "tokenOID", -1);

   /* decode token */

   rtInvokeStartElement (pctxt, "token", -1);

   stat = asn1PD_H235CryptoToken_cryptoSignedToken_token (pctxt, &pvalue->token);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "token", -1);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  HASHED                                                    */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H235HASHED (OOCTXT* pctxt, H235HASHED* pvalue)
{
   int stat = ASN_OK;

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

   /* decode hash */

   rtInvokeStartElement (pctxt, "hash", -1);

   stat = decodeDynBitString (pctxt, (ASN1DynBitStr*)&pvalue->hash);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeBitStrValue (pctxt, pvalue->hash.numbits, pvalue->hash.data);

   rtInvokeEndElement (pctxt, "hash", -1);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  CryptoToken_cryptoHashedToken                             */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H235CryptoToken_cryptoHashedToken (OOCTXT* pctxt, H235CryptoToken_cryptoHashedToken* pvalue)
{
   int stat = ASN_OK;

   /* decode tokenOID */

   rtInvokeStartElement (pctxt, "tokenOID", -1);

   stat = decodeObjectIdentifier (pctxt, &pvalue->tokenOID);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   rtInvokeOidValue (pctxt, pvalue->tokenOID.numids, pvalue->tokenOID.subid);

   rtInvokeEndElement (pctxt, "tokenOID", -1);

   /* decode hashedVals */

   rtInvokeStartElement (pctxt, "hashedVals", -1);

   stat = asn1PD_H235ClearToken (pctxt, &pvalue->hashedVals);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "hashedVals", -1);

   /* decode token */

   rtInvokeStartElement (pctxt, "token", -1);

   stat = asn1PD_H235HASHED (pctxt, &pvalue->token);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "token", -1);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  CryptoToken                                               */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H235CryptoToken (OOCTXT* pctxt, H235CryptoToken* pvalue)
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
         /* cryptoEncryptedToken */
         case 0:
            rtInvokeStartElement (pctxt, "cryptoEncryptedToken", -1);

            pvalue->u.cryptoEncryptedToken = ALLOC_ASN1ELEM (pctxt, H235CryptoToken_cryptoEncryptedToken);
            if (pvalue->u.cryptoEncryptedToken == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H235CryptoToken_cryptoEncryptedToken (pctxt, pvalue->u.cryptoEncryptedToken);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "cryptoEncryptedToken", -1);

            break;

         /* cryptoSignedToken */
         case 1:
            rtInvokeStartElement (pctxt, "cryptoSignedToken", -1);

            pvalue->u.cryptoSignedToken = ALLOC_ASN1ELEM (pctxt, H235CryptoToken_cryptoSignedToken);
            if (pvalue->u.cryptoSignedToken == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H235CryptoToken_cryptoSignedToken (pctxt, pvalue->u.cryptoSignedToken);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "cryptoSignedToken", -1);

            break;

         /* cryptoHashedToken */
         case 2:
            rtInvokeStartElement (pctxt, "cryptoHashedToken", -1);

            pvalue->u.cryptoHashedToken = ALLOC_ASN1ELEM (pctxt, H235CryptoToken_cryptoHashedToken);
            if (pvalue->u.cryptoHashedToken == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H235CryptoToken_cryptoHashedToken (pctxt, pvalue->u.cryptoHashedToken);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "cryptoHashedToken", -1);

            break;

         /* cryptoPwdEncr */
         case 3:
            rtInvokeStartElement (pctxt, "cryptoPwdEncr", -1);

            pvalue->u.cryptoPwdEncr = ALLOC_ASN1ELEM (pctxt, H235ENCRYPTED);
            if (pvalue->u.cryptoPwdEncr == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H235ENCRYPTED (pctxt, pvalue->u.cryptoPwdEncr);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "cryptoPwdEncr", -1);

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
/*  H235Key_certProtectedKey                                  */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H235H235Key_certProtectedKey (OOCTXT* pctxt, H235H235Key_certProtectedKey* pvalue)
{
   int stat = ASN_OK;

   /* decode toBeSigned */

   rtInvokeStartElement (pctxt, "toBeSigned", -1);

   stat = asn1PD_H235EncodedKeySignedMaterial (pctxt, &pvalue->toBeSigned);
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
/*  H235Key                                                   */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H235H235Key (OOCTXT* pctxt, H235H235Key* pvalue)
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
         /* secureChannel */
         case 0:
            rtInvokeStartElement (pctxt, "secureChannel", -1);

            pvalue->u.secureChannel = ALLOC_ASN1ELEM (pctxt, H235KeyMaterial);
            if (pvalue->u.secureChannel == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H235KeyMaterial (pctxt, pvalue->u.secureChannel);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "secureChannel", -1);

            break;

         /* sharedSecret */
         case 1:
            rtInvokeStartElement (pctxt, "sharedSecret", -1);

            pvalue->u.sharedSecret = ALLOC_ASN1ELEM (pctxt, H235ENCRYPTED);
            if (pvalue->u.sharedSecret == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H235ENCRYPTED (pctxt, pvalue->u.sharedSecret);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "sharedSecret", -1);

            break;

         /* certProtectedKey */
         case 2:
            rtInvokeStartElement (pctxt, "certProtectedKey", -1);

            pvalue->u.certProtectedKey = ALLOC_ASN1ELEM (pctxt, H235H235Key_certProtectedKey);
            if (pvalue->u.certProtectedKey == NULL)
               return LOG_ASN1ERR (pctxt, ASN_E_NOMEM);

            stat = asn1PD_H235H235Key_certProtectedKey (pctxt, pvalue->u.certProtectedKey);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

            rtInvokeEndElement (pctxt, "certProtectedKey", -1);

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
/*  KeySignedMaterial                                         */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H235KeySignedMaterial (OOCTXT* pctxt, H235KeySignedMaterial* pvalue)
{
   int stat = ASN_OK;
   ASN1BOOL optbit;

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.srandomPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.timeStampPresent = optbit;

   /* decode generalId */

   rtInvokeStartElement (pctxt, "generalId", -1);

   stat = asn1PD_H235Identifier (pctxt, &pvalue->generalId);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "generalId", -1);

   /* decode mrandom */

   rtInvokeStartElement (pctxt, "mrandom", -1);

   stat = asn1PD_H235RandomVal (pctxt, &pvalue->mrandom);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "mrandom", -1);

   /* decode srandom */

   if (pvalue->m.srandomPresent) {
      rtInvokeStartElement (pctxt, "srandom", -1);

      stat = asn1PD_H235RandomVal (pctxt, &pvalue->srandom);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "srandom", -1);
   }

   /* decode timeStamp */

   if (pvalue->m.timeStampPresent) {
      rtInvokeStartElement (pctxt, "timeStamp", -1);

      stat = asn1PD_H235TimeStamp (pctxt, &pvalue->timeStamp);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "timeStamp", -1);
   }

   /* decode encrptval */

   rtInvokeStartElement (pctxt, "encrptval", -1);

   stat = asn1PD_H235ENCRYPTED (pctxt, &pvalue->encrptval);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "encrptval", -1);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  H235CertificateSignature_signature                        */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H235H235CertificateSignature_signature (OOCTXT* pctxt, H235H235CertificateSignature_signature* pvalue)
{
   int stat = ASN_OK;

   /* decode toBeSigned */

   rtInvokeStartElement (pctxt, "toBeSigned", -1);

   stat = asn1PD_H235EncodedReturnSig (pctxt, &pvalue->toBeSigned);
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
/*  H235CertificateSignature                                  */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H235H235CertificateSignature (OOCTXT* pctxt, H235H235CertificateSignature* pvalue)
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
   else pvalue->m.requesterRandomPresent = optbit;

   /* decode certificate */

   rtInvokeStartElement (pctxt, "certificate", -1);

   stat = asn1PD_H235TypedCertificate (pctxt, &pvalue->certificate);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "certificate", -1);

   /* decode responseRandom */

   rtInvokeStartElement (pctxt, "responseRandom", -1);

   stat = asn1PD_H235RandomVal (pctxt, &pvalue->responseRandom);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "responseRandom", -1);

   /* decode requesterRandom */

   if (pvalue->m.requesterRandomPresent) {
      rtInvokeStartElement (pctxt, "requesterRandom", -1);

      stat = asn1PD_H235RandomVal (pctxt, &pvalue->requesterRandom);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "requesterRandom", -1);
   }

   /* decode signature */

   rtInvokeStartElement (pctxt, "signature", -1);

   stat = asn1PD_H235H235CertificateSignature_signature (pctxt, &pvalue->signature);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "signature", -1);

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
/*  ReturnSig                                                 */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H235ReturnSig (OOCTXT* pctxt, H235ReturnSig* pvalue)
{
   int stat = ASN_OK;
   ASN1BOOL optbit;

   /* optional bits */

   memset (&pvalue->m, 0, sizeof(pvalue->m));

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.requestRandomPresent = optbit;

   stat = DECODEBIT (pctxt, &optbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   else pvalue->m.certificatePresent = optbit;

   /* decode generalId */

   rtInvokeStartElement (pctxt, "generalId", -1);

   stat = asn1PD_H235Identifier (pctxt, &pvalue->generalId);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "generalId", -1);

   /* decode responseRandom */

   rtInvokeStartElement (pctxt, "responseRandom", -1);

   stat = asn1PD_H235RandomVal (pctxt, &pvalue->responseRandom);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "responseRandom", -1);

   /* decode requestRandom */

   if (pvalue->m.requestRandomPresent) {
      rtInvokeStartElement (pctxt, "requestRandom", -1);

      stat = asn1PD_H235RandomVal (pctxt, &pvalue->requestRandom);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "requestRandom", -1);
   }

   /* decode certificate */

   if (pvalue->m.certificatePresent) {
      rtInvokeStartElement (pctxt, "certificate", -1);

      stat = asn1PD_H235TypedCertificate (pctxt, &pvalue->certificate);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      rtInvokeEndElement (pctxt, "certificate", -1);
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  KeySyncMaterial                                           */
/*                                                            */
/**************************************************************/

EXTERN int asn1PD_H235KeySyncMaterial (OOCTXT* pctxt, H235KeySyncMaterial* pvalue)
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

   /* decode generalID */

   rtInvokeStartElement (pctxt, "generalID", -1);

   stat = asn1PD_H235Identifier (pctxt, &pvalue->generalID);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "generalID", -1);

   /* decode keyMaterial */

   rtInvokeStartElement (pctxt, "keyMaterial", -1);

   stat = asn1PD_H235KeyMaterial (pctxt, &pvalue->keyMaterial);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   rtInvokeEndElement (pctxt, "keyMaterial", -1);

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

