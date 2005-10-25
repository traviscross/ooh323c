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

#include "ooCommon.h"
#include <string.h>
#include "oortp.h"
#include "g711.h"
#include <time.h>
#ifdef _WIN32
#include "ooWave.h"
#else
#include "oomedialx.h"
#endif

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_PTHREAD_H
#include <pthread.h>
#endif

/* Initialize the media plug-in */
int ooInitializePlugin()
{
   /* Open the log file for write */
   fpLog = NULL;
   fpLog = fopen("media.log", "w");
   if(fpLog == NULL)
   {
      printf("\nERROR:Failed to open media.log log file");
      return -1;
   }
  
#ifdef _WIN32
   OOLOG2(1, "Initializing sockets api");
   ooSocketsInit();
#else
   ooOpenAudioDevice();
#endif
   return 0;
}

/* Create a transmit RTP channel. This creates a UDP socket for transmit
   and initializes the seqNo, timeStamp and ssrc for transmit session
*/
int ooCreateTransmitRTPChannel(int* channelId, char * destip, int port)
{
   int ret;
   OOLOG4(1, "StartOf:CreateTransmitRTPChannel:Destination %s:%d", destip,
          port);
#ifdef _WIN32
   InitializeCriticalSection(&gReadMutex);
#endif
   if(destip == 0)
   {
      OOLOG2(1, "ERROR: NULL ip address");
      return -1;
   }
   if(port == 0)
   {
      OOLOG2(1, "ERROR: port can not be zero");
      return -1;
   }
   memset(&gXmitChannel, 0, sizeof(struct OORTPChannel));
   strcpy(gXmitChannel.ip, destip);
   gXmitChannel.port = port;

   /* Create transmit socket */
   ret = ooSocketCreateUDP (&(gXmitChannel.sock));
   if(ret != 0)
   {
      OOLOG2(1, "ERROR: Creation of rtp socket failed");
      return -1;
   }
  
#ifdef _WIN32
   /* Generate a random seqNo to start with */
   /* 65536 is max rtp seqNo after which it is reset*/
   srand((unsigned)time( NULL ) );
   gXmitChannel.seqNo = (unsigned short)((unsigned long)(rand() % 65536));
   /* Assign a random value to synchronization source */
   gXmitChannel.ssrc = (unsigned long)rand();
   /* generate a random timestamp. This will be incremented monotonically*/
   gXmitChannel.timestamp = (unsigned long) rand();
#else
   srandom(time(0));
   gXmitChannel.seqNo = (unsigned short)((unsigned long)(random() % 65536));
   /* Assign a random value to synchronization source */
   gXmitChannel.ssrc = (unsigned long)random();
   /* generate a random timestamp. This will be incremented monotonically*/
   gXmitChannel.timestamp = (unsigned long) random();
#endif  
  
   gXmitThrdHdl = 0;
   gXmitChannel.status = OO_CHAN_OPEN;
   OOLOG2(1, "EndOf:CreateTransmitRTPChannel");
   return 0;
}

/* Close the transmit RTP channel*/
int ooCloseTransmitRTPChannel(int channelId)
{
   int i=0, ret=0;
   OOLOG2(1, "StartOf:CloseTransmitRTPChannel");
   /* First stop the transmit thread if running */
   if(gXmitThrdHdl != 0)
   {
      gXmitFileLoop = 0;
      gXmitMicLoop = 0;
      i = 0;
      while(gXmitThrdHdl != 0)
      {
         ooSleep(1000);
         if(i++ == 3)
         {
            OOLOG2(1, "ERROR: Transmit thread not exiting...");
            exit(0);
         }
      }
   }
   /* Now close the socket */
   if(gXmitChannel.status == OO_CHAN_OPEN)
   {
#ifdef _WIN32
      DeleteCriticalSection(&gReadMutex);
#endif
      ret = ooSocketClose(gXmitChannel.sock);
      if(ret != 0)
      {
         OOLOG2(1, "Warning: Failed to close the xmit RTP socket");
         return -1;
      }
      gXmitChannel.status = OO_CHAN_CLOSE;
   }
   OOLOG2(1, "EndOf:CloseTransmitRTPChannel");
   return 0;
}

