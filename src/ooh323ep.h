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

#ifndef OO_H323EP_H_
#define OO_H323EP_H_
#include "ootypes.h"
#include "ooasn1.h"

#define OO_RX_CAP 51
#define OO_TX_CAP 52
#define OO_RXTX_CAP 53

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
 * @param txAudioChan    1, if transmit audio channel has to be opened, 0
 *                       otherwise.
 * @return               OO_OK, on success. OO_FAILED, on failure
 */
EXTERN int ooInitializeH323Ep( const char * tracefile, int h245Tunneling,
         int fastStart, int termType, int t35CountryCode, int t35Extension,
         int manufacturer, char *productID, char *versionID, int callType,
         int listenport, char *callerid, char *callername, int txAudioChan);

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
EXTERN int ooDestroyH323Ep();

/**
 * Function to add audio capabilities to the endpoint. 'dir' indicates
 * whether we have a transmit capability or a receive capability or both.
 * Last four parameters are the callback functions for channel control
 * @param audioCap            Audio Capability to be added.
 * @param dir                 Direction - Indicates whether endpoint has
 *                            receive capability, or transmit capability or
 *                            both.
 * @param startReceiveChannel Callback function to call receive channel.
 * @param startTransmitChannel Callback function to start transmit channel.
 * @param stopReceiveChannel   Callback function to stop receive channel.
 * @param stopTransmitChannel  Callback function to stop transmit channel.
 *
 * @return                     OO_OK, on success. OO_FAILED, on failure.
*/
EXTERN int ooAddAudioCapability(H245AudioCapability audioCap, int dir,
                                cb_StartReceiveChannel startReceiveChannel,
                                cb_StartTransmitChannel startTransmitChannel,
                                cb_StopReceiveChannel stopReceiveChannel,
                                cb_StopTransmitChannel stopTransmitChannel);

/**
 * This function is used to copy audio capability from src to destination.
 * @param src                  Pointer to audio capability to be copied.
 * @param dest                 Pointer to destination audio capability
 *
 * @return                     OO_OK, on success. OO_FAILED, on failure
 */
EXTERN int ooCopyAudioCapability(H245AudioCapability *src,
                                 H245AudioCapability *dest);

/**
 * This function is used to check whether local endpoint supports a particular
 * type of audio capability.
 * @param capType              Type of audio capability to serach for.
 * @param dir                  Direction in which support is required(RX/TX).
 *
 * @return                     Pointer to the capability, if found. Null,
 *                             otherwise.
 */
EXTERN ooH323EpCapability* ooIsAudioCapabilitySupported(int capType, int dir);


/**
 * @}
 */
#ifdef __cplusplus
}
#endif

#endif
