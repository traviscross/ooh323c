/*
 * Copyright (C) 2005 by Page Iberica, S.A.
 * Copyright (C) 2005 by Objective Systems, Inc.
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
 * @file ooGkClient.c
 * This file contains functions to support RAS protocol.
 *
 */
#include "ooGkClient.h"
#include "ootypes.h"
#include "ootrace.h"
#include "ooports.h"
#include "ooasn1.h"
#include "oochannels.h"
#include "printHandler.h"
#include "ooCalls.h"
#include "H323-MESSAGES.h"
#include "ooDateTime.h"
#include "ooq931.h"
#include "ooh323.h"
#include "ooTimer.h"
/** Global endpoint structure */
extern ooEndPoint gH323ep;




static ASN1OBJID gProtocolID = {
   6, { 0, 0, 8, 2250, 0, 4 }
};



int ooGkClientInit(enum RasGatekeeperMode eGkMode,
              char *szGkAddr, int iGkPort )
{
   ooGkClient *pGkClient=NULL;
   pGkClient = (ooGkClient*)
                         memAlloc(&gH323ep.ctxt, sizeof(ooGkClient));
   if(!pGkClient)
   {
      OOTRACEERR1("Error: Failed to allocate memory to Gatekeeper Client.\n");
      return OO_FAILED;
   }

   memset(pGkClient, 0, sizeof(ooGkClient));
   gH323ep.gkClient = pGkClient;
   initContext(&(pGkClient->ctxt));
   initContext(&(pGkClient->msgCtxt));
   pGkClient->rrqRetries = 0;
   pGkClient->grqRetries = 0;
  
   if(OO_OK != ooGkClientSetGkMode(pGkClient, eGkMode, szGkAddr, iGkPort))
   {
      OOTRACEERR1("Error:Failed to set Gk mode\n");
      memReset(&gH323ep.ctxt);
      return OO_FAILED;
   }
 
   /* Create default parameter set */
   pGkClient->grqTimeout = DEFAULT_GRQ_TIMEOUT;
   pGkClient->rrqTimeout = DEFAULT_RRQ_TIMEOUT;
   pGkClient->regTimeout = DEFAULT_REG_TTL;
   pGkClient->arqTimeout = DEFAULT_ARQ_TIMEOUT;
   pGkClient->drqTimeout = DEFAULT_DRQ_TIMEOUT;
   dListInit(&pGkClient->callsPendingList);
   dListInit(&pGkClient->callsAdmittedList);
   dListInit(&pGkClient->timerList);
   pGkClient->state = GkClientIdle;
   return OO_OK;
}


int ooGkClientReInit(ooGkClient *pGkClient)
{

   ooGkClientCloseChannel(pGkClient);
   pGkClient->gkRasIP[0]='\0';
   pGkClient->gkCallSignallingIP[0]='\0';
   pGkClient->gkRasPort = 0;
   pGkClient->gkCallSignallingPort = 0;
   pGkClient->rrqRetries = 0;
   pGkClient->grqRetries = 0;
   pGkClient->requestSeqNum = 0;
  
   dListFreeAll(&pGkClient->ctxt, &pGkClient->callsPendingList);
   dListFreeAll(&pGkClient->ctxt, &pGkClient->callsAdmittedList);
   dListFreeAll(&pGkClient->ctxt, &pGkClient->timerList);
   pGkClient->state = GkClientIdle;
   return OO_OK;
}

void ooGkClientPrintConfig(ooGkClient *pGkClient)
{
   OOTRACEINFO1("Gatekeeper Client Configuration:\n");
   if(pGkClient->gkMode == RasUseSpecificGatekeeper)
   {
      OOTRACEINFO1("\tGatekeeper mode - UseSpecificGatekeeper\n");
      OOTRACEINFO3("\tGatekeeper To Use - %s:%d\n", pGkClient->gkRasIP,
                                                    pGkClient->gkRasPort);
   }else if(pGkClient->gkMode == RasDiscoverGatekeeper)
   {
      OOTRACEINFO1("\tGatekeeper mode - UseSpecificGatekeeper\n");
   }else{
      OOTRACEERR1("Invalid GatekeeperMode\n");
   }
}

int ooGkClientDestroy(void)
{
   if(gH323ep.gkClient)
   {
      OOTRACEINFO1("Destroying Gatekeeper Client\n");
      ooGkClientCloseChannel(gH323ep.gkClient);
      freeContext(&gH323ep.gkClient->msgCtxt);
      freeContext(&gH323ep.gkClient->ctxt);
      memFreePtr(&gH323ep.ctxt, gH323ep.gkClient);
      gH323ep.gkClient = NULL;
   }
   return OO_OK;
}

int ooGkClientStart(ooGkClient *pGkClient)
{
   int iRet=0;
   iRet = ooGkClientCreateChannel(pGkClient);

   if(iRet != OO_OK)
   {
      OOTRACEERR1("Error: GkClient Channel Creation failed\n");
      return OO_FAILED;
   }
  
   pGkClient->discoveryComplete = FALSE;
   iRet = ooGkClientSendGRQ(pGkClient);
   if(iRet != OO_OK)
   {
      OOTRACEERR1("Error:Failed to send GRQ message\n");
      pGkClient->state = GkClientFailed;
      return OO_FAILED;
   }
   return OO_OK;
}
  

int ooGkClientSetGkMode(ooGkClient *pGkClient, enum RasGatekeeperMode eGkMode,
                        char *szGkAddr, int iGkPort )
{
   pGkClient->gkMode = eGkMode;
   if(eGkMode == RasUseSpecificGatekeeper)
   {
      OOTRACEINFO1("Gatekeeper Mode - RasUseSpecificGatekeeper\n");
      if(szGkAddr)
      {
         if(strlen(szGkAddr)>MAX_IP_LEN)
         {
            OOTRACEERR2("Error:Invalid IP address specified - %s\n", szGkAddr);
            return OO_FAILED;
         }
         strcpy(pGkClient->gkRasIP, szGkAddr);
      }
      if(iGkPort)
         pGkClient->gkRasPort = iGkPort;
      else
         pGkClient->gkRasPort = DEFAULT_GKPORT;

      OOTRACEINFO3("Gatekeeper IP:port set to - %s:%d\n",
                    szGkAddr,  pGkClient->gkRasPort);
   }else if(eGkMode == RasDiscoverGatekeeper)
   {
      OOTRACEINFO1("Gatekeeper Mode - RasDiscoverGatekeeper\n");
   }else if(eGkMode == RasNoGatekeeper)
   {
      OOTRACEINFO1("Gatekeeper Mode - RasNoGatekeeper\n");
   }
   return OO_OK;
}

  
/**
 * Create the RAS channel (socket).
 *
 */

int ooGkClientCreateChannel(ooGkClient *pGkClient)
{
   int ret=0;
   OOIPADDR ipaddrs;
   /* Create socket */
   if((ret=ooSocketCreateUDP(&pGkClient->rasSocket))!=ASN_OK)
   {
      OOTRACEERR1("Failed to create RAS socket\n");
      pGkClient->state = GkClientFailed;
      return OO_FAILED;
   }
   if(pGkClient->localRASPort)
   {
      ret= ooSocketStrToAddr (gH323ep.signallingIP, &ipaddrs);
      if( (ret=ooSocketBind( pGkClient->rasSocket, ipaddrs,
           pGkClient->localRASPort))==ASN_OK )
      {
         OOTRACEINFO1("H323 RAS channel creation - successful\n");
         return OO_OK;
      }
      else
      {
         OOTRACEERR1("ERROR:Failed to create RAS channel\n");
         pGkClient->state = GkClientFailed;
         return OO_FAILED;
      }
   }
   else {
      ret = ooBindPort(&gH323ep, OOUDP, pGkClient->rasSocket);
      if(ret == OO_FAILED)
      {
         OOTRACEERR1("ERROR: Failed to bind port to RAS socket\n");
         pGkClient->state = GkClientFailed;
         return OO_FAILED;
      }
      OOTRACEINFO1("H323 RAS channel creation - successful\n");
      pGkClient->localRASPort = ret;
   }
   return OO_OK;
}


int ooGkClientCloseChannel(ooGkClient *pGkClient)
{
   int ret;
   if(pGkClient->rasSocket != 0)
   {
      ret = ooSocketClose(pGkClient->rasSocket);
      if(ret != ASN_OK)
      {
         OOTRACEERR1("Error: failed to close RAS channel\n");
         pGkClient->rasSocket = 0;
         return OO_FAILED;
      }
      pGkClient->rasSocket = 0;
   }
   OOTRACEINFO1("Closed RAS channel\n");
   return OO_OK;
}


/**
 * Fill vendor data in RAS message structure.
 */

void ooGkClientFillVendor
   (ooGkClient *pGkClient, H225VendorIdentifier *pVendor )
{
   pVendor->vendor.t35CountryCode = gH323ep.t35CountryCode;
   pVendor->vendor.t35Extension = gH323ep.t35Extension;
   pVendor->vendor.manufacturerCode = gH323ep.manufacturerCode;
   pVendor->enterpriseNumber.numids=0;
   if(gH323ep.productID)
   {
      pVendor->m.productIdPresent = TRUE;
      pVendor->productId.numocts = ASN1MIN(strlen(gH323ep.productID),
                                             sizeof(pVendor->productId.data));
      memcpy(pVendor->productId.data, gH323ep.productID,
                                                  pVendor->productId.numocts);
   }
   if(gH323ep.versionID)
   {
      pVendor->m.versionIdPresent = 1;
      pVendor->versionId.numocts = ASN1MIN(strlen(gH323ep.versionID),
                                             sizeof(pVendor->versionId.data));
      memcpy(pVendor->versionId.data, gH323ep.versionID,
                                                 pVendor->versionId.numocts);
   }
}  


