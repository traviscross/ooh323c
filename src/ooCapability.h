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

#define OO_GSMFRAMESIZE 33 /* standard frame size for gsm is 33 bytes */

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
#define OO_G7231               9
#define OO_G729                11
#define OO_G729A               12
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
}ooCapParams;

typedef struct ooGSMCapParams{
   unsigned txframes;
   unsigned rxframes;
   OOBOOL scrambled;
   OOBOOL comfortNoise;
}ooGSMCapParams;



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
 * @param call                   Call if enabling for call, else null for
 *                               endpoint.
 * @param dynamicRTPPayloadType  dynamicRTPPayloadType to be used.
 * @return                       OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooCapabilityEnableDTMFRFC2833
   (ooCallData *call, int dynamicRTPPayloadType);

/**
 * This function is used to remove rfc2833 dtmf detection capability.
 * @param call             Handle to call, if disabling for the call, else NULL
 *                         for end-point.
 * @return                 OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooCapabilityDisableDTMFRFC2833(ooCallData *call);



/**
 * This function is used to add simple capabilities which have only rxframes
 * and txframes parameters to the endpoint or call.(ex. G711, G729)
 * @param call                 Handle to a call. If this is not Null, then
 *                             capability is added to call's remote enpoint
 *                             capability list, else it is added to local H323
 *                             endpoint list.
 * @param cap                  Type of G711 capability to be added.
 * @param txframes             Number of frames per packet for transmission.
 * @param rxframes             Number of frames per packet for reception.
 * @param dir                  Direction of capability.OORX, OOTX, OORXANDTX
 * @param startReceiveChannel  Callback function to start receive channel.
 * @param startTransmitChannel Callback function to start transmit channel.
 * @param stopReceiveChannel   Callback function to stop receive channel.
 * @param stopTransmitChannel  Callback function to stop transmit channel.
 * @param remote               TRUE, if adding call's remote capability.
 *
 * @return                     OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooCapabilityAddSimpleCapability
   (ooCallData *call, int cap, int txframes, int rxframes, int dir,
    cb_StartReceiveChannel startReceiveChannel,
    cb_StartTransmitChannel startTransmitChannel,
    cb_StopReceiveChannel stopReceiveChannel,
    cb_StopTransmitChannel stopTransmitChannel,
    OOBOOL remote);

#if 0
/**
 * This is an internal helper function which is used to add a G729 capability
 * to local endpoints capability list or to remote endpoints capability list or
 * to a calls capability list.
 * @param call                 Handle to a call. If this is not Null, then
 *                             capability is added to call's remote enpoint
 *                             capability list, else it is added to local H323
 *                             endpoint list.
 * @param cap                  Type of G729 capability to be added.
 * @param txframes             Number of frames per packet for transmission.
 * @param rxframes             Number of frames per packet for reception.
 * @param dir                  Direction of capability.OORX, OOTX, OORXANDTX
 * @param startReceiveChannel  Callback function to start receive channel.
 * @param startTransmitChannel Callback function to start transmit channel.
 * @param stopReceiveChannel   Callback function to stop receive channel.
 * @param stopTransmitChannel  Callback function to stop transmit channel.
 * @param remote               TRUE, if adding call's remote capability.
 *
 * @return                     OO_OK, on success. OO_FAILED, on failure.
 *
 */
EXTERN int ooCapabilityAddG729Capability(ooCallData *call, int cap,
                                         int txframes, int rxframes, int dir,
                                 cb_StartReceiveChannel startReceiveChannel,
                                 cb_StartTransmitChannel startTransmitChannel,
                                 cb_StopReceiveChannel stopReceiveChannel,
                                 cb_StopTransmitChannel stopTransmitChannel,
                                  OOBOOL remote);

/**
 * This is an internal helper function which is used to add a G711 capability
 * to local endpoints capability list or to remote endpoints capability list or
 * to a call's capability list.
 * @param call                 Handle to a call. If this is not Null, then
 *                             capability is added to call's remote enpoint
 *                             capability list, else it is added to local H323
 *                             endpoint list.
 * @param cap                  Type of G711 capability to be added.
 * @param txframes             Number of frames per packet for transmission.
 * @param rxframes             Number of frames per packet for reception.
 * @param dir                  Direction of capability.OORX, OOTX, OORXANDTX
 * @param startReceiveChannel  Callback function to start receive channel.
 * @param startTransmitChannel Callback function to start transmit channel.
 * @param stopReceiveChannel   Callback function to stop receive channel.
 * @param stopTransmitChannel  Callback function to stop transmit channel.
 * @param remote               TRUE, if adding call's remote capability.
 *
 * @return                     OO_OK, on success. OO_FAILED, on failure.
 *
 */
