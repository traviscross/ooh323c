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

#include "ooq931.h"
#include "oo.h"
#include "ooasn1.h"
#include "oochannels.h"
#include "printHandler.h"
#include "ooCalls.h"
#include "ooh245.h"
#include "ooh323ep.h"
#include "ooCapability.h"
#include <time.h>

static ASN1OBJID gProtocolID = {
   6, { 0, 0, 8, 2250, 0, 4 }
};

EXTERN int ooQ931Decode (Q931Message* msg, int length, ASN1OCTET *data)
{
   int offset;
   int rv = ASN_OK;
 
   dListInit (&msg->ies); /* clear information elements list */

   if (length < 5)  /* Packet too short */
      return Q931_E_TOOSHORT;

   msg->protocolDiscriminator = data[0];

   if (data[1] != 2) /* Call reference must be 2 bytes long */
      return Q931_E_INVCALLREF;

   msg->callReference = ((data[2] & 0x7f) << 8) | data[3];
   msg->fromDestination = (data[2] & 0x80) != 0;

   msg->messageType = data[4];

   /* Have preamble, start getting the informationElements into buffers */
   offset = 5;
   while (offset < length) {
      Q931InformationElement *ie;
      int ieOff = offset;
      /* Get field discriminator */
      int discriminator = data[offset++];

      /* For discriminator with high bit set there is no data */
      if ((discriminator & 0x80) == 0) {
         int len = data[offset++], alen;

         if (discriminator == Q931UserUserIE) {
            /* Special case of User-user field, there is some confusion here as
               the Q931 documentation claims the length is a single byte,
               unfortunately all H.323 based apps have a 16 bit length here, so
               we allow for said longer length. There is presumably an addendum
               to Q931 which describes this, and provides a means to
               discriminate between the old 1 byte and the new 2 byte systems.
               However, at present we assume it is always 2 bytes until we find
               something that breaks it.
            */
            len <<= 8;
            len |= data[offset++];

            /* we also have a protocol discriminator, which we ignore */
            offset++;
            len--;
         }

         /* watch out for negative lengths! (ED, 11/5/03) */
         if (len < 0) {
            return Q931_E_INVLENGTH;
         }
         else if (offset + len > length) {
            alen = 0;
            len = -len;
            rv = Q931_E_INVLENGTH;
         }
         else alen = len;

         ie = (Q931InformationElement*)
            ASN1MALLOC (msg->pctxt, sizeof(*ie) - sizeof(ie->data) + alen);
         ie->discriminator = discriminator;
         ie->offset = ieOff;
         ie->length = len;
         if (alen != 0)
            memcpy(ie->data, data + offset, alen);
         offset += len;
      }
      else {
         ie = (Q931InformationElement*) ASN1MALLOC (msg->pctxt,
                                        sizeof(*ie) - sizeof(ie->data));
         ie->discriminator = discriminator;
         ie->offset = offset;
         ie->length = 0;
      }

      dListAppend (msg->pctxt, &msg->ies, ie);
      if (rv != ASN_OK)
         return rv;
   }
  
   rv = ooDecodeUUIE(msg);
   return rv;
}

EXTERN Q931InformationElement* ooQ931GetIE (const Q931Message* q931msg,
                                              int ieCode)
{
   DListNode* curNode;
   unsigned int i;

   for(i = 0, curNode = q931msg->ies.head; i < q931msg->ies.count; i++) {
      Q931InformationElement *ie = (Q931InformationElement*) curNode->data;
      if (ie->discriminator == ieCode) {
         return ie;
      }
      curNode = curNode->next;
   }
   return NULL;
}

char* ooQ931GetMessageTypeName(int messageType, char* buf) {
   switch (messageType) {
      case Q931AlertingMsg :
         strcpy(buf, "Alerting");
         break;
      case Q931CallProceedingMsg :
         strcpy(buf, "CallProceeding");
         break;
      case Q931ConnectMsg :
         strcpy(buf, "Connect");
         break;
      case Q931ConnectAckMsg :
         strcpy(buf, "ConnectAck");
         break;
      case Q931ProgressMsg :
         strcpy(buf, "Progress");
         break;
      case Q931SetupMsg :
         strcpy(buf, "Setup");
         break;
      case Q931SetupAckMsg :
         strcpy(buf, "SetupAck");
         break;
      case Q931FacilityMsg :
         strcpy(buf, "Facility");
         break;
      case Q931ReleaseCompleteMsg :
         strcpy(buf, "ReleaseComplete");
         break;
      case Q931StatusEnquiryMsg :
         strcpy(buf, "StatusEnquiry");
         break;
      case Q931StatusMsg :
         strcpy(buf, "Status");
         break;
      case Q931InformationMsg :
         strcpy(buf, "Information");
         break;
      case Q931NationalEscapeMsg :
         strcpy(buf, "Escape");
         break;
      default:
         sprintf(buf, "<%u>", messageType);
   }
   return buf;
}

char* ooQ931GetIEName(int number, char* buf) {
   switch (number) {
      case Q931BearerCapabilityIE :
         strcpy(buf, "Bearer-Capability");
         break;
      case Q931CauseIE :
         strcpy(buf, "Cause");
         break;
      case Q931FacilityIE :
         strcpy(buf, "Facility");
         break;
      case Q931ProgressIndicatorIE :
         strcpy(buf, "Progress-Indicator");
         break;
      case Q931CallStateIE :
         strcpy(buf, "Call-State");
         break;
      case Q931DisplayIE :
         strcpy(buf, "Display");
         break;
      case Q931SignalIE :
         strcpy(buf, "Signal");
         break;
      case Q931CallingPartyNumberIE :
         strcpy(buf, "Calling-Party-Number");
         break;
      case Q931CalledPartyNumberIE :
         strcpy(buf, "Called-Party-Number");
         break;
      case Q931RedirectingNumberIE :
         strcpy(buf, "Redirecting-Number");
         break;
      case Q931UserUserIE :
         strcpy(buf, "User-User");
         break;
      default:
         sprintf(buf, "0x%02x", number);
   }
   return buf;
}

EXTERN void ooQ931Print (const Q931Message* q931msg) {
   char buf[1000];
   DListNode* curNode;
   unsigned int i;

   printf("Q.931 Message:\n");
   printf("   protocolDiscriminator: %i\n", q931msg->protocolDiscriminator);
   printf("   callReference: %i\n", q931msg->callReference);
   printf("   from: %s\n", (q931msg->fromDestination ?
                                       "destination" : "originator"));
   printf("   messageType: %s (0x%X)\n\n",
              ooQ931GetMessageTypeName(q931msg->messageType, buf),
                                               q931msg->messageType);

   for(i = 0, curNode = q931msg->ies.head; i < q931msg->ies.count; i++) {
      Q931InformationElement *ie = (Q931InformationElement*) curNode->data;
      int length = (ie->length >= 0) ? ie->length : -ie->length;
      printf("   IE[%i] (offset 0x%X):\n", i, ie->offset);
      printf("      discriminator: %s (0x%X)\n",
               ooQ931GetIEName(ie->discriminator, buf), ie->discriminator);
      printf("      data length: %i\n", length);

      curNode = curNode->next;
      printf("\n");
   }
}