int ooGkClientReceive(ooGkClient *pGkClient)
{
   ASN1OCTET recvBuf[1024];
   int recvLen;
   char remoteHost[32];
   int iFromPort=0;
   OOCTXT *pctxt=NULL;
   H225RasMessage *pRasMsg=NULL;
   int iRet=OO_OK;
  
   pctxt = &pGkClient->msgCtxt;

   recvLen = ooSocketRecvFrom(pGkClient->rasSocket, recvBuf, 1024, remoteHost,
                              32, &iFromPort);
   if(recvLen <0)
   {
      OOTRACEERR1("Error:Failed to receive RAS message\n");
      return OO_FAILED;
   }
   OOTRACEDBGA1("GkClient Received RAS Message\n");
 
   /* Verify the gk */
   if(pGkClient->discoveryComplete)
   {
      if((strncmp(pGkClient->gkRasIP, remoteHost,strlen(pGkClient->gkRasIP)))||
         (pGkClient->gkRasPort!= iFromPort) )
      {
         OOTRACEWARN3("WARN:Ignoring message received from unknown gatekeeper "
                      "%s:%d\n", remoteHost, iFromPort);
         return OO_OK;
      }
   }

   if(ASN_OK != setPERBuffer (pctxt, recvBuf, recvLen, TRUE ) )
   {
      OOTRACEERR1("Error:Failed to set PER buffer for RAS message decoding\n");
      memReset(pctxt);
      pGkClient->state = GkClientFailed;
      return OO_FAILED;
   }     
   pRasMsg = (H225RasMessage*)memAlloc(pctxt, sizeof(H225RasMessage));
   if(!pRasMsg)
   {
      OOTRACEERR1("Error: Failed to allocate memory for RAS message\n");
      memReset(pctxt);
      pGkClient->state = GkClientFailed;
      return OO_FAILED;
   }
#ifndef _COMPACT
   initializePrintHandler(&printHandler, "Received RAS Message");
   /* Add event handler to list */
   setEventHandler (pctxt, &printHandler);
#endif
   if(ASN_OK == asn1PD_H225RasMessage(pctxt, pRasMsg))
   {
#ifndef _COMPACT
      finishPrint();
      removeEventHandler(pctxt);
#endif
      iRet=ooGkClientHandleRASMessage( pGkClient, pRasMsg );
      if(iRet != OO_OK)
      {
         OOTRACEERR1("Error: Failed to handle received RAS message\n");
         //pGkClient->state = GkClientFailed;
      }
      memReset(pctxt);
   }
   else{
      OOTRACEERR1("ERROR:Failed to decode received RAS message- ignoring"
                  "received message.\n");
#ifndef _COMPACT
      removeEventHandler(pctxt);
#endif
      memReset(pctxt);
      return OO_FAILED;
   }
   return iRet;
}




/**
 * Manage incoming RAS message.
 */

int ooGkClientHandleRASMessage(ooGkClient *pGkClient, H225RasMessage *pRasMsg)
{
   int iRet = OO_OK;  
   switch( pRasMsg->t)
   {
   case T_H225RasMessage_gatekeeperConfirm:
      OOTRACEINFO1("Gatekeeper Confirmed (GCF) message received.\n");
      iRet = ooGkClientHandleGatekeeperConfirm(pGkClient,
                                          pRasMsg->u.gatekeeperConfirm);
      break;
   case T_H225RasMessage_gatekeeperReject:
      OOTRACEINFO1("Gatekeeper Reject (GRJ) message received\n");
      iRet = ooGkClientHandleGatekeeperReject(pGkClient,
                                              pRasMsg->u.gatekeeperReject);
      break;
   case T_H225RasMessage_registrationConfirm:  
      OOTRACEINFO1("Registration Confirm (RCF) message received\n");
      iRet = ooGkClientHandleRegistrationConfirm(pGkClient, 
                                              pRasMsg->u.registrationConfirm );
      break;
   case T_H225RasMessage_registrationReject:
      OOTRACEINFO1("Registration Reject (RRJ) message received.\n");
      iRet = ooGkClientHandleRegistrationReject(pGkClient,
                                                pRasMsg->u.registrationReject);
      break;
   case T_H225RasMessage_infoRequest: 
      //ooRasSendIRR( psRasMsg->sMessage.u.infoRequest->requestSeqNum );
      break;
   case T_H225RasMessage_admissionConfirm:
      OOTRACEINFO1("Admission Confirm (ACF) message received\n");
      iRet = ooGkClientHandleAdmissionConfirm(pGkClient,
                                              pRasMsg->u.admissionConfirm);
      break;
   case T_H225RasMessage_unregistrationRequest:
      OOTRACEINFO1("UnRegistration Request (URQ) message received.\n");
      iRet = ooGkClientHandleUnregistrationRequest(pGkClient,
                                            pRasMsg->u.unregistrationRequest);
      break;
   case T_H225RasMessage_unregistrationConfirm:
      OOTRACEINFO1("UnRegistration Confirm (UCF) message received.\n");
      break;
   case T_H225RasMessage_unregistrationReject:
      OOTRACEINFO1("UnRegistration Reject (URJ) message received.\n");
      break;
   case T_H225RasMessage_admissionReject:
      OOTRACEINFO1("Admission Reject (ARJ) message received.\n");
      break;
   case T_H225RasMessage_disengageConfirm:
      iRet = ooGkClientHandleDisengageConfirm(pGkClient,
                                              pRasMsg->u.disengageConfirm);
      break;
   case T_H225RasMessage_disengageReject:
   case T_H225RasMessage_bandwidthConfirm:
   case T_H225RasMessage_bandwidthReject:
   case T_H225RasMessage_locationRequest:
   case T_H225RasMessage_locationConfirm:
   case T_H225RasMessage_locationReject:
   case T_H225RasMessage_infoRequestResponse:
   case T_H225RasMessage_nonStandardMessage:
   case T_H225RasMessage_unknownMessageResponse:
   case T_H225RasMessage_requestInProgress:
   case T_H225RasMessage_resourcesAvailableIndicate:
   case T_H225RasMessage_resourcesAvailableConfirm:
   case T_H225RasMessage_infoRequestAck:
   case T_H225RasMessage_infoRequestNak:
   case T_H225RasMessage_serviceControlIndication:
   case T_H225RasMessage_serviceControlResponse:
   case T_H225RasMessage_admissionConfirmSequence:
   default:
      /* Unhandled RAS message */
      iRet= OO_OK;
   }

   return iRet;
}

#ifndef _COMPACT
void ooGkClientPrintMessage
   (ooGkClient *pGkClient, ASN1OCTET *msg, ASN1UINT len)
{
   OOCTXT ctxt;
   H225RasMessage rasMsg;
   int ret;

   initContext(&ctxt);
   setPERBuffer(&ctxt, msg, len, TRUE);
   initializePrintHandler(&printHandler, "Sending RAS Message");
   setEventHandler(&ctxt, &printHandler);

   ret = asn1PD_H225RasMessage(&ctxt, &rasMsg);
   if(ret != ASN_OK)
   {
      OOTRACEERR1("Error: Failed to decode RAS message\n");
   }
   finishPrint();
   freeContext(&ctxt);
}
#endif
/**
 * Encode and send RAS message.
 */

int ooGkClientSendMsg(ooGkClient *pGkClient, H225RasMessage *pRasMsg)
{
   ASN1OCTET msgBuf[MAXMSGLEN];
   ASN1OCTET *msgPtr=NULL;
   int  iLen;
   OOCTXT *pctxt = &pGkClient->msgCtxt;

   setPERBuffer( pctxt, msgBuf, MAXMSGLEN, TRUE );
   if ( ASN_OK == asn1PE_H225RasMessage(pctxt, pRasMsg) )
   {
      OOTRACEDBGC1("Ras message encoding - successful\n");
   }
   else {
      OOTRACEERR1("Error: RAS message encoding failed\n");
      return OO_FAILED;
   }

   msgPtr = encodeGetMsgPtr( pctxt, &iLen );
   /* If gatekeeper specified or have been discovered */
   if(pGkClient->gkMode == RasUseSpecificGatekeeper ||
      pGkClient->discoveryComplete)
   {
      if(ASN_OK != ooSocketSendTo( pGkClient->rasSocket, msgPtr, iLen,
                                   pGkClient->gkRasIP, pGkClient->gkRasPort))
      {
         OOTRACEERR1("Error sending RAS message\n");
         return OO_FAILED;
      }
   }else if(pGkClient->gkMode == RasDiscoverGatekeeper &&
            !pGkClient->discoveryComplete)
   {
      if(ASN_OK != ooSocketSendTo(pGkClient->rasSocket, msgPtr, iLen,
                                       MULTICAST_GKADDRESS, MULTICAST_GKPORT))
      {
         OOTRACEERR1("Error sending multicast RAS message\n" );
         return OO_FAILED;
      }
   }else {/* should never go here */
      OOTRACEERR1("Error: GkClient in invalid state.\n");
      return OO_FAILED;
   }
#ifndef _COMPACT
   ooGkClientPrintMessage(pGkClient, msgPtr, iLen);
#endif
   return OO_OK;
}



