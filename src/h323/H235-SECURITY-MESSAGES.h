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
#ifndef H235_SECURITY_MESSAGES_H
#define H235_SECURITY_MESSAGES_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include "ooasn1.h"

/**************************************************************/
/*                                                            */
/*  ChallengeString                                           */
/*                                                            */
/**************************************************************/

typedef struct EXTERN H235ChallengeString {
   ASN1UINT numocts;
   ASN1OCTET data[128];
} H235ChallengeString;

EXTERN int asn1PE_H235ChallengeString (OOCTXT* pctxt, H235ChallengeString* pvalue);

EXTERN int asn1PD_H235ChallengeString (OOCTXT* pctxt, H235ChallengeString* pvalue);

/**************************************************************/
/*                                                            */
/*  TimeStamp                                                 */
/*                                                            */
/**************************************************************/

typedef ASN1UINT H235TimeStamp;

EXTERN int asn1PE_H235TimeStamp (OOCTXT* pctxt, H235TimeStamp value);

EXTERN int asn1PD_H235TimeStamp (OOCTXT* pctxt, H235TimeStamp* pvalue);

/**************************************************************/
/*                                                            */
/*  RandomVal                                                 */
/*                                                            */
/**************************************************************/

typedef ASN1INT H235RandomVal;

EXTERN int asn1PE_H235RandomVal (OOCTXT* pctxt, H235RandomVal value);

EXTERN int asn1PD_H235RandomVal (OOCTXT* pctxt, H235RandomVal* pvalue);

/**************************************************************/
/*                                                            */
/*  Password                                                  */
/*                                                            */
/**************************************************************/

typedef ASN1BMPString H235Password;

EXTERN int asn1PE_H235Password (OOCTXT* pctxt, H235Password value);

EXTERN int asn1PD_H235Password (OOCTXT* pctxt, H235Password* pvalue);

/**************************************************************/
/*                                                            */
/*  Identifier                                                */
/*                                                            */
/**************************************************************/

typedef ASN1BMPString H235Identifier;

EXTERN int asn1PE_H235Identifier (OOCTXT* pctxt, H235Identifier value);

EXTERN int asn1PD_H235Identifier (OOCTXT* pctxt, H235Identifier* pvalue);

/**************************************************************/
/*                                                            */
/*  KeyMaterial                                               */
/*                                                            */
/**************************************************************/

typedef struct EXTERN H235KeyMaterial {
   ASN1UINT  numbits;
   ASN1OCTET data[256];
} H235KeyMaterial;

EXTERN int asn1PE_H235KeyMaterial (OOCTXT* pctxt, H235KeyMaterial* pvalue);

EXTERN int asn1PD_H235KeyMaterial (OOCTXT* pctxt, H235KeyMaterial* pvalue);

/**************************************************************/
/*                                                            */
/*  IV8                                                       */
/*                                                            */
/**************************************************************/

typedef struct EXTERN H235IV8 {
   ASN1UINT numocts;
   ASN1OCTET data[8];
} H235IV8;

EXTERN int asn1PE_H235IV8 (OOCTXT* pctxt, H235IV8* pvalue);

EXTERN int asn1PD_H235IV8 (OOCTXT* pctxt, H235IV8* pvalue);

/**************************************************************/
/*                                                            */
/*  IV16                                                      */
/*                                                            */
/**************************************************************/

typedef struct EXTERN H235IV16 {
   ASN1UINT numocts;
   ASN1OCTET data[16];
} H235IV16;

EXTERN int asn1PE_H235IV16 (OOCTXT* pctxt, H235IV16* pvalue);

EXTERN int asn1PD_H235IV16 (OOCTXT* pctxt, H235IV16* pvalue);

/**************************************************************/
/*                                                            */
/*  EncodedGeneralToken                                       */
/*                                                            */
/**************************************************************/

typedef ASN1OpenType H235EncodedGeneralToken;

EXTERN int asn1PE_H235EncodedGeneralToken (OOCTXT* pctxt, H235EncodedGeneralToken value);

EXTERN int asn1PD_H235EncodedGeneralToken (OOCTXT* pctxt, H235EncodedGeneralToken* pvalue);

/**************************************************************/
/*                                                            */
/*  DHset_halfkey                                             */
/*                                                            */
/**************************************************************/

typedef struct EXTERN H235DHset_halfkey {
   ASN1UINT  numbits;
   ASN1OCTET data[256];
} H235DHset_halfkey;