int ooCreateQ931Message(Q931Message **q931msg, int msgType)
{
   OOCTXT *pctxt = NULL;
   pctxt = (OOCTXT*)newContext();
   if(!pctxt)
   {
      OOTRACEERR1("Error: Failed to allocate OOCTXT for "
                           "Q931 message creation\n");
      return OO_FAILED;
   }
       
  
   *q931msg = (Q931Message*)ASN1MALLOC(pctxt, sizeof(Q931Message));
               
   if(!*q931msg)
   {
      OOTRACEERR1("Error: Failed to allocate memory for q931 message\n");
      return OO_FAILED;
   }
   else
   {
      (*q931msg)->pctxt = pctxt;
      (*q931msg)->protocolDiscriminator = 8;
     

      (*q931msg)->fromDestination = FALSE;
      (*q931msg)->messageType = msgType;
      return OO_OK;
   }
}

/* CallReference is a two octet field, thus max value can be 0xffff
   or 65535 decimal. We restrict max value to 32760, however, this should
   not cause any problems as there won't be those many simultaneous calls
*/
ASN1USINT ooGenerateCallReference()
{
   static ASN1USINT lastCallRef=0;
  
   if(lastCallRef == 0)
   {
      /* Generate a new random callRef */
      srand((unsigned)time(0));
      lastCallRef = (ASN1USINT)(rand()%100);
   }
   else
      lastCallRef++;

   if(lastCallRef>=32760)
      lastCallRef=0;

   return lastCallRef;
}


int ooGenerateCallIdentifier(H225CallIdentifier *callid)
{
   ASN1INT64 timestamp;
   int i=0;
#ifdef _WIN32
  
   SYSTEMTIME systemTime;
   GetLocalTime(&systemTime);
   SystemTimeToFileTime(&systemTime, (LPFILETIME)&timestamp);
#else
   struct timeval systemTime;
   gettimeofday(&systemTime, NULL);
   timestamp = systemTime.tv_sec * 10000000 + systemTime.tv_usec*10;
#endif

   callid->guid.numocts = 16;
   callid->guid.data[0] = 'o';
   callid->guid.data[1] = 'o';
   callid->guid.data[2] = 'h';
   callid->guid.data[3] = '3';
   callid->guid.data[4] = '2';
   callid->guid.data[5] = '3';
   callid->guid.data[6] = 'c';
   callid->guid.data[7] = '-';

   for (i = 8; i < 16; i++)
       callid->guid.data[i] = (ASN1OCTET)((timestamp>>((i-8+1)*8))&0xff);

   return OO_OK;

}


/* Need to figure out what parameters are to be passed to create
   bearer capability
*/
/*
int ooAddBearerCapabilityIE(struct ooAppContext *context)
{
}
*/


int ooFreeQ931Message(Q931Message *q931Msg)
{
   OOCTXT * pctxt;
   if(!q931Msg)
   {
      pctxt = q931Msg->pctxt;
      freeContext(pctxt);
      ASN1CRTFREE0(pctxt);
   }
   return OO_OK;
}

int ooEncodeUUIE(Q931Message *q931msg)
{
   ASN1OCTET msgbuf[1024];
   ASN1OCTET * msgptr=NULL;
   int  len;
   ASN1BOOL aligned = TRUE, trace = FALSE;
   Q931InformationElement* ie=NULL;
   memset(msgbuf, 0, sizeof(msgbuf));
   if(!q931msg)
   {
      OOTRACEERR1("ERROR: Invalid Q931 message in add user-user IE\n");
      return OO_FAILED;
   }
       
   if(!q931msg->userInfo)
   {
      OOTRACEERR1("ERROR:No User-User IE to encode\n");
      return OO_FAILED;
   }

   setPERBuffer(q931msg->pctxt, msgbuf, sizeof(msgbuf), aligned);
  
   if(asn1PE_H225H323_UserInformation (q931msg->pctxt,
                                           q931msg->userInfo)==ASN_OK)
   {
      OOTRACEDBGC1("UserInfo encoding - successful\n");
   }
   else{
      OOTRACEERR1("Error: UserInfo encoding failed\n");
      return OO_FAILED;
   }
   msgptr = encodeGetMsgPtr(q931msg->pctxt, &len);

   /* Allocate memory to hold complete UserUser Information */
   ie = (Q931InformationElement*)ASN1MALLOC (q931msg->pctxt,
                                     sizeof(*ie) - sizeof(ie->data) + len);
   if(ie == NULL)
   {
      OOTRACEERR1("Error: Allocation of Q931Information "
                  "Element Failed\n");
      return OO_FAILED;
   }
   ie->discriminator = Q931UserUserIE;
   ie->length = len;
   memcpy(ie->data, msgptr, len);
   /* Add the user to user IE NOTE: ALL IEs SHOULD BE IN ASCENDING ORDER OF
      THEIR DISCRIMINATOR AS PER SPEC.
   */
   dListInit (&(q931msg->ies));
   if((dListAppend (q931msg->pctxt,
                      &(q931msg->ies), ie)) == NULL)
   {
      OOTRACEERR1("Error: Failed to add UUIE in outgoing message\n");
      return OO_FAILED;
   }

   return OO_OK;
}

int ooDecodeUUIE(Q931Message *q931Msg)
{
   DListNode* curNode;
   unsigned int i;
   ASN1BOOL aligned=TRUE, trace=FALSE;
   int stat;
   Q931InformationElement *ie;

   if(q931Msg ==NULL)
   {
      OOTRACEERR1("Error: ooDecodeUUIE failed - NULL q931 message\n");
      return OO_FAILED;
   }
       
   /* Search for UserUser IE */
   for(i = 0, curNode = q931Msg->ies.head; i < q931Msg->ies.count;
                                             i++, curNode = curNode->next)
   {
      ie = (Q931InformationElement*) curNode->data;
      if(ie->discriminator == Q931UserUserIE)
         break;
   }
   if(i == q931Msg->ies.count)
   {
      OOTRACEERR1("No UserUser IE found in ooDecodeUUIE\n");
      return OO_FAILED;
   }
       
   /* Decode user-user ie */
   q931Msg->userInfo = (H225H323_UserInformation *) ASN1MALLOC(q931Msg->pctxt,
                                             sizeof(H225H323_UserInformation));
   if(!q931Msg->userInfo)
   {
      OOTRACEERR1("ERROR:Memory allocation failed for user-user ie\n");
      return OO_FAILED;
   }
   memset(q931Msg->userInfo, 0, sizeof(H225H323_UserInformation));

   setPERBuffer (q931Msg->pctxt, ie->data, ie->length, aligned);
  
   stat = asn1PD_H225H323_UserInformation (q931Msg->pctxt, q931Msg->userInfo);
   if(stat != ASN_OK)
   {
      OOTRACEERR1("Error: UserUser IE decode failed\n");
      return OO_FAILED;
   }
  
   return OO_OK;
}

