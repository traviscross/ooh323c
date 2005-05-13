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
#include "ooCapability.h"
#include "ootrace.h"
#include "ooCalls.h"

/** Global endpoint structure */
extern ooEndPoint gH323ep;

static int giDynamicRTPPayloadType = 101;


int ooCapabilityEnableDTMFRFC2833(ooCallData *call, int dynamicRTPPayloadType)
{
   if(!call)
   {
      gH323ep.dtmfmode |= OO_CAP_DTMF_RFC2833;
      OOTRACEINFO1("Enabled RFC2833 DTMF capability for end-point\n");
   }else{
      call->dtmfmode |= OO_CAP_DTMF_RFC2833;
      OOTRACEINFO3("Enabled RFC2833 DTMF capability for (%s, %s) \n",
                   call->callType, call->callToken);
   }

   /*Dynamic RTP payload type range is from 96 - 127 */
   if(dynamicRTPPayloadType >= 96 && dynamicRTPPayloadType <= 127)
      giDynamicRTPPayloadType = dynamicRTPPayloadType;

   return OO_OK;
}



int ooCapabilityDisableDTMFRFC2833(ooCallData *call)
{
   if(!call){
      gH323ep.dtmfmode ^= OO_CAP_DTMF_RFC2833;
      OOTRACEINFO1("Disabled RFC2833 DTMF capability for end-point\n");
   }else{
      call->dtmfmode ^= OO_CAP_DTMF_RFC2833;
      OOTRACEINFO3("Disabled RFC2833 DTMF capability for (%s, %s)\n",
                    call->callType, call->callToken);
   }

   return OO_OK;
}

#if 0
int ooCapabilityAddG729Capability(ooCallData *call, int cap, int txframes,
                                 int rxframes, int dir,
                                 cb_StartReceiveChannel startReceiveChannel,
                                 cb_StartTransmitChannel startTransmitChannel,
                                 cb_StopReceiveChannel stopReceiveChannel,
                                 cb_StopTransmitChannel stopTransmitChannel,
                                 OOBOOL remote)
{
   return ooCapabilityAddSimpleCapability(call, cap, txframes, rxframes, dir,
                                 startReceiveChannel, startTransmitChannel,
                                 stopReceiveChannel, stopTransmitChannel,
                                 remote);

}


int ooCapabilityAddG711Capability(ooCallData *call, int cap, int txframes,
                                 int rxframes, int dir,
                                 cb_StartReceiveChannel startReceiveChannel,
                                 cb_StartTransmitChannel startTransmitChannel,
                                 cb_StopReceiveChannel stopReceiveChannel,
                                 cb_StopTransmitChannel stopTransmitChannel,
                                 OOBOOL remote)
{
   return ooCapabilityAddSimpleCapability(call, cap, txframes, rxframes, dir,
                                 startReceiveChannel, startTransmitChannel,
                                 stopReceiveChannel, stopTransmitChannel,
                                 remote);

}
#endif

int ooCapabilityAddSimpleCapability(ooCallData *call, int cap, int txframes,
                                 int rxframes, int dir,
                                 cb_StartReceiveChannel startReceiveChannel,
                                 cb_StartTransmitChannel startTransmitChannel,
                                 cb_StopReceiveChannel stopReceiveChannel,
                                 cb_StopTransmitChannel stopTransmitChannel,
                                 OOBOOL remote)
{
   int iRet=0;
   ooH323EpCapability *epCap = NULL, *cur=NULL, *lHead=NULL;
   ooCapParams *params=NULL;  
   OOCTXT *pctxt=NULL;
   if(!call) pctxt = &gH323ep.ctxt;
   else pctxt = call->pctxt;

   epCap = (ooH323EpCapability*)memAlloc(pctxt, sizeof(ooH323EpCapability));
   params = (ooCapParams*) memAlloc(pctxt, sizeof(ooCapParams));
   if(!epCap || !params)
   {
      OOTRACEERR1("Error:Memory - ooCapabilityAddSimpleCapability - "
                  "epCap/params\n");
      return OO_FAILED;
   }


   params->txframes = txframes;
   params->rxframes = rxframes;
   if(dir & OORXANDTX)
   {
      epCap->dir = OORX;
      epCap->dir |= OOTX;
   }else
      epCap->dir = dir;
  
   epCap->cap = cap;
   epCap->capType = OO_CAP_TYPE_AUDIO;
   epCap->params = (void*)params;
   epCap->startReceiveChannel = startReceiveChannel;
   epCap->startTransmitChannel = startTransmitChannel;
   epCap->stopReceiveChannel = stopReceiveChannel;
   epCap->stopTransmitChannel = stopTransmitChannel;
   epCap->next = NULL;

   if(!call)
   {/*Add as local capability */
      OOTRACEDBGC2("Adding endpoint capability %s. \n",
                     ooGetAudioCapTypeText(epCap->cap));
      if(!gH323ep.myCaps)
         gH323ep.myCaps = epCap;
      else{
         cur = gH323ep.myCaps;
         while(cur->next) cur = cur->next;
         cur->next = epCap;
      }
      ooAppendCapToCapPrefs(NULL, cap);
      gH323ep.noOfCaps++;
   }else{
      if(remote)
      {
         /*Add as remote capability */
         if(!call->remoteCaps)
            call->remoteCaps = epCap;
         else{
            cur = call->remoteCaps;
            while(cur->next) cur = cur->next;
            cur->next = epCap;
         }
     }else{
        /*Add as our capability */
        OOTRACEDBGC4("Adding call specific capability %s. (%s, %s)\n",
                     ooGetAudioCapTypeText(epCap->cap), call->callType,
                     call->callToken);
        if(!call->ourCaps){
           call->ourCaps = epCap;
           ooResetCapPrefs(call);
        }
        else{
           cur = call->ourCaps;
           while(cur->next) cur = cur->next;
           cur->next = epCap;
        }
        ooAppendCapToCapPrefs(call, cap);
     }
   }
          
   return OO_OK;
}


