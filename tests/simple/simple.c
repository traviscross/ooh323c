/*
 * Copyright (C) 2004 by Objective Systems, Inc.
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
#include "ooStackCmds.h"
#include "oosndrtp.h"
#ifndef _WIN32
#include <pthread.h>
#endif

int osEpStartReceiveChannel(ooCallData *call);
int osEpStartTransmitChannel(ooCallData *call);
int osEpStopReceiveChannel(ooCallData *call);
int osEpStopTransmitChannel(ooCallData *call);
int osEpOnIncomingCall(ooCallData* call );
int osEpOnCallCleared(ooCallData* call );
void * osEpHandleCommand(void*);

int isActive;
char callToken[20];

int main(int argc, char ** argv)
{
   int ret=0;
   H245AudioCapability audioCap;
#ifdef _WIN32
   HANDLE threadHdl;
#else
   pthread_t threadHdl;
#endif

#ifdef _WIN32
   ooSocketsInit (); /*Initialize the windows socket api  */
#endif

   ret = ooInitializeH323Ep("simple.log", 0, 0, 30, 9, 0, 61, "obj-sys",
                      "Version 0.3", T_H225CallType_pointToPoint, 1720,
                      "objsyscall", "simple", 1);
   if(ret != OO_OK)
   {
      printf("Failed to initialize H.323 Endpoint\n");
      return -1;
   }
   audioCap.t = T_H245AudioCapability_g711Ulaw64k;
   audioCap.u.g711Ulaw64k = 240;
   ooAddAudioCapability(audioCap, OO_TX_CAP,
                        NULL, &osEpStartTransmitChannel, NULL,
                        &osEpStopTransmitChannel);
    ooAddAudioCapability(audioCap, OO_RX_CAP,
                         &osEpStartReceiveChannel, NULL,
                         &osEpStopReceiveChannel, NULL);

   ooH323EpRegisterCallbacks(&osEpOnIncomingCall, NULL, NULL, osEpOnCallCleared, NULL);  
 
   /* Load plug-in*/
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
      ooMakeCall(argv[1], 1720, callToken);
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

int osEpStartReceiveChannel(ooCallData *call)
{
   printf("Starting Receive channel\n");
   isActive = 1;
   ooCreateReceiveRTPChannel(call->localIP,
                             call->localRtpPort);
   ooStartReceiveAudioAndPlayback();
   return OO_OK;
}
int osEpStartTransmitChannel(ooCallData *call)
{
   printf("Starting transmit channel\n");
   isActive = 1;
   ooCreateTransmitRTPChannel(call->remoteIP,
                              call->remoteRtpPort);
   ooStartTransmitMic();
   return OO_OK;
}

int osEpStopReceiveChannel(ooCallData *call)
{
   printf("Stopping Receive Channel\n");
   ooStopReceiveAudioAndPlayback();
   return OO_OK;
}

int osEpStopTransmitChannel(ooCallData *call)
{
   printf("Stopping Transmit Channel\n");
   ooStopTransmitMic();
   return OO_OK;
}

int osEpOnIncomingCall(ooCallData* call )
{
   strcpy(callToken, call->callToken);
   isActive = 1;
   return OO_OK;
}

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
