/*
 * Copyright (C) 2005 by Page Iberica, S.A.
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

/**
 * @file ooqras.c
 * This file contains functions to support RAS protocol.
 *
 */
#include "ooras.h"
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

/** Global endpoint structure */
extern ooEndPoint gH323ep;

#define OORAS_MAX_ID_LENGTH 128
#define DEFAULT_GKPORT 1719
#define MULTICAST_GKADDRESS "224.0.1.41"
#define MULTICAST_GKPORT 1718
#define DEFAULT_REGISTRATIONTTL 300 /* 300 sec */
#define RASARQDRQTIMEOUT 5 /* 5 sec */
#define RASARQDRQMAXRETRIES 3

enum RasStatus {
   RasIdle = 0,
   RasGRQSent,
   RasGRQRejected,
   RasRRQSent,
   RasRRQRejected,
   RasRegistered,
   RasRegisteredRRQ
};

enum RasCallStatus {
   RasCallIdle = 0,
   RasCallARQSent,
   RasCallACFRecv,
   RasCallARQRej,
   RasCallDRQSent,
   RasCallDCFRecv,
   RasCallDRQRej
};

typedef struct RasGatekeeperInfo
{
   ASN1BOOL willRespondToIRR;
   H225UUIEsRequested uuiesRequested;
   H225BandWidth bw;
   H225RegistrationConfirm_preGrantedARQ preGrantedARQ;
}RasGatekeeperInfo;
/**
 *  Call Admission Information
 */
typedef struct RasCallAdmissionInfo
{
   ooCallData *call;
   unsigned char ucStatus;
   struct timeval sLastTime; /* Last time the ARQ/DRQ message was sent */
   unsigned int uiRetries;
   unsigned short usRequestSeqNum;
   enum RasCallModel eCallModel;
   ASN1USINT irrFrequency;
} RasCallAdmissionInfo;

static int giRasInitialized=0;
static OOCTXT gsRASCTXT;
static OOSOCKET gRASSocket=0;
static gLocalRASPort = 0;
static int giDiscoveryComplete = 0;
char gGkRasIP[20], gGkCallSignallingIP[20];
static RasGatekeeperInfo gGkInfo;
int gGkRasPort, gGkCallSignallingPort;
unsigned short gusRequestSeqNum=0;
static unsigned int guiRRQRetries=0;
static ASN1UINT guiRegistrationTTL=0; /* Initial Value, updated using value from RCF*/
static enum RasGatekeeperMode geCurGkMode=RasNoGatekeeper;
static enum RasStatus geRasStatus=RasIdle;
static ooRasParams gsRasParams;
static struct timeval sLastTime;
static struct timeval gsRegistrationTime;
static ASN1OBJID gProtocolID = {
   6, { 0, 0, 8, 2250, 0, 4 }
};

H225GatekeeperIdentifier sGatekeeperId;
ASN116BITCHAR ausGatekeeperId[OORAS_MAX_ID_LENGTH];
unsigned char ucGkIdValid=0;

H225EndpointIdentifier sEndpointId;
ASN116BITCHAR ausEndpointId[OORAS_MAX_ID_LENGTH];

/**
 *  Calls pending either for for admission or disengage.
 */

DList gsCallsPendingList;

/**
 * Calls currently admitted and active
 */
DList gsCallsAdmittedList;

static int ooRasCreateChannel();
static int ooRasCloseChannel();
static int ooCreateRasMessage( ooRasMessage **ppsRasMsg);
static int ooFreeRasMessage( ooRasMessage *psRasMsg );
static int ooRasSend( ooRasMessage *psRasMsg);
static int ooRasSendRRQ(int keepAlive );
static int ooRasManageMessage( ooRasMessage *psRasMsg );
static int ooRasManageGatekeeperConfirm
   ( H225GatekeeperConfirm *psGatekeeperConfirm );
static int ooRasManageGatekeeperReject(H225GatekeeperReject *psGatekeeperReject);
static int ooRasManageRegistrationConfirm
   (H225RegistrationConfirm *psRegistrationConfirm);
static int ooRasManageRegistrationReject
   (H225RegistrationReject *psRegistrationReject);
static int ooRasManageUnregistrationRequest
   (H225UnregistrationRequest * psunregistrationRequest);
static int ooRasManageAdmissionConfirm
   (H225AdmissionConfirm *psAdmissionConfirm);
static void ooRasManageDisengageConfirm(H225DisengageConfirm * psDCF);
static long ooRasTimeDiff( struct timeval *psTime1,
                           struct timeval *psTime2 );
static int ooRasSendGRQ();



int ooDestroyRas()
{
   if(giRasInitialized)
      freeContext(&gsRASCTXT);
   giRasInitialized = 0;
   return OO_OK;
}
/**
 * RAS module startup stuff.
 */
int ooInitRas(int localRasPort, enum RasGatekeeperMode eGkMode,
              char *szGkAddr, int iGkPort )
{
   int iRet;
   initContext(&(gsRASCTXT));
   giRasInitialized = 1;
   geRasStatus=RasIdle;
   guiRRQRetries=0;
   memset((void*)&sGatekeeperId, 0, sizeof(H225GatekeeperIdentifier));
   memset((void*)ausGatekeeperId, 0, OORAS_MAX_ID_LENGTH*sizeof(ASN116BITCHAR));
   memset((void*)&sEndpointId, 0, sizeof(H225EndpointIdentifier));
   memset((void*)&ausEndpointId, 0, OORAS_MAX_ID_LENGTH*sizeof(ASN116BITCHAR));
   gLocalRASPort = localRasPort;
   ooRasSetGatekeeperMode(eGkMode, szGkAddr, iGkPort);
 
   /* Create default parameter set */
   gsRasParams.uiGRQTimeout = 6000;  /* GRQ timeout = 6 seconds */
   gsRasParams.uiRRQTimeout = 6000;  /* RRQ timeout = 6 seconds */
   gsRasParams.uiRRQRetries = 3;     /* 3 RRQ retries       */
   gsRasParams.uiAddRegPeriod = 30000; /* Additive registrations each 30 secs*/

  
   dListInit(&gsCallsPendingList);
   dListInit(&gsCallsAdmittedList);

   ooGetTimeOfDay(&sLastTime, NULL);
  
   iRet = ooRasCreateChannel();

   if(iRet == OO_OK)
      OOTRACEINFO1("RAS Initialization successful\n");
   else
      OOTRACEERR1("Error: RAS Initialization failed\n");

   return iRet;
}

/**
 * Call this function periodically.
 */