int ooCapabilityAddGSMCapability(ooCallData *call, int cap,
                                unsigned framesPerPkt, OOBOOL comfortNoise,
                                OOBOOL scrambled, int dir,
                                cb_StartReceiveChannel startReceiveChannel,
                                cb_StartTransmitChannel startTransmitChannel,
                                cb_StopReceiveChannel stopReceiveChannel,
                                cb_StopTransmitChannel stopTransmitChannel,
                                OOBOOL remote)
{
   int iRet=0;
   ooH323EpCapability *epCap = NULL, *cur=NULL;
   ooGSMCapParams *params=NULL;  
   OOCTXT *pctxt = NULL;

   if(!call) pctxt = &gH323ep.ctxt;
   else pctxt = call->pctxt;

   epCap = (ooH323EpCapability*)memAlloc(pctxt, sizeof(ooH323EpCapability));
   params = (ooGSMCapParams*) memAlloc(pctxt, sizeof(ooGSMCapParams));
   if(!epCap || !params)
   {
      OOTRACEERR1("Error:Memory - ooCapabilityAddGSMCapability - "
                  "epCap/params\n");
      return OO_FAILED;
   }


   params->rxframes = framesPerPkt;
   params->txframes = framesPerPkt;
   params->comfortNoise = comfortNoise;
   params->scrambled = scrambled;
   if(dir & OORXANDTX)
   {
      epCap->dir = OORX;
      epCap->dir |= OOTX;
   }else
      epCap->dir = dir;

   epCap->cap = cap;
   epCap->capType = OO_CAP_TYPE_AUDIO;
   epCap->params = (void*)params;
   epCap->startReceiveChannel = startReceiveChannel;
   epCap->startTransmitChannel = startTransmitChannel;
   epCap->stopReceiveChannel = stopReceiveChannel;
   epCap->stopTransmitChannel = stopTransmitChannel;
  
   epCap->next = NULL;
   /* Add as local capability */
   if(!call)
   {
      if(!gH323ep.myCaps)
         gH323ep.myCaps = epCap;
      else{
         cur = gH323ep.myCaps;
         while(cur->next) cur = cur->next;
         cur->next = epCap;
      }
      ooAppendCapToCapPrefs(NULL, cap);
      gH323ep.noOfCaps++;
   }else{
      if(remote)
      {
         /*Add as remote capability */
         if(!call->remoteCaps)
            call->remoteCaps = epCap;
         else{
            cur = call->remoteCaps;
            while(cur->next) cur = cur->next;
            cur->next = epCap;
         }
      }else{
         OOTRACEDBGC4("Adding call specific capability %s. (%s, %s)\n",
                     ooGetAudioCapTypeText(epCap->cap), call->callType,
                     call->callToken);
         /*Add as our capability */
         if(!call->ourCaps){
            call->ourCaps = epCap;
            ooResetCapPrefs(call);
         }
         else{
            cur = call->ourCaps;
            while(cur->next) cur = cur->next;
            cur->next = epCap;
         }
         ooAppendCapToCapPrefs(call, cap);
      }
   }

   return OO_OK;
}


  
struct H245AudioCapability* ooCreateAudioCapability
      (ooH323EpCapability *epCap, OOCTXT *pctxt, int dir)
{

   if(!epCap)
   {
     OOTRACEERR1("Error:Invalid capability parameter passed to "
                 "ooCreateAudioCapability.\n");
     return NULL;
   }
  
   if(!(epCap->dir & dir))
   {
      OOTRACEERR1("Error:Failed to create capability due to direction "
                  "mismatch.\n");
      return NULL;
   }

   switch(OOABSAUDIOCAP(epCap->cap))
   {
   case T_H245AudioCapability_g711Alaw64k:
   case T_H245AudioCapability_g711Alaw56k:
   case T_H245AudioCapability_g711Ulaw64k:
   case T_H245AudioCapability_g711Ulaw56k:
   case T_H245AudioCapability_g729:
   case T_H245AudioCapability_g729AnnexA:
     return ooCreateSimpleCapability(epCap, pctxt, dir);
   case T_H245AudioCapability_g722_64k:
   case T_H245AudioCapability_g722_56k:
   case T_H245AudioCapability_g722_48k:
   case T_H245AudioCapability_g7231:
   case T_H245AudioCapability_g728:
   case T_H245AudioCapability_is11172AudioCapability:
   case T_H245AudioCapability_is13818AudioCapability:
   case T_H245AudioCapability_g729wAnnexB:
   case T_H245AudioCapability_g729AnnexAwAnnexB:
   case T_H245AudioCapability_g7231AnnexCCapability:
   case T_H245AudioCapability_gsmFullRate:
      return ooCreateGSMFullRateCapability(epCap, pctxt, dir);
   case T_H245AudioCapability_gsmHalfRate:
   case T_H245AudioCapability_gsmEnhancedFullRate:
   case T_H245AudioCapability_genericAudioCapability:
   case T_H245AudioCapability_g729Extensions:
   case T_H245AudioCapability_vbd:
   case T_H245AudioCapability_audioTelephonyEvent:
   case T_H245AudioCapability_audioTone:
   case T_H245AudioCapability_extElem1:
   case T_H245AudioCapability_nonStandard:
   default:
      OOTRACEERR2("ERROR: Don't know how to create audio capability %d\n",
                  epCap->cap);
   }
   return NULL;
}