int ooCapabilityAddG711Capability(ooCallData *call, int cap, int txframes,
                                 int rxframes, int dir,
                                 cb_StartReceiveChannel startReceiveChannel,
                                 cb_StartTransmitChannel startTransmitChannel,
                                 cb_StopReceiveChannel stopReceiveChannel,
                                 cb_StopTransmitChannel stopTransmitChannel,
                                 OOBOOL remote);


#endif

/**
 * This is an internal helper function which is used to add a GSM capability
 * to local endpoints capability list or to remote endpoints capability list.
 * @param call                 Handle to a call. If this is not Null, then
 *                             capability is added to call's remote enpoint
 *                             capability list, else it is added to local H323
 *                             endpoint list.
 * @param cap                  Type of GSM capability to be added.
 * @param framesPerPkt         Number of GSM frames per packet.
 * @param comfortNoise         Comfort noise spec for the capability.
 * @param scrambled            Scrambled enabled/disabled for the capability.
 * @param dir                  Direction of capability.OORX, OOTX, OORXANDTX
 * @param startReceiveChannel  Callback function to start receive channel.
 * @param startTransmitChannel Callback function to start transmit channel.
 * @param stopReceiveChannel   Callback function to stop receive channel.
 * @param stopTransmitChannel  Callback function to stop transmit channel.
 * @param remote               TRUE, if adding call's remote capabilities.
 *
 * @return                     OO_OK, on success. OO_FAILED, on failure.
 */
int ooCapabilityAddGSMCapability(ooCallData *call, int cap,
                                unsigned framesPerPkt, OOBOOL comfortNoise,
                                OOBOOL scrambled, int dir,
                                cb_StartReceiveChannel startReceiveChannel,
                                cb_StartTransmitChannel startTransmitChannel,
                                cb_StopReceiveChannel stopReceiveChannel,
                                cb_StopTransmitChannel stopTransmitChannel,
                                OOBOOL remote);


/**
 * This function is used to add a audio capability to calls remote 
 * capability list.
 * @param call                Handle to the call.
 * @param audioCap            Handle to the remote endpoint's audio capability.
 * @param dir                 Direction in which capability is supported by
 *                            remote endpoint.
 *
 * @return                    OO_OK, on success. OO_FAILED, otherwise.
 */
int ooAddRemoteAudioCapability(ooCallData *call, H245AudioCapability *audioCap,
                               int dir);


/**
 * This function is used to add a capability to call's remote  capability list.
 * The capabilities to be added are extracted from received TCS message.
 * @param call           Handle to the call.
 * @param cap            Handle to the remote endpoint's H245 capability.
 *
 * @return               OO_OK, on success. OO_FAILED, otherwise.
 */
int ooAddRemoteCapability(ooCallData *call, H245Capability *cap);

/**
 * This function is used to update joint capabilities for call. It checks
 * whether remote capability can be supported by local capabilities for the
 * call and if supported makes entry into the joint capability list for the
 * call.
 * @param call           Handle to the call
 * @param cap            Remote cap which will be tested for compatibility.
 *
 * @return               returns OO_OK, if updated else OO_FAILED; 
 */
EXTERN int ooCapabilityUpdateJointCapabilities
                                       (ooCallData* call, H245Capability *cap);

/**
 * This function is used to test the compatibility of the two capabilities.
 * It checks whether tx capability can be received by rx capability.
 * @param call                Handle to the call.
 * @param txCap               Transmit capability to be tested for
 *                            compatibility.
 * @param rxCap               Receive capability to be tested for compatibility
 *
 * @return                    TRUE, if compatible, FALSE otherwise.
 */
ASN1BOOL ooCheckCompatibility
(ooCallData *call, ooH323EpCapability *txCap, ooH323EpCapability *rxCap);

/**
 * This function is used to test whether the endpoint capability in the
 * specified direction can be supported by the audio capability.
 * @param call               Handle to the call.
 * @param epCap              Endpoint capability.
 * @param audioCap           Audio capability with which compatibility has to
 *                           be tested.
 * @param dir                Direction indicating whether endpoint capability
 *                           will be used for transmission or reception.
 *
 * @return                   TRUE, if compatible. FALSE, otherwise.
 */

ASN1BOOL ooCheckCompatibility_1(ooCallData *call, ooH323EpCapability *epCap,
                                     H245AudioCapability * audioCap, int dir);


/**
 * This function is used to create a audio capability structure using the
 * capability type.
 * @param epCap       Capability.
 * @param pctxt       Handle to OOCTXT which will be used to allocate memory
 *                    for new audio capability.
 * @param dir         Direction in which the newly created capability will be
 *                    used.
 *
 * @return            Newly created audio capability on success, NULL on
 *                    failure.
 */