int ooGkClientSendGRQ(ooGkClient *pGkClient)
{
   int iRet;
   H225RasMessage *pRasMsg=NULL;
   H225GatekeeperRequest *pGkReq=NULL;
   H225TransportAddress_ipAddress *pRasAddress;
   OOCTXT *pctxt = &pGkClient->msgCtxt;
   ooGkClientTimerCb *cbData=NULL;

  

   /* Allocate memory for RAS message */
   pRasMsg = (H225RasMessage*)memAlloc(pctxt, sizeof(H225RasMessage));
   if(!pRasMsg)
   {
      OOTRACEERR1("Error: Memory allocation for GRQ RAS message failed\n");
      pGkClient->state = GkClientFailed;
      return OO_FAILED;
   }

   pGkReq = (H225GatekeeperRequest*)memAlloc(pctxt,
                                                sizeof(H225GatekeeperRequest));
   if(!pGkReq)
   {
      OOTRACEERR1("Error:Memory allocation for GRQ failed\n");
      memReset(pctxt);
      pGkClient->state = GkClientFailed;
      return OO_FAILED;
   }
   memset(pGkReq, 0, sizeof(H225GatekeeperRequest));
   pRasMsg->t = T_H225RasMessage_gatekeeperRequest;
   pRasMsg->u.gatekeeperRequest = pGkReq;

   /* Populate message structure */
   pGkReq->requestSeqNum = pGkClient->requestSeqNum++;
   if ( !pGkReq->requestSeqNum )
      pGkReq->requestSeqNum = pGkClient->requestSeqNum++;

   pGkReq->protocolIdentifier = gProtocolID;
   pGkReq->m.nonStandardDataPresent=0;
   pGkReq->rasAddress.t=T_H225TransportAddress_ipAddress; /* IPv4 address */
   pRasAddress = (H225TransportAddress_ipAddress*)memAlloc(pctxt,
                                      sizeof(H225TransportAddress_ipAddress));
   if(!pRasAddress)
   {
      OOTRACEERR1("Error: Memory allocation for Ras Address of GRQ message "
                  "failed\n");
      memReset(&pGkClient->msgCtxt);
      pGkClient->state = GkClientFailed;
      return OO_FAILED;
   }


   ooConvertIpToNwAddr(gH323ep.signallingIP, pRasAddress->ip.data);

   pRasAddress->ip.numocts = 4;
   pRasAddress->port = pGkClient->localRASPort;
   pGkReq->rasAddress.u.ipAddress = pRasAddress;
   pGkReq->endpointType.m.nonStandardDataPresent=0;
   pGkReq->endpointType.m.vendorPresent=1;

   ooGkClientFillVendor(pGkClient, &pGkReq->endpointType.vendor);


   pGkReq->m.endpointAliasPresent=TRUE;
   if(OO_OK != ooPopulateAliasList(&pGkClient->msgCtxt, gH323ep.aliases,
                                      &pGkReq->endpointAlias))
   {
      OOTRACEERR1("Error Failed to fill alias information for GRQ message\n");
      memReset(&pGkClient->msgCtxt);
      pGkClient->state = GkClientFailed;
      return OO_FAILED;
   }
   iRet = ooGkClientSendMsg(pGkClient, pRasMsg);
   if(iRet != OO_OK)
   {
      OOTRACEERR1("Error: Failed to send GRQ message\n");
      memReset(&pGkClient->msgCtxt);
      pGkClient->state = GkClientFailed;
      return OO_FAILED;
   }
   OOTRACEINFO1("Sent GRQ message\n");
   cbData = (ooGkClientTimerCb*) memAlloc
                               (&pGkClient->ctxt, sizeof(ooGkClientTimerCb));
   if(!cbData)
   {
      OOTRACEERR1("Error:Failed to allocate memory to GRQ timer callback\n");
      pGkClient->state = GkClientFailed;
      return OO_FAILED;
   }
   cbData->timerType = OO_GRQ_TIMER;
   cbData->pGkClient = pGkClient;
   if(!ooTimerCreate(&pGkClient->ctxt, &pGkClient->timerList,
                     &ooGkClientGRQTimerExpired, pGkClient->grqTimeout,
                     cbData, FALSE))     
   {
      OOTRACEERR1("Error:Unable to create GRQ timer.\n ");
      memFreePtr(&pGkClient->ctxt, cbData);
      pGkClient->state = GkClientFailed;
      return OO_FAILED;
   }
   return OO_OK;
}

int ooGkClientHandleGatekeeperReject
   (ooGkClient *pGkClient, H225GatekeeperReject *pGatekeeperReject)
{
   unsigned int x=0;
   DListNode *pNode = NULL;
   OOTimer *pTimer = NULL;

   if(pGkClient->gkMode == RasUseSpecificGatekeeper)
   {
      /* delete the corresponding GRQ timer */
      for(x=0; x<pGkClient->timerList.count; x++)
      {
         pNode =  dListFindByIndex(&pGkClient->timerList, x);
         pTimer = (OOTimer*)pNode->data;
         if(((ooGkClientTimerCb*)pTimer->cbData)->timerType & OO_GRQ_TIMER)
         {
            memFreePtr(&pGkClient->ctxt, pTimer->cbData);
            ooTimerDelete(&pGkClient->ctxt, &pGkClient->timerList, pTimer);
            OOTRACEDBGA1("Deleted GRQ Timer.\n");
            break;
         }
      }

      pGkClient->state = GkClientGkErr;
      switch(pGatekeeperReject->rejectReason.t)
      {
      case T_H225GatekeeperRejectReason_resourceUnavailable:
         OOTRACEERR1("Error: Gatekeeper Reject - Resource Unavailable\n");
         break;
      case T_H225GatekeeperRejectReason_terminalExcluded:
         OOTRACEERR1("Error: Gatekeeper Reject - Terminal Excluded\n");
         break;
      case T_H225GatekeeperRejectReason_invalidRevision:
         OOTRACEERR1("Error: Gatekeeper Reject - Invalid Revision\n");
         break;
      case T_H225GatekeeperRejectReason_undefinedReason:
         OOTRACEERR1("Error: Gatekeeper Reject - Undefined Reason\n");
         break;
      case T_H225GatekeeperRejectReason_securityDenial:
         OOTRACEERR1("Error: Gatekeeper Reject - Security Denial\n");
         break;
      case T_H225GatekeeperRejectReason_genericDataReason:
         OOTRACEERR1("Error: Gatekeeper Reject - Generic Data Reason\n");
         break;
      case T_H225GatekeeperRejectReason_neededFeatureNotSupported:
         OOTRACEERR1("Error: Gatekeeper Reject - Needed Feature Not "
                     "Supported\n");
         break;
      case T_H225GatekeeperRejectReason_securityError:
         OOTRACEERR1("Error:Gatekeeper Reject - Security Error\n");
         break;
      default:
         OOTRACEERR1("Error: Gatekeeper Reject - Invalid reason\n");
      }
      return OO_OK;
   }
   OOTRACEDBGB1("Gatekeeper Reject response received for multicast GRQ "
                "request\n");
   return OO_OK;

}

int ooGkClientHandleGatekeeperConfirm
   (ooGkClient *pGkClient, H225GatekeeperConfirm *pGatekeeperConfirm)
{
   int iRet=0;
   unsigned int x=0;
   DListNode *pNode = NULL;
   OOTimer *pTimer = NULL;
   H225TransportAddress_ipAddress *pRasAddress;

   if(pGkClient->discoveryComplete)
   {
      OOTRACEDBGB1("Ignoring GKConfirm as Gatekeeper has been discovered\n");
      return OO_OK;
   }

   if(pGatekeeperConfirm->m.gatekeeperIdentifierPresent)
   {
      pGkClient->gkId.nchars = pGatekeeperConfirm->gatekeeperIdentifier.nchars;
      pGkClient->gkId.data = (ASN116BITCHAR*)memAlloc(&pGkClient->ctxt,
                              sizeof(ASN116BITCHAR)*pGkClient->gkId.nchars);
      if(!pGkClient->gkId.data)
      {
         OOTRACEERR1("Error:Failed to allocate memory for GK ID data\n");
         pGkClient->state = GkClientFailed;
         return OO_FAILED;
      }

      memcpy(pGkClient->gkId.data,
             pGatekeeperConfirm->gatekeeperIdentifier.data,
             sizeof(ASN116BITCHAR)* pGkClient->gkId.nchars);
   }
   else{
      OOTRACEERR1("ERROR:No Gatekeeper ID present in received GKConfirmed "
                  "message\n");
      OOTRACEINFO1("Ignoring message and will retransmit GRQ after timeout\n");
      return OO_FAILED;
   }
  
   /* Extract Gatekeeper's RAS address */
   if(pGatekeeperConfirm->rasAddress.t != T_H225TransportAddress_ipAddress)
   {
      OOTRACEERR1("ERROR:Unsupported RAS address type in received Gk Confirm"
                  " message.\n");
      pGkClient->state = GkClientGkErr;
      return OO_FAILED;
   }
   pRasAddress =   pGatekeeperConfirm->rasAddress.u.ipAddress;
   sprintf(pGkClient->gkRasIP, "%d.%d.%d.%d", pRasAddress->ip.data[0],
                                              pRasAddress->ip.data[1],
                                              pRasAddress->ip.data[2],
                                              pRasAddress->ip.data[3]);
   pGkClient->gkRasPort = pRasAddress->port;
  
   pGkClient->discoveryComplete = TRUE;
   pGkClient->state = GkClientDiscovered;
   OOTRACEINFO1("Gatekeeper Confirmed\n");


   /* Delete the corresponding GRQ timer */
   for(x=0; x<pGkClient->timerList.count; x++)
   {
      pNode =  dListFindByIndex(&pGkClient->timerList, x);
      pTimer = (OOTimer*)pNode->data;
      if(((ooGkClientTimerCb*)pTimer->cbData)->timerType & OO_GRQ_TIMER)
      {
         memFreePtr(&pGkClient->ctxt, pTimer->cbData);
         ooTimerDelete(&pGkClient->ctxt, &pGkClient->timerList, pTimer);
         OOTRACEDBGA1("Deleted GRQ Timer.\n");
         break;
      }
   }

   iRet = ooGkClientSendRRQ(pGkClient, FALSE);
   if(iRet != OO_OK)
   {
      OOTRACEERR1("Error:Failed to send initial RRQ\n");
      return OO_FAILED;
   }
   return OO_OK;
}