void* ooCreateDTMFCapability(int cap, OOCTXT *pctxt)
{
   H245AudioTelephonyEventCapability *pATECap=NULL;
   char *events=NULL;
   switch(cap)
   {
   case OO_CAP_DTMF_RFC2833:
      pATECap = (H245AudioTelephonyEventCapability*)memAlloc(pctxt,
                                   sizeof(H245AudioTelephonyEventCapability));
      if(!pATECap)
      {
         OOTRACEERR1("Error:Memory - ooCreateDTMFCapability - pATECap\n");
         return NULL;
      }
      memset(pATECap, 0, sizeof(H245AudioTelephonyEventCapability));
      pATECap->dynamicRTPPayloadType = giDynamicRTPPayloadType;
      events = (char*)memAlloc(pctxt, strlen("0-16")+1);
      if(!events)
      {
         OOTRACEERR1("Error:Memory - ooCreateDTMFCapability - events\n");
         memFreePtr(pctxt, pATECap);
         return NULL;
      }
      strncpy(events, "0-16", strlen("0-16"));
      pATECap->audioTelephoneEvent = events;
      return pATECap;
   default:
     OOTRACEERR1("Error:unknown dtmf capability type\n");
   }
   return NULL;
}

struct H245AudioCapability* ooCreateGSMFullRateCapability
   (ooH323EpCapability *epCap, OOCTXT* pctxt, int dir)
{
   H245AudioCapability *pAudio=NULL;
   H245GSMAudioCapability *pGSMCap=NULL;
   if(!epCap || !epCap->params)
   {
     OOTRACEERR1("Error:Invalid capability parameters to "
                 "ooCreateGSMFullRateCapability.\n");
     return NULL;
   }

   pAudio = (H245AudioCapability*)memAlloc(pctxt,
                                                sizeof(H245AudioCapability));
   pGSMCap = (H245GSMAudioCapability*)memAlloc(pctxt,
                                              sizeof(H245GSMAudioCapability));
   if(!pAudio || !pGSMCap)
   {
      OOTRACEERR1("ERROR:Memory - ooCreateGSMFullRateCapability - "
                  "pAudio/pGSMCap\n");
      return NULL;
   }
  
   pAudio->t = T_H245AudioCapability_gsmFullRate;
   pAudio->u.gsmFullRate = pGSMCap;
   if(dir & OORX)
      pGSMCap->audioUnitSize = ((ooGSMCapParams*)epCap->params)->rxframes*OO_GSMFRAMESIZE;
   else
      pGSMCap->audioUnitSize = ((ooGSMCapParams*)epCap->params)->txframes*OO_GSMFRAMESIZE;

   pGSMCap->comfortNoise = ((ooGSMCapParams*)epCap->params)->comfortNoise;
   pGSMCap->scrambled = ((ooGSMCapParams*)epCap->params)->scrambled;

   return pAudio;
}

struct H245AudioCapability* ooCreateSimpleCapability
   (ooH323EpCapability *epCap, OOCTXT* pctxt, int dir)
{
   H245AudioCapability *pAudio=NULL;
   ooCapParams *params;
   if(!epCap || !epCap->params)
   {
     OOTRACEERR1("Error:Invalid capability parameters to "
                 "ooCreateSimpleCapability.\n");
     return NULL;
   }
   params =(ooCapParams*)epCap->params;
   pAudio = (H245AudioCapability*)memAlloc(pctxt,
                                                sizeof(H245AudioCapability));
   if(!pAudio)
   {
      OOTRACEERR1("ERROR:Memory - ooCreateSimpleCapability - pAudio\n");
      return NULL;
   }

  
   switch(OOABSAUDIOCAP(epCap->cap))
   {
   case T_H245AudioCapability_g711Alaw64k:
      pAudio->t = T_H245AudioCapability_g711Alaw64k;
      if(dir & OORX)
         pAudio->u.g711Alaw64k = params->rxframes;
      else
         pAudio->u.g711Alaw64k = params->txframes;
      return pAudio;
   case T_H245AudioCapability_g711Alaw56k:
      pAudio->t = T_H245AudioCapability_g711Alaw56k;
      if(dir & OORX)
         pAudio->u.g711Alaw56k = params->rxframes;
      else
         pAudio->u.g711Alaw56k = params->txframes;
      return pAudio;
   case T_H245AudioCapability_g711Ulaw64k:
      pAudio->t = T_H245AudioCapability_g711Ulaw64k;
      if(dir & OORX)
         pAudio->u.g711Ulaw64k = params->rxframes;
      else
         pAudio->u.g711Ulaw64k = params->txframes;
      return pAudio;
   case T_H245AudioCapability_g711Ulaw56k:
      pAudio->t = T_H245AudioCapability_g711Ulaw56k;
      if(dir & OORX)
         pAudio->u.g711Ulaw56k = params->rxframes;
      else
         pAudio->u.g711Ulaw64k = params->txframes;
      return pAudio;
   case T_H245AudioCapability_g729:
      pAudio->t = T_H245AudioCapability_g729;
      if(dir & OORX)
         pAudio->u.g729 = params->rxframes;
      else
         pAudio->u.g729 = params->txframes;
      return pAudio;
   case T_H245AudioCapability_g729AnnexA:
      pAudio->t = T_H245AudioCapability_g729AnnexA;
      if(dir & OORX)
         pAudio->u.g729AnnexA = params->rxframes;
      else
         pAudio->u.g729AnnexA = params->txframes;
      return pAudio;
   default:
      OOTRACEERR2("ERROR: Don't know how to create audio capability %d\n",
                   epCap->cap);
   }
   return NULL;
}