int ooGetOutgoingQ931Msgbuf(ooCallData *call, ASN1OCTET * msgbuf, int * len,
                            int *msgType)
{
   int i=0, ieLen=0, k=0;
   ASN1UINT j=0;
   DListNode* curNode=NULL, *p_msgNode=NULL;
   Q931Message * q931Msg=NULL;
   OOCTXT * pctxt=NULL;
   if(call->outH225Queue.count == 0)
   {
      OOTRACEWARN1("Warning: No outgoing h225 message\n");
      return OO_FAILED;
   }
  
   p_msgNode = call->outH225Queue.head;
  
   q931Msg = (Q931Message*) p_msgNode->data;
   pctxt = q931Msg->pctxt;

   ooEncodeUUIE(q931Msg);
  
   if(q931Msg->messageType == Q931SetupMsg)
      *msgType = OOSetup;
   else if(q931Msg->messageType == Q931ConnectMsg)
      *msgType = OOConnect;
   else if(q931Msg->messageType == Q931CallProceedingMsg)
      *msgType = OOCallProceeding;
   else if(q931Msg->messageType == Q931AlertingMsg)
      *msgType = OOAlert;
   else if(q931Msg->messageType == Q931ReleaseCompleteMsg)
      *msgType = OOReleaseComplete;
   else if(q931Msg->messageType == Q931FacilityMsg)
      *msgType = OOFacility;

  
   msgbuf[i++] = 3; /* TPKT version */
   msgbuf[i++] = 0; /* TPKT resevred */
   /* 1st octet of length, will be populated once len is determined */
   msgbuf[i++] = 0;
   /* 2nd octet of length, will be populated once len is determined */
   msgbuf[i++] = 0;
   /* Q931 protocol discriminator */
   msgbuf[i++] = q931Msg->protocolDiscriminator;
   msgbuf[i++] = 2; /* length of call ref is two octets */
   msgbuf[i++] = (q931Msg->callReference >> 8); /* populate 1st octet */
   msgbuf[i++] = q931Msg->callReference; /* populate 2nd octet */
   msgbuf[i++] = q931Msg->messageType; /* type of q931 message */
  
   /*Add display ie. We use callername as display name as well as alias */
   if(strlen(gH323ep.callername)>0)
   {
      msgbuf[i++] = Q931DisplayIE;
      ieLen = strlen(gH323ep.callername)+1;
      msgbuf[i++] = ieLen;
      memcpy(msgbuf+i, gH323ep.callername, ieLen-1);
      i += ieLen-1;
      msgbuf[i++] = '\0';
   }
  

   for(j = 0, curNode = q931Msg->ies.head; j < q931Msg->ies.count; j++)
   {
      Q931InformationElement *ie = (Q931InformationElement*) curNode->data;
         
      ieLen = ie->length;

      /* Add the ie discriminator in message buffer */
      msgbuf[i++] = ie->discriminator;
         
      /* For user-user IE, we have to add protocol discriminator */
      if (ie->discriminator == Q931UserUserIE)
      {
         ieLen++; /* length includes protocol discriminator octet. */
         msgbuf[i++] = (ieLen>>8); /* 1st octet for length */
         msgbuf[i++] = ieLen;      /* 2nd octet for length */
         ieLen--;
         msgbuf[i++] = 5; /* protocol discriminator */
         memcpy((msgbuf + i), ie->data, ieLen);
         i += ieLen-1;
        
      }
      else if(ie->discriminator == Q931CauseIE)
      {
         msgbuf[i++] = (ieLen>>8);
         msgbuf[i++] = ieLen;
         memcpy((msgbuf+i), ie->data, ieLen);
         i += ieLen -1;
      }else
      {
         OOTRACEWARN1("Warning: Only UUIE is supported currently\n");
         return OO_FAILED;
      }
   }
   *len = i+1; /* complete message length */
   /* Tpkt length octets populated with total length of the message */
   msgbuf[2] = (*len >> 8);
   msgbuf[3] = *len;        /* including tpkt header */

   /* Remove the message from rtdlist outH225Queue */
   dListRemove(&(call->outH225Queue), p_msgNode);

   /* Free memory associated with the message */
   freeContext(pctxt);
   ASN1CRTFREE0(pctxt);

   if(p_msgNode)
      ASN1MEMFREEPTR(call->pctxt, p_msgNode);
   return OO_OK;
}

/*

*/

int ooSendCallProceeding(ooCallData *call)
{
   int ret;   
   H225VendorIdentifier *vendor;
   H225CallProceeding_UUIE *callProceeding;
   Q931Message *q931msg=NULL;
   OOTRACEDBGC3("Building CallProceeding (%s, %s)\n", call->callType,
                 call->callToken);
   ret = ooCreateQ931Message(&q931msg, Q931CallProceedingMsg);
   if(ret != OO_OK)
   {     
      OOTRACEERR1("Error: In allocating memory for - H225 Call "
                           "Proceeding message\n");
      return OO_FAILED;
   }
  
   q931msg->userInfo = (H225H323_UserInformation*)ASN1MALLOC(q931msg->pctxt,
                             sizeof(H225H323_UserInformation));
   if(!q931msg->userInfo)
   {
      OOTRACEERR1("ERROR:Memory allocation for User-User ie for CallProceeding"
                  " message failed\n");
      return OO_FAILED;
   }
   memset (q931msg->userInfo, 0, sizeof(H225H323_UserInformation));
   q931msg->userInfo->h323_uu_pdu.m.h245TunnelingPresent=1;
   q931msg->userInfo->h323_uu_pdu.h245Tunneling = gH323ep.h245Tunneling;
   q931msg->userInfo->h323_uu_pdu.h323_message_body.t =
         T_H225H323_UU_PDU_h323_message_body_callProceeding;
  
   callProceeding = (H225CallProceeding_UUIE*)ASN1MALLOC(q931msg->pctxt,
                                             sizeof(H225CallProceeding_UUIE));
   if(!callProceeding)
   {
      OOTRACEERR1("ERROR:Memory Allocation failed for Call Proceeding UUIE\n");
      return OO_FAILED;
   }
   memset(callProceeding, 0, sizeof(H225CallProceeding_UUIE));
   q931msg->userInfo->h323_uu_pdu.h323_message_body.u.callProceeding = callProceeding;
   callProceeding->m.multipleCallsPresent = 1;
   callProceeding->m.maintainConnectionPresent = 1;
   callProceeding->multipleCalls = FALSE;
   callProceeding->maintainConnection = FALSE;

   callProceeding->m.callIdentifierPresent = 1;
   callProceeding->callIdentifier.guid.numocts =
                                   call->callIdentifier.guid.numocts;
   memcpy(callProceeding->callIdentifier.guid.data,
          call->callIdentifier.guid.data,
          call->callIdentifier.guid.numocts);
   callProceeding->protocolIdentifier = gProtocolID; 
   callProceeding->destinationInfo.m.vendorPresent = 1;
   vendor = &callProceeding->destinationInfo.vendor;
   if(gH323ep.productID)
   {
      vendor->m.productIdPresent = 1;
      vendor->productId.numocts = ASN1MIN(strlen(gH323ep.productID),
                                    sizeof(vendor->productId.data));
      strncpy(vendor->productId.data, gH323ep.productID,
              vendor->productId.numocts);
   }
   if(gH323ep.versionID)
   {
      vendor->m.versionIdPresent = 1;
      vendor->versionId.numocts = ASN1MIN(strlen(gH323ep.versionID),
                                     sizeof(vendor->versionId.data));
      strncpy(vendor->versionId.data, gH323ep.versionID,
              vendor->versionId.numocts);
   }

   vendor->vendor.t35CountryCode = gH323ep.t35CountryCode;
   vendor->vendor.t35Extension = gH323ep.t35Extension;
   vendor->vendor.manufacturerCode = gH323ep.manufacturerCode;
     
  
   ooSendH225Msg(call, q931msg);
   OOTRACEINFO3("Built Call Proceeding(%s, %s)\n", call->callType,
                 call->callToken);
   return OO_OK;
}