/**
 * Send RRQ.
 */

int ooGkClientSendRRQ(ooGkClient *pGkClient, ASN1BOOL keepAlive)
{
   int iRet;
   H225RasMessage *pRasMsg=NULL;
   H225RegistrationRequest *pRegReq=NULL;
   OOCTXT *pctxt=NULL;
   H225TransportAddress *pTransportAddress=NULL;
   H225TransportAddress_ipAddress *pIpAddress=NULL;
   ooGkClientTimerCb *cbData =NULL;

   pctxt = &pGkClient->msgCtxt;

   pRasMsg = (H225RasMessage*)memAlloc(pctxt, sizeof(H225RasMessage));
   if(!pRasMsg)
   {
      OOTRACEERR1("Error: Memory allocation for RRQ RAS message failed\n");
      pGkClient->state = GkClientFailed;
      return OO_FAILED;
   }

   pRegReq = (H225RegistrationRequest*)memAlloc(pctxt,
                                          sizeof(H225RegistrationRequest));
   if(!pRegReq)
   {
      OOTRACEERR1("Error:Memory allocation for RRQ failed\n");
      memReset(pctxt);
      pGkClient->state = GkClientFailed;
      return OO_FAILED;
   }
   memset(pRegReq, 0, sizeof(H225RegistrationRequest));
   pRasMsg->t = T_H225RasMessage_registrationRequest;
   pRasMsg->u.registrationRequest = pRegReq;
  
   pRegReq->protocolIdentifier = gProtocolID;
   pRegReq->m.nonStandardDataPresent=0;
   /* Populate CallSignal Address List*/
   pTransportAddress = (H225TransportAddress*) memAlloc(pctxt,
                                                 sizeof(H225TransportAddress));
   pIpAddress = (H225TransportAddress_ipAddress*) memAlloc(pctxt,
                                       sizeof(H225TransportAddress_ipAddress));
   if(!pTransportAddress || !pIpAddress)
   {
      OOTRACEERR1("Error:Failed to allocate memory for signalling address of "
                  "RRQ message\n");
      memReset(pctxt);
      pGkClient->state = GkClientFailed;
      return OO_FAILED;
   }
   pTransportAddress->t = T_H225TransportAddress_ipAddress;
   pTransportAddress->u.ipAddress = pIpAddress;
   ooConvertIpToNwAddr(gH323ep.signallingIP, pIpAddress->ip.data);
   pIpAddress->ip.numocts = 4;
   pIpAddress->port = gH323ep.listenPort;
  
   dListInit(&pRegReq->callSignalAddress);
   dListAppend(pctxt, &pRegReq->callSignalAddress,
                                       (void*)pTransportAddress);

   /* Populate RAS Address List*/
   pTransportAddress = NULL;
   pIpAddress = NULL;
   pTransportAddress = (H225TransportAddress*) memAlloc(pctxt,
                                                 sizeof(H225TransportAddress));
   pIpAddress = (H225TransportAddress_ipAddress*) memAlloc(pctxt,
                                       sizeof(H225TransportAddress_ipAddress));
   if(!pTransportAddress || !pIpAddress)
   {
      OOTRACEERR1("Error:Failed to allocate memory for RAS address of "
                  "RRQ message\n");
      memReset(pctxt);
      pGkClient->state = GkClientFailed;
      return OO_FAILED;
   }

   pTransportAddress->t = T_H225TransportAddress_ipAddress;
   pTransportAddress->u.ipAddress = pIpAddress;
  
   ooConvertIpToNwAddr(gH323ep.signallingIP, pIpAddress->ip.data);

   pIpAddress->ip.numocts = 4;
   pIpAddress->port = pGkClient->localRASPort;
  
   dListInit(&pRegReq->rasAddress);
   dListAppend(pctxt, &pRegReq->rasAddress,
                                       (void*)pTransportAddress);
  
   pRegReq->terminalType.m.vendorPresent=TRUE;
   ooGkClientFillVendor(pGkClient, &pRegReq->terminalType.vendor );
 
   pRegReq->m.terminalAliasPresent=TRUE;
   if(OO_OK != ooPopulateAliasList(pctxt, gH323ep.aliases,
                                      &pRegReq->terminalAlias))
   {
      OOTRACEERR1("Error filling alias for RRQ\n");
      memReset(pctxt);
      pGkClient->state = GkClientFailed;
      return OO_FAILED;
   }
  
   pRegReq->m.gatekeeperIdentifierPresent=TRUE;
   pRegReq->gatekeeperIdentifier.nchars = pGkClient->gkId.nchars;
   pRegReq->gatekeeperIdentifier.data = (ASN116BITCHAR*)memAlloc
                         (pctxt, pGkClient->gkId.nchars*sizeof(ASN116BITCHAR));
   if(!pRegReq->gatekeeperIdentifier.data)
   {
      OOTRACEERR1("Error: Failed to allocate memory for GKIdentifier in RRQ "
                   "message.\n");
      memReset(pctxt);
      pGkClient->state = GkClientFailed;
      return OO_FAILED;
   }
   memcpy(pRegReq->gatekeeperIdentifier.data, pGkClient->gkId.data,
                                pGkClient->gkId.nchars*sizeof(ASN116BITCHAR));
  
   ooGkClientFillVendor(pGkClient, &pRegReq->endpointVendor);
  
   pRegReq->m.willSupplyUUIEsPresent=TRUE;
   pRegReq->willSupplyUUIEs=FALSE;

   pRegReq->requestSeqNum = pGkClient->requestSeqNum++;
   if(!pRegReq->requestSeqNum)
      pRegReq->requestSeqNum = pGkClient->requestSeqNum++;
  
   pRegReq->discoveryComplete= pGkClient->discoveryComplete;
   pRegReq->m.keepAlivePresent=TRUE;
   pRegReq->keepAlive= keepAlive;
   pRegReq->m.timeToLivePresent = TRUE;
   pRegReq->timeToLive = pGkClient->regTimeout;

   iRet = ooGkClientSendMsg(pGkClient, pRasMsg);
   if(iRet != OO_OK)
   {
      OOTRACEERR1("Error: Failed to send RRQ message\n");
      memReset(pctxt);
      pGkClient->state = GkClientFailed;
      return OO_FAILED;
   }
   OOTRACEINFO1("Sent RRQ message \n");
   /* Start RRQ Timer */
   cbData = (ooGkClientTimerCb*) memAlloc
                            (&pGkClient->ctxt, sizeof(ooGkClientTimerCb));
   if(!cbData)
   {
      OOTRACEERR1("Error:Failed to allocate memory to RRQ timer callback\n");
      pGkClient->state = GkClientFailed;
      return OO_FAILED;
   }
   cbData->timerType = OO_RRQ_TIMER;
   cbData->pGkClient = pGkClient;
   if(!ooTimerCreate(&pGkClient->ctxt, &pGkClient->timerList,
                     &ooGkClientRRQTimerExpired, pGkClient->rrqTimeout,
                     cbData, FALSE))     
   {
      OOTRACEERR1("Error:Unable to create GRQ timer.\n ");
      memFreePtr(&pGkClient->ctxt, cbData);
      pGkClient->state = GkClientFailed;
      return OO_FAILED;
   }
  
   return OO_OK;
}



/**
 * Manage incoming RCF message.
 */