ASN1BOOL ooCapabilityCheckCompatibility_Simple
   (ooCallData *call, ooH323EpCapability* epCap,
    H245AudioCapability* audioCap, int dir)
{
   int noofframes=0, cap;
   switch(audioCap->t)
   {
   case T_H245AudioCapability_g711Ulaw56k:
      cap = OO_G711ULAW56K;
      noofframes = audioCap->u.g711Ulaw56k;
      break;
   case T_H245AudioCapability_g711Ulaw64k:
      cap = OO_G711ULAW64K;
      noofframes = audioCap->u.g711Ulaw64k;
      break;
   case T_H245AudioCapability_g711Alaw64k:
      cap = OO_G711ALAW64K;
      noofframes = audioCap->u.g711Alaw64k;
      break;
   case T_H245AudioCapability_g711Alaw56k:
      cap = OO_G711ALAW56K;
      noofframes = audioCap->u.g711Alaw56k;
      break;
   case T_H245AudioCapability_g729:
      cap = OO_G729;
      noofframes = audioCap->u.g729;
      break;
   case T_H245AudioCapability_g729AnnexA:
      cap = OO_G729;
      noofframes = audioCap->u.g729AnnexA;
      break;  
   default:
      return FALSE;
   }

   /* can we receive this capability */
   if(dir & OORX)
   {
      if(((ooCapParams*)epCap->params)->rxframes >= noofframes)
         return TRUE;
   }

   /* Can we transmit compatible stream */
   if(dir & OOTX)
   {
      if(((ooCapParams*)epCap->params)->txframes <= noofframes)
         return TRUE;
   }
   return FALSE;

}


OOBOOL ooCapabilityCheckCompatibility_GSM
   (ooCallData *call, ooH323EpCapability* epCap,
    H245AudioCapability* audioCap, int dir)
{
   unsigned noofframes=0, cap;
   switch(audioCap->t)
   {
   case T_H245AudioCapability_gsmFullRate:
      cap = OO_GSMFULLRATE;
      noofframes = (audioCap->u.gsmFullRate->audioUnitSize)/OO_GSMFRAMESIZE;
      break;
   case T_H245AudioCapability_gsmHalfRate:
      cap = OO_GSMHALFRATE;
      noofframes = (audioCap->u.gsmHalfRate->audioUnitSize)/OO_GSMFRAMESIZE;
      break;
   case T_H245AudioCapability_gsmEnhancedFullRate:
      cap = OO_GSMENHANCEDFULLRATE;
      noofframes = (audioCap->u.gsmEnhancedFullRate->audioUnitSize)/OO_GSMFRAMESIZE;
      break;
   default:
      return FALSE;
   }

   /* can we receive this capability */
   if(dir & OORX)
   {
      if(((ooGSMCapParams*)epCap->params)->rxframes >= noofframes)
         return TRUE;
   }

   /* Make sure we transmit compatible stream */
   if(dir & OOTX)
   {
      if(((ooGSMCapParams*)epCap->params)->txframes > noofframes){
         OOTRACEDBGA5("Reducing txframes for GSM from %d to %d to match "
                      "receive capability of remote end.(%s, %s)\n",
                     ((ooGSMCapParams*)epCap->params)->txframes, noofframes,
                     call->callType, call->callToken);
         ((ooGSMCapParams*)epCap->params)->txframes = noofframes;
      }
      return TRUE;
   }
   return FALSE;

}

OOBOOL ooCheckCompatibility_1
   (ooCallData *call, ooH323EpCapability* epCap,
    H245AudioCapability* audioCap, int dir)
{

   switch(audioCap->t)
   {
   case T_H245AudioCapability_g711Ulaw56k:
   case T_H245AudioCapability_g711Ulaw64k:
   case T_H245AudioCapability_g711Alaw64k:
   case T_H245AudioCapability_g711Alaw56k:
   case T_H245AudioCapability_g729:
   case T_H245AudioCapability_g729AnnexA:
      return ooCapabilityCheckCompatibility_Simple(call, epCap, audioCap, dir);
   case T_H245AudioCapability_gsmFullRate:
      return ooCapabilityCheckCompatibility_GSM(call, epCap, audioCap, dir);
   default:
      return FALSE;
   }

   return FALSE; 
}

/**
  TODO: If txCap is local and number of txframes is greater than remote can
  receive, we should automatically decrease it. And logical channel cap should
  be the one where it should be decreased. The start logical channel will
  indicate the application that it is supposed to tx and a reduced rate.
 */
ASN1BOOL ooCheckCompatibility
   (ooCallData *call, ooH323EpCapability *txCap, ooH323EpCapability *rxCap)
{

   if(txCap->cap != rxCap->cap) return FALSE;

   if(!(txCap->dir & OOTX)) return FALSE;
  
   if(!(rxCap->dir & OORX)) return FALSE;

   switch(txCap->cap)
   {  
   case OO_G711ALAW64K:
   case OO_G711ALAW56K:
   case OO_G711ULAW64K:
   case OO_G711ULAW56K:
   case OO_G729:
   case OO_G729A:
     if(((ooCapParams*)txCap->params)->txframes <=
                                ((ooCapParams*)rxCap->params)->rxframes)
       return TRUE;
     else{
       OOTRACEDBGA4("Simple caps %s are not compatible.(%s, %s)\n",
                    ooGetAudioCapTypeText(txCap->cap), call->callType,
                    call->callToken);
       return FALSE;
     }
   case OO_GSMFULLRATE:
   case OO_GSMHALFRATE:
   case OO_GSMENHANCEDFULLRATE:
     if(((ooGSMCapParams*)txCap->params)->txframes <=
                                ((ooGSMCapParams*)rxCap->params)->rxframes)
       return TRUE;
     else{
       OOTRACEDBGA3("GSM caps are not compatible. (%s, %s)\n", call->callType,
                     call->callToken);
       return FALSE;
     } 
   default:
     OOTRACEWARN3("WARN: Unsupported capabilities being compared. (%s, %s)\n",
                   call->callType, call->callToken);
   }
   return FALSE;

}



