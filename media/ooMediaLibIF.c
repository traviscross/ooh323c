/*
 * Copyright (C) 2004-2010 by Objective Systems, Inc.
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

#include "ooMediaLibIF.h"
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

static OOBOOL checkPlugin (void* addr, const char* name)
{
   if (0 == addr) {
      printf ("ERROR: Plugin %s not available\n", name);
      return FALSE;
   }
   return TRUE;
}

int ooLoadSndRTPPlugin (const char* name)
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
   FuncPtr_InitializePlugin = (MediaAPI_InitializePlugin)
      OODLSYM (media, "ooInitializePlugin");

   FuncPtr_CreateTxRTPChan = (MediaAPI_CreateTxRTPChan)
      OODLSYM (media, "ooCreateTransmitRTPChannel");

   FuncPtr_CloseTxRTPChan = (MediaAPI_CloseTxRTPChan)
      OODLSYM (media, "ooCloseTransmitRTPChannel");

   FuncPtr_CreateRecvRTPChan = (MediaAPI_CreateRecvRTPChan)
      OODLSYM (media, "ooCreateReceiveRTPChannel");

   FuncPtr_CloseRecvRTPChan = (MediaAPI_CloseRecvRTPChan)
      OODLSYM (media, "ooCloseReceiveRTPChannel");

   FuncPtr_StartTxWaveFile = (MediaAPI_StartTxWaveFile)
      OODLSYM (media, "ooStartTransmitWaveFile");

   FuncPtr_StopTxWaveFile = (MediaAPI_StopTxWaveFile)
      OODLSYM (media, "ooStopTransmitWaveFile");

   FuncPtr_StartTxMic = (MediaAPI_StartTxMic)
      OODLSYM (media, "ooStartTransmitMic");

   FuncPtr_StopTxMic = (MediaAPI_StopTxMic)
      OODLSYM (media, "ooStopTransmitMic");

   FuncPtr_StartRecvAndPlayback = (MediaAPI_StartRecvAndPlayback)
      OODLSYM (media, "ooStartReceiveAudioAndPlayback");

   FuncPtr_StopRecvAndPlayback = (MediaAPI_StopRecvAndPlayback)
      OODLSYM (media, "ooStopReceiveAudioAndPlayback");

   /* Initialize the plugin */
   if (checkPlugin (FuncPtr_InitializePlugin, "ooInitializePlugin")) {
      FuncPtr_InitializePlugin();
      return OO_OK;
   }
   else return OO_FAILED;
}


int ooCreateTransmitRTPChannel (const char* destip, int port)
{
   int channelId = 0;
   if (!checkPlugin (FuncPtr_CreateTxRTPChan, "ooCreateTransmitRTPChannel")) {
      return OO_FAILED;
   }
   return FuncPtr_CreateTxRTPChan(&channelId, destip, port);
}

int ooCloseTransmitRTPChannel()
{
   int channelId = 0;
   if (!checkPlugin (FuncPtr_CloseTxRTPChan, "ooCloseTransmitRTPChannel")) {
      return OO_FAILED;
   }
   return FuncPtr_CloseTxRTPChan(channelId);
}

int ooCreateReceiveRTPChannel (const char* localip, int localport)
{
   int channelId = 0;
   if (!checkPlugin (FuncPtr_CreateRecvRTPChan, "ooCreateReceiveRTPChannel")) {
      return OO_FAILED;
   }
   return FuncPtr_CreateRecvRTPChan(&channelId, localip, localport);
}

int ooCloseReceiveRTPChannel()
{
   int channelId = 0;
   if (!checkPlugin (FuncPtr_CloseRecvRTPChan, "ooCloseReceiveRTPChannel")) {
      return OO_FAILED;
   }
   return FuncPtr_CloseRecvRTPChan(channelId);
}

int ooStartTransmitWaveFile (const char* filename)
{
   int channelId = 0;
   if (!checkPlugin (FuncPtr_StartTxWaveFile, "ooStartTransmitWaveFile")) {
      return OO_FAILED;
   }
   return FuncPtr_StartTxWaveFile(channelId, filename);
}

int ooStopTransmitWaveFile()
{
   int channelId = 0;
   if (!checkPlugin (FuncPtr_StopTxWaveFile, "ooStopTransmitWaveFile")) {
      return OO_FAILED;
   }
   return FuncPtr_StopTxWaveFile(channelId);
}

int ooStartTransmitMic()
{
   int channelId = 0;
   if (!checkPlugin (FuncPtr_StartTxMic, "ooStartTransmitMic")) {
      return OO_FAILED;
   }
   return FuncPtr_StartTxMic(channelId);
}

int ooStopTransmitMic()
{
   int channelId = 0;
   if (!checkPlugin (FuncPtr_StopTxMic, "ooStopTransmitMic")) {
      return OO_FAILED;
   }
   return FuncPtr_StopTxMic(channelId);
}

int ooStartReceiveAudioAndPlayback()
{
   int channelId = 0;
   if (!checkPlugin (FuncPtr_StartRecvAndPlayback,
                     "ooStartReceiveAudioAndPlayback")) {
      return OO_FAILED;
   }
   return FuncPtr_StartRecvAndPlayback(channelId);
}

int ooStopReceiveAudioAndPlayback()
{
   int channelId = 0;
   if (!checkPlugin (FuncPtr_StopRecvAndPlayback,
                     "ooStopReceiveAudioAndPlayback")) {
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
