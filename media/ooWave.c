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

#include "ooWave.h"

/* Open the wave file for read and seek to the data chunk
   in the open file.
*/
int ooOpenWaveFileForRead(char* filename)
{
   MMCKINFO mmckinfoParent;   /* for the Group Header */
   MMCKINFO mmckinfoSubchunk;   /* for finding chunks within the Group */
   OOLOG2(1, "StartOf:OpenWaveWaveFileForRead");
   if(filename == 0)
   {
      OOLOG2(1, "ERROR: null file name");
      return -1;
   }

   strcpy(gWaveFile.filename, filename);
   /* Open the file */
   if (!(gWaveFile.hWaveFile = mmioOpen(filename, 0, MMIO_READ|MMIO_ALLOCBUF)))
   {
      OOLOG2(1, "ERROR: Failed to open the wave file");
      return -1;
   }

   /* Search for the WAVE Group header within the file */
   mmckinfoParent.fccType = mmioFOURCC('W', 'A', 'V', 'E');
   if (mmioDescend(gWaveFile.hWaveFile, (LPMMCKINFO)&mmckinfoParent, 0,
                   MMIO_FINDRIFF))
   {
      OOLOG2(1, "ERROR: This file doesn't contain a WAVE format");
      return -1;
   }

   /* Read the fmt chunk */
   mmckinfoSubchunk.ckid = mmioFOURCC('f', 'm', 't', ' ');
   if (mmioDescend(gWaveFile.hWaveFile, &mmckinfoSubchunk, &mmckinfoParent,
                   MMIO_FINDCHUNK))
   {
      OOLOG2(1, "ERROR: Failed to locate the fmt chunk");
      return -1;
   }
   /* Read the format chunk - this gives the format of the wave file*/
   if (mmioRead(gWaveFile.hWaveFile, (HPSTR)&gWaveFile.waveFormat,
                mmckinfoSubchunk.cksize) != (LRESULT)mmckinfoSubchunk.cksize)
   {
      OOLOG2(1, "ERROR: reading the fmt chunk");
      return -1;
   }
  
   if(gWaveFile.waveFormat.wFormatTag      != 1     ||
      gWaveFile.waveFormat.nAvgBytesPerSec != 16000 ||
      gWaveFile.waveFormat.nSamplesPerSec  != 8000  ||
      gWaveFile.waveFormat.wBitsPerSample  != 16    ||
      gWaveFile.waveFormat.nChannels       != 1     ||
      gWaveFile.waveFormat.nBlockAlign     != 2     ||
      gWaveFile.waveFormat.cbSize          != 0      )
   {
      OOLOG2(1, "ERROR: Unsupported wave file format");
      OOLOG9(1, "WAVEFORMATEX wFormatTag=%u, nAvgBytesPerSec=%u,"
                 "nSamplesPerSec=%u, wBitsPerSample=%u, nChannels=%u, "
                 "nBlockAlign=%u, cbSize=%u", gWaveFile.waveFormat.wFormatTag,
                 gWaveFile.waveFormat.nAvgBytesPerSec,
                 gWaveFile.waveFormat.nSamplesPerSec,
                 gWaveFile.waveFormat.wBitsPerSample,
                 gWaveFile.waveFormat.nChannels,
                 gWaveFile.waveFormat.nBlockAlign,
                 gWaveFile.waveFormat.cbSize);
      return -1;
   }
                
                
   /*Read data chunk now */        
   mmioAscend(gWaveFile.hWaveFile, &mmckinfoSubchunk, 0);
   mmckinfoSubchunk.ckid = mmioFOURCC('d', 'a', 't', 'a');
   if (mmioDescend(gWaveFile.hWaveFile, &mmckinfoSubchunk, &mmckinfoParent,
                   MMIO_FINDCHUNK))
   {
      OOLOG2(1, "Error: An erroring reading the data chunk");
      return -1;
   }
   gWaveFile.dataSize = mmckinfoSubchunk.cksize;
   OOLOG2(1, "EndOf:OpenWaveWaveFileForRead");
   return 0;
}