int ooGkClientHandleRegistrationConfirm
   (ooGkClient *pGkClient, H225RegistrationConfirm *pRegistrationConfirm)
{
   int i=0;
   unsigned int x=0;
   OOTimer *pTimer = NULL;
   DListNode *pNode = NULL;
   H225TransportAddress *pCallSigAddr=NULL;
   ooGkClientTimerCb *cbData;
   ASN1UINT regTTL=0;
   /* Extract Endpoint Id */
   pGkClient->endpointId.nchars =
                              pRegistrationConfirm->endpointIdentifier.nchars;
   pGkClient->endpointId.data = (ASN116BITCHAR*)memAlloc(&pGkClient->ctxt,
                          sizeof(ASN116BITCHAR)*pGkClient->endpointId.nchars);
   if(!pGkClient->endpointId.data)
   {
      OOTRACEERR1("Error:Failed to allocate memory for endpoint Id.\n");
      pGkClient->state = GkClientFailed;
      return OO_FAILED;
   }
  
   memcpy(pGkClient->endpointId.data,
          pRegistrationConfirm->endpointIdentifier.data,
          sizeof(ASN116BITCHAR)*pGkClient->endpointId.nchars);
  
   /* Extract CallSignalling Address */
   for(i=0; i<(int)pRegistrationConfirm->callSignalAddress.count; i++)
   {
      pNode = dListFindByIndex(&pRegistrationConfirm->callSignalAddress, i);
      if(!pNode)
      {
         OOTRACEERR1("Error:Invalid Registration confirmed message\n");
         OOTRACEINFO1("Ignoring RCF, will retransmit RRQ after timeout\n");
         return OO_FAILED;
      }
      pCallSigAddr = (H225TransportAddress*)pNode->data;
      if(pCallSigAddr->t != T_H225TransportAddress_ipAddress)
         continue;
      sprintf(pGkClient->gkCallSignallingIP, "%d.%d.%d.%d",
                            pCallSigAddr->u.ipAddress->ip.data[0],
                            pCallSigAddr->u.ipAddress->ip.data[1],
                            pCallSigAddr->u.ipAddress->ip.data[2],
                            pCallSigAddr->u.ipAddress->ip.data[3]);
      pGkClient->gkCallSignallingPort = pCallSigAddr->u.ipAddress->port;
   }
  
   /* Is keepAlive supported */
   if(pRegistrationConfirm->m.timeToLivePresent)
   {
      pGkClient->regTimeout = pRegistrationConfirm->timeToLive;
      OOTRACEINFO2("Gatekeeper supports KeepAlive, Registration TTL is %d\n",
                    pRegistrationConfirm->timeToLive);

      if(pGkClient->regTimeout > DEFAULT_TTL_OFFSET)
         regTTL = pGkClient->regTimeout - DEFAULT_TTL_OFFSET;
      else
         regTTL = pGkClient->regTimeout;

      cbData = (ooGkClientTimerCb*) memAlloc
                                (&pGkClient->ctxt, sizeof(ooGkClientTimerCb));
      if(!cbData)
      {
         OOTRACEERR1("Error:Failed to allocate memory for Regisration timer."
                     "\n");
         pGkClient->state = GkClientFailed;
         return OO_FAILED;
      }
      cbData->timerType = OO_REG_TIMER;
      cbData->pGkClient = pGkClient;
      if(!ooTimerCreate(&pGkClient->ctxt, &pGkClient->timerList,
                     &ooGkClientREGTimerExpired, regTTL,
                     cbData, FALSE))
      {
         OOTRACEERR1("Error:Unable to create REG timer.\n ");
         memFreePtr(&pGkClient->ctxt, cbData);
         pGkClient->state = GkClientFailed;
         return OO_FAILED;
      }   
     
   }else{
      pGkClient->regTimeout = 0;
      OOTRACEINFO1("Gatekeeper does not support KeepAlive.\n");
   }
   /* Extract Pre-Granted ARQ */
   if(pRegistrationConfirm->m.preGrantedARQPresent)
   {
      memcpy(&pGkClient->gkInfo.preGrantedARQ,
             &pRegistrationConfirm->preGrantedARQ,
             sizeof(H225RegistrationConfirm_preGrantedARQ));
   }


   /* First delete the corresponding RRQ timer */
   pNode = NULL;
   for(x=0; x<pGkClient->timerList.count; x++)
   {
      pNode =  dListFindByIndex(&pGkClient->timerList, x);
      pTimer = (OOTimer*)pNode->data;
      if(((ooGkClientTimerCb*)pTimer->cbData)->timerType & OO_RRQ_TIMER)
      {
         memFreePtr(&pGkClient->ctxt, pTimer->cbData);
         ooTimerDelete(&pGkClient->ctxt, &pGkClient->timerList, pTimer);
         OOTRACEDBGA1("Deleted RRQ Timer.\n");
         break;
      }
   }
   pGkClient->state = GkClientRegistered;
   return OO_OK;
}

int ooGkClientHandleRegistrationReject
   (ooGkClient *pGkClient, H225RegistrationReject *pRegistrationReject)
{
   int iRet=0;
   unsigned int x=0;
   DListNode *pNode = NULL;
   OOTimer *pTimer = NULL;
   /* First delete the corresponding RRQ timer */
   for(x=0; x<pGkClient->timerList.count; x++)
   {
      pNode =  dListFindByIndex(&pGkClient->timerList, x);
      pTimer = (OOTimer*)pNode->data;
      if(((ooGkClientTimerCb*)pTimer->cbData)->timerType & OO_RRQ_TIMER)
      {
         memFreePtr(&pGkClient->ctxt, pTimer->cbData);
         ooTimerDelete(&pGkClient->ctxt, &pGkClient->timerList, pTimer);
         OOTRACEDBGA1("Deleted RRQ Timer.\n");
         break;
      }
   }

   switch(pRegistrationReject->rejectReason.t)
   {
   case T_H225RegistrationRejectReason_discoveryRequired:
      OOTRACEINFO1("RRQ Rejected - Discovery Required\n");

      pGkClient->discoveryComplete = FALSE;
      pGkClient->state = GkClientIdle;
      pGkClient->rrqRetries = 0;
      pGkClient->grqRetries = 0;
      if(OO_OK != ooGkClientSendGRQ(pGkClient))
      {
         OOTRACEERR1("Error:Failed to send GRQ message\n");
         return OO_FAILED;
      }
      return OO_OK;
   case T_H225RegistrationRejectReason_invalidRevision:
      OOTRACEERR1("RRQ Rejected - Invalid Revision\n");
      break;
   case T_H225RegistrationRejectReason_invalidCallSignalAddress:
      OOTRACEERR1("RRQ Rejected - Invalid CallSignalAddress\n");
      break;
   case T_H225RegistrationRejectReason_invalidRASAddress:
      OOTRACEERR1("RRQ Rejected - Invalid RAS Address\n");
      break;
   case T_H225RegistrationRejectReason_duplicateAlias:
      OOTRACEERR1("RRQ Rejected - Duplicate Alias\n");
      break;
   case T_H225RegistrationRejectReason_invalidTerminalType:
      OOTRACEERR1("RRQ Rejected - Invalid Terminal Type\n");
      break;
   case T_H225RegistrationRejectReason_undefinedReason:
      OOTRACEERR1("RRQ Rejected - Undefined Reason\n");
      break;
   case T_H225RegistrationRejectReason_transportNotSupported:
      OOTRACEERR1("RRQ Rejected - Transport Not supported\n");
      break;
   case T_H225RegistrationRejectReason_transportQOSNotSupported:
      OOTRACEERR1("RRQ Rejected - Transport QOS Not Supported\n");
      break;
   case T_H225RegistrationRejectReason_resourceUnavailable:
      OOTRACEERR1("RRQ Rejected - Resource Unavailable\n");
      break;
   case T_H225RegistrationRejectReason_invalidAlias:
      OOTRACEERR1("RRQ Rejected - Invalid Alias\n");
      break;
   case T_H225RegistrationRejectReason_securityDenial:
      OOTRACEERR1("RRQ Rejected - Security Denial\n");
      break;
   case T_H225RegistrationRejectReason_fullRegistrationRequired:
      OOTRACEINFO1("RRQ Rejected - Full Registration Required\n");
      pGkClient->state = GkClientDiscovered;
      pGkClient->rrqRetries = 0;
      iRet = ooGkClientSendRRQ(pGkClient, 0); /* No keepAlive */
      if(iRet != OO_OK){
         OOTRACEERR1("\nError: Full Registration transmission failed\n");
         return OO_FAILED;
      }
      return OO_OK;
   case T_H225RegistrationRejectReason_additiveRegistrationNotSupported:
      OOTRACEERR1("RRQ Rejected - Additive Registration Not Supported\n");
      break;
   case T_H225RegistrationRejectReason_invalidTerminalAliases:
      OOTRACEERR1("RRQ Rejected - Invalid Terminal Aliases\n");
      break;
   case T_H225RegistrationRejectReason_genericDataReason:
      OOTRACEERR1("RRQ Rejected - Generic Data Reason\n");
      break;
   case T_H225RegistrationRejectReason_neededFeatureNotSupported:
      OOTRACEERR1("RRQ Rejected - Needed Feature Not Supported\n");
      break;
   case T_H225RegistrationRejectReason_securityError:
      OOTRACEERR1("RRQ Rejected - Security Error\n");
      break;
   default:
      OOTRACEINFO1("RRQ Rejected - Invalid Reason\n");
   }
   pGkClient->state = GkClientGkErr;
   return OO_OK;
}


/*TODO: If the unregister request has list of aliases,
        only those aliases are unregistered. Right now we assume
        complete unregistration and start again.
        We also need to send UnRegistrationConfirm
*/
int ooGkClientHandleUnregistrationRequest
   (ooGkClient *pGkClient, H225UnregistrationRequest * punregistrationRequest)
{
   int iRet=0;
   /* Send a fresh Registration request and if that fails, go back to
      Gatekeeper discovery.
   */
   OOTRACEINFO1("Sending fresh RRQ - as unregistration request received\n");
   pGkClient->rrqRetries = 0;
   pGkClient->state = GkClientDiscovered;

   iRet = ooGkClientSendRRQ(pGkClient, 0);
   if(iRet != OO_OK)
   {
      OOTRACEERR1("Error: Failed to send RRQ message\n");
      return OO_FAILED;
   }
   return OO_OK;
}