EXTERN int asn1PE_H235DHset_halfkey (OOCTXT* pctxt, H235DHset_halfkey* pvalue);

EXTERN int asn1PD_H235DHset_halfkey (OOCTXT* pctxt, H235DHset_halfkey* pvalue);

/**************************************************************/
/*                                                            */
/*  DHset_modSize                                             */
/*                                                            */
/**************************************************************/

typedef struct EXTERN H235DHset_modSize {
   ASN1UINT  numbits;
   ASN1OCTET data[256];
} H235DHset_modSize;

EXTERN int asn1PE_H235DHset_modSize (OOCTXT* pctxt, H235DHset_modSize* pvalue);

EXTERN int asn1PD_H235DHset_modSize (OOCTXT* pctxt, H235DHset_modSize* pvalue);

/**************************************************************/
/*                                                            */
/*  DHset_generator                                           */
/*                                                            */
/**************************************************************/

typedef struct EXTERN H235DHset_generator {
   ASN1UINT  numbits;
   ASN1OCTET data[256];
} H235DHset_generator;

EXTERN int asn1PE_H235DHset_generator (OOCTXT* pctxt, H235DHset_generator* pvalue);

EXTERN int asn1PD_H235DHset_generator (OOCTXT* pctxt, H235DHset_generator* pvalue);

/**************************************************************/
/*                                                            */
/*  DHset                                                     */
/*                                                            */
/**************************************************************/

typedef struct EXTERN H235DHset {
   H235DHset_halfkey halfkey;
   H235DHset_modSize modSize;
   H235DHset_generator generator;
   DList extElem1;
} H235DHset;

EXTERN int asn1PE_H235DHset (OOCTXT* pctxt, H235DHset* pvalue);

EXTERN int asn1PD_H235DHset (OOCTXT* pctxt, H235DHset* pvalue);

/**************************************************************/
/*                                                            */
/*  TypedCertificate                                          */
/*                                                            */
/**************************************************************/

typedef struct EXTERN H235TypedCertificate {
   ASN1OBJID type;
   ASN1DynOctStr certificate;
   DList extElem1;
} H235TypedCertificate;

EXTERN int asn1PE_H235TypedCertificate (OOCTXT* pctxt, H235TypedCertificate* pvalue);

EXTERN int asn1PD_H235TypedCertificate (OOCTXT* pctxt, H235TypedCertificate* pvalue);

/**************************************************************/
/*                                                            */
/*  NonStandardParameter                                      */
/*                                                            */
/**************************************************************/

typedef struct EXTERN H235NonStandardParameter {
   ASN1OBJID nonStandardIdentifier;
   ASN1DynOctStr data;
} H235NonStandardParameter;

EXTERN int asn1PE_H235NonStandardParameter (OOCTXT* pctxt, H235NonStandardParameter* pvalue);

EXTERN int asn1PD_H235NonStandardParameter (OOCTXT* pctxt, H235NonStandardParameter* pvalue);

/**************************************************************/
/*                                                            */
/*  ECpoint_x                                                 */
/*                                                            */
/**************************************************************/

typedef struct EXTERN H235ECpoint_x {
   ASN1UINT  numbits;
   ASN1OCTET data[64];
} H235ECpoint_x;

EXTERN int asn1PE_H235ECpoint_x (OOCTXT* pctxt, H235ECpoint_x* pvalue);

EXTERN int asn1PD_H235ECpoint_x (OOCTXT* pctxt, H235ECpoint_x* pvalue);

/**************************************************************/
/*                                                            */
/*  ECpoint_y                                                 */
/*                                                            */
/**************************************************************/

typedef struct EXTERN H235ECpoint_y {
   ASN1UINT  numbits;
   ASN1OCTET data[64];
} H235ECpoint_y;

EXTERN int asn1PE_H235ECpoint_y (OOCTXT* pctxt, H235ECpoint_y* pvalue);

EXTERN int asn1PD_H235ECpoint_y (OOCTXT* pctxt, H235ECpoint_y* pvalue);

/**************************************************************/
/*                                                            */
/*  ECpoint                                                   */
/*                                                            */
/**************************************************************/

typedef struct EXTERN H235ECpoint {
   struct {
      unsigned xPresent : 1;
      unsigned yPresent : 1;
   } m;
   H235ECpoint_x x;
   H235ECpoint_y y;
   DList extElem1;
} H235ECpoint;

