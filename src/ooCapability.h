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
 * @file ooCapability.h
 * This file contains Capability management functions.
 */
#ifndef OO_CAPABILITY_H_
#define OO_CAPABILITY_H_
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
 * @defgroup capmgmt  Capability Management
 * @{
 */
/**
 * Function to add audio capabilities to the endpoint. 'dir' indicates
 * whether we have a transmit capability or a receive capability or both.
 * Last four parameters are the callback functions for channel control
 * @param audioCap            Audio Capability to be added.
 * @param dir                 Direction - Indicates whether endpoint has
 *                            receive capability, or transmit capability or
 *                            both.ex T_H245Capability_receiveAudioCapability
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
 * This function is used to create a duplicate audio capability from the one supplied
 * as parameter.
 * @param call     Handle to the call for which duplicate capability has to be created.
 * @param audioCap Source audio capability.
 * @param pctxt    Pointer to an OOCTXT which will be used to allocate memory for the
 *                 new capability.
 *
 * @return         Pointer to the newly created audio capability or NULL in case of failure
 */
EXTERN H245AudioCapability * ooCreateDupAudioCap
   (ooCallData *call, H245AudioCapability *audioCap, OOCTXT *pctxt);

/**
 * This function is used to create a duplicate GSM audio capability.
 * @param call     Handle to the call for which duplicate has to be created.
 * @param gsmCap   Pointer to the source capability.
 * @param pctxt    Pointer to the OOCTXT context which will be used for memory allocation.
 *
 * @return         Pointer to the newly created GSM capability or NULL in case of failure.
 */
EXTERN H245GSMAudioCapability* ooCreateDupGSMAudioCap
   (ooCallData *call, H245GSMAudioCapability *gsmCap, OOCTXT *pctxt);

/**
 * This function is used to check whether local endpoint supports a particular
 * type of audio capability.
 * @param call     Handle to the call.
 * @param audioCap Pointer to the audio capability
 * @param dir      Direction in which support is required.
 *                 ex. T_H245Capability_receiveAudioCapability
 *
 * @return         Pointer to the matching capability, if found. Null,
 *                 otherwise.
 */
EXTERN ooH323EpCapability* ooIsAudioCapSupported
   (ooCallData *call, H245AudioCapability* audioCap, int dir);

/**
 * This function is used to compare two audio capabilities for a match.
 * @param call     Handle to the call for which comparison is being carried out.
 * @param cap1     First capability to be compared.
 * @param cap2     Second capability to be compared.
 *
 * @return         1, on match. 0. otherwise.
 */
EXTERN int ooCompareAudioCaps
   (ooCallData *call, H245AudioCapability * cap1, H245AudioCapability *cap2);

/**
 * This function is used to compare two G711 audio capabilities for a match.
 * @param cap1     First G711 capability to be compared.
 * @param cap2     Second G711 capability to be compared.
 *
 * @return         1, on match. 0. otherwise.
 */
EXTERN int ooCompareG711Caps(H245AudioCapability * cap1, H245AudioCapability *cap2);

/**
 * This function is used to compare two GSM audio capabilities for a match.
 * @param cap1     First GSM capability to be compared.
 * @param cap2     Second GSM capability to be compared.
 *
 * @return         1, on match. 0. otherwise.
 */
EXTERN int ooCompareGSMCaps(H245AudioCapability *cap1, H245AudioCapability *cap2);

/**
 * Checks whether a particular datatype is supported by the end point.
 * @param call     Handle to the call
 * @param data     Pointer to the data type
 * @param dir      Direction in which support is required.
 *
 * @return         Pointer to the matching capability, if supported, else NULL.
 */
EXTERN ooH323EpCapability* ooIsDataTypeSupported(ooCallData *call,
                                                 H245DataType *data, int dir);

/**
 * Checks whether a particular audio datatype is supported by the end point.
 * @param call      Handle to the call
 * @param audioData Pointer to the audio data type
 * @param dir       Direction in which support is required.
 *
 * @return          Pointer to the matching capability, if supported, else NULL.
 */
EXTERN ooH323EpCapability* ooIsAudioDataTypeSupported(ooCallData* call,
                                    H245AudioCapability* audioData, int dir);

EXTERN ooH323EpCapability* ooIsNonStandardDataTypeSupported(ooCallData* call,
                               H245NonStandardParameter *nonStandard, int dir);

EXTERN ooH323EpCapability* ooIsVideoDataTypeSupported(ooCallData* call,
                                    H245VideoCapability* videoData, int dir);

EXTERN ooH323EpCapability* ooIsApplicationDataTypeSupported(ooCallData* call,
                                 H245DataApplicationCapability *data, int dir);

EXTERN ooH323EpCapability* ooIsEncryptedDataTypeSupported(ooCallData* call,
                                  H245EncryptionMode *encryptionData, int dir);

EXTERN ooH323EpCapability* ooIsH235ControlDataTypeSupported(ooCallData* call,
                               H245NonStandardParameter *h235Control, int dir);

EXTERN ooH323EpCapability* ooIsH235MediaDataTypeSupported(ooCallData* call,
                                        H245H235Media *h235Media, int dir);

EXTERN ooH323EpCapability* ooIsMultiplexedStreamDataTypeSupported
       (ooCallData* call, H245MultiplexedStreamParameter *multiplexedStream,
        int dir);

/**
 * @}
 */
#ifdef __cplusplus
}
#endif

#endif


