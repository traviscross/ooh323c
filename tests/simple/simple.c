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
#include "ootrace.h"
#include "oochannels.h"
#include "ooh323ep.h"
#include "ooCalls.h"
#include "ooCapability.h"
#include "ooConfig.h"
#include "ooStackCmds.h"
#include "oosndrtp.h"
#include "ooGkClient.h"
#include "ooUtils.h"
#include "ooLogChan.h"
#include <ctype.h>
#ifndef _WIN32
#include <pthread.h>
#endif

int osEpStartReceiveChannel(ooCallData *call, ooLogicalChannel *pChannel);
int osEpStartTransmitChannel(ooCallData *call, ooLogicalChannel *pChannel);
int osEpStopReceiveChannel(ooCallData *call, ooLogicalChannel *pChannel);
int osEpStopTransmitChannel(ooCallData *call, ooLogicalChannel *pChannel);
int osEpOnIncomingCall(ooCallData* call );
int osEpOnNewCallCreated(ooCallData* call );
int osEpOnCallForwarded(ooCallData *call);
int osEpOnCallCleared(ooCallData* call );
int osEpOnAlerting(ooCallData* call);
int osEpOpenLogicalChannels(ooCallData *call);
int osEpOnReceivedDTMF(OOH323CallData *call, const char* dtmf);
void * osEpHandleCommand(void*);

static OOBOOL bActive;
static char callToken[20];
static char ourip[20];
static int ourport = 1720;
static OOBOOL gCmdThrd;
static OOBOOL bAutoAnswer=FALSE;
static OOBOOL bRinging=FALSE;


static char USAGE[]={
   "Listen to incoming calls\n"
   "\tsimple [options] --listen\n"
   "Make a call and then go in listen mode\n"
   "\tsimple [options] <remote>\n"
   "\t                 remote => Remote endpoint to call\n"
   "\t                           Can be ip:[port] or aliases(needs gk)\n"

   "options:\n"
   "--user <username>      - Name of the user. This will be used as\n"
   "                         display name for outbound calls\n"
   "--user-number <number> - Caller number\n"
   "--gk-discover          - Discover gatekeeper\n"
   "--gk <ip:port>         - Use specific gatekeeper\n"
   "--auto-answer          - Enables auto answer mode\n"
   "--h323id <h323id>      - H323ID to be used for this endpoint\n"
   "--e164 <number>        - E164 number used as callerid for\n"
   "                         this endpoint\n"
   "--use-ip <ip>          - Ip address for the endpoint\n"
   "                         (default - uses gethostbyname)\n"
   "--use-port <port>      - Port number to use for listening to incoming\n"
   "                         calls.(default-1720)\n"
   "--no-faststart         - Disable fast start(default - enabled)\n"
   "--no-tunneling         - Disable H245 Tunneling\n" 
   "--config <filename>    - Read Asterisk h323.conf configuration file\n"
   "--mediadll <path>      - Full path to media DLL or shared object file\n"
   "-t                     - Trace. Use multiple times to increase\n"
   "                         trace level\n"
   "--help                 - Prints this usage message\n"
};

static char CMD_USAGE[]={
   "\tSupported commands:\n"
   "\tc <ip:port/alias> - Place a call\n"
   "\ta                 - Answer call\n"
   "\tr                 - Reject call\n"
   "\td                 - Send digit\n"
   "\tf <ip:port/alias> - Forward call\n"
   "\th                 - Hangup call\n"
   "\tq                 - Quit\n"
   "\thelp              - Help\n"
};