ooH323EpCapability* ooIsAudioDataTypeGSMSupported
   (ooCallData *call, H245AudioCapability* audioCap, int dir)
{
   unsigned framesPerPkt=0;
   int cap=0;
   ooH323EpCapability *cur = NULL, *epCap=NULL;
   ooGSMCapParams *params = NULL;

   switch(audioCap->t)
   {
   case T_H245AudioCapability_gsmFullRate:
      framesPerPkt = (audioCap->u.gsmFullRate->audioUnitSize)/OO_GSMFRAMESIZE;
      cap = OO_GSMFULLRATE;
      break;
   case T_H245AudioCapability_gsmHalfRate:
      framesPerPkt = (audioCap->u.gsmHalfRate->audioUnitSize)/OO_GSMFRAMESIZE;
      cap = OO_GSMHALFRATE;
      break;
   case T_H245AudioCapability_gsmEnhancedFullRate:
      framesPerPkt = (audioCap->u.gsmEnhancedFullRate->audioUnitSize)/OO_GSMFRAMESIZE;
      cap = OO_GSMENHANCEDFULLRATE;
      break;
   default:
      OOTRACEERR3("Error:Invalid GSM capability type.(%s, %s)\n",
                                           call->callType, call->callToken);
     return NULL;
   }

   OOTRACEDBGC4("Determined audio data type to be of type %d. Searching"
                " for matching capability.(%s, %s)\n", cap, call->callType,
                call->callToken);

   /* If we have call specific caps then we use them, otherwise we use
      general endpoint caps*/
   if(call->ourCaps)  
      cur = call->ourCaps;
   else
      cur = gH323ep.myCaps;

   while(cur)
   {
      OOTRACEDBGC4("Local cap being compared %d. (%s, %s)\n", cur->cap,
                     call->callType, call->callToken);
     
      if(cur->cap == cap && (cur->dir & dir))
         break;
      cur = cur->next;
   }
  
   if(!cur) return NULL;
  
   OOTRACEDBGC4("Found matching audio capability type %d. Comparing"
                " other parameters. (%s, %s)\n", cap, call->callType,
                call->callToken);
  
   /* can we receive this capability */
   if(dir & OORX)
   {
      if(((ooGSMCapParams*)cur->params)->rxframes < framesPerPkt)
         return NULL;
      else{
         epCap = (ooH323EpCapability*)memAlloc(call->pctxt,
                                                 sizeof(ooH323EpCapability));
         params =(ooGSMCapParams*)memAlloc(call->pctxt,sizeof(ooGSMCapParams));
         if(!epCap || !params)
         {
            OOTRACEERR3("Error:Memory - ooIsAudioDataTypeGSMSupported - "
                        "epCap/params (%s, %s)\n", call->callType,
                        call->callToken);
            return NULL;
         }
         epCap->params = params;
         epCap->cap = cur->cap;
         epCap->dir = cur->dir;
         epCap->capType = cur->capType;
         epCap->startReceiveChannel = cur->startReceiveChannel;
         epCap->startTransmitChannel= cur->startTransmitChannel;
         epCap->stopReceiveChannel = cur->stopReceiveChannel;
         epCap->stopTransmitChannel = cur->stopTransmitChannel;
         epCap->next = NULL;
         memcpy(epCap->params, cur->params, sizeof(ooGSMCapParams));
         return epCap;
      }
   }

   /* Can we transmit compatible stream */
   if(dir & OOTX)
   {
      epCap = (ooH323EpCapability*)memAlloc(call->pctxt,
                                                sizeof(ooH323EpCapability));
      params =(ooGSMCapParams*)memAlloc(call->pctxt,sizeof(ooGSMCapParams));
      if(!epCap || !params)
      {
         OOTRACEERR3("Error:Memory - ooIsAudioDataTypeGSMSupported - "
                     "epCap/params (%s, %s)\n", call->callType,
                     call->callToken);
         return NULL;
      }
      epCap->params = params;
      epCap->cap = cur->cap;
      epCap->dir = cur->dir;
      epCap->capType = cur->capType;
      epCap->startReceiveChannel = cur->startReceiveChannel;
      epCap->startTransmitChannel= cur->startTransmitChannel;
      epCap->stopReceiveChannel = cur->stopReceiveChannel;
      epCap->stopTransmitChannel = cur->stopTransmitChannel;
      epCap->next = NULL;
      memcpy(epCap->params, cur->params, sizeof(ooGSMCapParams));
      if(params->txframes > framesPerPkt)
      {
         OOTRACEINFO5("Reducing framesPerPkt for transmission of GSM "
                      "capability from %d to %d to match receive capability of"
                      " remote endpoint.(%s, %s)\n", params->txframes,
                      framesPerPkt, call->callType, call->callToken);
         params->txframes = framesPerPkt;
      }

      return epCap;

   }
   return NULL;

}

