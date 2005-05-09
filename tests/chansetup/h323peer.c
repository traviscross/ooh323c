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
static int onOutgoingCallAdmitted (ooCallData* call);
static int onCallCleared (ooCallData* call);
static int onAlerting (ooCallData* call);
static void* runtest (void*);
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
   "Options specific to make call mode\n"
   "\t-n <NoOfCalls>            -  Number of outgoing calls\n"
   "\t-duration <call duration> -  Duration of each call in seconds\n"
   "\t-interval <interval>      -  Interval between successive calls in seconds\n"
};

static int gCalls = 0;
static int gDuration = 5; /*sec*/
static int gInterval = 0; /* 0 interval means let previous call finish */
static char gDest[256];
static int gCallCounter=0;

int main (int argc, char** argv)
{
   char callToken[20], *token=NULL;
   char localIPAddr[20];
   int  localPort = 0;
   OOTimer *pTimer = NULL;
   int ret=0, x;
   OOBOOL isActive = FALSE;
   OOBOOL bDestFound=FALSE;
   OOBOOL bListen = FALSE;
   char logfile[50];

   OOH323CALLBACKS h323Callbacks ={
     .onNewCallCreated = NULL,
     .onAlerting = onAlerting,
     .onIncomingCall = onIncomingCall,
     .onOutgoingCall = NULL,
     .onCallAnswered = NULL,
     .onCallEstablished = NULL,
     .onOutgoingCallAdmitted = onOutgoingCallAdmitted,
     .onCallCleared = onCallCleared,
     .openLogicalChannels=NULL
  };
#ifdef _WIN32
   ooSocketsInit (); /* Initialize the windows socket api  */
#endif
  
   localIPAddr[0] = '\0';
   gDest[0] = '\0';
  
   /* parse args */

   if(argc > 12 || argc ==1)
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
      else if (!bDestFound && !bListen) {
         strncpy (gDest, argv[x], sizeof(gDest)-1);
         bDestFound = TRUE;
         if (0 == gCalls) gCalls++;
      }
      else {
         printf("USAGE:\n%s",USAGE);
         return -1;
      }
   }
  
    if(!bListen && !bDestFound)
   {
      printf("USAGE:\n%s",USAGE);
      return -1;
   }
   /* Determine local IP address if not specified by user */

   if (0 == strlen(localIPAddr)) {
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
           gDest, logfile);

   ret = ooH323EpInitialize("objsyscall", OO_CALLMODE_AUDIOCALL, logfile);

   if (ret != OO_OK) {
      printf ("Failed to initialize H.323 endpoint\n");
      return -1;
   }

   ooH323EpSetLocalAddress(localIPAddr, localPort);
   ooH323EpAddAliasH323ID ("objsys");
   ooH323EpAddAliasDialedDigits ("5087556929");
   ooH323EpAddAliasURLID ("http://www.obj-sys.com");

   /* Register callbacks */
   ooH323EpSetH323Callbacks(h323Callbacks);


   /* Add audio capability */

   ooH323EpAddG711Capability
      (OO_G711ULAW64K,30, 240, OORXANDTX, &startReceiveChannel,
       &startTransmitChannel, &stopReceiveChannel, &stopTransmitChannel);

   /* Create H.323 Listener */
   if(bListen){
      ret = ooCreateH323Listener();

      if (ret != OO_OK) {
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
         token = (char*)malloc(strlen(callToken)+1);
         strcpy(token, callToken);
         pTimer = ooTimerCreate
            (&gH323ep.ctxt, NULL, callIntervalTimerExpired,
             gInterval, token, FALSE);

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
           call->remoteIP, pChannel->mediaPort, call->callToken);
   if(gCalls != 0)
   {
      token = (char*)malloc(strlen(call->callToken)+1);
      strcpy(token, call->callToken);
      timer =  ooTimerCreate
         (&gH323ep.ctxt, NULL, callDurationTimerExpired, gDuration,
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
   printf ("onIncomingCall - %s\n", call->callToken);

   /* TODO: user would add application specific logic here to handle    */
   /* an incoming call request..                                        */

   return OO_OK;
}

/* Callback to handle outgoing call admitted */

static int onOutgoingCallAdmitted (ooCallData* call)
{
   printf ("onOutgoingCallAdmitted - %s\n", call->callToken);

   /* TODO: user would add application specific logic here to handle    */
   /* outgoing call admitted..                                          */

   return OO_OK;
}

/* Callback to handle call cleared */

static int onCallCleared (ooCallData* call)
{
  char callToken[20];
   printf ("onCallCleared - %s\n", call->callToken);
   if (gInterval == 0) {
      if (gCallCounter < gCalls) {
         ooMakeCall (gDest, callToken, sizeof(callToken), NULL);
         gCallCounter++;
      }
      else {
         ooStopMonitor();
      }
   }
   return OO_OK;
}

static int callDurationTimerExpired (void *pdata)
{
   printf("callDurationTimerExpired - %s\n", (char*)pdata);
   ooHangCall((char*)pdata);
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
      token = (char*)malloc(strlen(callToken)+1);
      strcpy(token, callToken);
      pTimer =  ooTimerCreate(&gH323ep.ctxt, NULL, callIntervalTimerExpired,
                               gInterval, token, FALSE);
      gCallCounter++;
   }

   return OO_OK;
}