EXTERN void ooRasExplorer()
{
   struct timeval sCurrentTime;
   int iRet=0;
   long keepAliveTimeout=0;
   int keepAlive=0;
   if ( RasNoGatekeeper==geCurGkMode )
   {
      /* Do nothing ? */
   }
   else
   {  
      /* Searching for gatekeeper*/
      switch( geRasStatus )
      {
      case RasIdle:  /*--- IDLE ---------------*/
         giDiscoveryComplete = 0;
         guiRRQRetries = 0;
         iRet = ooRasSendGRQ();
         if(iRet != OO_OK)
         {
           OOTRACEERR1("Error: Failed to Send GRQ message: Forcing No Gatekeeper mode\n");
           ooRasSetGatekeeperMode(RasNoGatekeeper, 0, 0);
         }else{
            geRasStatus=RasGRQSent;
         }
         break;
      case RasGRQSent:  /*-- GRQ SENT ----------*/
         ooGetTimeOfDay(&sCurrentTime, NULL);
         if ( (long)gsRasParams.uiGRQTimeout <=
                              ooRasTimeDiff(&sLastTime,&sCurrentTime) )
         {
            /* Time to resend GRQ */
            OOTRACEINFO1("Resending GRQ as no response from gatekeeper\n");
            ooRasSendGRQ();
         }
         break;
      case RasGRQRejected:
         OOTRACEERR1("Error: Gatekeeper Rejected. Forcing No Gatekeeper mode\n");
         geRasStatus = RasIdle;
         ooRasSetGatekeeperMode(RasNoGatekeeper, 0, 0);
         break;
      case RasRRQSent:  /*-- RRQ SENT -----------*/
             ooGetTimeOfDay(&sCurrentTime, NULL);
         if ( (long)gsRasParams.uiRRQTimeout <=
                                   ooRasTimeDiff(&sLastTime,&sCurrentTime) )
         {
            OOTRACEINFO1("RRQ Timeout - ");
            if ( gsRasParams.uiRRQRetries <= guiRRQRetries++ )
            {
               OOTRACEINFO1("RRQ Max Retries done - Resending GRQ\n");
               /* Back to GRQs */
               iRet = ooRasSendGRQ();
               if(iRet == OO_OK)
               {
                  geRasStatus=RasGRQSent;
                  giDiscoveryComplete = 0;
               }else {
                  geRasStatus=RasIdle;
                  guiRRQRetries = 0;
                  giDiscoveryComplete = 0;
               }
            }
            else{
               OOTRACEINFO1("Resending RRQ\n");
               ooRasSendRRQ(0);   /* Time to resend RRQ */
            }
         }
         break;
      case RasRRQRejected:
         OOTRACEINFO1("RRQ Rejected - Can't recover, forcing NoGatekeeperMode\n");
         ooRasSetGatekeeperMode(RasNoGatekeeper, 0, 0);
         giDiscoveryComplete = 0;
         guiRRQRetries = 0;
         break;
      case RasRegistered:  /*--- REGISTERED -----------*/
         if(guiRegistrationTTL) /* KeepAlive supported by gatekeeper*/
         {
                ooGetTimeOfDay(&sCurrentTime, NULL);
            /* Subtract 5 secs from TTL, just to make sure that we
               re-register before the TTL expires */
            if(guiRegistrationTTL>5)
               keepAliveTimeout = (guiRegistrationTTL-5)*1000;
            else
               keepAliveTimeout = guiRegistrationTTL*1000;
            if ( (long)keepAliveTimeout <=
                           ooRasTimeDiff(&gsRegistrationTime,&sCurrentTime) )
                        {
               /* Time to resend RRQ(KeepAlive) */
               guiRRQRetries = 0;
               iRet=ooRasSendRRQ(1);
               if(iRet != OO_OK)
               {
                  OOTRACEERR1("Error: Failed to send KeepAlive RRQ message - "
                              "Rediscovering\n");
                  geRasStatus=RasIdle;
                  guiRRQRetries = 0;
                  giDiscoveryComplete = 0;
               }
            }
         }
         break;
      case RasRegisteredRRQ: /*-- REGISTERED RRQ ---------*/
         ooGetTimeOfDay(&sCurrentTime, NULL);
         if((long)guiRegistrationTTL*1000 <=
                         ooRasTimeDiff(&gsRegistrationTime, &sCurrentTime))
            keepAlive = 0;
         else
            keepAlive = 1;
         if((long)gsRasParams.uiRRQTimeout <=
                                  ooRasTimeDiff(&sLastTime,&sCurrentTime) )
         {
            if(gsRasParams.uiRRQRetries <= guiRRQRetries++ )
            {
               OOTRACEINFO1("RRQ KeepAlive Max Retries done - Resending GRQ\n");
               /* Back to GRQs */
               iRet = ooRasSendGRQ();
               if(iRet == OO_OK)
               {
                  geRasStatus=RasGRQSent;
                  giDiscoveryComplete = 0;
               }else {
                  geRasStatus=RasIdle;
                  guiRRQRetries = 0;
                  giDiscoveryComplete = 0;
               }
            }
            else{
               iRet = ooRasSendRRQ(keepAlive); /* Time to resend RRQ */
               if(iRet != OO_OK)
               {
                  OOTRACEERR1("Error: Failed to send RRQ message\n");
               }
               else{
                  if(!keepAlive)
                     guiRRQRetries = 0;
               }
            }
         }
         break;
      default:
         OOTRACEERR1("ERROR:Unknown RAS status(VERY BAD)!!!\n");
         geRasStatus=RasIdle;
      }
   }    
}


int ooRasSetGatekeeperMode(enum RasGatekeeperMode eGkMode,
                                   char *szGkAddr,
                                   int iGkPort )
{
   geCurGkMode = eGkMode;
   if(eGkMode == RasUseSpecificGatekeeper)
   {
      OOTRACEINFO1("Gatekeeper Mode - RasUseSpecificGatekeeper\n");
      if(szGkAddr)
         strcpy(gGkRasIP, szGkAddr);
      if(iGkPort)
         gGkRasPort = iGkPort;
      else
         gGkRasPort = DEFAULT_GKPORT;
      OOTRACEINFO3("Gatekeeper IP:port set to - %s:%d\n",
                     (szGkAddr)?szGkAddr:"discover",  gGkRasPort);
   }else if(eGkMode == RasDiscoverGatekeeper)
   {
      OOTRACEINFO1("Gatekeeper Mode - RasDiscoverGatekeeper\n");
   }else if(eGkMode == RasNoGatekeeper)
   {
      OOTRACEINFO1("Gatekeeper Mode - RasNoGatekeeper\n");
   }

   return OO_OK;
}

  
static long ooRasTimeDiff( struct timeval *psTime1,
                           struct timeval *psTime2 )
{
   return ( ((psTime2->tv_sec-psTime1->tv_sec)*1000) +
            ((psTime2->tv_usec-psTime1->tv_usec)/1000) );
}

/**
 * Create the RAS channel (socket).
 *
 */
static int ooRasCreateChannel()
{
   int ret=0;
   OOIPADDR ipaddrs;
   /* Create socket */
   if((ret=ooSocketCreateUDP(&gRASSocket))!=ASN_OK)
   {
      OOTRACEERR1("Failed to create RAS socket\n");
      return OO_FAILED;
   }
   if(gLocalRASPort)
   {
      ret= ooSocketStrToAddr (gH323ep.signallingIP, &ipaddrs);
      if( (ret=ooSocketBind( gRASSocket, ipaddrs, gLocalRASPort))==ASN_OK )
      {
         OOTRACEINFO1("H323 RAS channel creation - successful\n");
         return OO_OK;
      }
      else
      {
         OOTRACEERR1("ERROR:Failed to create RAS channel\n");
         return OO_FAILED;
      }
   }
   else {
      ret = ooBindPort(&gH323ep, OOUDP, gRASSocket);
      if(ret == OO_FAILED)
      {
         OOTRACEERR1("ERROR: Failed to bind port to RAS socket\n");
         return OO_FAILED;
      }
      gLocalRASPort = ret;
   }
   return OO_OK;
}

static int ooRasCloseChannel()
{
   int ret;
   ret = ooSocketClose(gRASSocket);
   if(ret != ASN_OK)
   {
      OOTRACEERR1("Error: failed to close RAS channel\n");
      return OO_FAILED;
   }
   OOTRACEINFO1("Closing RAS channel\n");
   return OO_OK;
}

static int ooCreateRasMessage( ooRasMessage **ppsRasMsg)
{
   OOCTXT *pctxt = NULL;
  
   pctxt = (OOCTXT*)newContext();
   if(!pctxt)
   {
      OOTRACEERR1("Error: Failed to allocate OOCTXT for RAS message "
                  "creation\n");
      return OO_FAILED;
   }
  
   *ppsRasMsg = (ooRasMessage*)ASN1MALLOC(pctxt, sizeof(ooRasMessage));
               
   if(!*ppsRasMsg)
   {
      OOTRACEERR1("Error: Failed to allocate memory for RAS message\n");
      return OO_FAILED;
   }
   else
   {
      memset(*ppsRasMsg, 0, sizeof(ooRasMessage));
      (*ppsRasMsg)->psContext = pctxt;
      return OO_OK;
   }
}

/**
 * Free message.
 */

static int ooFreeRasMessage( ooRasMessage *psRasMsg )
{
   OOCTXT * pctxt;
   if ( NULL!=psRasMsg )
   {
      pctxt = psRasMsg->psContext;
      freeContext(pctxt);
      ASN1CRTFREE0(pctxt);
   }
   return OO_OK;
}

/**
 * Fill vendor data in RAS message structure.
 */

static void ooRasFillVendor( H225VendorIdentifier *psVendor )
{
   psVendor->vendor.t35CountryCode = gH323ep.t35CountryCode;
   psVendor->vendor.t35Extension = gH323ep.t35Extension;
   psVendor->vendor.manufacturerCode = gH323ep.manufacturerCode;
   psVendor->enterpriseNumber.numids=0;
   if(gH323ep.productID)
   {
      psVendor->m.productIdPresent = 1;
      psVendor->productId.numocts = ASN1MIN(strlen(gH323ep.productID),
                                             sizeof(psVendor->productId.data));
      strncpy(psVendor->productId.data, gH323ep.productID,
                                                  psVendor->productId.numocts);
   }
   if(gH323ep.versionID)
   {
      psVendor->m.versionIdPresent = 1;
      psVendor->versionId.numocts = ASN1MIN(strlen(gH323ep.versionID),
                                             sizeof(psVendor->versionId.data));
      strncpy(psVendor->versionId.data, gH323ep.versionID,
                                                 psVendor->versionId.numocts);
   }
}  