/* Create Receive RTP channel. Basically, creates a socket for
   receiving RTP packets. The socket is bound to ip and port which
   will be communicated to the remote endpoint via H.245 signalling
*/

int ooCreateReceiveRTPChannel(int* channelId, char * localip, int localport)
{
   int ret;
   OOIPADDR ipAddr;
   OOLOG4(1, "StartOf:CreateReceiveRTPChannel: local %s:%d", localip,
          localport);
#ifdef _WIN32
   InitializeCriticalSection(&gPlayMutex);
#endif
   memset(&gRecvChannel, 0, sizeof(struct OORTPChannel));
   strcpy(gRecvChannel.ip, localip);
   gRecvChannel.port = localport;
  
   /* Create receive socket */
   ret = ooSocketCreateUDP (&(gRecvChannel.sock));
   if(ret != 0)
   {
      OOLOG2(1, "ERROR: Creation of rtp socket failed");
      return -1;
   }
   ret = ooSocketStrToAddr (localip, &ipAddr);
   if(ret != 0)
   {
      OOLOG2(1, "ERROR: IP address conversion failed");
      return -1;
   }
   ret = ooSocketBind (gRecvChannel.sock, ipAddr, localport);
   if(ret != 0)
   {
      OOLOG2(1, "ERROR: Socket bind failed");
      return -1;
   }
   /* Initailze seqNo, ssrc and timestamp */
   gRecvChannel.seqNo = 0;
   /* Assign a random value to synchronization source */
   gRecvChannel.ssrc = 0;
   /* generate a random timestamp. This will be incremented monotonically*/
   gRecvChannel.timestamp = 0;
   gRecvChannel.status = OO_CHAN_OPEN;
   OOLOG2(1, "EndOf:CreateReceiveRTPChannel:");
   return 0;
}

/* Close the receive RTP channel. Stops any receive thread running on the
   channel and then closes the socket.
*/
int ooCloseReceiveRTPChannel(int channelId)
{
   int i, ret=0;
   OOLOG2(1, "StartOf:CloseReceiveRTPChannel");
   /* Stop the receive thread, if running */
   if(gRecvThrdHdl != 0)
   {
      gRecvPlayLoop = 0;
      i = 0;
      while(gRecvThrdHdl != 0)
      {
         ooSleep(1000);
         if(i++ == 3)
         {
            OOLOG2(1, "ERROR: Receive thread not exiting...");
            exit(0);
         }
      }
   }
   /* Close the socket */
   if(gRecvChannel.status == OO_CHAN_OPEN)
   {
#ifdef _WIN32
      DeleteCriticalSection(&gPlayMutex);
#endif
      ret = ooSocketClose(gRecvChannel.sock);
      if(ret != 0)
      {
         OOLOG2(1, "Warning: Failed to close the receive RTP socket");
         return -1;
      }
      gRecvChannel.status = OO_CHAN_CLOSE;
   }
   OOLOG2(1, "EndOf:CloseReceiveRTPChannel");
   return 0;
}

/* Start transmitting wave file. Note that a transmit RTP channel
   has to be created before using this function.
*/
int ooStartTransmitWaveFile(int channelId, char * filename)
{
   int ret =0;
   OOLOG2(1, "StartOf:StartTransmitWaveFile");
   if(gXmitChannel.status != OO_CHAN_OPEN)
   {
      OOLOG2(1,"ERROR:Transmit RTP channel is not open");
      return -1;
   }
   /* Open the wave file for read */
   ret = ooOpenWaveFileForRead(filename);
   if( ret < 0)
   {
      OOLOG3(1, "ERROR: Failed to open the %s wave file for read", filename);
      return -1;
   }

   /* Create a thread which will read from the wave file and transmit data
      as rtp packets.
   */
#ifdef _WIN32
   gXmitThrdHdl = CreateThread(0, 0,
                    (LPTHREAD_START_ROUTINE)ooTransmitFileThreadFunc, 0, 0, 0);
#else
   pthread_create(&gXmitThrdHdl, NULL, ooTransmitFileThreadFuncLnx, 0);
#endif

   OOLOG2(1, "EndOf:StartTransmitWaveFile");
   return 0;
}
/* Stop the wave file transmission. Basically stops the transmit thread
*/
int ooStopTransmitWaveFile(int channelId)
{
   OOLOG2(1, "StartOf:StopTransmitWaveFile");
   gXmitFileLoop = 0;
   OOLOG2(1, "EndOf:StopTransmitWaveFile");
   return 0;
}