int ooGkClientSendAdmissionRequest
   (ooGkClient *pGkClient, ooCallData *call, ASN1BOOL retransmit)
{
   int iRet = 0;
   unsigned int x;
   DListNode *pNode;
   ooGkClientTimerCb *cbData=NULL;
   H225RasMessage *pRasMsg=NULL;
   OOCTXT* pctxt;
   H225AdmissionRequest *pAdmReq=NULL;
   H225TransportAddress_ipAddress *pIpAddressLocal =NULL, *pIpAddressRemote=NULL;
   ooAliases *destAliases = NULL, *srcAliases=NULL;
   RasCallAdmissionInfo *pCallAdmInfo=NULL;
   pctxt = &pGkClient->msgCtxt;

   pRasMsg = (H225RasMessage*)memAlloc(pctxt, sizeof(H225RasMessage));
   if(!pRasMsg)
   {
      OOTRACEERR1("Error: Memory allocation for ARQ RAS message failed\n");
      pGkClient->state = GkClientFailed;
      return OO_FAILED;
   }
   pRasMsg->t = T_H225RasMessage_admissionRequest;
   pAdmReq = (H225AdmissionRequest*) memAlloc(pctxt,
                                                 sizeof(H225AdmissionRequest));
   if(!pAdmReq)
   {
      OOTRACEERR1("Error: Failed to allocate memory for Admission Request.\n");
      memReset(pctxt);
      pGkClient->state = GkClientFailed;
      return OO_FAILED;
   }
   memset(pAdmReq, 0, sizeof(H225AdmissionRequest));
   pRasMsg->u.admissionRequest = pAdmReq;
  
   /* Populate call signalling addresses */
   pIpAddressLocal = (H225TransportAddress_ipAddress*)memAlloc(pctxt,
                                     sizeof(H225TransportAddress_ipAddress));
   if(strlen(call->remoteIP))
      pIpAddressRemote = (H225TransportAddress_ipAddress*)memAlloc(pctxt,
                                      sizeof(H225TransportAddress_ipAddress));

   if(!pIpAddressLocal || (strlen(call->remoteIP) && (!pIpAddressRemote)))
   {
      OOTRACEERR1("Error:Failed to allocate memory for Call Signalling "
                  "Addresses of ARQ message\n");
      memReset(pctxt);
      pGkClient->state = GkClientFailed;
      return OO_FAILED;
   }
   ooConvertIpToNwAddr(gH323ep.signallingIP, pIpAddressLocal->ip.data);

   pIpAddressLocal->ip.numocts = 4;
   pIpAddressLocal->port = gH323ep.listenPort;

   if(strlen(call->remoteIP))
   {
      ooConvertIpToNwAddr(call->remoteIP, pIpAddressRemote->ip.data);
      pIpAddressRemote->ip.numocts = 4;
      pIpAddressRemote->port = call->remotePort;
   }

   if(!strcmp(call->callType, "incoming"))
   {
      pAdmReq->m.destCallSignalAddressPresent = TRUE;
      pAdmReq->destCallSignalAddress.t = T_H225TransportAddress_ipAddress;
      pAdmReq->destCallSignalAddress.u.ipAddress = pIpAddressLocal;
      if(strlen(call->remoteIP))
      {
         pAdmReq->m.srcCallSignalAddressPresent = TRUE;
         pAdmReq->srcCallSignalAddress.t = T_H225TransportAddress_ipAddress;
         pAdmReq->srcCallSignalAddress.u.ipAddress = pIpAddressRemote;
      }
   }else {
      pAdmReq->m.srcCallSignalAddressPresent = TRUE;
      pAdmReq->srcCallSignalAddress.t = T_H225TransportAddress_ipAddress;
      pAdmReq->srcCallSignalAddress.u.ipAddress = pIpAddressLocal;
      if(strlen(call->remoteIP))
      {
         pAdmReq->m.destCallSignalAddressPresent = TRUE;
         pAdmReq->destCallSignalAddress.t = T_H225TransportAddress_ipAddress;
         pAdmReq->destCallSignalAddress.u.ipAddress = pIpAddressRemote;
      }
   }

   /* Populate seq number */
   pAdmReq->requestSeqNum = pGkClient->requestSeqNum++;
   if(!pAdmReq->requestSeqNum)
      pAdmReq->requestSeqNum = pGkClient->requestSeqNum++;

   /* Populate call type - For now only PointToPoint supported*/
   pAdmReq->callType.t = T_H225CallType_pointToPoint;
  
   /* Add call model to message*/
   pAdmReq->m.callModelPresent = 1;
   if(OO_TESTFLAG(call->flags, OO_M_GKROUTED))
      pAdmReq->callModel.t = T_H225CallModel_gatekeeperRouted;
   else
      pAdmReq->callModel.t = T_H225CallModel_direct;

   /* Populate Endpoint Identifier */
   pAdmReq->endpointIdentifier.nchars = pGkClient->endpointId.nchars;
   pAdmReq->endpointIdentifier.data = (ASN116BITCHAR*)memAlloc(pctxt,
                           sizeof(ASN116BITCHAR)*pGkClient->endpointId.nchars);
   if(!pAdmReq->endpointIdentifier.data)
   {
      OOTRACEERR1("Error: Failed to allocate memory for EndPoint Id in ARQ "
                  "message.\n");
      memReset(pctxt);
      pGkClient->state = GkClientFailed;
      return OO_FAILED;
   }
   memcpy((void*)pAdmReq->endpointIdentifier.data,
          (void*)pGkClient->endpointId.data,
          sizeof(ASN116BITCHAR)*pGkClient->endpointId.nchars);

   /* Get Destination And source aliases for call -  */
   if(!strcmp(call->callType, "incoming"))
   {
      if(call->ourAliases)
         destAliases = call->ourAliases;
      else
         destAliases = gH323ep.aliases;

      srcAliases = call->remoteAliases;
   }else {
      if(call->ourAliases)
         srcAliases = call->ourAliases;
      else
         srcAliases = gH323ep.aliases;

      destAliases = call->remoteAliases;
   }

   /* Populate destination info */
   if(destAliases)
   {
      pAdmReq->m.destinationInfoPresent = 1;
      if(OO_OK != ooPopulateAliasList(&pGkClient->msgCtxt, destAliases,
                                      &pAdmReq->destinationInfo))
      {
        OOTRACEERR1("Error:Failed to populate destination aliases - "
                    "ARQ message\n");
         pGkClient->state = GkClientFailed;
         memReset(pctxt);
         return OO_FAILED;
      }
   }

   /* Populate Source Info */
   if(srcAliases)
   {
      iRet = ooPopulateAliasList(&pGkClient->msgCtxt, srcAliases,
                                                          &pAdmReq->srcInfo);
      if(OO_OK != iRet)
      {
         OOTRACEERR1("Error:Failed to populate source aliases -ARQ message\n");
         memReset(pctxt);
         pGkClient->state = GkClientFailed;     
         return OO_FAILED;
      }
   }
  
   /* Populate bandwidth*/
   pAdmReq->bandWidth = DEFAULT_BW_REQUEST;
   /* Populate call Reference */
   pAdmReq->callReferenceValue = call->callReference;
  
   /* populate conferenceID */
   memcpy((void*)&pAdmReq->conferenceID, (void*)&call->confIdentifier,
                                         sizeof(H225ConferenceIdentifier));
   /*populate answerCall */
   if(!strcmp(call->callType, "incoming"))
      pAdmReq->answerCall = TRUE;
   else
      pAdmReq->answerCall = FALSE;

   /* Populate CanMapAlias */
   pAdmReq->m.canMapAliasPresent = TRUE;
   pAdmReq->canMapAlias = FALSE;

   /* Populate call identifier */
   pAdmReq->m.callIdentifierPresent = TRUE;
   memcpy((void*)&pAdmReq->callIdentifier, (void*)&call->callIdentifier,
                                             sizeof(H225CallIdentifier));

   /* Populate Gatekeeper Id */
   pAdmReq->m.gatekeeperIdentifierPresent = TRUE;
   pAdmReq->gatekeeperIdentifier.nchars = pGkClient->gkId.nchars;
   pAdmReq->gatekeeperIdentifier.data = (ASN116BITCHAR*)memAlloc(pctxt,
                                 sizeof(ASN116BITCHAR)*pGkClient->gkId.nchars);
   if(!pAdmReq->gatekeeperIdentifier.data)
   {
      OOTRACEERR1("Error:Failed to allocate memory for GKID of ARQ message\n");
      memReset(pctxt);
      pGkClient->state = GkClientFailed;
      return OO_FAILED;
   }
   memcpy((void*)pAdmReq->gatekeeperIdentifier.data,
          (void*)pGkClient->gkId.data,
          sizeof(ASN116BITCHAR)*pGkClient->gkId.nchars);

   pAdmReq->m.willSupplyUUIEsPresent = 1;
   pAdmReq->willSupplyUUIEs = FALSE;

   /* Create RasCallAdmissionInfo */
   if(!retransmit)
   {
      pCallAdmInfo = (RasCallAdmissionInfo*)memAlloc(&pGkClient->ctxt,
                                                sizeof(RasCallAdmissionInfo));
      if(!pCallAdmInfo)
      {
         OOTRACEERR1("Error: Failed to allocate memory for new CallAdmission"
                  " Info entry\n");
         memReset(pctxt);
         pGkClient->state = GkClientFailed;
         return OO_FAILED;
      }

      pCallAdmInfo->call = call;
      pCallAdmInfo->retries = 0;
      pCallAdmInfo->requestSeqNum = pAdmReq->requestSeqNum;
      dListAppend(&pGkClient->ctxt, &pGkClient->callsPendingList,pCallAdmInfo);
   }else{
      for(x=0; x<pGkClient->callsPendingList.count; x++)
      {
         pNode = dListFindByIndex(&pGkClient->callsPendingList, x);
         pCallAdmInfo = (RasCallAdmissionInfo*)pNode->data;
         if(pCallAdmInfo->call->callReference == call->callReference)
         {
            pCallAdmInfo->requestSeqNum = pAdmReq->requestSeqNum;
            break;
         }
      }
   }
  
   iRet = ooGkClientSendMsg(pGkClient, pRasMsg);
   if(iRet != OO_OK)
   {
      OOTRACEERR1("Error:Failed to send AdmissionRequest message\n");
      memReset(pctxt);
      pGkClient->state = GkClientFailed;
      return OO_FAILED;
   }
   OOTRACEINFO3("Admission Request message sent for (%s, %s)\n",
                 call->callType, call->callToken);
   memReset(pctxt);
   
   /* Add ARQ timer */
   cbData = (ooGkClientTimerCb*) memAlloc
                               (&pGkClient->ctxt, sizeof(ooGkClientTimerCb));
   if(!cbData)
   {
      OOTRACEERR1("Error:Failed to allocate memory for Regisration timer."
                  "\n");
      pGkClient->state = GkClientFailed;
      return OO_FAILED;
   }
   cbData->timerType = OO_ARQ_TIMER;
   cbData->pGkClient = pGkClient;
   cbData->pAdmInfo =  pCallAdmInfo;
   if(!ooTimerCreate(&pGkClient->ctxt, &pGkClient->timerList,
                  &ooGkClientARQTimerExpired, pGkClient->arqTimeout,
                  cbData, FALSE))
   {
      OOTRACEERR1("Error:Unable to create ARQ timer.\n ");
      memFreePtr(&pGkClient->ctxt, cbData);
      pGkClient->state = GkClientFailed;
      return OO_FAILED;
   }   
  
   return OO_OK;
}

