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


#define OO_CAP_TYPE_AUDIO 1
#define OO_CAP_TYPE_VIDEO 2
#define OO_CAP_TYPE_DATA  3

#define OORX      (1<<0)
#define OOTX      (1<<1)
#define OORXANDTX (1<<2)
#define OORXTX    (1<<3) /* For symmetric capabilities */
/* Various types of caps. Note that not all
   supported */

#define OO_CAP_AUDIO_BASE      0
#define OO_G711ALAW64K         2
#define OO_G711ALAW56K         3
#define OO_G711ULAW64K         4
#define OO_G711ULAW56K         5
#define OO_GSMFULLRATE         18
#define OO_GSMHALFRATE         19
#define OO_GSMENHANCEDFULLRATE 20

#define OO_CAP_VIDEO_BASE 26
#define OO_CAP_DATA_BASE -1 /* place holder */

#define OOABSAUDIOCAP(cap) cap-OO_CAP_AUDIO_BASE
#define OOABSVIDEOCAP(cap) cap-OO_CAP_VIDEO_BASE
#define OOABSDATACAP(cap)  cap-OO_CAP_DATA_BASE

/*DTMF capabilities*/
#define OO_CAP_DTMF_RFC2833 (1<<0)
#define OO_CAP_DTMF_Q931    (1<<1)
#define OO_CAP_DTMF_H245    (1<<2)






typedef struct ooG711CapParams{
   int txframes;
   int rxframes;
}ooG711CapParams;


int caps_supported[10];
/* Bit macros to work with caps_supported*/

#define BINTSIZE (sizeof(int)*8)
#define BWORD(i) ((int)((i)/BINTSIZE))
#define BMASK(i) (1<< ((i)% BINTSIZE))
#define BISSET(caps,i) (caps[BWORD(i)] & BMASK(i))
#define BSET(caps,i) (caps[BWORD(i)] |= BMASK(i))
#define BRESET(caps,i) (caps[BWORD(i)] &= ~BMASK(i))


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
 * This function is used to add rfc2833 based dtmf detection capability
 * @param dynamicRTPPayloadType  dynamicRTPPayloadType to be used.
 * @return                       OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooEnableDTMFRFC2833(int dynamicRTPPayloadType);

/**
 * This function is used to remove rfc2833 dtmf detection capability.
 * @return                 OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooDisableDTMFRFC2833(void);
EXTERN int ooAddG711Capability(int cap, int txframes, int rxframes, int dir,
                               cb_StartReceiveChannel startReceiveChannel,
                               cb_StartTransmitChannel startTransmitChannel,
                               cb_StopReceiveChannel stopReceiveChannel,
                               cb_StopTransmitChannel stopTransmitChannel);

int ooAddG711Capability_internal(ooCallData *call, int cap, int txframes,
                                 int rxframes, int dir,
                                 cb_StartReceiveChannel startReceiveChannel,
                                 cb_StartTransmitChannel startTransmitChannel,
                                 cb_StopReceiveChannel stopReceiveChannel,
                                 cb_StopTransmitChannel stopTransmitChannel);

EXTERN int ooAddGSMCapability(int cap, ASN1USINT audioUnitSize,
                             ASN1BOOL comfortNoise,ASN1BOOL scrambled,int dir,
                             cb_StartReceiveChannel startReceiveChannel,
                             cb_StartTransmitChannel startTransmitChannel,
                             cb_StopReceiveChannel stopReceiveChannel,
                             cb_StopTransmitChannel stopTransmitChannel);

int ooAddGSMCapability_internal(ooCallData *call, int cap,
                                ASN1USINT audioUnitSize, ASN1BOOL comfortNoise,
                                ASN1BOOL scrambled, int dir,
                                cb_StartReceiveChannel startReceiveChannel,
                                cb_StartTransmitChannel startTransmitChannel,
                                cb_StopReceiveChannel stopReceiveChannel,
                                cb_StopTransmitChannel stopTransmitChannel);

int ooAddRemoteAudioCapability(ooCallData *call, H245AudioCapability *audioCap,
                               int dir);
int ooAddRemoteCapability(ooCallData *call, H245Capability *cap);

ASN1BOOL ooCheckCompatibility
(ooCallData *call, ooH323EpCapability *txCap, ooH323EpCapability *rxCap);

ASN1BOOL ooCheckCompatibility_1(ooCallData *call, ooH323EpCapability *epCap, H245AudioCapability * audioCap, int dir);
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
(ooH323EpCapability* epCap, OOCTXT *pctxt, int dir);

/**
 *
 */
void * ooCreateDTMFCapability(int cap, OOCTXT *pctxt);

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
(ooH323EpCapability *epCap, OOCTXT* pctxt, int dir);


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
ooH323EpCapability* ooIsAudioDataTypeSupported
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
int ooAppendCapToCapPrefs(ooCallData *call, int cap);
/**
 * @}
 */
#ifdef __cplusplus
}
#endif

#endif


