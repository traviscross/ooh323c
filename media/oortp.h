/*
 * Copyright (C) 2004 by Objective Systems, Inc.
 *
 * This software is furnished under an open source license and may be
 * used and copied only in accordance with the terms of this license.
 * The text of the license may generally be found in the root
 * directory of this installation in the LICENSE.txt file.  It
 * can also be viewed online at the following URL:
 *
 *   http://www.obj-sys.com/open/license.html
 *
 * Any redistributions of this file including modified versions must
 * maintain this copyright notice.
 *
 *****************************************************************************/
/**
 * @file oortp.h
 * This file contains functions to create and use media channels.
 */

#ifndef _OORTP_H_
#define _OORTP_H_

/* oortp.h */

#include <stdio.h>
#include "ooSock.h"
#include "ooCommon.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef EXTERN
#ifdef _WIN32
#define EXTERN __declspec(dllexport)
#else
#define EXTERN
#endif /* _WIN32 */
#endif

/**
 * @defgroup mediaapi Media API
 * @{
 */

#define OO_CHAN_CLOSE 0
#define OO_CHAN_OPEN  1


/* Structure for holding RTP channel information*/
struct OORTPChannel{
   OOSOCKET sock;
   char    ip[20];/*Remote ip for transmit channel, local ip for recv channel*/
   int port; /*remote port for xmit chan and local port for recv chan*/
   unsigned short seqNo; /* last SeqNo recv or xmitted */
   unsigned long timestamp; /* last timestamp recv or xmitted */
   unsigned long ssrc; /* synchronization source */
   int status; /* 0 - closed, 1- open */
} gXmitChannel, gRecvChannel; /* Xmit and Recv RTP channels */


static int gRecvPlayLoop, gXmitFileLoop, gXmitMicLoop;

/* Thread handles, for Xmit and Recv threads. */
#ifdef _WIN32
HANDLE gXmitThrdHdl, gRecvThrdHdl;
#else
pthread_t gXmitThrdHdl, gRecvThrdHdl;
#endif

/**
 * This function initializes the plugin library.
 * @return 0, on success. -ve on failure.
 */
EXTERN int ooInitializePlugin();

/**
 * This function is invoked to create Transmit RTP channel.
 *
 * @param channelId     Pointer to int for returning the newly created channel
 *                      id. Not used currently as only two channels are
 *                      supported, 1 xmit channel and 1 recv channel.
 * @param destip        IP address of the destination endpoint.
 * @param port          RTP receive port number at the destination endpoint.
 *
 * @return              Completion status - 0 on success, -1 on failure
 */
EXTERN int ooCreateTransmitRTPChannel(int* channelId, char * destip, int port);

/**
 * This function is invoked to close the Transmit RTP channel.
 *
 * @param channelId     An integer value indicating the RTP channel to be
 *                      closed. Not used currently as only two channels are
 *                      supported, 1 xmit channel and 1 recv channel.
 *
 * @return              Completion status - 0 on success, -1 on failure
 */
EXTERN int ooCloseTransmitRTPChannel(int channelId);

/**
 * This function is invoked to create Receive RTP channel.
 *
 * @param channelId     Pointer to int for returning the newly created channel
 *                      id. Not used currently as only two channels are
 *                      supported, 1 xmit channel and 1 recv channel.
 * @param localip       IP address of the local endpoint.
 * @param localport     RTP receive port number at the local endpoint.
 *
 * @return              Completion status - 0 on success, -1 on failure
 */
EXTERN int ooCreateReceiveRTPChannel(int* channelId, char* localip,
                                     int localport);

/**
 * This function is invoked to close the Receive RTP channel.
 *
 * @param channelId     An integer value indicating the RTP channel to be
 *                      closed. Not used currently as only two channels are
 *                      supported, 1 xmit channel and 1 recv channel.
 *
 * @return              Completion status - 0 on success, -1 on failure
 */
EXTERN int ooCloseReceiveRTPChannel(int channelId);

