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
#include "oo.h"
#include "ooCalls.h"
#include "ooCapability.h"
#include "ooras.h"

/** Global endpoint structure */
ooEndPoint gH323ep;

/* Initialize the application context within stack */
int ooInitializeH323Ep( const char * tracefile, int h245Tunneling,
                        int fastStart, int termType, int t35CountryCode,
                        int t35extension, int manufacturerCode,
                        char *productID, char *versionID, int callType,
                        int listenport, char *callerid, char *callername,
                        int callMode)
{
  
   initContext(&(gH323ep.ctxt));
   initContext(&(gH323ep.msgctxt));
  
   if(tracefile != NULL)
   {
      gH323ep.fptraceFile = fopen(tracefile, "w");
      if(gH323ep.fptraceFile == NULL)
      {
         return OO_FAILED;
      }
   }
   else
   {
      gH323ep.fptraceFile = stdout;
   }
  
   /* Initialize default port ranges that will be used by stack.
      Apps can override these by explicitely setting port ranges
   */
   OOTRACEINFO1("Initializing H.323 endpoint\n");
   gH323ep.tcpPorts.start = TCPPORTSSTART;
   gH323ep.tcpPorts.max = TCPPORTSEND;
   gH323ep.tcpPorts.current=TCPPORTSSTART;

   gH323ep.udpPorts.start = UDPPORTSSTART;
   gH323ep.udpPorts.max = UDPPORTSEND;
   gH323ep.udpPorts.current = UDPPORTSSTART;

   gH323ep.rtpPorts.start = RTPPORTSSTART;
   gH323ep.rtpPorts.max = RTPPORTSEND;
   gH323ep.rtpPorts.current = RTPPORTSSTART;
  
   /* FastStart */
   if(!fastStart)
   {
      OOTRACEINFO1("\tFastStart - disabled\n");
   }
   else{
      OOTRACEINFO1("\tFastStart - enabled\n");
   }
   gH323ep.fastStart = fastStart;
   gH323ep.aliases = NULL;
  
   /* H245 Tunneling */
   gH323ep.h245Tunneling = h245Tunneling;
   if(!h245Tunneling)
   {
      OOTRACEINFO1("\tH245 Tunneling - disabled\n");
   }
   else{
      OOTRACEINFO1("\tH245 Tunneling - enabled\n");
   }
     
   gH323ep.termType = termType;
   OOTRACEINFO2("\tTerminal Type - %d\n", termType);
   gH323ep.t35CountryCode = t35CountryCode;
   OOTRACEINFO2("\tT35 CountryCode - %d\n", t35CountryCode);
   gH323ep.t35Extension = t35extension;
   OOTRACEINFO2("\tT35 Extension - %d\n", t35extension);
   gH323ep.manufacturerCode = manufacturerCode;
   OOTRACEINFO2("\tManufacturer Code - %d\n", manufacturerCode);
   if(productID)
   {
      gH323ep.productID = (char*)ASN1MALLOC(&gH323ep.ctxt, strlen(productID)+1);
      memset(gH323ep.productID, 0, strlen(productID)+1);
      strcpy(gH323ep.productID, productID);
   }
   else{
      gH323ep.productID = (char*) ASN1MALLOC(&gH323ep.ctxt, strlen("objsys")+1);
      memset(gH323ep.productID, 0, strlen("objsys")+1);
      strcpy(gH323ep.productID, "objsys");
   }
   OOTRACEINFO2("\tProductID - %s\n", gH323ep.productID);
  
   if(versionID)
   {
      gH323ep.versionID = (char*) ASN1MALLOC(&gH323ep.ctxt, strlen(versionID)+1);
      memset(gH323ep.versionID, 0, strlen(versionID)+1);
      strcpy(gH323ep.versionID, versionID);
   }
   else{
      gH323ep.versionID = (char*) ASN1MALLOC(&gH323ep.ctxt, strlen("0.4")+1);
      memset(gH323ep.versionID, 0, strlen("0.4")+1);
      strcpy(gH323ep.versionID, "0.4");
   }
   OOTRACEINFO2("\tVersionID - %s\n", gH323ep.versionID);

   gH323ep.callType = callType;
   ooGetLocalIPAddress(gH323ep.signallingIP);
   OOTRACEINFO2("\tLocal signalling IP address - %s\n", gH323ep.signallingIP);
   gH323ep.listenPort = listenport;
   OOTRACEINFO2("\tH225 ListenPort - %d\n", listenport);

   gH323ep.listener = NULL;
  
   if(callerid)
   {
      gH323ep.callerid = (char *) ASN1MALLOC(&gH323ep.ctxt, strlen(callerid)+1);
      memset(gH323ep.callerid, 0, strlen(callerid)+1);
      strcpy(gH323ep.callerid, callerid);
   }else{
      gH323ep.callerid = (char*) ASN1MALLOC(&gH323ep.ctxt, strlen("objsyscall")+1);
      memset(gH323ep.callerid, 0, strlen("objsyscall")+1);
      strcpy(gH323ep.callerid, "objsyscall");
   }
   OOTRACEINFO2("\tCallerID - %s\n", gH323ep.callerid);

   if(callername)
   {
      gH323ep.callername =(char*)ASN1MALLOC(&gH323ep.ctxt, strlen(callername)+1);
      memset(gH323ep.callername, 0, strlen(callername)+1);
      strcpy(gH323ep.callername, callername);
   }else {
      gH323ep.callername = (char*) ASN1MALLOC(&gH323ep.ctxt, strlen("objsys")+1);
      memset(gH323ep.callername, 0, strlen("objsys")+1);
      strcpy(gH323ep.callername, "objsys");
   }
   OOTRACEINFO2("\tCallerName - %s\n", gH323ep.callername);

   gH323ep.myCaps = NULL;
   gH323ep.noOfCaps = 0;
   gH323ep.callList = NULL;
   gH323ep.dtmfmode = 0;
    
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
   dListInit(&gCmdList);
   initContext(&gCtxt);
   gCallTokenBase = 1;
   gCallTokenMax = 1000;
   gCurCallToken = 1;
   gH323ep.autoAnswer = 1;
   OOTRACEINFO1("\tAutoAnswer - enabled\n");
  
   OOTRACEINFO1("H323 endpoint initialize - successful\n");
   return OO_OK;
}