/**
 * Manage incoming ACF message.
 */

int ooGkClientHandleAdmissionConfirm
   (ooGkClient *pGkClient, H225AdmissionConfirm *pAdmissionConfirm)
{
   RasCallAdmissionInfo* pCallAdmInfo=NULL;
   unsigned int x, y;
   DListNode *pNode, *pNode1=NULL;
   H225TransportAddress_ipAddress * ipAddress=NULL;
   OOTimer *pTimer = NULL;
   char ip[20];

   /* Search call in pending calls list */
   for(x=0 ; x<pGkClient->callsPendingList.count; x++)
   {
      pNode = dListFindByIndex(&pGkClient->callsPendingList, x);
      pCallAdmInfo = (RasCallAdmissionInfo*) pNode->data;
      if(pCallAdmInfo->requestSeqNum == pAdmissionConfirm->requestSeqNum)
      {
         OOTRACEDBGC3("Found Pending call(%s, %s)\n",
                      pCallAdmInfo->call->callType,
                      pCallAdmInfo->call->callToken);
         /* Populate Remote IP */
         if(pAdmissionConfirm->destCallSignalAddress.t !=
                                      T_H225TransportAddress_ipAddress)
         {
            OOTRACEERR1("Error:Destination Call Signal Address provided by"
                        "Gatekeeper is not an IPv4 address\n");
            OOTRACEINFO1("Ignoring ACF, will wait for timeout and retransmit "
                         "ARQ\n");
            return OO_FAILED;
         }
         ipAddress = pAdmissionConfirm->destCallSignalAddress.u.ipAddress;
        
         sprintf(ip, "%d.%d.%d.%d", ipAddress->ip.data[0],
                                    ipAddress->ip.data[1],
                                    ipAddress->ip.data[2],
                                    ipAddress->ip.data[3]);
         if(strcmp(ip, "0.0.0.0"))
            strcpy(pCallAdmInfo->call->remoteIP, ip);
         pCallAdmInfo->call->remotePort = ipAddress->port;
         /* Update call model */
         if(pAdmissionConfirm->callModel.t == T_H225CallModel_direct)
         {
            if(OO_TESTFLAG(pCallAdmInfo->call->flags, OO_M_GKROUTED))
            {
               OOTRACEINFO3("Gatekeeper changed call model from GkRouted to "
                            "direct. (%s, %s)\n", pCallAdmInfo->call->callType,
                            pCallAdmInfo->call->callToken);
               OO_CLRFLAG(pCallAdmInfo->call->flags, OO_M_GKROUTED);
            }
         }
        
         if(pAdmissionConfirm->callModel.t == T_H225CallModel_gatekeeperRouted)
         {
            if(!OO_TESTFLAG(pCallAdmInfo->call->flags, OO_M_GKROUTED))
            {
               OOTRACEINFO3("Gatekeeper changed call model from direct to "
                            "GkRouted. (%s, %s)\n",
                            pCallAdmInfo->call->callType,
                            pCallAdmInfo->call->callToken);
               OO_SETFLAG(pCallAdmInfo->call->flags, OO_M_GKROUTED);
            }
         }

         /* Delete ARQ timer */
         for(y=0; y<pGkClient->timerList.count; y++)
         {
            pNode1 =  dListFindByIndex(&pGkClient->timerList, y);
            pTimer = (OOTimer*)pNode1->data;
            if(((ooGkClientTimerCb*)pTimer->cbData)->timerType & OO_ARQ_TIMER)
            {
               if(((ooGkClientTimerCb*)pTimer->cbData)->pAdmInfo ==
                                                                 pCallAdmInfo)
               {
                  memFreePtr(&pGkClient->ctxt, pTimer->cbData);
                  ooTimerDelete(&pGkClient->ctxt, &pGkClient->timerList,
                                                                       pTimer);
                  OOTRACEDBGA1("Deleted ARQ Timer.\n");
                  break;
               }
            }
         }      
         OOTRACEINFO3("Admission Confirm message received for (%s, %s)\n",
                       pCallAdmInfo->call->callType,
                       pCallAdmInfo->call->callToken); 
         ooH323CallAdmitted( pCallAdmInfo->call);
         dListRemove(&pGkClient->callsPendingList, pNode);
         dListAppend(&pGkClient->ctxt, &pGkClient->callsAdmittedList,
                                                        pNode->data);
         memFreePtr(&pGkClient->ctxt, pNode);
         return OO_OK;
         break;
      }
      else
      {
         pNode = pNode->next;
      }
   }
   OOTRACEERR1("Error: Failed to process ACF as there is no corresponding "
               "pending call\n");
   return OO_OK;
}



/**
 * This function is invoked to request call disengage to gatekeeper.
 *
 * @param   szCallToken    Call token.    
 *
 * @return  Completion status - 0 on success, -1 on failure
 */

int ooGkClientSendDisengageRequest(ooGkClient *pGkClient, ooCallData *call)
{
   int iRet = 0;  
   unsigned int x;
   H225RasMessage *pRasMsg=NULL;
   OOCTXT *pctxt = NULL;
   DListNode *pNode = NULL;
   H225DisengageRequest * pDRQ = NULL;
   RasCallAdmissionInfo* pCallAdmInfo=NULL;
   pctxt = &pGkClient->msgCtxt;

   OOTRACEINFO3("Sending disengage Request for  call. (%s, %s)\n",
                 call->callType, call->callToken);

   pRasMsg = (H225RasMessage*)memAlloc(pctxt, sizeof(H225RasMessage));
   if(!pRasMsg)
   {
      OOTRACEERR1("Error: Memory allocation for DRQ RAS message failed\n");
      pGkClient->state = GkClientFailed;
      return OO_FAILED;
   }

   pRasMsg->t = T_H225RasMessage_disengageRequest;
   pDRQ = (H225DisengageRequest*) memAlloc(pctxt,
                                               sizeof(H225DisengageRequest));
   if(!pDRQ)
   {
      OOTRACEERR1("Error: Failed to allocate memory for DRQ message\n");
      memReset(pctxt);
      pGkClient->state = GkClientFailed;
      return OO_FAILED;
   }

   memset(pDRQ, 0, sizeof(H225DisengageRequest));
   pRasMsg->u.disengageRequest = pDRQ;
  
   pDRQ->requestSeqNum = pGkClient->requestSeqNum++;
   if(!pDRQ->requestSeqNum )
      pDRQ->requestSeqNum = pGkClient->requestSeqNum++;
  
  
   pDRQ->endpointIdentifier.nchars = pGkClient->endpointId.nchars;
   pDRQ->endpointIdentifier.data = (ASN116BITCHAR*)memAlloc(pctxt,
                           sizeof(ASN116BITCHAR)*pGkClient->endpointId.nchars);
   if(!pDRQ->endpointIdentifier.data)
   {
      OOTRACEERR1("Error: Failed to allocate memory for EndPoint Id in DRQ "
                  "message.\n");
      memReset(pctxt);
      pGkClient->state = GkClientFailed;
      return OO_FAILED;
   }
   memcpy((void*)pDRQ->endpointIdentifier.data,
                 (void*)pGkClient->endpointId.data,
                 sizeof(ASN116BITCHAR)*pGkClient->endpointId.nchars);

   memcpy((void*)&pDRQ->conferenceID, (void*)&call->confIdentifier,
                                         sizeof(H225ConferenceIdentifier));

   pDRQ->callReferenceValue = call->callReference;
  
   pDRQ->disengageReason.t = T_H225DisengageReason_normalDrop;

   pDRQ->m.answeredCallPresent = 1;
   if(!strcmp(call->callType, "incoming"))
      pDRQ->answeredCall = 1;
   else
      pDRQ->answeredCall = 0;

   pDRQ->m.callIdentifierPresent = 1;
   memcpy((void*)&pDRQ->callIdentifier, (void*)&call->callIdentifier,
                                             sizeof(H225CallIdentifier));
   pDRQ->m.gatekeeperIdentifierPresent = 1;
   pDRQ->gatekeeperIdentifier.nchars = pGkClient->gkId.nchars;
   pDRQ->gatekeeperIdentifier.data = (ASN116BITCHAR*)memAlloc
                       (pctxt, pGkClient->gkId.nchars*sizeof(ASN116BITCHAR));
   if(!pDRQ->gatekeeperIdentifier.data)
   {
      OOTRACEERR1("Error:Failed to allocate memory for GKId in DRQ.\n");
      memReset(pctxt);
      pGkClient->state = GkClientFailed;
      return OO_FAILED;
   }
   memcpy(pDRQ->gatekeeperIdentifier.data, pGkClient->gkId.data,
                                pGkClient->gkId.nchars*sizeof(ASN116BITCHAR));

   pDRQ->m.terminationCausePresent = 1;
   pDRQ->terminationCause.t = T_H225CallTerminationCause_releaseCompleteCauseIE;
   pDRQ->terminationCause.u.releaseCompleteCauseIE =
      (H225CallTerminationCause_releaseCompleteCauseIE*)memAlloc(pctxt,
      sizeof(H225CallTerminationCause_releaseCompleteCauseIE));
   if(!pDRQ->terminationCause.u.releaseCompleteCauseIE)
   {
      OOTRACEERR1("Error: Failed to allocate memory for cause ie in DRQ.\n");
      memReset(pctxt);
      pGkClient->state = GkClientFailed;
      return OO_FAILED;
   }
   pDRQ->terminationCause.u.releaseCompleteCauseIE->numocts =
                                                         strlen("Call Ended");
   strcpy(pDRQ->terminationCause.u.releaseCompleteCauseIE->data, "Call Ended");

   iRet = ooGkClientSendMsg(pGkClient, pRasMsg);
   if(iRet != OO_OK)
   {
      OOTRACEERR1("Error: Failed to send DRQ message\n");
      pGkClient->state = GkClientFailed;
   }
  


   /* Search call in admitted calls list */
   for(x=0 ; x<pGkClient->callsAdmittedList.count ; x++)
   {
      pNode = (DListNode*)dListFindByIndex(&pGkClient->callsAdmittedList, x);
      pCallAdmInfo = (RasCallAdmissionInfo*) pNode->data;
      if(pCallAdmInfo->call->callReference == call->callReference)
      {
         dListRemove( &pGkClient->callsAdmittedList, pNode);
         memFreePtr(&pGkClient->ctxt, pNode->data);
         memFreePtr(&pGkClient->ctxt, pNode);
         break;
      }
   }
   return iRet;
}    

