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

#include <stdio.h>
#include <stdlib.h>

#include "ooPlayer.h"

#include "ooasn1.h"
#include "ooStackCmds.h"
#include "ootrace.h"
#include "ooports.h"
#include "ooh323ep.h"
#include "oosndrtp.h"
#include "ooCalls.h"
#include "ooCapability.h"
#ifndef _OOCHANNELS_H_
#include "oochannels.h"
#endif
#ifndef _WIN32
#include <pthread.h>
#endif

int osEpOnCallCleared(ooCallData* call );
int osEpOnOutgoingCallAdmitted(ooCallData* call );
int isCallActive;
char ooPlayFile[100];
char callToken[20];

int main(int argc, char ** argv)
{
   int ret=0;
   char localip[20];
  
   OOH323CALLBACKS h323Callbacks;

#ifdef _WIN32
   HANDLE threadHdl;
#else
   pthread_t threadHdl;
#endif

#ifdef _WIN32
   ooSocketsInit (); /*Initialize the windows socket api  */
#endif
   /* Initialize H323 endpoint */
   ret = ooH323EpInitialize("objsyscall", OO_CALLMODE_AUDIOTX, "player.log");
   if(ret != OO_OK)
   {
      printf("Failed to initialize H.323 Endpoint\n");
      return -1;
   }

   /* Register callbacks */

   h323Callbacks.onNewCallCreated = NULL;
   h323Callbacks.onAlerting = NULL;
   h323Callbacks.onIncomingCall = NULL;
   h323Callbacks.onOutgoingCall = NULL;
   h323Callbacks.onCallAnswered = NULL;
   h323Callbacks.onCallEstablished = NULL;
   h323Callbacks.onOutgoingCallAdmitted = osEpOnOutgoingCallAdmitted;
   h323Callbacks.onCallCleared = osEpOnCallCleared;
   h323Callbacks.openLogicalChannels=NULL;

   ooH323EpSetH323Callbacks(h323Callbacks);
   ooSetTCPPorts(16050, 16250);
   ooSetUDPPorts(17050, 17250);
   ooSetRTPPorts(18050, 18250);

   /* Add transmit audio capability of type G711 ULaw */

   ooH323EpAddG711Capability(OO_G711ULAW64K, 240, 0, OOTX, NULL,
                   &osEpStartTransmitChannel, NULL, &osEpStopTransmitChannel);
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
  
  
   if(argc == 2)
   {
      strcpy(ooPlayFile, argv[1]);
      memset(localip, 0, sizeof(localip));
      ooGetLocalIPAddress(localip);
      ooMakeCall (localip, callToken, sizeof(callToken), NULL); /* make call*/  
      isCallActive = 1;
   }
   else
     {/* Otherwise print usage message and exit */
      printf("\nUsage : player <filename>");
      exit(0);
   }
   /* This thread is created to monitor user input on stdin while stack is
      monitoring sockets. Required for windows as windows does not allow
      file handles(i.e. stdin in this case) to be monitored by select.
   */
#ifdef _WIN32
   threadHdl = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)osEpHandleCommand,
                            0, 0, 0);
#else
   pthread_create(&threadHdl, NULL, osEpHandleCommand, NULL);
#endif
   /*Monitor channels for incoming messages*/

   ooMonitorChannels();
   ooH323EpDestroy();
   return ASN_OK;
}


/* Callback to start transmit media channel */
int osEpStartTransmitChannel(ooCallData *call, ooLogicalChannel *pChannel)
{
   printf ("Starting transmit channel %s:%d\n",
           call->remoteIP, pChannel->mediaPort);

   ooCreateTransmitRTPChannel(call->remoteIP, pChannel->mediaPort);
   ooStartTransmitWaveFile(ooPlayFile);
   return OO_OK;
}

/* Callback to stop transmit media channel*/
int osEpStopTransmitChannel(ooCallData *call, ooLogicalChannel *pChannel)
{
   printf("Stopping Transmit Channel\n");
   ooStopTransmitWaveFile();
   return OO_OK;
}

/* This function handles used commands. This function is basically used
   as a callback function on linux as we can monitor stdin with sockets
   on linux. On windows, however, this function is used as a separate
  thread monitoring user activity.
*/  
void* osEpHandleCommand(void *dummy)
{
   int ret;
   char command[20];
   printf("Hit <ENTER> to hang call\n");
   memset(command, 0, sizeof(command));
   fgets(command, 20, stdin);
   if(isCallActive)
   {
      printf("Hanging up call\n");
      ret = ooHangCall(callToken, OO_REASON_LOCAL_CLEARED);
      printf("Hit <ENTER> to quit\n");
      fgets(command, 20, stdin);
   }
   else{
      printf("No active call\n");
   }
   printf("closing stack\n");
   ooStopMonitor();
   return dummy;
}

/* Call cleared callback */
int osEpOnCallCleared(ooCallData* call )
{
   printf("Call Ended\n");
   isCallActive = 0;
   return OO_OK;
}

/* out going call callback */
int osEpOnOutgoingCallAdmitted(ooCallData* call )
{
   ooMediaInfo mediaInfo;
   char localip[20];
   memset(&mediaInfo, 0, sizeof(ooMediaInfo));
   memset(localip, 0, 20);
   /* Configure mediainfo for transmit media of type G711*/
   ooGetLocalIPAddress(localip);
   mediaInfo.lMediaCntrlPort = 5001;
   mediaInfo.lMediaPort = 5000;
   strcpy(mediaInfo.lMediaIP, localip);
   strcpy(mediaInfo.dir, "transmit");
   mediaInfo.cap = OO_G711ULAW64K;
   ooAddMediaInfo(call, mediaInfo);
    
   strcpy(callToken, call->callToken);
  
   return OO_OK;
}
