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

#include <string.h>
#include "ooh323ep.h"
#include "ootrace.h"
#include "ooCalls.h"
#include "ooCapability.h"
#include "ooGkClient.h"
#include "ooStackCmds.h"
#include "ooCmdChannel.h"
#include "ooUtils.h"

/** Global endpoint structure */
ooEndPoint gH323ep;

#ifdef _WIN32
extern int gCmdPort;
extern char gCmdIP[20];
#endif
extern DList g_TimerList;

static int setTraceFilename (const char* tracefile)
{
   if (!ooUtilsIsStrEmpty (tracefile)) {
      strncpy (gH323ep.traceFile, tracefile, MAXFILENAME-1);
      gH323ep.traceFile[MAXFILENAME-1] = '\0';
      if (strlen (tracefile)>= MAXFILENAME) {
         printf ("ERROR: File name longer than allowed maximum %d\n",
                 MAXFILENAME-1);

         return OO_FAILED;
      }
   }
   else {
      strcpy (gH323ep.traceFile, DEFAULT_TRACEFILE);
   }

   return 0;
}

int ooH323EpInitialize
   (enum OOCallMode callMode, const char* tracefile)
{
   memset(&gH323ep, 0, sizeof(ooEndPoint));

   initContext(&(gH323ep.ctxt));
   initContext(&(gH323ep.msgctxt));

   if (0 != setTraceFilename (tracefile)) {
      return OO_FAILED;
   }

   gH323ep.fptraceFile = fopen(gH323ep.traceFile, "a");
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

   ooH323EpSetCallerID(DEFAULT_CALLERID);


   gH323ep.myCaps = NULL;
   gH323ep.noOfCaps = 0;
   gH323ep.callList = NULL;
   gH323ep.dtmfmode = 0;
   gH323ep.callingPartyNumber[0]='\0';
   gH323ep.callMode = callMode;
   gH323ep.isGateway = FALSE;
   gH323ep.bearercap = Q931TransferUnrestrictedDigital;

   dListInit(&g_TimerList);/* This is for test application chansetup only*/

   gH323ep.callEstablishmentTimeout = DEFAULT_CALLESTB_TIMEOUT;

   gH323ep.msdTimeout = DEFAULT_MSD_TIMEOUT;

   gH323ep.tcsTimeout = DEFAULT_TCS_TIMEOUT;

   gH323ep.logicalChannelTimeout = DEFAULT_LOGICALCHAN_TIMEOUT;

   gH323ep.sessionTimeout = DEFAULT_ENDSESSION_TIMEOUT;
   gH323ep.ifList = NULL;

   ooSetTraceThreshold(OOTRCLVLINFO);
   OO_SETFLAG(gH323ep.flags, OO_M_ENDPOINTCREATED);

   gH323ep.cmdSock = 0;
#ifdef _WIN32
   gH323ep.cmdListener = 0;
   gH323ep.cmdPort = OO_DEFAULT_CMDLISTENER_PORT;
#endif
   return OO_OK;
}

static int h323EpConfigSetFlag (const OOConfigNVP* pnvp, unsigned mask)
{
   OOBOOL bvalue;
   if (0 == ooUtilsTextToBool (pnvp->value, &bvalue)) {
      if (bvalue) {
         OO_SETFLAG (gH323ep.flags, mask);
      }
      else {
         OO_CLRFLAG (gH323ep.flags, mask);
      }
   }
   else {
      OOTRACEERR3 ("ERROR: invalid value %s specified for %s\n",
                   pnvp->value, pnvp->name);
      return OO_FAILED;
   }

   return 0;
}

