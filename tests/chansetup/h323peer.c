/**
 * This sample program demonstrates basic H.323 channel setup and teardown.
 * It should be run using two instances:
 *
 * 1. Instance 1 with no parameters will go into listen mode and wait for a
 *    connection request.
 *
 * 2. Instance 2 with "[ip[:port]]" will initiate an outgoing call.
 *    If [ip[:port]] is not specified, call will be made to listener on
 *    the local machine.
 *
 * Once the channel is setup and established, the receiver will delay for
 * a short period of time and then hang up the call.
 */
#include "oochannels.h"
#include "ooh323ep.h"
#include "ooCalls.h"
#include "ooCapability.h"
#include "ooStackCmds.h"
#include "ooTimer.h"
#include "ooUtils.h"
#ifndef _WIN32
#include <pthread.h>
#endif

#ifdef _WIN32
#include <process.h>
#define getpid _getpid
#endif
/** Global endpoint structure */
extern ooEndPoint gH323ep;

static int startReceiveChannel (ooCallData *call, ooLogicalChannel *pChannel);
static int startTransmitChannel(ooCallData *call, ooLogicalChannel *pChannel);
static int stopReceiveChannel (ooCallData *call, ooLogicalChannel *pChannel);
static int stopTransmitChannel(ooCallData *call, ooLogicalChannel *pChannel);
static int onIncomingCall (ooCallData* call);
static int onNewCallCreated (ooCallData* call);
static int onCallEstablished(ooCallData *call);
static int onCallCleared (ooCallData* call);
static int onAlerting (ooCallData* call);
static int callDurationTimerExpired (void *pdata);
static int callIntervalTimerExpired(void *pdata);


static char USAGE[]={
   "Listen to incoming calls\n"
   "\th323peer [options] --listen\n"
   "Make test calls\n"
   "\th323peer [options] <remote>\n"
   "\t                    remote => Remote endpoint to callip:[port]\n"
   "\n\n"
   "options:\n"
   "Common Options for listen and Make call modes\n"
   "\t--help                    -  To display usage information\n"
   "\t--use-ip <ip>             -  local ip to use\n"
   "\t--use-port <port>         -  local port to use\n"
   "\t--gk-discover             -  Discover gatekeeper\n"
   "\t--gk <ip:[port]>          -  Use specific gatekeeper\n"
   "\t--user <username>         -  Local user name. Used as display name\n"
   "\t--user-number <number>    -  Local number\n"
   "\t--h323id <h323id>         -  H323ID to be used for this endpoint\n"
   "\t--no-faststart            -  Disable fast start(default - enabled)\n"
   "\t--no-tunneling            -  Disable tunneling (default - enabled)\n"
   "\t-t                        -  Trace. Use multiple times to increase \n"
   "\t                             trace level\n"
   "\t--tcp-min <port>          -  TCP port range - start\n"
   "\t--tcp-max <port>          -  TCP port range - end\n"
   "\t--udp-min <port>          -  UDP port range - start\n"
   "\t--udp-max <port>          -  UDP port range - end\n"
   "\t--rtp-min <port>          -  RTP port range start\n"
   "\t--rtp-max <port>          -  RTP port range end\n"
   "\n"
   "Options specific to make call mode\n"
   "\t-n <NoOfCalls>            -  Number of outgoing calls\n"
   "\t-duration <call duration> -  Duration of each call in seconds\n"
   "\t-interval <interval>      -  Interval between successive calls \n"
   "\t                             in seconds\n"
};
/* globals */
static int gCalls = 0;
static int gDuration = 5; /*sec*/
static int gInterval = 0; /* 0 interval means let previous call finish */
static char gDest[256];
static int gCallCounter=0;
static OOBOOL bListen = FALSE;