/**
 * Fill alias data in RAS message structure.
 */

int ooRasFillAlias( OOCTXT *psContext, ooAliases *psAliases,
                     H225_SeqOfH225AliasAddress *psAliasList )
{
   H225AliasAddress *psAliasEntry=NULL;
   ooAliases * psAlias;
   int i = 0;
   dListInit(psAliasList);
   if(psAliases)
   {
      psAlias = psAliases;
      while(psAlias)
      {
         psAliasEntry = (H225AliasAddress*)ASN1MALLOC(psContext,
                                                     sizeof(H225AliasAddress));
         if(!psAliasEntry)
         {
            OOTRACEERR1("ERROR: Failed to allocate memory for alias entry\n");
            return OO_FAILED;
         }
         switch(psAlias->type)
         {
         case T_H225AliasAddress_dialedDigits:
            psAliasEntry->t = T_H225AliasAddress_dialedDigits;
            psAliasEntry->u.dialedDigits = (ASN1IA5String)ASN1MALLOC(psContext,
                                                     strlen(psAlias->value)+1);
            if(!psAliasEntry->u.dialedDigits)
            {
               OOTRACEERR1("ERROR:Failed to allocated memory for dialedDigits"
                           " alias entry\n");
               ASN1MEMFREEPTR(psContext, psAliasEntry);
               return OO_FAILED;
            }
            memset((void*)psAliasEntry->u.dialedDigits, 0,
                                                strlen(psAlias->value)+1);
            strcpy((char*)psAliasEntry->u.dialedDigits, psAlias->value);
            break;
         case T_H225AliasAddress_h323_ID:
            psAliasEntry->t = T_H225AliasAddress_h323_ID;
            psAliasEntry->u.h323_ID.nchars = strlen(psAlias->value);
            psAliasEntry->u.h323_ID.data = (ASN116BITCHAR*)ASN1MALLOC
                     (psContext, strlen(psAlias->value)*sizeof(ASN116BITCHAR));
            if(!psAliasEntry->u.h323_ID.data)
            {
               OOTRACEERR1("Error: Failed to allocate memory for h323id"
                           " data alias entry\n");
               ASN1MEMFREEPTR(psContext, psAliasEntry);
               return OO_FAILED;
            }
            for(i=0; i<(int)strlen(psAlias->value); i++)
               psAliasEntry->u.h323_ID.data[i] = (ASN116BITCHAR)psAlias->value[i];
            break;
         case T_H225AliasAddress_url_ID:
            psAliasEntry->t = T_H225AliasAddress_url_ID;
            psAliasEntry->u.url_ID = (ASN1IA5String)ASN1MALLOC(psContext,
                                                     strlen(psAlias->value)+1);
            if(!psAliasEntry->u.url_ID)
            {
               OOTRACEERR1("ERROR: Failed to allocate memory for urlID alias "
                           "entry \n");
               ASN1MEMFREEPTR(psContext, psAliasEntry);
               return OO_FAILED;
            }
            memset((void*)psAliasEntry->u.url_ID, 0, strlen(psAlias->value)+1);
            strcpy((char*)psAliasEntry->u.url_ID, psAlias->value);
            break;
         case T_H225AliasAddress_email_ID:
            psAliasEntry->t = T_H225AliasAddress_email_ID;
            psAliasEntry->u.email_ID = (ASN1IA5String)ASN1MALLOC(psContext,
                                                     strlen(psAlias->value)+1);
            if(!psAliasEntry->u.email_ID)
            {
               OOTRACEERR1("ERROR: Failed to allocate memory for EmailID "
                           "alias entry \n");
               ASN1MEMFREEPTR(psContext, psAliasEntry);
               return OO_FAILED;
            }
            memset((void*)psAliasEntry->u.email_ID, 0, strlen(psAlias->value)+1);
            strcpy((char*)psAliasEntry->u.email_ID, psAlias->value);
            break;
         default:
             OOTRACEERR1("ERROR: Unhandled alias type\n");
         }
         dListAppend( psContext, psAliasList, (void*)psAliasEntry );
         psAlias = psAlias->next;
      }
   }
   return OO_OK;
}

/* This function is invoked to get current RAS socket.
 *
 * @return  RAS socket.
 */

OOSOCKET ooRasGetSocket()
{
   return gRASSocket;
}

ASN1BOOL ooRasIsRegistered()
{
   return (geRasStatus == RasRegistered || geRasStatus == RasRegisteredRRQ);
}

enum RasGatekeeperMode ooRasGetGatekeeperMode()
{
   return geCurGkMode;
}

int ooRasReceive()
{
   ASN1OCTET atBuffer[1024];
   int iLongPacket;
   char acRemoteHost[32];
   int iFromPort=0;
   OOCTXT *psContext=NULL;
   ooRasMessage *psRasMsg=NULL;
   int iRet=OO_OK;

   memset((void*)atBuffer, 0, sizeof(atBuffer));
   memset((void*)acRemoteHost, 0, sizeof(acRemoteHost));

   iLongPacket = ooSocketRecvFrom(gRASSocket, atBuffer, 1024, acRemoteHost, &iFromPort);
   OOTRACEINFO2("Receive length %d\n", iLongPacket);
   if(iLongPacket <=0)
      return OO_OK;
   OOTRACEINFO1("RAS Message received\n");
   if((strncmp(gGkRasIP,acRemoteHost,strlen(gGkRasIP))) ||
      (gGkRasPort!= iFromPort) )
   {
      OOTRACEWARN3("WARN:Ignoring message received from unknown gatekeeper "
                   "%s:%d\n", acRemoteHost, iFromPort);
      return OO_OK;
   }
   if(NULL == (psContext=newContext()) )
   {
      OOTRACEERR1("Error: Failed to allocate OOCTXT for RAS message decoding\n");
      return OO_FAILED;
   }
   if(ASN_OK != setPERBuffer (psContext, atBuffer, iLongPacket, TRUE ) )
   {
      OOTRACEERR1("Error: Failed to set PER buffer in context for RAS message decoding\n");
      freeContext(psContext);
      ASN1CRTFREE0(psContext);
      return OO_FAILED;
   }     
   psRasMsg = (ooRasMessage*)ASN1MALLOC(psContext, sizeof(ooRasMessage));
   if(!psRasMsg)
   {
      OOTRACEERR1("Error: Failed to allocate memory for RAS message\n");
      freeContext(psContext);
      ASN1CRTFREE0(psContext);
      return OO_FAILED;
   }
   psRasMsg->psContext = psContext;
   initializePrintHandler(&printHandler, "Received RAS Message");
   /* Add event handler to list */
   rtAddEventHandler (psContext, &printHandler);
   if ( ASN_OK== asn1PD_H225RasMessage( psContext, &psRasMsg->sMessage ) )
   {
      finishPrint();
      rtRemoveEventHandler(psContext, &printHandler);
      iRet=ooRasManageMessage( psRasMsg );
      if(iRet != OO_OK)
      {
         OOTRACEERR1("Error: Failed to handle received RAS message\n");
      }
   }
   else{
      OOTRACEERR1("ERROR:Failed to decode receive RAS message\n");
      rtRemoveEventHandler(psContext, &printHandler);
      freeContext(psContext);
      ASN1CRTFREE0(psContext);
      return OO_FAILED;
   }

   return iRet;
}

/**
 * Manage incoming RAS message.
 */