int main (int argc, char ** argv)
{
   int ret=0, x;
   char h323id[50], e164[50], user[50], user_num[50], dest[256], tmp[40];
   int trace_level=0;
   OOBOOL bListen=FALSE, bDestFound=FALSE, bFastStart=TRUE, bTunneling=TRUE;
   OOH323CALLBACKS h323Callbacks;
   enum RasGatekeeperMode gkMode = RasNoGatekeeper;
   const char* configFile = 0;
   char *gkIP=NULL, *pcPort=NULL;
   int gkPort=0;

#ifdef _WIN32
   HANDLE threadHdl;
   const char* mediadll = "../../lib/oomedia.dll";
#else
   pthread_t threadHdl;
   const char* mediadll = "liboomedia.so";
#endif

#ifdef _WIN32
   ooSocketsInit (); /*Initialize the windows socket api  */
#endif

   if(argc==1)
   {
      printf("USAGE:\n%s", USAGE);
      return -1;
   }
   h323id[0]='\0';
   e164[0]='\0';
   ourip[0]='\0';
   dest[0]='\0';
   user[0]='\0';
   user_num[0]='\0';
  
   gCmdThrd = FALSE;
   /* parse args */
   for(x=1; x<argc; x++)
   {
      if(!strcmp(argv[x], "--listen")) {
         if(!bDestFound)
            bListen=TRUE;
         else {
            printf("USAGE:\n%s",USAGE);
            return -1;
         }
      }
      else if(!strcmp(argv[x], "--no-faststart")) {
         bFastStart = FALSE;
      }
      else if(!strcmp(argv[x], "--no-tunneling")) {
         bTunneling = FALSE;
      }
      else if(!strcmp(argv[x], "--help")) {
              printf("USAGE:\n%s",USAGE);
         return 0;
      }
      else if(!strcmp(argv[x], "--user")) {
         x++;
         strncpy(user, argv[x], sizeof(user)-1);
         user[sizeof(user)-1]='\0';
      }
      else if(!strcmp(argv[x], "--user-number")) {
         x++;
         strncpy(user_num, argv[x], sizeof(user_num)-1);
         user_num[sizeof(user_num)-1]='\0';
      }
      else if(!strcmp(argv[x], "--h323id")){
         x++;
         strncpy(h323id, argv[x], sizeof(h323id)-1);
         h323id[sizeof(h323id)-1]='\0';
      }
      else if(!strcmp(argv[x], "--e164")) {
         x++;
         strncpy(e164, argv[x], sizeof(e164)-1);
         e164[sizeof(e164)-1]='\0';
      }
      else if(!strcmp(argv[x], "--auto-answer")) {
         bAutoAnswer = TRUE;
      }
      else if(!strcmp(argv[x], "--gk-discover")) {
         gkMode = RasDiscoverGatekeeper;
      }
      else if(!strcmp(argv[x], "--gk")) {
         x++;
         strncpy(tmp, argv[x], sizeof(tmp)-1);
         tmp[sizeof(tmp)-1]='\0';
         pcPort = strchr(tmp, ':');
         if(pcPort)
         {
            *pcPort = '\0';
            pcPort++;
            gkPort = atoi(pcPort);
         }
         gkIP = tmp;
         gkMode = RasUseSpecificGatekeeper;
      }
      else if(!strcmp(argv[x], "-t")) {
         trace_level++;
      }
      else if(!strcmp(argv[x], "--use-ip")) {
         x++;
         strncpy(ourip, argv[x], sizeof(ourip)-1);
         ourip[sizeof(ourip)-1]='\0';
      }
      else if(!strcmp(argv[x], "--use-port")) {
         x++;
         ourport = atoi(argv[x]);
      }
      else if(!strcmp(argv[x], "--config")) {
         x++;
         configFile = (const char*) argv[x];
      }
      else if(!strcmp(argv[x], "--mediadll")) {
         x++;
         mediadll = (const char*) argv[x];
      }
      else if(!bDestFound && !bListen) {
         strncpy(dest, argv[x], sizeof(dest)-1);
         bDestFound=TRUE;
      }
      else {
         printf ("USAGE:\n%s", USAGE);
         return -1;
      }
   }
  
   if(!bListen && !bDestFound)
   {
      printf("USAGE:\n%s",USAGE);
      return -1;
   }

   /* Determine ourip if not specified by user */
   if(ooUtilsIsStrEmpty(ourip))
   {
      ooGetLocalIPAddress(ourip);
   }

   if(!strcmp(ourip, "127.0.0.1"))
   {
      printf("Failed to determine local ip, please specify as command line"
             " option\n");
      printf("USAGE:\n%s", USAGE);
      return -1;
   }
      
   /* If user specified local IP address as destination, convert to
      real IP address */
   if (!strcmp (dest, "127.0.0.1")) {
      ooGetLocalIPAddress (dest);
   }

   /* Initialize the H323 endpoint - faststart and tunneling enabled*/
   ret = ooH323EpInitialize(OO_CALLMODE_AUDIOCALL, "simple.log");
   if(ret != OO_OK)
   {
      printf("Failed to initialize H.323 Endpoint\n");
      return -1;
   }

   /* Read configuration file */
   if (0 != configFile) {
      OOConfigFile config;
      ret = ooConfigInit (&config);
      if (0 == ret) {
         ret = ooConfigFileRead (configFile, &config);
      }
      if (0 == ret) {
         /* ooConfigPrint (&config); */

         /* Apply configuration to H.323 endpoint */
         ooH323EpApplyConfig (&config);

         ooConfigDestroy (&config);
      }
      else {
         printf ("Configuration initialization failed; status = %d\n", ret);
         return ret;
      }
   }

   /* Command-line opts overrule config file settings */
   if(!bFastStart)
      ooH323EpDisableFastStart();

   if(!bTunneling)
      ooH323EpDisableH245Tunneling();

   if(trace_level == 1)
      ooH323EpSetTraceLevel(OOTRCLVLDBGA);
   else if(trace_level == 2)
      ooH323EpSetTraceLevel(OOTRCLVLDBGB);
   else if(trace_level >= 3)
      ooH323EpSetTraceLevel(OOTRCLVLDBGC);
  
   ooH323EpSetLocalAddress(ourip, ourport);
   /* CmdListener should always be created after local address is set*/
   if(ooH323EpCreateCmdListener(0) != OO_OK)
   {
      printf("Failed to initialize Command Listener\n");
      return -1; 
   }

   if(bAutoAnswer)
      ooH323EpEnableAutoAnswer();
   else
      ooH323EpDisableAutoAnswer();
  
   if(!ooUtilsIsStrEmpty(user))
      ooH323EpSetCallerID(user);

   if(!ooUtilsIsStrEmpty(h323id))
      ooH323EpAddAliasH323ID(h323id);

   if(!ooUtilsIsStrEmpty(user_num))
      ooH323EpSetCallingPartyNumber(user_num);

   if(!ooUtilsIsStrEmpty(e164)){
      ooH323EpSetCallingPartyNumber(e164);
   }
  
  

   /* Set callbacks */
   h323Callbacks.onNewCallCreated = osEpOnNewCallCreated;
   h323Callbacks.onAlerting = osEpOnAlerting;
   h323Callbacks.onIncomingCall = osEpOnIncomingCall;
   h323Callbacks.onOutgoingCall = NULL;
   h323Callbacks.onCallEstablished = NULL;
   h323Callbacks.onCallForwarded = osEpOnCallForwarded;
   h323Callbacks.onCallCleared = osEpOnCallCleared;
   h323Callbacks.openLogicalChannels=NULL;
   h323Callbacks.onReceivedDTMF = osEpOnReceivedDTMF;

   /*dtmf stuff */
   ooH323EpEnableDTMFH245Alphanumeric();
   ooH323EpEnableDTMFH245Signal();
   ooH323EpEnableDTMFRFC2833(0);

   ooH323EpSetH323Callbacks(h323Callbacks);

   /* Add audio capability */
   ooH323EpAddG711Capability(OO_G711ULAW64K, 30, 240, OORXANDTX,
                       &osEpStartReceiveChannel, &osEpStartTransmitChannel,
                       &osEpStopReceiveChannel, &osEpStopTransmitChannel);

   /* To use a gatekeeper */
   if(gkMode != RasNoGatekeeper)
   {
      if(gkMode == RasDiscoverGatekeeper)
         ooGkClientInit(RasDiscoverGatekeeper, NULL, 0);
      else if(gkMode == RasUseSpecificGatekeeper)
         ooGkClientInit(RasUseSpecificGatekeeper, gkIP, gkPort);
   }

  
   /* Load media plug-in*/
   ret = ooLoadSndRTPPlugin ((char*)mediadll);
   if(ret != OO_OK)
   {
      printf ("Failed to load the media plug-in library - %s\n", mediadll);
      exit (0);
   }

   /* Print config */
   printf("Endpoint configuration:\n");
   printf("\tFastStart: %s\n", bFastStart?"Enabled":"Disabled");
   printf("\tTunneling: %s\n", bTunneling?"Enabled":"Disabled");
   if(!ooUtilsIsStrEmpty(h323id))  
      printf("\tH323ID: %s\n", h323id);

   if(!ooUtilsIsStrEmpty(e164))
      printf("\te164: %s\n", e164);
   if(!ooUtilsIsStrEmpty(user))
     printf("\tUser: %s\n", user);
   if(!ooUtilsIsStrEmpty(user_num))
     printf("\tUser Number: %s\n", user_num);

   if(gkMode == RasNoGatekeeper)
      printf("\tGatekeeper: Not Used\n");
   else if(gkMode == RasDiscoverGatekeeper)
      printf("\tGatekeeper: Discover Gatekeeper\n");
   else if(gkMode == RasUseSpecificGatekeeper)
   {
      char gkAddr[60];

      if(gkPort != 0)
         sprintf(gkAddr, "%s:%d", gkIP, gkPort);
      else
         sprintf(gkAddr, "%s:DEFAULT", gkIP);

      printf("\tGatekeeper: %s\n", gkAddr);
   }
   printf("\tListening for Calls At: %s:%d\n", ourip, ourport);

     


   /* Create H.323 Listener */
   ret = ooCreateH323Listener();
   if(ret != OO_OK)
   {
      OOTRACEERR1("Failed to Create H.323 Listener");
      return -1;
   }
   if(bDestFound)
   {
      printf("--->Calling %s\n", dest);
      memset(callToken, 0, 20);
      ooMakeCall(dest, callToken, sizeof(callToken), NULL); /* Make call */
      bActive = TRUE;
   }

  
#ifdef _WIN32
   threadHdl = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)osEpHandleCommand,
                            0, 0, 0);
