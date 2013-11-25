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

#ifndef _OOMEDIALX_H_
#define _OOMEDIALX_H_

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
#include <stdio.h>

/**
 * @defgroup linuxhelpers Media API Internal Linux Helper Functions
 * @{
 */
/** Global handle to open sound device */
int ghSoundDevice;
/** Global handle to open raw audio data file */
int ghSndFile;

/** Opens a RAW audio data file for read.
 * @param filename  Name of the file to be opened.
 *
 * @returns         Completion status - 0 on success, -1 on failure
 */
int ooOpenWaveFileForRead(const char * filename);

/**
 * Reads data from the opened raw audio file. The number of
 * bytes to be read is specified by the size parameter.
 * @param databuf     Pointer to a buffer in which data is returned.
 * @param size        Number of bytes to be read.
 *
 * @return            Number of bytes read on success, -1 on failure.
 */
int ooReadWaveFileData(char * databuf, int size);

/**
 * Close the open raw audio data file.
 *
 * @return            Completion status - 0 on success, -1 on failure
 */
int ooCloseWaveFile();

/**
 * Opens the audio device for read/write operation.
 *
 * @return            Completion status - 0 on success, -1 on failure
 */
int ooOpenAudioDevice();

/**
 * Plays a buffer full of audio data onto the audio device.
 * @param buff        Buffer containing the audio data to be played.
 * @param size        Size of the audio data in the buffer
 *
 * @return            Completion status - 0 on success, -1 on failure
 */
int ooPlayAudioBuffer(unsigned char * buff, long size);

/**
 * Reads audio data from the microphone device.
 * @param buff        Buffer in which data has to be captured.
 * @param size        Size of the capture buffer
 *
 * @return            Completion status - 0 on success, -1 on failure
 */
int ooGetMicAudioBuffer(unsigned char * buff, long size);

/**
 * @}
 */
#ifdef __cplusplus
}
#endif

#endif