/* Close the wave file */
int ooCloseWaveFile()
{
   int ret;
   OOLOG2(1, "StartOf:CloseWaveFile");
   ret = mmioClose(gWaveFile.hWaveFile, 0);
   if(ret != 0)
   {
      OOLOG2(1, "ERROR: Failed to close the wave file");
      return -1;
   }
   OOLOG2(1, "EndOf:CloseWaveFile");
   return 0;
}

/* Read buffer full of data from the wave file */
int ooReadWaveFileData(char * buffer, int size)
{
   long ret;
/* OOLOG2(1, "StartOf:ReadWaveFileData");*/
   ret = mmioRead(gWaveFile.hWaveFile, buffer, size);
   if(ret < 0)
   {
      OOLOG2(1, "Error: in reading data chunk");
      return -1;
   }

/* OOLOG2(1, "EndOf:ReadWaveFileData");*/
   return ret;
}

/* Open Speaker output device for playback*/
int ooOpenSpeaker(HWAVEOUT * phWaveOut, WAVEFORMATEX waveFormat)
{
   MMRESULT result;
   int i = 0;
   OOLOG2(1, "StartOf:OpenSpeaker");
   result = waveOutOpen(phWaveOut, WAVE_MAPPER, &waveFormat,
                        (DWORD)ooSpeakerCallback, (DWORD)&gPlayQueueCount,
                        CALLBACK_FUNCTION);
   if (result != MMSYSERR_NOERROR)
   {
      OOLOG2(1, "ERROR: Opening waveOutDevice");
      return -1;
   }
   OOLOG2(1, "EndOf:OpenSpeaker");
   return 0;
}

/* Playback a audio buffer onto the speaker device. */
int ooPlayWaveBuffer(HWAVEOUT hWaveOut, unsigned char * buff, long size)
{
   WAVEHDR *pWavHdr;
   long ret;
   /*OOLOG2(1, "StartOf:PlayWaveBuffer");*/
   /* Allocate memory for wave hdr. The WaveHdr memory will be released
      in speaker callback function, once the playback is done and
      the buffer pointed to by lpData will be returned to free buffer
      list.
   */
   pWavHdr = (WAVEHDR*) malloc(sizeof(WAVEHDR));
   memset(pWavHdr, 0, sizeof(WAVEHDR));
   pWavHdr->lpData = buff;
   pWavHdr->dwBufferLength = size;
   pWavHdr->dwBytesRecorded=0;
   /* Prepare the wave header */
   ret = waveOutPrepareHeader(hWaveOut, pWavHdr, sizeof(WAVEHDR));
   if(ret != MMSYSERR_NOERROR)
   {
      OOLOG3(1, "ERROR: Preparing WaveHDR in ooPlayWaveBuffer %d", ret);
      return -1;
   }
   /* Queue the buffer onto the wave out device for playback*/  
   ret = waveOutWrite(hWaveOut, pWavHdr, sizeof(WAVEHDR));
   if(ret != MMSYSERR_NOERROR)
   {
      OOLOG3(1, "ERROR: waveOutWrite in ooPlayWaveBuffer %d", ret);
      return -1;
   }
   EnterCriticalSection(&gPlayMutex);
   gPlayQueueCount++; /* Increment the count of queued buffers */
   LeaveCriticalSection(&gPlayMutex);
   /*OOLOG2(1, "EndOf:PlayWaveBuffer");*/
   return 0;
}

