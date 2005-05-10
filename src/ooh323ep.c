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
#include "ooh323ep.h"
#include "ootrace.h"
#include "ooCalls.h"
#include "ooCapability.h"
#include "ooGkClient.h"

/** Global endpoint structure */
ooEndPoint gH323ep;

/**
 * Mutex to protect ooGenerateCallReference function.
 * This is required as this function will be called by
 * multiple threads trying to place calls using stack commands
 */
OO_MUTEX gCallRefMutex;

/**
 * Mutex to protect access to global call token variables.
 * This is required as these variables will be used by
 * multiple threads trying to place calls using stack commands
 */
OO_MUTEX gCallTokenMutex;

/**
 * Mutex to protect access to stack commands list
 */
OO_MUTEX gCmdMutex;

extern DList g_TimerList;

int ooH323EpInitialize
   (const char *callerid, int callMode, const char* tracefile)
{
  
   memset(&gH323ep, 0, sizeof(ooEndPoint));

   initContext(&(gH323ep.ctxt));
   initContext(&(gH323ep.msgctxt));

   if(tracefile)
   {
      if(strlen(tracefile)>= MAXFILENAME)
      {
         printf("Error:File name longer than allowed maximum %d\n",
                 MAXFILENAME-1);
         return OO_FAILED;
      }
      strcpy(gH323ep.traceFile, tracefile);
   }else{
      strcpy(gH323ep.traceFile, DEFAULT_TRACEFILE);     
   }

   gH323ep.fptraceFile = fopen(gH323ep.traceFile, "w");
   if(gH323ep.fptraceFile == NULL)
   {
      printf("Error:Failed to open trace file %s for write.\n",
                  gH323ep.traceFile);
      return OO_FAILED;
   }

   /* Initialize default port ranges that will be used by stack.
      Apps can override these by explicitely setting port ranges
   */

   gH323ep.tcpPorts.start = TCPPORTSSTART;
   gH323ep.tcpPorts.max = TCPPORTSEND;
   gH323ep.tcpPorts.current=TCPPORTSSTART;

   gH323ep.udpPorts.start = UDPPORTSSTART;
   gH323ep.udpPorts.max = UDPPORTSEND;
   gH323ep.udpPorts.current = UDPPORTSSTART;

   gH323ep.rtpPorts.start = RTPPORTSSTART;
   gH323ep.rtpPorts.max = RTPPORTSEND;
   gH323ep.rtpPorts.current = RTPPORTSSTART;
  
   OO_SETFLAG(gH323ep.flags, OO_M_FASTSTART);
   OO_SETFLAG(gH323ep.flags, OO_M_TUNNELING);
   OO_SETFLAG(gH323ep.flags, OO_M_AUTOANSWER);
   OO_CLRFLAG(gH323ep.flags, OO_M_GKROUTED);
  
   gH323ep.aliases = NULL;
  
   gH323ep.termType = DEFAULT_TERMTYPE;

   gH323ep.t35CountryCode = DEFAULT_T35COUNTRYCODE;

   gH323ep.t35Extension = DEFAULT_T35EXTENSION;

   gH323ep.manufacturerCode = DEFAULT_MANUFACTURERCODE;

   gH323ep.productID = DEFAULT_PRODUCTID;

   gH323ep.versionID = OOH323C_VERSION;

   gH323ep.callType = T_H225CallType_pointToPoint;
   ooGetLocalIPAddress(gH323ep.signallingIP);
   gH323ep.listenPort = DEFAULT_H323PORT;

   gH323ep.listener = NULL;

   if (0 != callerid) {
      ooH323EpSetCallerID (callerid);
   }
   else {
      gH323ep.callerid = DEFAULT_CALLERID;
   }

   gH323ep.myCaps = NULL;
   gH323ep.noOfCaps = 0;
   gH323ep.callList = NULL;
   gH323ep.dtmfmode = 0;
   gH323ep.callingPartyNumber[0]='\0';    
   gH323ep.callMode = callMode;
#ifdef _WIN32
   InitializeCriticalSection(&gCmdMutex);
   InitializeCriticalSection(&gCallTokenMutex);
   InitializeCriticalSection(&gCallRefMutex);
#else
   pthread_mutex_init(&gCmdMutex, 0);
   pthread_mutex_init(&gCallTokenMutex, 0);
   pthread_mutex_init(&gCallRefMutex, 0);
#endif
   dListInit(&g_TimerList);/* This is for test application chansetup only*/

   gH323ep.callEstablishmentTimeout = DEFAULT_CALLESTB_TIMEOUT;

   gH323ep.msdTimeout = DEFAULT_MSD_TIMEOUT;

   gH323ep.tcsTimeout = DEFAULT_TCS_TIMEOUT;

   gH323ep.logicalChannelTimeout = DEFAULT_LOGICALCHAN_TIMEOUT;

   gH323ep.sessionTimeout = DEFAULT_ENDSESSION_TIMEOUT;
#ifdef HAVE_PIPE
   if(pipe(gH323ep.cmdPipe)<0){
      OOTRACEERR1("Error:Failed to create command pipe\n");
      return OO_FAILED;
   }
#endif
   ooSetTraceThreshold(OOTRCLVLINFO);
   OO_SETFLAG(gH323ep.flags, OO_M_ENDPOINTCREATED);
   return OO_OK;
}

