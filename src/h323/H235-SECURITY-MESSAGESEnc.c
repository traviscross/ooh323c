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
#include "ooasn1.h"
#include "H235-SECURITY-MESSAGES.h"

/**************************************************************/
/*                                                            */
/*  ChallengeString                                           */
/*                                                            */
/**************************************************************/

EXTERN int asn1PE_H235ChallengeString (OOCTXT* pctxt, H235ChallengeString* pvalue)
{
   static Asn1SizeCnst lsize1 = { 0, 8, 128, 0 };
   int stat = ASN_OK;

   stat = addSizeConstraint (pctxt, &lsize1);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   stat = encodeOctetString (pctxt, pvalue->numocts, pvalue->data);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  TimeStamp                                                 */
/*                                                            */
/**************************************************************/

EXTERN int asn1PE_H235TimeStamp (OOCTXT* pctxt, H235TimeStamp value)
{
   int stat = ASN_OK;

   stat = encodeConsUnsigned (pctxt, value, 1U, ASN1UINT_MAX);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  RandomVal                                                 */
/*                                                            */
/**************************************************************/

EXTERN int asn1PE_H235RandomVal (OOCTXT* pctxt, H235RandomVal value)
{
   int stat = ASN_OK;

   stat = encodeUnconsInteger (pctxt, value);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  Password                                                  */
/*                                                            */
/**************************************************************/

EXTERN int asn1PE_H235Password (OOCTXT* pctxt, H235Password value)
{
   static Asn1SizeCnst lsize1 = { 0, 1, 128, 0 };
   int stat = ASN_OK;

   stat = addSizeConstraint (pctxt, &lsize1);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   stat = encodeBMPString (pctxt, value, 0);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  Identifier                                                */
/*                                                            */
/**************************************************************/

EXTERN int asn1PE_H235Identifier (OOCTXT* pctxt, H235Identifier value)
{
   static Asn1SizeCnst lsize1 = { 0, 1, 128, 0 };
   int stat = ASN_OK;

   stat = addSizeConstraint (pctxt, &lsize1);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   stat = encodeBMPString (pctxt, value, 0);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  KeyMaterial                                               */
/*                                                            */
/**************************************************************/

EXTERN int asn1PE_H235KeyMaterial (OOCTXT* pctxt, H235KeyMaterial* pvalue)
{
   static Asn1SizeCnst lsize1 = { 0, 1, 2048, 0 };
   int stat = ASN_OK;

   stat = addSizeConstraint (pctxt, &lsize1);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   stat = encodeBitString (pctxt, pvalue->numbits, pvalue->data);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  IV8                                                       */
/*                                                            */
/**************************************************************/

EXTERN int asn1PE_H235IV8 (OOCTXT* pctxt, H235IV8* pvalue)
{
   static Asn1SizeCnst lsize1 = { 0, 8, 8, 0 };
   int stat = ASN_OK;

   stat = addSizeConstraint (pctxt, &lsize1);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   stat = encodeOctetString (pctxt, pvalue->numocts, pvalue->data);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  IV16                                                      */
/*                                                            */
/**************************************************************/

EXTERN int asn1PE_H235IV16 (OOCTXT* pctxt, H235IV16* pvalue)
{
   static Asn1SizeCnst lsize1 = { 0, 16, 16, 0 };
   int stat = ASN_OK;

   stat = addSizeConstraint (pctxt, &lsize1);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   stat = encodeOctetString (pctxt, pvalue->numocts, pvalue->data);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  EncodedGeneralToken                                       */
/*                                                            */
/**************************************************************/

EXTERN int asn1PE_H235EncodedGeneralToken (OOCTXT* pctxt, H235EncodedGeneralToken value)
{
   int stat = ASN_OK;

   stat = encodeOpenType (pctxt, value.numocts, value.data);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  DHset_halfkey                                             */
/*                                                            */
/**************************************************************/

EXTERN int asn1PE_H235DHset_halfkey (OOCTXT* pctxt, H235DHset_halfkey* pvalue)
{
   static Asn1SizeCnst lsize1 = { 0, 0, 2048, 0 };
   int stat = ASN_OK;

   stat = addSizeConstraint (pctxt, &lsize1);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   stat = encodeBitString (pctxt, pvalue->numbits, pvalue->data);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  DHset_modSize                                             */
/*                                                            */
/**************************************************************/

EXTERN int asn1PE_H235DHset_modSize (OOCTXT* pctxt, H235DHset_modSize* pvalue)
{
   static Asn1SizeCnst lsize1 = { 0, 0, 2048, 0 };
   int stat = ASN_OK;

   stat = addSizeConstraint (pctxt, &lsize1);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   stat = encodeBitString (pctxt, pvalue->numbits, pvalue->data);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  DHset_generator                                           */
/*                                                            */
/**************************************************************/

EXTERN int asn1PE_H235DHset_generator (OOCTXT* pctxt, H235DHset_generator* pvalue)
{
   static Asn1SizeCnst lsize1 = { 0, 0, 2048, 0 };
   int stat = ASN_OK;

   stat = addSizeConstraint (pctxt, &lsize1);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   stat = encodeBitString (pctxt, pvalue->numbits, pvalue->data);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  DHset                                                     */
/*                                                            */
/**************************************************************/

EXTERN int asn1PE_H235DHset (OOCTXT* pctxt, H235DHset* pvalue)
{
   int stat = ASN_OK;
   ASN1BOOL extbit;

   /* extension bit */

   extbit = (ASN1BOOL)(pvalue->extElem1.count > 0);

   stat = encodeBit (pctxt, extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* encode halfkey */

   stat = asn1PE_H235DHset_halfkey (pctxt, &pvalue->halfkey);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* encode modSize */

   stat = asn1PE_H235DHset_modSize (pctxt, &pvalue->modSize);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* encode generator */

   stat = asn1PE_H235DHset_generator (pctxt, &pvalue->generator);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   if (extbit) {

      /* encode extension optional bits length */

      stat = encodeSmallNonNegWholeNumber (pctxt, pvalue->extElem1.count - 1);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      /* encode optional bits */

      stat = encodeOpenTypeExtBits (pctxt, &pvalue->extElem1);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      /* encode extension elements */

      if (pvalue->extElem1.count > 0) {
         stat = encodeOpenTypeExt (pctxt, &pvalue->extElem1);
         if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  TypedCertificate                                          */
/*                                                            */
/**************************************************************/

EXTERN int asn1PE_H235TypedCertificate (OOCTXT* pctxt, H235TypedCertificate* pvalue)
{
   int stat = ASN_OK;
   ASN1BOOL extbit;

   /* extension bit */

   extbit = (ASN1BOOL)(pvalue->extElem1.count > 0);

   stat = encodeBit (pctxt, extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* encode type */

   stat = encodeObjectIdentifier (pctxt, &pvalue->type);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* encode certificate */

   stat = encodeOctetString (pctxt, pvalue->certificate.numocts, pvalue->certificate.data);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   if (extbit) {

      /* encode extension optional bits length */

      stat = encodeSmallNonNegWholeNumber (pctxt, pvalue->extElem1.count - 1);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      /* encode optional bits */

      stat = encodeOpenTypeExtBits (pctxt, &pvalue->extElem1);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      /* encode extension elements */

      if (pvalue->extElem1.count > 0) {
         stat = encodeOpenTypeExt (pctxt, &pvalue->extElem1);
         if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  NonStandardParameter                                      */
/*                                                            */
/**************************************************************/

EXTERN int asn1PE_H235NonStandardParameter (OOCTXT* pctxt, H235NonStandardParameter* pvalue)
{
   int stat = ASN_OK;

   /* encode nonStandardIdentifier */

   stat = encodeObjectIdentifier (pctxt, &pvalue->nonStandardIdentifier);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* encode data */

   stat = encodeOctetString (pctxt, pvalue->data.numocts, pvalue->data.data);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  ECpoint_x                                                 */
/*                                                            */
/**************************************************************/

EXTERN int asn1PE_H235ECpoint_x (OOCTXT* pctxt, H235ECpoint_x* pvalue)
{
   static Asn1SizeCnst lsize1 = { 0, 0, 511, 0 };
   int stat = ASN_OK;

   stat = addSizeConstraint (pctxt, &lsize1);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   stat = encodeBitString (pctxt, pvalue->numbits, pvalue->data);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  ECpoint_y                                                 */
/*                                                            */
/**************************************************************/

EXTERN int asn1PE_H235ECpoint_y (OOCTXT* pctxt, H235ECpoint_y* pvalue)
{
   static Asn1SizeCnst lsize1 = { 0, 0, 511, 0 };
   int stat = ASN_OK;

   stat = addSizeConstraint (pctxt, &lsize1);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   stat = encodeBitString (pctxt, pvalue->numbits, pvalue->data);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  ECpoint                                                   */
/*                                                            */
/**************************************************************/

EXTERN int asn1PE_H235ECpoint (OOCTXT* pctxt, H235ECpoint* pvalue)
{
   int stat = ASN_OK;
   ASN1BOOL extbit;

   /* extension bit */

   extbit = (ASN1BOOL)(pvalue->extElem1.count > 0);

   stat = encodeBit (pctxt, extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   stat = encodeBit (pctxt, (ASN1BOOL)pvalue->m.xPresent);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   stat = encodeBit (pctxt, (ASN1BOOL)pvalue->m.yPresent);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* encode x */

   if (pvalue->m.xPresent) {
      stat = asn1PE_H235ECpoint_x (pctxt, &pvalue->x);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   }

   /* encode y */

   if (pvalue->m.yPresent) {
      stat = asn1PE_H235ECpoint_y (pctxt, &pvalue->y);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   }

   if (extbit) {

      /* encode extension optional bits length */

      stat = encodeSmallNonNegWholeNumber (pctxt, pvalue->extElem1.count - 1);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      /* encode optional bits */

      stat = encodeOpenTypeExtBits (pctxt, &pvalue->extElem1);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      /* encode extension elements */

      if (pvalue->extElem1.count > 0) {
         stat = encodeOpenTypeExt (pctxt, &pvalue->extElem1);
         if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  ECKASDH_eckasdhp_modulus                                  */
/*                                                            */
/**************************************************************/

EXTERN int asn1PE_H235ECKASDH_eckasdhp_modulus (OOCTXT* pctxt, H235ECKASDH_eckasdhp_modulus* pvalue)
{
   static Asn1SizeCnst lsize1 = { 0, 0, 511, 0 };
   int stat = ASN_OK;

   stat = addSizeConstraint (pctxt, &lsize1);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   stat = encodeBitString (pctxt, pvalue->numbits, pvalue->data);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  ECKASDH_eckasdhp_weierstrassA                             */
/*                                                            */
/**************************************************************/

EXTERN int asn1PE_H235ECKASDH_eckasdhp_weierstrassA (OOCTXT* pctxt, H235ECKASDH_eckasdhp_weierstrassA* pvalue)
{
   static Asn1SizeCnst lsize1 = { 0, 0, 511, 0 };
   int stat = ASN_OK;

   stat = addSizeConstraint (pctxt, &lsize1);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   stat = encodeBitString (pctxt, pvalue->numbits, pvalue->data);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  ECKASDH_eckasdhp_weierstrassB                             */
/*                                                            */
/**************************************************************/

EXTERN int asn1PE_H235ECKASDH_eckasdhp_weierstrassB (OOCTXT* pctxt, H235ECKASDH_eckasdhp_weierstrassB* pvalue)
{
   static Asn1SizeCnst lsize1 = { 0, 0, 511, 0 };
   int stat = ASN_OK;

   stat = addSizeConstraint (pctxt, &lsize1);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   stat = encodeBitString (pctxt, pvalue->numbits, pvalue->data);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  ECKASDH_eckasdhp                                          */
/*                                                            */
/**************************************************************/

EXTERN int asn1PE_H235ECKASDH_eckasdhp (OOCTXT* pctxt, H235ECKASDH_eckasdhp* pvalue)
{
   int stat = ASN_OK;

   /* encode public_key */

   stat = asn1PE_H235ECpoint (pctxt, &pvalue->public_key);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* encode modulus */

   stat = asn1PE_H235ECKASDH_eckasdhp_modulus (pctxt, &pvalue->modulus);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* encode base */

   stat = asn1PE_H235ECpoint (pctxt, &pvalue->base);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* encode weierstrassA */

   stat = asn1PE_H235ECKASDH_eckasdhp_weierstrassA (pctxt, &pvalue->weierstrassA);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* encode weierstrassB */

   stat = asn1PE_H235ECKASDH_eckasdhp_weierstrassB (pctxt, &pvalue->weierstrassB);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  ECKASDH_eckasdh2_fieldSize                                */
/*                                                            */
/**************************************************************/

EXTERN int asn1PE_H235ECKASDH_eckasdh2_fieldSize (OOCTXT* pctxt, H235ECKASDH_eckasdh2_fieldSize* pvalue)
{
   static Asn1SizeCnst lsize1 = { 0, 0, 511, 0 };
   int stat = ASN_OK;

   stat = addSizeConstraint (pctxt, &lsize1);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   stat = encodeBitString (pctxt, pvalue->numbits, pvalue->data);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  ECKASDH_eckasdh2_weierstrassA                             */
/*                                                            */
/**************************************************************/

EXTERN int asn1PE_H235ECKASDH_eckasdh2_weierstrassA (OOCTXT* pctxt, H235ECKASDH_eckasdh2_weierstrassA* pvalue)
{
   static Asn1SizeCnst lsize1 = { 0, 0, 511, 0 };
   int stat = ASN_OK;

   stat = addSizeConstraint (pctxt, &lsize1);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   stat = encodeBitString (pctxt, pvalue->numbits, pvalue->data);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  ECKASDH_eckasdh2_weierstrassB                             */
/*                                                            */
/**************************************************************/

EXTERN int asn1PE_H235ECKASDH_eckasdh2_weierstrassB (OOCTXT* pctxt, H235ECKASDH_eckasdh2_weierstrassB* pvalue)
{
   static Asn1SizeCnst lsize1 = { 0, 0, 511, 0 };
   int stat = ASN_OK;

   stat = addSizeConstraint (pctxt, &lsize1);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   stat = encodeBitString (pctxt, pvalue->numbits, pvalue->data);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  ECKASDH_eckasdh2                                          */
/*                                                            */
/**************************************************************/

EXTERN int asn1PE_H235ECKASDH_eckasdh2 (OOCTXT* pctxt, H235ECKASDH_eckasdh2* pvalue)
{
   int stat = ASN_OK;

   /* encode public_key */

   stat = asn1PE_H235ECpoint (pctxt, &pvalue->public_key);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* encode fieldSize */

   stat = asn1PE_H235ECKASDH_eckasdh2_fieldSize (pctxt, &pvalue->fieldSize);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* encode base */

   stat = asn1PE_H235ECpoint (pctxt, &pvalue->base);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* encode weierstrassA */

   stat = asn1PE_H235ECKASDH_eckasdh2_weierstrassA (pctxt, &pvalue->weierstrassA);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* encode weierstrassB */

   stat = asn1PE_H235ECKASDH_eckasdh2_weierstrassB (pctxt, &pvalue->weierstrassB);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  ECKASDH                                                   */
/*                                                            */
/**************************************************************/

EXTERN int asn1PE_H235ECKASDH (OOCTXT* pctxt, H235ECKASDH* pvalue)
{
   int stat = ASN_OK;
   ASN1BOOL extbit;

   /* extension bit */

   extbit = (ASN1BOOL)(pvalue->t > 2);

   stat = encodeBit (pctxt, extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   if (!extbit) {

      /* Encode choice index value */

      stat = encodeConsUnsigned (pctxt, pvalue->t - 1, 0, 1);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      /* Encode root element data value */

      switch (pvalue->t)
      {
         /* eckasdhp */
         case 1:
            stat = asn1PE_H235ECKASDH_eckasdhp (pctxt, pvalue->u.eckasdhp);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
            break;

         /* eckasdh2 */
         case 2:
            stat = asn1PE_H235ECKASDH_eckasdh2 (pctxt, pvalue->u.eckasdh2);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
            break;

         default:
            return LOG_ASN1ERR (pctxt, ASN_E_INVOPT);
      }
   }
   else {
      /* Encode extension choice index value */

      stat = encodeSmallNonNegWholeNumber (pctxt, pvalue->t - 3);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      /* Encode extension element data value */

      stat = encodeOpenType (pctxt, pvalue->u.extElem1->numocts, pvalue->u.extElem1->data);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  ClearToken                                                */
/*                                                            */
/**************************************************************/

EXTERN int asn1PE_H235ClearToken (OOCTXT* pctxt, H235ClearToken* pvalue)
{
   int stat = ASN_OK;
   ASN1BOOL extbit;
   ASN1OpenType openType;
   OOCTXT lctxt;

   /* extension bit */

   extbit = (ASN1BOOL)(pvalue->m.eckasdhkeyPresent ||
   pvalue->m.sendersIDPresent ||
   pvalue->extElem1.count > 0);

   stat = encodeBit (pctxt, extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   stat = encodeBit (pctxt, (ASN1BOOL)pvalue->m.timeStampPresent);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   stat = encodeBit (pctxt, (ASN1BOOL)pvalue->m.passwordPresent);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   stat = encodeBit (pctxt, (ASN1BOOL)pvalue->m.dhkeyPresent);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   stat = encodeBit (pctxt, (ASN1BOOL)pvalue->m.challengePresent);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   stat = encodeBit (pctxt, (ASN1BOOL)pvalue->m.randomPresent);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   stat = encodeBit (pctxt, (ASN1BOOL)pvalue->m.certificatePresent);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   stat = encodeBit (pctxt, (ASN1BOOL)pvalue->m.generalIDPresent);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   stat = encodeBit (pctxt, (ASN1BOOL)pvalue->m.nonStandardPresent);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* encode tokenOID */

   stat = encodeObjectIdentifier (pctxt, &pvalue->tokenOID);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* encode timeStamp */

   if (pvalue->m.timeStampPresent) {
      stat = asn1PE_H235TimeStamp (pctxt, pvalue->timeStamp);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   }

   /* encode password */

   if (pvalue->m.passwordPresent) {
      stat = asn1PE_H235Password (pctxt, pvalue->password);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   }

   /* encode dhkey */

   if (pvalue->m.dhkeyPresent) {
      stat = asn1PE_H235DHset (pctxt, &pvalue->dhkey);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   }

   /* encode challenge */

   if (pvalue->m.challengePresent) {
      stat = asn1PE_H235ChallengeString (pctxt, &pvalue->challenge);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   }

   /* encode random */

   if (pvalue->m.randomPresent) {
      stat = asn1PE_H235RandomVal (pctxt, pvalue->random);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   }

   /* encode certificate */

   if (pvalue->m.certificatePresent) {
      stat = asn1PE_H235TypedCertificate (pctxt, &pvalue->certificate);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   }

   /* encode generalID */

   if (pvalue->m.generalIDPresent) {
      stat = asn1PE_H235Identifier (pctxt, pvalue->generalID);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   }

   /* encode nonStandard */

   if (pvalue->m.nonStandardPresent) {
      stat = asn1PE_H235NonStandardParameter (pctxt, &pvalue->nonStandard);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   }

   if (extbit) {

      /* encode extension optional bits length */

      stat = encodeSmallNonNegWholeNumber (pctxt, pvalue->extElem1.count + 1);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      /* encode optional bits */

      stat = encodeBit (pctxt, (ASN1BOOL)pvalue->m.eckasdhkeyPresent);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = encodeBit (pctxt, (ASN1BOOL)pvalue->m.sendersIDPresent);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = encodeOpenTypeExtBits (pctxt, &pvalue->extElem1);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      /* encode extension elements */

      if (pvalue->m.eckasdhkeyPresent) {
         initContext (&lctxt);
         stat = setPERBuffer (&lctxt, 0, 0, TRUE);
         if (stat != ASN_OK) {
            freeContext (&lctxt);
            return LOG_ASN1ERR (pctxt, stat);
         }

         stat = asn1PE_H235ECKASDH (&lctxt, &pvalue->eckasdhkey);
         if (stat != ASN_OK) {
            freeContext (&lctxt);
            return LOG_ASN1ERR (pctxt, stat);
         }

         stat = encodeByteAlign (&lctxt);
         if (stat != ASN_OK) {
            freeContext (&lctxt);
            return LOG_ASN1ERR (pctxt, stat);
         }

         openType.data = encodeGetMsgPtr (&lctxt, (int*)&openType.numocts);

         stat = encodeOpenType (pctxt, openType.numocts, openType.data);
         if (stat != ASN_OK) {
            freeContext (&lctxt);
            return LOG_ASN1ERR (pctxt, stat);
         }

         freeContext (&lctxt);
      }

      if (pvalue->m.sendersIDPresent) {
         initContext (&lctxt);
         stat = setPERBuffer (&lctxt, 0, 0, TRUE);
         if (stat != ASN_OK) {
            freeContext (&lctxt);
            return LOG_ASN1ERR (pctxt, stat);
         }

         stat = asn1PE_H235Identifier (&lctxt, pvalue->sendersID);
         if (stat != ASN_OK) {
            freeContext (&lctxt);
            return LOG_ASN1ERR (pctxt, stat);
         }

         stat = encodeByteAlign (&lctxt);
         if (stat != ASN_OK) {
            freeContext (&lctxt);
            return LOG_ASN1ERR (pctxt, stat);
         }

         openType.data = encodeGetMsgPtr (&lctxt, (int*)&openType.numocts);

         stat = encodeOpenType (pctxt, openType.numocts, openType.data);
         if (stat != ASN_OK) {
            freeContext (&lctxt);
            return LOG_ASN1ERR (pctxt, stat);
         }

         freeContext (&lctxt);
      }

      if (pvalue->extElem1.count > 0) {
         stat = encodeOpenTypeExt (pctxt, &pvalue->extElem1);
         if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  PwdCertToken                                              */
/*                                                            */
/**************************************************************/

EXTERN int asn1PE_H235PwdCertToken (OOCTXT* pctxt, H235PwdCertToken* pvalue)
{
   int stat = ASN_OK;

   stat = asn1PE_H235ClearToken (pctxt, pvalue);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  EncodedPwdCertToken                                       */
/*                                                            */
/**************************************************************/

EXTERN int asn1PE_H235EncodedPwdCertToken (OOCTXT* pctxt, H235EncodedPwdCertToken value)
{
   int stat = ASN_OK;

   stat = encodeOpenType (pctxt, value.numocts, value.data);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  EncodedKeySignedMaterial                                  */
/*                                                            */
/**************************************************************/

EXTERN int asn1PE_H235EncodedKeySignedMaterial (OOCTXT* pctxt, H235EncodedKeySignedMaterial value)
{
   int stat = ASN_OK;

   stat = encodeOpenType (pctxt, value.numocts, value.data);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  EncodedReturnSig                                          */
/*                                                            */
/**************************************************************/

EXTERN int asn1PE_H235EncodedReturnSig (OOCTXT* pctxt, H235EncodedReturnSig value)
{
   int stat = ASN_OK;

   stat = encodeOpenType (pctxt, value.numocts, value.data);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  EncodedKeySyncMaterial                                    */
/*                                                            */
/**************************************************************/

EXTERN int asn1PE_H235EncodedKeySyncMaterial (OOCTXT* pctxt, H235EncodedKeySyncMaterial value)
{
   int stat = ASN_OK;

   stat = encodeOpenType (pctxt, value.numocts, value.data);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  ECGDSASignature_r                                         */
/*                                                            */
/**************************************************************/

EXTERN int asn1PE_H235ECGDSASignature_r (OOCTXT* pctxt, H235ECGDSASignature_r* pvalue)
{
   static Asn1SizeCnst lsize1 = { 0, 0, 511, 0 };
   int stat = ASN_OK;

   stat = addSizeConstraint (pctxt, &lsize1);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   stat = encodeBitString (pctxt, pvalue->numbits, pvalue->data);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  ECGDSASignature_s                                         */
/*                                                            */
/**************************************************************/

EXTERN int asn1PE_H235ECGDSASignature_s (OOCTXT* pctxt, H235ECGDSASignature_s* pvalue)
{
   static Asn1SizeCnst lsize1 = { 0, 0, 511, 0 };
   int stat = ASN_OK;

   stat = addSizeConstraint (pctxt, &lsize1);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   stat = encodeBitString (pctxt, pvalue->numbits, pvalue->data);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  ECGDSASignature                                           */
/*                                                            */
/**************************************************************/

EXTERN int asn1PE_H235ECGDSASignature (OOCTXT* pctxt, H235ECGDSASignature* pvalue)
{
   int stat = ASN_OK;

   /* encode r */

   stat = asn1PE_H235ECGDSASignature_r (pctxt, &pvalue->r);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* encode s */

   stat = asn1PE_H235ECGDSASignature_s (pctxt, &pvalue->s);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  AuthenticationBES                                         */
/*                                                            */
/**************************************************************/

EXTERN int asn1PE_H235AuthenticationBES (OOCTXT* pctxt, H235AuthenticationBES* pvalue)
{
   int stat = ASN_OK;
   ASN1BOOL extbit;

   /* extension bit */

   extbit = (ASN1BOOL)(pvalue->t > 2);

   stat = encodeBit (pctxt, extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   if (!extbit) {

      /* Encode choice index value */

      stat = encodeConsUnsigned (pctxt, pvalue->t - 1, 0, 1);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      /* Encode root element data value */

      switch (pvalue->t)
      {
         /* default_ */
         case 1:
            /* NULL */
            break;

         /* radius */
         case 2:
            /* NULL */
            break;

         default:
            return LOG_ASN1ERR (pctxt, ASN_E_INVOPT);
      }
   }
   else {
      /* Encode extension choice index value */

      stat = encodeSmallNonNegWholeNumber (pctxt, pvalue->t - 3);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      /* Encode extension element data value */

      stat = encodeOpenType (pctxt, pvalue->u.extElem1->numocts, pvalue->u.extElem1->data);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  AuthenticationMechanism                                   */
/*                                                            */
/**************************************************************/

EXTERN int asn1PE_H235AuthenticationMechanism (OOCTXT* pctxt, H235AuthenticationMechanism* pvalue)
{
   int stat = ASN_OK;
   OOCTXT lctxt;
   ASN1OpenType openType;
   ASN1BOOL extbit;

   /* extension bit */

   extbit = (ASN1BOOL)(pvalue->t > 7);

   stat = encodeBit (pctxt, extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   if (!extbit) {

      /* Encode choice index value */

      stat = encodeConsUnsigned (pctxt, pvalue->t - 1, 0, 6);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      /* Encode root element data value */

      switch (pvalue->t)
      {
         /* dhExch */
         case 1:
            /* NULL */
            break;

         /* pwdSymEnc */
         case 2:
            /* NULL */
            break;

         /* pwdHash */
         case 3:
            /* NULL */
            break;

         /* certSign */
         case 4:
            /* NULL */
            break;

         /* ipsec */
         case 5:
            /* NULL */
            break;

         /* tls */
         case 6:
            /* NULL */
            break;

         /* nonStandard */
         case 7:
            stat = asn1PE_H235NonStandardParameter (pctxt, pvalue->u.nonStandard);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
            break;

         default:
            return LOG_ASN1ERR (pctxt, ASN_E_INVOPT);
      }
   }
   else {
      /* Encode extension choice index value */

      stat = encodeSmallNonNegWholeNumber (pctxt, pvalue->t - 8);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      /* Encode extension element data value */

      initContext (&lctxt);
      stat = setPERBuffer (&lctxt, 0, 0, TRUE);
      if (stat != ASN_OK) {
         freeContext (&lctxt);
         return LOG_ASN1ERR (pctxt, stat);
      }

      switch (pvalue->t)
      {
         /* authenticationBES */
         case 8:
            stat = asn1PE_H235AuthenticationBES (&lctxt, pvalue->u.authenticationBES);
            if (stat != ASN_OK) {
               freeContext (&lctxt);
               return LOG_ASN1ERR (pctxt, stat);
            }
            openType.data = encodeGetMsgPtr (&lctxt, (int*)&openType.numocts);
            break;

         default:
            openType.numocts = pvalue->u.extElem1->numocts;
            openType.data = pvalue->u.extElem1->data;
      }

      stat = encodeByteAlign (pctxt);
      if (stat != ASN_OK) {
         freeContext (&lctxt);
         return LOG_ASN1ERR (pctxt, stat);
      }

      stat = encodeOpenType (pctxt, openType.numocts, openType.data);
      if (stat != ASN_OK) {
         freeContext (&lctxt);
         return LOG_ASN1ERR (pctxt, stat);
      }

      freeContext (&lctxt);
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  Params                                                    */
/*                                                            */
/**************************************************************/

EXTERN int asn1PE_H235Params (OOCTXT* pctxt, H235Params* pvalue)
{
   int stat = ASN_OK;
   ASN1BOOL extbit;
   ASN1OpenType openType;
   OOCTXT lctxt;

   /* extension bit */

   extbit = (ASN1BOOL)(pvalue->m.iv16Present ||
   pvalue->extElem1.count > 0);

   stat = encodeBit (pctxt, extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   stat = encodeBit (pctxt, (ASN1BOOL)pvalue->m.ranIntPresent);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   stat = encodeBit (pctxt, (ASN1BOOL)pvalue->m.iv8Present);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* encode ranInt */

   if (pvalue->m.ranIntPresent) {
      stat = encodeUnconsInteger (pctxt, pvalue->ranInt);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   }

   /* encode iv8 */

   if (pvalue->m.iv8Present) {
      stat = asn1PE_H235IV8 (pctxt, &pvalue->iv8);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   }

   if (extbit) {

      /* encode extension optional bits length */

      stat = encodeSmallNonNegWholeNumber (pctxt, pvalue->extElem1.count);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      /* encode optional bits */

      stat = encodeBit (pctxt, (ASN1BOOL)pvalue->m.iv16Present);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      stat = encodeOpenTypeExtBits (pctxt, &pvalue->extElem1);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      /* encode extension elements */

      if (pvalue->m.iv16Present) {
         initContext (&lctxt);
         stat = setPERBuffer (&lctxt, 0, 0, TRUE);
         if (stat != ASN_OK) {
            freeContext (&lctxt);
            return LOG_ASN1ERR (pctxt, stat);
         }

         stat = asn1PE_H235IV16 (&lctxt, &pvalue->iv16);
         if (stat != ASN_OK) {
            freeContext (&lctxt);
            return LOG_ASN1ERR (pctxt, stat);
         }

         stat = encodeByteAlign (&lctxt);
         if (stat != ASN_OK) {
            freeContext (&lctxt);
            return LOG_ASN1ERR (pctxt, stat);
         }

         openType.data = encodeGetMsgPtr (&lctxt, (int*)&openType.numocts);

         stat = encodeOpenType (pctxt, openType.numocts, openType.data);
         if (stat != ASN_OK) {
            freeContext (&lctxt);
            return LOG_ASN1ERR (pctxt, stat);
         }

         freeContext (&lctxt);
      }

      if (pvalue->extElem1.count > 0) {
         stat = encodeOpenTypeExt (pctxt, &pvalue->extElem1);
         if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  ENCRYPTED                                                 */
/*                                                            */
/**************************************************************/

EXTERN int asn1PE_H235ENCRYPTED (OOCTXT* pctxt, H235ENCRYPTED* pvalue)
{
   int stat = ASN_OK;

   /* encode algorithmOID */

   stat = encodeObjectIdentifier (pctxt, &pvalue->algorithmOID);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* encode paramS */

   stat = asn1PE_H235Params (pctxt, &pvalue->paramS);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* encode encryptedData */

   stat = encodeOctetString (pctxt, pvalue->encryptedData.numocts, pvalue->encryptedData.data);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  CryptoToken_cryptoEncryptedToken                          */
/*                                                            */
/**************************************************************/

EXTERN int asn1PE_H235CryptoToken_cryptoEncryptedToken (OOCTXT* pctxt, H235CryptoToken_cryptoEncryptedToken* pvalue)
{
   int stat = ASN_OK;

   /* encode tokenOID */

   stat = encodeObjectIdentifier (pctxt, &pvalue->tokenOID);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* encode token */

   stat = asn1PE_H235ENCRYPTED (pctxt, &pvalue->token);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  CryptoToken_cryptoSignedToken_token                       */
/*                                                            */
/**************************************************************/

EXTERN int asn1PE_H235CryptoToken_cryptoSignedToken_token (OOCTXT* pctxt, H235CryptoToken_cryptoSignedToken_token* pvalue)
{
   int stat = ASN_OK;

   /* encode toBeSigned */

   stat = asn1PE_H235EncodedGeneralToken (pctxt, pvalue->toBeSigned);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* encode algorithmOID */

   stat = encodeObjectIdentifier (pctxt, &pvalue->algorithmOID);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* encode paramS */

   stat = asn1PE_H235Params (pctxt, &pvalue->paramS);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* encode signature */

   stat = encodeBitString (pctxt, pvalue->signature.numbits, pvalue->signature.data);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  CryptoToken_cryptoSignedToken                             */
/*                                                            */
/**************************************************************/

EXTERN int asn1PE_H235CryptoToken_cryptoSignedToken (OOCTXT* pctxt, H235CryptoToken_cryptoSignedToken* pvalue)
{
   int stat = ASN_OK;

   /* encode tokenOID */

   stat = encodeObjectIdentifier (pctxt, &pvalue->tokenOID);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* encode token */

   stat = asn1PE_H235CryptoToken_cryptoSignedToken_token (pctxt, &pvalue->token);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  HASHED                                                    */
/*                                                            */
/**************************************************************/

EXTERN int asn1PE_H235HASHED (OOCTXT* pctxt, H235HASHED* pvalue)
{
   int stat = ASN_OK;

   /* encode algorithmOID */

   stat = encodeObjectIdentifier (pctxt, &pvalue->algorithmOID);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* encode paramS */

   stat = asn1PE_H235Params (pctxt, &pvalue->paramS);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* encode hash */

   stat = encodeBitString (pctxt, pvalue->hash.numbits, pvalue->hash.data);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  CryptoToken_cryptoHashedToken                             */
/*                                                            */
/**************************************************************/

EXTERN int asn1PE_H235CryptoToken_cryptoHashedToken (OOCTXT* pctxt, H235CryptoToken_cryptoHashedToken* pvalue)
{
   int stat = ASN_OK;

   /* encode tokenOID */

   stat = encodeObjectIdentifier (pctxt, &pvalue->tokenOID);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* encode hashedVals */

   stat = asn1PE_H235ClearToken (pctxt, &pvalue->hashedVals);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* encode token */

   stat = asn1PE_H235HASHED (pctxt, &pvalue->token);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  CryptoToken                                               */
/*                                                            */
/**************************************************************/

EXTERN int asn1PE_H235CryptoToken (OOCTXT* pctxt, H235CryptoToken* pvalue)
{
   int stat = ASN_OK;
   ASN1BOOL extbit;

   /* extension bit */

   extbit = (ASN1BOOL)(pvalue->t > 4);

   stat = encodeBit (pctxt, extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   if (!extbit) {

      /* Encode choice index value */

      stat = encodeConsUnsigned (pctxt, pvalue->t - 1, 0, 3);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      /* Encode root element data value */

      switch (pvalue->t)
      {
         /* cryptoEncryptedToken */
         case 1:
            stat = asn1PE_H235CryptoToken_cryptoEncryptedToken (pctxt, pvalue->u.cryptoEncryptedToken);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
            break;

         /* cryptoSignedToken */
         case 2:
            stat = asn1PE_H235CryptoToken_cryptoSignedToken (pctxt, pvalue->u.cryptoSignedToken);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
            break;

         /* cryptoHashedToken */
         case 3:
            stat = asn1PE_H235CryptoToken_cryptoHashedToken (pctxt, pvalue->u.cryptoHashedToken);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
            break;

         /* cryptoPwdEncr */
         case 4:
            stat = asn1PE_H235ENCRYPTED (pctxt, pvalue->u.cryptoPwdEncr);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
            break;

         default:
            return LOG_ASN1ERR (pctxt, ASN_E_INVOPT);
      }
   }
   else {
      /* Encode extension choice index value */

      stat = encodeSmallNonNegWholeNumber (pctxt, pvalue->t - 5);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      /* Encode extension element data value */

      stat = encodeOpenType (pctxt, pvalue->u.extElem1->numocts, pvalue->u.extElem1->data);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  H235Key_certProtectedKey                                  */
/*                                                            */
/**************************************************************/

EXTERN int asn1PE_H235H235Key_certProtectedKey (OOCTXT* pctxt, H235H235Key_certProtectedKey* pvalue)
{
   int stat = ASN_OK;

   /* encode toBeSigned */

   stat = asn1PE_H235EncodedKeySignedMaterial (pctxt, pvalue->toBeSigned);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* encode algorithmOID */

   stat = encodeObjectIdentifier (pctxt, &pvalue->algorithmOID);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* encode paramS */

   stat = asn1PE_H235Params (pctxt, &pvalue->paramS);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* encode signature */

   stat = encodeBitString (pctxt, pvalue->signature.numbits, pvalue->signature.data);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  H235Key                                                   */
/*                                                            */
/**************************************************************/

EXTERN int asn1PE_H235H235Key (OOCTXT* pctxt, H235H235Key* pvalue)
{
   int stat = ASN_OK;
   ASN1BOOL extbit;

   /* extension bit */

   extbit = (ASN1BOOL)(pvalue->t > 3);

   stat = encodeBit (pctxt, extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   if (!extbit) {

      /* Encode choice index value */

      stat = encodeConsUnsigned (pctxt, pvalue->t - 1, 0, 2);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      /* Encode root element data value */

      switch (pvalue->t)
      {
         /* secureChannel */
         case 1:
            stat = asn1PE_H235KeyMaterial (pctxt, pvalue->u.secureChannel);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
            break;

         /* sharedSecret */
         case 2:
            stat = asn1PE_H235ENCRYPTED (pctxt, pvalue->u.sharedSecret);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
            break;

         /* certProtectedKey */
         case 3:
            stat = asn1PE_H235H235Key_certProtectedKey (pctxt, pvalue->u.certProtectedKey);
            if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
            break;

         default:
            return LOG_ASN1ERR (pctxt, ASN_E_INVOPT);
      }
   }
   else {
      /* Encode extension choice index value */

      stat = encodeSmallNonNegWholeNumber (pctxt, pvalue->t - 4);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      /* Encode extension element data value */

      stat = encodeOpenType (pctxt, pvalue->u.extElem1->numocts, pvalue->u.extElem1->data);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  KeySignedMaterial                                         */
/*                                                            */
/**************************************************************/

EXTERN int asn1PE_H235KeySignedMaterial (OOCTXT* pctxt, H235KeySignedMaterial* pvalue)
{
   int stat = ASN_OK;

   stat = encodeBit (pctxt, (ASN1BOOL)pvalue->m.srandomPresent);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   stat = encodeBit (pctxt, (ASN1BOOL)pvalue->m.timeStampPresent);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* encode generalId */

   stat = asn1PE_H235Identifier (pctxt, pvalue->generalId);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* encode mrandom */

   stat = asn1PE_H235RandomVal (pctxt, pvalue->mrandom);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* encode srandom */

   if (pvalue->m.srandomPresent) {
      stat = asn1PE_H235RandomVal (pctxt, pvalue->srandom);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   }

   /* encode timeStamp */

   if (pvalue->m.timeStampPresent) {
      stat = asn1PE_H235TimeStamp (pctxt, pvalue->timeStamp);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   }

   /* encode encrptval */

   stat = asn1PE_H235ENCRYPTED (pctxt, &pvalue->encrptval);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  H235CertificateSignature_signature                        */
/*                                                            */
/**************************************************************/

EXTERN int asn1PE_H235H235CertificateSignature_signature (OOCTXT* pctxt, H235H235CertificateSignature_signature* pvalue)
{
   int stat = ASN_OK;

   /* encode toBeSigned */

   stat = asn1PE_H235EncodedReturnSig (pctxt, pvalue->toBeSigned);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* encode algorithmOID */

   stat = encodeObjectIdentifier (pctxt, &pvalue->algorithmOID);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* encode paramS */

   stat = asn1PE_H235Params (pctxt, &pvalue->paramS);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* encode signature */

   stat = encodeBitString (pctxt, pvalue->signature.numbits, pvalue->signature.data);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  H235CertificateSignature                                  */
/*                                                            */
/**************************************************************/

EXTERN int asn1PE_H235H235CertificateSignature (OOCTXT* pctxt, H235H235CertificateSignature* pvalue)
{
   int stat = ASN_OK;
   ASN1BOOL extbit;

   /* extension bit */

   extbit = (ASN1BOOL)(pvalue->extElem1.count > 0);

   stat = encodeBit (pctxt, extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   stat = encodeBit (pctxt, (ASN1BOOL)pvalue->m.requesterRandomPresent);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* encode certificate */

   stat = asn1PE_H235TypedCertificate (pctxt, &pvalue->certificate);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* encode responseRandom */

   stat = asn1PE_H235RandomVal (pctxt, pvalue->responseRandom);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* encode requesterRandom */

   if (pvalue->m.requesterRandomPresent) {
      stat = asn1PE_H235RandomVal (pctxt, pvalue->requesterRandom);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   }

   /* encode signature */

   stat = asn1PE_H235H235CertificateSignature_signature (pctxt, &pvalue->signature);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   if (extbit) {

      /* encode extension optional bits length */

      stat = encodeSmallNonNegWholeNumber (pctxt, pvalue->extElem1.count - 1);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      /* encode optional bits */

      stat = encodeOpenTypeExtBits (pctxt, &pvalue->extElem1);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      /* encode extension elements */

      if (pvalue->extElem1.count > 0) {
         stat = encodeOpenTypeExt (pctxt, &pvalue->extElem1);
         if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      }
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  ReturnSig                                                 */
/*                                                            */
/**************************************************************/

EXTERN int asn1PE_H235ReturnSig (OOCTXT* pctxt, H235ReturnSig* pvalue)
{
   int stat = ASN_OK;

   stat = encodeBit (pctxt, (ASN1BOOL)pvalue->m.requestRandomPresent);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   stat = encodeBit (pctxt, (ASN1BOOL)pvalue->m.certificatePresent);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* encode generalId */

   stat = asn1PE_H235Identifier (pctxt, pvalue->generalId);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* encode responseRandom */

   stat = asn1PE_H235RandomVal (pctxt, pvalue->responseRandom);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* encode requestRandom */

   if (pvalue->m.requestRandomPresent) {
      stat = asn1PE_H235RandomVal (pctxt, pvalue->requestRandom);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   }

   /* encode certificate */

   if (pvalue->m.certificatePresent) {
      stat = asn1PE_H235TypedCertificate (pctxt, &pvalue->certificate);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
   }

   return (stat);
}

/**************************************************************/
/*                                                            */
/*  KeySyncMaterial                                           */
/*                                                            */
/**************************************************************/

EXTERN int asn1PE_H235KeySyncMaterial (OOCTXT* pctxt, H235KeySyncMaterial* pvalue)
{
   int stat = ASN_OK;
   ASN1BOOL extbit;

   /* extension bit */

   extbit = (ASN1BOOL)(pvalue->extElem1.count > 0);

   stat = encodeBit (pctxt, extbit);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* encode generalID */

   stat = asn1PE_H235Identifier (pctxt, pvalue->generalID);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   /* encode keyMaterial */

   stat = asn1PE_H235KeyMaterial (pctxt, &pvalue->keyMaterial);
   if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

   if (extbit) {

      /* encode extension optional bits length */

      stat = encodeSmallNonNegWholeNumber (pctxt, pvalue->extElem1.count - 1);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      /* encode optional bits */

      stat = encodeOpenTypeExtBits (pctxt, &pvalue->extElem1);
      if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);

      /* encode extension elements */

      if (pvalue->extElem1.count > 0) {
         stat = encodeOpenTypeExt (pctxt, &pvalue->extElem1);
         if (stat != ASN_OK) return LOG_ASN1ERR (pctxt, stat);
      }
   }

   return (stat);
}

