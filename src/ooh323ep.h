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
 *
 * @param tracefile      Absolute path to the trace file to be used for
 *                       storing traces
 * @param h245Tunneling  Indicates whether h245Tunneling enabled(1)/disabled(0)
 * @param fastStart      Indicates whether fast start is enabled(1)/disabled(0)
 * @param termType       Terminal type of the endpoint.
 * @param t35CountryCode Country code to be used
 * @param t35Extension   t35Extension value
 * @param manufacturer   manufacturer code
 * @param productID      Product ID to be used
 * @param versionID      Version Id of the software
 * @param callType       Type of the call ex. T_H225CallType_pointToPoint
 * @param listenport     Port on which to listen for incoming calls
 * @param callerid       ID to be used for outgoing calls.
 * @param callername     Caller name to be used for outgoing calls
 * @param callMode       Type of calls to be made(audio/video/fax).
 *                       (OO_CALLMODE_AUDIO, OO_CALLMODE_VIDEO)
 * @return               OO_OK, on success. OO_FAILED, on failure
 */
EXTERN int ooInitializeH323Ep( const char * tracefile, int h245Tunneling,
         int fastStart, int termType, int t35CountryCode, int t35Extension,
         int manufacturer, char *productID, char *versionID, int callType,
         int listenport, char *callerid, char *callername, int callMode);


/**
 * This function is used to assign a local ip address to be used for call
 * signalling.
 * @param localip        Dotted IP address to be used for call signalling.
 * @param listenport     Port to be used for listening for incoming calls.
 *
 * @return               OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooSetLocalCallSignallingAddress(char * localip, int listenport);

/**
 * This function is used to set the h323id alias for the endpoint.
 * @param h323id         H323-ID to be set as alias.
 *
 * @return               OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooSetAliasH323ID(char * h323id);

/**
 * This function is used to set the dialed digits alias for the
 * endpoint.
 * @param dialedDigits   Dialed-Digits to be set as alias.
 *
 * @return               OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooSetAliasDialedDigits(char * dialedDigits);

/**
 * This function is used to set the url alias for the endpoint.
 * @param url            URL to be set as an alias.
 *
 * @return               OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooSetAliasURLID(char * url);

/**
 * This function is used to set an email id as an alias for the endpoint.
 * @param email          Email id to be set as an alias.
 *
 * @return               OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooSetAliasEmailID(char * email);

/**
 * This function is used to set an ip address as an alias.
 * @param ipaddress      IP address to be set as an alias.
 *
 * @return               OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooSetAliasTransportID(char * ipaddress);

/**
 * This function is used to register the H323 Endpoint callback functions.
 * @param onIncomingCall        Callback function to be called when a new
 *                              incoming call is detected.
 * @param onOutgoingCall        Callback function to be called when an outgoing
 *                              call is placed on behalf of the application.
 * @param onCallEstablished     Callback function to be called when a call is
 *                              established with the remote end point.
 * @param onCallCleared         Callback function to be called when a call is
 *                              cleared.
 * @param onStartLogicalChannel Callback function to be called when a logical
 *                              channel is started.
 *
 * @return                      OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooH323EpRegisterCallbacks(cb_OnIncomingCall onIncomingCall,
                              cb_OnOutgoingCall onOutgoingCall,
                              cb_OnCallEstablished onCallEstablished,
                              cb_OnCallCleared onCallCleared,
                              cb_OnStartLogicalChannel onStartLogicalChannel);

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
EXTERN int ooDestroyH323Ep(void);


/**
 * This function is used to enable the auto answer feature for
 * incoming calls
 *
 * @return          OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooEnableAutoAnswer(void);

/**
 * This function is used to disable the auto answer feature for
 * incoming calls.
 *
 * @return          OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooDisableAutoAnswer(void);

/**
 * This function is used to enable/disable faststart.
 * @param fastStart   1, to enable and 0, to disable faststart
 *
 * @return            OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooSetFastStart(int fastStart);

/**
 * This function is used to enable/disable tunneling.
 * @param tunneling   1, to enable and 0, to disable tunneling.
 *
 * @return            OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooSetH245Tunneling(int tunneling);

/**
 * This function is used to set the product ID.
 * @param productID  New value for the product id.
 *
 * @return           OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooSetProductID(char * productID);

/**
 * This function is used to set version id.
 * @param versionID  New value for the version id.
 *
 * @return           OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooSetVersionID(char * versionID);

/**
 * This function is used to set callerid to be used for outbound
 * calls
 * @param callerID  New value for the caller id.
 *
 * @return          OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooSetCallerID(char * callerID);

/**
 * This function is used to set the caller name, which is used
 * for display purposes.
 * @param callerName New value for the caller name.
 *
 * @return           OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooSetCallerName(char * callerName);

/**
 * @}
 */
#ifdef __cplusplus
}
#endif

#endif