/* Close the speaker device*/
int ooCloseSpeaker(HWAVEOUT hWaveOut)
{
   int i = 0;
   struct BufferNode * cur, *temp;
   OOLOG2(1, "StartOf:CloseSpeaker");
  
   /* This stops playback and resets the device. All pending
      audio buffers are marked as done and returned to the
      application.
   */
   if(waveOutReset(hWaveOut) != MMSYSERR_NOERROR)
   {
      OOLOG2(1, "ERROR: waveOutReset in ooCloseSpeaker");
      return -1;
   }
   for(i =0; i<10; i++)
   {
      if(gPlayQueueCount == 0) break;
      Sleep(100);
   }
   if(gPlayQueueCount != 0)
      OOLOG2(1, "Warn:All buffers are not released from the waveout device");
   /* Close the wave out device. */
   if(waveOutClose(hWaveOut) != MMSYSERR_NOERROR)
   {
      OOLOG2(1, "ERROR: waveOutClose in ooCloseSpeaker");
      return -1;
   }
   /* Release memroy. This code is not used, but currently kept
      just because it might be required in the future.
   */
   cur = gpFreeBufHead;
   while(cur)
   {
      temp = cur;
      cur = cur->next;
     
          if(temp->buf)
         free(temp->buf);
      free(temp);
   }
   OOLOG2(1, "EndOf:CloseSpeaker");
   return 0;
}

/* Open microphone device for data capture.*/
int ooOpenMic()
{
   WAVEHDR * pWaveHdr=0;
   int i=0;
   WAVEFORMATEX waveFormat;
   OOLOG2(1, "StartOf:OpenMic");
   /* Format for reading data*/
   memset(&waveFormat, 0, sizeof(WAVEFORMATEX));
   waveFormat.wFormatTag = WAVE_FORMAT_PCM;
   waveFormat.nChannels = 1;
   waveFormat.nSamplesPerSec = 8000;
   waveFormat.nAvgBytesPerSec = 16000;
   waveFormat.nBlockAlign = 2;
   waveFormat.wBitsPerSample = 16;
   waveFormat.cbSize = 0;
   gQueuedBufCount = 0; /* No buffers are yet queued into the waveIn device*/
   /* Open mic device*/
   if(waveInOpen(&ghWaveIn, WAVE_MAPPER, &waveFormat, (DWORD)ooMICCallback,
                 (DWORD)&gQueuedBufCount, CALLBACK_FUNCTION) !=
                 MMSYSERR_NOERROR)
   {
      OOLOG2(1, "ERROR: In opening Wave-in device");
      return -1;
   }
  
   /* Note that these buffers are queued into the waveInDevie,i.e, MIC. Once
      a buffer is full with recorded audio, it is returned to the application
      via ooMICCallback function, which queus it into waveHdr queue. The
      consumer of this data is supposed to process it and remove it from the
      queue. The consumer can then requeue the buffer into MIC for more
      recording or release the memory if recording is done.
   */
   for(i=0; i<10; i++)
   {
      pWaveHdr = (WAVEHDR*)malloc(sizeof(WAVEHDR));
      memset(pWaveHdr, 0, sizeof(WAVEHDR));
      pWaveHdr->dwBufferLength = OORTPPACKETDATASIZE*sizeof(short);
      pWaveHdr->lpData = (char *)malloc(OORTPPACKETDATASIZE*sizeof(short));
      memset(pWaveHdr->lpData, 0, OORTPPACKETDATASIZE*sizeof(short));
      if(waveInPrepareHeader(ghWaveIn, pWaveHdr, sizeof(WAVEHDR)) !=
                             MMSYSERR_NOERROR)
      {
         OOLOG2(1, "ERROR: In preparing wave-in headers");
         return -1;
      }
      if(waveInAddBuffer(ghWaveIn, pWaveHdr, sizeof(WAVEHDR)) !=
                         MMSYSERR_NOERROR)
      {
         OOLOG2(1, "ERROR: In adding wave-in buffers");
         return -1;
      }
      EnterCriticalSection(&gReadMutex);
      gQueuedBufCount++; /* Increment the count of queued buffers */
      LeaveCriticalSection(&gReadMutex);
   }
   OOLOG3(1, "EndOf:OpenMic:gQueuedBufCount:%d", gQueuedBufCount);
   return 0;
}

