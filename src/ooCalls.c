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


#include "oo.h"
#include "ootypes.h"
#include "ooCalls.h"
#include "ooports.h"

ooCallData* ooCreateCall(char * type, char*callToken)
{
   ooCallData *call=NULL;
   OOCTXT *pctxt=NULL;
  
   int port;
   char localip[20];
   pctxt = newContext();
   if(!pctxt)
   {
      OOTRACEERR1("ERROR:Failed to create OOCTXT for new call\n");
      return NULL;
   }
   call = (ooCallData*)ASN1MALLOC(pctxt, sizeof(ooCallData));
   if(!call)
   {
      OOTRACEERR1("ERROR: Failed to allocate memory for new call data\n");
      return NULL;
   }
   memset(call, 0, sizeof(ooCallData));
   call->pctxt = pctxt;
  
   sprintf(call->callToken, "%s", callToken);
  
   sprintf(call->callType, "%s", type);
   /* TODO: Need better way to define which channels are to be created */
   if(gH323ep.txAudioChan)
      call->audioChannel = 1; /* audio channel always created */
   else
      call->audioChannel = 0;
   port = ooGetNextPort(&gH323ep, OORTP);
   /* Ensures that RTP port is an even one */
   if((port & 1) == 1)
     port = ooGetNextPort(&gH323ep, OORTP);
   ooSetLocalRtpPort(call, port);
   ooSetLocalRtcpPort(call, ooGetNextPort(&gH323ep, OORTP));
   call->masterSlaveState = OO_MasterSlave_Idle;
   call->capabilityExchangeState = OO_TermCapExchagne_Idle;
   ooGetLocalIPAddress(localip);
   strcpy(call->localIP, localip);
   call->noOfLogicalChannels = 0;
   call->logicalChanNoBase = 1001;
   call->logicalChanNoMax = 1100;
   call->logicalChanNoCur = 1001;
   OOTRACEINFO3("Created a new call (%s, %s)\n", call->callType,
                 call->callToken);
   /* Add new call to calllist */
   ooAddCallToList(&gH323ep, call);
   return call;
}

int ooAddCallToList(ooEndPoint * h323ep, ooCallData *call)
{
   if(!h323ep->callList)
   {
      h323ep->callList = call;
      call->next = NULL;
      call->prev = NULL;
   }
   else{
      call->next = h323ep->callList;
      call->prev = NULL;
      h323ep->callList->prev = call;
      h323ep->callList = call;
   }
   return OO_OK;
}

int ooRemoveCallFromList(ooEndPoint * h323ep, ooCallData *call)
{
   if(!call)
      return OO_OK;

   if(call == h323ep->callList)
   {
      if(!call->next)
         h323ep->callList = NULL;
      else{
         call->next->prev = NULL;
         h323ep->callList = call->next;
      }
   }
   else{
      call->prev->next = call->next;
      if(call->next)
         call->next->prev = call->prev;
   }
   return OO_OK;
}
/*TODO: Stop logical channels if active. Refer to closeH245Session*/
int ooClearCall(ooCallData *call, int reason)
{
   OOCTXT *pctxt;
   int msg = reason - 20;
   OOTRACEINFO1("Clearing call\n");  
   if(msg >= 0 && msg < OO_TotalMessages)
      OOTRACEWARN4("Clearing Call (%s, %s)- reason:%s\n", messages[msg],
                   call->callType, call->callToken);
   else
      OOTRACEWARN3("Clearing Call (%s, %s)- reason:unknown\n",
                   call->callType, call->callToken);

   if(call->h245listener)
   {
      ooSocketClose(*(call->h245listener));
      ASN1MEMFREEPTR(call->pctxt, call->h245listener);
      call->h245listener = NULL;
   }
   if(call->h245Channel)
   {
      ooSocketClose(*(call->h245Channel));
      ASN1MEMFREEPTR(call->pctxt, call->h245Channel);
      call->h245Channel = NULL;
   }
   if(call->h225Channel)
   {
      ooSocketClose(*(call->h225Channel));
      ASN1MEMFREEPTR(call->pctxt, call->h225Channel);
      call->h225Channel = NULL;
   }
   ooRemoveCallFromList(&gH323ep, call);
   OOTRACEINFO3("Removed call (%s, %s) from list\n", call->callType,
                 call->callToken);
   dListFreeAll(call->pctxt, &(call->outH245Queue));
   call->sendH245 = 0;
   dListFreeAll(call->pctxt, &(call->outH225Queue));
   call->sendH225 = 0;
   gH323ep.onCallCleared(call);
   pctxt = call->pctxt;
   freeContext(pctxt);
   ASN1CRTFREE0(pctxt);
   return OO_OK;
}


