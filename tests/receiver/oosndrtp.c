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

#include "oosndrtp.h"
#include "ooCommon.h"


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

int ooLoadSndRTPPlugin(char * name)
{
   media = 0;
   /* Load the plugin library */
#ifdef _WIN32
   media = LoadLibrary(name);
#else
   media = dlopen(name, RTLD_NOW|RTLD_GLOBAL);
#endif
   if(media == 0)
   {
      printf("ERROR:Failed to load plug-in library %s\n", name);
      return OO_FAILED;
   }
  
   /* Retrieve addreses of the plugin interface functions */
#ifdef _WIN32
   FuncPtr_InitializePlugin = (MediaAPI_InitializePlugin) GetProcAddress(media,
                                                         "ooInitializePlugin");
#else
   FuncPtr_InitializePlugin = (MediaAPI_InitializePlugin) dlsym(media,
                                                         "ooInitializePlugin");
#endif
   if(FuncPtr_InitializePlugin == 0)
   {
      printf("ERROR: Failed to retrieve the address of "
                           "ooInitializePlugin\n");
      return OO_FAILED;
   }

#ifdef _WIN32
   FuncPtr_CreateTxRTPChan = (MediaAPI_CreateTxRTPChan) GetProcAddress(media,
                                                 "ooCreateTransmitRTPChannel");
#else
   FuncPtr_CreateTxRTPChan = (MediaAPI_CreateTxRTPChan) dlsym(media,
                                                 "ooCreateTransmitRTPChannel");
#endif
   if(FuncPtr_CreateTxRTPChan == 0)
   {
      printf("ERROR: Failed to retrieve the address of "
                           "ooCreateTransmitRTPChannel\n");
      return OO_FAILED;
   }
  
#ifdef _WIN32
   FuncPtr_CloseTxRTPChan = (MediaAPI_CloseTxRTPChan) GetProcAddress(media,
                                                 "ooCloseTransmitRTPChannel");
#else
   FuncPtr_CloseTxRTPChan = (MediaAPI_CloseTxRTPChan) dlsym(media,
                                                 "ooCloseTransmitRTPChannel");
#endif
   if(FuncPtr_CloseTxRTPChan == 0)
   {
      printf("ERROR: Failed to retrieve the address of "
                           "ooCloseTransmitRTPChannel\n");
      return OO_FAILED;
   }
  
#ifdef _WIN32
   FuncPtr_CreateRecvRTPChan = (MediaAPI_CreateRecvRTPChan) GetProcAddress
                                          (media, "ooCreateReceiveRTPChannel");
#else
   FuncPtr_CreateRecvRTPChan = (MediaAPI_CreateRecvRTPChan) dlsym(media,
                                                 "ooCreateReceiveRTPChannel");
#endif
   if(FuncPtr_CreateRecvRTPChan == 0)
   {
      printf("ERROR: Failed to retrieve the address of "
                           "ooCreateReceiveRTPChannel\n");
      return OO_FAILED;
   }
  
#ifdef _WIN32
   FuncPtr_CloseRecvRTPChan = (MediaAPI_CloseRecvRTPChan)GetProcAddress(media,
                                                 "ooCloseReceiveRTPChannel");
#else
   FuncPtr_CloseRecvRTPChan = (MediaAPI_CloseRecvRTPChan) dlsym(media,
                                                 "ooCloseReceiveRTPChannel");
#endif
   if(FuncPtr_CloseRecvRTPChan == 0)
   {
      printf("ERROR: Failed to retrieve the address of "
                           "ooCloseReceiveRTPChannel\n");
      return OO_FAILED;
   }

#ifdef _WIN32
   FuncPtr_StartTxWaveFile = (MediaAPI_StartTxWaveFile) GetProcAddress(media,
                                                  "ooStartTransmitWaveFile");
#else
   FuncPtr_StartTxWaveFile = (MediaAPI_StartTxWaveFile) dlsym(media,
                                                 "ooStartTransmitWaveFile");
#endif
   if(FuncPtr_StartTxWaveFile == 0)
   {
      printf("ERROR: Failed to retrieve the address of "
                           "ooStartTransmitWaveFile\n");
      return OO_FAILED;
   }
  
#ifdef _WIN32
   FuncPtr_StopTxWaveFile = (MediaAPI_StopTxWaveFile) GetProcAddress(media,
                                                  "ooStopTransmitWaveFile");
#else
   FuncPtr_StopTxWaveFile = (MediaAPI_StopTxWaveFile) dlsym(media,
                                                 "ooStopTransmitWaveFile");
#endif

   if(FuncPtr_StopTxWaveFile == 0)
   {
      printf("ERROR: Failed to retrieve the address of "
                           "ooStopTransmitWaveFile\n");
      return OO_FAILED;
   }

#ifdef _WIN32
   FuncPtr_StartTxMic = (MediaAPI_StartTxMic) GetProcAddress(media,
                                                  "ooStartTransmitMic");
#else
   FuncPtr_StartTxMic = (MediaAPI_StartTxMic) dlsym(media,
                                                 "ooStartTransmitMic");
#endif
   if(FuncPtr_StartTxMic == 0)
   {
      printf("ERROR: Failed to retrieve the address of "
                           "ooStartTransmitMic\n");
      return OO_FAILED;
   }
  
#ifdef _WIN32
   FuncPtr_StopTxMic = (MediaAPI_StopTxMic) GetProcAddress(media,
                                                  "ooStopTransmitMic");
#else
   FuncPtr_StopTxMic = (MediaAPI_StopTxMic) dlsym(media,
                                                 "ooStopTransmitMic");
#endif
   if(FuncPtr_StopTxMic == 0)
   {
      printf("ERROR: Failed to retrieve the address of "
                           "ooStopTransmitMic\n");
      return OO_FAILED;
   }

#ifdef _WIN32
   FuncPtr_StartRecvAndPlayback =(MediaAPI_StartRecvAndPlayback)GetProcAddress
                                     (media, "ooStartReceiveAudioAndPlayback");
#else
   FuncPtr_StartRecvAndPlayback = (MediaAPI_StartRecvAndPlayback) dlsym(media,
                                             "ooStartReceiveAudioAndPlayback");
#endif
   if(FuncPtr_StartRecvAndPlayback == 0)
   {
      printf("ERROR: Failed to retrieve the address of "
                           "ooStartReceiveAudioAndPlayback\n");
      return OO_FAILED;
   }

#ifdef _WIN32
   FuncPtr_StopRecvAndPlayback = (MediaAPI_StopRecvAndPlayback) GetProcAddress
                                      (media, "ooStopReceiveAudioAndPlayback");
#else
   FuncPtr_StopRecvAndPlayback = (MediaAPI_StopRecvAndPlayback) dlsym(media,
                                              "ooStopReceiveAudioAndPlayback");
#endif

   if(FuncPtr_StopRecvAndPlayback == 0)
   {
      printf("ERROR: Failed to retrieve the address of "
                           "ooStopRecvAndPlayback\n");
      return OO_FAILED;
   }
  
   /* Initialize the plugin */
   FuncPtr_InitializePlugin();

   return OO_OK;
}