/* Start capturing data from microphone and transmitting as RTP packets.
*/  
int ooStartTransmitMic(int channelId)
{
   int ret = 0;
   OOLOG2(1, "StartOf:StartTransmitMic");
#ifdef _WIN32
   ret = ooOpenMic(); /* Open the Mic device for read */
   if( ret < 0)
   {
      OOLOG2(1, "ERROR: Failed to open the Mic device");
      return ret;
   }
   /* Start thread which will read data from Mic, and transmit it as RTP
      packets.
   */  
   gXmitThrdHdl = CreateThread(0, 0,
                    (LPTHREAD_START_ROUTINE)ooTransmitMicThreadFunc, 0, 0, 0);
#else
   pthread_create(&gXmitThrdHdl, NULL, ooTransmitMicThreadFuncLnx, 0);
#endif
   OOLOG2(1, "EndOf:StartTransmitMic");
   return ret;
}

/* Stop the transmit Mic thread */
int ooStopTransmitMic(int channelId)
{
   OOLOG2(1, "StartOf:StopTransmitMic");
   gXmitMicLoop = 0;
   OOLOG2(1, "EndOf:StopTransmitMic");
   return 0;
}

/* Start receiving audio on Receive RTP channel and playing it on
   speakers.
*/
int ooStartReceiveAudioAndPlayback(int channelId)
{
 
#ifdef _WIN32
   int ret;
   WAVEFORMATEX waveFormat;
  
   /* Wave format of the audio to be played.*/
   waveFormat.wFormatTag = WAVE_FORMAT_PCM;
   waveFormat.nChannels = 1;
   waveFormat.nSamplesPerSec = 8000;
   waveFormat.nAvgBytesPerSec = 16000;
   waveFormat.nBlockAlign = 2;
   waveFormat.wBitsPerSample = 16;
   waveFormat.cbSize = 0;
#endif

   OOLOG2(1, "StartOf:StartReceiveAudioAndPlayback");
#ifdef _WIN32
   /*Open the speaker device */
   ret = ooOpenSpeaker(&ghWaveOut, waveFormat);
   if(ret != 0)
   {
      OOLOG2(1, "ERROR: Failed to open the Wave-out device");
      return -1;
   }

   /*Create  a thread which will receive rtp packets and playback the audio
     on the speaker device.
   */
   gRecvThrdHdl = CreateThread(0, 0,
                  (LPTHREAD_START_ROUTINE)ooReceiveSpeakerThreadFunc, 0, 0, 0);
#else
   pthread_create(&gRecvThrdHdl, NULL, ooReceiveSpeakerThreadFuncLnx, 0);
#endif
   OOLOG2(1, "EndOf:StartReceiveAudioAndPlayback");
   return 0;
}

/* Stops the receive and playback audio thread. */
int ooStopReceiveAudioAndPlayback(int channelId)
{
   OOLOG2(1, "StartOf:StopReceiveAudioAndPlayback");
   gRecvPlayLoop = 0;
   OOLOG2(1, "EndOf:StopReceiveAudioAndPlayback");
   return 0;
}

