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
#include "oo.h"
#include "ooCalls.h"

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
   ooH323EpCapability *epCap = NULL;
   H245AudioCapability *pAudioCap = NULL;

   epCap = (ooH323EpCapability*)ASN1MALLOC(&gH323ep.ctxt,
                                            sizeof(ooH323EpCapability));
   if(!epCap)
   {
      OOTRACEERR1("Error: Failed to allocate memory for adding new end point "
                  "capability.\n");
      return OO_FAILED;
   }
   memset(epCap, 0, sizeof(ooH323EpCapability));
  
   epCap->t = dir;
   if(epCap->t == T_H245Capability_receiveAudioCapability)
   {
      OOTRACEINFO1("Adding receiveAudioCapability of type ");
   }
   else if(epCap->t == T_H245Capability_transmitAudioCapability)
   {
      OOTRACEINFO1("Adding transmitAudioCapability of type ");
   }
   else if(epCap->t == T_H245Capability_receiveAndTransmitAudioCapability)
   {
      OOTRACEINFO1("Adding receiveAndTransmitAudioCapability of type ");
   }
   else
   {
      OOTRACEERR1("ERROR: Can not add audio capability. Unknown direction\n");
      return OO_FAILED;
   }

   pAudioCap = (H245AudioCapability*) ASN1MALLOC(&gH323ep.ctxt,
                                                 sizeof(H245AudioCapability));

   if(!pAudioCap)
   {
      OOTRACEERR1("Error:Failed to allocate memory for new audio capability\n");
      ASN1MEMFREEPTR(&gH323ep.ctxt, epCap);
      return OO_FAILED;
   }
   memset(pAudioCap, 0, sizeof(H245AudioCapability));  
   epCap->cap = (void*)pAudioCap;
   epCap->startReceiveChannel = startReceiveChannel;
   epCap->startTransmitChannel = startTransmitChannel;
   epCap->stopReceiveChannel = stopReceiveChannel;
   epCap->stopTransmitChannel = stopTransmitChannel;

   switch(audioCap.t)
   {
   case T_H245AudioCapability_g711Ulaw64k:
      OOTRACEINFO1("G711 U-Law 64k\n");
      pAudioCap->t = T_H245AudioCapability_g711Ulaw64k;
      pAudioCap->u.g711Ulaw64k = audioCap.u.g711Ulaw64k;;
      break;
   case T_H245AudioCapability_g711Ulaw56k:
      OOTRACEINFO1("G711 U-Law 56k\n");
      pAudioCap->t = T_H245AudioCapability_g711Ulaw56k;
      pAudioCap->u.g711Ulaw56k = audioCap.u.g711Ulaw56k;;
      break;
   case T_H245AudioCapability_g711Alaw64k:
      OOTRACEINFO1("G711 A-Law 64k\n");
      pAudioCap->t = T_H245AudioCapability_g711Alaw64k;
      pAudioCap->u.g711Alaw64k = audioCap.u.g711Alaw64k;
      break;
   case T_H245AudioCapability_g711Alaw56k:
      OOTRACEINFO1("G711 A-Law 56k\n");
      pAudioCap->t = T_H245AudioCapability_g711Alaw56k;
      pAudioCap->u.g711Alaw64k = audioCap.u.g711Alaw56k;
      break;
   case T_H245AudioCapability_g729:
      OOTRACEINFO1("G729\n");
      pAudioCap->t = T_H245AudioCapability_g729;
      pAudioCap->u.g729 = audioCap.u.g729;
      break;
   case T_H245AudioCapability_g729AnnexA:
      OOTRACEINFO1("G729 AnnexA\n");
      pAudioCap->t = T_H245AudioCapability_g729AnnexA;
      pAudioCap->u.g729AnnexA = audioCap.u.g729AnnexA;
      break;
   case T_H245AudioCapability_gsmFullRate:
      OOTRACEINFO1("GSM Full Rate\n");
      pAudioCap->t = T_H245AudioCapability_gsmFullRate;
      pAudioCap->u.gsmFullRate = (H245GSMAudioCapability*)ASN1MALLOC(&gH323ep.ctxt,
                                         sizeof(H245GSMAudioCapability));
      if(!pAudioCap->u.gsmFullRate)
      {
         OOTRACEERR1("ERROR:Failed to allocate memory for adding gsmFullRate cap");
         return OO_FAILED;
      }
      pAudioCap->u.gsmFullRate->audioUnitSize = audioCap.u.gsmFullRate->audioUnitSize;
      pAudioCap->u.gsmFullRate->comfortNoise = audioCap.u.gsmFullRate->comfortNoise;
      pAudioCap->u.gsmFullRate->scrambled = audioCap.u.gsmFullRate->scrambled;
      dListInit(&pAudioCap->u.gsmFullRate->extElem1);
      break;
   case T_H245AudioCapability_gsmHalfRate:
      OOTRACEINFO1("GSM Half Rate\n");
      pAudioCap->t = T_H245AudioCapability_gsmHalfRate;
      pAudioCap->u.gsmHalfRate = (H245GSMAudioCapability*)ASN1MALLOC(&gH323ep.ctxt,
                                         sizeof(H245GSMAudioCapability));
      if(!pAudioCap->u.gsmHalfRate)
      {
         OOTRACEERR1("ERROR:Failed to allocate memory for adding gsmFullRate cap");
         return OO_FAILED;
      }
      pAudioCap->u.gsmHalfRate->audioUnitSize = audioCap.u.gsmHalfRate->audioUnitSize;
      pAudioCap->u.gsmHalfRate->comfortNoise = audioCap.u.gsmHalfRate->comfortNoise;
      pAudioCap->u.gsmHalfRate->scrambled = audioCap.u.gsmHalfRate->scrambled;
      dListInit(&pAudioCap->u.gsmHalfRate->extElem1);
   case T_H245AudioCapability_gsmEnhancedFullRate:
      OOTRACEINFO1("GSM Enhanced Full Rate\n");
      pAudioCap->t = T_H245AudioCapability_gsmEnhancedFullRate;
      pAudioCap->u.gsmEnhancedFullRate = (H245GSMAudioCapability*)ASN1MALLOC(&gH323ep.ctxt,
                                         sizeof(H245GSMAudioCapability));
      if(!pAudioCap->u.gsmEnhancedFullRate)
      {
         OOTRACEERR1("ERROR:Failed to allocate memory for adding gsmFullRate cap");
         return OO_FAILED;
      }
      pAudioCap->u.gsmEnhancedFullRate->audioUnitSize = audioCap.u.gsmEnhancedFullRate->audioUnitSize;
      pAudioCap->u.gsmEnhancedFullRate->comfortNoise = audioCap.u.gsmEnhancedFullRate->comfortNoise;
      pAudioCap->u.gsmEnhancedFullRate->scrambled = audioCap.u.gsmEnhancedFullRate->scrambled;
      dListInit(&pAudioCap->u.gsmEnhancedFullRate->extElem1);
   default:
      OOTRACEERR1("ERROR:Capability type not supported yet\n");
      return OO_FAILED;
   }

   dListAppend(&(gH323ep.ctxt), &(gH323ep.audioCaps), epCap);
   return OO_OK;
}