int ooH323EpSetLocalAddress(char * localip, int listenport)
{
   if(localip)
   {
      memset(gH323ep.signallingIP, 0, sizeof(gH323ep.signallingIP));
      strcpy(gH323ep.signallingIP, localip);
      OOTRACEINFO2("Signalling IP address is set to %s\n", localip);
   }
  
   if(listenport)
   {
      gH323ep.listenPort = listenport;
      OOTRACEINFO2("Listen port number is set to %d\n", listenport);
   }
   return OO_OK;
}

int ooH323EpAddAliasH323ID(char *h323id)
{
   ooAliases * psNewAlias=NULL;
   psNewAlias = (ooAliases*)ASN1MALLOC(&gH323ep.ctxt, sizeof(ooAliases));
   if(!psNewAlias)
   {
      OOTRACEERR1("Error: Failed to allocate memory for new H323-ID alias\n");
      return OO_FAILED;
   }
   psNewAlias->type = T_H225AliasAddress_h323_ID;
   psNewAlias->value = (char*) ASN1MALLOC(&gH323ep.ctxt, strlen(h323id)+1);
   if(!psNewAlias->value)
   {
      OOTRACEERR1("Error: Failed to allocate memory for the new H323-ID alias "
                  "value\n");
      ASN1MEMFREEPTR(&gH323ep.ctxt, psNewAlias);
      return OO_FAILED;
   }
   strcpy(psNewAlias->value, h323id);
   psNewAlias->next = gH323ep.aliases;
   gH323ep.aliases = psNewAlias;
   OOTRACEDBGA2("Added alias: H323ID - %s\n", h323id);
   return OO_OK;
}

int ooH323EpAddAliasDialedDigits(char * dialedDigits)
{
   ooAliases * psNewAlias=NULL;
   psNewAlias = (ooAliases*)ASN1MALLOC(&gH323ep.ctxt, sizeof(ooAliases));
   if(!psNewAlias)
   {
      OOTRACEERR1("Error: Failed to allocate memory for new DialedDigits alias\n");
      return OO_FAILED;
   }
   psNewAlias->type = T_H225AliasAddress_dialedDigits;
   psNewAlias->value = (char*) ASN1MALLOC(&gH323ep.ctxt, strlen(dialedDigits)+1);
   if(!psNewAlias->value)
   {
      OOTRACEERR1("Error: Failed to allocate memory for the new DialedDigits alias value\n");
      ASN1MEMFREEPTR(&gH323ep.ctxt, psNewAlias);
      return OO_FAILED;
   }
   strcpy(psNewAlias->value, dialedDigits);
   psNewAlias->next = gH323ep.aliases;
   gH323ep.aliases = psNewAlias;
   OOTRACEDBGA2("Added alias: DialedDigits - %s\n", dialedDigits);
   return OO_OK;
}

int ooH323EpAddAliasURLID(char * url)
{
   ooAliases * psNewAlias=NULL;
   psNewAlias = (ooAliases*)ASN1MALLOC(&gH323ep.ctxt, sizeof(ooAliases));
   if(!psNewAlias)
   {
      OOTRACEERR1("Error: Failed to allocate memory for new URL-ID alias\n");
      return OO_FAILED;
   }
   psNewAlias->type = T_H225AliasAddress_url_ID;
   psNewAlias->value = (char*) ASN1MALLOC(&gH323ep.ctxt, strlen(url)+1);
   if(!psNewAlias->value)
   {
      OOTRACEERR1("Error: Failed to allocate memory for the new URL-ID alias value\n");
      ASN1MEMFREEPTR(&gH323ep.ctxt, psNewAlias);
      return OO_FAILED;
   }
   strcpy(psNewAlias->value, url);
   psNewAlias->next = gH323ep.aliases;
   gH323ep.aliases = psNewAlias;
   OOTRACEDBGA2("Added alias: URL-ID - %s\n", url);
   return OO_OK;
}