#ifdef _WIN32
/* Thread function to receive RTP data and play it onto the
   speaker device.
*/
int ooReceiveSpeakerThreadFunc()
{
   int ret = 0, i;
   char * pcSndBuf;
   short * psSndBuf;
   struct timeval timeout;
   fd_set readfds;
   char buffer[1024];
   OOLOG2(1, "StartOf:ReceiveSpeakerThread");
   gRecvPlayLoop = 1; /* start recv loop */
   while(1)
   {
      if(!gRecvPlayLoop)
      {
         OOLOG2(1, "Exiting from RecvPlay loop ");
         break;
      }
      FD_ZERO(&readfds);
      FD_SET(gRecvChannel.sock, &readfds);
      timeout.tv_sec = 1;
      timeout.tv_usec = 0;
      /* Check whether there is data for read */
      ret = ooSocketSelect(gRecvChannel.sock+1, &readfds, 0,
                           0, &timeout);

      if(FD_ISSET(gRecvChannel.sock, &readfds))
      {
         pcSndBuf = (char*) malloc(1024);
         if(pcSndBuf == 0)
         {
            OOLOG2(1, "ERROR: Memalloc for playback buffer failed");
         }
         else{
            ret = ooSocketRecvFrom (gRecvChannel.sock, buffer,
                                    1024, 0, 0);
                       
            /* We have a RTP packet in buffer. Assume that data always begins
               from 12th octet, since standard RTP header length is 12.
               Convert G711 data to 16 bit PCM.
            */
           
            psSndBuf = (short*)pcSndBuf;
            for( i =12; i < ret; i++)
            {               
               psSndBuf[i-12] = (short)ulaw2linear((unsigned char)(buffer[i]));
               /*c = (short)ulaw2linear((unsigned char)(buffer[i]));
               pcSndBuf[j++]= (unsigned char) ((c>>8) & 0xff);
               pcSndBuf[j++] = (unsigned char) (c & 0xff);*/
            }
            /* Play the data buffer onto the speaker device*/
            /*ooPlayWaveBuffer(ghWaveOut, (char*)pcSndBuf, j);*/
            ooPlayWaveBuffer(ghWaveOut, (char*)psSndBuf, (i-12)*sizeof(short));
            pcSndBuf =0;
         }
      }
   }
   /*Close speaker*/
   if(ooCloseSpeaker(ghWaveOut) != 0)
      OOLOG2(1, "ERROR:Closing Speaker device");
   gRecvThrdHdl = 0;
   ExitThread(0);
   OOLOG2(1, "EndOf:ReceiveSpeakerThread");
   return 0;
}


/* Read the wave file and transmit wave file data
   as rtp packets on RTP transmit channel.
*/
int ooTransmitFileThreadFunc()
{
   int DataTxed=0;
   long audioDataSize;
   int ret = 0;
   char databuf[480];
   short *tempBuf;
   unsigned char sendBuf[252]; /* additional 12 bytes for RTP header */
   int marker = 1; /* Only first packet has marker bit set */
   int i;
   OOLOG2(1, "StartOf:TransmitFileThread");
  
   gXmitFileLoop = 1;
   while(1)
   {
      if(!gXmitFileLoop)
         break;
      /* Number of bytes read is returned and stored in audioDataSize */

      audioDataSize = ooReadWaveFileData(databuf, 480);

      if(audioDataSize>0) /* If any data is read */
      {
         DataTxed += audioDataSize;
         /* RTP version = 2, padding = 0, extension = 0, CSRC = 0 */
         sendBuf[0] = 0x80;
         sendBuf[1] = 0x00; /* RTP Payload type PCM ulaw Need to use #define */
         if(marker)
         {
            sendBuf[1] |= 0x80; /* Set marker bit for first packet */
            marker = 0;
         }
         /*packet sequence number*/
         *((short *)(sendBuf + 2)) = ooHTONS((short)gXmitChannel.seqNo);
         /*packet timestamp*/
         *((long  *)(sendBuf + 4)) = ooHTONL((long)gXmitChannel.timestamp);
         /*packet ssrc*/
         *((long  *)(sendBuf + 8)) = ooHTONL((long)gXmitChannel.ssrc);
        
         if(gXmitChannel.seqNo == 65535)
            gXmitChannel.seqNo = 0; /* Seq num wrap around */
         else
            gXmitChannel.seqNo += 1;

         /* Next timestamp is current timestamp + play duration of the
            current buffer.
         */
         gXmitChannel.timestamp += 480;
         tempBuf = (short*) databuf;
         /* Copy PCM audio data into sendBuf as ulaw audio data*/
         for(i=0; i<(audioDataSize/2); i++)
         {
            sendBuf[12+i] = (unsigned char) linear2ulaw((int)*(tempBuf+i));
         }
         /* transmit rtp packet */
         ret = ooSocketSendTo(gXmitChannel.sock, sendBuf,
                     (audioDataSize/2+12), gXmitChannel.ip, gXmitChannel.port);
         if(ret<0)
         {
            OOLOG2(1, "ERROR: Failed to transmit rtp packet");
            return -1;
         }
         Sleep(2);/* Just slows down the transmit rate a bit */
      }
      else{
         if(DataTxed == gWaveFile.dataSize)
         {
            printf("File transmission over\n");
            gXmitFileLoop = 0;
         }
         else{
            OOLOG2(1, "ERROR: Data read failure");
            gXmitFileLoop = 0;
         }
      }
   }
   /* Close the wave file */
   ret = ooCloseWaveFile();
   if(ret < 0)
   {
      OOLOG2(1, "ERROR: Failed to close the open wave file");
      return -1;
   }
   gXmitThrdHdl = 0;
   ExitThread(0);
   OOLOG2(1, "EndOf:TransmitFileThread");
   return 0;
}