static int ooRasManageMessage( ooRasMessage *psRasMsg )
{
   int iRet = OO_OK;  
   switch( psRasMsg->sMessage.t)
   {
   case T_H225RasMessage_gatekeeperConfirm:
      OOTRACEINFO1("Gatekeeper Confirmed (GCF) message received.\n");
      ooRasManageGatekeeperConfirm(psRasMsg->sMessage.u.gatekeeperConfirm);
      break;
   case T_H225RasMessage_gatekeeperReject:
      OOTRACEINFO1("Gatekeeper Reject (GRJ) message received\n");
      ooRasManageGatekeeperReject(psRasMsg->sMessage.u.gatekeeperReject);
      break;
   case T_H225RasMessage_registrationConfirm:  
      OOTRACEINFO1("Registration Confirm (RCF) message received\n");
      ooRasManageRegistrationConfirm( psRasMsg->sMessage.u.registrationConfirm );
      break;
   case T_H225RasMessage_registrationReject:
      OOTRACEINFO1("Registration Reject (RRJ) message received.\n");
      ooRasManageRegistrationReject( psRasMsg->sMessage.u.registrationReject);
      break;
   case T_H225RasMessage_infoRequest: 
      //ooRasSendIRR( psRasMsg->sMessage.u.infoRequest->requestSeqNum );
      break;
   case T_H225RasMessage_admissionConfirm:
      OOTRACEINFO1("Admission Confirm (ACF) message received\n");
      ooRasManageAdmissionConfirm( psRasMsg->sMessage.u.admissionConfirm );
      break;
   case T_H225RasMessage_gatekeeperRequest:
   case T_H225RasMessage_registrationRequest:
   case T_H225RasMessage_unregistrationRequest:
      OOTRACEINFO1("UnRegistration Request (URQ) message received.\n");
      ooRasManageUnregistrationRequest(psRasMsg->sMessage.u.unregistrationRequest);
      break;
   case T_H225RasMessage_unregistrationConfirm:
      OOTRACEINFO1("UnRegistration Confirm (UCF) message received.\n");
      break;
   case T_H225RasMessage_unregistrationReject:
      OOTRACEINFO1("UnRegistration Reject (URJ) message received.\n");
      break;
   case T_H225RasMessage_admissionRequest:
   case T_H225RasMessage_admissionReject:
      OOTRACEINFO1("Admission Reject (ARJ) message received.\n");
      break;
   case T_H225RasMessage_bandwidthRequest:
   case T_H225RasMessage_bandwidthConfirm:
   case T_H225RasMessage_bandwidthReject:
   case T_H225RasMessage_disengageRequest:
   case T_H225RasMessage_disengageConfirm:
      ooRasManageDisengageConfirm(psRasMsg->sMessage.u.disengageConfirm);
      break;
   case T_H225RasMessage_disengageReject:
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
      iRet= OO_FAILED;
   }
   ooFreeRasMessage(psRasMsg);
   return iRet;
}

/**
 * Send encoded RAS message given RAS message structure.
 */

static int ooRasSend( ooRasMessage *psRasMsg )
{
   ASN1OCTET atMsgBuf[MAXMSGLEN];
   ASN1OCTET *ptMsgPtr=NULL;
   int  iLen;

   setPERBuffer( psRasMsg->psContext, atMsgBuf, MAXMSGLEN, TRUE );

   if ( ASN_OK == asn1PE_H225RasMessage(psRasMsg->psContext, &psRasMsg->sMessage) )
   {
      OOTRACEDBGC1("Ras message encoding - successful\n");
   }
   else {
      OOTRACEERR1("Error: RAS message encoding failed\n");
      ooFreeRasMessage(psRasMsg);
      return OO_FAILED;
   }

   ptMsgPtr = encodeGetMsgPtr( psRasMsg->psContext, &iLen );
   /* If gatekeeper specified or have been discovered */
   if(geCurGkMode == RasUseSpecificGatekeeper || giDiscoveryComplete)
   {
      if(ASN_OK != ooSocketSendTo( gRASSocket, ptMsgPtr, iLen, gGkRasIP, gGkRasPort))
      {
         OOTRACEERR1("Error sending RAS message\n");
         ooFreeRasMessage(psRasMsg);
         return OO_FAILED;
      }
   }else if(geCurGkMode == RasDiscoverGatekeeper && !giDiscoveryComplete)
   {
      if(ASN_OK != ooSocketSendTo(gRASSocket, ptMsgPtr, iLen,
                                       MULTICAST_GKADDRESS, MULTICAST_GKPORT))
      {
         OOTRACEERR1("Error sending multicast RAS message\n" );
         ooFreeRasMessage(psRasMsg);
         return OO_FAILED;
      }
   }else if(geCurGkMode == RasNoGatekeeper)
   {
      OOTRACEWARN1("Ignoring RAS message send as Gatekeeper usage disabled\n");
      ooFreeRasMessage(psRasMsg);
      return OO_FAILED;
   }
   ooFreeRasMessage(psRasMsg);
   return OO_OK;
}

static int ooRasSendGRQ()
{
   int iRet;
   
    /*   int iRet, iEk, iDon, iTeen, iChaar;*/
   ooRasMessage *psRasMsg=NULL;
   H225GatekeeperRequest *psGkReq=NULL;
   OOCTXT *psCtxt;
   H225TransportAddress_ipAddress *psRasAddress;
   /*   char hexip[20];*/
   giDiscoveryComplete = 0;
  
   memset(&gGkInfo, 0, sizeof(RasGatekeeperInfo));
   iRet = ooCreateRasMessage(&psRasMsg);
   if(!psRasMsg)
   {
      OOTRACEERR1("Error: Memory allocation for GRQ RAS message failed\n");
      return OO_FAILED;
   }
   psCtxt = psRasMsg->psContext;
   psGkReq = (H225GatekeeperRequest*)ASN1MALLOC(psCtxt,
                                                sizeof(H225GatekeeperRequest));
   if(!psGkReq)
   {
      OOTRACEERR1("Error:Memory allocation for GRQ failed\n");
      ooFreeRasMessage(psRasMsg);
      return OO_FAILED;
   }
   memset(psGkReq, 0, sizeof(H225GatekeeperRequest));
   psRasMsg->sMessage.t = T_H225RasMessage_gatekeeperRequest;
   psRasMsg->sMessage.u.gatekeeperRequest = psGkReq;

   /* Populate message structure */
   psGkReq->requestSeqNum = gusRequestSeqNum++;
   if ( !psGkReq->requestSeqNum )
      psGkReq->requestSeqNum = gusRequestSeqNum++;
   psGkReq->protocolIdentifier = gProtocolID;
   psGkReq->m.nonStandardDataPresent=0;
   psGkReq->rasAddress.t=T_H225TransportAddress_ipAddress; /* IPv4 address */
   psRasAddress = (H225TransportAddress_ipAddress*)ASN1MALLOC(psCtxt,
                                      sizeof(H225TransportAddress_ipAddress));
   if(!psRasAddress)
   {
      OOTRACEERR1("Error: Memory allocation for Ras Address of GRQ message "
                  "failed\n");
      ooFreeRasMessage(psRasMsg);
      return OO_FAILED;
   }
   memset(psRasAddress, 0, sizeof(H225TransportAddress_ipAddress));
   ooConvertIpToNwAddr(gH323ep.signallingIP, psRasAddress->ip.data);
#if 0
   sscanf(gH323ep.signallingIP, "%d.%d.%d.%d", &iEk, &iDon, &iTeen, &iChaar);
   sprintf(hexip, "%x %x %x %x", iEk, iDon, iTeen, iChaar);
   sscanf(hexip, "%x %x %x %x", &(psRasAddress->ip.data[0]),
                                &(psRasAddress->ip.data[1]),
                                &(psRasAddress->ip.data[2]),
                                &(psRasAddress->ip.data[3]));  
#endif
   psRasAddress->ip.numocts = 4;
   psRasAddress->port = gLocalRASPort;
   psGkReq->rasAddress.u.ipAddress = psRasAddress;
   psGkReq->endpointType.m.nonStandardDataPresent=0;
   psGkReq->endpointType.m.vendorPresent=1;
   ooRasFillVendor( &psGkReq->endpointType.vendor );
   psGkReq->m.endpointAliasPresent=1;
   if ( OO_OK != ooRasFillAlias( psCtxt, gH323ep.aliases, &psGkReq->endpointAlias ) )
   {
      OOTRACEERR1("Error filling alias for GRQ\n");
      ooFreeRasMessage( psRasMsg );
      return OO_FAILED;
   }
   iRet = ooRasSend(psRasMsg);
   if(iRet != OO_OK)
   {
      OOTRACEERR1("Error: Failed to send GRQ message\n");
      return OO_FAILED;
   }
   ooGetTimeOfDay(&sLastTime, 0); /* Get time stamp */
   OOTRACEINFO1("GRQ Ras message sent successfully\n");
   return OO_OK;
}

