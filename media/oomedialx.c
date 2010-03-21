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

#ifndef _WIN32
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/soundcard.h>
#include <pthread.h>
#include "oomedialx.h"
#include "ooCommon.h"

int ooOpenAudioDevice()
{
  
   int sampleSize, numChannels, rate;
   ghSoundDevice = open("/dev/dsp", O_RDWR);
   if(ghSoundDevice == -1)
   {
      OOLOG2(1, "Error: Opening sound device for read/write");
      return -1;
   }
   OOLOG2(1, "Audio device open successful");
   ioctl(ghSoundDevice, SOUND_PCM_READ_BITS, &sampleSize);
   /*   sampleSize =8; */
   /*   ioctl(context->ooSoundDevice, SOUND_PCM_WRITE_BITS, &sampleSize);*/
   ioctl(ghSoundDevice, SOUND_PCM_READ_CHANNELS, &numChannels);
   ioctl(ghSoundDevice, SOUND_PCM_READ_RATE, &rate);
   /*   ioctl(context->ooSoundDevice, FIONBIO, &on);*/
   OOLOG5(1, "Sampe size %d bits, channels %d, rate %d",
          sampleSize, numChannels, rate);

   return 0;
}


int ooPlayAudioBuffer(unsigned char * buff, long size)
{
   int ret = 0;
   ret = write(ghSoundDevice, buff, size);
   return ret;
}

int ooGetMicAudioBuffer(unsigned char * buff, long size)
{
   int ret;
   ret = read(ghSoundDevice, buff, size);
   return ret;
}

int ooOpenWaveFileForRead(char * filename)
{
   ghSndFile = 0;
   ghSndFile = open(filename, O_RDONLY);
   if(ghSndFile == 0)
   {
      OOLOG2(1, "ERROR: Failed to open the raw audio file");
      return -1;
   }
   return 0;
}

int ooReadWaveFileData(char * databuf, int size)
{
   int ret = 0;
   ret = read(ghSndFile, databuf, size);
   if(ret < 0)
   {
      OOLOG2(1, "ERROR:Reading raw audio file");
      return -1;
   }
   return ret;
}

int ooCloseWaveFile()
{
   int ret = 0;
   ret = close(ghSndFile);
   return ret;
}

#endif