int ooH323EpApplyConfig (const OOConfigFile* pconfig)
{
   /* Note: this assumes the end point has already been initialized */
   OOConfigSection* pConfigSect;
   DListNode* pnode = pconfig->sections.head;

   /* Clear existing aliases.  List will be rebuilt from config. */
   ooH323EpClearAllAliases();

   while (0 != pnode) {
      pConfigSect = (OOConfigSection*) pnode->data;

      if (0 != pConfigSect) {
         OOConfigNVP* pnvp;
         DListNode* pnode2 = pConfigSect->nameValuePairs.head;

         if (!strcasecmp (pConfigSect->sectionName, "general")) {
            /* global variables section */
            while (0 != pnode2) {
               pnvp = (OOConfigNVP*) pnode2->data;

               /* trace/log file */
               if (!strcasecmp (pnvp->name, "logfile")) {
                  if (0 != setTraceFilename (pnvp->value)) {
                     return OO_FAILED;
                  }
                  /* Close existing trace file before opening new one */
                  if (0 != gH323ep.fptraceFile) {
                     fclose (gH323ep.fptraceFile);
                  }
                  gH323ep.fptraceFile = fopen (gH323ep.traceFile, "w");
                  if (0 == gH323ep.fptraceFile) {
                     printf ("ERROR: Failed to open trace file %s for write.\n",
                             gH323ep.traceFile);
                     return OO_FAILED;
                  }
               }

               /* listen port */
               else if (!strcasecmp (pnvp->name, "port")) {
                  gH323ep.listenPort = (int)strtol (pnvp->value, NULL, 10);
                  OOTRACEINFO2 ("Listen port number is set to %d\n",
                                gH323ep.listenPort);
               }

               /* local IP address */
               else if (!strcasecmp (pnvp->name, "bindaddr")) {
                  strcpy (gH323ep.signallingIP, pnvp->value);
#ifdef _WIN32
                  strcpy (gCmdIP, pnvp->value);
#endif
                  OOTRACEINFO2 ("Signalling IP address set to %s\n",
                                gH323ep.signallingIP);
               }

               /* gateway */
               else if (!strcasecmp (pnvp->name, "gateway")) {
                  OOBOOL bvalue;
                  if (0 == ooUtilsTextToBool (pnvp->value, &bvalue)) {
                     gH323ep.isGateway = bvalue;
                     OOTRACEINFO2 ("isGateway set to %d\n",
                                   gH323ep.isGateway);
                  }
                  else {
                     OOTRACEERR2 ("ERROR: invalid value %s specified for "
                                  "gateway\n", pnvp->value);
                  }
               }

               /* fast start */
               else if (!strcasecmp (pnvp->name, "faststart")) {
                  h323EpConfigSetFlag (pnvp, OO_M_FASTSTART);
               }

               /* media wait for connect */
               else if (!strcasecmp (pnvp->name, "mediawaitforconnect")) {
                  h323EpConfigSetFlag (pnvp, OO_M_MEDIAWAITFORCONN);
               }

               /* tunneling */
               else if (!strcasecmp (pnvp->name, "h245tunneling")) {
                  h323EpConfigSetFlag (pnvp, OO_M_TUNNELING);
               }

               /* H.323 ID alias */
               else if (!strcasecmp (pnvp->name, "h323id")) {
                  ooH323EpAddAliasH323ID (pnvp->value);
               }

               /* E.164 (dialed digits) alias */
               else if (!strcasecmp (pnvp->name, "e164")) {
                  ooH323EpAddAliasDialedDigits (pnvp->value);
               }

               /* EMail alias */
               else if (!strcasecmp (pnvp->name, "email")) {
                  ooH323EpAddAliasEmailID (pnvp->value);
               }

               /* Caller ID */
               else if (!strcasecmp (pnvp->name, "callerid")) {
                  ooH323EpSetCallerID (pnvp->value);
               }

               /* Gatekeeper */
               else if (!strcasecmp (pnvp->name, "gatekeeper")) {
                  if (!strcasecmp (pnvp->value, "DISABLE")) {
                     ooGkClientDestroy();
                  }
                  else if (!strcasecmp (pnvp->value, "DISCOVER")) {
                     ooGkClientInit (RasDiscoverGatekeeper, 0, 0);
                  }
                  else {
                     ooGkClientInit (RasUseSpecificGatekeeper, pnvp->value, 0);
                  }
               }

               /* DTMF mode */
               else if (!strcasecmp (pnvp->name, "dtmfmode")) {
                  if (!strcasecmp (pnvp->value, "rfc2833"))
                     gH323ep.dtmfmode |= OO_CAP_DTMF_RFC2833;
                  else if (!strcasecmp (pnvp->value, "q931keypad"))
                     gH323ep.dtmfmode |= OO_CAP_DTMF_Q931;
                  else if (!strcasecmp (pnvp->value, "h245alphanumeric"))
                     gH323ep.dtmfmode |= OO_CAP_DTMF_H245_alphanumeric;
                  else if (!strcasecmp (pnvp->value, "h245signal"))
                     gH323ep.dtmfmode |= OO_CAP_DTMF_H245_signal;
                  else {
                     OOTRACEERR2 ("ERROR: invalid value %s specified for "
                                  "dtmfmode\n", pnvp->value);
                  }
               }

               /* Bearer capability */
               else if (!strcasecmp (pnvp->name, "bearercap")) {
                  ooH323EpSetBearerCap (pnvp->value);
               }

               pnode2 = pnode2->next;
            }
         }

      }
      pnode = pnode->next;
   }

   return OO_OK;
}