int ooCreateTransmitRTPChannel(char * destip,
                               int port)
{
   int channelId=0;
   if(FuncPtr_CreateTxRTPChan == 0)
   {
      printf("ERROR: Plugin not available\n");
      return OO_FAILED;
   }
   return FuncPtr_CreateTxRTPChan(&channelId, destip, port);

}

int ooCloseTransmitRTPChannel()
{
   int channelId=0;
   if(FuncPtr_CloseTxRTPChan == 0)
   {
      printf("ERROR: Plugin not available\n");
      return OO_FAILED;
   }
   return FuncPtr_CloseTxRTPChan(channelId);
}

int ooCreateReceiveRTPChannel(char* localip,
                              int localport)
{
   int channelId = 0;
   if(FuncPtr_CreateRecvRTPChan == 0)
   {
      printf("ERROR: Plugin not available\n");
      return OO_FAILED;
   }
   return FuncPtr_CreateRecvRTPChan(&channelId, localip, localport);
}

int ooCloseReceiveRTPChannel()
{
   int channelId = 0;
   if(FuncPtr_CloseRecvRTPChan == 0)
   {
      printf("ERROR: Plugin not available\n");
      return OO_FAILED;
   }
   return FuncPtr_CloseRecvRTPChan(channelId);
}

int ooStartTransmitWaveFile(char * filename)
{
   int channelId = 0;
   if(FuncPtr_StartTxWaveFile == 0)
   {
      printf("ERROR: Plugin not available\n");
      return OO_FAILED;
   }
   return FuncPtr_StartTxWaveFile(channelId, filename);
}

