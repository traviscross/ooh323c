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
/**
 * @file ooh323ep.h
 * This file contains H323 endpoint related functions.
 */
#ifndef OO_H323EP_H_
#define OO_H323EP_H_
#include "ootypes.h"
#include "ooasn1.h"
#include "ooGkClient.h"

#define DEFAULT_TRACEFILE "trace.log"
#define DEFAULT_TERMTYPE 50
#define DEFAULT_PRODUCTID  "objsys"
#define DEFAULT_CALLERID   "objsyscall"
#define DEFAULT_T35COUNTRYCODE 1
#define DEFAULT_T35EXTENSION 0
#define DEFAULT_MANUFACTURERCODE 71
#define DEFAULT_CALLESTB_TIMEOUT 60
#define DEFAULT_MSD_TIMEOUT 30
#define DEFAULT_TCS_TIMEOUT 30
#define DEFAULT_LOGICALCHAN_TIMEOUT 30
#define DEFAULT_ENDSESSION_TIMEOUT 15
#define DEFAULT_H323PORT 1720

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

/**
 * @defgroup h323ep  H323 Endpoint management functions
 * @{
 */

/**
 * This function is the first function to be invoked before using stack. It
 * initializes the H323 Endpoint.
 * @param callerid       ID to be used for outgoing calls.
 * @param callMode       Type of calls to be made(audio/video/fax).
 *                       (OO_CALLMODE_AUDIO, OO_CALLMODE_VIDEO)
 * @param tracefile      Trace file name.
 *
 * @return               OO_OK, on success. OO_FAILED, on failure
 */
EXTERN int ooH323EpInitialize
   (const char *callerid, int callMode, const char* tracefile);


/**
 * This function is used to assign a local ip address to be used for call
 * signalling.
 * @param localip        Dotted IP address to be used for call signalling.
 * @param listenport     Port to be used for listening for incoming calls.
 *
 * @return               OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooH323EpSetLocalAddress(char * localip, int listenport);

/**
 * This function is used to set the trace level for the H.323 endpoint.
 * @param traceLevel     Level of tracing.
 *
 * @return               OO_OK, on success. OO_FAILED, otherwise.
 */
EXTERN int ooH323EpSetTraceLevel(int traceLevel);

/**
 * This function is used to add the h323id alias for the endpoint.
 * @param h323id         H323-ID to be set as alias.
 *
 * @return               OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooH323EpAddAliasH323ID(char * h323id);

/**
 * This function is used to add the dialed digits alias for the
 * endpoint.
 * @param dialedDigits   Dialed-Digits to be set as alias.
 *
 * @return               OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooH323EpAddAliasDialedDigits(char * dialedDigits);

/**
 * This function is used to add the url alias for the endpoint.
 * @param url            URL to be set as an alias.
 *
 * @return               OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooH323EpAddAliasURLID(char * url);

/**
 * This function is used to add an email id as an alias for the endpoint.
 * @param email          Email id to be set as an alias.
 *
 * @return               OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooH323EpAddAliasEmailID(char * email);

/**
 * This function is used to add an ip address as an alias.
 * @param ipaddress      IP address to be set as an alias.
 *
 * @return               OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooH323EpAddAliasTransportID(char * ipaddress);

/**
 * This function is used to clear all the aliases used by the
 * H323 endpoint.
 *
 * @return               OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooH323EpClearAllAliases(void);

/**
 * This function is used to set the H225 message callbacks for the
 * endpoint.
 * @param h225Callbacks  Callback structure containing various callbacks.
 *
 * @return               OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooH323EpSetH225MsgCallbacks(OOH225MsgCallbacks h225Callbacks);

/**
 * This function is used to set high level H323 call backs for the endpoint.
 * Make sure all unused callbacks in the structure are set to NULL before
 * calling this function.
 * @param h323Callbacks    Callback structure containing various high level
 *                         callbacks.
 * @return                 OO_OK, on success. OO_FAILED, on failure
 */
EXTERN int ooH323EpSetH323Callbacks(OOH323CALLBACKS h323Callbacks);


/**
 * This function is the last function to be invoked after done using the
 * stack. It closes the H323 Endpoint for an application, releasing all
 * the associated memory.
 *
 * @param None
 *
 * @return          OO_OK on success
 *                  OO_FAILED on failure
 */
EXTERN int ooH323EpDestroy(void);


