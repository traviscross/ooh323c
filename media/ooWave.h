/*
 * Copyright (C) 2004-2010 by Objective Systems, Inc.
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
 * @file ooWave.h
 * This file contains low level wave functions.
 */

#ifndef _OOWAVE_H_
#define _OOWAVE_H_


#include <stdio.h>
#include "ooCommon.h"
#include <Windows.h>
#include <Mmsystem.h>
#include <Basetsd.h>



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
 * @defgroup winhelpers Media API Internal Windows Helper Functions
 * @{
 */



/** Helper structure for reading wavefile */
struct ooWaveFile{
   char filename[1024]; /** Wave file name*/
   HMMIO hWaveFile; /** Wave file handle */
   WAVEFORMATEX waveFormat; /** Wave file format */
   int dataSize; /** Data chunk size */
};

/** Global handle to open wave file */
struct ooWaveFile gWaveFile;


/**
   This holds a list of buffers holding recorded data from MIC.
   This data can then be sent on rtp channel or played back on
   speakers.
*/
struct WaveBuffer{
   WAVEHDR* pWaveHdr;
   struct WaveBuffer * next;
};
/** Global pointers to list of wave buffers. */
struct WaveBuffer *gpWaveHead, *gpWaveTail;

int gRecording; /* Flag indicating whether recording is on/off */
/** Count of number of buffers queued inside the wave-in device*/
int gQueuedBufCount;

/** This holds the list of free buffers which can be used for sending
   data to waveout device*/
struct BufferNode{
   char* buf;
   struct BufferNode * next;
};
/** Global pointers to the list holding free buffers */
struct BufferNode *gpFreeBufHead, *gpFreeBufTail;

/* Count of number of buffers queued inside wave-out device for plaback. */
int gPlayQueueCount;

/** Global handle to open wave-in device*/
HWAVEIN ghWaveIn;
/** Global handle to open wave-out device*/
HWAVEOUT ghWaveOut;

/** As callback functions run in their own threads,
   we need mutex protection for data which is used
   by multiple threads. */
CRITICAL_SECTION gPlayMutex, gReadMutex;

/**
 * Opens a WaveFile for read and traverses up to the data chunk,
 * so that next mmioRead will return wavedata.
 *
 * @param filename    Name of the wave file to be opened.
 *
 * @return            Completion status - 0 on success, -1 on failure.
*/
int ooOpenWaveFileForRead (const char* filename);

/**
 * Closes the open WaveFile.
 *
 * @return            Completion status - 0 on success, -1 on failure.
 */
int ooCloseWaveFile();

/** Reads data from already opened wave file. Number of bytes to be read
 *  is specified by the "size" parameter. The data is returned in "buffer"
 *  parameter and the number of bytes read is returned as a return value.
 *  @param buffer     Buffer which will contain the data read.
 *  @param size       Size of the buffer passed.
 *
 *  @return           Number of bytes read on success, -1 on failure  
 */
int ooReadWaveFileData (char* buffer, int size);

/**
 * Opens a waveOut device, i.e., speaker for playback. waveFormat specifies
 * the format to be used for playback.
 * @param phWaveOut     Pointer to an empty HWAVEOUT handle which will contain
 *                      the handle to the opened device on return.
 * @param waveFormat    Wave format to be used for playback.
 *
 * @return              Completion status - 0 on success, -1 on failure.
 */
int ooOpenSpeaker(HWAVEOUT * phWaveOut, WAVEFORMATEX waveFormat);


/**
 * Plays the number of bytes specified by the size parameter from the buff
 * onto the speaker device.
 * @param hWaveOut      Handle to the speaker device.
 * @param buff          Pointer to the buffer containing the data to be played.
 * @param size          Size of the buffer to be played out.
 *
 * @return              Completion status - 0 on success, -1 on failure.
 */
int ooPlayWaveBuffer(HWAVEOUT hWaveOut, unsigned char * buff,
                            long size);

/**
 * Release all the buffers queued into the waveOut device(speaker) for playback
 * and close speaker.
 * @param hWaveOut    Handle to the wave out device to be closed.
 *
 * @return            Completion status - 0 on success, -1 on failure.
 */
int ooCloseSpeaker(HWAVEOUT hWaveOut);

/**
 * Opens the WaveIn device, MIC, for recording and queues the buffers into the
 * device which can be used for storing recorded data.
 * @return      Completion status - 0 on success, -1 on failure.
 */
int ooOpenMic();

/**
 * Start recording audio into the buffers.
 * @param hWaveIn        Handle to wave-in device.
 *
 * @return               Completion status - 0 on success, -1 on failure.
 */
int ooStartMicDataCapture(HWAVEIN hWaveIn);


/**
 * Stop recording audio. Note that this marks current buffer as done and frees
 * it by calling callback function. However, other queued buffers will stay
 * there in wave-in device.
 * @param hWaveIn       Handle to the wave-in device.
 *
 * @return              Completion status - 0 on success, -1 on failure
 */
int ooStopMicDataCapture(HWAVEIN hWaveIn);

/**
 * This releases all the buffers queued inside wave-in device, mic, and then
 * closes the mic.
 * @param hWaveIn       Handle to the wave-in device.
 *
 * @return              Completion status - 0 on success, -1 on failure
 */
int ooCloseMic(HWAVEIN hWaveIn);


/** This is a callback function registered with the mic. It will be called by
 *  mic device when a buffer full of data is recorded.
 * @param hwi        Handle to the wave-in device.
 * @param uMsg       Event message sent by the device.
 * @param dwInstance User data.
 * @param dwParam1   Message parameter.
 * @param dwParam2   Message parameter.
 *
 * @return           None 
 */
void CALLBACK ooMICCallback(HWAVEIN hwi, UINT uMsg, DWORD dwInstance,
                                   DWORD dwParam1, DWORD dwParam2);

/**
 * This is a callback function registered with the  speaker. It is called by
 * the speaker, when a buffer full of data is played back.
 * @param hwo        Handle to the wave out device.
 * @param uMsg       Waveform audio output message.
 * @param dwInstance User instance data passed during waveOutOpen.
 * @param dwParam1   Message parameter.
 * @param dwParam2   Message parameter
 *
 * @return           None
 */
void CALLBACK ooSpeakerCallback(HWAVEOUT hwo, UINT uMsg,
                             DWORD dwInstance, DWORD dwParam1, DWORD dwParam2);

/**
 * Helper function to maintain the list of buffers returned by mic,
 * after recording. These buffers are kept in this list till there processing
 * is done and then again queued back into the mic for further recording.
 * @param waveHdr  Pointer to wave header structure which in turn contains
 *                 the data buffer.
 *
 * @return         Completion status - 0 on success, -1 on failure.
 */
int ooAddToWaveBufferList(WAVEHDR * waveHdr);

/**
 * Helper function which removes the WAVEHDR at the front of the list. Note
 * this does not free up the mem used by WAVEHDR as it will be queued into the
 * mic for further recording.
 *
 * @return       Completion status - 0 on success, -1 on failure.
 */
int ooRemoveHeadOfWaveBufferList();

/**
 * Helper function to put a buffer back into the free buffer list. Once the
 * buffer playback is done, it can be added to the free list
 * @param buffer     Pointer to the buffer to be added to the free list.
 *
 * @return           Completion status - 0 on success, -1 on failure
 */
int ooAddToFreeBufferList(char * buffer);

/**
 * Get a free buffer from free buffer list. A free buffer is retrieved using
 * this function for storing data which will then passed onto ooPlayWaveBuffer
 * function for playback.
 *
 * @return                 Pointer to a character buffer
 */
char* ooGetFreeBuffer();
/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* _OOWAVE_H_ */
