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
#include "ooh323ep.h"
#include "oo.h"
#include "ooCalls.h"


/* Initialize the application context within stack */
int ooInitializeH323Ep( const char * tracefile, int h245Tunneling,
                        int fastStart, int termType, int t35CountryCode,
                        int t35extension, int manufacturerCode,
                        char *productID, char *versionID, int callType,
                        int listenport, char *callerid, char *callername,
                        int txAudioChan)
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
   gH323ep.tcpPorts.start = TCPPORTSSTART;
   gH323ep.tcpPorts.max = TCPPORTSEND;
   gH323ep.tcpPorts.current=TCPPORTSSTART;

   gH323ep.udpPorts.start = UDPPORTSSTART;
   gH323ep.udpPorts.max = UDPPORTSEND;
   gH323ep.udpPorts.current = UDPPORTSSTART;

   gH323ep.rtpPorts.start = RTPPORTSSTART;
   gH323ep.rtpPorts.max = RTPPORTSEND;
   gH323ep.rtpPorts.current = RTPPORTSSTART;
  
   gH323ep.fastStart = fastStart;
   gH323ep.h245Tunneling = h245Tunneling;
   gH323ep.termType = termType;
   gH323ep.t35CountryCode = t35CountryCode;
   gH323ep.t35Extension = t35extension;
   gH323ep.manufacturerCode = manufacturerCode;
   if(productID)
   {
      gH323ep.productID = (char*)ASN1MALLOC(&gH323ep.ctxt, strlen(productID));
      memcpy(gH323ep.productID, productID, strlen(productID));
   }
   if(versionID)
   {
      gH323ep.versionID = (char*) ASN1MALLOC(&gH323ep.ctxt, strlen(versionID));
      memcpy(gH323ep.versionID, versionID, strlen(versionID));
   }
   gH323ep.callType = callType;
   gH323ep.listenPort = listenport;
   gH323ep.listener = NULL;
   if(callerid)
   {
      gH323ep.callerid = (char *) ASN1MALLOC(&gH323ep.ctxt, strlen(callerid));
      memcpy(gH323ep.callerid, callerid, strlen(callerid));
   }
   if(callername)
   {
      gH323ep.callername =(char*)ASN1MALLOC(&gH323ep.ctxt, strlen(callername));
      memcpy(gH323ep.callername, callername, strlen(callername));
   }

   dListInit(&gH323ep.audioCaps);
   dListInit(&gH323ep.dataApplicationCaps);
   dListInit(&gH323ep.videoCaps);
   gH323ep.callList = NULL;
  
   gH323ep.txAudioChan = txAudioChan;
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

int ooDestroyH323Ep()
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
         ooClearCall(temp, OO_HOST_CLEARED);
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