#else
   pthread_create(&threadHdl, NULL, osEpHandleCommand, NULL);
#endif
   ooMonitorChannels();

   if(gCmdThrd)
   {
#ifdef _WIN32
      TerminateThread(threadHdl, 0);
#else
      pthread_cancel(threadHdl);
      pthread_join(threadHdl, NULL);

#endif
   }
   printf("--->Destroying H323Ep\n");
   ooH323EpDestroy();
   return 0;
}


/* Callback to start receive media channel */
int osEpStartReceiveChannel(ooCallData *call, ooLogicalChannel *pChannel)
{
   printf("\n--->Starting Receive channel at %s:%d", pChannel->localIP,
                                              pChannel->localRtpPort);
   printf("\nCMD>");
   fflush(stdout);
   ooCreateReceiveRTPChannel(pChannel->localIP,
                             pChannel->localRtpPort);
   ooStartReceiveAudioAndPlayback();
   return OO_OK;
}

/* Callback to start transmit media channel */
int osEpStartTransmitChannel(ooCallData *call, ooLogicalChannel *pChannel)
{
   printf("\n--->Starting transmit channel to %s:%d",
            pChannel->remoteIP, pChannel->remoteMediaPort);
   printf("\nCMD>");
   fflush(stdout);
   ooCreateTransmitRTPChannel (pChannel->remoteIP, pChannel->remoteMediaPort);
   ooStartTransmitMic();
   return OO_OK;
}

