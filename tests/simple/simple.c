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

#include "ootypes.h"
#include "oo.h"
#include "oochannels.h"
#include "ooh323ep.h"
#include "ooCalls.h"
#include "ooCapability.h"
#include "ooStackCmds.h"
#include "oosndrtp.h"
#include "ooras.h"
#ifndef _WIN32
#include <pthread.h>
#endif

int osEpStartReceiveChannel(ooCallData *call, ooLogicalChannel *pChannel);
int osEpStartTransmitChannel(ooCallData *call, ooLogicalChannel *pChannel);
int osEpStopReceiveChannel(ooCallData *call, ooLogicalChannel *pChannel);
int osEpStopTransmitChannel(ooCallData *call, ooLogicalChannel *pChannel);
int osEpOnIncomingCall(ooCallData* call );
int osEpOnOutgoingCall(ooCallData* call );
int osEpOnCallCleared(ooCallData* call );
int osEpOnAlerting(ooCallData* call);
void * osEpHandleCommand(void*);

int isActive;
char callToken[20];

int main(int argc, char ** argv)
{
   int ret=0;
   char localip[20]; //test
   H245AudioCapability audioCap;
#ifdef _WIN32
   HANDLE threadHdl;
#else
   pthread_t threadHdl;
#endif

#ifdef _WIN32
   ooSocketsInit (); /*Initialize the windows socket api  */
#endif
   /* Initialize the H323 endpoint - faststart and tunneling enabled*/
   ret = ooInitializeH323Ep("simple.log", 1, 1, 30, 9, 0, 61, "obj-sys",
                      "Version 0.4", T_H225CallType_pointToPoint, 1720,
                      "objsyscall", "simple", OO_CALLMODE_AUDIOCALL);
   if(ret != OO_OK)
   {
      printf("Failed to initialize H.323 Endpoint\n");
      return -1;
   }




   /* Configure mediainfo for transmit media channel of type G711 */
   memset(localip, 0, 20);//test
   ooGetLocalIPAddress(localip);//test
   printf("Local ip is %s\n", localip);
  
   /* Add audio capability */
   audioCap.t = T_H245AudioCapability_g711Ulaw64k;
   audioCap.u.g711Ulaw64k = 240;
   ooAddAudioCapability(audioCap, T_H245Capability_transmitAudioCapability,
                        NULL, &osEpStartTransmitChannel, NULL,
                        &osEpStopTransmitChannel);
   ooAddAudioCapability(audioCap, T_H245Capability_receiveAudioCapability,
                         &osEpStartReceiveChannel, NULL,
                         &osEpStopReceiveChannel, NULL);
   /* Register callbacks */
   ooH323EpRegisterCallbacks(&osEpOnAlerting, &osEpOnIncomingCall,
                             &osEpOnOutgoingCall, NULL,
                             osEpOnCallCleared, NULL);  
   ooSetAliasH323ID("vish");
   ooSetAliasDialedDigits("6109528747");
   ooSetAliasURLID("http://www.obj-sys.com");
  
   /* Init RAS module */
   ooInitRas(0, RasNoGatekeeper, 0, 0);
   /* Load media plug-in*/
#ifdef _WIN32
   ret = ooLoadSndRTPPlugin("oomedia.dll");
   if(ret != OO_OK)
   {
      printf("\n Failed to load the media plug-in library - oomedia.dll\n");
      exit(0);
   }
#else
   ret = ooLoadSndRTPPlugin("liboomedia.so");
   if(ret != OO_OK)
   {
      printf("\n Failed to load the media plug-in library - liboomedia.so\n");
      exit(0);
   }
#endif
   /* Create H.323 Listener */
   ret = ooCreateH323Listener();
   if(ret != OO_OK)
   {
      OOTRACEERR1("Failed to Create H.323 Listener");
      return -1;
   }
   if(argc == 2)
   {
      printf("Calling %s\n", argv[1]);
      memset(callToken, 0, 20);
      ooMakeCall(argv[1], callToken); /* Make call */
   }
#ifdef _WIN32
   threadHdl = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)osEpHandleCommand,
                            0, 0, 0);
#else
   pthread_create(&threadHdl, NULL, osEpHandleCommand, NULL);
#endif
   ooMonitorChannels();
   return 0;
}

/* Callback to start receive media channel */
int osEpStartReceiveChannel(ooCallData *call, ooLogicalChannel *pChannel)
{
   printf("Starting Receive channel at %s:%d\n", call->localIP,
                                              pChannel->localRtpPort);
   isActive = 1;
   ooCreateReceiveRTPChannel(call->localIP,
                             pChannel->localRtpPort);
   ooStartReceiveAudioAndPlayback();
   return OO_OK;
}