ooH323EpCapability* ooIsAudioCapSupported
   (ooCallData *call, H245AudioCapability* audioCap, int dir)
{
   int i=0;
   ooH323EpCapability *epCap=NULL;
   H245AudioCapability *lAudioCap;
   DListNode *curNode = NULL;
   int found = 0;

   for(i=0; i< (int)gH323ep.audioCaps.count; i++)
   {
      /* Retrieve local capability */
      curNode = dListFindByIndex (&gH323ep.audioCaps, i) ;
      epCap = (ooH323EpCapability*)curNode->data;
      lAudioCap = (H245AudioCapability*) epCap->cap;
      if((epCap->t != dir) ||
         (lAudioCap->t != audioCap->t))
         continue;
     

      switch(audioCap->t)
      {
      case T_H245AudioCapability_g711Ulaw64k:
      case T_H245AudioCapability_g711Alaw64k:
      case T_H245AudioCapability_g711Alaw56k:
      case T_H245AudioCapability_g711Ulaw56k:
         found = ooCompareG711Caps(lAudioCap, audioCap);
         break;
      case T_H245AudioCapability_nonStandard:
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
         OOTRACEERR3("Error:Unsupported Capability type in "
                     "ooIsAudioCapSupported. (%s, %s)\n",
                      call->callType, call->callToken);
         break;
      case T_H245AudioCapability_gsmFullRate:
      case T_H245AudioCapability_gsmHalfRate:
      case T_H245AudioCapability_gsmEnhancedFullRate:
         found = ooCompareGSMCaps(lAudioCap, audioCap);
         break;
      case T_H245AudioCapability_genericAudioCapability:
      case T_H245AudioCapability_g729Extensions:
      case T_H245AudioCapability_vbd:
      case T_H245AudioCapability_audioTelephonyEvent:
      case T_H245AudioCapability_audioTone:
      case T_H245AudioCapability_extElem1:
      default:;
      }
      if(found)
         break;
   }
   if(found) return epCap;
   else return NULL;

}