int ooSendAlerting(ooCallData *call)
{
   int ret;   
   H225Alerting_UUIE *alerting;
   H225VendorIdentifier *vendor;
   Q931Message *q931msg=NULL;
   ret = ooCreateQ931Message(&q931msg, Q931AlertingMsg);
   if(ret != OO_OK)
   {     
      OOTRACEERR1("Error: In allocating memory for - H225 "
                  "Alerting message\n");
      return OO_FAILED;
   }
  
   q931msg->userInfo = (H225H323_UserInformation*)ASN1MALLOC(q931msg->pctxt,
                             sizeof(H225H323_UserInformation));
   if(!q931msg->userInfo)
   {
      OOTRACEERR1("ERROR:Memory allocation for User-User ie for Alerting"
                  " message failed\n");
      return OO_FAILED;
   }
   memset (q931msg->userInfo, 0, sizeof(H225H323_UserInformation));
   q931msg->userInfo->h323_uu_pdu.m.h245TunnelingPresent=1;
   q931msg->userInfo->h323_uu_pdu.h245Tunneling = gH323ep.h245Tunneling;
   q931msg->userInfo->h323_uu_pdu.h323_message_body.t =
         T_H225H323_UU_PDU_h323_message_body_alerting;
  
   alerting = (H225Alerting_UUIE*)ASN1MALLOC(q931msg->pctxt,
                                             sizeof(H225Alerting_UUIE));
   if(!alerting)
   {
      OOTRACEERR1("ERROR:Memory allocation for Alerting UUIE failed\n");
      return OO_FAILED;
   }
   memset(alerting, 0, sizeof(H225Alerting_UUIE));
   q931msg->userInfo->h323_uu_pdu.h323_message_body.u.alerting = alerting;
   alerting->m.multipleCallsPresent = 1;
   alerting->m.maintainConnectionPresent = 1;
   alerting->multipleCalls = FALSE;
   alerting->maintainConnection = FALSE;

   alerting->m.callIdentifierPresent = 1;
   alerting->callIdentifier.guid.numocts =
                                   call->callIdentifier.guid.numocts;
   memcpy(alerting->callIdentifier.guid.data,
          call->callIdentifier.guid.data,
          call->callIdentifier.guid.numocts);
   alerting->protocolIdentifier = gProtocolID; 
   alerting->destinationInfo.m.vendorPresent = 1;
   vendor = &alerting->destinationInfo.vendor;
   if(gH323ep.productID)
   {
      vendor->m.productIdPresent = 1;
      vendor->productId.numocts = ASN1MIN(strlen(gH323ep.productID),
                                        sizeof(vendor->productId.data));
      strncpy(vendor->productId.data, gH323ep.productID,
                                        vendor->productId.numocts);
   }
   if(gH323ep.versionID)
   {
      vendor->m.versionIdPresent = 1;
      vendor->versionId.numocts = ASN1MIN(strlen(gH323ep.versionID),
                                        sizeof(vendor->versionId.data));
      strncpy(vendor->versionId.data, gH323ep.versionID,
              vendor->versionId.numocts);
   }
     
   vendor->vendor.t35CountryCode = gH323ep.t35CountryCode;
   vendor->vendor.t35Extension = gH323ep.t35Extension;
   vendor->vendor.manufacturerCode = gH323ep.manufacturerCode;
  

  
   ooSendH225Msg(call, q931msg);
   OOTRACEINFO3("Built Alerting (%s, %s)\n", call->callType, call->callToken);
   return OO_OK;
}


int ooSendFacility(ooCallData *call)
{
   int ret=0;
   Q931Message *pQ931Msg = NULL;
   H225Facility_UUIE *facility=NULL;
   OOTRACEDBGA3("Building Facility message (%s, %s)\n", call->callType,
                 call->callToken);
   ret = ooCreateQ931Message(&pQ931Msg, Q931FacilityMsg);
   if(ret != OO_OK)
   {
      OOTRACEERR3("ERROR: In allocating memory for facility message (%s, %s)\n",
                   call->callType, call->callToken);
      return OO_FAILED;
   }
   pQ931Msg->userInfo = (H225H323_UserInformation*)ASN1MALLOC(pQ931Msg->pctxt,
                             sizeof(H225H323_UserInformation));
   if(!pQ931Msg->userInfo)
   {
      OOTRACEERR3("ERROR:Memory allocation for User-User ie for Facility"
                  " message failed (%s, %s)\n", call->callType, call->callToken);
      return OO_FAILED;
   }
   memset (pQ931Msg->userInfo, 0, sizeof(H225H323_UserInformation));
   pQ931Msg->userInfo->h323_uu_pdu.m.h245TunnelingPresent=1;
   pQ931Msg->userInfo->h323_uu_pdu.h245Tunneling = gH323ep.h245Tunneling;
   pQ931Msg->userInfo->h323_uu_pdu.h323_message_body.t =
         T_H225H323_UU_PDU_h323_message_body_facility;
  
   facility = (H225Facility_UUIE*)ASN1MALLOC(pQ931Msg->pctxt,
                                             sizeof(H225Facility_UUIE));
   if(!facility)
   {
      OOTRACEERR3("ERROR:Memory allocation for facility UUIE failed (%s, %s)\n",
                   call->callType, call->callToken);
      return OO_FAILED;
   }
   memset(facility, 0, sizeof(H225Facility_UUIE));
   pQ931Msg->userInfo->h323_uu_pdu.h323_message_body.u.facility = facility;
   /* Populate Facility UUIE */
   facility->protocolIdentifier = gProtocolID; 
   facility->m.callIdentifierPresent = 1;
   facility->callIdentifier.guid.numocts =
                                   call->callIdentifier.guid.numocts;
   memcpy(facility->callIdentifier.guid.data,
          call->callIdentifier.guid.data,
          call->callIdentifier.guid.numocts);
   facility->reason.t = T_H225FacilityReason_transportedInformation;
   ooSendH225Msg(call, pQ931Msg);
   OOTRACEINFO3("Built Facility message to send (%s, %s)\n", call->callType,
                 call->callToken);

   return OO_OK;
}