/* Callback to stop receive media channel */
int osEpStopReceiveChannel(ooCallData *call, ooLogicalChannel *pChannel)
{
   printf("\n--->Stopping Receive Channel");
   printf("\nCMD>");
   fflush(stdout);
   ooStopReceiveAudioAndPlayback();
   return OO_OK;
}

/* Callback to stop transmit media channel */
int osEpStopTransmitChannel(ooCallData *call, ooLogicalChannel *pChannel)
{
   printf("\n--->Stopping Transmit Channel");
   printf("\nCMD>");  
   fflush(stdout);
   ooStopTransmitMic();
   return OO_OK;
}

int osEpOnReceivedDTMF(OOH323CallData *call, const char* dtmf)
{
   printf("\n--->Received user input %s", dtmf);
   printf("\nCMD>");
   fflush(stdout);
   return OO_OK;
}

int osEpOnAlerting(ooCallData* call)
{
   if(!strcmp(call->callType, "incoming"))
   {
      bRinging = TRUE;
      printf("\n--->Incoming call from\"%s\"(%s).",
             (call->remoteDisplayName)?call->remoteDisplayName:"Unknown Name",
           call->callingPartyNumber?call->callingPartyNumber:"Unknown Number");
      if(!bAutoAnswer)
      {
         printf("Do you want to Answer/reject?(a/r)");
      }
      printf("\nCMD>");
      fflush(stdout);
   }
   return OO_OK;
}
/* on incoming call callback */
int osEpOnIncomingCall(ooCallData* call )
{
   if(!bActive){
      bActive = TRUE;
   }
   else{
      ooHangCall(call->callToken, OO_REASON_LOCAL_BUSY);
      printf("\n--->Incoming call Rejected - line busy");
      printf("\nCMD>");
      fflush(stdout);
      return OO_OK;
   }

   return OO_OK;
}