/**
 * This function is used to transmit a wave file on already created transmit
 * RTP channel. This basically, creates a thread which will start reading
 * from the wave file and transmit data as rtp packets on the transmit
 * channel.
 * @param channelId  Indicates the transmit channel to be used. Not used
 *                   currently as only one transmit channel is supported.
 * @param filename   Name of the wave file to be transmitted.
 *
 * @return           Completion status - 0 on success, -1 on failure.
 */
EXTERN int ooStartTransmitWaveFile(int channelId, char * filename);

/**
 * This function is used to stop transmitting the wave file.
 * @param channelId  Indicates the transmit channel on which wave file
 *                   transmission needs to be halted. Not used
 *                   currently as only one transmit channel is supported.
 *
 * @return           Completion status - 0 on success, -1 on failure.
 */
EXTERN int ooStopTransmitWaveFile(int channelId);

/**
 * This function is used to start capturing and transmitting the data from
 * microphone.
 * @param channelId  Indicates the transmit channel on data transmission
 *                   should begin. Not used currently as only one transmit
 *                   channel is supported.
 *
 * @return           Completion status - 0 on success, -1 on failure.
 */
EXTERN int ooStartTransmitMic(int channelId);

/**
 * This function is used to stop transmitting the data from microphone.
 * @param channelId  Indicates the transmit channel on which data
 *                   transmission needs to be halted. Not used
 *                   currently as only one transmit channel is supported.
 *
 * @return           Completion status - 0 on success, -1 on failure.
 */
EXTERN int ooStopTransmitMic(int channelId);

/**
 * This function is used to start receiving rtp stream and playing the
 * received audio onto the speaker device.
 * @param channelId  Indicates the receive channel on which data
 *                   reception needs to be started. Not used
 *                   currently as only one transmit channel is supported.
 *
 * @return           Completion status - 0 on success, -1 on failure.
 */
EXTERN int ooStartReceiveAudioAndPlayback(int channelId);

/**
 * This function is used to stop receiving rtp stream.
 * @param channelId  Indicates the receive channel on which data
 *                   reception needs to be halted. Not used
 *                   currently as only one transmit channel is supported.
 *
 * @return           Completion status - 0 on success, -1 on failure.
 */
EXTERN int ooStopReceiveAudioAndPlayback(int channelId);

/* Not Available currently*/
EXTERN int ooStartReceiveAudioAndRecord(int channelId);
/* Not Available currently*/
EXTERN int ooStopReceiveAudioAndRecord(int channelId);
/**
 * @}
 */

/*
 * @defgroup winhelpers Media API Windows Helper Functions
 * @{
 */

/**
 * Internal helper function executed as a thread for wave
 * file transmission.
 * @return Completion status - 0 on success, -1 on failure.
 */
static int ooTransmitFileThreadFunc();

/**
 * Internal helper function executed as a thread for microphone
 * data transmission.
 * @return Completion status - 0 on success, -1 on failure.
 */
static int ooTransmitMicThreadFunc();

/**
 * Internal helper function executed as a thread for rtp stream
 * reception and playback.
 * @return     Completion status - 0 on success, -1 on failure.
 */
static int ooReceiveSpeakerThreadFunc();
/*
 * @}
 */

/**
 * @addtogroup linuxhelpers
 * @{
 */

/**
 * Internal helper function executed as a thread for microphone
 * data transmission.
 * @return Completion status - 0 on success, -1 on failure.
 */
static int ooTransmitMicThreadFuncLnx();

/**
 *
 * Internal helper function executed as a thread for rtp stream
 * reception and playback.
 * @return     Completion status - 0 on success, -1 on failure.
 */
static int ooReceiveSpeakerThreadFuncLnx();

/**
 *
 * Internal helper function executed as a thread for wave
 * file transmission.
 * @return Completion status - 0 on success, -1 on failure.
 */
static int ooTransmitFileThreadFuncLnx();
/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