static int ooRasManageGatekeeperReject(H225GatekeeperReject *psGatekeeperReject)
{
   if(geCurGkMode == RasUseSpecificGatekeeper)
   {
      geRasStatus = RasGRQRejected;
      switch(psGatekeeperReject->rejectReason.t)
      {
      case T_H225GatekeeperRejectReason_resourceUnavailable:
         OOTRACEERR1("Error: Gatekeeper Reject - Resource Unavailable\n");
         break;
      case T_H225GatekeeperRejectReason_terminalExcluded:
         OOTRACEERR1("Gatekeeper Reject - Terminal Excluded\n");
         break;
      case T_H225GatekeeperRejectReason_invalidRevision:
         OOTRACEERR1("Gatekeeper Reject - Invalid Revision\n");
         break;
      case T_H225GatekeeperRejectReason_undefinedReason:
         OOTRACEERR1("Gatekeeper Reject - Undefined Reason\n");
         break;
      case T_H225GatekeeperRejectReason_securityDenial:
         OOTRACEERR1("Gatekeeper Reject - Security Denial\n");
         break;
      case T_H225GatekeeperRejectReason_genericDataReason:
         OOTRACEERR1("Gatekeeper Reject - Generic Data Reason\n");
         break;
      case T_H225GatekeeperRejectReason_neededFeatureNotSupported:
         OOTRACEERR1("Gatekeeper Reject - Needed Feature Not Supported\n");
         break;
      case T_H225GatekeeperRejectReason_securityError:
         OOTRACEERR1("Gatekeeper Reject - Security Error\n");
         break;
      default:
         OOTRACEERR1("Gatekeeper Reject - Invalid reason\n");
      }
      return OO_FAILED;
   }
   OOTRACEDBGB1("Gatekeeper Reject received\n");
   return OO_OK;

}

static int ooRasManageGatekeeperConfirm
                           ( H225GatekeeperConfirm *psGatekeeperConfirm )
{
   int iRet=0;
   H225TransportAddress_ipAddress *psRasAddress;
   if(giDiscoveryComplete)
   {
      OOTRACEDBGB1("Ignoring GKConfirm as Gatekeeper has been discovered\n");
      return OO_OK;
   }

   if(psGatekeeperConfirm->m.gatekeeperIdentifierPresent)
   {
      if(psGatekeeperConfirm->gatekeeperIdentifier.nchars > (int)OORAS_MAX_ID_LENGTH)
         sGatekeeperId.nchars = OORAS_MAX_ID_LENGTH;
      else
         sGatekeeperId.nchars = psGatekeeperConfirm->gatekeeperIdentifier.nchars;
      memcpy(ausGatekeeperId,
               psGatekeeperConfirm->gatekeeperIdentifier.data,
               sizeof(ASN116BITCHAR)*sGatekeeperId.nchars );
      sGatekeeperId.data = ausGatekeeperId;
   }
   else{
      OOTRACEERR1("ERROR:No Gatekeeper ID present in received GKConfirmed message\n");
      return OO_FAILED;
   }
  
   /* Extract Gatekeeper's RAS address */
   if(psGatekeeperConfirm->rasAddress.t != T_H225TransportAddress_ipAddress)
   {
      OOTRACEERR1("ERROR:Unsupported RAS address type in received GkConfirm\n");
      return OO_FAILED;
   }
   psRasAddress =   psGatekeeperConfirm->rasAddress.u.ipAddress;
   sprintf(gGkRasIP, "%d.%d.%d.%d", psRasAddress->ip.data[0],
                                    psRasAddress->ip.data[1],
                                    psRasAddress->ip.data[2],
                                    psRasAddress->ip.data[3]);
   gGkRasPort = psRasAddress->port;
  
   giDiscoveryComplete = 1;
   OOTRACEINFO1("Gatekeeper Confirmed\n");
   iRet = ooRasSendRRQ(0);
   if(iRet != OO_OK)
   {
      OOTRACEERR1("Error:Failed to send initial RRQ\n");
   }
   return OO_OK;
}

/**
 * Send RRQ.
 */

static int ooRasSendRRQ(int keepAlive )
{
    int iRet;
    /*   int iEk, iDon, iTeen, iChaar;*/
   ooRasMessage *psRasMsg=NULL;
   H225RegistrationRequest *psRegReq=NULL;
   OOCTXT *psCtxt=NULL;
   H225TransportAddress *psTransportAddress=NULL;
   H225TransportAddress_ipAddress *psIpAddress=NULL;
   /*   char hexip[20];*/

   iRet = ooCreateRasMessage(&psRasMsg);
   if(!psRasMsg)
   {
      OOTRACEERR1("Error: Memory allocation for RRQ RAS message failed\n");
      return OO_FAILED;
   }
   psCtxt = psRasMsg->psContext;
   psRegReq = (H225RegistrationRequest*)ASN1MALLOC(psCtxt,
                                          sizeof(H225RegistrationRequest));
   if(!psRegReq)
   {
      OOTRACEERR1("Error:Memory allocation for RRQ failed\n");
      ooFreeRasMessage(psRasMsg);
      return OO_FAILED;
   }
   memset(psRegReq, 0, sizeof(H225RegistrationRequest));
   psRasMsg->sMessage.t = T_H225RasMessage_registrationRequest;
   psRasMsg->sMessage.u.registrationRequest = psRegReq;
  
   psRegReq->protocolIdentifier = gProtocolID;
   psRegReq->m.nonStandardDataPresent=0;
   /* Populate CallSignal Address List*/
   psTransportAddress = (H225TransportAddress*) ASN1MALLOC(psCtxt,
                                                 sizeof(H225TransportAddress));
   psIpAddress = (H225TransportAddress_ipAddress*) ASN1MALLOC(psCtxt,
                                        sizeof(H225TransportAddress_ipAddress));
   if(!psTransportAddress || !psIpAddress)
   {
      OOTRACEERR1("Error:Failed to allocate memory for signalling address of "
                  "RRQ message\n");
      ooFreeRasMessage(psRasMsg);
      return OO_FAILED;
   }
   memset(psTransportAddress, 0, sizeof(H225TransportAddress));
   memset(psIpAddress, 0, sizeof(H225TransportAddress_ipAddress));
   psTransportAddress->t = T_H225TransportAddress_ipAddress;
   psTransportAddress->u.ipAddress = psIpAddress;
   ooConvertIpToNwAddr(gH323ep.signallingIP, psIpAddress->ip.data);
#if 0
   sscanf(gH323ep.signallingIP, "%d.%d.%d.%d", &iEk, &iDon, &iTeen, &iChaar);
   sprintf(hexip, "%x %x %x %x", iEk, iDon, iTeen, iChaar);
   sscanf(hexip, "%x %x %x %x", &(psIpAddress->ip.data[0]),
                                &(psIpAddress->ip.data[1]),
                                &(psIpAddress->ip.data[2]),
                                &(psIpAddress->ip.data[3]));  
#endif
   psIpAddress->ip.numocts = 4;
   psIpAddress->port = gH323ep.listenPort;
  
   dListInit(&psRegReq->callSignalAddress);
   dListAppend(psCtxt, &psRegReq->callSignalAddress,
                                       (void*)psTransportAddress);

   /* Populate RAS Address List*/
   psTransportAddress = NULL;
   psIpAddress = NULL;
   psTransportAddress = (H225TransportAddress*) ASN1MALLOC(psCtxt,
                                                 sizeof(H225TransportAddress));
   psIpAddress = (H225TransportAddress_ipAddress*) ASN1MALLOC(psCtxt,
                                       sizeof(H225TransportAddress_ipAddress));
   if(!psTransportAddress || !psIpAddress)
   {
      OOTRACEERR1("Error:Failed to allocate memory for RAS address of "
                  "RRQ message\n");
      ooFreeRasMessage(psRasMsg);
      return OO_FAILED;
   }
   memset(psTransportAddress, 0, sizeof(H225TransportAddress));
   memset(psIpAddress, 0, sizeof(H225TransportAddress_ipAddress));
   psTransportAddress->t = T_H225TransportAddress_ipAddress;
   psTransportAddress->u.ipAddress = psIpAddress;
  
   ooConvertIpToNwAddr(gH323ep.signallingIP, psIpAddress->ip.data);
#if 0
   sscanf(gH323ep.signallingIP, "%d.%d.%d.%d", &iEk, &iDon, &iTeen, &iChaar);
   sprintf(hexip, "%x %x %x %x", iEk, iDon, iTeen, iChaar);
   sscanf(hexip, "%x %x %x %x", &(psIpAddress->ip.data[0]),
                                &(psIpAddress->ip.data[1]),
                                &(psIpAddress->ip.data[2]),
                                &(psIpAddress->ip.data[3]));  
#endif
   psIpAddress->ip.numocts = 4;
   psIpAddress->port = gLocalRASPort;
  
   dListInit(&psRegReq->rasAddress);
   dListAppend(psCtxt, &psRegReq->rasAddress,
                                       (void*)psTransportAddress);
  
   psRegReq->terminalType.m.vendorPresent=1;
   ooRasFillVendor( &psRegReq->terminalType.vendor );
 
   psRegReq->m.terminalAliasPresent=1;
   if(OO_OK != ooRasFillAlias( psRasMsg->psContext, gH323ep.aliases, &psRegReq->terminalAlias))
   {
      OOTRACEERR1("Error filling alias for RRQ\n");
      ooFreeRasMessage( psRasMsg );
      return OO_FAILED;
   }
  
   psRegReq->m.gatekeeperIdentifierPresent=1;
   psRegReq->gatekeeperIdentifier.nchars = sGatekeeperId.nchars;
   psRegReq->gatekeeperIdentifier.data = (ASN116BITCHAR*)ASN1MALLOC
                          (psCtxt, sGatekeeperId.nchars*sizeof(ASN116BITCHAR));
   if(!psRegReq->gatekeeperIdentifier.data)
   {
      OOTRACEERR1("Error: Failed to allocate memory for GKIdentifier in RRQ.\n");
      ooFreeRasMessage(psRasMsg);
      return OO_FAILED;
   }
   memcpy(psRegReq->gatekeeperIdentifier.data, ausGatekeeperId,
                                sGatekeeperId.nchars*sizeof(ASN116BITCHAR));
  
   ooRasFillVendor( &psRegReq->endpointVendor );
  
   psRegReq->m.willSupplyUUIEsPresent=1;
   psRegReq->willSupplyUUIEs=1;

   psRegReq->requestSeqNum = gusRequestSeqNum++;
   if ( !psRegReq->requestSeqNum )
      psRegReq->requestSeqNum = gusRequestSeqNum++;
  
   psRegReq->discoveryComplete= giDiscoveryComplete;
   psRegReq->m.keepAlivePresent=1;
   psRegReq->keepAlive=keepAlive;
   psRegReq->m.timeToLivePresent = 1;
   psRegReq->timeToLive = DEFAULT_REGISTRATIONTTL; /* Initial value */

   iRet = ooRasSend(psRasMsg);
   if(iRet != OO_OK)
   {
      OOTRACEERR1("Error: Failed to send RRQ message\n");
      return OO_FAILED;
   }
   if(geRasStatus == RasRRQSent || geRasStatus == RasRegisteredRRQ)
      guiRRQRetries++;
   if(!keepAlive)
      geRasStatus = RasRRQSent;
   else
      geRasStatus = RasRegisteredRRQ;

   ooGetTimeOfDay(&sLastTime, 0); /* Get time stamp */
   return OO_OK;
}