/* Thread function to read data from microphone and transmit it
   as rtp packets.
*/
int ooTransmitMicThreadFunc()
{
   unsigned char sendBuf[252];
   short* tempBuf;
   int marker = 1;
   unsigned int i;
   int ret;
   OOLOG2(1, "StartOf:TransmitMicThread");
   /*   ret = ooOpenMic();// Open the Mic device
   if(ret < 0)
   {
      OOLOG2(1, "ERROR: Opening the Mic device");
      return -1;
   }*/
   /* Start data capture */
   ooStartMicDataCapture(ghWaveIn);
   gXmitMicLoop = 1;
   while(1)
   {
      if(!gXmitMicLoop)
         break;
      if(gpWaveHead != 0)
      {
         memset(sendBuf, 0, 252);
         /* RTP version = 2, padding = 0, extension = 0, CSRC = 0 */
         sendBuf[0] = 0x80;
         /* RTP Payload type PCM ulaw Need to use #define */
         sendBuf[1] = 0x00;
         if(marker)
         {
            sendBuf[1] |= 0x80; /* Set marker bit for first packet */
            marker = 0;
         }
         /*packet sequence number*/
         *((short *)(sendBuf + 2)) = ooHTONS((short)gXmitChannel.seqNo);
         /*packet timestamp*/
         *((long  *)(sendBuf + 4)) = ooHTONL((long)gXmitChannel.timestamp);
         /*packet ssrc*/
         *((long  *)(sendBuf + 8)) = ooHTONL((long)gXmitChannel.ssrc);
        
         if(gXmitChannel.seqNo == 65535)
            gXmitChannel.seqNo = 0; /* Seq num wrap around */
         else
            gXmitChannel.seqNo += 1;

         /* Next timestamp is current timestamp + play duration of the
            current buffer
         */
         gXmitChannel.timestamp += OORTPPACKETDATASIZE;
              
         /* Convert data into g711 ulaw and copy to sendBuf */
         tempBuf = (short*)gpWaveHead->pWaveHdr->lpData;
         for(i=0; i<(gpWaveHead->pWaveHdr->dwBytesRecorded/2); i=i+1)
         {
            sendBuf[12+i] = (unsigned char) linear2ulaw((int)*(tempBuf+i));
         }
         /* transmit rtp packet */
         ret = ooSocketSendTo(gXmitChannel.sock, sendBuf,
                              (gpWaveHead->pWaveHdr->dwBytesRecorded/2+12),
                              gXmitChannel.ip, gXmitChannel.port);
         if(ret<0)
         {
            OOLOG2(1, "ERROR: Failed to transmit rtp packet");
            return -1;
         }
         /* Requeue the buffer into the Mic device for data capture. */
         if(waveInAddBuffer(ghWaveIn, gpWaveHead->pWaveHdr,
                            sizeof(WAVEHDR)) != MMSYSERR_NOERROR)
         {
            OOLOG2(1, "ERROR: In adding wave-in buffers");
            return -1;
         }
         EnterCriticalSection(&gReadMutex);
         gQueuedBufCount++; /* Increase the count of queued buffers by one*/
         ooRemoveHeadOfWaveBufferList();
         LeaveCriticalSection(&gReadMutex);
      }
      else
         Sleep(5); /*else sleep for 5 ms*/
   }/*End of while loop*/
   ooCloseMic(ghWaveIn);
   gXmitThrdHdl = 0;
   ExitThread(0);
   OOLOG2(1, "EndOf:TransmitMicThread");
   return 0;
}
#endif
#ifndef _WIN32
/* Thread function to read data from microphone and transmit it
   as rtp packets.
*/
void* ooTransmitMicThreadFuncLnx(void *dummy)
{
   unsigned char sendBuf[252];
   short buffer[OORTPPACKETDATASIZE];
   int marker = 1;
   unsigned int i;
   int bytesRead, ret;
   OOLOG2(1, "StartOf:TransmitMicThread");
  
   gXmitMicLoop = 1;
   while(1)
   {
      if(!gXmitMicLoop)
         break;
      memset(sendBuf, 0, 252);
      /* RTP version = 2, padding = 0, extension = 0, CSRC = 0 */
      sendBuf[0] = 0x80;
      /* RTP Payload type PCM ulaw Need to use #define */
      sendBuf[1] = 0x00;
      if(marker)
      {
         sendBuf[1] |= 0x80; /* Set marker bit for first packet */
         marker = 0;
      }
      /*packet sequence number*/
      *((short *)(sendBuf + 2)) = ooHTONS((short)gXmitChannel.seqNo);
      /*packet timestamp*/
      *((long  *)(sendBuf + 4)) = ooHTONL((long)gXmitChannel.timestamp);
      /*packet ssrc*/
      *((long  *)(sendBuf + 8)) = ooHTONL((long)gXmitChannel.ssrc);
        
      if(gXmitChannel.seqNo == 65535)
         gXmitChannel.seqNo = 0; /* Seq num wrap around */
      else
         gXmitChannel.seqNo += 1;

      /* Next timestamp is current timestamp + play duration of the
         current buffer
      */
      gXmitChannel.timestamp += OORTPPACKETDATASIZE;
              
      /* Convert data into g711 ulaw and copy to sendBuf */
      bytesRead = ooGetMicAudioBuffer((unsigned char*)buffer,
                                          OORTPPACKETDATASIZE*sizeof(short));
      if(bytesRead > 0)
      {
         for(i=0; i<(bytesRead/sizeof(short)); i++)
            sendBuf[12+i] = (unsigned char) linear2ulaw((short)buffer[i]);


         /* transmit rtp packet */
         ret = ooSocketSendTo(gXmitChannel.sock, sendBuf,
                                    (bytesRead/sizeof(short)+12),
                                    gXmitChannel.ip, gXmitChannel.port);

         if(ret<0)
         {
            OOLOG2(1, "ERROR: Failed to transmit rtp packet");
            return dummy;
         }
         ooSleep(5); /*sleep for 5 ms*/
      }
   }/*End of while loop*/
  
   gXmitThrdHdl = 0;
   pthread_exit(0);
   OOLOG2(1, "EndOf:TransmitMicThread");
   return dummy;
}

