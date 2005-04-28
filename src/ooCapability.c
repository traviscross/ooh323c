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


int ooEnableDTMFRFC2833(int dynamicRTPPayloadType)
{
   gH323ep.dtmfmode |= OO_CAP_DTMF_RFC2833;
   /*Dynamic RTP payload type range is from 96 - 127 */
   if(dynamicRTPPayloadType >= 96 && dynamicRTPPayloadType <= 127)
      giDynamicRTPPayloadType = dynamicRTPPayloadType;
   OOTRACEINFO1("Enabled RFC2833 DTMF capability \n");
   return OO_OK;
}



int ooDisableDTMFRFC2833()
{
   gH323ep.dtmfmode ^= OO_CAP_DTMF_RFC2833;
   OOTRACEINFO1("Disabled RFC2833 DTMF capability\n");
   return OO_OK;
}
  
int ooAddG711Capability(int cap, int txframes,
                        int rxframes, int dir,
                        cb_StartReceiveChannel startReceiveChannel,
                        cb_StartTransmitChannel startTransmitChannel,
                        cb_StopReceiveChannel stopReceiveChannel,
                        cb_StopTransmitChannel stopTransmitChannel)
{
  return ooAddG711Capability_internal(NULL, cap, txframes, rxframes, dir,
         startReceiveChannel, startTransmitChannel, stopReceiveChannel,
         stopTransmitChannel);
}

int ooAddG711Capability_internal(ooCallData *call, int cap, int txframes,
                                 int rxframes, int dir,
                                 cb_StartReceiveChannel startReceiveChannel,
                                 cb_StartTransmitChannel startTransmitChannel,
                                 cb_StopReceiveChannel stopReceiveChannel,
                                 cb_StopTransmitChannel stopTransmitChannel)
{
   int iRet=0;
   ooH323EpCapability *epCap = NULL, *cur=NULL, *lHead=NULL;
   ooG711CapParams *params=NULL;  
   OOCTXT *pctxt=NULL;
   if(!call) pctxt = &gH323ep.ctxt;
   else pctxt = call->pctxt;

   epCap = (ooH323EpCapability*)ASN1MALLOC(pctxt,
                                            sizeof(ooH323EpCapability));
   params = (ooG711CapParams*) ASN1MALLOC(pctxt,
                                                    sizeof(ooG711CapParams));
   if(!epCap || !params)
   {
      OOTRACEERR1("Error: Failed to allocate memory for adding new end point "
                  "capability.\n");
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
      if(!gH323ep.myCaps)
         gH323ep.myCaps = epCap;
      else{
         cur = gH323ep.myCaps;
         while(cur->next) cur = cur->next;
         cur->next = epCap;
      }
      ooAppendCapToCapPrefs(NULL, cap);
      gH323ep.noOfCaps++;
     }else{/*Add as remote capability */
      if(!call->remoteCaps)
        call->remoteCaps = epCap;
      else{
         cur = call->remoteCaps;
         while(cur->next) cur = cur->next;
         cur->next = epCap;
      }
   }
        
   return OO_OK;
}


int ooAddGSMCapability(int cap, ASN1USINT audioUnitSize, ASN1BOOL comfortNoise,
                       ASN1BOOL scrambled, int dir,
                       cb_StartReceiveChannel startReceiveChannel,
                       cb_StartTransmitChannel startTransmitChannel,
                       cb_StopReceiveChannel stopReceiveChannel,
                       cb_StopTransmitChannel stopTransmitChannel)
{
  return ooAddGSMCapability_internal(NULL, cap, audioUnitSize, comfortNoise,
                                     scrambled, dir, startReceiveChannel,
                                     startTransmitChannel, stopReceiveChannel,
                                     stopTransmitChannel);
}