/**
 * Manage incoming RCF message.
 */

static int ooRasManageRegistrationConfirm
   (H225RegistrationConfirm *psRegistrationConfirm)
{
   int i=0;
   DListNode *pNode = NULL;
   H225TransportAddress *psCallSigAddr=NULL;
   /* Extract Endpoint Id */
   if(psRegistrationConfirm->endpointIdentifier.nchars > OORAS_MAX_ID_LENGTH)
      sEndpointId.nchars = OORAS_MAX_ID_LENGTH;
   else
      sEndpointId.nchars = psRegistrationConfirm->endpointIdentifier.nchars;
  
   memcpy(ausEndpointId,
            psRegistrationConfirm->endpointIdentifier.data,
            sizeof(ASN116BITCHAR)*sEndpointId.nchars );
  
   sEndpointId.data = ausEndpointId;

   /* Extract CallSignalling Address */
   for(i=0; i<(int)psRegistrationConfirm->callSignalAddress.count; i++)
   {
      pNode = dListFindByIndex(&psRegistrationConfirm->callSignalAddress, i);
      if(!pNode)
      {
         OOTRACEERR1("Error:Invalid Registration confirmed message\n");
         return OO_FAILED;
      }
      psCallSigAddr = (H225TransportAddress*)pNode->data;
      if(psCallSigAddr->t != T_H225TransportAddress_ipAddress)
         continue;
      sprintf(gGkCallSignallingIP, "%d.%d.%d.%d",
                            psCallSigAddr->u.ipAddress->ip.data[0],
                            psCallSigAddr->u.ipAddress->ip.data[1],
                            psCallSigAddr->u.ipAddress->ip.data[2],
                            psCallSigAddr->u.ipAddress->ip.data[3]);
      gGkCallSignallingPort = psCallSigAddr->u.ipAddress->port;
   }
   /* Is keepAlive supported */
   if(psRegistrationConfirm->m.timeToLivePresent)
   {
      guiRegistrationTTL = psRegistrationConfirm->timeToLive;
      OOTRACEINFO2("Gatekeeper supports KeepAlive, Registration TTL is %d\n",
                    psRegistrationConfirm->timeToLive);
   }else{
      guiRegistrationTTL = 0;
      OOTRACEINFO1("Gatekeeper does not support KeepAlive.\n");
   }
   /* Extract Pre-Granted ARQ */
   if(psRegistrationConfirm->m.preGrantedARQPresent)
   {
      memcpy(&gGkInfo.preGrantedARQ, &psRegistrationConfirm->preGrantedARQ,
             sizeof(H225RegistrationConfirm_preGrantedARQ));
   }
   /* sLastTime is the time when last message was sent,
      in this case RRQ, is the actual RegistrationTime.
   */
   gsRegistrationTime.tv_sec = sLastTime.tv_sec;
   gsRegistrationTime.tv_usec = sLastTime.tv_usec;

   geRasStatus=RasRegistered;
   return OO_OK;
}