/* Thread function to receive RTP data and play it onto the
   speaker device.
*/

void* ooReceiveSpeakerThreadFuncLnx(void *dummy)
{
   int ret = 0, i, j;
   short psSndBuf[OORTPPACKETDATASIZE];
   struct timeval timeout;
   fd_set readfds;
   char buffer[1024];
   OOLOG2(1, "StartOf:ReceiveSpeakerThread");
   gRecvPlayLoop = 1; /* start recv loop */
   while(1)
   {
      if(!gRecvPlayLoop)
      {
         OOLOG2(1, "Exiting from RecvPlay loop ");
         break;
      }
      FD_ZERO(&readfds);
      FD_SET(gRecvChannel.sock, &readfds);
      timeout.tv_sec = 1;
      timeout.tv_usec = 0;
      /* Check whether there is data for read */
      ret = ooSocketSelect(gRecvChannel.sock+1, &readfds, 0,
                           0, &timeout);

      if(FD_ISSET(gRecvChannel.sock, &readfds))
      {
         ret = ooSocketRecvFrom (gRecvChannel.sock, buffer,
                                 1024, 0, 0);
                       
         /* We have a RTP packet in buffer. Assume that data always begins
            from 12th octet, since standard RTP header length is 12.
            Convert G711 data to 16 bit PCM.
         */
         j = 0;
         for( i =12; i < ret; i++)
         {                          
            psSndBuf[j++] = (short)ulaw2linear((unsigned char)(buffer[i]));
            /*c = (short)ulaw2linear((unsigned char)(buffer[i]));
             pcSndBuf[j++]= (unsigned char) ((c>>8) & 0xff);
             pcSndBuf[j++] = (unsigned char) (c & 0xff);
            */
         }
         /* Play the data buffer onto the speaker device*/
         ooPlayAudioBuffer((char*)psSndBuf, j*sizeof(short));
         /*        ooPlayAudioBuffer(pcSndBuf, j);*/
      }
   }
  
   gRecvThrdHdl = 0;
   pthread_exit(0);  
   OOLOG2(1, "EndOf:ReceiveSpeakerThread");
   return dummy;
}