int ooH323EpSetBearerCap (const char* configText)
{
   if (!strcasecmp (configText, "unrestricted_digital") ||
       !strcasecmp (configText, "unrestricteddigital")) {
      gH323ep.bearercap = Q931TransferUnrestrictedDigital;
   }
   else if (!strcasecmp (configText, "speech")) {
      gH323ep.bearercap = Q931TransferSpeech;
   }
   else {
      OOTRACEERR2 ("ERROR: invalid/unsupported value %s specified for "
                   "bearercap\n", configText);
      return OO_FAILED;
   }

   return OO_OK;
}

int ooH323EpSetAsGateway()
{
   gH323ep.isGateway = TRUE;
   return OO_OK;
}

int ooH323EpSetLocalAddress(const char* localip, int listenport)
{
   if(localip)
   {
      strcpy(gH323ep.signallingIP, localip);
#ifdef _WIN32
      strcpy(gCmdIP, localip);
#endif
      OOTRACEINFO2("Signalling IP address is set to %s\n", localip);
   }

   if(listenport)
   {
      gH323ep.listenPort = listenport;
      OOTRACEINFO2("Listen port number is set to %d\n", listenport);
   }
   return OO_OK;
}

#ifdef _WIN32
int ooH323EpCreateCmdListener(int cmdPort)
{
   if(cmdPort != 0)
   {
      gH323ep.cmdPort = cmdPort;
      gCmdPort = cmdPort;
   }
   if(ooCreateCmdListener() != OO_OK)
      return OO_FAILED;

   return OO_OK;
}
#endif

int ooH323EpAddAliasH323ID(const char *h323id)
{
   ooAliases * psNewAlias=NULL;
   psNewAlias = (ooAliases*)memAlloc(&gH323ep.ctxt, sizeof(ooAliases));
   if(!psNewAlias)
   {
      OOTRACEERR1("Error: Failed to allocate memory for new H323-ID alias\n");
      return OO_FAILED;
   }
   psNewAlias->type = T_H225AliasAddress_h323_ID;
   psNewAlias->registered = FALSE;
   psNewAlias->value = (char*) memAlloc(&gH323ep.ctxt, strlen(h323id)+1);
   if(!psNewAlias->value)
   {
      OOTRACEERR1("Error: Failed to allocate memory for the new H323-ID alias "
                  "value\n");
      memFreePtr(&gH323ep.ctxt, psNewAlias);
      return OO_FAILED;
   }
   strcpy(psNewAlias->value, h323id);
   psNewAlias->next = gH323ep.aliases;
   gH323ep.aliases = psNewAlias;
   OOTRACEDBGA2("Added alias: H323ID - %s\n", h323id);
   return OO_OK;
}