static int ooRasManageRegistrationReject
   (H225RegistrationReject *psRegistrationReject)
{
   int iRet=0;
   switch(psRegistrationReject->rejectReason.t)
   {
   case T_H225RegistrationRejectReason_discoveryRequired:
      OOTRACEINFO1("RRQ Rejected - Discovery Required\n");
      giDiscoveryComplete = 0;
      guiRRQRetries = 0;
      iRet = ooRasSendGRQ();
      if(iRet == OO_OK)
         geRasStatus=RasGRQSent;
      else
         geRasStatus=RasIdle;
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
      iRet = ooRasSendRRQ(0); /* No keepAlive */
      if(iRet != OO_OK)
         OOTRACEERR1("\nError: Full Registration transmission failed\n");
      else
         return OO_OK;
      break;
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
   geRasStatus = RasRRQRejected;
   return OO_OK;
}


/*TODO: If the unregister request has list of aliases,
        only those aliases are unregistered. Right now we assume
        complete unregistration and start again.
        We also need to send UnRegistrationConfirm
*/
static int ooRasManageUnregistrationRequest
   (H225UnregistrationRequest * psunregistrationRequest)
{
   int iRet=0;
   /* Send a fresh Registration request and if that fails, go back to
      Gatekeeper discovery.
   */
   OOTRACEINFO1("Sending fresh RRQ - as unregistration request received\n");
   guiRRQRetries = 0;
   geRasStatus = RasGRQSent;
   iRet = ooRasSendRRQ(0);
   if(iRet != OO_OK)
   {
      OOTRACEERR1("Error: Failed to send RRQ message - Rediscovering Gatekeeper\n");
      giDiscoveryComplete = 0;
      geRasStatus = RasIdle;
      guiRRQRetries = 0;
   }
   return OO_OK;
}


int ooRasSendAdmissionRequest(ooCallData * call, enum RasCallModel eModel,
   ooAliases *psSrcAliases, ooAliases *psDestAliases)
{
   int iRet = 0;
   ooRasMessage *psRasMsg=NULL;
   OOCTXT* psCtxt;
   H225AdmissionRequest *psAdmReq=NULL;
   H225TransportAddress_ipAddress *psIpAddressLocal =NULL, *psIpAddressRemote=NULL;
   RasCallAdmissionInfo *psCallAdmInfo=NULL;
   /*   char hexip[20];
        int iEk, iDon, iTeen, iChaar;*/

   OOTRACEINFO1("Sending Admission Request for new call\n");

   iRet = ooCreateRasMessage(&psRasMsg);
   if(!psRasMsg)
   {
      OOTRACEERR1("Error: Memory allocation for ARQ RAS message failed\n");
      return OO_FAILED;
   }
   psCtxt = psRasMsg->psContext;
   psRasMsg->sMessage.t = T_H225RasMessage_admissionRequest;
   psAdmReq = (H225AdmissionRequest*) ASN1MALLOC(psCtxt,
                                                 sizeof(H225AdmissionRequest));
   if(!psAdmReq)
   {
      OOTRACEERR1("Error: Failed to allocate memory for Admission Request.\n");
      ooFreeRasMessage(psRasMsg);
      return OO_FAILED;
   }
   memset(psAdmReq, 0, sizeof(H225AdmissionRequest));
   psRasMsg->sMessage.u.admissionRequest = psAdmReq;
  
   /* Populate call signalling addresses */
   psIpAddressLocal = (H225TransportAddress_ipAddress*)ASN1MALLOC(psCtxt,
                                     sizeof(H225TransportAddress_ipAddress));
   if(strlen(call->remoteIP))
      psIpAddressRemote = (H225TransportAddress_ipAddress*)ASN1MALLOC(psCtxt,
                                      sizeof(H225TransportAddress_ipAddress));

   if(!psIpAddressLocal || (strlen(call->remoteIP) && (!psIpAddressRemote)))
   {
      OOTRACEERR1("Error:Failed to allocate memory for Call Signalling "
                  "Addresses of ARQ message\n");
      ooFreeRasMessage(psRasMsg);
      return OO_FAILED;
   }
   ooConvertIpToNwAddr(gH323ep.signallingIP, psIpAddressLocal->ip.data);
#if 0
   sscanf(gH323ep.signallingIP, "%d.%d.%d.%d", &iEk, &iDon, &iTeen, &iChaar);
   sprintf(hexip, "%x %x %x %x", iEk, iDon, iTeen, iChaar);
   sscanf(hexip, "%x %x %x %x", &(psIpAddressLocal->ip.data[0]),
                                &(psIpAddressLocal->ip.data[1]),
                                &(psIpAddressLocal->ip.data[2]),
                                &(psIpAddressLocal->ip.data[3]));
#endif
   psIpAddressLocal->ip.numocts = 4;
   psIpAddressLocal->port = gH323ep.listenPort;

   if(strlen(call->remoteIP))
   {
      ooConvertIpToNwAddr(call->remoteIP, psIpAddressRemote->ip.data);
#if 0
      sscanf(call->remoteIP, "%d.%d.%d.%d", &iEk, &iDon, &iTeen, &iChaar);
      sprintf(hexip, "%x %x %x %x", iEk, iDon, iTeen, iChaar);
      sscanf(hexip, "%x %x %x %x", &(psIpAddressRemote->ip.data[0]),
                                &(psIpAddressRemote->ip.data[1]),
                                &(psIpAddressRemote->ip.data[2]),
                                &(psIpAddressRemote->ip.data[3]));
#endif
      psIpAddressRemote->ip.numocts = 4;
      psIpAddressRemote->port = call->remotePort;
   }

   if(!strcmp(call->callType, "incoming"))
   {
      psAdmReq->m.destCallSignalAddressPresent = TRUE;
      psAdmReq->destCallSignalAddress.t = T_H225TransportAddress_ipAddress;
      psAdmReq->destCallSignalAddress.u.ipAddress = psIpAddressLocal;
      if(strlen(call->remoteIP))
      {
         psAdmReq->m.srcCallSignalAddressPresent = TRUE;
         psAdmReq->srcCallSignalAddress.t = T_H225TransportAddress_ipAddress;
         psAdmReq->srcCallSignalAddress.u.ipAddress = psIpAddressRemote;
      }
   }else {
      psAdmReq->m.srcCallSignalAddressPresent = TRUE;
      psAdmReq->srcCallSignalAddress.t = T_H225TransportAddress_ipAddress;
      psAdmReq->srcCallSignalAddress.u.ipAddress = psIpAddressLocal;
      if(strlen(call->remoteIP))
      {
         psAdmReq->m.destCallSignalAddressPresent = TRUE;
         psAdmReq->destCallSignalAddress.t = T_H225TransportAddress_ipAddress;
         psAdmReq->destCallSignalAddress.u.ipAddress = psIpAddressRemote;
      }
   }

   /* Populate seq number */
   psAdmReq->requestSeqNum = gusRequestSeqNum++;
   if(!psAdmReq->requestSeqNum)
      psAdmReq->requestSeqNum = gusRequestSeqNum++;

   /* Populate call type - For now only PointToPoint supported*/
   psAdmReq->callType.t = T_H225CallType_pointToPoint;
  
   /* Add call model to message*/
   psAdmReq->m.callModelPresent = 1;
   if(eModel == RasGkRouted)
      psAdmReq->callModel.t = T_H225CallModel_gatekeeperRouted;
   else
      psAdmReq->callModel.t = T_H225CallModel_direct;

   /* Populate Endpoint Identifier */
   psAdmReq->endpointIdentifier.nchars = sEndpointId.nchars;
   psAdmReq->endpointIdentifier.data = (ASN116BITCHAR*)ASN1MALLOC(psCtxt,
                                      sizeof(ASN116BITCHAR)*sEndpointId.nchars);
   if(!psAdmReq->endpointIdentifier.data)
   {
      OOTRACEERR1("Error: Failed to allocate memory for EndPoint Id in ARQ "
                  "message.\n");
      ooFreeRasMessage(psRasMsg);
      return OO_FAILED;
   }
   memcpy((void*)psAdmReq->endpointIdentifier.data, (void*)ausEndpointId,
                                      sizeof(ASN116BITCHAR)*sEndpointId.nchars);
   /* Populate Destination Info -  */
   if(psDestAliases)
   {
      psAdmReq->m.destinationInfoPresent = 1;
      ooRasFillAlias(psCtxt, psDestAliases, &psAdmReq->destinationInfo);
   }
   /* Populate Source Info */
   ooRasFillAlias(psCtxt, psSrcAliases, &psAdmReq->srcInfo);
  
   /* Populate bandwidth*/
   psAdmReq->bandWidth = 100000;
   /* Populate call Reference */
   psAdmReq->callReferenceValue = call->callReference;
  
   /* populate conferenceID */
   memcpy((void*)&psAdmReq->conferenceID, (void*)&call->confIdentifier,
                                         sizeof(H225ConferenceIdentifier));
   /*populate answerCall */
   if(!strcmp(call->callType, "incoming"))
      psAdmReq->answerCall = TRUE;
   else
      psAdmReq->answerCall = FALSE;

   /* Populate CanMapAlias */
   psAdmReq->m.canMapAliasPresent = TRUE;
   psAdmReq->canMapAlias = FALSE;

   /* Populate call identifier */
   psAdmReq->m.callIdentifierPresent = TRUE;
   memcpy((void*)&psAdmReq->callIdentifier, (void*)&call->callIdentifier,
                                             sizeof(H225CallIdentifier));

   /* Populate Gatekeeper Id */
   psAdmReq->m.gatekeeperIdentifierPresent = TRUE;
   psAdmReq->gatekeeperIdentifier.nchars = sGatekeeperId.nchars;
   psAdmReq->gatekeeperIdentifier.data = (ASN116BITCHAR*)ASN1MALLOC(psCtxt,
                                  sizeof(ASN116BITCHAR)*sGatekeeperId.nchars);
   if(!psAdmReq->gatekeeperIdentifier.data)
   {
      OOTRACEERR1("Error:Failed to allocate memory for GK ID of ARQ message\n");
      ooFreeRasMessage(psRasMsg);
      return OO_FAILED;
   }
   memcpy((void*)psAdmReq->gatekeeperIdentifier.data, (void*)ausGatekeeperId,
                                   sizeof(ASN116BITCHAR)*sGatekeeperId.nchars);

   psAdmReq->m.willSupplyUUIEsPresent = 1;
   psAdmReq->willSupplyUUIEs = TRUE;

   /* Create RasCallAdmissionInfo */
   psCallAdmInfo = (RasCallAdmissionInfo*)ASN1MALLOC(&gsRASCTXT,
                                                 sizeof(RasCallAdmissionInfo));
   if(!psCallAdmInfo)
   {
      OOTRACEERR1("Error: Failed to allocate memory for new CallAdmission"
                  " Info entry\n");
      ooFreeRasMessage(psRasMsg);
      return OO_FAILED;
   }
   memset((void*)psCallAdmInfo, 0, sizeof(RasCallAdmissionInfo));

   psCallAdmInfo->call = call;
   psCallAdmInfo->ucStatus = RasCallARQSent;
   ooGetTimeOfDay(&psCallAdmInfo->sLastTime, 0);
   psCallAdmInfo->uiRetries = 0;
   psCallAdmInfo->usRequestSeqNum = psAdmReq->requestSeqNum;
   psCallAdmInfo->eCallModel = eModel;
   dListAppend(&gsRASCTXT, &gsCallsPendingList, psCallAdmInfo);
  
   iRet = ooRasSend(psRasMsg);
   if(iRet != OO_OK)
   {
      OOTRACEERR1("Error:Failed to send AdmissionRequest message\n");
      return OO_FAILED;
   }
   return OO_OK;
}

/**
 * Manage incoming ACF message.
 */

static int ooRasManageAdmissionConfirm( H225AdmissionConfirm *psAdmissionConfirm )
{
   RasCallAdmissionInfo* psCallAdmInfo=NULL;
   unsigned int uiAux;
   DListNode *psNode;
   H225TransportAddress_ipAddress * ipAddress=NULL;
   /* Search call in pending calls list */
   psNode=(DListNode*)gsCallsPendingList.head;
   for( uiAux=0 ; uiAux<gsCallsPendingList.count ; uiAux++ )
   {
      psCallAdmInfo = (RasCallAdmissionInfo*) psNode->data;
      if(psCallAdmInfo->usRequestSeqNum == psAdmissionConfirm->requestSeqNum)
      {
         psCallAdmInfo->ucStatus = RasCallACFRecv;
         /* Populate Remote IP */
         if(psAdmissionConfirm->destCallSignalAddress.t !=
                                      T_H225TransportAddress_ipAddress)
         {
            OOTRACEERR1("Error:Destination Call Signal Address provided by"
                        "Gatekeeper is not an IPv4 address\n");
            return OO_FAILED;
         }
         ipAddress = psAdmissionConfirm->destCallSignalAddress.u.ipAddress;
         sprintf(psCallAdmInfo->call->remoteIP, "%d.%d.%d.%d", ipAddress->ip.data[0],
                                                ipAddress->ip.data[1],
                                                ipAddress->ip.data[2],
                                                ipAddress->ip.data[3]);
         psCallAdmInfo->call->remotePort = ipAddress->port;
         OO_SETFLAG (psCallAdmInfo->call->flags, OO_M_GKENGAGED);        
         ooH323CallAdmitted( psCallAdmInfo->call);
         dListRemove( &gsCallsPendingList, psNode );
         dListAppend(&gsRASCTXT, &gsCallsAdmittedList, psNode->data);
         ASN1MEMFREEPTR(&gsRASCTXT, psNode);
         return OO_OK;
         break;
      }
      else
      {
         psNode = psNode->next;
      }
   }
   OOTRACEERR1("Error: Failed to process ACF as there is no corresponding "
               "pending call\n");
   return OO_FAILED;
}



/**
 * This function is invoked to request call disengage to gatekeeper.
 *
 * @param   szCallToken    Call token.    
 *
 * @return  Completion status - 0 on success, -1 on failure
 */

int ooRasSendDisengageRequest(ooCallData *call)
{
   int iRet = 0;  
   unsigned int uiAux;
   ooRasMessage *psRasMsg=NULL;
   OOCTXT *psCtxt = NULL;
   DListNode *psNode = NULL;
   H225DisengageRequest * psDRQ = NULL;
   RasCallAdmissionInfo* psCallAdmInfo=NULL;
   if (!OO_TESTFLAG (call->flags, OO_M_GKENGAGED))
   {
      OOTRACEDBGC3("No need to send Disengage request as gatekeeper is not "
                   "engaged. (%s, %s)\n", call->callType, call->callToken);
      return OO_OK;
   }
   if(!ooRasIsRegistered())
   {
      OOTRACEERR1("Error:Disengage can't be sent as endpoint is not"
                  "registered.\n");
      return OO_FAILED;
   }
  
   OOTRACEINFO3("Sending disengage Request for  call. (%s, %s)\n",
                 call->callType, call->callToken);

   iRet = ooCreateRasMessage(&psRasMsg);
   if(!psRasMsg)
   {
      OOTRACEERR1("Error: Memory allocation for DRQ RAS message failed\n");
      return OO_FAILED;
   }
   psCtxt = psRasMsg->psContext;
   psRasMsg->sMessage.t = T_H225RasMessage_disengageRequest;
   psDRQ = (H225DisengageRequest*) ASN1MALLOC(psCtxt,
                                               sizeof(H225DisengageRequest));
   if(!psDRQ)
   {
      OOTRACEERR1("Error: Failed to allocate memory for DRQ message\n");
      ooFreeRasMessage(psRasMsg);
      return OO_FAILED;
   }

   memset(psDRQ, 0, sizeof(H225DisengageRequest));
   psRasMsg->sMessage.u.disengageRequest = psDRQ;
  
   psDRQ->requestSeqNum = gusRequestSeqNum++;
   if (!psDRQ->requestSeqNum )
      psDRQ->requestSeqNum = gusRequestSeqNum++;
  
  
   psDRQ->endpointIdentifier.nchars = sEndpointId.nchars;
   psDRQ->endpointIdentifier.data = (ASN116BITCHAR*)ASN1MALLOC(psCtxt,
                                     sizeof(ASN116BITCHAR)*sEndpointId.nchars);
   if(!psDRQ->endpointIdentifier.data)
   {
      OOTRACEERR1("Error: Failed to allocate memory for EndPoint Id in DRQ "
                  "message.\n");
      ooFreeRasMessage(psRasMsg);
      return OO_FAILED;
   }
   memcpy((void*)psDRQ->endpointIdentifier.data, (void*)ausEndpointId,
                                      sizeof(ASN116BITCHAR)*sEndpointId.nchars);

   memcpy((void*)&psDRQ->conferenceID, (void*)&call->confIdentifier,
                                         sizeof(H225ConferenceIdentifier));

   psDRQ->callReferenceValue = call->callReference;
  
   psDRQ->disengageReason.t = T_H225DisengageReason_normalDrop;

   psDRQ->m.answeredCallPresent = 1;
   if(!strcmp(call->callType, "incoming"))
      psDRQ->answeredCall = 1;
   else
      psDRQ->answeredCall = 0;

   psDRQ->m.callIdentifierPresent = 1;
   memcpy((void*)&psDRQ->callIdentifier, (void*)&call->callIdentifier,
                                             sizeof(H225CallIdentifier));
   psDRQ->m.gatekeeperIdentifierPresent = 1;
   psDRQ->gatekeeperIdentifier.nchars = sGatekeeperId.nchars;
   psDRQ->gatekeeperIdentifier.data = (ASN116BITCHAR*)ASN1MALLOC
                          (psCtxt, sGatekeeperId.nchars*sizeof(ASN116BITCHAR));
   if(!psDRQ->gatekeeperIdentifier.data)
   {
      OOTRACEERR1("Error: Failed to allocate memory for GKIdentifier in DRQ.\n");
      ooFreeRasMessage(psRasMsg);
      return OO_FAILED;
   }
   memcpy(psDRQ->gatekeeperIdentifier.data, ausGatekeeperId,
                                sGatekeeperId.nchars*sizeof(ASN116BITCHAR));

   psDRQ->m.terminationCausePresent = 1;
   psDRQ->terminationCause.t = T_H225CallTerminationCause_releaseCompleteCauseIE;
   psDRQ->terminationCause.u.releaseCompleteCauseIE =
      (H225CallTerminationCause_releaseCompleteCauseIE*)ASN1MALLOC(psCtxt,
      sizeof(H225CallTerminationCause_releaseCompleteCauseIE));
   if(!psDRQ->terminationCause.u.releaseCompleteCauseIE)
   {
      OOTRACEERR1("Error: Failed to allocate memory for cause ie in DRQ.\n");
      ooFreeRasMessage(psRasMsg);
      return OO_FAILED;
   }
   psDRQ->terminationCause.u.releaseCompleteCauseIE->numocts =
                                                         strlen("Call Ended");
   strcpy(psDRQ->terminationCause.u.releaseCompleteCauseIE->data, "Call Ended");

   iRet = ooRasSend(psRasMsg);
   if(iRet != OO_OK)
   {
      OOTRACEERR1("Error: Failed to send DRQ message\n");
   }
  
   OO_CLRFLAG (call->flags, OO_M_GKENGAGED);

   /* Search call in admitted calls list */
   psNode=(DListNode*)gsCallsAdmittedList.head;
   for( uiAux=0 ; uiAux<gsCallsPendingList.count ; uiAux++ )
   {
      psCallAdmInfo = (RasCallAdmissionInfo*) psNode->data;
      if(psCallAdmInfo->call->callReference == call->callReference)
      {
         dListRemove( &gsCallsPendingList, psNode );
         ASN1MEMFREEPTR(&gsRASCTXT, psNode->data);
         ASN1MEMFREEPTR(&gsRASCTXT, psNode);
         break;
      }
   }
   return iRet;
}    

static void ooRasManageDisengageConfirm(H225DisengageConfirm * psDCF)
{
   OOTRACEINFO1("Received disengage confirm\n");
}
  
  
  