EXTERN int asn1PE_H235ECpoint (OOCTXT* pctxt, H235ECpoint* pvalue);

EXTERN int asn1PD_H235ECpoint (OOCTXT* pctxt, H235ECpoint* pvalue);

/**************************************************************/
/*                                                            */
/*  ECKASDH_eckasdhp_modulus                                  */
/*                                                            */
/**************************************************************/

typedef struct EXTERN H235ECKASDH_eckasdhp_modulus {
   ASN1UINT  numbits;
   ASN1OCTET data[64];
} H235ECKASDH_eckasdhp_modulus;

EXTERN int asn1PE_H235ECKASDH_eckasdhp_modulus (OOCTXT* pctxt, H235ECKASDH_eckasdhp_modulus* pvalue);

EXTERN int asn1PD_H235ECKASDH_eckasdhp_modulus (OOCTXT* pctxt, H235ECKASDH_eckasdhp_modulus* pvalue);

/**************************************************************/
/*                                                            */
/*  ECKASDH_eckasdhp_weierstrassA                             */
/*                                                            */
/**************************************************************/

typedef struct EXTERN H235ECKASDH_eckasdhp_weierstrassA {
   ASN1UINT  numbits;
   ASN1OCTET data[64];
} H235ECKASDH_eckasdhp_weierstrassA;

EXTERN int asn1PE_H235ECKASDH_eckasdhp_weierstrassA (OOCTXT* pctxt, H235ECKASDH_eckasdhp_weierstrassA* pvalue);

EXTERN int asn1PD_H235ECKASDH_eckasdhp_weierstrassA (OOCTXT* pctxt, H235ECKASDH_eckasdhp_weierstrassA* pvalue);

/**************************************************************/
/*                                                            */
/*  ECKASDH_eckasdhp_weierstrassB                             */
/*                                                            */
/**************************************************************/

typedef struct EXTERN H235ECKASDH_eckasdhp_weierstrassB {
   ASN1UINT  numbits;
   ASN1OCTET data[64];
} H235ECKASDH_eckasdhp_weierstrassB;

EXTERN int asn1PE_H235ECKASDH_eckasdhp_weierstrassB (OOCTXT* pctxt, H235ECKASDH_eckasdhp_weierstrassB* pvalue);

EXTERN int asn1PD_H235ECKASDH_eckasdhp_weierstrassB (OOCTXT* pctxt, H235ECKASDH_eckasdhp_weierstrassB* pvalue);

/**************************************************************/
/*                                                            */
/*  ECKASDH_eckasdhp                                          */
/*                                                            */
/**************************************************************/

typedef struct EXTERN H235ECKASDH_eckasdhp {
   H235ECpoint public_key;
   H235ECKASDH_eckasdhp_modulus modulus;
   H235ECpoint base;
   H235ECKASDH_eckasdhp_weierstrassA weierstrassA;
   H235ECKASDH_eckasdhp_weierstrassB weierstrassB;
} H235ECKASDH_eckasdhp;

EXTERN int asn1PE_H235ECKASDH_eckasdhp (OOCTXT* pctxt, H235ECKASDH_eckasdhp* pvalue);

EXTERN int asn1PD_H235ECKASDH_eckasdhp (OOCTXT* pctxt, H235ECKASDH_eckasdhp* pvalue);

/**************************************************************/
/*                                                            */
/*  ECKASDH_eckasdh2_fieldSize                                */
/*                                                            */
/**************************************************************/

typedef struct EXTERN H235ECKASDH_eckasdh2_fieldSize {
   ASN1UINT  numbits;
   ASN1OCTET data[64];
} H235ECKASDH_eckasdh2_fieldSize;

EXTERN int asn1PE_H235ECKASDH_eckasdh2_fieldSize (OOCTXT* pctxt, H235ECKASDH_eckasdh2_fieldSize* pvalue);

EXTERN int asn1PD_H235ECKASDH_eckasdh2_fieldSize (OOCTXT* pctxt, H235ECKASDH_eckasdh2_fieldSize* pvalue);

/**************************************************************/
/*                                                            */
/*  ECKASDH_eckasdh2_weierstrassA                             */
/*                                                            */
/**************************************************************/

typedef struct EXTERN H235ECKASDH_eckasdh2_weierstrassA {
   ASN1UINT  numbits;
   ASN1OCTET data[64];
} H235ECKASDH_eckasdh2_weierstrassA;