/* Callback to start transmit media channel */
int osEpStartTransmitChannel(ooCallData *call, ooLogicalChannel *pChannel)
{
   printf("Starting transmit channel to %s:%d\n", call->remoteIP,
                                               pChannel->remoteRtpPort);
   isActive = 1;
   ooCreateTransmitRTPChannel(call->remoteIP,
                              pChannel->remoteRtpPort);
   ooStartTransmitMic();
   return OO_OK;
}

/* Callback to stop receive media channel */
int osEpStopReceiveChannel(ooCallData *call, ooLogicalChannel *pChannel)
{
   printf("Stopping Receive Channel\n");
   ooStopReceiveAudioAndPlayback();
   return OO_OK;
}

/* Callback to stop transmit media channel */
int osEpStopTransmitChannel(ooCallData *call, ooLogicalChannel *pChannel)
{
   printf("Stopping Transmit Channel\n");
   ooStopTransmitMic();
   return OO_OK;
}

int osEpOnAlerting(ooCallData* call)
{
   ooMediaInfo mediaInfo1, mediaInfo2;
   char localip[20];
   memset(&mediaInfo1, 0, sizeof(ooMediaInfo));
   memset(&mediaInfo2, 0, sizeof(ooMediaInfo));

   /* Configure mediainfo for transmit media channel of type G711 */
   memset(localip, 0, 20);
   ooGetLocalIPAddress(localip);
   mediaInfo1.lMediaCntrlPort = 5001;
   mediaInfo1.lMediaPort = 5000;
   mediaInfo1.capType = T_H245AudioCapability_g711Ulaw64k;
   strcpy(mediaInfo1.lMediaIP, localip);
   strcpy(mediaInfo1.dir, "transmit");
   strcpy(mediaInfo1.mediaType, "audio");
   ooAddMediaInfo(call, mediaInfo1);
  
   /* Configure mediainfo for receive media channel of type G711 */
   mediaInfo2.lMediaCntrlPort = 5001;
   mediaInfo2.lMediaPort = 5000;
   mediaInfo2.capType = T_H245AudioCapability_g711Ulaw64k;
   strcpy(mediaInfo2.lMediaIP, localip);
   strcpy(mediaInfo2.dir, "receive");
   strcpy(mediaInfo2.mediaType, "audio");
   ooAddMediaInfo(call, mediaInfo2);
    
   strcpy(callToken, call->callToken);
   return OO_OK;
}
/* on incoming call callback */
int osEpOnIncomingCall(ooCallData* call )
{
   isActive = 1;
   return OO_OK;
}

int osEpOnOutgoingCall(ooCallData* call )
{
   ooMediaInfo mediaInfo1, mediaInfo2;
   char localip[20];
   memset(&mediaInfo1, 0, sizeof(ooMediaInfo));
   memset(&mediaInfo2, 0, sizeof(ooMediaInfo));
   /* Configure mediainfo for transmit media channel of type G711 */
   memset(localip, 0, 20);
   ooGetLocalIPAddress(localip);
   mediaInfo1.lMediaCntrlPort = 5001;
   mediaInfo1.lMediaPort = 5000;
   strcpy(mediaInfo1.lMediaIP, localip);
   strcpy(mediaInfo1.dir, "transmit");
   mediaInfo1.capType = T_H245AudioCapability_g711Ulaw64k;
   strcpy(mediaInfo1.mediaType, "audio");
   ooAddMediaInfo(call, mediaInfo1);
  
   /* Configure mediainfo for receive media channel of type G711 */
   mediaInfo2.lMediaCntrlPort = 5001;
   mediaInfo2.lMediaPort = 5000;
   strcpy(mediaInfo2.lMediaIP, localip);
   strcpy(mediaInfo2.dir, "receive");
   mediaInfo2.capType = T_H245AudioCapability_g711Ulaw64k;
   strcpy(mediaInfo2.mediaType, "audio");
   ooAddMediaInfo(call, mediaInfo2);
    
   strcpy(callToken, call->callToken);

  
   return OO_OK;
}

/* CallCleared callback */
int osEpOnCallCleared(ooCallData* call )
{
   printf("Call Ended\n");
   isActive = 0;
   return OO_OK;
}

void* osEpHandleCommand(void* dummy)
{
   int ret;
   char command[20];
   memset(command, 0, sizeof(command));
   printf("Hit <ENTER> to Hang call\n");
   fgets(command, 20, stdin);
   if(isActive)
   {
      printf("Hanging up call\n");
      ret = ooHangCall(callToken);
   }
   else {
      printf("No active call\n");
   }
   printf("Hit <ENTER> to close stack\n");
   fgets(command, 20, stdin);
   printf("Closing down stack\n");
   ooStopMonitor();
   return dummy;
}