int ooH323EpAddAliasEmailID(char * email)
{
   ooAliases * psNewAlias=NULL;
   psNewAlias = (ooAliases*)ASN1MALLOC(&gH323ep.ctxt, sizeof(ooAliases));
   if(!psNewAlias)
   {
      OOTRACEERR1("Error: Failed to allocate memory for new Email-ID alias\n");
      return OO_FAILED;
   }
   psNewAlias->type = T_H225AliasAddress_email_ID;
   psNewAlias->value = (char*) ASN1MALLOC(&gH323ep.ctxt, strlen(email)+1);
   if(!psNewAlias->value)
   {
      OOTRACEERR1("Error: Failed to allocate memory for the new Email-ID alias"
                  " value\n");
      ASN1MEMFREEPTR(&gH323ep.ctxt, psNewAlias);
      return OO_FAILED;
   }
   strcpy(psNewAlias->value, email);
   psNewAlias->next = gH323ep.aliases;
   gH323ep.aliases = psNewAlias;
   OOTRACEDBGA2("Added alias: Email-ID - %s\n", email);
   return OO_OK;
}

int ooH323EpAddAliasTransportID(char * ipaddress)
{
   ooAliases * psNewAlias=NULL;
   psNewAlias = (ooAliases*)ASN1MALLOC(&gH323ep.ctxt, sizeof(ooAliases));
   if(!psNewAlias)
   {
      OOTRACEERR1("Error: Failed to allocate memory for new Transport-ID alias\n");
      return OO_FAILED;
   }
   psNewAlias->type = T_H225AliasAddress_transportID;
   psNewAlias->value = (char*) ASN1MALLOC(&gH323ep.ctxt, strlen(ipaddress)+1);
   if(!psNewAlias->value)
   {
      OOTRACEERR1("Error: Failed to allocate memory for the new Transport-ID "
                  "alias value\n");
      ASN1MEMFREEPTR(&gH323ep.ctxt, psNewAlias);
      return OO_FAILED;
   }
   strcpy(psNewAlias->value, ipaddress);
   psNewAlias->next = gH323ep.aliases;
   gH323ep.aliases = psNewAlias;
   OOTRACEDBGA2("Added alias: Transport-ID - %s\n", ipaddress);
   return OO_OK;
}

int ooH323EpClearAllAliases(void)
{
   if(gH323ep.aliases)
   {
      memFreePtr(&gH323ep.ctxt, gH323ep.aliases);
      gH323ep.aliases = NULL;
   }
   return OO_OK;
}


int ooH323EpSetH225MsgCallbacks(OOH225MsgCallbacks h225Callbacks)
{
   gH323ep.h225Callbacks.onReceivedSetup = h225Callbacks.onReceivedSetup;
   gH323ep.h225Callbacks.onReceivedConnect = h225Callbacks.onReceivedConnect;
   gH323ep.h225Callbacks.onBuiltSetup = h225Callbacks.onBuiltSetup;
   gH323ep.h225Callbacks.onBuiltConnect = h225Callbacks.onBuiltConnect;

   return OO_OK;
}
   
int ooH323EpSetH323Callbacks(OOH323CALLBACKS h323Callbacks)
{
   gH323ep.h323Callbacks.onNewCallCreated = h323Callbacks.onNewCallCreated;
   gH323ep.h323Callbacks.onAlerting = h323Callbacks.onAlerting;
   gH323ep.h323Callbacks.onIncomingCall = h323Callbacks.onIncomingCall;
   gH323ep.h323Callbacks.onOutgoingCall = h323Callbacks.onOutgoingCall;
   gH323ep.h323Callbacks.onCallAnswered = h323Callbacks.onCallAnswered;
   gH323ep.h323Callbacks.onCallEstablished = h323Callbacks.onCallEstablished;
   gH323ep.h323Callbacks.onOutgoingCallAdmitted = h323Callbacks.onOutgoingCallAdmitted;
   gH323ep.h323Callbacks.onCallCleared = h323Callbacks.onCallCleared;
   gH323ep.h323Callbacks.openLogicalChannels = h323Callbacks.openLogicalChannels;
   return OO_OK;
}