EXTERN int asn1PE_H235ECKASDH_eckasdh2_weierstrassA (OOCTXT* pctxt, H235ECKASDH_eckasdh2_weierstrassA* pvalue);

EXTERN int asn1PD_H235ECKASDH_eckasdh2_weierstrassA (OOCTXT* pctxt, H235ECKASDH_eckasdh2_weierstrassA* pvalue);

/**************************************************************/
/*                                                            */
/*  ECKASDH_eckasdh2_weierstrassB                             */
/*                                                            */
/**************************************************************/

typedef struct EXTERN H235ECKASDH_eckasdh2_weierstrassB {
   ASN1UINT  numbits;
   ASN1OCTET data[64];
} H235ECKASDH_eckasdh2_weierstrassB;

EXTERN int asn1PE_H235ECKASDH_eckasdh2_weierstrassB (OOCTXT* pctxt, H235ECKASDH_eckasdh2_weierstrassB* pvalue);

EXTERN int asn1PD_H235ECKASDH_eckasdh2_weierstrassB (OOCTXT* pctxt, H235ECKASDH_eckasdh2_weierstrassB* pvalue);

/**************************************************************/
/*                                                            */
/*  ECKASDH_eckasdh2                                          */
/*                                                            */
/**************************************************************/

typedef struct EXTERN H235ECKASDH_eckasdh2 {
   H235ECpoint public_key;
   H235ECKASDH_eckasdh2_fieldSize fieldSize;
   H235ECpoint base;
   H235ECKASDH_eckasdh2_weierstrassA weierstrassA;
   H235ECKASDH_eckasdh2_weierstrassB weierstrassB;
} H235ECKASDH_eckasdh2;

EXTERN int asn1PE_H235ECKASDH_eckasdh2 (OOCTXT* pctxt, H235ECKASDH_eckasdh2* pvalue);

EXTERN int asn1PD_H235ECKASDH_eckasdh2 (OOCTXT* pctxt, H235ECKASDH_eckasdh2* pvalue);

/**************************************************************/
/*                                                            */
/*  ECKASDH                                                   */
/*                                                            */
/**************************************************************/

/* Choice tag constants */

#define T_H235ECKASDH_eckasdhp          1
#define T_H235ECKASDH_eckasdh2          2
#define T_H235ECKASDH_extElem1          3

typedef struct EXTERN H235ECKASDH {
   int t;
   union {
      /* t = 1 */
      H235ECKASDH_eckasdhp *eckasdhp;
      /* t = 2 */
      H235ECKASDH_eckasdh2 *eckasdh2;
      /* t = 3 */
      ASN1OpenType *extElem1;
   } u;
} H235ECKASDH;

EXTERN int asn1PE_H235ECKASDH (OOCTXT* pctxt, H235ECKASDH* pvalue);

EXTERN int asn1PD_H235ECKASDH (OOCTXT* pctxt, H235ECKASDH* pvalue);

/**************************************************************/
/*                                                            */
/*  ClearToken                                                */
/*                                                            */
/**************************************************************/

typedef struct EXTERN H235ClearToken {
   struct {
      unsigned timeStampPresent : 1;
      unsigned passwordPresent : 1;
      unsigned dhkeyPresent : 1;
      unsigned challengePresent : 1;
      unsigned randomPresent : 1;
      unsigned certificatePresent : 1;
      unsigned generalIDPresent : 1;
      unsigned nonStandardPresent : 1;
      unsigned eckasdhkeyPresent : 1;
      unsigned sendersIDPresent : 1;
   } m;
   ASN1OBJID tokenOID;
   H235TimeStamp timeStamp;
   H235Password password;
   H235DHset dhkey;
   H235ChallengeString challenge;
   H235RandomVal random;
   H235TypedCertificate certificate;
   H235Identifier generalID;
   H235NonStandardParameter nonStandard;
   H235ECKASDH eckasdhkey;
   H235Identifier sendersID;
   DList extElem1;
} H235ClearToken;

EXTERN int asn1PE_H235ClearToken (OOCTXT* pctxt, H235ClearToken* pvalue);

EXTERN int asn1PD_H235ClearToken (OOCTXT* pctxt, H235ClearToken* pvalue);

/**************************************************************/
/*                                                            */
/*  PwdCertToken                                              */
/*                                                            */
/**************************************************************/

