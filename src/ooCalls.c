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


#include "oo.h"
#include "ootypes.h"
#include "ooCalls.h"
#include "ooports.h"
#include "oochannels.h"
#include "ooh245.h"

ooCallData* ooCreateCall(char * type, char*callToken)
{
   ooCallData *call=NULL;
   OOCTXT *pctxt=NULL;
/*   char localip[20];*/

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
  
   call->remoteAliases = NULL;
   call->localAliases = NULL;
   call->masterSlaveState = OO_MasterSlave_Idle;
   call->localTermCapState = OO_LocalTermCapExchange_Idle;
   call->remoteTermCapState = OO_RemoteTermCapExchange_Idle;
/*   ooGetLocalIPAddress(localip);
   strcpy(call->localIP, localip);*/
   strcpy(call->localIP, gH323ep.signallingIP);
   call->noOfLogicalChannels = 0;
   call->logicalChanNoBase = 1001;
   call->logicalChanNoMax = 1100;
   call->logicalChanNoCur = 1001;
   call->isTunnelingActive = gH323ep.h245Tunneling;
   call->mediaInfo = NULL;
   call->isFastStartActive = gH323ep.fastStart;
   dListInit(&call->remoteFastStartOLCs);
   call->callState = OO_CALL_CREATED;
   call->h245SessionState = OO_H245SESSION_INACTIVE;
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

int ooEndCall(ooCallData *call)
{
   if(call->logicalChans)
   {
      if(call->callState == OO_CALL_CLEAR)
      {
         OOTRACEINFO3("Call Clearing - CloseAllLogicalChannels. (%s, %s)\n",
                       call->callType, call->callToken);
         ooCloseAllLogicalChannels(call);
         call->callState = OO_CALL_CLEAR_CLOLCS;
         return OO_OK;
      }
      if(call->callState >= OO_CALL_CLEAR_CLOLCS)
      {
         /* Wait till all the queued H245 messages are sent */
         if(call->sendH245 > 0)
            return OO_OK;
         OOTRACEINFO3("Call Clearing - ClearAllLogicalChannels. (%s, %s)\n",
                       call->callType, call->callToken);
         ooClearAllLogicalChannels(call);
         if(call->callState < OO_CALL_CLEAR_CLELCS)
            call->callState = OO_CALL_CLEAR_CLELCS;
       }
   }
   else{
      if(call->callState < OO_CALL_CLEAR_CLELCS)
         call->callState = OO_CALL_CLEAR_CLELCS;
   }
  
   if(call->h245SessionState == OO_H245SESSION_ACTIVE)
   {
      if(call->callState == OO_CALL_CLEAR_CLELCS)
      {
         OOTRACEINFO3("Call Clearing - sendEndSessionCommand. (%s, %s)\n",
                       call->callType, call->callToken);
         ooSendEndSessionCommand(call);
         call->callState = OO_CALL_CLEAR_ENDSESSION;
         return OO_OK;
      }
      if(call->callState >= OO_CALL_CLEAR_ENDSESSION)
      {
         /* Wait till all the queued H245 messages are sent */
         if(call->sendH245 > 0)
            return OO_OK;
         OOTRACEINFO3("Call Clearing - CloseH245Connection. (%s, %s)\n",
                       call->callType, call->callToken);
         ooCloseH245Connection(call);
        
         if(call->callState < OO_CALL_CLEAR_CLOSEH245)
            call->callState = OO_CALL_CLEAR_CLOSEH245;
      }
   }
   else{
      if(call->callState < OO_CALL_CLEAR_CLOSEH245)
         call->callState = OO_CALL_CLEAR_CLOSEH245;
   }
   if(call->callState == OO_CALL_CLEAR_CLOSEH245)
   {
      if(call->h225Channel)
      {
         OOTRACEINFO3("Call Clearing - sendReleaseComplete. (%s, %s)\n",
                       call->callType, call->callToken);
         ooSendReleaseComplete(call);
         call->callState = OO_CALL_CLEAR_RELEASE;
         return OO_OK;
      }
      else
         call->callState = OO_CALL_CLEAR_RELEASE;
   }
   if(call->callState = OO_CALL_CLEAR_RELEASE)
   {
      /* Wait till all the queued H.2250 messages are sent */
      if(call->sendH225 > 0)
         return OO_OK;
      call->callState = OO_CALL_CLEARED;
     
   }
   if(call->callState == OO_CALL_CLEARED)
   {
      ooCleanCall(call);
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

int ooCleanCall(ooCallData *call)
{
   OOCTXT *pctxt;
   int msg = call->callEndReason - 20;

   if(msg >= 0 && msg < OO_TotalMessages)
      OOTRACEWARN4("Cleaning Call (%s, %s)- reason:%s\n",
                   call->callType, call->callToken, messages[msg]);
   else
      OOTRACEWARN3("Cleaning Call (%s, %s)- reason:unknown\n",
                   call->callType, call->callToken);

   /* First close all the logical channels. */
   if(call->logicalChans)
      ooClearAllLogicalChannels(call);
  
   /* Close H.245 connection */
   if(call->h245SessionState != OO_H245SESSION_INACTIVE)
      ooCloseH245Connection(call);
   else{
      if(call->outH245Queue.count > 0)
      {
         dListFreeAll(call->pctxt, &(call->outH245Queue));
         call->sendH245 = 0;
      }
   }

   if(call->h225Channel)
   {
      ooCloseH225Connection(call);
   }
   ooRemoveCallFromList(&gH323ep, call);
   OOTRACEINFO3("Removed call (%s, %s) from list\n", call->callType,
                 call->callToken);
 
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



ooLogicalChannel* ooAddNewLogicalChannel(ooCallData *call, int channelNo, int sessionID,
                                         char *type, char * dir, ooH323EpCapability *epCap)
{
   H245AudioCapability *audioCap;
   ooLogicalChannel *pNewChannel=NULL, *pChannel=NULL;
   ooMediaInfo *pMediaInfo = NULL;

   /* Create a new logical channel entry */
   pNewChannel = (ooLogicalChannel*)ASN1MALLOC(call->pctxt, sizeof(ooLogicalChannel));
   if(!pNewChannel)
   {
      OOTRACEERR3("ERROR:Memory allocation for new logical channel failed "
                  "(%s, %s)\n", call->callType, call->callToken);
      return NULL;
   }
  
   memset(pNewChannel, 0, sizeof(ooLogicalChannel));
   pNewChannel->channelNo = channelNo;
   pNewChannel->sessionID = sessionID;
   pNewChannel->state = OO_LOGICALCHAN_IDLE;
   strcpy(pNewChannel->type, type);
   strcpy(pNewChannel->dir, dir);
   /* Create h323ep capability */
   pNewChannel->chanCap = (ooH323EpCapability*)ASN1MALLOC(call->pctxt,
                                                           sizeof(ooH323EpCapability));
   if(!pNewChannel->chanCap)
   {
      OOTRACEERR3("ERROR:Memory allocation for new logical channels chanCap "
                  "failed (%s, %s)\n", call->callType, call->callToken);
      ASN1MEMFREEPTR(call->pctxt, pNewChannel);
      return NULL;
   }
  
   memset(pNewChannel->chanCap, 0, sizeof(ooH323EpCapability));
  
   pNewChannel->chanCap->t = epCap->t;
  
   pNewChannel->chanCap->startReceiveChannel = epCap->startReceiveChannel;
   pNewChannel->chanCap->startTransmitChannel = epCap->startTransmitChannel;
   pNewChannel->chanCap->stopReceiveChannel = epCap->stopReceiveChannel;
   pNewChannel->chanCap->stopTransmitChannel = epCap->stopTransmitChannel;
  
   /* If audio Capability */
   if(epCap->t == T_H245Capability_receiveAudioCapability ||
      epCap->t == T_H245Capability_transmitAudioCapability ||
      epCap->t == T_H245Capability_receiveAndTransmitAudioCapability)
   {
          
      /* Allocate mem for audio cap */
      pNewChannel->chanCap->cap = (void*)ASN1MALLOC(call->pctxt, sizeof(H245AudioCapability));
      if(!pNewChannel->chanCap->cap)
      {
         OOTRACEERR3("ERROR:Memory allocation for new logical channels audio "
                     "capability failed (%s, %s)n", call->callType,
                     call->callToken);
         ASN1MEMFREEPTR(call->pctxt, pNewChannel->chanCap);
         ASN1MEMFREEPTR(call->pctxt, pNewChannel);
         return NULL;
      }
      memset(pNewChannel->chanCap->cap, 0, sizeof(H245AudioCapability));
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
         ASN1MEMFREEPTR(call->pctxt, pNewChannel->chanCap->cap);
         ASN1MEMFREEPTR(call->pctxt, pNewChannel->chanCap);
         ASN1MEMFREEPTR(call->pctxt, pNewChannel);
         OOTRACEERR3("ERROR:AddNewLogicalChannel - Unhandled capability type "
                     "(%s, %s)\n", call->callType, call->callToken);
         return NULL;
      }

      /* Copy audio Capability */
      memcpy((void*)pNewChannel->chanCap->cap, (void*)epCap->cap, sizeof(H245AudioCapability));
   }
   else
   {
      /* Only audio capabilities supported at this point */
      ASN1MEMFREEPTR(call->pctxt, pNewChannel->chanCap);
      ASN1MEMFREEPTR(call->pctxt, pNewChannel);
      OOTRACEERR3("ERROR: AddNewLogicalChannel - Unhandled Capability type "
                  "(%s, %s)\n", call->callType, call->callToken);
      return NULL;
   }
  
   /* As per standards, media control port should be same for all
      proposed channels with same session ID. However, most applications
      use same media port for transmit and receive of audio streams. Infact,
      testing of OpenH323 based asterisk assumed that same ports are used. Hence
      we first search for existing media ports for smae session and use them. This
      should take care of all cases.
   */
   if(call->mediaInfo)
   {
      pMediaInfo = call->mediaInfo;
      while(pMediaInfo)
      {
         if(!strcmp(pMediaInfo->mediaType, type) &&
            !strcmp(pMediaInfo->dir, dir))
         {
            if(!call->isFastStartActive || (pMediaInfo->capType == audioCap->t))
               break;
         }
         pMediaInfo = pMediaInfo->next;
       
      }
   }
   if(pMediaInfo)
   {
      pNewChannel->localRtpPort = pMediaInfo->lMediaPort;
      pNewChannel->localRtcpPort = pMediaInfo->lMediaCntrlPort;
      strcpy(pNewChannel->localIP, pMediaInfo->lMediaIP);
   }else{
      pNewChannel->localRtpPort = ooGetNextPort(&gH323ep, OORTP);
      /* Ensures that RTP port is an even one */
      if((pNewChannel->localRtpPort & 1) == 1)
        pNewChannel->localRtpPort = ooGetNextPort(&gH323ep, OORTP);
      pNewChannel->localRtcpPort = ooGetNextPort(&gH323ep, OORTP);
      strcpy(pNewChannel->localIP, call->localIP);
   }
  
  
   /* Add new channel to the list */
   pNewChannel->next = NULL;
   if(!call->logicalChans)
      call->logicalChans = pNewChannel;
   else{
      pChannel = call->logicalChans;
      while(pChannel->next)  pChannel = pChannel->next;
      pChannel->next = pNewChannel;
   }
  
   /* increment logical channels */
   call->noOfLogicalChannels++;
   OOTRACEINFO3("Created new logical channel entry (%s, %s)\n", call->callType,
                call->callToken);
   return pNewChannel;
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

/* This function is used to get a logical channel with a particular session ID */
ooLogicalChannel* ooGetLogicalChannel(ooCallData *call, int sessionID)
{
   ooLogicalChannel * pChannel = NULL;
   pChannel = call->logicalChans;
   while(pChannel)
   {
      if(pChannel->sessionID == sessionID)
         return pChannel;
      else
         pChannel = pChannel->next;
   }
   return NULL;
}

/* Checks whether session with suplied ID and direction is already active*/
ASN1BOOL ooIsSessionEstablished(ooCallData *call, int sessionID, char* dir)
{
   ooLogicalChannel * temp = NULL;
   temp = call->logicalChans;

   while(temp)
   {
      if(temp->sessionID == sessionID              &&
         temp->state == OO_LOGICALCHAN_ESTABLISHED &&
         !strcmp(temp->dir, dir)                     )
         return TRUE;
      temp = temp->next;
   }
   return FALSE;
}

int ooClearAllLogicalChannels(ooCallData *call)
{
   ooLogicalChannel * temp = NULL, *prev = NULL;

   OOTRACEINFO3("Clearing all logical channels (%s, %s)\n", call->callType,
                 call->callToken);
  
   temp = call->logicalChans;
   while(temp)
   {
      prev = temp;
      temp = temp->next;
      ooClearLogicalChannel(call, prev->channelNo);/* TODO: efficiency - This causes re-search
                                                      of of logical channel in the list. Can be
                                                      easily improved.*/
   }
   return OO_OK;
}

int ooClearLogicalChannel(ooCallData *call, int channelNo)
{
   int ret = OO_OK;
   ooLogicalChannel *pLogicalChannel = NULL;
   ooH323EpCapability *epCap=NULL;
   pLogicalChannel = ooFindLogicalChannelByLogicalChannelNo(call,channelNo);
   if(!pLogicalChannel)
   {
      OOTRACEERR4("ERROR:Invalid logical Channel No %d (%s, %s)\n",
                  channelNo, call->callType, call->callToken);
      return OO_FAILED;
   }

   epCap = (ooH323EpCapability*) pLogicalChannel->chanCap;
   if(!strcmp(pLogicalChannel->dir, "receive"))
   {
      if(epCap->stopReceiveChannel)
      {
         epCap->stopReceiveChannel(call, pLogicalChannel);
         OOTRACEINFO4("Stopped Receive channel %d (%s, %s)\n",
              channelNo, call->callType, call->callToken);
      }
      else{
         OOTRACEERR4("ERROR:No callback registered for stopReceiveChannel %d "
                     "(%s, %s)\n", channelNo, call->callType, call->callToken);
      }
   }
   else
   {
      if(pLogicalChannel->state == OO_LOGICALCHAN_ESTABLISHED)
      {
         if(epCap->stopTransmitChannel)
         {
            epCap->stopTransmitChannel(call, pLogicalChannel);
            OOTRACEINFO4("Stopped Transmit channel %d (%s, %s)\n",
                 channelNo, call->callType, call->callToken);
         }
         else{
            OOTRACEERR4("ERROR:No callback registered for stopTransmitChannel %d "
                        "(%s, %s)\n", channelNo, call->callType, call->callToken);
         }
      }
   }
   ooRemoveLogicalChannel(call, channelNo);/* TODO: efficiency - This causes re-search of
                                                    of logical channel in the list. Can be
                                                    easily improved.*/
   return OO_OK;
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

int ooOnLogicalChannelEstablished(ooCallData *call, ooLogicalChannel * pChannel)
{
   ooLogicalChannel * temp = NULL, *prev=NULL;
   /* Change the state of the channel as established and close all other
      channels with same session IDs. This is useful for handling fastStart,
      as the endpoint can open multiple logical channels for same sessionID.
      Once the remote endpoint confirms it's selection, all other channels for
      the same sessionID must be closed.
   */
   OOTRACEDBGC3("In ooOnLogicalChannelEstablished (%s, %s)\n",
                call->callType, call->callToken);
   pChannel->state = OO_LOGICALCHAN_ESTABLISHED;
   temp = call->logicalChans;
   while(temp)
   {
      if(temp->channelNo != pChannel->channelNo &&
         temp->sessionID == pChannel->sessionID &&
         !strcmp(temp->dir, pChannel->dir)        )
      {
         prev = temp;
         temp = temp->next;
         ooClearLogicalChannel(call, prev->channelNo);
      }
      else
         temp = temp->next;
   }
   return OO_OK;  
}



int ooAddMediaInfo(ooCallData *call, ooMediaInfo mediaInfo)
{
   ooMediaInfo *newMediaInfo=NULL, *cur=NULL;

   if(!call)
   {
      OOTRACEERR3("Error:Invalid 'call' param for ooAddMediaInfo.(%s, 5s)\n",
                   call->callType, call->callToken);
      return OO_FAILED;
   }
   newMediaInfo = (ooMediaInfo*) ASN1MALLOC(call->pctxt, sizeof(ooMediaInfo));
   if(!newMediaInfo)
   {
      OOTRACEERR3("Error:failed to allocate memory for new mediaInfo. "
                  "(%s, %s)\n", call->callType, call->callToken);
      return OO_FAILED;
   }
   memset(newMediaInfo, 0, sizeof(ooMediaInfo));
   strcpy(newMediaInfo->dir, mediaInfo.dir);
   strcpy(newMediaInfo->mediaType, mediaInfo.mediaType);
   newMediaInfo->lMediaCntrlPort = mediaInfo.lMediaCntrlPort;
   strcpy(newMediaInfo->lMediaIP,mediaInfo.lMediaIP);
   newMediaInfo->lMediaPort = mediaInfo.lMediaPort;
   newMediaInfo->next = NULL;

   if(!call->mediaInfo)
      call->mediaInfo = newMediaInfo;
   else{
      cur = call->mediaInfo;
      while(cur->next != NULL) cur = cur->next;
      cur->next = newMediaInfo;
   }
   return OO_OK;
}