int ooH323EpDestroy(void)
{
   /* free any internal memory allocated
      close trace file free context structure
   */
   ooCallData * cur, *temp;
   if(OO_TESTFLAG(gH323ep.flags, OO_M_ENDPOINTCREATED))
   {
      OOTRACEINFO1("Destroying H323 Endpoint\n");
      if(gH323ep.callList)
      {
         cur = gH323ep.callList;
         while(cur)
         {
            temp = cur;
            cur = cur->next;
            temp->callEndReason = OO_HOST_CLEARED;
            ooCleanCall(temp);
         }
         gH323ep.callList = NULL;
      }


      if(gH323ep.listener)
      {
         ooSocketClose(*(gH323ep.listener));
         gH323ep.listener = NULL;  
      }

      ooGkClientDestroy(); 

      if(gH323ep.fptraceFile)
      {
         fclose(gH323ep.fptraceFile);
         gH323ep.fptraceFile = NULL;
      }

      freeContext(&(gH323ep.ctxt));

#ifdef _WIN32
      DeleteCriticalSection(&gCmdMutex);
      DeleteCriticalSection(&gCallTokenMutex);
      DeleteCriticalSection(&gCallRefMutex);
#endif
      OO_CLRFLAG(gH323ep.flags, OO_M_ENDPOINTCREATED);
   }
   return OO_OK;
}

int ooH323EpEnableGkRouted(void)
{
   OO_SETFLAG(gH323ep.flags, OO_M_GKROUTED);
   return OO_OK;
}

int ooH323EpDisableGkRouted(void)
{
   OO_CLRFLAG(gH323ep.flags, OO_M_GKROUTED);
   return OO_OK;
}

int ooH323EpEnableAutoAnswer(void)
{
   OO_SETFLAG(gH323ep.flags, OO_M_AUTOANSWER);
   return OO_OK;
}

int ooH323EpDisableAutoAnswer(void)
{
   OO_CLRFLAG(gH323ep.flags, OO_M_AUTOANSWER);
   return OO_OK;
}

int ooH323EpEnableFastStart(void)
{
   OO_SETFLAG(gH323ep.flags, OO_M_FASTSTART);
   return OO_OK;
}

int ooH323EpDisableFastStart(void)
{
   OO_CLRFLAG(gH323ep.flags, OO_M_FASTSTART);
   return OO_OK;
}

int ooH323EpEnableH245Tunneling(void)
{
   OO_SETFLAG(gH323ep.flags, OO_M_TUNNELING);
   return OO_OK;
}

int ooH323EpDisableH245Tunneling(void)
{
   OO_CLRFLAG(gH323ep.flags, OO_M_TUNNELING);
   return OO_OK;
}

int ooH323EpSetTermType(int value)
{
   gH323ep.termType = value;
   return OO_OK;
}

int ooH323EpSetProductID (const char* productID)
{
   if (0 != productID) {
      char* pstr = (char*) memAlloc (&gH323ep.ctxt, strlen(productID)+1);
      strcpy (pstr, productID);
      gH323ep.productID = pstr;
      return OO_OK;
   }
   else return OO_FAILED;
}

int ooH323EpSetVersionID (const char* versionID)
{
   if (0 != versionID) {
      char* pstr = (char*) memAlloc (&gH323ep.ctxt, strlen(versionID)+1);
      strcpy (pstr, versionID);
      gH323ep.versionID = pstr;
      return OO_OK;
   }
   else return OO_FAILED;
}

int ooH323EpSetCallerID (const char* callerID)
{
   if (0 != callerID) {
      char* pstr = (char*) memAlloc (&gH323ep.ctxt, strlen(callerID)+1);
      strcpy (pstr, callerID);
      gH323ep.callerid = pstr;
      return OO_OK;
   }
   else return OO_FAILED;
}

int ooH323EpSetCallingPartyNumber(const char* number)
{
   int ret=OO_OK;
   if(number)
   {
      strncpy(gH323ep.callingPartyNumber, number,
                                        sizeof(gH323ep.callingPartyNumber)-1);
      ret = ooH323EpAddAliasDialedDigits((char*)number);
      return ret;
   }
   else return OO_FAILED;
}