typedef H235ClearToken H235PwdCertToken;

EXTERN int asn1PE_H235PwdCertToken (OOCTXT* pctxt, H235PwdCertToken* pvalue);

EXTERN int asn1PD_H235PwdCertToken (OOCTXT* pctxt, H235PwdCertToken* pvalue);

/**************************************************************/
/*                                                            */
/*  EncodedPwdCertToken                                       */
/*                                                            */
/**************************************************************/

typedef ASN1OpenType H235EncodedPwdCertToken;

EXTERN int asn1PE_H235EncodedPwdCertToken (OOCTXT* pctxt, H235EncodedPwdCertToken value);

EXTERN int asn1PD_H235EncodedPwdCertToken (OOCTXT* pctxt, H235EncodedPwdCertToken* pvalue);

/**************************************************************/
/*                                                            */
/*  EncodedKeySignedMaterial                                  */
/*                                                            */
/**************************************************************/

typedef ASN1OpenType H235EncodedKeySignedMaterial;

EXTERN int asn1PE_H235EncodedKeySignedMaterial (OOCTXT* pctxt, H235EncodedKeySignedMaterial value);

EXTERN int asn1PD_H235EncodedKeySignedMaterial (OOCTXT* pctxt, H235EncodedKeySignedMaterial* pvalue);

/**************************************************************/
/*                                                            */
/*  EncodedReturnSig                                          */
/*                                                            */
/**************************************************************/

typedef ASN1OpenType H235EncodedReturnSig;

EXTERN int asn1PE_H235EncodedReturnSig (OOCTXT* pctxt, H235EncodedReturnSig value);

EXTERN int asn1PD_H235EncodedReturnSig (OOCTXT* pctxt, H235EncodedReturnSig* pvalue);

/**************************************************************/
/*                                                            */
/*  EncodedKeySyncMaterial                                    */
/*                                                            */
/**************************************************************/

typedef ASN1OpenType H235EncodedKeySyncMaterial;

EXTERN int asn1PE_H235EncodedKeySyncMaterial (OOCTXT* pctxt, H235EncodedKeySyncMaterial value);

EXTERN int asn1PD_H235EncodedKeySyncMaterial (OOCTXT* pctxt, H235EncodedKeySyncMaterial* pvalue);

/**************************************************************/
/*                                                            */
/*  ECGDSASignature_r                                         */
/*                                                            */
/**************************************************************/

typedef struct EXTERN H235ECGDSASignature_r {
   ASN1UINT  numbits;
   ASN1OCTET data[64];
} H235ECGDSASignature_r;

EXTERN int asn1PE_H235ECGDSASignature_r (OOCTXT* pctxt, H235ECGDSASignature_r* pvalue);

EXTERN int asn1PD_H235ECGDSASignature_r (OOCTXT* pctxt, H235ECGDSASignature_r* pvalue);

/**************************************************************/
/*                                                            */
/*  ECGDSASignature_s                                         */
/*                                                            */
/**************************************************************/

typedef struct EXTERN H235ECGDSASignature_s {
   ASN1UINT  numbits;
   ASN1OCTET data[64];
} H235ECGDSASignature_s;

EXTERN int asn1PE_H235ECGDSASignature_s (OOCTXT* pctxt, H235ECGDSASignature_s* pvalue);

EXTERN int asn1PD_H235ECGDSASignature_s (OOCTXT* pctxt, H235ECGDSASignature_s* pvalue);

/**************************************************************/
/*                                                            */
/*  ECGDSASignature                                           */
/*                                                            */
/**************************************************************/

typedef struct EXTERN H235ECGDSASignature {
   H235ECGDSASignature_r r;
   H235ECGDSASignature_s s;
} H235ECGDSASignature;

EXTERN int asn1PE_H235ECGDSASignature (OOCTXT* pctxt, H235ECGDSASignature* pvalue);

EXTERN int asn1PD_H235ECGDSASignature (OOCTXT* pctxt, H235ECGDSASignature* pvalue);

/**************************************************************/
/*                                                            */
/*  AuthenticationBES                                         */
/*                                                            */
/**************************************************************/

/* Choice tag constants */

#define T_H235AuthenticationBES_default_ 1
#define T_H235AuthenticationBES_radius  2
#define T_H235AuthenticationBES_extElem1 3