int ooCompareAudioCaps
   (ooCallData *call, H245AudioCapability * cap1, H245AudioCapability *cap2)
{
   if(cap1->t != cap2->t)
      return 0;

   switch(cap1->t)
   {
   case T_H245AudioCapability_g711Ulaw64k:
   case T_H245AudioCapability_g711Alaw64k:
   case T_H245AudioCapability_g711Alaw56k:
   case T_H245AudioCapability_g711Ulaw56k:
      return ooCompareG711Caps(cap1, cap2);
     
   case T_H245AudioCapability_nonStandard:
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
      OOTRACEERR3("Error:Unsupported Capability type in "
                  "ooIsAudioCapSupported. (%s, %s)\n",
                   call->callType, call->callToken);
      return 0;
   case T_H245AudioCapability_gsmFullRate:
   case T_H245AudioCapability_gsmHalfRate:
   case T_H245AudioCapability_gsmEnhancedFullRate:
      return ooCompareGSMCaps(cap1, cap2);
        
   case T_H245AudioCapability_genericAudioCapability:
   case T_H245AudioCapability_g729Extensions:
   case T_H245AudioCapability_vbd:
   case T_H245AudioCapability_audioTelephonyEvent:
   case T_H245AudioCapability_audioTone:
   case T_H245AudioCapability_extElem1:
   default:;
   }
   return 0;
}

int ooCompareG711Caps(H245AudioCapability * cap1, H245AudioCapability *cap2)
{
   OOTRACEINFO1("Comparing G711 Capabilities.\n");
   if(cap1->t != cap2->t)
      return 0;

   /* Note we use '>=', because an endpoint should support any number of
      frames per packet as long as the number is less than the max allowed in the
      capability.
   */
   switch(cap1->t)
   {
   case T_H245AudioCapability_g711Alaw64k:
      if(cap1->u.g711Alaw64k >= cap2->u.g711Alaw64k)
         return 1;
      break;
   case T_H245AudioCapability_g711Alaw56k:
      if(cap1->u.g711Alaw56k >= cap2->u.g711Alaw56k)
         return 1;
      break;
   case T_H245AudioCapability_g711Ulaw64k:
      if(cap1->u.g711Ulaw64k >= cap2->u.g711Ulaw64k)
         return 1;
      break;
   case T_H245AudioCapability_g711Ulaw56k:
      if(cap1->u.g711Ulaw56k >= cap2->u.g711Ulaw56k)
         return 1;
   default:
      OOTRACEERR1("Error:Invalid G711 Audio capability type\n");
   }
   return 0;
}

