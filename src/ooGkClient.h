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
 * @file ooGkClient.h
 * This file contains functions to support RAS protocol.
 *
 *
 */


#ifndef _OOGKCLIENT_H_
#define _OOGKCLIENT_H_

#include "ooasn1.h"
#include "ootypes.h"
#include "H323-MESSAGES.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef EXTERN
#ifdef _WIN32
#define EXTERN __declspec(dllexport)
#else
#define EXTERN
#endif /* _WIN32 */
#endif /* EXTERN */


/*-------------------------------------------------------------------*/
/*  Public definitions                                               */
/*-------------------------------------------------------------------*/



#define MAX_IP_LEN 15
#define DEFAULT_GKPORT 1719
#define MULTICAST_GKADDRESS "224.0.1.41"
#define MULTICAST_GKPORT 1718
#define DEFAULT_BW_REQUEST  100000

/* Various timeouts in seconds */
#define DEFAULT_REG_TTL 300
#define DEFAULT_TTL_OFFSET 20
#define DEFAULT_ARQ_TIMEOUT 5
#define DEFAULT_DRQ_TIMEOUT 5
#define DEFAULT_GRQ_TIMEOUT 15
#define DEFAULT_RRQ_TIMEOUT 10

/* Number of retries before giving up */
#define OO_MAX_GRQ_RETRIES 3
#define OO_MAX_RRQ_RETRIES 3
#define OO_MAX_ARQ_RETRIES 3

/* Gk Client timers */
#define OO_GRQ_TIMER (1<<0)
#define OO_RRQ_TIMER (1<<1)
#define OO_REG_TIMER (1<<2)
#define OO_ARQ_TIMER (1<<3)
#define OO_DRQ_TIMER (1<<4)

/**
 * @defgroup ras RAS Channel and Message handling
 * @{
 */

struct ooGkClient;
struct RasCallAdmissionInfo;

typedef struct ooGkClientTimerCb{
   int timerType;
   struct ooGkClient *pGkClient;
   struct RasCallAdmissionInfo *pAdmInfo;
}ooGkClientTimerCb;

enum RasGatekeeperMode {
   RasNoGatekeeper = 0,
   RasDiscoverGatekeeper = 1,
   RasUseSpecificGatekeeper = 2,
};

enum RasCallType{
   RasPointToPoint =0,
   RasOneToN,
   RasnToOne,
   RasnToN,
};


enum OOGkClientState {
   GkClientIdle = 0,
   GkClientDiscovered, /* Gk Discovery is complete */
   GkClientRegistered, /* registered with gk */
   GkClientGkErr,/*Gk is not responding, in discover mode can look for new GK*/
   GkClientFailed
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
   unsigned int retries;
   unsigned short requestSeqNum;
   ASN1USINT irrFrequency;
} RasCallAdmissionInfo;




typedef struct ooGkClient{
   ASN1BOOL discoveryComplete;
   OOCTXT ctxt;
   OOCTXT msgCtxt;
   OOSOCKET rasSocket;
   int localRASPort;
   char gkRasIP[20];
   char gkCallSignallingIP[20];
   RasGatekeeperInfo gkInfo;
   int gkRasPort;
   int gkCallSignallingPort;
   unsigned short requestSeqNum;
   enum RasGatekeeperMode gkMode; /* Current Gk mode */
   struct timeval registrationTime;
   H225GatekeeperIdentifier gkId;
   H225EndpointIdentifier endpointId;
   DList callsPendingList;
   DList callsAdmittedList;
   DList timerList;
   ASN1UINT grqRetries;
   ASN1UINT rrqRetries;
   ASN1UINT grqTimeout;
   ASN1UINT rrqTimeout;
   ASN1UINT regTimeout;
   ASN1UINT arqTimeout;
   ASN1UINT drqTimeout;
   enum OOGkClientState  state;
} ooGkClient;



/**
 * This function is used to initialize the Gatekeeper client.
 * @param eGkMode          Gatekeeper mode.
 * @param szGkAddr         Dotted gk ip address, if gk has to be specified.
 * @param iGkPort          Gk port.
 *
 * @return                 OO_OK, on success. OO_FAILED, on failure.
 *
 */
EXTERN int ooGkClientInit
   (enum RasGatekeeperMode eGkMode, char *szGkAddr, int iGkPort );