int ooH323EpAddAliasDialedDigits(const char* dialedDigits)
{
   ooAliases * psNewAlias=NULL;
   psNewAlias = (ooAliases*)memAlloc(&gH323ep.ctxt, sizeof(ooAliases));
   if(!psNewAlias)
   {
      OOTRACEERR1("Error: Failed to allocate memory for new DialedDigits "
                  "alias\n");
      return OO_FAILED;
   }
   psNewAlias->type = T_H225AliasAddress_dialedDigits;
   psNewAlias->registered = FALSE;
   psNewAlias->value = (char*) memAlloc(&gH323ep.ctxt, strlen(dialedDigits)+1);
   if(!psNewAlias->value)
   {
      OOTRACEERR1("Error: Failed to allocate memory for the new DialedDigits"
                  " alias value\n");
      memFreePtr(&gH323ep.ctxt, psNewAlias);
      return OO_FAILED;
   }
   strcpy(psNewAlias->value, dialedDigits);
   psNewAlias->next = gH323ep.aliases;
   gH323ep.aliases = psNewAlias;
   OOTRACEDBGA2("Added alias: DialedDigits - %s\n", dialedDigits);
   return OO_OK;
}

int ooH323EpAddAliasURLID(const char * url)
{
   ooAliases * psNewAlias=NULL;
   psNewAlias = (ooAliases*)memAlloc(&gH323ep.ctxt, sizeof(ooAliases));
   if(!psNewAlias)
   {
      OOTRACEERR1("Error: Failed to allocate memory for new URL-ID alias\n");
      return OO_FAILED;
   }
   psNewAlias->type = T_H225AliasAddress_url_ID;
   psNewAlias->registered = FALSE;
   psNewAlias->value = (char*) memAlloc(&gH323ep.ctxt, strlen(url)+1);
   if(!psNewAlias->value)
   {
      OOTRACEERR1("Error: Failed to allocate memory for the new URL-ID alias"
                  " value\n");
      memFreePtr(&gH323ep.ctxt, psNewAlias);
      return OO_FAILED;
   }
   strcpy(psNewAlias->value, url);
   psNewAlias->next = gH323ep.aliases;
   gH323ep.aliases = psNewAlias;
   OOTRACEDBGA2("Added alias: URL-ID - %s\n", url);
   return OO_OK;
}

int ooH323EpAddAliasEmailID(const char * email)
{
   ooAliases * psNewAlias=NULL;
   psNewAlias = (ooAliases*)memAlloc(&gH323ep.ctxt, sizeof(ooAliases));
   if(!psNewAlias)
   {
      OOTRACEERR1("Error: Failed to allocate memory for new Email-ID alias\n");
      return OO_FAILED;
   }
   psNewAlias->type = T_H225AliasAddress_email_ID;
   psNewAlias->registered = FALSE;
   psNewAlias->value = (char*) memAlloc(&gH323ep.ctxt, strlen(email)+1);
   if(!psNewAlias->value)
   {
      OOTRACEERR1("Error: Failed to allocate memory for the new Email-ID alias"
                  " value\n");
      memFreePtr(&gH323ep.ctxt, psNewAlias);
      return OO_FAILED;
   }
   strcpy(psNewAlias->value, email);
   psNewAlias->next = gH323ep.aliases;
   gH323ep.aliases = psNewAlias;
   OOTRACEDBGA2("Added alias: Email-ID - %s\n", email);
   return OO_OK;
}