int ooSetLocalCallSignallingAddress(char * localip, int listenport)
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

int ooSetAliasH323ID(char *h323id)
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
      OOTRACEERR1("Error: Failed to allocate memory for the new H323-ID alias value\n");
      ASN1MEMFREEPTR(&gH323ep.ctxt, psNewAlias);
      return OO_FAILED;
   }
   memset(psNewAlias->value, 0, strlen(h323id)+1);
   strcpy(psNewAlias->value, h323id);
   psNewAlias->next = gH323ep.aliases;
   gH323ep.aliases = psNewAlias;
   OOTRACEDBGA2("Added alias: H323ID - %s\n", h323id);
   return OO_OK;
}

int ooSetAliasDialedDigits(char * dialedDigits)
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
   memset(psNewAlias->value, 0, strlen(dialedDigits)+1);
   strcpy(psNewAlias->value, dialedDigits);
   psNewAlias->next = gH323ep.aliases;
   gH323ep.aliases = psNewAlias;
   OOTRACEDBGA2("Added alias: DialedDigits - %s\n", dialedDigits);
   return OO_OK;
}

int ooSetAliasURLID(char * url)
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
   memset(psNewAlias->value, 0, strlen(url)+1);
   strcpy(psNewAlias->value, url);
   psNewAlias->next = gH323ep.aliases;
   gH323ep.aliases = psNewAlias;
   OOTRACEDBGA2("Added alias: URL-ID - %s\n", url);
   return OO_OK;
}

int ooSetAliasEmailID(char * email)
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
      OOTRACEERR1("Error: Failed to allocate memory for the new Email-ID alias value\n");
      ASN1MEMFREEPTR(&gH323ep.ctxt, psNewAlias);
      return OO_FAILED;
   }
   memset(psNewAlias->value, 0, strlen(email)+1);
   strcpy(psNewAlias->value, email);
   psNewAlias->next = gH323ep.aliases;
   gH323ep.aliases = psNewAlias;
   OOTRACEDBGA2("Added alias: Email-ID - %s\n", email);
   return OO_OK;
}