ooCallData* ooFindCallByToken(char *callToken)
{
   ooCallData *call;
   if(!callToken)
   {
      OOTRACEERR1("ERROR:Invalid call token passed - ooFindCallByToken\n");
      return NULL;
   }
   if(!gH323ep.callList)
   {
      OOTRACEERR1("ERROR: Empty calllist - ooFindCallByToken failed\n");
      return NULL;
   }
   call = gH323ep.callList;
   while(call)
   {
      if(!strcmp(call->callToken, callToken))
         break;
          else
         call = call->next;
   }
  
   if(!call)
   {
      OOTRACEERR2("ERROR:Call with token %s not found\n", callToken);
      return NULL;
   }
   return call;
}

int ooSetLocalRtpPort(ooCallData *call, int port)
{
   call->localRtpPort = port;
   return OO_OK;
}

int ooSetLocalRtcpPort(ooCallData *call, int port)
{
   call->localRtcpPort = port;
   return OO_OK;
}

int ooGetLocalRtpPort(ooCallData*call)
{
   return call->localRtpPort;
}

int ooGetLocalRtcpPort(ooCallData*call)
{
   return call->localRtcpPort;
}

int ooAddNewLogicalChannel(ooCallData *call, int channelNo, int sessionID, char *type,
                           char * dir, ooH323EpCapability *epCap)
{
   H245AudioCapability *audioCap;
   ooLogicalChannel *pLogicalChannel=NULL, *temp=NULL;
   /* Create a new logical channel entry */
   pLogicalChannel = (ooLogicalChannel*)ASN1MALLOC(call->pctxt, sizeof(ooLogicalChannel));
   if(!pLogicalChannel)
   {
      OOTRACEERR3("ERROR:Memory allocation for new logical channel failed "
                  "(%s, %s)\n", call->callType, call->callToken);
      return OO_FAILED;
   }
   memset(pLogicalChannel, 0, sizeof(ooLogicalChannel));
   pLogicalChannel->channelNo = channelNo;
   pLogicalChannel->sessionID = sessionID;
   strcpy(pLogicalChannel->type, type);
   strcpy(pLogicalChannel->dir, dir);
   /* Create h323ep capability */
   pLogicalChannel->chanCap = (ooH323EpCapability*)ASN1MALLOC(call->pctxt, sizeof(ooH323EpCapability));
   if(!pLogicalChannel->chanCap)
   {
      OOTRACEERR3("ERROR:Memory allocation for new logical channels chanCap "
                  "failed (%s, %s)\n", call->callType, call->callToken);
      ASN1MEMFREEPTR(call->pctxt, pLogicalChannel);
      return OO_FAILED;
   }
   memset(pLogicalChannel->chanCap, 0, sizeof(ooH323EpCapability));
   pLogicalChannel->chanCap->t = epCap->t;
   pLogicalChannel->chanCap->startReceiveChannel = epCap->startReceiveChannel;
   pLogicalChannel->chanCap->startTransmitChannel = epCap->startTransmitChannel;
   pLogicalChannel->chanCap->stopReceiveChannel = epCap->stopReceiveChannel;
   pLogicalChannel->chanCap->stopTransmitChannel = epCap->stopTransmitChannel;
   /* If audio Capability */
   if(epCap->t == T_H245Capability_receiveAudioCapability ||
      epCap->t == T_H245Capability_transmitAudioCapability ||
      epCap->t == T_H245Capability_receiveAndTransmitAudioCapability)
   {
      /* Allocate mem for audio cap */
      pLogicalChannel->chanCap->cap = (void*)ASN1MALLOC(call->pctxt, sizeof(H245AudioCapability));
      if(!pLogicalChannel->chanCap->cap)
      {
         OOTRACEERR3("ERROR:Memory allocation for new logical channels audio "
                     "capability failed (%s, %s)n", call->callType,
                     call->callToken);
         ASN1MEMFREEPTR(call->pctxt, pLogicalChannel->chanCap);
         ASN1MEMFREEPTR(call->pctxt, pLogicalChannel);
         return OO_FAILED;
      }
      memset(pLogicalChannel->chanCap->cap, 0, sizeof(H245AudioCapability));
      audioCap = (H245AudioCapability*)epCap->cap;
      /* For handling these capabilities we will have to add ASN1MALLOCS as they are dynamic*/
      if(audioCap->t == T_H245AudioCapability_nonStandard ||
          audioCap->t == T_H245AudioCapability_g7231 ||
          audioCap->t == T_H245AudioCapability_is11172AudioCapability ||
          audioCap->t == T_H245AudioCapability_is13818AudioCapability ||
          audioCap->t == T_H245AudioCapability_g7231AnnexCCapability ||
          audioCap->t == T_H245AudioCapability_gsmFullRate ||
          audioCap->t == T_H245AudioCapability_gsmHalfRate ||
          audioCap->t == T_H245AudioCapability_gsmEnhancedFullRate ||
          audioCap->t == T_H245AudioCapability_genericAudioCapability ||
          audioCap->t == T_H245AudioCapability_g729Extensions ||
          audioCap->t == T_H245AudioCapability_vbd ||
          audioCap->t == T_H245AudioCapability_audioTelephonyEvent ||
          audioCap->t == T_H245AudioCapability_audioTone ||
          audioCap->t == T_H245AudioCapability_extElem1)
      {
         ASN1MEMFREEPTR(call->pctxt, pLogicalChannel->chanCap->cap);
         ASN1MEMFREEPTR(call->pctxt, pLogicalChannel->chanCap);
         ASN1MEMFREEPTR(call->pctxt, pLogicalChannel);
         OOTRACEERR3("ERROR:AddNewLogicalChannel - Unhandled capability type "
                     "(%s, %s)\n", call->callType, call->callToken);
         return OO_FAILED;
      }
      /* Copy audio Capability */
      memcpy((void*)pLogicalChannel->chanCap->cap, (void*)epCap->cap, sizeof(H245AudioCapability));
   }
   else
   {
      /* Only audio capabilities supported at this point */
      ASN1MEMFREEPTR(call->pctxt, pLogicalChannel->chanCap);
      ASN1MEMFREEPTR(call->pctxt, pLogicalChannel);
      OOTRACEERR3("ERROR: AddNewLogicalChannel - Unhandled Capability type "
                  "(%s, %s)\n", call->callType, call->callToken);
      return OO_FAILED;
   }
  
   pLogicalChannel->next = NULL;
   if(!call->logicalChans)
      call->logicalChans = pLogicalChannel;
   else{
      temp = call->logicalChans;
      while(temp->next)   temp = temp->next;
      temp->next = pLogicalChannel;
   }
   /* increment logical channels */
   call->noOfLogicalChannels++;
   OOTRACEINFO3("Created new logical channel entry (%s, %s)\n", call->callType,
                call->callToken);
   return OO_OK;
}