int ooAddGSMCapability_internal(ooCallData *call, int cap,
                                ASN1USINT audioUnitSize, ASN1BOOL comfortNoise,
                                ASN1BOOL scrambled, int dir,
                                cb_StartReceiveChannel startReceiveChannel,
                                cb_StartTransmitChannel startTransmitChannel,
                                cb_StopReceiveChannel stopReceiveChannel,
                                cb_StopTransmitChannel stopTransmitChannel)
{
  int iRet=0;
   ooH323EpCapability *epCap = NULL, *cur=NULL;
   H245GSMAudioCapability *params=NULL;  
   OOCTXT *pctxt = NULL;

   if(!call) pctxt = &gH323ep.ctxt;
   else pctxt = call->pctxt;

   epCap = (ooH323EpCapability*)ASN1MALLOC(pctxt,
                                            sizeof(ooH323EpCapability));
   params = (H245GSMAudioCapability*) ASN1MALLOC(pctxt,
                                               sizeof(H245GSMAudioCapability));
   if(!epCap || !params)
   {
      OOTRACEERR1("Error: Failed to allocate memory for adding new end point "
                  "capability.\n");
      return OO_FAILED;
   }


   params->audioUnitSize = audioUnitSize;
   params->comfortNoise = comfortNoise;
   params->scrambled = scrambled;
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
   }else{/* Add as remote capability */
      if(!call->remoteCaps)
         call->remoteCaps = epCap;
      else{
         cur = call->remoteCaps;
         while(cur->next) cur = cur->next;
         cur->next = epCap;
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
     return ooCreateG711Capability(epCap, pctxt, dir);
   case T_H245AudioCapability_g722_64k:
   case T_H245AudioCapability_g722_56k:
   case T_H245AudioCapability_g722_48k:
   case T_H245AudioCapability_g7231:
   case T_H245AudioCapability_g728:
   case T_H245AudioCapability_g729:
   case T_H245AudioCapability_g729AnnexA:
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
      pATECap = (H245AudioTelephonyEventCapability*)ASN1MALLOC(pctxt,
                                   sizeof(H245AudioTelephonyEventCapability));
      if(!pATECap)
      {
         OOTRACEERR1("Error:Failed to allocate memory to Audio Telephony Event"
                    "capability\n");
         return NULL;
      }
      memset(pATECap, 0, sizeof(H245AudioTelephonyEventCapability));
      pATECap->dynamicRTPPayloadType = giDynamicRTPPayloadType;
      events = (char*)ASN1MALLOC(pctxt, strlen("0-16")+1);
      if(!events)
      {
         OOTRACEERR1("Error: Failed to allocate memory for dtmf events\n");
         ASN1MEMFREEPTR(pctxt, pATECap);
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
      OOTRACEERR1("ERROR:Failed to allocate mem for GSMFullRate capability\n");
      return NULL;
   }
  
   pAudio->t = T_H245AudioCapability_gsmFullRate;
   pAudio->u.gsmFullRate = pGSMCap;
   pGSMCap->audioUnitSize = ((H245GSMAudioCapability*)epCap->params)->audioUnitSize;
   pGSMCap->comfortNoise = ((H245GSMAudioCapability*)epCap->params)->comfortNoise;
   pGSMCap->scrambled = ((H245GSMAudioCapability*)epCap->params)->scrambled;

   return pAudio;
}

struct H245AudioCapability* ooCreateG711Capability
(ooH323EpCapability *epCap, OOCTXT* pctxt, int dir)
{
   H245AudioCapability *pAudio=NULL;
   ooG711CapParams *params;
   if(!epCap || !epCap->params)
   {
     OOTRACEERR1("Error:Invalid capability parameters to "
                 "ooCreateG711Capability.\n");
     return NULL;
   }
   params =(ooG711CapParams*)epCap->params;
   pAudio = (H245AudioCapability*)ASN1MALLOC(pctxt,
                                                sizeof(H245AudioCapability));
   if(!pAudio)
   {
      OOTRACEERR1("ERROR:Failed to allocate mem for g711 capability\n");
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
   default:
      OOTRACEERR2("ERROR: Don't know how to create audio capability %d\n",
                   epCap->cap);
   }
   return NULL;
}



ASN1BOOL ooCheckCompatibility_1
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
   default:
      return FALSE;
   }

   /* can we receive this capability */
   if(dir & OORX)
   {
      if(((ooG711CapParams*)epCap->params)->rxframes >= noofframes)
         return TRUE;
   }

   /* Can we transmit compatible stream */
   if(dir & OOTX)
   {
      if(((ooG711CapParams*)epCap->params)->txframes <= noofframes)
         return TRUE;
   }
   return FALSE; 
}

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
     if(((ooG711CapParams*)txCap->params)->txframes <=
                                ((ooG711CapParams*)rxCap->params)->rxframes)
       return TRUE;
     else{
       OOTRACEDBGA3("G711 caps are not compatible. (%s, %s)\n", call->callType,
                     call->callToken);
       return FALSE;
     }
   case OO_GSMFULLRATE:
   case OO_GSMHALFRATE:
   case OO_GSMENHANCEDFULLRATE:
   default:
     OOTRACEWARN3("WARN: Unsupported capabilities being compared. (%s, %s)\n",
                   call->callType, call->callToken);
   }
   return FALSE;

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