typedef struct EXTERN H235AuthenticationBES {
   int t;
   union {
      /* t = 1 */
      /* t = 2 */
      /* t = 3 */
      ASN1OpenType *extElem1;
   } u;
} H235AuthenticationBES;

EXTERN int asn1PE_H235AuthenticationBES (OOCTXT* pctxt, H235AuthenticationBES* pvalue);

EXTERN int asn1PD_H235AuthenticationBES (OOCTXT* pctxt, H235AuthenticationBES* pvalue);

/**************************************************************/
/*                                                            */
/*  AuthenticationMechanism                                   */
/*                                                            */
/**************************************************************/

/* Choice tag constants */

#define T_H235AuthenticationMechanism_dhExch 1
#define T_H235AuthenticationMechanism_pwdSymEnc 2
#define T_H235AuthenticationMechanism_pwdHash 3
#define T_H235AuthenticationMechanism_certSign 4
#define T_H235AuthenticationMechanism_ipsec 5
#define T_H235AuthenticationMechanism_tls 6
#define T_H235AuthenticationMechanism_nonStandard 7
#define T_H235AuthenticationMechanism_authenticationBES 8
#define T_H235AuthenticationMechanism_extElem1 9

typedef struct EXTERN H235AuthenticationMechanism {
   int t;
   union {
      /* t = 1 */
      /* t = 2 */
      /* t = 3 */
      /* t = 4 */
      /* t = 5 */
      /* t = 6 */
      /* t = 7 */
      H235NonStandardParameter *nonStandard;
      /* t = 8 */
      H235AuthenticationBES *authenticationBES;
      /* t = 9 */
      ASN1OpenType *extElem1;
   } u;
} H235AuthenticationMechanism;

EXTERN int asn1PE_H235AuthenticationMechanism (OOCTXT* pctxt, H235AuthenticationMechanism* pvalue);

EXTERN int asn1PD_H235AuthenticationMechanism (OOCTXT* pctxt, H235AuthenticationMechanism* pvalue);

/**************************************************************/
/*                                                            */
/*  Params                                                    */
/*                                                            */
/**************************************************************/

typedef struct EXTERN H235Params {
   struct {
      unsigned ranIntPresent : 1;
      unsigned iv8Present : 1;
      unsigned iv16Present : 1;
   } m;
   ASN1INT ranInt;
   H235IV8 iv8;
   H235IV16 iv16;
   DList extElem1;
} H235Params;

EXTERN int asn1PE_H235Params (OOCTXT* pctxt, H235Params* pvalue);

EXTERN int asn1PD_H235Params (OOCTXT* pctxt, H235Params* pvalue);

/**************************************************************/
/*                                                            */
/*  ENCRYPTED                                                 */
/*                                                            */
/**************************************************************/

typedef struct EXTERN H235ENCRYPTED {
   ASN1OBJID algorithmOID;
   H235Params paramS;
   ASN1DynOctStr encryptedData;
} H235ENCRYPTED;

EXTERN int asn1PE_H235ENCRYPTED (OOCTXT* pctxt, H235ENCRYPTED* pvalue);

EXTERN int asn1PD_H235ENCRYPTED (OOCTXT* pctxt, H235ENCRYPTED* pvalue);

/**************************************************************/
/*                                                            */
/*  CryptoToken_cryptoEncryptedToken                          */
/*                                                            */
/**************************************************************/

typedef struct EXTERN H235CryptoToken_cryptoEncryptedToken {
   ASN1OBJID tokenOID;
   H235ENCRYPTED token;
} H235CryptoToken_cryptoEncryptedToken;

EXTERN int asn1PE_H235CryptoToken_cryptoEncryptedToken (OOCTXT* pctxt, H235CryptoToken_cryptoEncryptedToken* pvalue);

EXTERN int asn1PD_H235CryptoToken_cryptoEncryptedToken (OOCTXT* pctxt, H235CryptoToken_cryptoEncryptedToken* pvalue);

/**************************************************************/
/*                                                            */
/*  CryptoToken_cryptoSignedToken_token                       */
/*                                                            */
/**************************************************************/

typedef struct EXTERN H235CryptoToken_cryptoSignedToken_token {
   H235EncodedGeneralToken toBeSigned;
   ASN1OBJID algorithmOID;
   H235Params paramS;
   ASN1DynBitStr signature;
} H235CryptoToken_cryptoSignedToken_token;