ooH323EpCapability* ooIsAudioDataTypeSimpleSupported
   (ooCallData *call, H245AudioCapability* audioCap, int dir)
{
   int cap, framesPerPkt=0;
   ooH323EpCapability *cur=NULL, *epCap=NULL;
   ooCapParams * params= NULL;

   /* Find similar capability */
   switch(audioCap->t)
   {
      case T_H245AudioCapability_g711Alaw64k:
         framesPerPkt = audioCap->u.g711Alaw64k;
         cap = OO_G711ALAW64K;
         break;
      case T_H245AudioCapability_g711Alaw56k:
         framesPerPkt = audioCap->u.g711Alaw56k;
         cap = OO_G711ALAW56K;
         break;
      case T_H245AudioCapability_g711Ulaw56k:
         framesPerPkt = audioCap->u.g711Ulaw56k;
         cap = OO_G711ULAW56K;
         break;
      case T_H245AudioCapability_g711Ulaw64k:
         framesPerPkt = audioCap->u.g711Ulaw64k;
         cap = OO_G711ULAW64K;
         break;
      case T_H245AudioCapability_g729:
         framesPerPkt = audioCap->u.g729;
         cap = OO_G729;
         break;
      case T_H245AudioCapability_g729AnnexA:
         framesPerPkt = audioCap->u.g729AnnexA;
         cap = OO_G729A;
         break;
      default:
        return NULL;
   }

   OOTRACEDBGC4("Determined Simple audio data type to be of type %s. Searching"
                " for matching capability.(%s, %s)\n",
                ooGetAudioCapTypeText(cap), call->callType, call->callToken);

   /* If we have call specific caps, we use them; otherwise use general
      endpoint caps
   */  
   if(call->ourCaps)
     cur = call->ourCaps;
   else
     cur = gH323ep.myCaps;

   while(cur)
   {
      OOTRACEDBGC4("Local cap being compared %s. (%s, %s)\n",
              ooGetAudioCapTypeText(cur->cap),call->callType, call->callToken);
     
      if(cur->cap == cap && (cur->dir & dir))
         break;
      cur = cur->next;
   }
  
   if(!cur) return NULL;
  
   OOTRACEDBGC4("Found matching simple audio capability type %s. Comparing"
                " other parameters. (%s, %s)\n", ooGetAudioCapTypeText(cap),
                call->callType, call->callToken);
  
   /* can we receive this capability */
   if(dir & OORX)
   {
     if(((ooCapParams*)cur->params)->rxframes < framesPerPkt)
         return NULL;
     else{
        OOTRACEDBGC4("We can receive Simple capability %s. (%s, %s)\n",
                      ooGetAudioCapTypeText(cur->cap), call->callType,
                      call->callToken);
        epCap = (ooH323EpCapability*)memAlloc(call->pctxt,
                                                 sizeof(ooH323EpCapability));
        params=(ooCapParams*)memAlloc(call->pctxt,sizeof(ooCapParams));
        if(!epCap || !params)
        {
           OOTRACEERR3("Error:Memory - ooIsAudioDataTypeSimpleSupported - "
                       "epCap/params (%s, %s)\n", call->callType,
                       call->callToken);
           return NULL;
        }
        epCap->params = params;
        epCap->cap = cur->cap;
        epCap->dir = cur->dir;
        epCap->capType = cur->capType;
        epCap->startReceiveChannel = cur->startReceiveChannel;
        epCap->startTransmitChannel= cur->startTransmitChannel;
        epCap->stopReceiveChannel = cur->stopReceiveChannel;
        epCap->stopTransmitChannel = cur->stopTransmitChannel;
        epCap->next = NULL;
        memcpy(epCap->params, cur->params, sizeof(ooCapParams));
        OOTRACEDBGC4("Returning copy of matched receive capability %s. "
                     "(%s, %s)\n",
                     ooGetAudioCapTypeText(cur->cap), call->callType,
                     call->callToken);
        return epCap;
     }
   }

   /* Can we transmit compatible stream */
   if(dir & OOTX)
   {
      OOTRACEDBGC4("We can transmit Simple capability %s. (%s, %s)\n",
                   ooGetAudioCapTypeText(cur->cap), call->callType,
                   call->callToken);
      epCap = (ooH323EpCapability*)memAlloc(call->pctxt,
                                                sizeof(ooH323EpCapability));
      params =(ooCapParams*)memAlloc(call->pctxt,sizeof(ooCapParams));
      if(!epCap || !params)
      {
         OOTRACEERR3("Error:Memory - ooIsAudioDataTypeSimpleSupported - "
                     "epCap/params (%s, %s)\n", call->callType,
                     call->callToken);
         return NULL;
      }
      epCap->params = params;
      epCap->cap = cur->cap;
      epCap->dir = cur->dir;
      epCap->capType = cur->capType;
      epCap->startReceiveChannel = cur->startReceiveChannel;
      epCap->startTransmitChannel= cur->startTransmitChannel;
      epCap->stopReceiveChannel = cur->stopReceiveChannel;
      epCap->stopTransmitChannel = cur->stopTransmitChannel;
      epCap->next = NULL;
      memcpy(epCap->params, cur->params, sizeof(ooCapParams));
      if(params->txframes > framesPerPkt)
      {
         OOTRACEINFO5("Reducing framesPerPkt for transmission of Simple "
                      "capability from %d to %d to match receive capability of"
                      " remote endpoint.(%s, %s)\n", params->txframes,
                      framesPerPkt, call->callType, call->callToken);
         params->txframes = framesPerPkt;
      }
      OOTRACEDBGC4("Returning copy of matched transmit capability %s."
                   "(%s, %s)\n",
                   ooGetAudioCapTypeText(cur->cap), call->callType,
                   call->callToken);
      return epCap;
   }
   return NULL;
}



ooH323EpCapability* ooIsAudioDataTypeSupported
   (ooCallData *call, H245AudioCapability* audioCap, int dir)
{
   /* Find similar capability */
   switch(audioCap->t)
   {
      case T_H245AudioCapability_g711Alaw64k:
      case T_H245AudioCapability_g711Alaw56k:
      case T_H245AudioCapability_g711Ulaw56k:
      case T_H245AudioCapability_g711Ulaw64k:
         return ooIsAudioDataTypeSimpleSupported(call, audioCap, dir);
      case T_H245AudioCapability_gsmFullRate:
      case T_H245AudioCapability_gsmHalfRate:
      case T_H245AudioCapability_gsmEnhancedFullRate:
         return ooIsAudioDataTypeGSMSupported(call, audioCap, dir);  
      default:
         return NULL;
   }  
}