/*
   Function to add audio capabilities to the endpoint. 'dir' indicates
   whether we have a transmit capability or a receive capability or both.
   Last four parameters are the callback functions for starting the channels.
*/
int ooAddAudioCapability(H245AudioCapability audioCap, int dir,
                             cb_StartReceiveChannel startReceiveChannel,
                             cb_StartTransmitChannel startTransmitChannel,
                             cb_StopReceiveChannel stopReceiveChannel,
                             cb_StopTransmitChannel stopTransmitChannel)
{
   int ret = OO_OK;
   ooH323EpCapability *epCap;
   H245AudioCapability *pAudioCap;

   epCap = (ooH323EpCapability*)ASN1MALLOC(&gH323ep.ctxt,
                                            sizeof(ooH323EpCapability));
   memset(epCap, 0, sizeof(ooH323EpCapability));

   if(dir == OO_RX_CAP)
      epCap->t = T_H245Capability_receiveAudioCapability;
   else if(dir == OO_TX_CAP)
      epCap->t = T_H245Capability_transmitAudioCapability;
   else
      epCap->t = T_H245Capability_receiveAndTransmitAudioCapability;


   pAudioCap = (H245AudioCapability*) ASN1MALLOC(&gH323ep.ctxt,
                                                 sizeof(H245AudioCapability));

   memset(pAudioCap, 0, sizeof(H245AudioCapability));  
   epCap->cap = (void*)pAudioCap;
   epCap->startReceiveChannel = startReceiveChannel;
   epCap->startTransmitChannel = startTransmitChannel;
   epCap->stopReceiveChannel = stopReceiveChannel;
   epCap->stopTransmitChannel = stopTransmitChannel;

   switch(audioCap.t)
   {
   case T_H245AudioCapability_g711Ulaw64k:
      pAudioCap->t = T_H245AudioCapability_g711Ulaw64k;
      pAudioCap->u.g711Ulaw64k = audioCap.u.g711Ulaw64k;
      break;
   case T_H245AudioCapability_g711Ulaw56k:
      pAudioCap->t = T_H245AudioCapability_g711Ulaw56k;
      pAudioCap->u.g711Ulaw56k = audioCap.u.g711Ulaw56k;
      break;
   case T_H245AudioCapability_g711Alaw64k:
      pAudioCap->t = T_H245AudioCapability_g711Alaw64k;
      pAudioCap->u.g711Alaw64k = audioCap.u.g711Alaw64k;
      break;
   case T_H245AudioCapability_g711Alaw56k:
      pAudioCap->t = T_H245AudioCapability_g711Alaw56k;
      pAudioCap->u.g711Alaw64k = audioCap.u.g711Alaw56k;
      break;
   case T_H245AudioCapability_g729:
      pAudioCap->t = T_H245AudioCapability_g729;
      pAudioCap->u.g729 = audioCap.u.g729;
      break;
   case T_H245AudioCapability_g729AnnexA:
      pAudioCap->t = T_H245AudioCapability_g729AnnexA;
      pAudioCap->u.g729AnnexA = audioCap.u.g729AnnexA;
      break;
   default:
      OOTRACEERR1("ERROR:Capability not supported yet\n");
      return OO_FAILED;
   }

   dListAppend(&(gH323ep.ctxt), &(gH323ep.audioCaps), epCap);
   return OO_OK;
}

int ooAddH323DataApplicationCapaility(int capType)
{
   return OO_OK;
}