int ooCompareGSMCaps(H245AudioCapability *cap1, H245AudioCapability *cap2)
{
   H245GSMAudioCapability *gsm1=NULL, *gsm2=NULL;
  
   if(cap1->t != cap2->t)
      return 0;
  
   switch(cap1->t)
   {
   case T_H245AudioCapability_gsmFullRate:
      gsm1 = cap1->u.gsmFullRate;
      gsm2 = cap2->u.gsmFullRate;
      break;
   case T_H245AudioCapability_gsmHalfRate:
      gsm1 = cap1->u.gsmHalfRate;
      gsm2 = cap2->u.gsmHalfRate;
      break;
   case T_H245AudioCapability_gsmEnhancedFullRate:
      gsm1 = cap1->u.gsmEnhancedFullRate;
      gsm2 = cap2->u.gsmEnhancedFullRate;
   default:
      return 0;
   }
   if(gsm1 && gsm2)
   {
      if(gsm1->audioUnitSize == gsm2->audioUnitSize &&
         gsm1->comfortNoise  == gsm2->comfortNoise  &&
         gsm1->scrambled     == gsm2->scrambled )
         return 1;
   }
   return 0;
}

H245AudioCapability * ooCreateDupAudioCap
   (ooCallData *call, H245AudioCapability *audioCap, OOCTXT *pctxt)
{
   H245AudioCapability *pAudioCap = NULL;
   pAudioCap = (H245AudioCapability*)ASN1MALLOC(pctxt, sizeof(H245AudioCapability));
   if(!pAudioCap)
   {
      OOTRACEERR3("ERROR:Failed to allocate memory for duplicate audio "
                  "capability. (%s, %s)\n", call->callType, call->callToken);
      return NULL;
   }
   memset(pAudioCap, 0, sizeof(H245AudioCapability));
   switch(audioCap->t)
   {
   case T_H245AudioCapability_g711Ulaw64k:
      pAudioCap->t = audioCap->t;
      pAudioCap->u.g711Ulaw64k = audioCap->u.g711Ulaw64k;
      break;
   case T_H245AudioCapability_gsmFullRate:
      pAudioCap->t = audioCap->t;
      pAudioCap->u.gsmFullRate = ooCreateDupGSMAudioCap(call,
                                 audioCap->u.gsmFullRate, pctxt);
      if(!pAudioCap->u.gsmFullRate)
      {
         OOTRACEERR3("Error:Failed to create duplicate audio capability. "
                     "(%s, %s)\n", call->callType, call->callToken);
         ASN1MEMFREEPTR(pctxt, pAudioCap);
         return NULL;
      }
      break;
   case T_H245AudioCapability_gsmHalfRate:
      pAudioCap->t = audioCap->t;
      pAudioCap->u.gsmHalfRate = ooCreateDupGSMAudioCap(call,
                                 audioCap->u.gsmHalfRate, pctxt);
      if(!pAudioCap->u.gsmHalfRate)
      {
         OOTRACEERR3("Error:Failed to create duplicate audio capability. "
                     "(%s, %s)\n", call->callType, call->callToken);
         ASN1MEMFREEPTR(pctxt, pAudioCap);
         return NULL;
      }
      break;
   case T_H245AudioCapability_gsmEnhancedFullRate:
      pAudioCap->t = audioCap->t;
      pAudioCap->u.gsmEnhancedFullRate = ooCreateDupGSMAudioCap(call,
                                 audioCap->u.gsmEnhancedFullRate, pctxt);
      if(!pAudioCap->u.gsmEnhancedFullRate)
      {
         OOTRACEERR3("Error:Failed to create duplicate audio capability. "
                     "(%s, %s)\n", call->callType, call->callToken);
         ASN1MEMFREEPTR(pctxt, pAudioCap);
         return NULL;
      }
      break;
   case T_H245AudioCapability_nonStandard:
   case T_H245AudioCapability_g711Alaw64k:
   case T_H245AudioCapability_g711Alaw56k:
   case T_H245AudioCapability_g711Ulaw56k:
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
   case T_H245AudioCapability_genericAudioCapability:
   case T_H245AudioCapability_g729Extensions:
   case T_H245AudioCapability_vbd:
   case T_H245AudioCapability_audioTelephonyEvent:
   case T_H245AudioCapability_audioTone:
   case T_H245AudioCapability_extElem1:
   default:
      OOTRACEERR3("Error:Unsupported audio capability type. "
                  "Can't create duplicate. (%s, %s)\n", call->callType,
                   call->callToken);
      ASN1MEMFREEPTR(pctxt, pAudioCap);
      return NULL;
   }
   return pAudioCap;
}