EXTERN int asn1PE_H235CryptoToken_cryptoSignedToken_token (OOCTXT* pctxt, H235CryptoToken_cryptoSignedToken_token* pvalue);

EXTERN int asn1PD_H235CryptoToken_cryptoSignedToken_token (OOCTXT* pctxt, H235CryptoToken_cryptoSignedToken_token* pvalue);

/**************************************************************/
/*                                                            */
/*  CryptoToken_cryptoSignedToken                             */
/*                                                            */
/**************************************************************/

typedef struct EXTERN H235CryptoToken_cryptoSignedToken {
   ASN1OBJID tokenOID;
   H235CryptoToken_cryptoSignedToken_token token;
} H235CryptoToken_cryptoSignedToken;

EXTERN int asn1PE_H235CryptoToken_cryptoSignedToken (OOCTXT* pctxt, H235CryptoToken_cryptoSignedToken* pvalue);

EXTERN int asn1PD_H235CryptoToken_cryptoSignedToken (OOCTXT* pctxt, H235CryptoToken_cryptoSignedToken* pvalue);

/**************************************************************/
/*                                                            */
/*  HASHED                                                    */
/*                                                            */
/**************************************************************/

typedef struct EXTERN H235HASHED {
   ASN1OBJID algorithmOID;
   H235Params paramS;
   ASN1DynBitStr hash;
} H235HASHED;

EXTERN int asn1PE_H235HASHED (OOCTXT* pctxt, H235HASHED* pvalue);

EXTERN int asn1PD_H235HASHED (OOCTXT* pctxt, H235HASHED* pvalue);

/**************************************************************/
/*                                                            */
/*  CryptoToken_cryptoHashedToken                             */
/*                                                            */
/**************************************************************/

typedef struct EXTERN H235CryptoToken_cryptoHashedToken {
   ASN1OBJID tokenOID;
   H235ClearToken hashedVals;
   H235HASHED token;
} H235CryptoToken_cryptoHashedToken;

EXTERN int asn1PE_H235CryptoToken_cryptoHashedToken (OOCTXT* pctxt, H235CryptoToken_cryptoHashedToken* pvalue);

EXTERN int asn1PD_H235CryptoToken_cryptoHashedToken (OOCTXT* pctxt, H235CryptoToken_cryptoHashedToken* pvalue);

/**************************************************************/
/*                                                            */
/*  CryptoToken                                               */
/*                                                            */
/**************************************************************/

/* Choice tag constants */

#define T_H235CryptoToken_cryptoEncryptedToken 1
#define T_H235CryptoToken_cryptoSignedToken 2
#define T_H235CryptoToken_cryptoHashedToken 3
#define T_H235CryptoToken_cryptoPwdEncr 4
#define T_H235CryptoToken_extElem1      5

typedef struct EXTERN H235CryptoToken {
   int t;
   union {
      /* t = 1 */
      H235CryptoToken_cryptoEncryptedToken *cryptoEncryptedToken;
      /* t = 2 */
      H235CryptoToken_cryptoSignedToken *cryptoSignedToken;
      /* t = 3 */
      H235CryptoToken_cryptoHashedToken *cryptoHashedToken;
      /* t = 4 */
      H235ENCRYPTED *cryptoPwdEncr;
      /* t = 5 */
      ASN1OpenType *extElem1;
   } u;
} H235CryptoToken;

EXTERN int asn1PE_H235CryptoToken (OOCTXT* pctxt, H235CryptoToken* pvalue);

EXTERN int asn1PD_H235CryptoToken (OOCTXT* pctxt, H235CryptoToken* pvalue);

/**************************************************************/
/*                                                            */
/*  H235Key_certProtectedKey                                  */
/*                                                            */
/**************************************************************/

typedef struct EXTERN H235H235Key_certProtectedKey {
   H235EncodedKeySignedMaterial toBeSigned;
   ASN1OBJID algorithmOID;
   H235Params paramS;
   ASN1DynBitStr signature;
} H235H235Key_certProtectedKey;

EXTERN int asn1PE_H235H235Key_certProtectedKey (OOCTXT* pctxt, H235H235Key_certProtectedKey* pvalue);

EXTERN int asn1PD_H235H235Key_certProtectedKey (OOCTXT* pctxt, H235H235Key_certProtectedKey* pvalue);

/**************************************************************/
/*                                                            */
/*  H235Key                                                   */
/*                                                            */
/**************************************************************/

/* Choice tag constants */