int ooAddH323VideoCapability(int capType)
{
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

/*TODO:This will be used to add capabilities, number will be numeric unique
       capability identifier, type will be type of capability and dir will
       specify whether receive, transmit or reveiceTransmit capability
 */
int ooSetCapability(int number, int type, int dir)
{
   return OO_OK;
}

/*TODO: Aletrnate capability sets. Basically Set of capabilities which can be
        used simultaneously. 'set' is array of capability identifiers and 'size'
        is the size of the array.
*/
int ooSetAletrnateCapabilitySet(int *set, int size)
{
   return OO_OK;
}

int ooCopyAudioCapability(H245AudioCapability *src, H245AudioCapability *dest)
{
   switch(src->t)
   {
   case T_H245AudioCapability_nonStandard:
      OOTRACEWARN1("Warn: Capability type not supported\n");
      break;
   case T_H245AudioCapability_g711Alaw64k:
      OOTRACEWARN1("Warn: Capability type not supported\n");
      break;
   case T_H245AudioCapability_g711Alaw56k:
      OOTRACEWARN1("Warn: Capability type not supported\n");
      break;
   case T_H245AudioCapability_g711Ulaw64k:
      dest->t = src->t;
      dest->u.g711Ulaw64k = src->u.g711Ulaw64k;
      break;
   case T_H245AudioCapability_g711Ulaw56k:
      OOTRACEWARN1("Warn: Capability type not supported\n");
      break;
   case T_H245AudioCapability_g722_64k:
      OOTRACEWARN1("Warn: Capability type not supported\n");
      break;
   case T_H245AudioCapability_g722_56k:
      OOTRACEWARN1("Warn: Capability type not supported\n");
      break;
   case T_H245AudioCapability_g722_48k:
      OOTRACEWARN1("Warn: Capability type not supported\n");
      break;
   case T_H245AudioCapability_g7231:
      OOTRACEWARN1("Warn: Capability type not supported\n");
      break;
   case T_H245AudioCapability_g728:
      OOTRACEWARN1("Warn: Capability type not supported\n");
      break;
   case T_H245AudioCapability_g729:
      OOTRACEWARN1("Warn: Capability type not supported\n");
      break;
   case T_H245AudioCapability_g729AnnexA:
      OOTRACEWARN1("Warn: Capability type not supported\n");
      break;
   case T_H245AudioCapability_is11172AudioCapability:
      OOTRACEWARN1("Warn: Capability type not supported\n");
      break;
   case T_H245AudioCapability_is13818AudioCapability:
      OOTRACEWARN1("Warn: Capability type not supported\n");
      break;
   case T_H245AudioCapability_g729wAnnexB:
      OOTRACEWARN1("Warn: Capability type not supported\n");
      break;
   case T_H245AudioCapability_g729AnnexAwAnnexB:
      OOTRACEWARN1("Warn: Capability type not supported\n");
      break;
   case T_H245AudioCapability_g7231AnnexCCapability:
      OOTRACEWARN1("Warn: Capability type not supported\n");
      break;
   case T_H245AudioCapability_gsmFullRate:
      OOTRACEWARN1("Warn: Capability type not supported\n");
      break;
   case T_H245AudioCapability_gsmHalfRate:
      OOTRACEWARN1("Warn: Capability type not supported\n");
      break;
   case T_H245AudioCapability_gsmEnhancedFullRate:
      OOTRACEWARN1("Warn: Capability type not supported\n");
      break;
   case T_H245AudioCapability_genericAudioCapability:
      OOTRACEWARN1("Warn: Capability type not supported\n");
      break;
   case T_H245AudioCapability_g729Extensions:
      OOTRACEWARN1("Warn: Capability type not supported\n");
      break;
   case T_H245AudioCapability_vbd:
      OOTRACEWARN1("Warn: Capability type not supported\n");
      break;
   case T_H245AudioCapability_audioTelephonyEvent:
      OOTRACEWARN1("Warn: Capability type not supported\n");
      break;
   case T_H245AudioCapability_audioTone:
      OOTRACEWARN1("Warn: Capability type not supported\n");
      break;
   case T_H245AudioCapability_extElem1:
      OOTRACEWARN1("Warn: Capability type not supported\n");
      break;
   default:
      OOTRACEERR1("ERROR:Unknown audio capability type\n");
   }
   return OO_OK;
}

ooH323EpCapability* ooIsAudioCapabilitySupported(int capType, int dir)
{
   int i=0;
   ooH323EpCapability *epCap=NULL;
   H245AudioCapability *audioCapLocal;
   DListNode *curNode = NULL;
   ASN1BOOL found = 0;
   for(i=0; i< (int)gH323ep.audioCaps.count; i++)
   {
      /* Retrieve local capability */
      curNode = dListFindByIndex (&gH323ep.audioCaps, i) ;
      epCap = (ooH323EpCapability*)curNode->data;
      audioCapLocal = (H245AudioCapability*) epCap->cap;
     
          if(audioCapLocal->t == capType)
      {
         if(dir == OO_RX_CAP)
         {
            if(epCap->t == T_H245Capability_receiveAudioCapability ||
               epCap->t == T_H245Capability_receiveAndTransmitAudioCapability)
            {
               found = 1;
               break;
                        }
         }
         else if(dir == OO_TX_CAP)
         {
            if(epCap->t == T_H245Capability_transmitAudioCapability ||
               epCap->t == T_H245Capability_receiveAndTransmitAudioCapability)
            {
               found = 1;
               break;
                        }
         }
         else if(dir == OO_RXTX_CAP)
         {
            if(epCap->t == T_H245Capability_receiveAndTransmitAudioCapability)
            {
               found = 1;
               break;
            }
         }
      }/* EndOf if(audioCapLocal->t == audio->t)*/
   }/* EndOf For loop */
   if(found) return epCap;
   else return NULL;
}