int ooSendReleaseComplete(ooCallData *call)
{
   int ret, ieLen, standard = 0, location = 0;  
   Q931InformationElement* ie=NULL;
   ASN1OCTET dataBuf[1024];
   Q931Message *q931msg=NULL;
   H225ReleaseComplete_UUIE *releaseComplete;
  
   OOTRACEDBGA3("Building Release Complete message to send(%s, %s)\n",
                call->callType, call->callToken);
   ret = ooCreateQ931Message(&q931msg, Q931ReleaseCompleteMsg);
   if(ret != OO_OK)
   {     
      OOTRACEERR3("Error: In allocating memory for - H225 Release Complete "
                  "message(%s, %s)\n", call->callType, call->callToken);
      if(call->callState < OO_CALL_CLEAR)
      {
         call->callEndReason = OO_HOST_CLEARED;
         call->callState = OO_CALL_CLEAR;
      }
      return OO_FAILED;
   }
   q931msg->userInfo = (H225H323_UserInformation*)ASN1MALLOC(q931msg->pctxt,
                             sizeof(H225H323_UserInformation));
   if(!q931msg->userInfo)
   {
      OOTRACEERR1("ERROR:Memory allocation for User-User ie for Alerting"
                  " message failed\n");
      return OO_FAILED;
   }
   memset (q931msg->userInfo, 0, sizeof(H225H323_UserInformation));

   releaseComplete = (H225ReleaseComplete_UUIE*)ASN1MALLOC(q931msg->pctxt,
                                             sizeof(H225ReleaseComplete_UUIE));
   memset(releaseComplete, 0, sizeof(H225ReleaseComplete_UUIE));
   q931msg->userInfo->h323_uu_pdu.m.h245TunnelingPresent=1;
   q931msg->userInfo->h323_uu_pdu.h245Tunneling = gH323ep.h245Tunneling;
   q931msg->userInfo->h323_uu_pdu.h323_message_body.t =
         T_H225H323_UU_PDU_h323_message_body_releaseComplete;
  
   /* Add cause ie */
   ieLen = 2;
   dataBuf[0] = (0x80 | ((standard&3)<<5) | (location&15));
   dataBuf[1] = (0x80 | Q931NormalCallClearing);  
      
   /* Allocate memory to hold complete Cause IE Information */
   ie = (Q931InformationElement*)ASN1MALLOC (q931msg->pctxt,
                                     sizeof(*ie) - sizeof(ie->data) + ieLen);
   if(ie == NULL)
   {
      OOTRACEERR3("Error: Allocation of Q931Information Element Failed "
                  "for Release Complete(%s, %s)\n", call->callType,
                  call->callToken);
      if(call->callState < OO_CALL_CLEAR)
      {
         call->callEndReason = OO_HOST_CLEARED;
         call->callState = OO_CALL_CLEAR;
      }
      return OO_FAILED;
   }
   ie->discriminator = Q931CauseIE;
   ie->length = ieLen;
   memcpy(ie->data, dataBuf, ieLen);

   /* NOTE: ALL IEs SHOULD BE IN ASCENDING ORDER OF
      THEIR DISCRIMINATOR AS PER SPEC. WE NEED TO TAKE CARE OF THIS.
   */
   dListInit (&(q931msg->ies));
   if((dListAppend (q931msg->pctxt,
                    &(q931msg->ies), ie)) == NULL)
   {
      OOTRACEERR3("Error: Failed to add UUIE in ReleaseComplete message "
                  "(%s, %s)\n", call->callType, call->callToken);
      if(call->callState < OO_CALL_CLEAR)
      {
         call->callEndReason = OO_HOST_CLEARED;
         call->callState = OO_CALL_CLEAR;
      }
      return OO_FAILED;
   }

   /* Add user-user ie */
#ifndef _COMPACT
   dListInit(&q931msg->userInfo->extElem1);
   dListInit(&q931msg->userInfo->user_data.extElem1);
   dListInit(&q931msg->userInfo->h323_uu_pdu.extElem1);
#endif
   q931msg->userInfo->h323_uu_pdu.m.h245TunnelingPresent=1;
   q931msg->userInfo->h323_uu_pdu.h245Tunneling = FALSE;
   q931msg->userInfo->h323_uu_pdu.h323_message_body.t =
           T_H225H323_UU_PDU_h323_message_body_releaseComplete;
  
   q931msg->userInfo->h323_uu_pdu.h323_message_body.u.releaseComplete =
                                                             releaseComplete;
#ifndef _COMPACT
   dListInit(&releaseComplete->extElem1);
#endif
   releaseComplete->m.callIdentifierPresent = 1;
   releaseComplete->protocolIdentifier = gProtocolID;
   releaseComplete->callIdentifier.guid.numocts =
           call->callIdentifier.guid.numocts;
   memcpy(releaseComplete->callIdentifier.guid.data,
                                  call->callIdentifier.guid.data,
                                  call->callIdentifier.guid.numocts);
#ifndef _COMPACT
   dListInit(&releaseComplete->callIdentifier.extElem1);
#endif
     
   /* Send H225 message */  
   ooSendH225Msg(call, q931msg);
   OOTRACEINFO3("Built Release Complete message (%s, %s)\n",
                call->callType, call->callToken);
   return OO_OK;
}


int ooSendConnect(ooCallData *call)
{
   ooAcceptCall(call);
   return OO_OK;
}

