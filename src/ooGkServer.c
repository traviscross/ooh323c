/*
 * Copyright (C) 2016 by Seven Du
 * Copyright (C) 2009 by Page Iberica, S.A.
 * Copyright (C) 2009 by Objective Systems, Inc.
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
 * @file ooGkServer.c
 * This file contains functions to support RAS protocol.
 *
 */
#include "ooGkServer.h"
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
#include "ooh323ep.h"
#include "ooTimer.h"
#include "ooSocket.h"
#include "ooUtils.h"

/** Global endpoint structure */
extern OOH323EndPoint gH323ep;

static ASN1OBJID gProtocolID = {
   6, { 0, 0, 8, 2250, 0, 4 }
};

typedef struct gkServerGlobals_s {
	ooGkServer *pGkServer;
} gkServerGlobals;

static gkServerGlobals globals;

ooGkServer *ooGkServerInit(char *szGkAddr, int iGkPort)
{
   ooGkServer *pGkServer = NULL;
   OOInterface *cur = NULL;
   pGkServer = (ooGkServer *)
                         memAlloc(&gH323ep.ctxt, sizeof(ooGkServer));
   if(!pGkServer)
   {
      OOTRACEERR1("Error: Failed to allocate memory to Gatekeeper Server.\n");
      return NULL;
   }

   memset(pGkServer, 0, sizeof(ooGkServer));
   memset(&globals, 0, sizeof(gkServerGlobals));
   globals.pGkServer = pGkServer;
   initContext(&(pGkServer->ctxt));
   initContext(&(pGkServer->msgCtxt));
   pGkServer->rrqRetries = 0;
   pGkServer->grqRetries = 0;

   strcpy(pGkServer->localRASIP, szGkAddr);
   pGkServer->localRASPort = iGkPort;

#ifndef _WIN32
   if(!strcmp(pGkServer->localRASIP, "0.0.0.0") ||
      !strcmp(pGkServer->localRASIP, "127.0.0.1"))
   {
      if(!gH323ep.ifList)
      {
         if(ooSocketGetInterfaceList(&gH323ep.ctxt, &gH323ep.ifList)!= ASN_OK)
         {
            OOTRACEERR1("Error:Failed to retrieve interface addresses\n");
            return NULL;
         }
      }
      for(cur = gH323ep.ifList; cur; cur = cur->next)
      {
         if(!strcmp(cur->name, "lo") || !strcmp(cur->addr, "127.0.0.1"))
            continue;
         break;
      }
      if(cur)
      {
         OOTRACEINFO2("Using local RAS Ip address %s\n", cur->addr);
         strcpy(pGkServer->localRASIP, cur->addr);
      }
      else{
         OOTRACEERR1("Error:Failed to assign a local RAS IP address\n");
         return NULL;
      }
   }
#endif

   /* Create default parameter set */
   dListInit(&pGkServer->callsPendingList);
   dListInit(&pGkServer->callsAdmittedList);
   dListInit(&pGkServer->timerList);
   // pGkServer->state = GkServerIdle;
   return pGkServer;
}


int ooGkServerSetCallbacks
   (ooGkServer *pGkServer, OOGKSERVERCALLBACKS callbacks)
{
   pGkServer->callbacks.onReceivedRegistrationRequest =
                                      callbacks.onReceivedRegistrationRequest;
   pGkServer->callbacks.onReceivedUnregistrationRequest =
                                     callbacks.onReceivedUnregistrationRequest;
   return OO_OK;
}

int ooGkServerReInit(ooGkServer *pGkServer)
{

   ooGkServerCloseChannel(pGkServer);
   pGkServer->gkRasIP[0]='\0';
   pGkServer->gkCallSignallingIP[0]='\0';
   pGkServer->gkRasPort = 0;
   pGkServer->gkCallSignallingPort = 0;
   pGkServer->rrqRetries = 0;
   pGkServer->grqRetries = 0;
   pGkServer->requestSeqNum = 0;

   dListFreeAll(&pGkServer->ctxt, &pGkServer->callsPendingList);
   dListFreeAll(&pGkServer->ctxt, &pGkServer->callsAdmittedList);
   dListFreeAll(&pGkServer->ctxt, &pGkServer->timerList);
   // pGkServer->state = GkServerIdle;
   return OO_OK;
}

void ooGkServerPrintConfig(ooGkServer *pGkServer)
{
   OOTRACEINFO1("Gatekeeper Client Configuration:\n");
}