ooH323EpCapability* ooIsAudioDataTypeSupported
   (ooCallData *call, H245AudioCapability* audioCap, int dir)
{
   int cap, noofframes=0;
   ooH323EpCapability *epCap=NULL;

   /* Find similar capability */
   switch(audioCap->t)
   {
      case T_H245AudioCapability_g711Alaw64k:
         noofframes = audioCap->u.g711Alaw64k;
         cap = OO_G711ALAW64K;
         break;
      case T_H245AudioCapability_g711Alaw56k:
         noofframes = audioCap->u.g711Alaw56k;
         cap = OO_G711ALAW56K;
         break;
      case T_H245AudioCapability_g711Ulaw56k:
         noofframes = audioCap->u.g711Ulaw56k;
         cap = OO_G711ULAW56K;
         break;
      case T_H245AudioCapability_g711Ulaw64k:
         noofframes = audioCap->u.g711Ulaw64k;
         cap = OO_G711ULAW64K;
         break;
      default:
        return NULL;
   }

   OOTRACEDBGC4("Determined audio data type to be of type %d. Searching"
                " for matching capability.(%s, %s)\n", cap, call->callType,
                call->callToken);
  
   epCap = gH323ep.myCaps;
   while(epCap)
   {
      OOTRACEDBGC4("Local cap being compared %d. (%s, %s)\n", epCap->cap,
                     call->callType, call->callToken);
     
      if(epCap->cap == cap)
         break;
      epCap = epCap->next;
   }
  
   if(!epCap) return NULL;
  
   OOTRACEDBGC4("Found matching audio capability type %d. Comparing"
                " other parameters. (%s, %s)\n", cap, call->callType,
                call->callToken);
  
   /* can we receive this capability */
   if(dir & OORX)
   {
      if(((ooG711CapParams*)epCap->params)->rxframes >= noofframes)
         return epCap;
      else
         return NULL;
   }

   /* Can we transmit compatible stream */
   if(dir & OOTX)
   {
      if(((ooG711CapParams*)epCap->params)->txframes <= noofframes)
         return epCap;
      else
         return NULL;
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
   ASN1BOOL comfortNoise=FALSE, scrambled=FALSE;
 
   switch(audioCap->t)
   {
   case T_H245AudioCapability_g711Alaw64k:
      if(dir&OOTX) txframes = audioCap->u.g711Alaw64k;
      else if(dir&OORX) rxframes = audioCap->u.g711Alaw64k;
      else{
         txframes = audioCap->u.g711Alaw64k;
         rxframes = audioCap->u.g711Alaw64k;
      }
      return ooAddG711Capability_internal(call, OO_G711ALAW64K, txframes,
                                        rxframes, dir, NULL, NULL, NULL, NULL);
   case T_H245AudioCapability_g711Alaw56k:
      if(dir&OOTX) txframes = audioCap->u.g711Alaw56k;
      else if(dir&OORX) rxframes = audioCap->u.g711Alaw56k;
      else{
         txframes = audioCap->u.g711Alaw56k;
         rxframes = audioCap->u.g711Alaw56k;
      }
      return ooAddG711Capability_internal(call, OO_G711ALAW56K, txframes,
                                        rxframes, dir, NULL, NULL, NULL, NULL);
   case T_H245AudioCapability_g711Ulaw64k:
      if(dir&OOTX) txframes = audioCap->u.g711Ulaw64k;
      else if(dir&OORX) rxframes = audioCap->u.g711Ulaw64k;
      else{
         txframes = audioCap->u.g711Ulaw64k;
         rxframes = audioCap->u.g711Ulaw64k;
      }
      return ooAddG711Capability_internal(call, OO_G711ULAW64K, txframes,
                                        rxframes, dir, NULL, NULL, NULL, NULL);
   case T_H245AudioCapability_g711Ulaw56k:
      if(dir&OOTX) txframes = audioCap->u.g711Ulaw56k;
      else if(dir&OORX) rxframes = audioCap->u.g711Ulaw56k;
      else{
         txframes = audioCap->u.g711Ulaw56k;
         rxframes = audioCap->u.g711Ulaw56k;
      }
      return ooAddG711Capability_internal(call, OO_G711ULAW56K, txframes,
                                        rxframes, dir, NULL, NULL, NULL, NULL);
   case T_H245AudioCapability_gsmFullRate:
      return ooAddGSMCapability_internal(call, OO_GSMFULLRATE,
                                        audioCap->u.gsmFullRate->audioUnitSize,
                                        audioCap->u.gsmFullRate->comfortNoise,
                                        audioCap->u.gsmFullRate->scrambled,
                                        dir, NULL, NULL, NULL, NULL);
   case T_H245AudioCapability_gsmHalfRate:
      return ooAddGSMCapability_internal(call, OO_GSMHALFRATE,
                                        audioCap->u.gsmHalfRate->audioUnitSize,
                                        audioCap->u.gsmHalfRate->comfortNoise,
                                        audioCap->u.gsmHalfRate->scrambled,
                                        dir, NULL, NULL, NULL, NULL);
   case T_H245AudioCapability_gsmEnhancedFullRate:
      return ooAddGSMCapability_internal(call, OO_GSMENHANCEDFULLRATE,
                                audioCap->u.gsmEnhancedFullRate->audioUnitSize,
                                audioCap->u.gsmEnhancedFullRate->comfortNoise,
                                audioCap->u.gsmEnhancedFullRate->scrambled,
                                dir, NULL, NULL, NULL, NULL);

   default:
     OOTRACEDBGA1("Unsupported audio capability type\n");
  
   }

   return OO_OK;
}
