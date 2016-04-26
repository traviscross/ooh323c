/*
 * Copyright (C) 2016 by Seven Du.
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
 * @file ooGkServer.h
 * This file contains functions to support RAS protocol.
 *
 *
 */
#ifndef _OOGKSERVER_H_
#define _OOGKSERVER_H_

#include "ooasn1.h"
#include "ootypes.h"
#include "H323-MESSAGES.h"
#include "ooGkClient.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef EXTERN
#ifdef MAKE_DLL
#define EXTERN __declspec(dllexport)
#else
#define EXTERN
#endif /* _WIN32 */
#endif /* EXTERN */


/*-------------------------------------------------------------------*/
/*  Public definitions                                               */
/*-------------------------------------------------------------------*/


/**
 * @defgroup gkclient Gatekeeper client
 * @{
 */

struct OOH323CallData;
struct ooGkServer;
struct RasCallAdmissionInfo;
struct OOAliases;



/**
 * NOTE- This functionality is not yet fully completed.
 * This is a callback function which is triggered when registration confirm
 * message is received from the gatekeeper. The first parameter is the message
 * received. The second parameter provides updated list of aliases after the
 * message was processed by the stack.
 * @param rcf  Handle to the received registration confirm message
 */
// typedef int (*cb_OnReceivedRegistrationConfirm)
//      (H225RegistrationConfirm *rcf, struct OOAliases *aliases);


/**
 * NOTE- This functionality is not yet fully completed.
 * This is a callback function which is triggered when unregistration confirm
 * message is received. The first parameter is the message received. The second
 * parameter provides updated list of aliases after the message was processed
 * by the stack.
 */
// typedef int (*cb_OnReceivedUnregistrationConfirm)
//      (H225UnregistrationConfirm *ucf, struct OOAliases *aliases);

/**
 * NOTE- This functionality is not yet fully completed.
 * This is a callback function which is triggered when unregistration request
 * message is received. The first parameter is the message received. The second
 * parameter provides the list of aliases requested to be unregistered.
 */
// typedef int (*cb_OnReceivedUnregistrationRequest)
//      (H225UnregistrationRequest *urq, struct OOAliases *aliases);


typedef int (*cb_OnReceivedRegistrationRequest)
     (struct ooGkServer *pGkServer, H225RegistrationRequest *req, struct OOAliases *aliases, const char *remoteHost, int remotePort);

typedef struct OOGKSERVERCALLBACKS{
   cb_OnReceivedRegistrationRequest onReceivedRegistrationRequest;
   cb_OnReceivedUnregistrationConfirm onReceivedUnregistrationConfirm;
   cb_OnReceivedUnregistrationRequest onReceivedUnregistrationRequest;
}OOGKSERVERCALLBACKS;

/**
 * Structure to store all the configuration information for the gatekeeper
 * client. Gatekeeper client is responsible for all the communication with
 * a gatekeeper.
 */
typedef struct ooGkServer{
   ASN1BOOL discoveryComplete;
   OOCTXT ctxt;
   OOCTXT msgCtxt;
   OOSOCKET rasSocket;
   int localRASPort;
   char localRASIP[20];
   char gkRasIP[20];
   char gkCallSignallingIP[20];
   // RasGatekeeperInfo gkInfo;
   int gkRasPort;
   int gkCallSignallingPort;
   unsigned short requestSeqNum;
   struct timeval registrationTime;
   H225GatekeeperIdentifier gkId;
   H225EndpointIdentifier endpointId;
   DList callsPendingList;
   DList callsAdmittedList;
   DList timerList;
   OOGKSERVERCALLBACKS callbacks;
   int gkBandwidth;
} ooGkServer;

struct OOAliases;
struct OOH323CallData;

/**
 * This function is used to initialize the Gatekeeper client.If an application
 * wants to use gatekeeper services, it should call this function immediately
 * after initializing the H323 EndPoint.
 * @param eGkMode          Gatekeeper mode.
 * @param szGkAddr         Dotted gk ip address, if gk has to be specified.
 * @param iGkPort          Gk port.
 *
 * @return                 OO_OK, on success. OO_FAILED, on failure.
 *
 */
EXTERN ooGkServer *ooGkServerInit(char *szGkAddr, int iGkPort );

/**
 * This function is used to print the gatekeeper client configuration
 * information to log.
 * @param pGkServer        Handle to gatekeeper client.
 */
EXTERN void ooGkServerPrintConfig(ooGkServer *pGkServer);