/* Start the data capture*/
int ooStartMicDataCapture(HWAVEIN hWaveIn)
{
   OOLOG2(1, "StartOf:StartMicDataCapture");
   gRecording = 1;
   if(waveInStart(hWaveIn) != MMSYSERR_NOERROR)
   {
      OOLOG2(1, "ERROR: Error in StartMicDataCapture"); 
      return -1;
   }
   OOLOG2(1, "EndOf:StartMicDataCapture");
   return 0;
}

/* Stop data capture */
int ooStopMicDataCapture(HWAVEIN hWaveIn)
{
   OOLOG2(1, "StartOf:StopMicDataCapture");
   gRecording = 0;
   if(waveInStop(hWaveIn) != MMSYSERR_NOERROR)
   {
      OOLOG2(1, "ERROR: Error in stopping MicDataCapture");
      return -1;
   }
   OOLOG2(1, "EndOf:StopMicDataCapture");
   return 0;
}

/* Close mic device. */
int ooCloseMic(HWAVEIN hWaveIn)
{
   int i = 0;
   struct WaveBuffer *temp;
   OOLOG2(1, "StartOf:CloseMic");
   if(gRecording != 0)
      ooStopMicDataCapture(hWaveIn);
   /* This will ensure that all unutilized buffers are
      marked as done and released by the waveIn device.
   */
   if(waveInReset(hWaveIn) != MMSYSERR_NOERROR)
   {
      OOLOG2(1, "ERROR: In waveInReset call");
      return -1;
   }
   /* Wait till all the buffers are released*/
   for(i=0; i<10; i++)
   {
      if(gQueuedBufCount == 0)
         break;
      OOLOG3(1, "gQueuedBufCount is :%d", gQueuedBufCount);
      Sleep(100);
   }
   if(gQueuedBufCount != 0 )
      OOLOG2(1,"Warn: Not all buffers returned from waveInDevice");
   /* Now close the waveIn device */
   if(waveInClose(hWaveIn) != MMSYSERR_NOERROR)
   {
      OOLOG2(1, "ERROR: In WaveInClose call");
      return -1;
   }
   /* Release the memory */
   while(gpWaveHead != 0)
   {
      temp = gpWaveHead;
      gpWaveHead = gpWaveHead->next;
      if(temp->pWaveHdr)
      {
         if(temp->pWaveHdr->lpData)
            free(temp->pWaveHdr->lpData);
         free(temp->pWaveHdr);
      }
   }
   OOLOG2(1, "EndOf:CloseMic");
   return 0;
}

void CALLBACK ooMICCallback(HWAVEIN hwi, UINT uMsg, DWORD dwInstance,
                            DWORD dwParam1, DWORD dwParam2)
{
   int * bufCnt = (int*)dwInstance;
   /*OOLOG2(1, "StartOf:MICCallback");*/
   if(uMsg == WIM_DATA)
   {
      EnterCriticalSection(&gReadMutex);
       /* Decrement the queued buffer count, as one buffer is released*/
      (*bufCnt)--;
      ooAddToWaveBufferList((WAVEHDR*)dwParam1);
      LeaveCriticalSection(&gReadMutex); 
   }
   /*OOLOG2(1, "EndOf:MICCallback");*/
   return;
}

void CALLBACK ooSpeakerCallback(HWAVEOUT hwo, UINT uMsg, DWORD dwInstance, 
                                DWORD dwParam1, DWORD dwParam2)
{
   int* playCnt = (int*)dwInstance;
   WAVEHDR* pWaveHdr;
   /*OOLOG2(1, "StartOf:SpeakerCallback");*/
    /*
    * ignore calls that occur due to openining and closing the
    * device.
    */
   if(uMsg != WOM_DONE)
      return;

   pWaveHdr = (WAVEHDR*)dwParam1;
   /* Decrement the number of queued buffers in waveout as one buffer
      is done.
   */   
   EnterCriticalSection(&gPlayMutex);
   (*playCnt)--;
/***   ooAddToFreeBufferList(pWaveHdr->lpData);*/
   free(pWaveHdr->lpData);
   free(pWaveHdr);
   LeaveCriticalSection(&gPlayMutex);
  
   /*OOLOG2(1, "EndOf:SpeakerCallback");*/
   return;
}