int ooH323EpSetTraceLevel(int traceLevel)
{
   ooSetTraceThreshold(traceLevel);
   return OO_OK;
}

void ooH323EpPrintConfig(void)
{
   OOTRACEINFO1("H.323 Endpoint Configuration is as follows:\n");
  
   OOTRACEINFO2("\tTrace File: %s\n", gH323ep.traceFile);

   if(!OO_TESTFLAG(gH323ep.flags, OO_M_FASTSTART))
   {
      OOTRACEINFO1("\tFastStart - disabled\n");
   }
   else{
      OOTRACEINFO1("\tFastStart - enabled\n");
   }

   if(!OO_TESTFLAG(gH323ep.flags, OO_M_TUNNELING))
   {
      OOTRACEINFO1("\tH245 Tunneling - disabled\n");
   }
   else{
      OOTRACEINFO1("\tH245 Tunneling - enabled\n");
   }

   if(OO_TESTFLAG(gH323ep.flags, OO_M_AUTOANSWER))
      OOTRACEINFO1("\tAutoAnswer - enabled\n");
   else
      OOTRACEINFO1("\tAutoAnswer - disabled\n");
    
   OOTRACEINFO2("\tTerminal Type - %d\n", gH323ep.termType);

   OOTRACEINFO2("\tT35 CountryCode - %d\n", gH323ep.t35CountryCode);

   OOTRACEINFO2("\tT35 Extension - %d\n", gH323ep.t35Extension);

   OOTRACEINFO2("\tManufacturer Code - %d\n", gH323ep.manufacturerCode);

   OOTRACEINFO2("\tProductID - %s\n", gH323ep.productID);
  
   OOTRACEINFO2("\tVersionID - %s\n", gH323ep.versionID);

   OOTRACEINFO2("\tLocal signalling IP address - %s\n", gH323ep.signallingIP);

   OOTRACEINFO2("\tH225 ListenPort - %d\n", gH323ep.listenPort);

   OOTRACEINFO2("\tCallerID - %s\n", gH323ep.callerid);


   OOTRACEINFO2("\tCall Establishment Timeout - %d seconds\n",
                                          gH323ep.callEstablishmentTimeout);  

   OOTRACEINFO2("\tMasterSlaveDetermination Timeout - %d seconds\n",
                   gH323ep.msdTimeout);

   OOTRACEINFO2("\tTerminalCapabilityExchange Timeout - %d seconds\n",
                   gH323ep.tcsTimeout);

   OOTRACEINFO2("\tLogicalChannel  Timeout - %d seconds\n",
                   gH323ep.logicalChannelTimeout);

   OOTRACEINFO2("\tSession Timeout - %d seconds\n", gH323ep.sessionTimeout);

   return;  
}


int ooH323EpAddG711Capability(int cap, int txframes, int rxframes, int dir,
                              cb_StartReceiveChannel startReceiveChannel,
                              cb_StartTransmitChannel startTransmitChannel,
                              cb_StopReceiveChannel stopReceiveChannel,
                              cb_StopTransmitChannel stopTransmitChannel)
{
   return ooCapabilityAddG711Capability(NULL, cap, txframes, rxframes, dir,
         startReceiveChannel, startTransmitChannel, stopReceiveChannel,
         stopTransmitChannel, FALSE);
}


int ooH323EpAddGSMCapability(int cap, ASN1USINT framesPerPkt,
                             OOBOOL comfortNoise, OOBOOL scrambled, int dir,
                             cb_StartReceiveChannel startReceiveChannel,
                             cb_StartTransmitChannel startTransmitChannel,
                             cb_StopReceiveChannel stopReceiveChannel,
                             cb_StopTransmitChannel stopTransmitChannel)
{
   return ooCapabilityAddGSMCapability(NULL, cap, framesPerPkt, comfortNoise,
                                     scrambled, dir, startReceiveChannel,
                                     startTransmitChannel, stopReceiveChannel,
                                     stopTransmitChannel, FALSE);
}

int ooH323EpEnableRFC2833(int dynamicRTPPayloadType)
{
   return ooCapabilityEnableRFC2833(NULL, dynamicRTPPayloadType);
}

int ooH323EpDisableRFC2833(void)
{
  return ooCapabilityDisableRFC2833(NULL);
}

