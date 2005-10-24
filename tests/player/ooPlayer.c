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
#include "ooUtils.h"

#ifndef _OOCHANNELS_H_
#include "oochannels.h"
#endif
#ifndef _WIN32
#include <pthread.h>
#endif

static char USAGE[]={
   "To Run Player Application:\n"
   "\t./ooPlayer --audio-file <filename> [--use-ip<ip>] [remote-ip]\n"

   "where:\n"
   "--audio-file <filename>   - Wave file to play.\n"
   "--use-ip <ip>             - Local ip address to use\n"
   "                           (default - uses gethostbyname)\n"
   "remote-ip                 - Remote ip address. Required only when \n"
   "                            receiver is running on remote machine \n"
};

int osEpOnCallCleared(OOH323CallData* call );
int osEpOnNewCallCreated(OOH323CallData* call );
static OOBOOL bActive=FALSE;
static char gPlayFile[100];
char gCallToken[20];
static char gLocalIp[20];
char gDest[256];

int main(int argc, char ** argv)
{
   int ret=0, x=0;
   OOH323CALLBACKS h323Callbacks;

#ifdef _WIN32
   HANDLE threadHdl;
#else
   pthread_t threadHdl;
#endif

#ifdef _WIN32
   ooSocketsInit (); /*Initialize the windows socket api  */
#endif
   gPlayFile[0]='\0';
   gLocalIp[0]='\0';
   gDest[0]='\0';
   /* Parse arguments */
   if(argc == 1)
   {
      printf("\nUSAGE:%s", USAGE);
      return -1;
   }

   for(x=1; x<argc; x++)
   {
      if(!strcmp(argv[x], "--audio-file"))
      {
         x++;
         strncpy(gPlayFile, argv[x], sizeof(gPlayFile)-1);
         gPlayFile[sizeof(gPlayFile)-1]='\0';
      }else if(!strcmp(argv[x], "--use-ip"))
      {
         x++;
         strncpy(gLocalIp, argv[x], sizeof(gLocalIp)-1);
         gLocalIp[sizeof(gLocalIp)-1]='\0';
      }else{
         strncpy(gDest, argv[x], sizeof(gDest)-1);
         gDest[sizeof(gDest)-1]='\0';
      }
   }

   if(ooUtilsIsStrEmpty(gPlayFile))
   {     
      printf("Error:Audio file name is required\n");
      printf("Usage\n:%s", USAGE);
      return -1;
   }
  
   if(ooUtilsIsStrEmpty(gLocalIp))
   {
      ooGetLocalIPAddress(gLocalIp);
   }

   if(!strcmp(gLocalIp, "127.0.0.1"))
   { 
      printf("Could not determine local ip. Please pass as parameter\n");
      printf("Usage\n:%s", USAGE);
      return -1;
   }

   if(ooUtilsIsStrEmpty(gDest))
      strcpy(gDest, gLocalIp);

   /* Initialize H323 endpoint */
   ret = ooH323EpInitialize(OO_CALLMODE_AUDIOTX, "player.log");
   if(ret != OO_OK)
   {
      printf("Failed to initialize H.323 Endpoint\n");
      return -1;
   }

   ooH323EpSetLocalAddress(gLocalIp, 0);

   ooH323EpCreateCmdListener(0);
   /* Register callbacks */

   h323Callbacks.onNewCallCreated = osEpOnNewCallCreated;
   h323Callbacks.onAlerting = NULL;
   h323Callbacks.onIncomingCall = NULL;
   h323Callbacks.onOutgoingCall = NULL;
   h323Callbacks.onCallEstablished = NULL;
   h323Callbacks.onCallCleared = osEpOnCallCleared;
   h323Callbacks.openLogicalChannels=NULL;

   ooH323EpSetH323Callbacks(h323Callbacks);
   ooH323EpSetTCPPortRange(16050, 16250);
   ooH323EpSetUDPPortRange(17050, 17250);
   ooH323EpSetRTPPortRange(18050, 18250);

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
int osEpStartTransmitChannel(OOH323CallData *call, ooLogicalChannel *pChannel)
{
   printf ("Starting transmit channel %s:%d\n",
           call->remoteIP, pChannel->remoteMediaPort);

   ooCreateTransmitRTPChannel(call->remoteIP, pChannel->remoteMediaPort);
   ooStartTransmitWaveFile(gPlayFile);
   return OO_OK;
}

/* Callback to stop transmit media channel*/
int osEpStopTransmitChannel(OOH323CallData *call, ooLogicalChannel *pChannel)
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
   OOStkCmdStat stat;

   if((stat = ooMakeCall (gDest, gCallToken, sizeof(gCallToken), NULL)) != OO_STKCMD_SUCCESS)
   {
      printf("Failed to place a call - %s\n", ooGetStkCmdStatusCodeTxt(stat));
      return dummy;
   }
   bActive = TRUE;
   printf("Hit <ENTER> to hang call\n");
   memset(command, 0, sizeof(command));
   fgets(command, 20, stdin);
   if(bActive)
   {
      printf("Hanging up call\n");
      ret = ooHangCall(gCallToken, OO_REASON_LOCAL_CLEARED);
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
int osEpOnCallCleared(OOH323CallData* call )
{
   printf("Call Ended\n");
   bActive = FALSE;
   return OO_OK;
}

/* New call structure created callback */
int osEpOnNewCallCreated(OOH323CallData* call )
{
   ooMediaInfo mediaInfo;

   memset(&mediaInfo, 0, sizeof(ooMediaInfo));

   /* Configure mediainfo for transmit media of type G711*/

   mediaInfo.lMediaCntrlPort = 5001;
   mediaInfo.lMediaPort = 5000;
   strcpy(mediaInfo.lMediaIP, gLocalIp);
   strcpy(mediaInfo.dir, "transmit");
   mediaInfo.cap = OO_G711ULAW64K;
   ooAddMediaInfo(call, mediaInfo);
    
   strcpy(gCallToken, call->callToken);
  
   return OO_OK;
}