/* Add buffer to wave header list. */
int ooAddToWaveBufferList(WAVEHDR * waveHdr)
{
   /*OOLOG2(1, "StartOf:AddToWaveBufferList");*/
   /* Allocate memory for the new node */
   struct WaveBuffer * newBuf = (struct WaveBuffer*)
                                 malloc(sizeof(struct WaveBuffer));
   memset(newBuf, 0, sizeof(struct WaveBuffer));
   newBuf->next =0; /*set next to 0 as this will be the last node in the list*/
   newBuf->pWaveHdr = waveHdr;
   if(gpWaveHead == 0) /* List is empty*/
   {
      gpWaveHead = newBuf;
      gpWaveTail = newBuf;
   }
   else /* List has atleast one node*/
   {
      gpWaveTail->next = newBuf; /* Add the new node at the end of the list */
      gpWaveTail = newBuf; /*Change Tail to point to the new end of the list */
   }
   /*OOLOG2(1, "EndOf:AddToWaveBufferList");*/
   return 0;
}

/* Remove bufer from wave header list.
*/
int ooRemoveHeadOfWaveBufferList()
{
   struct WaveBuffer *temp = gpWaveHead;
   /*OOLOG2(1, "StartOf:RemoveHeadOfWaveBufferList"); */
   if(gpWaveHead != 0) /* if head is non-null, then free it*/
   {
      gpWaveHead = gpWaveHead->next;
      if(gpWaveHead == 0) /*This means there was only one node in the list*/
         gpWaveTail = 0;  /* Set tail to null as well */
      free(temp);       /* free the memory hold by earlier waveHead*/
   }
   /*OOLOG2(1, "EndOf:RemoveHeadOfWaveBufferList");*/
   return 0;
}

/* Add buffer to the free buffer list. */
int ooAddToFreeBufferList(char * buffer)
{
   struct BufferNode * newBuf;
   /*OOLOG2(1, "StartOf:AddToFreeBufferList");*/
   if(buffer == 0)
      return 0;
   newBuf = (struct BufferNode*) malloc(sizeof(struct BufferNode));
   memset(newBuf, 0, sizeof(struct BufferNode));

   newBuf->buf = buffer;
   newBuf->next = 0;

   if(gpFreeBufHead == 0) /* List is empty*/
   {
      gpFreeBufHead = newBuf;
      gpFreeBufTail = newBuf;
   }
   else /* List has atleast one node*/
   {
      gpFreeBufTail->next = newBuf;/*Add the new node at the end of the list */
      /*Change Tail to point to the new end of the list */
      gpFreeBufTail = newBuf;
   }
   /*OOLOG2(1, "EndOf:AddToFreeBufferList");*/
   return 0;
}

/* Get a buffer from the list of free buffers*/
char* ooGetFreeBuffer()
{
   char * buf;
   struct BufferNode * temp = gpFreeBufHead;
   /*OOLOG2(1, "StartOf:GetFreeBuffer");*/
   if(gpFreeBufHead == 0) /* If list is empty, return null*/
      return NULL;
   /* if there is only one node in the list, set head and tail to NULL */
   if(gpFreeBufHead == gpFreeBufTail)
   {
      gpFreeBufHead = 0;
      gpFreeBufTail = 0;
   }
   else{ /* Else set head is equal to the head's next*/
      gpFreeBufHead = gpFreeBufHead->next;
   }
   /* Get the pointer to the free buffer*/
   buf = temp->buf;

   /* Free the memory allocated to the node structure*/
   free(temp);
   /*OOLOG2(1, "EndOf:GetFreeBuffer");*/
   return buf; /* return buffer*/
}