H245GSMAudioCapability* ooCreateDupGSMAudioCap
   (ooCallData *call, H245GSMAudioCapability *gsmCap, OOCTXT *pctxt)
{
   H245GSMAudioCapability *pGSMCap = NULL;
   if(!gsmCap)
   {
      OOTRACEERR3("Error:Invalid gsm capability passed to "
                  "ooCreateDupGSMAudioCap. (%s, %s)\n", call->callType,
                   call->callToken);
      return NULL;
   }
   pGSMCap = (H245GSMAudioCapability*) ASN1MALLOC(pctxt,
                                             sizeof(H245GSMAudioCapability));
   if(!pGSMCap)
   {
      OOTRACEERR3("Error:Failed to allocate memory for duplicate gsm "
                  "capability. (%s, %s)\n", call->callType, call->callToken);
      return NULL;
   }
   memset(pGSMCap, 0, sizeof(H245GSMAudioCapability));
  
   pGSMCap->audioUnitSize = gsmCap->audioUnitSize;
   pGSMCap->comfortNoise  = gsmCap->comfortNoise;
   pGSMCap->scrambled     = gsmCap->scrambled;
   dListInit(&pGSMCap->extElem1); /* Extension elements not supported */
   return pGSMCap;

}

ooH323EpCapability* ooIsAudioDataTypeSupported(ooCallData* call,
                                    H245AudioCapability* audioData, int dir)
{
   return ooIsAudioCapSupported(call, audioData, dir);
}

ooH323EpCapability* ooIsApplicationDataTypeSupported(ooCallData* call,
                                  H245DataApplicationCapability *data, int dir)
{
   return NULL;
}

ooH323EpCapability* ooIsEncryptedDataTypeSupported(ooCallData* call,
                                   H245EncryptionMode *encryptionData, int dir)
{
   return NULL;
}

ooH323EpCapability* ooIsH235ControlDataTypeSupported(ooCallData* call,
                                H245NonStandardParameter *h235Control, int dir)
{
   return NULL;
}

ooH323EpCapability* ooIsH235MediaDataTypeSupported(ooCallData* call,
                                        H245H235Media *h235Media, int dir)
{
   return NULL;
}

ooH323EpCapability* ooIsMultiplexedStreamDataTypeSupported(ooCallData* call,
                    H245MultiplexedStreamParameter *multiplexedStream, int dir)
{
   return NULL;
}

ooH323EpCapability* ooIsDataTypeSupported(ooCallData *call, H245DataType *data, int dir)
{
   switch(data->t)
   {
   case T_H245DataType_nonStandard:
      return NULL;
   case T_H245DataType_nullData:
      return NULL;
   case T_H245DataType_videoData:
      return NULL;
   case T_H245DataType_audioData:
      return ooIsAudioDataTypeSupported(call, data->u.audioData, dir);
   case T_H245DataType_data:
      return ooIsApplicationDataTypeSupported(call, data->u.data, dir);
   case T_H245DataType_encryptionData:
      return ooIsEncryptedDataTypeSupported(call, data->u.encryptionData, dir);
   case T_H245DataType_h235Control:
      return ooIsH235ControlDataTypeSupported(call, data->u.h235Control, dir);
   case T_H245DataType_h235Media:
      return ooIsH235MediaDataTypeSupported(call, data->u.h235Media, dir);
   case T_H245DataType_multiplexedStream:
      return ooIsMultiplexedStreamDataTypeSupported(call, data->u.multiplexedStream, dir);
   default:
      OOTRACEINFO3("Unknown data type (%s, %s)\n", call->callType, call->callToken);
   }
   return NULL;
}