#define T_H235H235Key_secureChannel     1
#define T_H235H235Key_sharedSecret      2
#define T_H235H235Key_certProtectedKey  3
#define T_H235H235Key_extElem1          4

typedef struct EXTERN H235H235Key {
   int t;
   union {
      /* t = 1 */
      H235KeyMaterial *secureChannel;
      /* t = 2 */
      H235ENCRYPTED *sharedSecret;
      /* t = 3 */
      H235H235Key_certProtectedKey *certProtectedKey;
      /* t = 4 */
      ASN1OpenType *extElem1;
   } u;
} H235H235Key;

EXTERN int asn1PE_H235H235Key (OOCTXT* pctxt, H235H235Key* pvalue);

EXTERN int asn1PD_H235H235Key (OOCTXT* pctxt, H235H235Key* pvalue);

/**************************************************************/
/*                                                            */
/*  KeySignedMaterial                                         */
/*                                                            */
/**************************************************************/

typedef struct EXTERN H235KeySignedMaterial {
   struct {
      unsigned srandomPresent : 1;
      unsigned timeStampPresent : 1;
   } m;
   H235Identifier generalId;
   H235RandomVal mrandom;
   H235RandomVal srandom;
   H235TimeStamp timeStamp;
   H235ENCRYPTED encrptval;
} H235KeySignedMaterial;

EXTERN int asn1PE_H235KeySignedMaterial (OOCTXT* pctxt, H235KeySignedMaterial* pvalue);

EXTERN int asn1PD_H235KeySignedMaterial (OOCTXT* pctxt, H235KeySignedMaterial* pvalue);

/**************************************************************/
/*                                                            */
/*  H235CertificateSignature_signature                        */
/*                                                            */
/**************************************************************/

typedef struct EXTERN H235H235CertificateSignature_signature {
   H235EncodedReturnSig toBeSigned;
   ASN1OBJID algorithmOID;
   H235Params paramS;
   ASN1DynBitStr signature;
} H235H235CertificateSignature_signature;

EXTERN int asn1PE_H235H235CertificateSignature_signature (OOCTXT* pctxt, H235H235CertificateSignature_signature* pvalue);

EXTERN int asn1PD_H235H235CertificateSignature_signature (OOCTXT* pctxt, H235H235CertificateSignature_signature* pvalue);

/**************************************************************/
/*                                                            */
/*  H235CertificateSignature                                  */
/*                                                            */
/**************************************************************/

typedef struct EXTERN H235H235CertificateSignature {
   struct {
      unsigned requesterRandomPresent : 1;
   } m;
   H235TypedCertificate certificate;
   H235RandomVal responseRandom;
   H235RandomVal requesterRandom;
   H235H235CertificateSignature_signature signature;
   DList extElem1;
} H235H235CertificateSignature;

EXTERN int asn1PE_H235H235CertificateSignature (OOCTXT* pctxt, H235H235CertificateSignature* pvalue);

EXTERN int asn1PD_H235H235CertificateSignature (OOCTXT* pctxt, H235H235CertificateSignature* pvalue);

/**************************************************************/
/*                                                            */
/*  ReturnSig                                                 */
/*                                                            */
/**************************************************************/

typedef struct EXTERN H235ReturnSig {
   struct {
      unsigned requestRandomPresent : 1;
      unsigned certificatePresent : 1;
   } m;
   H235Identifier generalId;
   H235RandomVal responseRandom;
   H235RandomVal requestRandom;
   H235TypedCertificate certificate;
} H235ReturnSig;

EXTERN int asn1PE_H235ReturnSig (OOCTXT* pctxt, H235ReturnSig* pvalue);

EXTERN int asn1PD_H235ReturnSig (OOCTXT* pctxt, H235ReturnSig* pvalue);

/**************************************************************/
/*                                                            */
/*  KeySyncMaterial                                           */
/*                                                            */
/**************************************************************/

typedef struct EXTERN H235KeySyncMaterial {
   H235Identifier generalID;
   H235KeyMaterial keyMaterial;
   DList extElem1;
} H235KeySyncMaterial;

EXTERN int asn1PE_H235KeySyncMaterial (OOCTXT* pctxt, H235KeySyncMaterial* pvalue);

EXTERN int asn1PD_H235KeySyncMaterial (OOCTXT* pctxt, H235KeySyncMaterial* pvalue);

#ifdef __cplusplus
}
#endif

#endif