/**
 * This function is used to enable the auto answer feature for
 * incoming calls
 *
 * @return          OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooH323EpEnableAutoAnswer(void);

/**
 * This function is used to disable the auto answer feature for
 * incoming calls.
 *
 * @return          OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooH323EpDisableAutoAnswer(void);

/**
 * This function is used to enable faststart.
 *
 * @return            OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooH323EpEnableFastStart(void);

/**
 * This function is used to disable faststart.
 *
 * @return            OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooH323EpDisableFastStart(void);

/**
 * This function is used to enable tunneling.
 *
 * @return            OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooH323EpEnableH245Tunneling(void);

/**
 * This function is used to disable tunneling.
 *
 * @return            OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooH323EpDisableH245Tunneling(void);

/**
 * This function is used to enable GkRouted calls.
 *
 * @return            OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooH323EpEnableGkRouted(void);

/**
 * This function is used to disable Gkrouted calls.
 *
 * @return            OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooH323EpDisableGkRouted(void);

/**
 * This function is used to set the product ID.
 * @param productID  New value for the product id.
 *
 * @return           OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooH323EpSetProductID (const char * productID);

/**
 * This function is used to set version id.
 * @param versionID  New value for the version id.
 *
 * @return           OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooH323EpSetVersionID (const char * versionID);

/**
 * This function is used to set callerid to be used for outbound
 * calls.
 * @param callerID  New value for the caller id.
 *
 * @return          OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooH323EpSetCallerID (const char * callerID);

/**
 * This function is used to set calling party number to be used for outbound
 * calls.Note, you can override it for a specific call by using
 * ooCallSetCallingPartyNumber function.
 * @param number   e164 number to be used as calling party number.
 *
 * @return         OO_OK, on success; OO_FAILED, otherwise.
 */
EXTERN int ooH323EpSetCallingPartyNumber(const char * number);

/**
 * This function is used to print the current configuration information of
 * the H323 endpoint to log file.
 */
void ooH323EpPrintConfig(void);



/**
 * This function is used to add G729 capability to the H323 endpoint.
 * @param cap                  Type of G729 capability to be added.
 * @param txframes             Number of frames per packet for transmission.
 * @param rxframes             Number of frames per packet for reception.
 * @param dir                  Direction of capability.OORX, OOTX, OORXANDTX
 * @param startReceiveChannel  Callback function to start receive channel.
 * @param startTransmitChannel Callback function to start transmit channel.
 * @param stopReceiveChannel   Callback function to stop receive channel.
 * @param stopTransmitChannel  Callback function to stop transmit channel.
 *
 * @return                     OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooH323EpAddG729Capability
   (int cap, int txframes, int rxframes, int dir,
    cb_StartReceiveChannel startReceiveChannel,
    cb_StartTransmitChannel startTransmitChannel,
    cb_StopReceiveChannel stopReceiveChannel,
    cb_StopTransmitChannel stopTransmitChannel);

/**
 * This function is used to add G711 capability to the H323 endpoint.
 * @param cap                  Type of G711 capability to be added.
 * @param txframes             Number of frames per packet for transmission.
 * @param rxframes             Number of frames per packet for reception.
 * @param dir                  Direction of capability.OORX, OOTX, OORXANDTX
 * @param startReceiveChannel  Callback function to start receive channel.
 * @param startTransmitChannel Callback function to start transmit channel.
 * @param stopReceiveChannel   Callback function to stop receive channel.
 * @param stopTransmitChannel  Callback function to stop transmit channel.
 *
 * @return                     OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooH323EpAddG711Capability
   (int cap, int txframes, int rxframes, int dir,
    cb_StartReceiveChannel startReceiveChannel,
    cb_StartTransmitChannel startTransmitChannel,
    cb_StopReceiveChannel stopReceiveChannel,
    cb_StopTransmitChannel stopTransmitChannel);


/**
 * This function is used to add a new GSM capability to the endpoint.
 * @param cap                  Type of GSM capability to be added.
 * @param framesPerPkt         Number of GSM frames pre packet.
 * @param comfortNoise         Comfort noise spec for the capability.
 * @param scrambled            Scrambled enabled/disabled for the capability.
 * @param dir                  Direction of capability.OORX, OOTX, OORXANDTX
 * @param startReceiveChannel  Callback function to start receive channel.
 * @param startTransmitChannel Callback function to start transmit channel.
 * @param stopReceiveChannel   Callback function to stop receive channel.
 * @param stopTransmitChannel  Callback function to stop transmit channel.
 *
 * @return                     OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooH323EpAddGSMCapability(int cap, ASN1USINT framesPerPkt,
                             OOBOOL comfortNoise,OOBOOL scrambled,int dir,
                             cb_StartReceiveChannel startReceiveChannel,
                             cb_StartTransmitChannel startTransmitChannel,
                             cb_StopReceiveChannel stopReceiveChannel,
                             cb_StopTransmitChannel stopTransmitChannel);

/**
 * This function is used to enable rfc 2833 support for the endpoint.
 * @param dynamicRTPPayloadType   Payload type value to use.
 *
 * @return                        OO_OK, on success; OO_FAILED, on failure
 */
EXTERN int ooH323EpEnableDTMFRFC2833(int dynamicRTPPayloadType);

/**
 * This function is used to disable rfc 2833 support for the endpoint.
 *
 * @return                        OO_OK, on success; OO_FAILED, on failure
 */
EXTERN int ooH323EpDisableDTMFRFC2833(void);

/**
 * This function is used to add callbacks to the gatekeeper client. If user
 * application wants to do some special processing of various gatekeeper client
 * events, that can be done through these callbacks.
 * @param gkClientCallbacks      Handle to the callback structure.Make sure all
 *                               the members of the structure are appropriately
 *                               initialized.
 *
 * @retun                        OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooH323EpSetGkClientCallbacks(OOGKCLIENTCALLBACKS gkClientCallbacks);

/**
 * @}
 */
#ifdef __cplusplus
}
#endif

#endif