int ooH323EpAddAliasTransportID(const char * ipaddress)
{
   ooAliases * psNewAlias=NULL;
   psNewAlias = (ooAliases*)memAlloc(&gH323ep.ctxt, sizeof(ooAliases));
   if(!psNewAlias)
   {
      OOTRACEERR1("Error: Failed to allocate memory for new Transport-ID "
                  "alias\n");
      return OO_FAILED;
   }
   psNewAlias->type = T_H225AliasAddress_transportID;
   psNewAlias->registered = FALSE;
   psNewAlias->value = (char*) memAlloc(&gH323ep.ctxt, strlen(ipaddress)+1);
   if(!psNewAlias->value)
   {
      OOTRACEERR1("Error: Failed to allocate memory for the new Transport-ID "
                  "alias value\n");
      memFreePtr(&gH323ep.ctxt, psNewAlias);
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
   ooAliases *pAlias = NULL, *pTemp;
   if(gH323ep.aliases)
   {
      pAlias = gH323ep.aliases;
      while(pAlias)
      {
         pTemp = pAlias;
         pAlias = pAlias->next;
         memFreePtr(&gH323ep.ctxt, pTemp);
      }
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
   gH323ep.h323Callbacks.onCallEstablished = h323Callbacks.onCallEstablished;
   gH323ep.h323Callbacks.onCallForwarded = h323Callbacks.onCallForwarded;
   gH323ep.h323Callbacks.onCallCleared = h323Callbacks.onCallCleared;
   gH323ep.h323Callbacks.openLogicalChannels = h323Callbacks.openLogicalChannels;
   gH323ep.h323Callbacks.onReceivedDTMF = h323Callbacks.onReceivedDTMF;
   gH323ep.h323Callbacks.onReceivedCommand = h323Callbacks.onReceivedCommand;
   gH323ep.h323Callbacks.onReceivedVideoFastUpdate = h323Callbacks.onReceivedVideoFastUpdate;
   return OO_OK;
}

int ooH323EpDestroy(void)
{
   /* free any internal memory allocated
      close trace file free context structure
   */
   OOH323CallData * cur, *temp;
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
            temp->callEndReason = OO_REASON_LOCAL_CLEARED;
            ooCleanCall(temp);
         }
         gH323ep.callList = NULL;
      }


      if(gH323ep.listener)
      {
         ooSocketClose(*(gH323ep.listener));
         gH323ep.listener = NULL;
      }
#ifdef _WIN32
      if(gH323ep.cmdListener != 0)
      {
         ooSocketClose(gH323ep.cmdListener);
         gH323ep.cmdListener = 0;
      }
#endif
      ooGkClientDestroy();

      if(gH323ep.fptraceFile)
      {
         fclose(gH323ep.fptraceFile);
         gH323ep.fptraceFile = NULL;
      }

      freeContext(&(gH323ep.ctxt));

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

int ooH323EpEnableManualRingback(void)
{
   OO_SETFLAG(gH323ep.flags, OO_M_MANUALRINGBACK);
   return OO_OK;
}


int ooH323EpDisableManualRingback(void)
{
   OO_CLRFLAG(gH323ep.flags, OO_M_MANUALRINGBACK);
   return OO_OK;
}

int ooH323EpEnableMediaWaitForConnect(void)
{
   OO_SETFLAG(gH323ep.flags, OO_M_MEDIAWAITFORCONN);
   return OO_OK;
}

int ooH323EpDisableMediaWaitForConnect(void)
{
   OO_CLRFLAG(gH323ep.flags, OO_M_MEDIAWAITFORCONN);
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
      if(gH323ep.productID)
         memFreePtr(&gH323ep.ctxt, gH323ep.productID);
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
      if(gH323ep.versionID)
         memFreePtr(&gH323ep.ctxt, gH323ep.versionID);
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
      if(gH323ep.callerid)
         memFreePtr(&gH323ep.ctxt, gH323ep.callerid);
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

   if(!OO_TESTFLAG(gH323ep.flags, OO_M_MEDIAWAITFORCONN))
   {
      OOTRACEINFO1("\tMediaWaitForConnect - disabled\n");
   }
   else{
      OOTRACEINFO1("\tMediaWaitForConnect - enabled\n");
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
   OOTRACEINFO2 ("Number of preferred capabilities = %d\n",
                 gH323ep.capPrefs.index);

   /* Print capabilities */
   if (0 != gH323ep.myCaps) {
      ooH323EpCapability* epCap = gH323ep.myCaps;
      while (0 != epCap) {
         OOTRACEINFO1 ("\n");
         ooCapabilityDiagPrint (epCap);
         epCap = epCap->next;
      }
   }
   else {
      OOTRACEINFO1 ("\tNo configured endpoint capabilities\n");
   }

   OOTRACEINFO1 ("\n");

   return;
}


int ooH323EpAddG711Capability(int cap, int txframes, int rxframes, int dir,
                              cb_StartReceiveChannel startReceiveChannel,
                              cb_StartTransmitChannel startTransmitChannel,
                              cb_StopReceiveChannel stopReceiveChannel,
                              cb_StopTransmitChannel stopTransmitChannel)
{
   return ooCapabilityAddSimpleCapability(NULL, cap, txframes, rxframes, FALSE,
                            dir, startReceiveChannel, startTransmitChannel,
                            stopReceiveChannel, stopTransmitChannel, FALSE);
}

int ooH323EpAddG728Capability(int cap, int txframes, int rxframes, int dir,
                              cb_StartReceiveChannel startReceiveChannel,
                              cb_StartTransmitChannel startTransmitChannel,
                              cb_StopReceiveChannel stopReceiveChannel,
                              cb_StopTransmitChannel stopTransmitChannel)
{
   return ooCapabilityAddSimpleCapability(NULL, cap, txframes, rxframes, FALSE,
                               dir, startReceiveChannel, startTransmitChannel,
                               stopReceiveChannel, stopTransmitChannel, FALSE);
}

int ooH323EpAddG729Capability(int cap, int txframes, int rxframes, int dir,
                              cb_StartReceiveChannel startReceiveChannel,
                              cb_StartTransmitChannel startTransmitChannel,
                              cb_StopReceiveChannel stopReceiveChannel,
                              cb_StopTransmitChannel stopTransmitChannel)
{
   return ooCapabilityAddSimpleCapability(NULL, cap, txframes, rxframes, FALSE,
                               dir, startReceiveChannel, startTransmitChannel,
                               stopReceiveChannel, stopTransmitChannel, FALSE);
}


int ooH323EpAddG7231Capability(int cap, int txframes, int rxframes,
                              OOBOOL silenceSuppression, int dir,
                              cb_StartReceiveChannel startReceiveChannel,
                              cb_StartTransmitChannel startTransmitChannel,
                              cb_StopReceiveChannel stopReceiveChannel,
                              cb_StopTransmitChannel stopTransmitChannel)
{
   return ooCapabilityAddSimpleCapability(NULL, cap, txframes, rxframes,
                             silenceSuppression, dir, startReceiveChannel,
                             startTransmitChannel, stopReceiveChannel,
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


int ooH323EpAddH263VideoCapability(int cap, unsigned sqcifMPI,
                                 unsigned qcifMPI, unsigned cifMPI,
                                 unsigned cif4MPI, unsigned cif16MPI,
                                 unsigned maxBitRate, int dir,
                                 cb_StartReceiveChannel startReceiveChannel,
                                 cb_StartTransmitChannel startTransmitChannel,
                                 cb_StopReceiveChannel stopReceiveChannel,
                                 cb_StopTransmitChannel stopTransmitChannel)
{

   return ooCapabilityAddH263VideoCapability(NULL, sqcifMPI, qcifMPI, cifMPI,
                                     cif4MPI, cif16MPI, maxBitRate,dir,
                                     startReceiveChannel, startTransmitChannel,
                                     stopReceiveChannel, stopTransmitChannel,
                                     FALSE);

}

int ooH323EpAddH264VideoCapability(unsigned maxBitRate, int dir,
                                 cb_StartReceiveChannel startReceiveChannel,
                                 cb_StartTransmitChannel startTransmitChannel,
                                 cb_StopReceiveChannel stopReceiveChannel,
                                 cb_StopTransmitChannel stopTransmitChannel){

  return ooCapabilityAddH264VideoCapability(NULL, maxBitRate,dir,
                                     startReceiveChannel, startTransmitChannel,
                                     stopReceiveChannel, stopTransmitChannel,
                                     FALSE);

}

int ooH323EpEnableDTMFRFC2833(int dynamicRTPPayloadType)
{
   return ooCapabilityEnableDTMFRFC2833(NULL, dynamicRTPPayloadType);
}

int ooH323EpDisableDTMFRFC2833(void)
{
   return ooCapabilityDisableDTMFRFC2833(NULL);
}

int ooH323EpEnableDTMFH245Alphanumeric()
{
   return ooCapabilityEnableDTMFH245Alphanumeric(NULL);
}

int ooH323EpDisableDTMFH245Alphanumeric()
{
   return ooCapabilityDisableDTMFH245Alphanumeric(NULL);
}

int ooH323EpEnableDTMFH245Signal()
{
   return ooCapabilityEnableDTMFH245Signal(NULL);
}

int ooH323EpDisableDTMFH245Signal()
{
   return ooCapabilityDisableDTMFH245Signal(NULL);
}

int ooH323EpEnableDTMFQ931Keypad()
{
   return ooCapabilityEnableDTMFQ931Keypad(NULL);
}

int ooH323EpDisableDTMFQ931Keypad()
{
   return ooCapabilityDisableDTMFQ931Keypad(NULL);
}

int ooH323EpSetGkClientCallbacks(OOGKCLIENTCALLBACKS gkClientCallbacks)
{

   if(gH323ep.gkClient)
   {
      return ooGkClientSetCallbacks(gH323ep.gkClient, gkClientCallbacks);
   }
   else{
      OOTRACEERR1("Error:Gk Client hasn't been initialized yet\n");
      return OO_FAILED;
   }

}



/* 0-1024 are reserved for well known services */
int ooH323EpSetTCPPortRange(int base, int max)
{
   if(base <= 1024)
      gH323ep.tcpPorts.start = 1025;
   else
      gH323ep.tcpPorts.start = base;
   if(max > 65500)
      gH323ep.tcpPorts.max = 65500;
   else
      gH323ep.tcpPorts.max = max;

   if(gH323ep.tcpPorts.max<gH323ep.tcpPorts.start)
   {
      OOTRACEERR1("Error: Failed to set tcp ports- "
                  "Max port number less than Start port number\n");
      return OO_FAILED;
   }
   gH323ep.tcpPorts.current = gH323ep.tcpPorts.start;

   OOTRACEINFO1("TCP port range initialize - successful\n");
   return OO_OK;
}

int ooH323EpSetUDPPortRange(int base, int max)
{
   if(base <= 1024)
      gH323ep.udpPorts.start = 1025;
   else
      gH323ep.udpPorts.start = base;
   if(max > 65500)
      gH323ep.udpPorts.max = 65500;
   else
      gH323ep.udpPorts.max = max;

   if(gH323ep.udpPorts.max<gH323ep.udpPorts.start)
   {
      OOTRACEERR1("Error: Failed to set udp ports- Max port number"
                  " less than Start port number\n");
      return OO_FAILED;
   }

   gH323ep.udpPorts.current = gH323ep.udpPorts.start;

   OOTRACEINFO1("UDP port range initialize - successful\n");

   return OO_OK;
}

int ooH323EpSetRTPPortRange(int base, int max)
{
   if(base <= 1024)
      gH323ep.rtpPorts.start = 1025;
   else
      gH323ep.rtpPorts.start = base;
   if(max > 65500)
      gH323ep.rtpPorts.max = 65500;
   else
      gH323ep.rtpPorts.max = max;

   if(gH323ep.rtpPorts.max<gH323ep.rtpPorts.start)
   {
      OOTRACEERR1("Error: Failed to set rtp ports- Max port number"
                  " less than Start port number\n");
      return OO_FAILED;
   }

   gH323ep.rtpPorts.current = gH323ep.rtpPorts.start;
   OOTRACEINFO1("RTP port range initialize - successful\n");
   return OO_OK;
}