int ooSetAliasTransportID(char * ipaddress)
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
      OOTRACEERR1("Error: Failed to allocate memory for the new Transport-ID alias value\n");
      ASN1MEMFREEPTR(&gH323ep.ctxt, psNewAlias);
      return OO_FAILED;
   }
   memset(psNewAlias->value, 0, strlen(ipaddress)+1);
   strcpy(psNewAlias->value, ipaddress);
   psNewAlias->next = gH323ep.aliases;
   gH323ep.aliases = psNewAlias;
   OOTRACEDBGA2("Added alias: Transport-ID - %s\n", ipaddress);
   return OO_OK;
}
   
int ooH323EpRegisterCallbacks(cb_OnAlerting onAlerting,
                              cb_OnIncomingCall onIncomingCall,
                              cb_OnOutgoingCallAdmitted onOutgoingCallAdmitted,
                              cb_OnOutgoingCall onOutgoingCall,
                              cb_OnCallEstablished onCallEstablished,
                              cb_OnCallCleared onCallCleared)

{
   gH323ep.onAlerting = onAlerting;
   gH323ep.onIncomingCall = onIncomingCall;
   gH323ep.onOutgoingCallAdmitted = onOutgoingCallAdmitted;
   gH323ep.onOutgoingCall = onOutgoingCall;
   gH323ep.onCallEstablished = onCallEstablished;
   gH323ep.onCallCleared = onCallCleared;

   return OO_OK;
}

int ooDestroyH323Ep(void)
{
   /* free any internal memory allocated
      close trace file free context structure
   */
   ooCallData * cur, *temp;
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
          
   }
   if(gH323ep.listener)
   {
      ooSocketClose(*(gH323ep.listener));
   }
  
   freeContext(&(gH323ep.ctxt));
   ooDestroyRas();  
#ifdef _WIN32
   DeleteCriticalSection(&gCmdMutex);
   DeleteCriticalSection(&gCallTokenMutex);
   DeleteCriticalSection(&gCallRefMutex);
#endif
   dListFreeAll(&gCtxt, &gCmdList);
   freeContext(&gCtxt);
   return OO_OK;
}

int ooEnableAutoAnswer(void)
{
   OOTRACEINFO1("AutoAnswer is enabled\n");
   gH323ep.autoAnswer = 1;
   return OO_OK;
}

int ooDisableAutoAnswer(void)
{
   OOTRACEINFO1("AutoAnswer is disabled\n");
   gH323ep.autoAnswer = 0;
   return OO_OK;
}

int ooSetFastStart(int value)
{
   gH323ep.fastStart = value;
   return OO_OK;
}

int ooSetH245Tunneling(int value)
{
   gH323ep.h245Tunneling = value;
   return OO_OK;
}

int ooSetTermType(int value)
{
   gH323ep.termType = value;
   return OO_OK;
}

int ooSetProductID(char * productID)
{
   gH323ep.productID = (char*) ASN1MALLOC(&gH323ep.ctxt, strlen(productID)+1);
   memset(gH323ep.productID, 0, strlen(productID)+1);
   strcpy(gH323ep.productID, productID);
   return OO_OK;
}

int ooSetVersionID(char * versionID)
{
   gH323ep.versionID = (char*) ASN1MALLOC(&gH323ep.ctxt, strlen(versionID)+1);
   memset(gH323ep.versionID, 0, strlen(versionID)+1);
   strcpy(gH323ep.versionID, versionID);
   return OO_OK;
}

int ooSetCallerID(char * callerID)
{
   gH323ep.callerid = (char*) ASN1MALLOC(&gH323ep.ctxt, strlen(callerID)+1);
   memset(gH323ep.callerid, 0, strlen(callerID)+1);
   strcpy(gH323ep.callerid, callerID);
   return OO_OK;
}

int ooSetCallerName(char * callerName)
{
   gH323ep.callername = (char*) ASN1MALLOC(&gH323ep.ctxt, strlen(callerName)+1);
   memset(gH323ep.callername, 0, strlen(callerName)+1);
   strcpy(gH323ep.callername, callerName);
   return OO_OK;
}