/* Read the wave file and transmit wave file data
   as rtp packets on RTP transmit channel.
*/
void* ooTransmitFileThreadFuncLnx(void *dummy)
{
   int DataTxed=0;
   long audioDataSize;
   int ret = 0;
   char databuf[480];
   short *tempBuf;
   unsigned char sendBuf[252]; /* additional 12 bytes for RTP header */
   int marker = 1; /* Only first packet has marker bit set */
   int i;
   OOLOG2(1, "StartOf:TransmitFileThread");
  
   gXmitFileLoop = 1;
   while(1)
   {
      if(!gXmitFileLoop)
         break;
      /* Number of bytes read is returned and stored in audioDataSize */

      audioDataSize = ooReadWaveFileData(databuf, 480);

      if(audioDataSize>0) /* If any data is read */
      {
         DataTxed += audioDataSize;
         /* RTP version = 2, padding = 0, extension = 0, CSRC = 0 */
         sendBuf[0] = 0x80;
         sendBuf[1] = 0x00; /* RTP Payload type PCM ulaw Need to use #define */
         if(marker)
         {
            sendBuf[1] |= 0x80; /* Set marker bit for first packet */
            marker = 0;
         }
         /*packet sequence number*/
         *((short *)(sendBuf + 2)) = ooHTONS((short)gXmitChannel.seqNo);
         /*packet timestamp*/
         *((long  *)(sendBuf + 4)) = ooHTONL((long)gXmitChannel.timestamp);
         /*packet ssrc*/
         *((long  *)(sendBuf + 8)) = ooHTONL((long)gXmitChannel.ssrc);
        
         if(gXmitChannel.seqNo == 65535)
            gXmitChannel.seqNo = 0; /* Seq num wrap around */
         else
            gXmitChannel.seqNo += 1;

         /* Next timestamp is current timestamp + play duration of the
            current buffer.
         */
         gXmitChannel.timestamp += 480;
         tempBuf = (short*) databuf;
         /* Copy PCM audio data into sendBuf as ulaw audio data*/
         for(i=0; i<(audioDataSize/2); i++)
         {
            sendBuf[12+i] = (unsigned char) linear2ulaw((int)*(tempBuf+i));
         }
         /* transmit rtp packet */
         ret = ooSocketSendTo(gXmitChannel.sock, sendBuf,
                     (audioDataSize/2+12), gXmitChannel.ip, gXmitChannel.port);
         if(ret<0)
         {
            OOLOG2(1, "ERROR: Failed to transmit rtp packet");
            return dummy;
         }
         ooSleep(2);/* Just slows down the transmit rate a bit */
      }
      else{
         gXmitFileLoop = 0;
         OOLOG2(1, "End of file transmission");
      }
   }
   /* Close the wave file */
   ret = ooCloseWaveFile();
   if(ret < 0)
   {
      OOLOG2(1, "ERROR: Failed to close the open wave file");
      return dummy;
   }
   gXmitThrdHdl = 0;
   pthread_exit(0);  
   OOLOG2(1, "EndOf:TransmitFileThread");
   return dummy;
}

#endif