struct H245AudioCapability* ooCreateAudioCapability
(ooH323EpCapability* epCap, OOCTXT *pctxt, int dir);

/**
 * This function is used to create a dtmf capability which can be added to
 * a TCS message.
 * @param cap         Type of dtmf capability to be created.
 * @param pctxt       Pointer to OOCTXT structure to be used for memory
 *                    allocation.
 *
 * @return            Pointer to the created DTMF capability, NULL in case of
 *                    failure.
 */
void * ooCreateDTMFCapability(int cap, OOCTXT *pctxt);


/**
 * This function is used to create a GSM Full Rate capability structure.
 * @param epCap       Handle to the endpoint capability.
 * @param pctxt       Handle to OOCTXT which will be used to allocate memory
 *                    for new audio capability.
 * @param dir         Direction for the newly created capability.
 *
 * @return            Newly created audio capability on success, NULL on
 *                    failure.
 */
struct H245AudioCapability* ooCreateGSMFullRateCapability
   (ooH323EpCapability *epCap, OOCTXT* pctxt, int dir);

/**
 * This function is used to create a simple(g711, g729) audio capability
 * structure.
 * @param epCap       Handle to the endpoint capability
 * @param pctxt       Handle to OOCTXT which will be used to allocate memory
 *                    for new audio capability.
 * @param dir         Direction in which the newly created capability will be
 *                    used.
 *
 * @return            Newly created audio capability on success, NULL on
 *                    failure.
 */
struct H245AudioCapability* ooCreateSimpleCapability
   (ooH323EpCapability *epCap, OOCTXT* pctxt, int dir);


/**
 * This function is used to determine whether a particular capability
 * can be supported by the endpoint.
 * @param call       Handle to the call.
 * @param audioCap   Handle to the audio capability.
 * @param dir        Direction in which support is desired.
 *
 * @return          Handle to the copyof capability which supports audioCap,
 *                  Null if none found
 */
ooH323EpCapability* ooIsAudioDataTypeSupported
                (ooCallData *call, H245AudioCapability* audioCap, int dir);

/**
 * This function is used to determine whether a particular capability type
 * can be supported by the endpoint.
 * @param call       Handle to the call.
 * @param data       Handle to the capability type.
 * @param dir        Direction in which support is desired.
 *
 * @return          Handle to the copy of capability which supports specified
 *                  capability type, Null if none found
 */
ooH323EpCapability* ooIsDataTypeSupported
                           (ooCallData *call, H245DataType *data, int dir);

/**
 * This function is used to clear the capability preference order.
 * @param call      Handle to call, if capability preference order for call
 *                  has to be cleared, NULL for endpoint.
 *
 * @return          OO_OK, on success. OO_FAILED, on failure
 */
EXTERN  int ooResetCapPrefs(ooCallData *call);

/**
 * This function is used to remove a particular capability from preference
 * list.
 * @param call     Handle to call, if call's preference list has to be modified
 *                 else NULL, to modify endpoint's preference list.
 * @param cap      Capability to be removed
 *
 * @return         OO_OK, on success. OO_FAILED, otherwise.
 */
EXTERN  int ooRemoveCapFromCapPrefs(ooCallData *call, int cap);

/**
 * This function is used to append a particular capability to preference
 * list.
 * @param call     Handle to call, if call's preference list has to be modified
 *                 else NULL, to modify endpoint's preference list.
 * @param cap      Capability to be appended.
 *
 * @return         OO_OK, on success. OO_FAILED, otherwise.
 */
EXTERN int ooAppendCapToCapPrefs(ooCallData *call, int cap);

/**
 * This function is used to change preference order of a particular capability
 * in the preference list.
 * @param call     Handle to call, if call's preference list has to be modified
 *                 else NULL, to modify endpoint's preference list.
 * @param cap      Capability concerned
 * @param pos      New position in the preference order
 *
 * @return         OO_OK, on success. OO_FAILED, otherwise.
 */
EXTERN int ooChangeCapPrefOrder(ooCallData *call, int cap, int pos);

/**
 * This function is used to preppend a particular capability to preference
 * list.
 * @param call     Handle to call, if call's preference list has to be modified
 *                 else NULL, to modify endpoint's preference list.
 * @param cap      Capability to be preppended.
 *
 * @return         OO_OK, on success. OO_FAILED, otherwise.
 */
EXTERN int ooPreppendCapToCapPrefs(ooCallData *call, int cap);


/**
 * @}
 */
#ifdef __cplusplus
}
#endif

#endif