int ooGkServerDestroy(void)
{
   if(globals.pGkServer)
   {
      OOTRACEINFO1("Destroying Gatekeeper Server\n");
      ooGkServerCloseChannel(globals.pGkServer);
      freeContext(&globals.pGkServer->msgCtxt);
      freeContext(&globals.pGkServer->ctxt);
      memFreePtr(&gH323ep.ctxt, globals.pGkServer);
      globals.pGkServer = NULL;
   }
   return OO_OK;
}

int ooGkServerStart(ooGkServer *pGkServer)
{
   int iRet=0;
   iRet = ooGkServerCreateChannel(pGkServer);

   if(iRet != OO_OK)
   {
      OOTRACEERR1("Error: GkServer Channel Creation failed\n");
      return OO_FAILED;
   }

   return OO_OK;
}

int ooGkServerSetGkId (ooGkServer *pGkServer, const char* id)
{
   size_t i, idlen = strlen (id);

   if (0 == pGkServer) {
      OOTRACEERR1 ("ERROR: gatekeeper client not initialized in endpoint\n");
      return OO_FAILED;
   }
   if (0 != globals.pGkServer->gkId.data) {
      memFreePtr (&pGkServer->ctxt, pGkServer->gkId.data);
   }
   pGkServer->gkId.nchars = idlen;
   pGkServer->gkId.data = (ASN116BITCHAR*)
      memAlloc (&pGkServer->ctxt, sizeof(ASN116BITCHAR)*idlen);

   if (0 == pGkServer->gkId.data) {
      OOTRACEERR1 ("Error:Failed to allocate memory for GK ID data\n");
      return OO_FAILED;
   }

   for (i = 0; i < idlen; i++) {
      pGkServer->gkId.data[i] = id[i];
   }

   OOTRACEINFO2 ("Gatekeeper Server ID set to '%s'\n", id);

   return 0;
}

int ooGkServerSetEpId (ooGkServer *pGkServer, const char* id)
{
   size_t i, idlen = strlen (id);

   if (0 == pGkServer) {
      OOTRACEERR1 ("ERROR: gatekeeper client not initialized in endpoint\n");
      return OO_FAILED;
   }
   if (0 != globals.pGkServer->endpointId.data) {
      memFreePtr (&pGkServer->ctxt, pGkServer->endpointId.data);
   }
   pGkServer->endpointId.nchars = idlen;
   pGkServer->endpointId.data = (ASN116BITCHAR*)
      memAlloc (&pGkServer->ctxt, sizeof(ASN116BITCHAR)*idlen);

   if (0 == pGkServer->endpointId.data) {
      OOTRACEERR1 ("Error:Failed to allocate memory for GK ID data\n");
      return OO_FAILED;
   }

   for (i = 0; i < idlen; i++) {
      pGkServer->endpointId.data[i] = id[i];
   }

   OOTRACEINFO2 ("Gatekeeper Endpoint ID set to '%s'\n", id);

   return 0;
}

/**
 * Create the RAS channel (socket).
 */
int ooGkServerCreateChannel(ooGkServer *pGkServer)
{
   int ret=0;
   OOIPADDR ipaddrs;
   /* Create socket */
   if((ret=ooSocketCreateUDP(&pGkServer->rasSocket))!=ASN_OK)
   {
      OOTRACEERR1("Failed to create RAS socket\n");
      // pGkServer->state = GkServerFailed;
      return OO_FAILED;
   }

   if(pGkServer->localRASPort)
   {
      ret= ooSocketStrToAddr (pGkServer->localRASIP, &ipaddrs);
      if( (ret=ooSocketBind( pGkServer->rasSocket, ipaddrs,
           pGkServer->localRASPort))!=ASN_OK )
      {
         OOTRACEERR1("ERROR:Failed to create RAS channel\n");
         // pGkServer->state = GkServerFailed;
         return OO_FAILED;
      }
   }
   else {
      ret = ooBindPort (OOUDP, pGkServer->rasSocket, pGkServer->localRASIP);
      if(ret == OO_FAILED)
      {
         OOTRACEERR1("ERROR: Failed to bind port to RAS socket\n");
         // pGkServer->state = GkServerFailed;
         return OO_FAILED;
      }
      pGkServer->localRASPort = ret;
   }
   /* Test Code NOTE- This doesn't work..:(( Have to fix this */
   /* If multihomed, get ip from socket */
   if(!strcmp(pGkServer->localRASIP, "0.0.0.0"))
   {
      OOTRACEDBGA1("Determining ip address for RAS channel "
                   "multihomed mode. \n");
      ret = ooSocketGetIpAndPort(pGkServer->rasSocket, pGkServer->localRASIP,
                                 20, &pGkServer->localRASPort);
      if(ret != ASN_OK)
      {
         OOTRACEERR1("Error:Failed to retrieve local ip and port from "
                     "socket for RAS channel(multihomed).\n");
         // pGkServer->state = GkServerFailed;
         return OO_FAILED;
      }
      OOTRACEDBGA3("Using local ip %s and port %d for RAS channel"
                   "(multihomedMode).\n", pGkServer->localRASIP,
                   pGkServer->localRASPort);
   }
   /* End of Test code */
   OOTRACEINFO1("H323 RAS channel creation - successful\n");
   return OO_OK;
}