EXTERN void ooGkClientPrintConfig(ooGkClient *pGkClient);
/**
 * This function is used to destroy Gatekeeper client. It releases all the
 * associated memory.
 *
 * @return     OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooGkClientDestroy(void);

/**
 * This function is used to start the Gatekeeper client functionality.
 * @param pGkClient        Pointer to the Gatekeeper Client.
 *
 * @return                 OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooGkClientStart(ooGkClient *pGkClient);

/**
 * This function is invoked to set a gatekeeper mode.
 *
 * @param eGkMode  Gatekeeper mode selected. One of the following:
 *                    - RasNoGatekeeper (DEFAULT), No Gatekeeper.             
 *                    - RasDiscoverGatekeeper, to discover a gatekeeper
 *                      automatically.                                        
 *                    - RasUseSpecificGatekeeper, to use a specific gatekeeper.
 * @param szGkAddr  Gatekeeper address (only when using specific gatekeeper).
 * @param iGkPort  Gatekeeper RAS port
 *
 * @return         Completion status - OO_OK on success, OO_FAILED on failure
 */
EXTERN int ooGkClientSetGkMode(ooGkClient *pGkClient,
                               enum RasGatekeeperMode eGkMode, char *szGkAddr,
                               int iGkPort );

/**
 * This function is used to create a RAS channel for the gatekeeper.
 * @param pGkClient     Pointer to the Gatekeeper client for which RAS channel
 *                      has to be created.
 *
 * @return              OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooGkClientCreateChannel(ooGkClient *pGkClient);

/**
 * This function is used to close a RAS channel of the gatekeeper client.
 * @param pGkClient    Pointer to the gatekeeper client.
 *
 * @return             OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooGkClientCloseChannel(ooGkClient *pGkClient);



EXTERN void ooGkClientRasFillVendor
   (ooGkClient *pGkClient, H225VendorIdentifier *psVendor);


/**
 * This function is invoked to receive data on Gatekeeper client's RAS channel.
 *
 * @return         Completion status - OO_OK on success, OO_FAILED on failure
 */
EXTERN int ooGkClientReceive(ooGkClient *pGkClient);


EXTERN int ooGkClientHandleRASMessage
   (ooGkClient *pGkClient, H225RasMessage *pRasMsg);


EXTERN int ooGkClientSendMsg(ooGkClient *pGkClient, H225RasMessage *pRasMsg);



EXTERN int ooGkClientSendGRQ(ooGkClient *pGkClient);



EXTERN int ooGkClientHandleGatekeeperReject
   (ooGkClient *pGkClient, H225GatekeeperReject *pGatekeeperReject);

EXTERN int ooGkClientHandleGatekeeperConfirm
   (ooGkClient *pGkClient, H225GatekeeperConfirm *pGatekeeperConfirm);

EXTERN int ooGkClientSendRRQ(ooGkClient *pGkClient, ASN1BOOL keepAlive);

EXTERN int ooGkClientHandleRegistrationConfirm
   (ooGkClient *pGkClient, H225RegistrationConfirm *pRegistrationConfirm);

EXTERN int ooGkClientHandleRegistrationReject
   (ooGkClient *pGkClient, H225RegistrationReject *pRegistrationReject);

EXTERN int ooGkClientHandleUnregistrationRequest
   (ooGkClient *pGkClient, H225UnregistrationRequest *psunregistrationRequest);

/**
 * This function is invoked to request bandwith admission for a call.
 * @param pGkClient     Gatekeeper client to be used
 * @param call          Handle to the call.
 * @param retransmit    Indicates whether new call or retransmitting for
 *                      existing call.
 *
 * @return              OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooGkClientSendAdmissionRequest
   (ooGkClient *pGkClient, ooCallData *call, ASN1BOOL retransmit);

EXTERN int ooGkClientHandleAdmissionConfirm
   (ooGkClient *pGkClient, H225AdmissionConfirm *psAdmissionConfirm);

/**
 * This function is invoked to request call disengage to gatekeeper.
 * @param pGkClient  Gatekeeper client to be used.
 * @param call       Call Handle
 *
 * @return           Completion status - OO_OK on success, OO_FAILED on failure
 */
EXTERN int ooGkClientSendDisengageRequest
   (ooGkClient *pGkClient, ooCallData *call);

EXTERN int ooGkClientHandleDisengageConfirm
   (ooGkClient *pGkClient, H225DisengageConfirm *psDCF);

EXTERN int ooGkClientRRQTimerExpired(void*pdata);

EXTERN int ooGkClientGRQTimerExpired(void* pdata);

EXTERN int ooGkClientREGTimerExpired(void *pdata);

EXTERN int ooGkClientARQTimerExpired(void* pdata);

EXTERN int ooGkClientCleanCall(ooGkClient *pGkClient, ooCallData *call);
EXTERN int ooGkClientHandleClientOrGkFailure(ooGkClient *pGkClient);
/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __GKCLIENT_H */