ooH323EpCapability* ooIsDataTypeSupported
                   (ooCallData *call, H245DataType *data, int dir)
{
   OOTRACEDBGC3("Looking for data type support. (%s, %s)\n", call->callType,
                 call->callToken);
   
   switch(data->t)
   {
   case T_H245DataType_nonStandard:
      return NULL;
   case T_H245DataType_nullData:
      return NULL;
   case T_H245DataType_videoData:
      return NULL;
   case T_H245DataType_audioData:
      OOTRACEDBGC3("Looking for audio dataType support. (%s, %s)\n",
                   call->callType, call->callToken);
      return ooIsAudioDataTypeSupported(call, data->u.audioData, dir);
   case T_H245DataType_data:
     return NULL;
   case T_H245DataType_encryptionData:
     return NULL;
   case T_H245DataType_h235Control:
     return NULL;
   case T_H245DataType_h235Media:
     return NULL;
   case T_H245DataType_multiplexedStream:
     return NULL;
   default:
      OOTRACEINFO3("Unknown data type (%s, %s)\n", call->callType,
                    call->callToken);
   }
   return NULL;
}

int ooResetCapPrefs(ooCallData *call)
{
   ooCapPrefs *capPrefs=NULL;
   if(call)
      capPrefs = &call->capPrefs;
   else
      capPrefs = &gH323ep.capPrefs;
   memset(capPrefs, 0, sizeof(ooCapPrefs));
   return OO_OK;
}

int ooRemoveCapFromCapPrefs(ooCallData *call, int cap)
{
   int i=0, j=0;
   ooCapPrefs *capPrefs=NULL, oldPrefs;
   if(call)
      capPrefs = &call->capPrefs;
   else
      capPrefs = &gH323ep.capPrefs;

   memcpy(&oldPrefs, capPrefs, sizeof(ooCapPrefs));
   memset(capPrefs, 0, sizeof(ooCapPrefs));
   for(i=0; i<oldPrefs.index; i++)
   { 
      if(oldPrefs.order[i] != cap)
         capPrefs->order[j++] = oldPrefs.order[i];
   }
   capPrefs->index = j;
   return OO_OK;
}


int ooAppendCapToCapPrefs(ooCallData *call, int cap)
{
   ooCapPrefs *capPrefs=NULL;
   if(call)
      capPrefs = &call->capPrefs;
   else
      capPrefs = &gH323ep.capPrefs;

   capPrefs->order[capPrefs->index++] = cap;
   return OO_OK;
}

int ooChangeCapPrefOrder(ooCallData *call, int cap, int pos)
{
   int i=0, j=0;
   ooCapPrefs *capPrefs = NULL;

   /* Whether to change prefs for call or for endpoint as a whole */
   if(call)
      capPrefs = &call->capPrefs;
   else
      capPrefs = &gH323ep.capPrefs;

   /* check whether cap exists, cap must exist */
   for(i=0; i<capPrefs->index; i++)
   {
      if(capPrefs->order[i] == cap)
         break;
   }
   if(i == capPrefs->index) return OO_FAILED;

   if(i==pos) return OO_OK; /* No need to change */

   /* Decrease Pref order */
   if(i < pos)
   {
      for(i; i<pos; i++)
         capPrefs->order[i] = capPrefs->order[i+1];
      capPrefs->order[i]=cap;
      return OO_OK;
   }
   /* Increase Pref order */
   if(i>pos)
   {
     for(j=i; j>pos; j--)
       capPrefs->order[j] = capPrefs->order[j-1];
     capPrefs->order[j] = cap;
     return OO_OK;
   }

   return OO_FAILED;

}

int ooPreppendCapToCapPrefs(ooCallData *call, int cap)
{
   int i=0, j=0;
   ooCapPrefs *capPrefs=NULL, oldPrefs;
   if(call)
      capPrefs = &call->capPrefs;
   else
      capPrefs = &gH323ep.capPrefs;

   memcpy(&oldPrefs, capPrefs, sizeof(ooCapPrefs));


   capPrefs->order[j++] = cap;

   for(i=0; i<oldPrefs.index; i++)
   { 
      if(oldPrefs.order[i] != cap)
         capPrefs->order[j++] = oldPrefs.order[i];
   }
   capPrefs->index = j;
   return OO_OK;
}

      
int ooAddRemoteCapability(ooCallData *call, H245Capability *cap)
{
   switch(cap->t)
   {
   case T_H245Capability_receiveAudioCapability:
     return ooAddRemoteAudioCapability(call, cap->u.receiveAudioCapability,
                                       OORX);
   case T_H245Capability_transmitAudioCapability:
     return ooAddRemoteAudioCapability(call, cap->u.transmitAudioCapability,
                                       OOTX);
   case T_H245Capability_receiveAndTransmitAudioCapability:
     return ooAddRemoteAudioCapability(call,
                             cap->u.receiveAndTransmitAudioCapability, OORXTX);
   default:
     OOTRACEDBGA3("Unsupported cap type encountered. Ignoring. (%s, %s)\n",
                   call->callType, call->callToken);
   }
   return OO_OK;
}