int ooGkServerCloseChannel(ooGkServer *pGkServer)
{
   int ret;
   if(pGkServer->rasSocket != 0)
   {
      ret = ooSocketClose(pGkServer->rasSocket);
      if(ret != ASN_OK)
      {
         OOTRACEERR1("Error: failed to close RAS channel\n");
         pGkServer->rasSocket = 0;
         return OO_FAILED;
      }
      pGkServer->rasSocket = 0;
   }
   OOTRACEINFO1("Closed RAS channel\n");
   return OO_OK;
}


/**
 * Fill vendor data in RAS message structure.
 */

void ooGkServerFillVendor
   (ooGkServer *pGkServer, H225VendorIdentifier *pVendor )
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


int ooGkServerReceive(ooGkServer *pGkServer)
{
   ASN1OCTET recvBuf[1024];
   int recvLen;
   char remoteHost[32];
   int iFromPort=0;
   OOCTXT *pctxt=NULL;
   H225RasMessage *pRasMsg=NULL;
   int iRet=OO_OK;

   pctxt = &pGkServer->msgCtxt;

   recvLen = ooSocketRecvFrom(pGkServer->rasSocket, recvBuf, 1024, remoteHost,
                              32, &iFromPort);
   if(recvLen <0)
   {
      OOTRACEERR1("Error:Failed to receive RAS message\n");
      return OO_FAILED;
   }
   OOTRACEDBGA1("GkServer Received RAS Message\n");

   if(ASN_OK != setPERBuffer (pctxt, recvBuf, recvLen, TRUE ) )
   {
      OOTRACEERR1("Error:Failed to set PER buffer for RAS message decoding\n");
      memReset(pctxt);
      // pGkServer->state = GkServerFailed;
      return OO_FAILED;
   }
   pRasMsg = (H225RasMessage*)memAlloc(pctxt, sizeof(H225RasMessage));
   if(!pRasMsg)
   {
      OOTRACEERR1("Error: Failed to allocate memory for RAS message\n");
      memReset(pctxt);
      // pGkServer->state = GkServerFailed;
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
      iRet=ooGkServerHandleRASMessage( pGkServer, pRasMsg, remoteHost, iFromPort );
      if(iRet != OO_OK)
      {
         OOTRACEERR1("Error: Failed to handle received RAS message\n");
         //pGkServer->state = GkServerFailed;
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

int ooGkServerHandleRASMessage(ooGkServer *pGkServer, H225RasMessage *pRasMsg, const char *remoteHost, int remotePort)
{
   int iRet = OO_OK;
   switch( pRasMsg->t)
   {
   case T_H225RasMessage_gatekeeperConfirm:
      OOTRACEINFO1("Gatekeeper Confirmed (GCF) message received.\n");
      break;
   case T_H225RasMessage_gatekeeperReject:
      OOTRACEINFO1("Gatekeeper Reject (GRJ) message received\n");
      break;
   case T_H225RasMessage_registrationRequest:
      OOTRACEINFO1("Registration Request (RRQ) message received\n");
      iRet = ooGkServerHandleRegistrationRequest(pGkServer,
      	     pRasMsg->u.registrationRequest, remoteHost, remotePort );
      break;
   case T_H225RasMessage_registrationConfirm:
      OOTRACEINFO1("Registration Confirm (RCF) message received\n");
      break;
   case T_H225RasMessage_registrationReject:
      OOTRACEINFO1("Registration Reject (RRJ) message received.\n");
      break;
   case T_H225RasMessage_infoRequest:
      //ooRasSendIRR( psRasMsg->sMessage.u.infoRequest->requestSeqNum );
      break;
   case T_H225RasMessage_admissionConfirm:
      OOTRACEINFO1("Admission Confirm (ACF) message received\n");
      break;
   case T_H225RasMessage_unregistrationRequest:
      OOTRACEINFO1("UnRegistration Request (URQ) message received.\n");
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

// #ifndef _COMPACT
void ooGkServerPrintMessage
   (ooGkServer *pGkServer, ASN1OCTET *msg, ASN1UINT len)
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
// #endif
/**
 * Encode and send RAS message.
 */

int ooGkServerSendMsg(ooGkServer *pGkServer, H225RasMessage *pRasMsg, const char *host, int port)
{
   ASN1OCTET msgBuf[MAXMSGLEN];
   ASN1OCTET *msgPtr=NULL;
   int  iLen;
   OOCTXT *pctxt = &pGkServer->msgCtxt;

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

   if(ASN_OK != ooSocketSendTo( pGkServer->rasSocket, msgPtr, iLen,
                                host, port))
   {
      OOTRACEERR1("Error sending RAS message\n");
      return OO_FAILED;
   }
   else {/* should never go here */
      OOTRACEERR1("Error: GkServer in invalid state.\n");
      return OO_FAILED;
   }
// #ifndef _COMPACT
   ooGkServerPrintMessage(pGkServer, msgPtr, iLen);
// #endif
   return OO_OK;
}

/**
 * Manage incoming Reg message.
 */

int ooGkServerHandleRegistrationRequest
   (ooGkServer *pGkServer, H225RegistrationRequest *pRegistrationRequest, const char *remoteHost, int remotePort)
{
   // int i=0;
   // unsigned int x=0;
   // DListNode *pNode = NULL;
   // H225TransportAddress *pCallSigAddr=NULL;
   // ooGkServerTimerCb *cbData;
   // ASN1UINT regTTL=0;
   /* Extract Endpoint Id */

	printf("got request from %s:%d\n", remoteHost, remotePort);

	if(pGkServer->callbacks.onReceivedRegistrationRequest)
      pGkServer->callbacks.onReceivedRegistrationRequest(pGkServer,
                                      pRegistrationRequest, gH323ep.aliases, remoteHost, remotePort);
   return OO_OK;
}


int ooGkServerHandleUnregistrationRequest
   (ooGkServer *pGkServer, H225UnregistrationRequest * punregistrationRequest)
{
   int iRet=0;

   /* Lets first send unregistration confirm message back to gatekeeper*/
   ooGkServerSendUnregistrationConfirm(pGkServer,
                                      punregistrationRequest->requestSeqNum);

   if(punregistrationRequest->m.endpointAliasPresent)
   {
      OOTRACEINFO1("Gatekeeper requested a list of aliases be unregistered\n");
   }
   else{

      OOTRACEINFO1("Gatekeeper requested a all aliases to be unregistered\n");
      /* Send a fresh Registration request and if that fails, go back to
         Gatekeeper discovery.
      */
      OOTRACEINFO1("Sending fresh RRQ - as unregistration request received\n");
      pGkServer->rrqRetries = 0;
      // pGkServer->state = GkServerDiscovered;
   }

   if(pGkServer->callbacks.onReceivedUnregistrationRequest)
      pGkServer->callbacks.onReceivedUnregistrationRequest(
                                      punregistrationRequest, gH323ep.aliases);
   return OO_OK;
}

int ooGkServerSendUnregistrationConfirm(ooGkServer *pGkServer, unsigned reqNo)
{
   int iRet = OO_OK;
   OOCTXT *pctxt = &pGkServer->msgCtxt;
   H225RasMessage *pRasMsg=NULL;
   H225UnregistrationConfirm *pUCF=NULL;

   pRasMsg = (H225RasMessage*)memAlloc(pctxt, sizeof(H225RasMessage));
   pUCF = (H225UnregistrationConfirm*)memAlloc(pctxt,
                                           sizeof(H225UnregistrationConfirm));
   if(!pRasMsg || !pUCF)
   {
      OOTRACEERR1("Error: Memory allocation for UCF RAS message failed\n");
      // pGkServer->state = GkServerFailed;
      return OO_FAILED;
   }
   pRasMsg->t = T_H225RasMessage_unregistrationConfirm;
   pRasMsg->u.unregistrationConfirm = pUCF;
   memset(pUCF, 0, sizeof(H225UnregistrationConfirm));

   pUCF->requestSeqNum = reqNo;

   // todo: use real host and port
   iRet = ooGkServerSendMsg(pGkServer, pRasMsg, "host", 0);
   if(iRet != OO_OK)
   {
      OOTRACEERR1("Error:Failed to send UnregistrationConfirm message\n");
      memReset(pctxt);
      // pGkServer->state = GkServerFailed;
      return OO_FAILED;
   }
   OOTRACEINFO1("Unregistration Confirm message sent for \n");
   memReset(pctxt);

   return OO_OK;
}

int ooGkServerSendRegistrationConfirm(ooGkServer *pGkServer, unsigned reqNo, const char *host, int port)
{
   int iRet = OO_OK;
   OOCTXT *pctxt = &pGkServer->msgCtxt;
   H225RasMessage *pRasMsg=NULL;
   H225RegistrationConfirm *pRCF=NULL;
   H225TransportAddress *pTransportAddress=NULL;
   H225TransportAddress_ipAddress *pIpAddress=NULL;

   pRasMsg = (H225RasMessage*)memAlloc(pctxt, sizeof(H225RasMessage));
   pRCF = (H225RegistrationConfirm*)memAlloc(pctxt,
                                           sizeof(H225RegistrationConfirm));
   if(!pRasMsg || !pRCF)
   {
      OOTRACEERR1("Error: Memory allocation for UCF RAS message failed\n");
      // pGkServer->state = GkServerFailed;
      return OO_FAILED;
   }
   pRasMsg->t = T_H225RasMessage_registrationConfirm;
   pRasMsg->u.registrationConfirm = pRCF;
   memset(pRCF, 0, sizeof(H225RegistrationConfirm));

   pRCF->requestSeqNum = reqNo;
   pRCF->protocolIdentifier = gProtocolID;

   pTransportAddress = (H225TransportAddress*) memAlloc(pctxt,
                                                 sizeof(H225TransportAddress));
   pIpAddress = (H225TransportAddress_ipAddress*) memAlloc(pctxt,
                                       sizeof(H225TransportAddress_ipAddress));
   if(!pTransportAddress || !pIpAddress)
   {
      OOTRACEERR1("Error:Failed to allocate memory for signalling address of "
                  "RRQ message\n");
      memReset(pctxt);
      return OO_FAILED;
   }
   pTransportAddress->t = T_H225TransportAddress_ipAddress;
   pTransportAddress->u.ipAddress = pIpAddress;
   ooSocketConvertIpToNwAddr
     (pGkServer->localRASIP, pIpAddress->ip.data, sizeof(pIpAddress->ip.data));
   pIpAddress->ip.numocts = 4;
   pIpAddress->port = gH323ep.listenPort;

   dListInit(&pRCF->callSignalAddress);
   dListAppend(pctxt, &pRCF->callSignalAddress, pTransportAddress);

   pRCF->m.terminalAliasPresent=TRUE;
   if(OO_OK != ooPopulateAliasList(pctxt, gH323ep.aliases,
                                      &pRCF->terminalAlias))
   {
      OOTRACEERR1("Error filling alias for RRQ\n");
      memReset(pctxt);
      return OO_FAILED;
   }

   pRCF->m.gatekeeperIdentifierPresent=TRUE;
   pRCF->gatekeeperIdentifier.nchars = pGkServer->gkId.nchars;
   pRCF->gatekeeperIdentifier.data = (ASN116BITCHAR*)memAlloc
                         (pctxt, pGkServer->gkId.nchars*sizeof(ASN116BITCHAR));
   if(!pRCF->gatekeeperIdentifier.data)
   {
      OOTRACEERR1("Error: Failed to allocate memory for GKIdentifier in RCF "
                   "message.\n");
      memReset(pctxt);
      return OO_FAILED;
   }
   memcpy(pRCF->gatekeeperIdentifier.data, pGkServer->gkId.data,
                                pGkServer->gkId.nchars*sizeof(ASN116BITCHAR));

   pRCF->endpointIdentifier.data =
        (ASN116BITCHAR*)memAlloc(pctxt, pGkServer->endpointId.nchars*sizeof(ASN116BITCHAR));
   if (pRCF->endpointIdentifier.data) {
      pRCF->endpointIdentifier.nchars = pGkServer->endpointId.nchars;
      memcpy(pRCF->endpointIdentifier.data, pGkServer->endpointId.data, pGkServer->endpointId.nchars*sizeof(ASN116BITCHAR));
      OOTRACEINFO1("Sending RCF (with EndpointID)\n");
   } else {
      OOTRACEERR1("Error: Failed to allocate memory for EndpointIdentifier in RCF \n");
      memReset(pctxt);
      return OO_FAILED;
   }

   printf("send confirm to %s:%d\n", host, port);

   iRet = ooGkServerSendMsg(pGkServer, pRasMsg, host, port);
   if(iRet != OO_OK)
   {
      OOTRACEERR1("Error:Failed to send RegistrationConfirm message\n");
      memReset(pctxt);
      // pGkServer->state = GkServerFailed;
      return OO_FAILED;
   }
   OOTRACEINFO1("Registration Confirm message sent for \n");
   memReset(pctxt);

   return OO_OK;
}