int ooAcceptCall(ooCallData *call)
{
   int ret = 0, i=0, j=0, remoteRtpPort=0;
   /*char localip[20], hexip[20];*/
   char hexip[20];
   int addr_part1, addr_part2, addr_part3, addr_part4;
   H225Connect_UUIE *connect;
   H225TransportAddress_ipAddress *h245IpAddr;
   H225VendorIdentifier *vendor;
   Q931Message *q931msg=NULL;
   DListNode *pNode = NULL;
   H245OpenLogicalChannel *olc = NULL, *respOlc = NULL;
   ooH323EpCapability *epCap = NULL;
   ASN1DynOctStr *pFS=NULL;
   OOCTXT*pctxt;
   H245H2250LogicalChannelParameters * h2250lcp = NULL; 
   H245UnicastAddress *unicastAddress;
   ooLogicalChannel* pChannel;

   ret = ooCreateQ931Message(&q931msg, Q931ConnectMsg);
   if(ret != OO_OK)
   {     
      OOTRACEERR1("Error: In allocating memory for - H225 "
                  "Connect message\n");
      return OO_FAILED;
   }
   pctxt = q931msg->pctxt;
   q931msg->userInfo = (H225H323_UserInformation*)ASN1MALLOC(q931msg->pctxt,
                             sizeof(H225H323_UserInformation));
   if(!q931msg->userInfo)
   {
      OOTRACEERR1("ERROR:Memory allocation for User-User ie for Connect"
                  " message failed\n");
      return OO_FAILED;
   }  
   memset (q931msg->userInfo, 0, sizeof(H225H323_UserInformation));
   q931msg->userInfo->h323_uu_pdu.m.h245TunnelingPresent=1;
   q931msg->userInfo->h323_uu_pdu.h245Tunneling = FALSE;
   q931msg->userInfo->h323_uu_pdu.h323_message_body.t =
                     T_H225H323_UU_PDU_h323_message_body_connect;
  
   connect = (H225Connect_UUIE*)ASN1MALLOC(q931msg->pctxt,
                                           sizeof(H225Connect_UUIE));
   if(!connect)
   {
      OOTRACEERR1("ERROR:Memory allocation for Connect message failed\n");
      return OO_FAILED;
   }
   memset(connect, 0, sizeof(H225Connect_UUIE));
   q931msg->userInfo->h323_uu_pdu.h323_message_body.u.connect = connect;
   connect->m.fastStartPresent = 0;
   connect->m.multipleCallsPresent = 1;
   connect->m.maintainConnectionPresent = 1;
   connect->multipleCalls = FALSE;
   connect->maintainConnection = FALSE;
  
    /* Add h245 listener address. Do not add H245 listener address in case
       of fast-start. */
   if(!gH323ep.fastStart || call->remoteFastStartOLCs.count == 0)
   {
      ooCreateH245Listener(call); /* First create an H.245 listener */
      connect->m.h245AddressPresent = 1;
      connect->h245Address.t = T_H225TransportAddress_ipAddress;
  
      h245IpAddr = (H225TransportAddress_ipAddress*)ASN1MALLOC(q931msg->pctxt,
                                          sizeof(H225TransportAddress_ipAddress));
      memset(h245IpAddr, 0, sizeof(H225TransportAddress_ipAddress));

      /*ooGetLocalIPAddress(localip);
      sscanf(localip, "%d.%d.%d.%d", &addr_part1, &addr_part2,
                                     &addr_part3, &addr_part4);*/
      sscanf(gH323ep.signallingIP, "%d.%d.%d.%d", &addr_part1, &addr_part2,
                                     &addr_part3, &addr_part4);
      sprintf(hexip, "%x %x %x %x", addr_part1, addr_part2, addr_part3,
                                    addr_part4);
      sscanf(hexip, "%x %x %x %x", &h245IpAddr->ip.data[0], &h245IpAddr->ip.data[1],
              &h245IpAddr->ip.data[2], &h245IpAddr->ip.data[3]);
  
      h245IpAddr->ip.numocts=4;
      h245IpAddr->port = *(call->h245listenport);
      connect->h245Address.u.ipAddress = h245IpAddr;
   }
   connect->conferenceID.numocts = 16;
   for (i = 0; i < 16; i++)
      connect->conferenceID.data[i] = i + 1;

   connect->m.callIdentifierPresent = 1;
   connect->callIdentifier.guid.numocts =
                                 call->callIdentifier.guid.numocts;
   memcpy(connect->callIdentifier.guid.data, call->callIdentifier.guid.data,
                                         call->callIdentifier.guid.numocts);
   connect->protocolIdentifier = gProtocolID; 

   connect->destinationInfo.m.vendorPresent = 1;
   vendor = &connect->destinationInfo.vendor;
     
   vendor->vendor.t35CountryCode = gH323ep.t35CountryCode;
   vendor->vendor.t35Extension = gH323ep.t35Extension;
   vendor->vendor.manufacturerCode = gH323ep.manufacturerCode;
   if(gH323ep.productID)
   {
      vendor->m.productIdPresent = 1;
      vendor->productId.numocts = ASN1MIN(strlen(gH323ep.productID),
                                            sizeof(vendor->productId.data));
      strncpy(vendor->productId.data, gH323ep.productID,
                                                   vendor->productId.numocts);
   }
   if(gH323ep.versionID)
   {
      vendor->m.versionIdPresent = 1;
      vendor->versionId.numocts = ASN1MIN(strlen(gH323ep.versionID),
                                           sizeof(vendor->versionId.data));
      strncpy(vendor->versionId.data, gH323ep.versionID,
                                                   vendor->versionId.numocts);
   }
   /* If fast start supported and remote endpoint has sent faststart element */
   if(gH323ep.fastStart && call->remoteFastStartOLCs.count>0)
   {
      pFS = (ASN1DynOctStr*)ASN1MALLOC(q931msg->pctxt,
                        call->remoteFastStartOLCs.count*sizeof(ASN1DynOctStr));
      memset(pFS, 0, call->remoteFastStartOLCs.count*sizeof(ASN1DynOctStr));

      connect->m.fastStartPresent = TRUE;
      for(i=0, j=0; i<(int)call->remoteFastStartOLCs.count; i++)
      {
         pNode = dListFindByIndex(&call->remoteFastStartOLCs, i);
         olc = (H245OpenLogicalChannel*)pNode->data;
         /* Forward Channel */
         if(olc->forwardLogicalChannelParameters.dataType.t != T_H245DataType_nullData)
         {
            epCap = ooIsDataTypeSupported(call,
                                &olc->forwardLogicalChannelParameters.dataType,
                                T_H245Capability_receiveAudioCapability);
            if(!epCap)
               continue;
            OOTRACEINFO1("Receive Channel data type supported\n");
            if(olc->forwardLogicalChannelParameters.multiplexParameters.t !=
               T_H245OpenLogicalChannel_forwardLogicalChannelParameters_multiplexParameters_h2250LogicalChannelParameters)
            {
               OOTRACEERR4("ERROR:Unknown multiplex parameter type for "
                           "channel %d (%s, %s)\n",
                           olc->forwardLogicalChannelNumber,
                           call->callType, call->callToken);
               continue;
            }
            if(ooIsSessionEstablished(call, olc->forwardLogicalChannelParameters.multiplexParameters.u.h2250LogicalChannelParameters->sessionID, "receive"))
               continue;
         }
         else if(olc->m.reverseLogicalChannelParametersPresent)
         {
            epCap = ooIsDataTypeSupported(call,
                                &olc->reverseLogicalChannelParameters.dataType,
                                T_H245Capability_transmitAudioCapability);
            if(!epCap)
               continue;
            OOTRACEINFO1("Transmit Channel data type supported\n");
            if(olc->reverseLogicalChannelParameters.multiplexParameters.t !=
               T_H245OpenLogicalChannel_reverseLogicalChannelParameters_multiplexParameters_h2250LogicalChannelParameters)
            {
               OOTRACEERR4("ERROR:Unknown multiplex parameter type for "
                           "channel %d (%s, %s)\n",
                           olc->forwardLogicalChannelNumber,
                           call->callType, call->callToken);
               continue;
            }
            if(ooIsSessionEstablished(call, olc->reverseLogicalChannelParameters.multiplexParameters.u.h2250LogicalChannelParameters->sessionID, "transmit"))
               continue;
           
             /* Extract the remote media endpoint address */
            h2250lcp = olc->reverseLogicalChannelParameters.multiplexParameters.u.h2250LogicalChannelParameters;
            if(!h2250lcp)
            {
               OOTRACEERR3("ERROR:Invalid OLC received in fast start. No "
                           "reverse Logical Channel Parameters found. "
                           "(%s, %s)\n", call->callType, call->callToken);
               return OO_FAILED;
            }
            if(!h2250lcp->m.mediaChannelPresent)
            {
               OOTRACEERR3("ERROR:Invalid OLC received in fast start. No "
                           "reverse media channel information found. "
                           "(%s, %s)\n", call->callType, call->callToken);
               return OO_FAILED;
            }
            if(h2250lcp->mediaChannel.t !=
                                         T_H245TransportAddress_unicastAddress)
            {
               OOTRACEERR3("ERROR:Unsupported media channel address type "
                           "(%s, %s)\n", call->callType, call->callToken);
               return OO_FAILED;
            }
     
            unicastAddress = h2250lcp->mediaChannel.u.unicastAddress;
            if(unicastAddress->t != T_H245UnicastAddress_iPAddress)
            {
               OOTRACEERR3("ERROR:media channel address type is not IP"
                           "(%s, %s)\n", call->callType, call->callToken);
               return OO_FAILED;
            }
            remoteRtpPort = unicastAddress->u.iPAddress->tsapIdentifier; 
         }
         respOlc = (H245OpenLogicalChannel*) ASN1MALLOC(pctxt,
                                               sizeof(H245OpenLogicalChannel));
         memset(respOlc, 0, sizeof(H245OpenLogicalChannel));

         respOlc->forwardLogicalChannelNumber = olc->forwardLogicalChannelNumber;
        
         ooBuildOpenLogicalChannelAudio(call, respOlc, epCap, q931msg->pctxt);
        
         pChannel = ooFindLogicalChannelByLogicalChannelNo(call,
                                         respOlc->forwardLogicalChannelNumber);
  
         if(olc->forwardLogicalChannelParameters.dataType.t ==
                                                      T_H245DataType_nullData)
         {
            pChannel->remoteRtpPort = remoteRtpPort;
            if(epCap->startTransmitChannel)
            {  
               epCap->startTransmitChannel(call, pChannel);     
               OOTRACEINFO3("Transmit channel of type audio started "
                            "(%s, %s)\n", call->callType, call->callToken);
               call->isAudioActive = 1;
            }
            else{
               OOTRACEERR3("ERROR:No callback registered to start transmit"
                           " audio channel (%s, %s)\n", call->callType,
                           call->callToken);
               return OO_FAILED;
            }
         }
         /* Do not specify msg buffer let automatic allocation work */
         setPERBuffer(q931msg->pctxt, NULL, 0, 1);
         if(asn1PE_H245OpenLogicalChannel(pctxt, respOlc) != ASN_OK)
         {
            OOTRACEERR3("ERROR:Encoding of olc failed for faststart "
                        "(%s, %s)\n", call->callType, call->callToken);
            freeContext(pctxt);
            ASN1CRTFREE0(pctxt);
            if(call->callState < OO_CALL_CLEAR)
            {
               call->callEndReason = OO_HOST_CLEARED;
               call->callState = OO_CALL_CLEAR;
            }
            return OO_FAILED;
         }
         pFS[j].data = encodeGetMsgPtr(pctxt, &(pFS[j].numocts));
         respOlc = NULL;
         olc = NULL;
                 j++;
      }
      OOTRACEINFO4("Added %d fast start elements to CONNECT message "
                   "(%s, %s)\n",  j, call->callType, call->callToken);
      connect->fastStart.n = j;
      connect->fastStart.elem = pFS;
   }
   ooSendH225Msg(call, q931msg);
   OOTRACEINFO3("Built H.225 Connect message (%s, %s)\n", call->callType,
                 call->callToken);

   if(gH323ep.onIncomingCall)
      gH323ep.onIncomingCall(call);
   return OO_OK;
}

