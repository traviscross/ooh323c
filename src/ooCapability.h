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

/* Various types of caps. Note that not all
   supported */

#define OO_AUCAPS_MIN       1
#define OO_CAP_ULAW_64k_240 1 /* g711 ulaw 64k 240 frs/pkt */
#define OO_CAP_ULAW_64k_180 2
#define OO_CAP_ULAW_64k_30  3
#define OO_CAP_ULAW_64k_MAX 3

#define OO_CAP_ULAW_56k_MIN 4
#define OO_CAP_ULAW_56k_240 4
#define OO_CAP_ULAW_56k_180 5
#define OO_CAP_ULAW_56k_30  6
#define OO_CAP_ULAW_56k_MAX 6

#define OO_CAP_ALAW_64k_MIN 7
#define OO_CAP_ALAW_64k_240 7
#define OO_CAP_ALAW_64k_180 8
#define OO_CAP_ALAW_64k_30  9
#define OO_CAP_ALAW_64k_MAX 9

#define OO_CAP_ALAW_56k_MIN 10
#define OO_CAP_ALAW_56k_240 10
#define OO_CAP_ALAW_56k_180 11
#define OO_CAP_ALAW_56k_30  12
#define OO_CAP_ALAW_56k_MAX 12


#define OO_CAP_GSM          13
#define OO_CAP_G729A        14
#define OO_CAP_SPEEX        15
#define OO_CAP_G723_1       16
#define OO_AUCAPS_MAX       16
#define OO_CAP_DTMF_RFC2833 17

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
 * This function is used to add a new capability to the endpoint.
 * @param cap                  Type of capability to be added.
 * @param dir                  Direction - Indicates whether endpoint has
 *                             receive capability, or transmit capability or
 *                             both.ex T_H245Capability_receiveAudioCapability.
 * @param startReceiveChannel  Callback function to start receive channel.
 * @param startTransmitChannel Callback function to start transmit channel.
 * @param stopReceiveChannel   Callback function to stop receive channel.
 * @param stopTransmitChannel  Callback function to stop transmit channel.
 *
 * @return                     OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooAddCapability(int cap, int dir,
                    cb_StartReceiveChannel startReceiveChannel,
                    cb_StartTransmitChannel startTransmitChannel,
                    cb_StopReceiveChannel stopReceiveChannel,
                    cb_StopTransmitChannel stopTransmitChannel);

/**
 * This function is used to create a audio capability structure using the
 * capability type.
 * @param cap         Capability.
 * @param pctxt       Handle to OOCTXT which will be used to allocate memory
 *                    for new audio capability.
 *
 * @return            Newly created audio capability on success, NULL on
 *                    failure.
 */
struct H245AudioCapability* ooCreateAudioCapability
                                  (int cap, OOCTXT *pctxt);

/**
 * This function is used to create a g711 audio capability structure.
 * @param cap        Capability
 * @param pctxt      Handle to OOCTXT which will be used to allocate memory
 *                    for new audio capability.
 *
 * @return            Newly created audio capability on success, NULL on
 *                    failure.
 */
struct H245AudioCapability* ooCreateG711Capability
                                  (int cap, OOCTXT* pctxt);


/**
 * This function is used to determine whether a particular capability
 * can be supported by the endpoint.
 * @param call       Handle to the call.
 * @param audioCap   Handle to the audio capability.
 * @param dir        Direction in which support is desired.
 *
 * @return          Handle to the capability which supports audioCap, Null
 *                  if none found
 */
ooH323EpCapability* ooIsAudioCapSupported
                (ooCallData *call, H245AudioCapability* audioCap, int dir);


/**
 * This function is used to determine a capability match
 * @param cap       Local capability to be matched.
 * @param audioCap  Remote capability to be matched.
 * @param dir       Direction for local capability.
 *
 * @return          OO_OK, on success. OO_FAILED, on failure.
 */
int ooCompareAudioCaps(int cap, H245AudioCapability * audioCap, int dir);

/**
 * This function is used to determine a ulaw capability match
 * @param cap       Local capability to be matched.
 * @param audioCap  Remote capability to be matched.
 * @param dir       Direction for local capability.
 *
 * @return          OO_OK, on success. OO_FAILED, on failure.
 */
int ooCompareUlawCaps(int cap, H245AudioCapability* audioCap, int dir);

/**
 * This function is used to determine a Alaw capability match
 * @param cap       Local capability to be matched.
 * @param audioCap  Remote capability to be matched.
 * @param dir       Direction for local capability.
 *
 * @return          OO_OK, on success. OO_FAILED, on failure.
 */
int ooCompareAlawCaps(int cap, H245AudioCapability* audioCap, int dir);

/**
 * This function is used to determine whether a particular datatype
 * can be supported by the endpoint.
 * @param call       Handle to the call.
 * @param data       Handle to the data type.
 * @param dir        Direction in which support is desired.
 *
 * @return          Handle to the capability which supports 'data', Null
 *                  if none found
 */
ooH323EpCapability* ooIsDataTypeSupported
                           (ooCallData *call, H245DataType *data, int dir);
/**
 * @}
 */
#ifdef __cplusplus
}
#endif

#endif


