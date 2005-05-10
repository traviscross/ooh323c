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
 * @file ooCalls.h
 * This file contains Call management functions.
 */
#ifndef _OOCALLS_H_
#define _OOCALLS_H_

#include "ootypes.h"

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
 * @defgroup callmgmt  Call Management
 * @{
 */
/**
 * This function is used to create a new call entry.
 * @param type         Type of the call (incoming/outgoing)
 * @param callToken    Call Token, an uniques identifier for the call
 *
 * @return             Pointer to a newly created call
 */
EXTERN ooCallData* ooCreateCall(char *type, char *callToken);

/**
 * This function is used to add a call to the list of existing calls.
 * @param h323ep       Pointer to the H323 Endpoint structure.
 * @param call         Pointer to the call to be added.
 *
 * @return             OO_OK, on success. OO_FAILED, on failure
 */
EXTERN int ooAddCallToList(ooEndPoint * h323ep, ooCallData *call);

/**
 * This function is used to set callerid for the call.
 * @param call          Handle to the call
 * @param callerid      caller id value
 *
 * @return              OO_OK, on success. OO_FAILED, otherwise.
 */
EXTERN int ooCallSetCallerId(ooCallData* call, const char* callerid);

/**
 * This function is used to set calling party number for a particular call.
 * @param call          Handle to the call.
 * @param number        Calling Party number value.
 *
 * @return              OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooCallSetCallingPartyNumber(ooCallData *call, const char *number);

/**
 * This function is used to retrieve calling party number of a particular call.
 * @param call          Handle to the call.
 * @param buffer        Handle to the buffer in which value will be returned.
 * @param len           Length of the supplied buffer.
 *
 * @return              OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooCallGetCallingPartyNumber(ooCallData *call, char *buffer, int len);

/**
 * This function is used to retrieve called party number of a particular call.
 * @param call          Handle to the call.
 * @param buffer        Handle to the buffer in which value will be returned.
 * @param len           Length of the supplied buffer.
 *
 * @return              OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooCallGetCalledPartyNumber(ooCallData *call, char *buffer, int len);

/**
 * This function is used to set called party number for a particular call.
 * @param call          Handle to the call.
 * @param number        Called Party number value.
 *
 * @return              OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooCallSetCalledPartyNumber(ooCallData *call, const char *number);

/**
 * This function is used to clear the local aliases used by this call.
 * @param call          Handle to the call.
 *
 * @return              OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooCallClearAliases(ooCallData *call);

/**
 * This function is used to add an H323ID alias to be used by local endpoint
 * for a particular call.
 * @param call          Handle to the call
 * @param h323id        H323ID to add for the local endpoint for the call.
 *
 * @return              OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooCallAddAliasH323ID(ooCallData *call, const char* h323id);

/**
 * This function is used to add an dialedDigits alias to be used by local
 * endpoint for a particular call.
 * @param call          Handle to the call
 * @param dialedDigits  DialedDigits to add for the local endpoint for call.
 *
 * @return              OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooCallAddAliasDialedDigits
                             (ooCallData *call, const char* dialedDigits);

/**
 * This function is used to add an email-id alias to be used by local
 * endpoint for a particular call.
 * @param call          Handle to the call
 * @param email         Email-id to add for the local endpoint for call.
 *
 * @return              OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooCallAddAliasEmailID(ooCallData *call, const char* email);


/**
 * This function is used to add an email-id alias to be used by local
 * endpoint for a particular call.
 * @param call          Handle to the call
 * @param url           URL-id to add for the local endpoint for call.
 *
 * @return              OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooCallAddAliasURLID(ooCallData *call, const char* url);

/**
 * This function is used to add an H323ID alias for the remote endpoint
 * involved in a particular call.
 * @param call          Handle to the call
 * @param h323id        H323ID to add for the remote endpoint.
 *
 * @return              OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooCallAddRemoteAliasH323ID(ooCallData *call, const char* h323id);


/**
 * This function is used to add G711 capability for the call. The
 * "ooCallAdd...Capability" functions allow to override the global endpoint
 * capabilities and use specific capabilities for specific calls.
 * @param call           Call for which capability has to be added.
 * @param cap            Capability to be added.
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
EXTERN int ooCallAddG711Capability(ooCallData *call, int cap, int txframes,
                            int rxframes, int dir,
                            cb_StartReceiveChannel startReceiveChannel,
                            cb_StartTransmitChannel startTransmitChannel,
                            cb_StopReceiveChannel stopReceiveChannel,
                            cb_StopTransmitChannel stopTransmitChannel);


/**
 * This function is used to add GSM capability for the call. The
 * "ooCallAdd...Capability" functions allow to override the global endpoint
 * capabilities and use specific capabilities for specific calls.
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
EXTERN int ooCallAddGSMCapability(ooCallData* call, int cap,
                                  ASN1USINT framesPerPkt, OOBOOL comfortNoise,
                                  OOBOOL scrambled, int dir,
                                  cb_StartReceiveChannel startReceiveChannel,
                                  cb_StartTransmitChannel startTransmitChannel,
                                  cb_StopReceiveChannel stopReceiveChannel,
                                  cb_StopTransmitChannel stopTransmitChannel);

/**
 * This function is used to enable rfc 2833 capability for the call. By default
 * the stack uses the dtmf settings for the endpoint. But if you want to
 * enable/disable dtmf for a specific call, then you can override end-point
 * settings using this function
 * @param call                  Call for which rfc2833 has to be enabled.
 * @param dynamicRTPPayloadType dynamicRTPPayloadType to be used.
 *
 * @return                      OO_OK, on success. OO_FAILED, on failure
 */
EXTERN int ooCallEnableDTMFRFC2833(ooCallData *call, int dynamicRTPPayloadType);


/**
 * This function is used to disable rfc 2833 capability for the call.
 * By default the stack uses the dtmf settings for the endpoint. But if you
 * want to enable/disable dtmf for a specific call, then you can override
 * end-point settings using this function
 * @param call                  Call for which rfc2833 has to be disabled.
 *
 * @return                      OO_OK, on success. OO_FAILED, on failure
 */
EXTERN int ooCallDisableDTMFRFC2833(ooCallData *call);

/**
 * This function is used to find a call by using the unique token for the call.
 * @param callToken      The unique token for the call.
 *
 * @return               Pointer to the call if found, NULL otherwise.
 */
EXTERN ooCallData* ooFindCallByToken(char *callToken);

/**
 * This function is used to clear a call. Based on what stage of clearance the
 * call is it takes appropriate action.
 * @param call   Handle to the call which has to be cleared.
 *
 * @return       OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooEndCall(ooCallData *call);

/**
 * This function is used to remove a call from the list of existing calls.
 * @param h323ep        Pointer to the H323 Endpoint.
 * @param call          Pointer to the call to be removed.
 *
 * @return              OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooRemoveCallFromList(ooEndPoint * h323ep, ooCallData *call);

/**
 * This function is used to clean a call. It closes all associated sockets,
 * removes call from list and frees up associated memory.
 * @param call          Pointer to the call to be cleared.
 *
 * @return              OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooCleanCall(ooCallData *call);

/**
 * This function is used to add a new logical channel entry into the list
 * of currently active logical channels.
 * @param call      Pointer to the call for which new logical channel
 *                  entry has to be created.
 * @param channelNo Channel number for the new channel entry.
 * @param sessionID Session identifier for the new channel.
 * @param type      Type of the channel(audio/video/data)
 * @param dir       Direction of the channel(transmit/receive)
 * @param epCap     Capability to be used for the new channel.
 *
 * @return          Pointer to logical channel, on success. NULL, on failure
 */
EXTERN ooLogicalChannel* ooAddNewLogicalChannel
         (ooCallData *call, int channelNo, int sessionID, char *type,
          char * dir, ooH323EpCapability *epCap);

/**
 * This function is used to find a logical channel by logical channel number.
 * @param call          Pointer to the call for which logical channel is
 *                      required.
 * @param channelNo     Forward Logical Channel number for the logical channel
 *
 * @return              Pointer to the logical channel if found, NULL
 *                      otherwise.  
 */
EXTERN ooLogicalChannel* ooFindLogicalChannelByLogicalChannelNo
                                  (ooCallData *call,int channelNo);

/**
 * This function is called when a new logical channel is established. It is
 * particularly useful in case of faststart. When the remote endpoint selects
 * one of the proposed alternatives, other channels for the same session type
 * need to be closed. This function is used for that.
 * @param call      Handle to the call which owns the logical channel.
 * @param pChannel  Handle to the newly established logical channel.
 *
 * @return          OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooOnLogicalChannelEstablished
                              (ooCallData *call, ooLogicalChannel * pChannel);

/**
 * This fuction is used to check whether a specified session in specified
 * direction is active for the call.
 * @param call       Handle to call for which session has to be queried.
 * @param sessionID  Session id to identify the type of session(1 for audio,
 *                   2 for voice and 3 for data)
 * @param dir        Direction of the session(transmit/receive)
 *
 * @return           1, if session active. 0, otherwise.
 */
EXTERN ASN1BOOL ooIsSessionEstablished
                                  (ooCallData *call, int sessionID, char* dir);

/**
 * This function is used to retrieve a logical channel with particular
 * sessionID. Note that there can be two entries of logical channel, one in
 * each direction. This function will return the first channel which has the
 * same session ID.
 * @param call      Handle to the call which owns the channels to be searched.
 * @param sessionID Session id of the session which is to be searched for.
 *
 * @return          Returns a pointer to the logical channel if found, NULL
 *                  otherwise.
 */
EXTERN ooLogicalChannel* ooGetLogicalChannel(ooCallData *call, int sessionID);

/**
 * This function is used to remove a logical channel from the list of logical
 * channels.
 * @param call              Pointer to the call from which logical channel has
 *                          to be removed.
 * @param ChannelNo         Forward logical channel number of the channel to be
 *                          removed.
 */
EXTERN int ooRemoveLogicalChannel(ooCallData *call, int ChannelNo);

/**
 * This function is used to cleanup a logical channel. It first stops media, if
 * it is still active and then removes the channel from the list, freeing up
 * all the associated memory.
 * @param call       Handle to the call which owns the logical channel.
 * @param channelNo  Channel number identifying the channel.
 *
 * @return           OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooClearLogicalChannel(ooCallData *call, int channelNo);

/**
 * This function is used to cleanup all the logical channels associated with
 * the call.
 * @param call      Handle to the call which owns the channels.
 *
 * @return          OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooClearAllLogicalChannels(ooCallData *call);

/**
 * This function can be used by an application to specify media endpoint
 * information for different types of media. The stack by default uses local IP
 * and port for media. An application can provide mediainfo if it wants to
 * override default.
 * @param call      Handle to the call
 * @param mediaInfo mediainfo structure which defines the media endpoint to be
 *                  used.
 *
 * @return          OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooAddMediaInfo(ooCallData *call, ooMediaInfo mediaInfo);
/**
 * This function is used to find a logical channel from a received
 * olc.
 * @param call     Handle to the related call.
 * @param olc      Handle to the received OLC.
 *
 * @return         Returns the corresponding logical channel if found,
 *                 else returns NULL.
 */
EXTERN ooLogicalChannel * ooFindLogicalChannelByOLC
                               (ooCallData *call, H245OpenLogicalChannel *olc);

/**
 * This function is used to find a logical channel based on session Id,
 * direction of channel and datatype.
 * @param call       Handle to the call
 * @param sessionID  Session ID for the channel to be searched.
 * @param dir        Direction of the channel wrt local endpoint.
 *                   (transmit/receive)
 * @param dataType   Handle to the data type for the channel.
 *
 * @return           Logical channel, if found, NULL otherwise.
 */
EXTERN ooLogicalChannel * ooFindLogicalChannel
   (ooCallData *call, int sessionID, char *dir, H245DataType * dataType);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif



#endif