int ooGkClientHandleDisengageConfirm
   (ooGkClient *pGkClient, H225DisengageConfirm *pDCF)
{
   OOTRACEINFO1("Received disengage confirm\n");
   return OO_OK;
}

int ooGkClientRRQTimerExpired(void*pdata)
{
   int ret=0;
   ooGkClientTimerCb *cbData = (ooGkClientTimerCb*)pdata;
   ooGkClient *pGkClient = cbData->pGkClient;
   OOTRACEDBGA1("Gatekeeper client RRQ timer expired.\n");
  
   if(pGkClient->rrqRetries < OO_MAX_RRQ_RETRIES)
   {
      ret = ooGkClientSendRRQ(pGkClient, 0);     
      if(ret != OO_OK)
      {
         OOTRACEERR1("Error:Failed to send RRQ message\n");
        
         return OO_FAILED;
      }
      pGkClient->rrqRetries++;
      memFreePtr(&pGkClient->ctxt, cbData);
      return OO_OK;
   }
   memFreePtr(&pGkClient->ctxt, cbData);
   OOTRACEERR1("Error:Failed to register with gatekeeper\n");
   pGkClient->state = GkClientGkErr;
   return OO_FAILED;
}

int ooGkClientGRQTimerExpired(void* pdata)
{
   int ret=0;
   ooGkClientTimerCb *cbData = (ooGkClientTimerCb*)pdata;
   ooGkClient *pGkClient = cbData->pGkClient;

   OOTRACEDBGA1("Gatekeeper client GRQ timer expired.\n");

   memFreePtr(&pGkClient->ctxt, cbData);  

   if(pGkClient->grqRetries < OO_MAX_GRQ_RETRIES)
   {
      ret = ooGkClientSendGRQ(pGkClient);     
      if(ret != OO_OK)
      {
         OOTRACEERR1("Error:Failed to send GRQ message\n");
         pGkClient->state = GkClientFailed;
         return OO_FAILED;
      }
      pGkClient->grqRetries++;
      return OO_OK;
   }

   OOTRACEERR1("Error:Gatekeeper could not be found\n");
   pGkClient->state = GkClientGkErr;
   return OO_FAILED;
}
  
int ooGkClientREGTimerExpired(void *pdata)
{
   int ret=0;
   ooGkClientTimerCb *cbData = (ooGkClientTimerCb*)pdata;
   ooGkClient *pGkClient = cbData->pGkClient;
   OOTRACEDBGA1("Gatekeeper client additive registration timer expired\n");
   memFreePtr(&pGkClient->ctxt, cbData);  
   ret = ooGkClientSendRRQ(pGkClient, TRUE);     
   if(ret != OO_OK)
   {
      OOTRACEERR1("Error:Failed to send Additive RRQ message\n");
      pGkClient->state = GkClientFailed;
      return OO_FAILED;
   }
   return OO_OK;
}

int ooGkClientARQTimerExpired(void* pdata)
{
   int ret=0;
   ooGkClientTimerCb *cbData = (ooGkClientTimerCb*)pdata;
   ooGkClient *pGkClient = cbData->pGkClient;
   RasCallAdmissionInfo *pAdmInfo = cbData->pAdmInfo;

   OOTRACEDBGA1("Gatekeeper client ARQ timer expired.\n");
   memFreePtr(&pGkClient->ctxt, cbData);  

   if(pAdmInfo->retries < OO_MAX_ARQ_RETRIES)
   {
      ret = ooGkClientSendAdmissionRequest(pGkClient, pAdmInfo->call, TRUE);     
      if(ret != OO_OK)
      {
         OOTRACEERR1("Error:Failed to send ARQ message\n");
         return OO_FAILED;
      }
      pAdmInfo->retries++;
      return OO_OK;
   }

   OOTRACEERR1("Error:Gatekeeper not responding to ARQ\n");
   pGkClient->state = GkClientGkErr;
   return OO_FAILED;
}

int ooGkClientCleanCall(ooGkClient *pGkClient, ooCallData *call)
{
   unsigned int x=0;
   DListNode *pNode=NULL;
   OOTimer *pTimer;
   ooGkClientTimerCb *cbData=NULL;
   RasCallAdmissionInfo *pAdmInfo = NULL;


   for(x=0; x<pGkClient->callsAdmittedList.count; x++)
   {
      pNode = dListFindByIndex(&pGkClient->callsAdmittedList, x);
      pAdmInfo = (RasCallAdmissionInfo*)pNode->data;
      if(pAdmInfo->call->callReference == call->callReference)
      {
         dListRemove(&pGkClient->callsAdmittedList, pNode);
         memFreePtr(&pGkClient->ctxt, pAdmInfo);
         memFreePtr(&pGkClient->ctxt, pNode);
         return OO_OK;
      }
   }


   for(x=0; x<pGkClient->timerList.count; x++)
   {
      pNode = dListFindByIndex(&pGkClient->timerList, x);
      pTimer = (OOTimer*)pNode->data;
      cbData = (ooGkClientTimerCb*)pTimer->cbData;
      if(cbData->timerType & OO_ARQ_TIMER &&
         cbData->pAdmInfo->call->callReference == call->callReference)
      {
         memFreePtr(&pGkClient->ctxt, pTimer->cbData);
         ooTimerDelete(&pGkClient->ctxt, &pGkClient->timerList, pTimer);
         break;
      }
   }

   for(x=0; x<pGkClient->callsPendingList.count; x++)
   {
      pNode = dListFindByIndex(&pGkClient->callsPendingList, x);
      pAdmInfo = (RasCallAdmissionInfo*)pNode->data;
      if(pAdmInfo->call->callReference == call->callReference)
      {
         dListRemove(&pGkClient->callsPendingList, pNode);
         memFreePtr(&pGkClient->ctxt, pAdmInfo);
         memFreePtr(&pGkClient->ctxt, pNode);
         return OO_OK;
      }
   }

   return OO_OK;
}

/*
 * TODO: In case of GkErr, if GkMode is DiscoverGatekeeper,
 *       need to cleanup gkrouted calls, and discover another
 *       gatekeeper.
 * Note: This function returns OO_FAILED, when we can not recover from
 *       the failure.
 */
int ooGkClientHandleClientOrGkFailure(ooGkClient *pGkClient)
{
   if(pGkClient->state == GkClientFailed)
   {
      OOTRACEERR1("Error: Internal Failure in GkClient. Closing "
                  "GkClient\n");
      ooGkClientDestroy();
      return OO_FAILED;
   }else if(pGkClient->state == GkClientGkErr)
   {
      OOTRACEERR1("Error: Gatekeeper error. Either Gk not responding or "
                  "Gk sending invalid messages\n");
      if(pGkClient->gkMode == RasUseSpecificGatekeeper)
      {
         OOTRACEERR1("Error: Gatekeeper error detected. Closing GkClient as "
                     "Gk mode is UseSpecifcGatekeeper\n");
         ooGkClientDestroy();
         return OO_FAILED;
      }else{
        OOTRACEERR1("Error: Gatekeeper error detected. Closing GkClient. NEED"
                    " to implement recovery by rediscovering another gk\n");
        ooGkClientDestroy();
        return OO_FAILED;
      }
   }

   return OO_FAILED;
}

