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
 * @file oosndrtp.h
 * This file contains functions to read from sound device and playback.
 * It also provides a wrapper for oRTP function calls and creates threads
 * for receiving and sending rtp packets.
 */

#ifndef _OO_SNDRTP_H_
#define _OO_SNDRTP_H_


#include <stdlib.h>
#include "ootypes.h"


#ifndef _WIN32
#include <dlfcn.h>
#endif

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
 * @defgroup pluginInterface Media plug-in Interface definitions
 * @{
 */
/** Signature for function to Create Tx RTP channel*/
typedef int (*MediaAPI_CreateTxRTPChan)(int* channelId, char * destip, int port);
/** Signature for function to Close Tx RTP channel*/
typedef int (*MediaAPI_CloseTxRTPChan)(int);
/** Signature for function to Create Rx RTP channel*/
typedef int (*MediaAPI_CreateRecvRTPChan)(int* channelId, char* localip, int localport);
/** Signature for function to Close Rx RTP channel*/
typedef int (*MediaAPI_CloseRecvRTPChan)(int);
/** Signature for function to Start transmission of media file */
typedef int (*MediaAPI_StartTxWaveFile)(int channelId, char * filename);
/** Signature for function to Stop transmission of media file*/
typedef int (*MediaAPI_StopTxWaveFile)(int channelId);
/** Signature for function to Start transmitting captured audio from
 *   microphone
 */
typedef int (*MediaAPI_StartTxMic)(int channelId);
/** Signature for function to Stop transmitting microphone data*/
typedef int (*MediaAPI_StopTxMic)(int channelId);
/** Signature for function to Start receiving rtp data and playback*/
typedef int (*MediaAPI_StartRecvAndPlayback)(int channelId);
/** Signature for function to stop receiving rtp data */
typedef int (*MediaAPI_StopRecvAndPlayback)(int channelId);
/** Signature for function to Initialize the media plug-in*/
typedef int (*MediaAPI_InitializePlugin)(void);
/**
 * @}
 */

/**
 * @defgroup media Media plugin support functions
 * @{
 */

/* Media API */
#ifdef _WIN32
HMODULE media;
#else
void * media;
#endif

static MediaAPI_CreateTxRTPChan   FuncPtr_CreateTxRTPChan = 0;
static MediaAPI_CloseTxRTPChan    FuncPtr_CloseTxRTPChan = 0;
static MediaAPI_CreateRecvRTPChan FuncPtr_CreateRecvRTPChan = 0;
static MediaAPI_CloseRecvRTPChan  FuncPtr_CloseRecvRTPChan = 0;
static MediaAPI_StartTxWaveFile   FuncPtr_StartTxWaveFile = 0;
static MediaAPI_StopTxWaveFile    FuncPtr_StopTxWaveFile = 0;
static MediaAPI_StartTxMic        FuncPtr_StartTxMic = 0;
static MediaAPI_StopTxMic         FuncPtr_StopTxMic = 0;
static MediaAPI_StartRecvAndPlayback FuncPtr_StartRecvAndPlayback = 0;
static MediaAPI_StopRecvAndPlayback  FuncPtr_StopRecvAndPlayback = 0;
static MediaAPI_InitializePlugin  FuncPtr_InitializePlugin = 0;


/**
 * Loads the media plugin into the process space.
 * @param name      Name of the media plug-in library.
 *
 * @return          Completion status - 0 on success, -1 on failure
 */
EXTERN int ooLoadSndRTPPlugin(char * name);

/**
 * Unloads the plug-in from process space.
 *
 * @return          Completion status - 0 on success, -1 on failure 
 */
EXTERN int ooReleaseSndRTPPlugin(void);

/**
 * Creates a transmit RTP channel. Basically calls the corresponding function
 * of the plug-in library.
 * @param destip     IP address of the destination endpoint.
 * @param port       Destination port number.
 *
 * @return           Completion status - 0 on success, -1 on failure
 */
EXTERN int ooCreateTransmitRTPChannel
   ( char * destip, int port);

/**
 * Closes a transmit RTP channel. Basically calls the corresponding function
 * of the plug-in library.
 *
 * @return           Completion status - 0 on success, -1 on failure
 */
EXTERN int ooCloseTransmitRTPChannel
   (void);

/**
 * Creates a receive RTP channel. Basically calls the corresponding function
 * of the plug-in library.
 * @param localip    IP address of the endpoint where RTP data will be received.
 * @param localport  Port number of the local endpoint
 *
 * @return           Completion status - 0 on success, -1 on failure
 */
EXTERN int ooCreateReceiveRTPChannel
   (char* localip, int localport);

/**
 * Closes a receive RTP channel. Basically calls the corresponding function
 * of the plug-in library.
 *
 * @return           Completion status - 0 on success, -1 on failure
 */
EXTERN int ooCloseReceiveRTPChannel
   (void);

/**
 * Start transmitting a audio file. This calls corresponding function
 * of the plug-in library.
 * @param filename    Name of the file to be played.
 *
 * @return            Completion status - 0 on success, -1 on failure
 */
EXTERN int ooStartTransmitWaveFile
   (char * filename);

/**
 * Stop transmission of a audio file. This calls corresponding function
 * of the plug-in library.
 *
 * @return              Completion status - 0 on success, -1 on failure
 */
EXTERN int ooStopTransmitWaveFile
   (void);

/**
 * Starts capturing audio data from mic and transmits it as rtp stream. This
 * calls corresponding interface function of the plug-in library.
 *
 * @return            Completion status - 0 on success, -1 on failure
 */
EXTERN int ooStartTransmitMic
   (void);

/**
 * Stop transmission of mic audio data. This calls corresponding interface
 * function of the plug-in library.
 *
 * @return              Completion status - 0 on success, -1 on failure
 */
EXTERN int ooStopTransmitMic
   (void);

/**
 * Starts receiving rtp stream data and play it on the speakers. This
 * calls corresponding interface function of the plug-in library.
 *
 * @return            Completion status - 0 on success, -1 on failure
 */
EXTERN int ooStartReceiveAudioAndPlayback
   (void);

/**
 * Stop receiving rtp stream data.This calls corresponding interface function
 * of the plug-in library.
 *
 * @return            Completion status - 0 on success, -1 on failure
 */
EXTERN int ooStopReceiveAudioAndPlayback
   (void);

/** Not suuported currently */
EXTERN int ooStartReceiveAudioAndRecord
   (void);
/** Not supported currently */
EXTERN int ooStopReceiveAudioAndRecord
   (void);

/**
 * Set local RTP and RTCP addresses for the session. This function
 * gets next available ports for RTP and RTCP communication.
 *
 * @return            Completion status - 0 on success, -1 on failure
 */
EXTERN int ooSetLocalRTPAndRTCPAddrs(void);

/**
 * Closes transmit and receive RTP channels, if open. This calls
 * corresponding interface functions to close the channels.
 *
 * @return           Completion status - 0 on success, -1 on failure
 */
EXTERN int ooRTPShutDown(void);
/**
 * @}
 */
#ifdef __cplusplus
}
#endif
#endif