int ooH323MakeCall(char *destip, int port, char *callToken)
{
   OOCTXT *pctxt;
   ooCallData *call;
   int ret=0;
   if(!destip)
   {
      OOTRACEERR1("ERROR:Invalid destination ip address\n");
      return OO_FAILED;
   }
   if(!callToken)
   {
      OOTRACEERR1("ERROR: Invalid callToken parameter to make call\n");
      return OO_FAILED;
   }

   call = ooCreateCall("outgoing", callToken);
   pctxt = call->pctxt;
   memcpy(call->remoteIP, destip, strlen(destip));
   call->remotePort = port;
  
   ret = ooCreateH225Connection(call);
   if(ret != OO_OK)
   {
      OOTRACEERR3("ERROR:Failed to create H225 connection to %s:%d\n",
                   destip, port);
      return OO_FAILED;
   }

   ret = ooH323MakeCall_helper(call);
  
   strcpy(callToken, call->callToken);
  
   return OO_OK;
}


int ooH323MakeCall_helper(ooCallData *call)
{
   int ret=0,i;
   Q931Message *q931msg = NULL;
   H225Setup_UUIE *setup;

   ASN1DynOctStr *pFS=NULL;
   H225TransportAddress_ipAddress *destCallSignalIpAddress;
   int addr_part1, addr_part2, addr_part3, addr_part4;
   /*char localip[20], hexip[20];*/
   char hexip[20];
   H225TransportAddress_ipAddress *srcCallSignalIpAddress;
   ooH323EpCapability *epCap=NULL;
   DListNode *curNode = NULL;
   OOCTXT *pctxt = NULL;
   H245OpenLogicalChannel *olc;
   ASN1BOOL aligned = 1;
   H225AliasAddress * pAliasAddress=NULL;
  
   ret = ooCreateQ931Message(&q931msg, Q931SetupMsg);
   if(ret != OO_OK)
   {
      OOTRACEERR1("ERROR:Failed to Create Q931 SETUP Message\n ");
      return OO_FAILED;
   }
   /* If setup message then generate new call ref, otherwise use
      existing call ref.
   */
   q931msg->callReference = ooGenerateCallReference();
   call->callReference = q931msg->callReference;
   pctxt = q931msg->pctxt;

   q931msg->userInfo = (H225H323_UserInformation*)ASN1MALLOC(q931msg->pctxt,
                             sizeof(H225H323_UserInformation));
   if(!q931msg->userInfo)
   {
      OOTRACEERR1("ERROR:Memory allocation for User-User ie for SETUP"
                  " message failed\n");
      return OO_FAILED;
   }
   memset(q931msg->userInfo, 0, sizeof(H225H323_UserInformation));

   setup = (H225Setup_UUIE*) ASN1MALLOC(pctxt, sizeof(H225Setup_UUIE));
   memset (setup, 0, sizeof(H225Setup_UUIE));
   setup->protocolIdentifier = gProtocolID;
  
   /* Populate Alias Address. Note we use callername as alias as well as
      display name*/
   if(strlen(gH323ep.callername)>0)
   {
      setup->m.sourceAddressPresent = TRUE;
      pAliasAddress = (H225AliasAddress*)ASN1MALLOC(pctxt,
                                                    sizeof(H225AliasAddress));
      memset(pAliasAddress, 0, sizeof(H225AliasAddress));
      pAliasAddress->t = T_H225AliasAddress_h323_ID;
      pAliasAddress->u.h323_ID.nchars = strlen(gH323ep.callername);
      pAliasAddress->u.h323_ID.data = (ASN116BITCHAR*)ASN1MALLOC(pctxt,
                            strlen(gH323ep.callername)*sizeof(ASN116BITCHAR));
      for(i=0;i<(int)strlen(gH323ep.callername); i++)
        pAliasAddress->u.h323_ID.data[i] = (ASN116BITCHAR)gH323ep.callername[i];
      dListInit(&setup->sourceAddress);
      dListAppend(pctxt, &setup->sourceAddress, pAliasAddress);
   }

   /* Populate the vendor information */
   setup->sourceInfo.m.vendorPresent=TRUE;
   setup->sourceInfo.vendor.vendor.t35CountryCode = gH323ep.t35CountryCode;
   setup->sourceInfo.vendor.vendor.t35Extension = gH323ep.t35Extension;
   setup->sourceInfo.vendor.vendor.manufacturerCode= gH323ep.manufacturerCode;
  
   if(gH323ep.productID)
   {
      setup->sourceInfo.vendor.m.productIdPresent=TRUE;
      setup->sourceInfo.vendor.productId.numocts = ASN1MIN(
                              strlen(gH323ep.productID),
                              sizeof(setup->sourceInfo.vendor.productId.data));
      strncpy((char*)setup->sourceInfo.vendor.productId.data,
                gH323ep.productID, setup->sourceInfo.vendor.productId.numocts);
   }
   else
      setup->sourceInfo.vendor.m.productIdPresent=FALSE;
  
   if(gH323ep.versionID)
   {
      setup->sourceInfo.vendor.m.versionIdPresent=TRUE;
      setup->sourceInfo.vendor.versionId.numocts = ASN1MIN(
                              strlen(gH323ep.versionID),
                              sizeof(setup->sourceInfo.vendor.versionId.data));
      strncpy((char*)setup->sourceInfo.vendor.versionId.data,
              gH323ep.versionID, setup->sourceInfo.vendor.versionId.numocts);
   }
   else
      setup->sourceInfo.vendor.m.versionIdPresent=FALSE;
  
   setup->sourceInfo.mc = FALSE;
   setup->sourceInfo.undefinedNode = FALSE;

   /* Populate the destination Call Signal Address */
   setup->destCallSignalAddress.t=T_H225TransportAddress_ipAddress;
  
   sscanf(call->remoteIP, "%d.%d.%d.%d", &addr_part1, &addr_part2,
                                         &addr_part3, &addr_part4);
   sprintf(hexip, "%x %x %x %x", addr_part1, addr_part2, addr_part3,
                                 addr_part4);
   destCallSignalIpAddress = (H225TransportAddress_ipAddress*)ASN1MALLOC(pctxt,
                                  sizeof(H225TransportAddress_ipAddress));
   sscanf(hexip, "%x %x %x %x", &destCallSignalIpAddress->ip.data[0],
                                &destCallSignalIpAddress->ip.data[1],
                                &destCallSignalIpAddress->ip.data[2],
                                &destCallSignalIpAddress->ip.data[3]);

   destCallSignalIpAddress->ip.numocts=4;
   destCallSignalIpAddress->port = call->remotePort;

   setup->destCallSignalAddress.u.ipAddress = destCallSignalIpAddress;
   setup->m.destCallSignalAddressPresent=TRUE;
   setup->activeMC=FALSE;

   /* Populate the source Call Signal Address */
   setup->sourceCallSignalAddress.t=T_H225TransportAddress_ipAddress;
  /*  ooGetLocalIPAddress(localip);
   sscanf(localip, "%d.%d.%d.%d", &addr_part1, &addr_part2, &addr_part3,
   &addr_part4);*/
   sscanf(gH323ep.signallingIP, "%d.%d.%d.%d", &addr_part1, &addr_part2,
                                               &addr_part3, &addr_part4);

   sprintf(hexip, "%x %x %x %x", addr_part1, addr_part2, addr_part3,
                                 addr_part4);
   srcCallSignalIpAddress = (H225TransportAddress_ipAddress*)ASN1MALLOC(pctxt,
                                  sizeof(H225TransportAddress_ipAddress));
   sscanf(hexip, "%x %x %x %x", &srcCallSignalIpAddress->ip.data[0],
                                &srcCallSignalIpAddress->ip.data[1],
                                &srcCallSignalIpAddress->ip.data[2],
                                &srcCallSignalIpAddress->ip.data[3]);

   srcCallSignalIpAddress->ip.numocts=4;
   srcCallSignalIpAddress->port= *(call->h225ChanPort);
   setup->sourceCallSignalAddress.u.ipAddress = srcCallSignalIpAddress;
   setup->m.sourceCallSignalAddressPresent=TRUE;
   /* No fast start */
   if(!gH323ep.fastStart)
   {
      setup->m.fastStartPresent = FALSE;
   }
   else{
      setup->m.fastStartPresent = TRUE;
      pFS = (ASN1DynOctStr*)ASN1MALLOC(pctxt, gH323ep.audioCaps.count*
                                       sizeof(ASN1DynOctStr));
      for(i=0; i<(int) gH323ep.audioCaps.count; i++)
      {
         /* Retrieve local capability */
         curNode = dListFindByIndex (&gH323ep.audioCaps, i) ;
         if(!curNode)
         {
            OOTRACEERR3("ERROR: Failed to get handle of capability (%s, %s)\n",
                        call->callType, call->callToken);
            freeContext(pctxt);
            ASN1CRTFREE0(pctxt);
            if(call->callState < OO_CALL_CLEAR)
            {
               call->callEndReason = OO_HOST_CLEARED;
               call->callState = OO_CALL_CLEAR;
            }
            return OO_FAILED;
         }
         epCap = (ooH323EpCapability*)curNode->data;
         olc = (H245OpenLogicalChannel*)ASN1MALLOC(pctxt,
                                               sizeof(H245OpenLogicalChannel));
         if(!olc)
         {
            OOTRACEERR3("ERROR:Allocating memory for OLC, in faststart SETUP "
                        "message (%s, %s)\n", call->callType, call->callToken);
            freeContext(pctxt);
            ASN1CRTFREE0(pctxt);
            if(call->callState < OO_CALL_CLEAR)
            {
               call->callEndReason = OO_HOST_CLEARED;
               call->callState = OO_CALL_CLEAR;
            }
            return OO_FAILED;
         }
         memset(olc, 0, sizeof(H245OpenLogicalChannel));
         olc->forwardLogicalChannelNumber = call->logicalChanNoCur++;
         if(call->logicalChanNoCur > call->logicalChanNoMax)
            call->logicalChanNoCur = call->logicalChanNoBase;
       
         ooBuildOpenLogicalChannelAudio(call, olc, epCap, pctxt);
         /* Do not specify msg buffer let automatic allocation work */
         setPERBuffer(pctxt, NULL, 0, aligned);
         if(asn1PE_H245OpenLogicalChannel(pctxt, olc) != ASN_OK)
         {
            OOTRACEERR3("ERROR:Encoding of olc failed for faststart (%s, %s)"
                        "\n", call->callType, call->callToken);
            freeContext(pctxt);
            ASN1CRTFREE0(pctxt);
            if(call->callState < OO_CALL_CLEAR)
            {
               call->callEndReason = OO_HOST_CLEARED;
               call->callState = OO_CALL_CLEAR;
            }
            return OO_FAILED;
         }
         pFS[i].data = encodeGetMsgPtr(pctxt, &(pFS[i].numocts));
         olc = NULL;
      }
      OOTRACEINFO4("Added %d fast start elements to SETUP message (%s, %s)\n",
                   i, call->callType, call->callToken);
      setup->fastStart.n = i;
      setup->fastStart.elem = pFS;
   }
  
   setup->conferenceID.numocts = 16;
   for (i = 0; i < 16; i++)
      setup->conferenceID.data[i] = i + 1;
   setup->conferenceGoal.t = T_H225Setup_UUIE_conferenceGoal_create;
   /* H.225 point to point call */
   setup->callType.t = T_H225CallType_pointToPoint;
 
   /* Populate optional fields */
   setup->m.callIdentifierPresent = TRUE;
   ooGenerateCallIdentifier(&setup->callIdentifier);
   call->callIdentifier.guid.numocts = setup->callIdentifier.guid.numocts;
   memcpy(call->callIdentifier.guid.data, setup->callIdentifier.guid.data,
                               setup->callIdentifier.guid.numocts);
  
   setup->m.mediaWaitForConnectPresent = TRUE;
   setup->mediaWaitForConnect = FALSE;
   setup->m.canOverlapSendPresent = TRUE;
   setup->canOverlapSend = FALSE;

   /* Populate the userInfo structure with the setup UUIE */
  
   q931msg->userInfo->h323_uu_pdu.h323_message_body.t =
         T_H225H323_UU_PDU_h323_message_body_setup;
   q931msg->userInfo->h323_uu_pdu.h323_message_body.u.setup = setup;
   q931msg->userInfo->h323_uu_pdu.m.h245TunnelingPresent=1;
  
   q931msg->userInfo->h323_uu_pdu.h245Tunneling = gH323ep.h245Tunneling;
   /* For H.323 version 4 and higher, if fast connect, tunneling should be
      supported.
   */
   if(gH323ep.fastStart)
      q931msg->userInfo->h323_uu_pdu.h245Tunneling = TRUE;
   ooSendH225Msg(call, q931msg);
   OOTRACEINFO3("Built SETUP message (%s, %s)\n", call->callType,
                 call->callToken);
   return OO_OK;
}

  

int ooH323HangCall(char * callToken)
{
   int ret =0;
   ooCallData *call;

   call= ooFindCallByToken(callToken);
   if(!call)
   {
      OOTRACEWARN2("WARN: Call hangup failed - Call %s not present\n",
                    callToken);
      return OO_FAILED;
   }
   OOTRACEINFO3("Hanging up call (%s, %s)\n", call->callType, call->callToken);
   if(call->callState < OO_CALL_CLEAR)
   {
      call->callEndReason = OO_HOST_CLEARED;
      call->callState = OO_CALL_CLEAR;
   }
   return OO_OK;
}

