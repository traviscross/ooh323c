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

#include "ootrace.h"
#include "ootypes.h"
#include "ooCalls.h"
#include "ooports.h"
#include "oochannels.h"
#include "ooh245.h"
#include "ooCapability.h"
#include "ooGkClient.h"


/** Global endpoint structure */
extern ooEndPoint gH323ep;

ooCallData* ooCreateCall(char* type, char*callToken)
{
   ooCallData *call=NULL;
   OOCTXT *pctxt=NULL;

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
   memcpy(&call->capPrefs, &gH323ep.capPrefs, sizeof(ooCapPrefs));   
   call->remoteAliases = NULL;
   call->dtmfmode = gH323ep.dtmfmode;
   call->masterSlaveState = OO_MasterSlave_Idle;
   call->localTermCapState = OO_LocalTermCapExchange_Idle;
   call->remoteCaps = NULL;
   call->remoteTermCapState = OO_RemoteTermCapExchange_Idle;
   strcpy(call->localIP, gH323ep.signallingIP);
   call->noOfLogicalChannels = 0;
   call->logicalChanNoBase = 1001;
   call->logicalChanNoMax = 1100;
   call->logicalChanNoCur = 1001;

   if (OO_TESTFLAG(gH323ep.flags, OO_M_TUNNELING))
      OO_SETFLAG (call->flags, OO_M_TUNNELING);

   if(gH323ep.gkClient && OO_TESTFLAG(gH323ep.flags, OO_M_GKROUTED))
   {
      OO_SETFLAG(call->flags, OO_M_GKROUTED);
   }

   call->mediaInfo = NULL;

   if (OO_TESTFLAG(gH323ep.flags, OO_M_FASTSTART))
      OO_SETFLAG (call->flags, OO_M_FASTSTART);

   dListInit(&call->remoteFastStartOLCs);
   dListInit(&call->timerList);
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
   OOTRACEDBGA4("In ooEndCall call state is - %s (%s, %s)\n",
                 ooGetCallStateText(call->callState), call->callType,
                 call->callToken);

   if (OO_TESTFLAG (call->flags, OO_M_FASTSTART))
   {
      if(call->callState < OO_CALL_CLEAR_CLOSEH245)
         call->callState = OO_CALL_CLEAR_CLOSEH245;
   }
   else {
      if(call->callState == OO_CALL_CLEAR)  
      {
         if(call->logicalChans)
         {
            if (OO_TESTFLAG (call->flags, OO_M_TUNNELING) ||
                call->pH245Channel->sock != 0)
            {
               OOTRACEINFO3("Call Clearing - CloseAllLogicalChannels."
                            "(%s, %s)\n", call->callType, call->callToken);
               ooCloseAllLogicalChannels(call);
               call->callState = OO_CALL_CLEAR_CLOLCS;
               return OO_OK;
            }else{
               OOTRACEINFO3("Call Clearing - ClearAllLogicalChannels. "
                            "(%s, %s)\n", call->callType, call->callToken);
               ooClearAllLogicalChannels(call);
               call->callState = OO_CALL_CLEAR_CLELCS;
            }
         }else
            call->callState = OO_CALL_CLEAR_CLELCS;
      }
  
      if(call->callState == OO_CALL_CLEAR_CLOLCS)
      {
         if(call->logicalChans){
            OOTRACEDBGC3("Call Clearing - Waiting for logical channels to be "
                         "closed.(%s, %s)\n", call->callType, call->callToken);
            return OO_OK;
         }else{
            call->callState = OO_CALL_CLEAR_CLELCS;
         }
      }
      

      if(call->callState == OO_CALL_CLEAR_CLELCS)
      {
  
         if(call->h245SessionState == OO_H245SESSION_ACTIVE)
         {
            OOTRACEINFO3("Call Clearing - sendEndSessionCommand. (%s, %s)\n",
                          call->callType, call->callToken);
            ooSendEndSessionCommand(call);
            call->callState = OO_CALL_CLEAR_ENDSESSION;
            return OO_OK;
         }else{
            call->callState = OO_CALL_CLEAR_CLOSEH245;
         }
      }

      if(call->callState == OO_CALL_CLEAR_ENDSESSION)
      {
         if(call->logicalChans)
            ooClearAllLogicalChannels(call);

         if(call->h245SessionState == OO_H245SESSION_ACTIVE)
         {
            OOTRACEDBGC3("Waiting for H.245 connection to be closed."
                         "(%s, %s)\n", call->callType, call->callToken);
            return OO_OK;
         }
         call->callState = OO_CALL_CLEAR_CLOSEH245;
      }
   }


   if(call->callState == OO_CALL_CLEAR_CLOSEH245)
   {
      if (0 != call->pH225Channel && 0 != call->pH225Channel->sock)
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

   if(call->callState == OO_CALL_CLEAR_RELEASE)
   {
      /* Wait till all the queued H.2250 messages are sent */
      if (call->pH225Channel != 0 && call->pH225Channel->sock != 0 &&
          call->pH225Channel->outQueue.count > 0)
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

   OOTRACEWARN4 ("Cleaning Call (%s, %s)- reason:%s\n",
                 call->callType, call->callToken,
                 ooGetReasonText (call->callEndReason));

   /* First clean all the logical channels, if not already cleaned. */
   if(call->logicalChans)
      ooClearAllLogicalChannels(call);
  
   /* Close H.245 connection, if not already closed */
   if(call->h245SessionState != OO_H245SESSION_INACTIVE)
      ooCloseH245Connection(call);
   else{
      if(call->pH245Channel && call->pH245Channel->outQueue.count > 0)
      {
         dListFreeAll(call->pctxt, &(call->pH245Channel->outQueue));
         memFreePtr(call->pctxt, call->pH245Channel);
      }
   }
  
   /* Close H225 connection, if not already closed. */
   if (0 != call->pH225Channel && 0 != call->pH225Channel->sock)
   {
      ooCloseH225Connection(call);
   }

   /* Clean timers */
   if(call->timerList.count > 0)
   {
      dListFreeAll(call->pctxt, &(call->timerList));
   }

   if(gH323ep.gkClient)
   {
     ooGkClientCleanCall(gH323ep.gkClient, call);
   }

   ooRemoveCallFromList(&gH323ep, call);
   OOTRACEINFO3("Removed call (%s, %s) from list\n", call->callType,
                 call->callToken);
 
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



ooLogicalChannel* ooAddNewLogicalChannel(ooCallData *call, int channelNo,
                                         int sessionID, char *type, char * dir,
                                         ooH323EpCapability *epCap)
{
   ooLogicalChannel *pNewChannel=NULL, *pChannel=NULL;
   ooMediaInfo *pMediaInfo = NULL;
   OOTRACEDBGC5("Adding new media channel for cap %d dir %s (%s, %s)\n",
                epCap->cap, dir, call->callType, call->callToken);
   /* Create a new logical channel entry */
   pNewChannel = (ooLogicalChannel*)ASN1MALLOC(call->pctxt,
                                                     sizeof(ooLogicalChannel));
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
  
   pNewChannel->chanCap->dir = epCap->dir;
  
   pNewChannel->chanCap->startReceiveChannel = epCap->startReceiveChannel;
   pNewChannel->chanCap->startTransmitChannel = epCap->startTransmitChannel;
   pNewChannel->chanCap->stopReceiveChannel = epCap->stopReceiveChannel;
   pNewChannel->chanCap->stopTransmitChannel = epCap->stopTransmitChannel;
   pNewChannel->chanCap->cap = epCap->cap;
   pNewChannel->chanCap->capType = epCap->capType;
   pNewChannel->chanCap->params = epCap->params;
   OOTRACEDBGC4("Adding new channel with cap %d (%s, %s)\n", epCap->cap,
                call->callType, call->callToken);
   /* As per standards, media control port should be same for all
      proposed channels with same session ID. However, most applications
      use same media port for transmit and receive of audio streams. Infact,
      testing of OpenH323 based asterisk assumed that same ports are used.
      Hence we first search for existing media ports for smae session and use
      them. This should take care of all cases.
   */
   if(call->mediaInfo)
   {
      pMediaInfo = call->mediaInfo;
      while(pMediaInfo)
      {
         if(!strcmp(pMediaInfo->dir, dir) &&
            (pMediaInfo->cap == epCap->cap))
         {
            break;
         }
         pMediaInfo = pMediaInfo->next;
      }
   }
   
   if(pMediaInfo)
   {
      OOTRACEDBGC3("Using configured media info (%s, %s)\n", call->callType,
                  call->callToken);
      pNewChannel->localRtpPort = pMediaInfo->lMediaPort;
      pNewChannel->localRtcpPort = pMediaInfo->lMediaCntrlPort;
      strcpy(pNewChannel->localIP, pMediaInfo->lMediaIP);
   }else{
      OOTRACEDBGC3("Using default media info (%s, %s)\n", call->callType,
                  call->callToken);
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

ooLogicalChannel* ooFindLogicalChannelByLogicalChannelNo(ooCallData *call,
                                                         int ChannelNo)
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

ooLogicalChannel * ooFindLogicalChannelByOLC(ooCallData *call,
                               H245OpenLogicalChannel *olc)
{
   H245DataType * psDataType=NULL;
   H245H2250LogicalChannelParameters * pslcp=NULL;
   OOTRACEDBGC4("ooFindLogicalChannel by olc %d (%s, %s)\n",
            olc->forwardLogicalChannelNumber, call->callType, call->callToken);
   if(olc->m.reverseLogicalChannelParametersPresent)
   {
      OOTRACEDBGC3("Finding receive channel (%s,%s)\n", call->callType,
                                                       call->callToken);
      psDataType = &olc->reverseLogicalChannelParameters.dataType;
      /* Only H2250LogicalChannelParameters are supported */
      if(olc->reverseLogicalChannelParameters.multiplexParameters.t !=
         T_H245OpenLogicalChannel_reverseLogicalChannelParameters_multiplexParameters_h2250LogicalChannelParameters){
         OOTRACEERR4("Error:Invalid olc %d received (%s, %s)\n",
           olc->forwardLogicalChannelNumber, call->callType, call->callToken);
         return NULL;
      }
      pslcp = olc->reverseLogicalChannelParameters.multiplexParameters.u.h2250LogicalChannelParameters;

      return ooFindLogicalChannel(call, pslcp->sessionID, "receive", psDataType);
   }
   else{
      OOTRACEDBGC3("Finding transmit channel (%s, %s)\n", call->callType,
                                                           call->callToken);
      psDataType = &olc->forwardLogicalChannelParameters.dataType;
      /* Only H2250LogicalChannelParameters are supported */
      if(olc->forwardLogicalChannelParameters.multiplexParameters.t !=
         T_H245OpenLogicalChannel_forwardLogicalChannelParameters_multiplexParameters_h2250LogicalChannelParameters)
      {
         OOTRACEERR4("Error:Invalid olc %d received (%s, %s)\n",
           olc->forwardLogicalChannelNumber, call->callType, call->callToken);
         return NULL;
      }
      pslcp = olc->forwardLogicalChannelParameters.multiplexParameters.u.h2250LogicalChannelParameters;
      return ooFindLogicalChannel(call, pslcp->sessionID, "transmit", psDataType);
   }
}

ooLogicalChannel * ooFindLogicalChannel(ooCallData *call, int sessionID,
                                        char *dir, H245DataType * dataType)
{
   ooLogicalChannel * pChannel = NULL;
   pChannel = call->logicalChans;
   while(pChannel)
   {
      if(pChannel->sessionID == sessionID)
      {
         if(!strcmp(pChannel->dir, dir))
         {
            if(dataType->t == T_H245DataType_audioData)
            {
               if(!strcmp(dir, "receive"))
               {
                 if(ooCheckCompatibility_1(call, pChannel->chanCap,
                     dataType->u.audioData, OORX))
                     return pChannel;
               }else if(!strcmp(dir, "transmit"))
               {
                 if(ooCheckCompatibility_1(call, pChannel->chanCap,
                     dataType->u.audioData, OOTX))
                     return pChannel;
               }
            }
         }
      }
      pChannel = pChannel->next;
   }
   return NULL;
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
      OOTRACEWARN4("Logical Channel %d doesn't exist (%s, %s)\n",
                  channelNo, call->callType, call->callToken);
      return OO_OK;
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
         //ASN1MEMFREEPTR(call->pctxt, temp->chanCap->cap);
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
   ooMediaInfo *newMediaInfo=NULL;

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
   newMediaInfo->lMediaCntrlPort = mediaInfo.lMediaCntrlPort;
   strcpy(newMediaInfo->lMediaIP,mediaInfo.lMediaIP);
   newMediaInfo->lMediaPort = mediaInfo.lMediaPort;
   newMediaInfo->cap = mediaInfo.cap;
   newMediaInfo->next = NULL;
   OOTRACEDBGC4("Configured mediainfo for cap %d (%s, %s)\n", mediaInfo.cap,
                call->callType, call->callToken);
   if(!call->mediaInfo)
      call->mediaInfo = newMediaInfo;
   else{
      newMediaInfo->next = call->mediaInfo;
      call->mediaInfo = newMediaInfo;
   }
   return OO_OK;
}