int osEpOnNewCallCreated(ooCallData* call )
{
   ooMediaInfo mediaInfo1, mediaInfo2;
   memset(&mediaInfo1, 0, sizeof(ooMediaInfo));
   memset(&mediaInfo2, 0, sizeof(ooMediaInfo));

   /* Configure mediainfo for transmit media channel of type G711 */
   mediaInfo1.lMediaCntrlPort = 5001;
   mediaInfo1.lMediaPort = 5000;
   strcpy(mediaInfo1.lMediaIP, call->localIP);
   strcpy(mediaInfo1.dir, "transmit");
   mediaInfo1.cap = OO_G711ULAW64K;
   ooAddMediaInfo(call, mediaInfo1);
  
   /* Configure mediainfo for receive media channel of type G711 */
   mediaInfo2.lMediaCntrlPort = 5001;
   mediaInfo2.lMediaPort = 5000;
   strcpy(mediaInfo2.lMediaIP, call->localIP);
   strcpy(mediaInfo2.dir, "receive");
   mediaInfo2.cap = OO_G711ULAW64K;
   ooAddMediaInfo(call, mediaInfo2);
    
   strcpy(callToken, call->callToken);

   return OO_OK;
}

/* Callback when we are being forwarded to another destination by remote
   endpoint.
*/
int osEpOnCallForwarded(ooCallData *call)
{
   printf("\n--->Call %s being forwarded to ", call->callToken);

   if(!ooUtilsIsStrEmpty(call->pCallFwdData->ip))
   {
      printf("ip %s", call->pCallFwdData->ip);
   }

   if(call->pCallFwdData->aliases)
   {
      printf("(alias - %s)", call->pCallFwdData->aliases->value);
   }
   printf("\nCMD>");
   fflush(stdout);
   return OO_OK;
}

/* CallCleared callback */
int osEpOnCallCleared(ooCallData* call )
{
   if(!strcmp(call->callToken , callToken))
   {
      printf("\n--->Call %s Ended - %s\n", callToken,
                                 ooGetReasonCodeText(call->callEndReason));
      printf("\nCMD>");
      fflush(stdout);
      callToken[0]='\0';
      bActive = FALSE;
      bRinging = FALSE;
   }
   return OO_OK;
}

#if 0
void* osEpHandleCommand(void* dummy)
{
   int ret;
   char command[20];
   gCmdThrd = TRUE;
#ifndef _WIN32
   pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
#endif
   memset(command, 0, sizeof(command));
   printf("Hit <ENTER> to Hang call\n");
   fgets(command, 20, stdin);
   if(bActive)
   {
      printf("Hanging up call\n");
      ret = ooHangCall(callToken, OO_REASON_LOCAL_CLEARED);
   }
   else {
      printf("No active call\n");
   }
   printf("Hit <ENTER> to close stack\n");
   fgets(command, 20, stdin);
   printf("Closing down stack\n");
   ooStopMonitor();
   gCmdThrd=FALSE;
   return dummy;
}

#endif

