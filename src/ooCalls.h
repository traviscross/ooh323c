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
 * This function is used to remove a call from the list of existing calls.
 * @param h323ep        Pointer to the H323 Endpoint.
 * @param call          Pointer to the call to be removed.
 *
 * @return              OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooRemoveCallFromList(ooEndPoint * h323ep, ooCallData *call);

/**
 * This function is used to clear a call. It closes all associated sockets,
 * removes call from list and frees up associated memory.
 * @param call          Pointer to the call to be cleared.
 * @param reason        Reason for clearing the call.
 *
 * @return              OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooClearCall(ooCallData *call, int reason);

/**
 * This function is used to find a call by using the unique token for the call.
 * @param callToken      The unique token for the call.
 *
 * @return               Pointer to the call if found, NULL otherwise.
 */
EXTERN ooCallData* ooFindCallByToken(char *callToken);

/**
 * This function is used to set a local RTP port for the call.
 * @param call          Pointer to the call for which RTP port has to be set.
 * @param port          Port Number.
 *
 * @return              OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooSetLocalRtpPort(ooCallData *call, int port);

/**
 * This function is used to set a local RTCP port for the call.
 * @param call          Pointer to the call for which RTCP port has to be set.
 * @param port          Port Number.
 *
 * @return              OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooSetLocalRtcpPort(ooCallData *call, int port);

/**
 * This function is used to retrieve the RTP port for the call.
 * @param call          Pointer to the call for which RTP port is required.
 *
 * @return              RTP Port value.
 */
EXTERN int ooGetLocalRtpPort(ooCallData*call);

/**
 * This function is used to retrieve the RTCP port for the call.
 * @param call          Pointer to the call for which RTP port is required.
 *
 * @return              RTCP Port value.
 */
EXTERN int ooGetLocalRtcpPort(ooCallData*call);

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
 * This function is used to remove a logical channel from the list of logical
 * channels.
 * @param call              Pointer to the call from which logical channel has
 *                          to be removed.
 * @param ChannelNo         Forward logical channel number of the channel to be
 *                          removed.
 */
EXTERN int ooRemoveLogicalChannel(ooCallData *call, int ChannelNo);

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
 * @return          OO_OK, on success. OO_FAILED, on failure
 */
EXTERN int ooAddNewLogicalChannel(ooCallData *call, int channelNo,
                                  int sessionID, char *type, char * dir,
                                  ooH323EpCapability *epCap);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif



#endif