/**
 * This function is used to destroy Gatekeeper client. It releases all the
 * associated memory.
 *
 * @return     OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooGkServerDestroy(void);

/**
 * This function is used to start the Gatekeeper client functionality.
 * @param pGkServer        Pointer to the Gatekeeper Client.
 *
 * @return                 OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooGkServerStart(ooGkServer *pGkServer);


int ooGkServerSetGkId (ooGkServer *pGkServer, const char* id);
int ooGkServerSetEpId (ooGkServer *pGkServer, const char* id);

/**
 * This function is used to create a RAS channel for the gatekeeper.
 * @param pGkServer     Pointer to the Gatekeeper client for which RAS channel
 *                      has to be created.
 *
 * @return              OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooGkServerCreateChannel(ooGkServer *pGkServer);

/**
 * This function is used to close a RAS channel of the gatekeeper client.
 * @param pGkServer    Pointer to the gatekeeper client.
 *
 * @return             OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooGkServerCloseChannel(ooGkServer *pGkServer);


/**
 * This function is used to fill endpoint's vendor information into vendor
 * identifier.
 * @param pGkServer    Pointer to gatekeeper client.
 * @param psVendor     Pointer to vendor identifier to be filled.
 *
 */
EXTERN void ooGkServerRasFillVendor
   (ooGkServer *pGkServer, H225VendorIdentifier *psVendor);


/**
 * This function is invoked to receive data on Gatekeeper client's RAS channel.
 * @param pGkServer    Handle to Gatekeeper client for which message has to be
 *                     received.
 *
 * @return             Completion status - OO_OK on success, OO_FAILED on
 *                     failure
 */
EXTERN int ooGkServerReceive(ooGkServer *pGkServer);


/**
 * This function is used to handle a received RAS message by a gatekeeper
 * client.
 * @param pGkServer   Handle to gatekeeper client.
 * @param pRasMsg     Handle to received Ras message.
 *
 * @return            OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooGkServerHandleRASMessage
   (ooGkServer *pGkServer, H225RasMessage *pRasMsg, const char *remoteHost, int remotePort);

/**
 * This function is used to send a message on Gatekeeper clien't RAS channel.
 * @param pGkServer   Handle to the gatekeeper client.
 * @param pRasMsg     Handle to Ras message to be sent.
 *
 * @return            OO_OK, on success. OO_FAILED, otherwise.
 */
EXTERN int ooGkServerSendMsg(ooGkServer *pGkServer, H225RasMessage *pRasMsg, const char *host, int port);

/**
 * This function is used to handle a received registration confirm message.
 * @param pGkServer            Handle to gatekeeper server.
 * @param pRegistration        Handle to received registration message.
 *
 * @return                     OO_OK, on success. OO_FAILED, otherwise.
 */
EXTERN int ooGkServerHandleRegistration
   (ooGkServer *pGkServer, H225RegistrationRequest *pRegistrationRequest);


/**
 * This function is used to send UnRegistration request message.
 * @param pGkServer  Handle to gatekeeper client for which URQ message has to
 *                   be sent.
 * @param aliases    List of aliases to be unregistered. NULL, if all the
 *                   aliases have to be unregistered.
 *
 * @return           OO_OK, on success. OO_FAILED, otherwise.
 */
EXTERN int ooGkServerSendURQ(ooGkServer *pGkServer, struct OOAliases *aliases);

/**
 * This function is used to handle a received Unregistration request message.
 * @param pGkServer              Handle to gatekeeper client.
 * @param punregistrationRequest Handle to received unregistration request.
 *
 * @return                   OO_OK, on success. OO_FAILED, otherwise.
 */
EXTERN int ooGkServerHandleUnregistrationRequest
   (ooGkServer *pGkServer, H225UnregistrationRequest *punregistrationRequest);


EXTERN int ooGkServerHandleRegistrationRequest
   (ooGkServer *pGkServer, H225RegistrationRequest *pRegistrationRequest, const char *remoteHost, int remotePort);

/**
 * This function is used to send an unregistration confirm messsage to
 * gatekeeper.
 * @param pGkServer        Handle to gatekeeper client.
 * @param reqNo            Request Sequence number for the confirm message.
 *
 * @return                 OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooGkServerSendUnregistrationConfirm(ooGkServer *pGkServer,
                                                               unsigned reqNo);


EXTERN int ooGkServerSendRegistrationConfirm(ooGkServer *pGkServer, unsigned reqNo, const char *host, int port);


/**
 * This function is used internally to set Gatekeeper Clients callbacks.
 * Note: This functionality is not yet fully supported
 * @param pGkServer  Handle to the GK client.
 * @param callbacks  Callback structure contatining various gatekeeper client
 *                   callbacks.
 * @return           OO_OK, on success. OO_FAILED, on failure.
 */
int ooGkServerSetCallbacks
   (ooGkServer *pGkServer, OOGKSERVERCALLBACKS callbacks);
/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __GKSERVER_H */