void* osEpHandleCommand(void* dummy)
{
   int cmdLen, i=0;
   char command[256], *p=NULL, dest[256], ch=0;
   OOStkCmdStat stat;
   gCmdThrd = TRUE;
#ifndef _WIN32
   pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
#endif
   command[0]='\0';

   printf("\nCMD USAGE:\n%s", CMD_USAGE);
   while(ch != 'q')
   {
      printf("CMD>");
      fflush(stdout);
      memset(command, 0, sizeof(command));
      fgets(command, 256, stdin);
      cmdLen = strlen(command);
      for(p=command; (*p == ' ' || *p == '\t') && p < command+cmdLen; p++);

      if(*p == '\n')
         continue;

      if(p >= command + cmdLen)
      {
         printf("Invalid Command\n");
         printf("CMD USAGE:\n%s", CMD_USAGE);
         continue;
      }

      if(!strcmp(p, "help"))
      {
         printf("CMD USAGE:\n%s", CMD_USAGE);
         continue;
      } 
      ch = tolower(*p);
      if(*(p+1) != ' ' && *(p+1) != '\t' && *(p+1) != '\n') {
              ch ='i'; /* invalid command */
      }
      switch(ch)
      {
         case 'c':
            if(bActive)
            {   
               printf("--->Can not make a new call while an active call is "
                      "present\n");
               break;
            }
            p++;
            while((*p == ' ' || *p == '\t')&& p<command+cmdLen) p++;
            if(p >= command + cmdLen || *p == '\n')
            {
               printf("--->Invalid Command\n");
               printf("CMD USAGE:\n%s", CMD_USAGE);
               break;
            }
            i=0;
            while(*p != ' ' && *p != '\t' && *p != '\n')
            {
               dest[i++] = *p;
               p++;
            }
            dest[i]='\0';

            if((stat = ooMakeCall(dest, callToken, sizeof(callToken), NULL)) !=
                                                      OO_STKCMD_SUCCESS)
            {
               printf("--->Failed to place a call to %s \n",dest);
               printf("    Reason: %s\n", ooGetStkCmdStatusCodeTxt(stat));
            }
            else{
               printf("--->Calling %s \n", dest);
               bActive = TRUE;
            }
            break;

         case 'a':
            if(bActive && bRinging)
            {
               if((stat = ooAnswerCall(callToken)) == OO_STKCMD_SUCCESS) {
                  bRinging = FALSE;
               }
               else{
                  printf("--->Failed to answer the call %s\n", callToken);
                  printf("    Reason: %s\n", ooGetStkCmdStatusCodeTxt(stat));
               }
            }
            else{
               printf("--->There is no active call in ringing status\n");
                 }
            break;

         case 'r':
                 if(bActive && bRinging){
              if((stat = ooHangCall(callToken, OO_REASON_LOCAL_REJECTED)) !=
                                                           OO_STKCMD_SUCCESS)
              {
                 printf("--->Failed to reject call %s\n", callToken);
                 printf("    Reason: %s\n", ooGetStkCmdStatusCodeTxt(stat));
              }
              else{
                 bRinging = FALSE;
              }
                 }
            else{
              printf("--->There is no active call in ringing status\n");
            }
            break;

         case 'f':
            if(bActive && !bRinging)
            {
               p++;
               while((*p == ' ' || *p == '\t')&& p<command+cmdLen) p++;
               if(p >= command + cmdLen || *p == '\n')
               {
                  printf("--->Invalid Command\n");
                  printf("CMD USAGE:\n%s", CMD_USAGE);
                  break;
                }
                i=0;
                while(*p != ' ' && *p != '\t' && *p != '\n')
                {
                   dest[i++] = *p;
                   p++;
                }
                dest[i]='\0';

                if((stat = ooForwardCall(callToken, dest)) !=
                                                          OO_STKCMD_SUCCESS)
                {
                   printf("--->Failed to forward call %s to %s\n", callToken,
                                                                   dest);
                   printf("    Reason: %s\n", ooGetStkCmdStatusCodeTxt(stat));
                }else{
                   printf("--->Forwarding Call to %s \n", dest);
                }
            }else{
               printf("--->There is no established call to forward\n");
            }
                 break;

         case 'h':
            if(bActive)
            {
               if((stat = ooHangCall(callToken, OO_REASON_LOCAL_CLEARED)) !=
                                                                   OO_OK)
               {
                  printf("--->Failed to hang call %s\n", callToken);
                  printf("    Reason: %s\n", ooGetStkCmdStatusCodeTxt(stat));
               }else{
                  printf("--->Hanging call %s\n", callToken);
               }
            }else{
               printf("--->No active call to hang\n");
            }
            break;

         case 'q':
            if(bActive)
            {
               printf("--->Hanging Active Call %s\n", callToken);
               if((stat = ooHangCall(callToken, OO_REASON_LOCAL_CLEARED)) !=
                                                           OO_STKCMD_SUCCESS)
               {
                  printf("--->Failed to hang call %s\n", callToken);
                  printf("    Reason: %s\n", ooGetStkCmdStatusCodeTxt(stat));
               }
            }
            break;

         case 'd':
                 if(bActive) {
              p++;
              while((*p == ' ' || *p == '\t')&& p<command+cmdLen) p++;
               if(p >= command + cmdLen || *p == '\n')
               {
                  printf("--->Invalid Command\n");
                  printf("CMD USAGE:\n%s", CMD_USAGE);
                  break;
                }
                i=0;
                while(*p != ' ' && *p != '\t' && *p != '\n')
                {
                   dest[i++] = *p;
                   p++;
                }
                dest[i]='\0';
                if((stat = ooSendDTMFDigit(callToken, dest)) !=
                                                            OO_STKCMD_SUCCESS)
                {
                   printf("--->Failed to send DTMF digits\n");
                   printf("    Reason: %s\n", ooGetStkCmdStatusCodeTxt(stat));
                }
           }else {
              printf("--->There is no established call\n");
           }
           break;
          
         case 'i':
         default:
            printf("Invalid Command\n");
            printf("CMD USAGE:\n%s", CMD_USAGE);
      }
   }
     
   printf("--->Closing down stack\n");
   fflush(stdout);
   ooStopMonitor();
   gCmdThrd=FALSE;
   return dummy;
}
