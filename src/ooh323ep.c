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


/* Initialize the application context within stack */
int ooInitializeH323Ep( const char * tracefile, int h245Tunneling,
                        int fastStart, int termType, int t35CountryCode,
                        int t35extension, int manufacturerCode,
                        char *productID, char *versionID, int callType,
                        int listenport, char *callerid, char *callername,
                        int callMode)
{
  
   initContext(&(gH323ep.ctxt));
  
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
      gH323ep.productID = (char*)ASN1MALLOC(&gH323ep.ctxt, strlen(productID));
      memcpy(gH323ep.productID, productID, strlen(productID));
   }
   else{
      gH323ep.productID = (char*) ASN1MALLOC(&gH323ep.ctxt, strlen("objsys"));
      memcpy(gH323ep.productID, "objsys", strlen("objsys"));
   }
   OOTRACEINFO2("\tProductID - %s\n", gH323ep.productID);
  
   if(versionID)
   {
      gH323ep.versionID = (char*) ASN1MALLOC(&gH323ep.ctxt, strlen(versionID));
      memcpy(gH323ep.versionID, versionID, strlen(versionID));
   }
   else{
      gH323ep.versionID = (char*) ASN1MALLOC(&gH323ep.ctxt, strlen("0.4"));
      memcpy(gH323ep.versionID, "0.4", strlen("0.4"));
   }
   OOTRACEINFO2("\tVersionID - %s\n", gH323ep.versionID);

   gH323ep.callType = callType;
   gH323ep.listenPort = listenport;
   OOTRACEINFO2("\tH225 ListenPort - %d\n", listenport);

   gH323ep.listener = NULL;
  
   if(callerid)
   {
      gH323ep.callerid = (char *) ASN1MALLOC(&gH323ep.ctxt, strlen(callerid));
      memcpy(gH323ep.callerid, callerid, strlen(callerid));
   }else{
      gH323ep.callerid = (char*) ASN1MALLOC(&gH323ep.ctxt, strlen("objsyscall"));
      memcpy(gH323ep.callerid, "objsyscall", strlen("objsyscall"));
   }
   OOTRACEINFO2("\tCallerID - %s\n", gH323ep.callerid);

   if(callername)
   {
      gH323ep.callername =(char*)ASN1MALLOC(&gH323ep.ctxt, strlen(callername));
      memcpy(gH323ep.callername, callername, strlen(callername));
   }else {
      gH323ep.callername = (char*) ASN1MALLOC(&gH323ep.ctxt, strlen("objsys"));
      memcpy(gH323ep.callername, "objsys", strlen("objsys"));
   }
   OOTRACEINFO2("\tCallerName - %s\n", gH323ep.callername);

   dListInit(&gH323ep.audioCaps);
   dListInit(&gH323ep.dataApplicationCaps);
   dListInit(&gH323ep.videoCaps);
   gH323ep.callList = NULL;
  
   gH323ep.callMode = callMode;
#ifdef _WIN32
   InitializeCriticalSection(&gCmdMutex);
#else
   pthread_mutex_init(&gCmdMutex, 0);
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

int ooH323EpRegisterCallbacks(cb_OnIncomingCall onIncomingCall,
                              cb_OnOutgoingCall onOutgoingCall,
                              cb_OnCallEstablished onCallEstablished,
                              cb_OnCallCleared onCallCleared,
                              cb_OnStartLogicalChannel onStartLogicalChannel)
{
   gH323ep.onIncomingCall = onIncomingCall;
   gH323ep.onOutgoingCall = onOutgoingCall;
   gH323ep.onCallEstablished = onCallEstablished;
   gH323ep.onCallCleared = onCallCleared;
   gH323ep.onStartLogicalChannel = onStartLogicalChannel;
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
  
#ifdef _WIN32
   DeleteCriticalSection(&gCmdMutex);
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
   gH323ep.productID = (char*) ASN1MALLOC(&gH323ep.ctxt, strlen(productID));
   memcpy(gH323ep.productID, productID, strlen(productID));
   return OO_OK;
}

int ooSetVersionID(char * versionID)
{
   gH323ep.versionID = (char*) ASN1MALLOC(&gH323ep.ctxt, strlen(versionID));
   memcpy(gH323ep.versionID, versionID, strlen(versionID));
   return OO_OK;
}

int ooSetCallerID(char * callerID)
{
   gH323ep.callerid = (char*) ASN1MALLOC(&gH323ep.ctxt, strlen(callerID));
   memcpy(gH323ep.callerid, callerID, strlen(callerID));
   return OO_OK;
}

int ooSetCallerName(char * callerName)
{
   gH323ep.callername = (char*) ASN1MALLOC(&gH323ep.ctxt, strlen(callerName));
   memcpy(gH323ep.callername, callerName, strlen(callerName));
   return OO_OK;
}