ooLogicalChannel* ooFindLogicalChannelByLogicalChannelNo(ooCallData *call,int ChannelNo)
{
   ooLogicalChannel *pLogicalChannel=NULL;
   if(!call->logicalChans)
   {
      OOTRACEERR3("ERROR: No Open LogicalChannels - Failed "
                  "FindLogicalChannelByChannelNo (%s, %s\n", call->callType,
                   call->callToken);
      return NULL;
   }
   pLogicalChannel = call->logicalChans;
   while(pLogicalChannel)
   {
      if(pLogicalChannel->channelNo == ChannelNo)
         break;
      else
         pLogicalChannel = pLogicalChannel->next;
   }

   return pLogicalChannel;
}

int ooRemoveLogicalChannel(ooCallData *call, int ChannelNo)
{
   ooLogicalChannel * temp = NULL, *prev=NULL;
   if(!call->logicalChans)
   {
      OOTRACEERR4("ERROR:Remove Logical Channel - Channel %d not found "
                  "Empty channel List(%s, %s)\n", ChannelNo, call->callType,
                  call->callToken);
      return OO_FAILED;
   }

   temp = call->logicalChans;
   while(temp)
   {
      if(temp->channelNo == ChannelNo)
      {
         if(!prev)   call->logicalChans = temp->next;
         else   prev->next = temp->next;
         ASN1MEMFREEPTR(call->pctxt, temp->chanCap->cap);
         ASN1MEMFREEPTR(call->pctxt, temp->chanCap);
         ASN1MEMFREEPTR(call->pctxt, temp);
         OOTRACEDBGC4("Removed logical channel %d (%s, %s)\n", ChannelNo,
                       call->callType, call->callToken);
         call->noOfLogicalChannels--;
         return OO_OK;
      }
      prev = temp;
      temp = temp->next;
   }
  
   OOTRACEERR4("ERROR:Remove Logical Channel - Channel %d not found "
                  "(%s, %s)\n", ChannelNo, call->callType, call->callToken);
   return OO_FAILED;
}

        