int ooStopTransmitWaveFile()
{
   int channelId = 0;
   if(FuncPtr_StopTxWaveFile == 0)
   {
      printf("ERROR: Plugin not available\n");
      return OO_FAILED;
   }
   return FuncPtr_StopTxWaveFile(channelId);
}

int ooStartTransmitMic()
{
   int channelId = 0;
   if(FuncPtr_StartTxMic == 0)
   {
      printf("ERROR: Plugin not available\n");
      return OO_FAILED;
   }
   return FuncPtr_StartTxMic(channelId);
}

int ooStopTransmitMic()
{
   int channelId = 0;
   if(FuncPtr_StopTxMic == 0)
   {
      printf("ERROR: Plugin not available\n");
      return OO_FAILED;
   }
   return FuncPtr_StopTxMic(channelId);
}

int ooStartReceiveAudioAndPlayback()
{
   int channelId = 0;
   if(FuncPtr_StartRecvAndPlayback == 0)
   {
      printf("ERROR: Plugin not available\n");
      return OO_FAILED;
   }
   return FuncPtr_StartRecvAndPlayback(channelId);
}

int ooStopReceiveAudioAndPlayback()
{
   int channelId = 0;
   if(FuncPtr_StopRecvAndPlayback == 0)
   {
      printf("ERROR: Plugin not available\n");
      return OO_FAILED;
   }
   return FuncPtr_StopRecvAndPlayback(channelId);
}


int ooRTPShutDown()
{
   int ret = 0;
   ret = ooCloseTransmitRTPChannel();
   if(ret != 0)
   {
      printf("ERROR: Closing Transmit RTP Channel\n");
      return OO_FAILED;
   }
   ret = ooCloseReceiveRTPChannel();
   if(ret != 0)
   {
      printf("ERROR: Closing Receive RTP Channel\n");
      return OO_FAILED;
   }
   return OO_OK;
}

int ooReleaseSndRTPPlugin()
{
#ifdef _WIN32
   if(!FreeLibrary(media))
   {
      printf("Warn: Failed to release the plugin\n");
      return OO_FAILED;
   }
   else
      printf("INFO: Plugin released from memory\n");
#else
   if(dlclose(media) != 0)
   {
      printf("Warn: Failed to release the plugin\n");
      return OO_FAILED;
   }
   else
      printf("INFO: Plugin released from memory\n");
#endif
   media = 0;
   FuncPtr_InitializePlugin = 0;
   FuncPtr_CreateTxRTPChan = 0;
   FuncPtr_CloseTxRTPChan = 0;
   FuncPtr_CreateRecvRTPChan = 0;
   FuncPtr_CloseRecvRTPChan = 0;
   FuncPtr_StartTxWaveFile = 0;
   FuncPtr_StopTxWaveFile = 0;
   FuncPtr_StartTxMic = 0;
   FuncPtr_StopTxMic = 0;
   FuncPtr_StartRecvAndPlayback = 0;
   FuncPtr_StopRecvAndPlayback = 0;

   return OO_OK;
}

/*int ooSetLocalRTPAndRTCPAddrs()
{
   int port;
   ooGetLocalIPAddress(context->localIP);
   port = ooGetNextPort(context, OORTP);*/
   // Ensures that RTP port is an even one
/*   if((port & 1) == 1)
     port = ooGetNextPort(context, OORTP);
  
   context->localRtpPort = port;
   context->localRtcpPort = ooGetNextPort(context, OORTP);
   OOTRACEINFO4(context, "Local RTP port: %d Local RTCP port:%d",
                          context->localRtpPort, context->localRtcpPort);
   return OO_OK;
}

*/