int ooAddRemoteAudioCapability(ooCallData *call, H245AudioCapability *audioCap,
                               int dir)
{
   int rxframes=0, txframes=0;
   ASN1USINT audioUnitSize=0;
   OOBOOL comfortNoise=FALSE, scrambled=FALSE;
 
   switch(audioCap->t)
   {
   case T_H245AudioCapability_g711Alaw64k:
      if(dir&OOTX) txframes = audioCap->u.g711Alaw64k;
      else if(dir&OORX) rxframes = audioCap->u.g711Alaw64k;
      else{
         txframes = audioCap->u.g711Alaw64k;
         rxframes = audioCap->u.g711Alaw64k;
      }
      return ooCapabilityAddSimpleCapability(call, OO_G711ALAW64K, txframes,
                                   rxframes, dir, NULL, NULL, NULL, NULL,TRUE);
   case T_H245AudioCapability_g711Alaw56k:
      if(dir&OOTX) txframes = audioCap->u.g711Alaw56k;
      else if(dir&OORX) rxframes = audioCap->u.g711Alaw56k;
      else{
         txframes = audioCap->u.g711Alaw56k;
         rxframes = audioCap->u.g711Alaw56k;
      }
      return ooCapabilityAddSimpleCapability(call, OO_G711ALAW56K, txframes,
                                  rxframes, dir, NULL, NULL, NULL, NULL, TRUE);
   case T_H245AudioCapability_g711Ulaw64k:
      if(dir&OOTX) txframes = audioCap->u.g711Ulaw64k;
      else if(dir&OORX) rxframes = audioCap->u.g711Ulaw64k;
      else{
         txframes = audioCap->u.g711Ulaw64k;
         rxframes = audioCap->u.g711Ulaw64k;
      }
      return ooCapabilityAddSimpleCapability(call, OO_G711ULAW64K, txframes,
                                  rxframes, dir, NULL, NULL, NULL, NULL, TRUE);
   case T_H245AudioCapability_g711Ulaw56k:
      if(dir&OOTX) txframes = audioCap->u.g711Ulaw56k;
      else if(dir&OORX) rxframes = audioCap->u.g711Ulaw56k;
      else{
         txframes = audioCap->u.g711Ulaw56k;
         rxframes = audioCap->u.g711Ulaw56k;
      }
      return ooCapabilityAddSimpleCapability(call, OO_G711ULAW56K, txframes,
                                  rxframes, dir, NULL, NULL, NULL, NULL, TRUE);
   case T_H245AudioCapability_g729:
      if(dir&OOTX) txframes = audioCap->u.g729;
      else if(dir&OORX) rxframes = audioCap->u.g729;
      else{
         txframes = audioCap->u.g729;
         rxframes = audioCap->u.g729;
      }
      return ooCapabilityAddSimpleCapability(call, OO_G729, txframes,
                                  rxframes, dir, NULL, NULL, NULL, NULL, TRUE);

   case T_H245AudioCapability_g729AnnexA:
      if(dir&OOTX) txframes = audioCap->u.g729AnnexA;
      else if(dir&OORX) rxframes = audioCap->u.g729AnnexA;
      else{
         txframes = audioCap->u.g729AnnexA;
         rxframes = audioCap->u.g729AnnexA;
      }
      return ooCapabilityAddSimpleCapability(call, OO_G729A, txframes,
                                  rxframes, dir, NULL, NULL, NULL, NULL, TRUE);

   case T_H245AudioCapability_gsmFullRate:
      return ooCapabilityAddGSMCapability(call, OO_GSMFULLRATE,
            (unsigned)(audioCap->u.gsmFullRate->audioUnitSize/OO_GSMFRAMESIZE),
                                        audioCap->u.gsmFullRate->comfortNoise,
                                        audioCap->u.gsmFullRate->scrambled,
                                        dir, NULL, NULL, NULL, NULL, TRUE);
   case T_H245AudioCapability_gsmHalfRate:
      return ooCapabilityAddGSMCapability(call, OO_GSMHALFRATE,
            (unsigned)(audioCap->u.gsmHalfRate->audioUnitSize/OO_GSMFRAMESIZE),
                                        audioCap->u.gsmHalfRate->comfortNoise,
                                        audioCap->u.gsmHalfRate->scrambled,
                                        dir, NULL, NULL, NULL, NULL, TRUE);
   case T_H245AudioCapability_gsmEnhancedFullRate:
      return ooCapabilityAddGSMCapability(call, OO_GSMENHANCEDFULLRATE,
   (unsigned)(audioCap->u.gsmEnhancedFullRate->audioUnitSize/OO_GSMFRAMESIZE),
                                audioCap->u.gsmEnhancedFullRate->comfortNoise,
                                audioCap->u.gsmEnhancedFullRate->scrambled,
                                dir, NULL, NULL, NULL, NULL, TRUE);

   default:
     OOTRACEDBGA1("Unsupported audio capability type\n");
  
   }

   return OO_OK;
}





int ooCapabilityUpdateJointCapabilities(ooCallData* call, H245Capability *cap)
{
   ooH323EpCapability * epCap = NULL, *cur = NULL;
   OOTRACEDBGC3("checking whether we need to add cap to joint capabilities"
                "(%s, %s)\n", call->callType, call->callToken);
           
   switch(cap->t)
   {
   case T_H245Capability_receiveAudioCapability:
      epCap= ooIsAudioDataTypeSupported(call, cap->u.receiveAudioCapability,
                                        OOTX);
      break;
   case T_H245Capability_transmitAudioCapability:
      epCap = ooIsAudioDataTypeSupported(call, cap->u.transmitAudioCapability,
                                        OOTX);
      break;
   case T_H245Capability_receiveAndTransmitAudioCapability:
      epCap = NULL;
   default:
     OOTRACEDBGA3("Unsupported cap type encountered. Ignoring. (%s, %s)\n",
                   call->callType, call->callToken);
   }

   if(epCap)
   {
      OOTRACEDBGC3("Adding cap to joint capabilities(%s, %s)\n",call->callType,
                   call->callToken);
      /* Note:we add jointCaps in remote endpoints preference order.*/
      if(call->jointCaps)
      {
         cur = call->jointCaps;
         while(cur->next) cur = cur->next;
         cur->next = epCap;
      }
      call->jointCaps = epCap;
      return OO_OK;
   }

   OOTRACEDBGC3("Not adding to joint capabilities. (%s, %s)\n", call->callType,
                call->callToken);
   return OO_FAILED;
}



