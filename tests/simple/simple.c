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
int osEpOnOutgoingCallAdmitted(ooCallData* call );
int osEpOnCallCleared(ooCallData* call );
int osEpOnAlerting(ooCallData* call);
void * osEpHandleCommand(void*);

static int isActive;
static char callToken[20];
static char ourip[20];
static int ourport;
static char dest[256];

static char USAGE[]={
   "simple [--use-ip ip] [--use-port port] [remote]\n"
   "--use-ip   -  local ip to use\n"
   "--use-port -  local port to use\n"
   "remote     -  Remote endpoint to call(optional)\n"
   "              Can be ip:[port] or aliases\n"
};

int main(int argc, char ** argv)
{
    int ret=0, x, dest_found=0;
  
#ifdef _WIN32
   HANDLE threadHdl;
#else
   pthread_t threadHdl;
#endif

   if(argc>6)
   {
      printf("USAGE:\n%s", USAGE);
      return -1;
   }
  
   memset(ourip, 0, sizeof(ourip));
   ourport=0;
   memset(dest,0, sizeof(dest));
  
   /* parse args */
   for(x=1; x<argc; x++)
   {
      if(!strcmp(argv[x], "--use-ip"))
      {
         x++;
         strncpy(ourip, argv[x], sizeof(ourip)-1);
      }else if(!strcmp(argv[x],"--use-port"))
      {
         x++;
         ourport = atoi(argv[x]);
      }else if(!dest_found)
      {
         strncpy(dest, argv[x], sizeof(dest)-1);
         dest_found=1;
      }else {
         printf("USAGE:\n%s",USAGE);
         return -1;
      }
   }
  
   /* Determine ourip if not specified by user */
   if(!strlen(ourip))
   {
      ooGetLocalIPAddress(ourip);
      if(!strcmp(ourip, "127.0.0.1"))
      {
         printf("Failed to determine local ip, please specify as command line"
                "option\n");
         printf("USAGE:\n%s", USAGE);
      }
   }
      
          
#ifdef _WIN32
   ooSocketsInit (); /*Initialize the windows socket api  */
#endif
   /* Initialize the H323 endpoint - faststart and tunneling enabled*/
   ret = ooInitializeH323Ep("simple.log", 1, 1, 30, 9, 0, 71, "obj-sys",
                      "Version 0.4", T_H225CallType_pointToPoint, 1720,
                      "objsyscall", "simple", OO_CALLMODE_AUDIOCALL);
   if(ret != OO_OK)
   {
      printf("Failed to initialize H.323 Endpoint\n");
      return -1;
   }

   ooSetLocalCallSignallingAddress(ourip, ourport);

   /* Add audio capability */
   ooAddCapability(OO_CAP_ULAW_64k_30,T_H245Capability_transmitAudioCapability,
                   NULL, &osEpStartTransmitChannel, NULL,
                        &osEpStopTransmitChannel);

  ooAddCapability(OO_CAP_ULAW_64k_180, T_H245Capability_receiveAudioCapability,
                  &osEpStartReceiveChannel, NULL,
                  &osEpStopReceiveChannel, NULL);
   ooSetTraceThreshold(OOTRCLVLINFO);
   /* Register callbacks */
   ooH323EpRegisterCallbacks(&osEpOnAlerting, &osEpOnIncomingCall,
                             &osEpOnOutgoingCallAdmitted, NULL,NULL,
                             osEpOnCallCleared);  
   ooSetAliasH323ID("objsys");
   ooSetAliasDialedDigits("5087556929");
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
   ooDestroyRas();
   ooDestroyH323Ep();
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
 
   return OO_OK;
}
/* on incoming call callback */
int osEpOnIncomingCall(ooCallData* call )
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
   mediaInfo1.cap = OO_CAP_ULAW_64k_30;
   strcpy(mediaInfo1.lMediaIP, localip);
   strcpy(mediaInfo1.dir, "transmit");
   ooAddMediaInfo(call, mediaInfo1);
  
   /* Configure mediainfo for receive media channel of type G711 */
   mediaInfo2.lMediaCntrlPort = 5001;
   mediaInfo2.lMediaPort = 5000;
   mediaInfo2.cap =    OO_CAP_ULAW_64k_180;
   strcpy(mediaInfo2.lMediaIP, localip);
   strcpy(mediaInfo2.dir, "receive");
   ooAddMediaInfo(call, mediaInfo2);
    
   strcpy(callToken, call->callToken);
   isActive = 1;
  
   return OO_OK;
}

int osEpOnOutgoingCallAdmitted(ooCallData* call )
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
   mediaInfo1.cap = OO_CAP_ULAW_64k_30;
   ooAddMediaInfo(call, mediaInfo1);
  
   /* Configure mediainfo for receive media channel of type G711 */
   mediaInfo2.lMediaCntrlPort = 5001;
   mediaInfo2.lMediaPort = 5000;
   strcpy(mediaInfo2.lMediaIP, localip);
   strcpy(mediaInfo2.dir, "receive");
   mediaInfo2.cap = OO_CAP_ULAW_64k_180;
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