int main (int argc, char** argv)
{
   OOTimer *pTimer = NULL;
   OOBOOL bDestFound=FALSE, bFastStart=TRUE, bTunneling=TRUE;
   OOH323CALLBACKS h323Callbacks;
   int ret=0, x=0, localPort = 0, gkPort=0, tcpmin=0, tcpmax=0, udpmin=0;
   int udpmax=0, rtpmin=0, rtpmax=0;
   char localIPAddr[20], callToken[20], logfile[50], h323id[50], user[50];
   char user_num[50], tmp[40];
   char *pcPort =NULL, *gkIP=NULL, *token=NULL;
   unsigned char trace_level = 0;
   enum RasGatekeeperMode gkMode = RasNoGatekeeper;

#ifdef _WIN32
   ooSocketsInit (); /* Initialize the windows socket api  */
#endif
  
   localIPAddr[0] = '\0';
   gDest[0] = '\0';
   h323id[0]='\0';
   user[0]='\0';
   user_num[0]='\0';

   /* parse args */
   if(argc > 32  || argc == 1)
   {
      printf("USAGE:\n%s", USAGE);
      return -1;
   }
   for (x = 1; x < argc; x++) {
      if(!strcmp(argv[x], "--help")) {
         printf("USAGE:\n%s", USAGE);
         return 0;
      }
      else if(!strcmp(argv[x], "--listen"))
      {
         if(!bDestFound)
            bListen=TRUE;
         else{
            printf("USAGE:\n%s",USAGE);
            return -1;
         }
      }
      else if (!strcmp(argv[x], "--use-ip")) {
         x++;
         strncpy (localIPAddr, argv[x], sizeof(localIPAddr)-1);
      }
      else if (!strcmp(argv[x],"--use-port")) {
         x++;
         localPort = atoi (argv[x]);
      }
      else if (!strcmp(argv[x], "-n")){
         x++;
         gCalls= atoi(argv[x]);
      }
      else if(!strcmp(argv[x], "-duration")) {
         x++;
         gDuration = atoi(argv[x]);
      }
      else if(!strcmp(argv[x], "-interval")) {
         x++;
         gInterval = atoi(argv[x]);
      }
      else if(!strcmp(argv[x], "--gk-discover")){
         gkMode = RasDiscoverGatekeeper;
      }
      else if(!strcmp(argv[x], "--gk")){
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
      else if(!strcmp(argv[x], "--user")) {
         x++;
         strncpy(user, argv[x], sizeof(user)-1);
         user[sizeof(user)-1]='\0';
      }
      else if(!strcmp(argv[x], "--user-number")){
         x++;
         strncpy(user_num, argv[x], sizeof(user_num)-1);
         user_num[sizeof(user_num)-1]='\0';
      }
      else if(!strcmp(argv[x], "--h323id")){
         x++;
         strncpy(h323id, argv[x], sizeof(h323id)-1);
         h323id[sizeof(h323id)-1]='\0';
      }
      else if(!strcmp(argv[x], "--no-faststart")){
         bFastStart = FALSE;
      }
      else if(!strcmp(argv[x], "--no-tunneling")){
         bTunneling = FALSE;
      }
      else if(!strcmp(argv[x],"--tcp-min")){
         x++;
         tcpmin = atoi(argv[x]);
         if(tcpmin < 0 )
            tcpmin = 0;
      }
      else if(!strcmp(argv[x],"--tcp-max")){
         x++;
         tcpmax = atoi(argv[x]);
         if(tcpmax < 0 )
            tcpmax = 0;
      }
      else if(!strcmp(argv[x],"--udp-min")){
         x++;
         udpmin = atoi(argv[x]);
         if(udpmin < 0)
            udpmin = 0;
      }
      else if(!strcmp(argv[x],"--udp-max")){
         x++;
         udpmax = atoi(argv[x]);
         if(udpmax < 0)
            udpmax = 0;
      }
      else if(!strcmp(argv[x],"--rtp-min")){
         x++;
         rtpmin = atoi(argv[x]);
         if(rtpmin < 0)
            rtpmin = 0;
      }
      else if(!strcmp(argv[x],"--rtp-max")){
         x++;
         rtpmax = atoi(argv[x]);
         if(rtpmax < 0)
            rtpmax = 0;
      }
      else if(!strcmp(argv[x], "-t")){
         trace_level++;
      }
      else if (!bDestFound && !bListen) {
         strncpy (gDest, argv[x], sizeof(gDest)-1);
         bDestFound = TRUE;
      }
      else {
         printf("USAGE:\n%s",USAGE);
         return -1;
      }
   }
  
   if((!bListen && !bDestFound) || (bListen && bDestFound))
   {
      printf("USAGE:\n%s",USAGE);
      return -1;
   }
   if(bDestFound && !gCalls)
      gCalls++;

   /* Determine local IP address if not specified by user */
   if (ooUtilsIsStrEmpty(localIPAddr)) {                  
      ooGetLocalIPAddress (localIPAddr);
   }

   if (!strcmp (localIPAddr, "127.0.0.1")) {
      printf ("Failed to determine local IP address, "
              "please specify as command line option\n");
      printf ("USAGE:\n%s", USAGE);
      return -1;
   }

   sprintf(logfile, "h323peer_%d", getpid());      
          
   /* Initialize the H323 endpoint - faststart and tunneling enabled */
   printf ("Using:\n"
           "\tCalls to make : %d\n"
           "\tCall Duration : %d\n"
           "\tInterval      : %d\n"
           "\tlocal Address : %s:%d\n"
           "\tRemote Address: %s\n"
           "\tLog File      : %s\n",
           gCalls, gDuration, gInterval, localIPAddr, localPort,
           gDest[0] ? gDest : "(In listen mode)" , logfile);


   ret = ooH323EpInitialize(OO_CALLMODE_AUDIOCALL, logfile);
   if (ret != OO_OK) {
      printf ("Failed to initialize H.323 endpoint\n");
      return -1;
   }

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

   if(!ooUtilsIsStrEmpty(user))
      ooH323EpSetCallerID(user);
        

   if(!ooUtilsIsStrEmpty(h323id))
      ooH323EpAddAliasH323ID(h323id);

   if(!ooUtilsIsStrEmpty(user_num))
      ooH323EpSetCallingPartyNumber(user_num);

   if(tcpmax != 0 && tcpmin != 0 && tcpmin < tcpmax)
      ooH323EpSetTCPPortRange(tcpmin, tcpmax);

   if(udpmin != 0 && udpmax != 0 && udpmin < udpmax)
      ooH323EpSetUDPPortRange(udpmin,udpmax);

   if(rtpmin != 0 && rtpmax != 0 && rtpmin  < rtpmax)
      ooH323EpSetRTPPortRange(rtpmin,rtpmax);


   ooH323EpSetLocalAddress(localIPAddr, localPort);

   /* Register callbacks */
   h323Callbacks.onNewCallCreated = onNewCallCreated;
   h323Callbacks.onAlerting = onAlerting;
   h323Callbacks.onIncomingCall = onIncomingCall;
   h323Callbacks.onOutgoingCall = NULL;
   h323Callbacks.onCallEstablished = onCallEstablished;
   h323Callbacks.onCallCleared = onCallCleared;
   h323Callbacks.openLogicalChannels=NULL;
 
   ooH323EpSetH323Callbacks(h323Callbacks);


   /* Add audio capability */
   ooH323EpAddG711Capability (OO_G711ULAW64K,30, 240, OORXANDTX, &startReceiveChannel,
       &startTransmitChannel, &stopReceiveChannel, &stopTransmitChannel);

   if(gkMode != RasNoGatekeeper)
   {
      if(gkMode == RasDiscoverGatekeeper)
         ret = ooGkClientInit(RasDiscoverGatekeeper, NULL, 0);
      else if(gkMode == RasUseSpecificGatekeeper)
         ret = ooGkClientInit(RasUseSpecificGatekeeper, gkIP, gkPort);
      if(ret != OO_OK)
      {
         printf("Failed to initialize gatekeeper client\n");
         return -1;
      }
   }

   /* Create H.323 Listener */
   if(bListen)
   {
      ret = ooCreateH323Listener();
      if (ret != OO_OK)
      {
         OOTRACEERR1 ("Failed to Create H.323 Listener");
         return -1;
      }
   }

   if(bDestFound)
   {
      ooMakeCall (gDest, callToken, sizeof(callToken), NULL); /* Make call */
      gCallCounter++;
      if(gInterval != 0)
      {
         if( (token = (char * )malloc( strlen( callToken) + 1)) == NULL )             
         {
            printf("malloc failed\n");
            return 1;
         }                                                                      
         strcpy(token, callToken);
         pTimer = ooTimerCreate(&gH323ep.ctxt, NULL, callIntervalTimerExpired, gInterval,
                                 token, FALSE);
      }
   }

   /* Loop forever to process events */
   ooMonitorChannels();

   /* Application terminated - clean-up */

   ooH323EpDestroy();

   return 0;
}

/* Callback to start receive media channel */

static int startReceiveChannel (ooCallData *call, ooLogicalChannel *pChannel)
{
   printf ("Starting receive channel at %s:%d - %s\n",
           call->localIP, pChannel->localRtpPort, call->callToken);

   /* TODO: user would add application specific logic here to start     */
   /* the media receive channel..                                       */

   return OO_OK;
}

/* Callback to start transmit media channel */

static int startTransmitChannel (ooCallData *call, ooLogicalChannel *pChannel)
{
   OOTimer* timer = NULL;
   char *token=NULL;
   printf ("Starting transmit channel to %s:%d - %s\n",
           call->remoteIP, pChannel->remoteMediaPort, call->callToken);
   if(gCalls != 0)
   {
      token = (char*)malloc(strlen(call->callToken)+1);
      strcpy(token, call->callToken);
      timer =  ooTimerCreate (&gH323ep.ctxt, NULL, callDurationTimerExpired, gDuration,
                               token, FALSE);

   }
   /* TODO: user would add application specific logic here to start     */
   /* the media transmit channel..                                      */

   return OO_OK;
}

/* Callback to stop receive media channel */

static int stopReceiveChannel (ooCallData *call, ooLogicalChannel *pChannel)
{
   printf ("Stopping receive channel - %s\n", call->callToken);

   /* TODO: user would add application specific logic here to stop      */
   /* the media receive channel..                                       */

   return OO_OK;
}

/* Callback to stop transmit media channel */

static int stopTransmitChannel (ooCallData *call, ooLogicalChannel *pChannel)
{
   printf ("Stopping transmit channel - %s\n", call->callToken);

   /* TODO: user would add application specific logic here to stop      */
   /* the media transmit channel..                                      */

   return OO_OK;
}

/* Callback to process alerting signal */

static int onAlerting (ooCallData* call)
{
   printf ("onAlerting - %s\n", call->callToken);

   /* TODO: user would add application specific logic here to handle    */
   /* an H.225 alerting message..                                       */

   return OO_OK;
}

/* Callback to handle an incoming call request */

static int onIncomingCall (ooCallData* call)
{

   if(bListen)
         printf("onIncomingCall from\"%s\"(%s) ---- %s\n",
             (call->remoteDisplayName)?call->remoteDisplayName:"Unknown Name",
           call->callingPartyNumber?call->callingPartyNumber:"Unknown Number",call->callToken);
   /* TODO: user would add application specific logic here to handle    */
   /* an incoming call request..                                        */

   return OO_OK;
}

/* Callback to handle outgoing call admitted */

static int onNewCallCreated (ooCallData* call)
{
   printf ("onNewCallCreated - %s\n", call->callToken);

   /* TODO: user would add application specific logic here to handle    */
   /* outgoing call admitted..                                          */

   return OO_OK;
}

/* Callback to handle call established event */

int onCallEstablished(ooCallData *call)
{
   printf("onCallEstablished - %s\n", call->callToken);
   return OO_OK;
}

/* Callback to handle call cleared */

static int onCallCleared (ooCallData* call)
{
  char callToken[20];
  printf ("onCallCleared -(call end reason  %s ) -- %s\n",ooGetReasonCodeText(call->callEndReason), call->callToken);
   if(gInterval == 0 && !bListen)
   {
      if(gCallCounter < gCalls)
      {
         ooMakeCall (gDest, callToken, sizeof(callToken), NULL);
         gCallCounter++;
      }
      else{
         printf("Issuing StopMonitor command\n");
         ooStopMonitor();
      }
   }
   return OO_OK;
}

static int callDurationTimerExpired (void *pdata)
{
   printf("callDurationTimerExpired - %s\n", (char*)pdata);
   ooHangCall((char*)pdata, OO_REASON_LOCAL_CLEARED);
   free(pdata);
   return OO_OK;
}

static int callIntervalTimerExpired(void *pdata)
{
   char callToken[20];
   char *token=NULL;
   OOTimer* pTimer = NULL;
   printf("callIntervalTimerExpired\n");
   memset(callToken, 0, sizeof(callToken));
   if (gCallCounter < gCalls)
   {
      ooMakeCall (gDest, callToken, sizeof(callToken), NULL); /* Make call */
        if( ( token = (char*)malloc(strlen(callToken)+1)) == NULL)
        {
                printf("\nmalloc failed\n");
                return 1;
        }
      strcpy(token, callToken);
        pTimer =  ooTimerCreate(&gH323ep.ctxt, NULL, callIntervalTimerExpired, gInterval, token, FALSE);
      gCallCounter++;
   }

   return OO_OK;
}

