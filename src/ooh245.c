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

#include "ooh245.h"
#include "ooCalls.h"
#include "printHandler.h"
#include "ooh323ep.h"

#ifdef _WIN32
#include <stdlib.h>
#include <time.h>
#endif
static ASN1OBJID gh245ProtocolID = {
  6, { 0, 0, 8, 245, 0, 8 }
};

int ooCreateH245Message(H245Message **pph245msg, int type)
{
   OOCTXT* pctxt = NULL;
   pctxt = (OOCTXT*)newContext();
   if(!pctxt)
   {
      OOTRACEERR1("ERROR: Failed to allocate OOCTXT for "
                         "H245 message creation\n");
      return OO_FAILED;
   }
  

   *pph245msg = (H245Message*)ASN1MALLOC(pctxt,sizeof(H245Message));
   if(!(*pph245msg))
   {
      OOTRACEERR1("ERROR:Failed to allocate memory for h245 message\n");
      freeContext(pctxt);
      ASN1CRTFREE0(pctxt);
      return OO_FAILED;
   }
   else
   {
      memset(*pph245msg, 0, sizeof(H245Message));
      (*pph245msg)->pctxt = pctxt;
      (*pph245msg)->h245Msg.t = type;
      switch(type)
      {
         case  T_H245MultimediaSystemControlMessage_request:
            (*pph245msg)->h245Msg.u.request =
                                     (H245RequestMessage*)ASN1MALLOC(pctxt,
                                     sizeof(H245RequestMessage));
            /*Check for successful mem allocation, and if successful initialize
              mem to zero*/
            if(!(*pph245msg)->h245Msg.u.request)
            {
               OOTRACEERR1("ERROR:Memory allocation for H.245 request"
                                     " message failed\n");
               freeContext(pctxt);
               ASN1CRTFREE0(pctxt);
               return OO_FAILED;
            }
            else
               memset((*pph245msg)->h245Msg.u.request, 0,
                                              sizeof(H245RequestMessage));
            break;
         case T_H245MultimediaSystemControlMessage_response:
            (*pph245msg)->h245Msg.u.response =
                                     (H245ResponseMessage*)ASN1MALLOC(pctxt,
                                     sizeof(H245ResponseMessage));
            /*Check for successful mem allocation, and if successful initialize
              mem to zero*/
            if(!(*pph245msg)->h245Msg.u.response)
            {
               OOTRACEERR1("ERROR:Memory allocation for H.245 response"
                                     " message failed\n");
               freeContext(pctxt);
               ASN1CRTFREE0(pctxt);
               return OO_FAILED;
            }
            else
               memset((*pph245msg)->h245Msg.u.response, 0,
                                          sizeof(H245ResponseMessage));
            break;
         case T_H245MultimediaSystemControlMessage_command:
            (*pph245msg)->h245Msg.u.command =
                                     (H245CommandMessage*)ASN1MALLOC(pctxt,
                                     sizeof(H245CommandMessage));
            /*Check for successful mem allocation, and if successful initialize
              mem to zero*/
            if(!(*pph245msg)->h245Msg.u.command)
            {
               OOTRACEERR1("ERROR:Memory allocation for H.245 command"
                                     " message failed\n");
               freeContext(pctxt);
               ASN1CRTFREE0(pctxt);
               return OO_FAILED;
            }
            else
               memset((*pph245msg)->h245Msg.u.command, 0,
                                       sizeof(H245CommandMessage));
            break;
         case T_H245MultimediaSystemControlMessage_indication:
            (*pph245msg)->h245Msg.u.indication =
                                     (H245IndicationMessage*)ASN1MALLOC(pctxt,
                                     sizeof(H245IndicationMessage));
            /*Check for successful mem allocation, and if successful initialize
              mem to zero*/
            if(!(*pph245msg)->h245Msg.u.indication == 0)
            {
               OOTRACEERR1("ERROR:Memory allocation for H.245 indication"
                                     " message failed\n");
               freeContext(pctxt);
               ASN1CRTFREE0(pctxt);
               return OO_FAILED;
            }
            else
               memset((*pph245msg)->h245Msg.u.indication, 0,
                                              sizeof(H245IndicationMessage));
            break;
         default:
            freeContext(pctxt);
            ASN1CRTFREE0(pctxt);
            OOTRACEERR1("ERROR: H245 message type not supported\n");
      }
      return OO_OK;
   }
}

int ooFreeH245Message(H245Message *pmsg)
{
   OOCTXT *pctxt;
   if(!pmsg)
   {
      pctxt = pmsg->pctxt;
      freeContext(pctxt);
      ASN1CRTFREE0(pctxt);
   }
 
   return OO_OK;
}

int ooGetOutgoingH245Msgbuf(ooCallData *call,
                            ASN1OCTET *msgbuf, int *len, int *msgType)
{
   H245Message *p_h245Msg=NULL;
   H245MultimediaSystemControlMessage *multimediaMsg=NULL;
   DListNode * p_msgNode=NULL;
   int i =0;
   ASN1BOOL aligned =TRUE, trace =FALSE;
   ASN1OCTET encodeBuf[1024];
   ASN1OCTET* encodeptr=NULL;
   int encodeLen=0;
   OOCTXT* pctxt=NULL;
   memset(encodeBuf, 0, sizeof(encodeBuf));

   if(call->outH245Queue.count == 0)
   {
      OOTRACEERR3("ERROR:No outgoing h245 message (%s, %s)\n", call->callType,
                   call->callToken);
      return OO_FAILED;
   }
   p_msgNode = call->outH245Queue.head;
   p_h245Msg = (H245Message*) p_msgNode->data;
   *msgType = p_h245Msg->msgType;
   multimediaMsg = &(p_h245Msg->h245Msg);
  
   /* Encode the Multimedia Control Message */
   pctxt = p_h245Msg->pctxt;
   setPERBuffer(pctxt, encodeBuf, sizeof(encodeBuf), aligned);

   if(asn1PE_H245MultimediaSystemControlMessage(pctxt, multimediaMsg)!=ASN_OK)
   {
     
      OOTRACEERR3("ERROR: H245 Message encoding failed (%s, %s)\n",
                   call->callType, call->callToken);
      /* Free memory associated with the message */
      freeContext(pctxt);
      ASN1CRTFREE0(pctxt);
      return OO_FAILED;
   }
  
   encodeptr = encodeGetMsgPtr(pctxt, &encodeLen);
  
   /* Populate message buffer to be returned */
   *len = encodeLen + 4;
   msgbuf[i++] = 3; /* TPKT version */
   msgbuf[i++] = 0; /* TPKT resevred */
   /* 1st octet of length, will be populated once len is determined */
   msgbuf[i++] = (*len>>8);
   /* 2nd octet of length, will be populated once len is determined */
   msgbuf[i++] = *len;
   memcpy((msgbuf + i), encodeptr, encodeLen);

   /* Remove the message from rtdlist outH245Queue */
   dListRemove(&(call->outH245Queue), p_msgNode);

   /* Free memory associated with the message */
   freeContext(pctxt);
   ASN1CRTFREE0(pctxt);
  
   if(p_msgNode)
      ASN1MEMFREEPTR(call->pctxt, p_msgNode);     

  
  return OO_OK;
}


int ooSendTermCapMsg(ooCallData *call)
{
   int ret;
   H245RequestMessage * request;
   OOCTXT *pctxt;
   ooH323EpCapability *epCap;
   H245TerminalCapabilitySet *termCap;
   H245AudioCapability *audioCap, *epAudioCap;
   H245CapabilityTableEntry *entry;
   H245AlternativeCapabilitySet *altSet;
   DListNode * curNode=NULL;
   H245CapabilityDescriptor *capDesc;
   H245Message *ph245msg=NULL;
   int i=0, j=0;

   ret = ooCreateH245Message(&ph245msg, 
                             T_H245MultimediaSystemControlMessage_request);

   if(ret == OO_FAILED)
   {
      OOTRACEERR3("Error:Failed to create H245 message for Terminal "
                  "CapabilitySet (%s, %s)\n", call->callType,call->callToken);
      return OO_FAILED;
   }

  /* Set request type as TerminalCapabilitySet */
   request = ph245msg->h245Msg.u.request;
   pctxt = ph245msg->pctxt;
  
   memset(request, 0, sizeof(H245RequestMessage));
   if(request == NULL)
   {
      OOTRACEERR3("ERROR: No memory allocated for request message (%s, %s)\n",
                   call->callType, call->callToken);
      return OO_FAILED;
   }
  
   request->t = T_H245RequestMessage_terminalCapabilitySet;
   request->u.terminalCapabilitySet = (H245TerminalCapabilitySet*)
                  ASN1MALLOC(pctxt, sizeof(H245TerminalCapabilitySet));
   termCap = request->u.terminalCapabilitySet;
   memset(termCap, 0, sizeof(H245TerminalCapabilitySet));
   termCap->m.multiplexCapabilityPresent = 0;
   termCap->m.capabilityTablePresent = 1;
   termCap->m.capabilityDescriptorsPresent = 1;
   termCap->sequenceNumber = ++(call->localTermCapSeqNo); 
   termCap->protocolIdentifier = gh245ProtocolID; /* protocol id */
#ifndef _COMPACT
   dListInit(&(termCap->extElem1));
#endif /* !_COMPACT */
   /* TODO:Right now we add all the endpoint capabilities  to the message.
           Need a way by which application can specify one set of capabilities
           for one call and another set for another call. (First step is do we need
           this??)*/
   /* Add audio Capabilities */

   dListInit(&(termCap->capabilityTable));
   for(i=0; i< (int)gH323ep.audioCaps.count; i++)
   {
      /* Retrieve local capability */
      curNode = dListFindByIndex (&gH323ep.audioCaps, i) ;
      epCap = (ooH323EpCapability*)curNode->data;
      epAudioCap = (H245AudioCapability*) epCap->cap;

      /* Allocate memory using msg OOCTXT and then copy local capability
         into it.
      */
      audioCap = (H245AudioCapability*)ASN1MALLOC(pctxt,
                                                  sizeof(H245AudioCapability));
      memset(audioCap, 0, sizeof(H245AudioCapability));
      ooCopyAudioCapability(epAudioCap, audioCap);

      /* Add  Capabilities to Capability Table */
      entry = (H245CapabilityTableEntry*) ASN1MALLOC(pctxt,
                      sizeof(H245CapabilityTableEntry));
      memset(entry, 0, sizeof(H245CapabilityTableEntry));
      entry->m.capabilityPresent = 1;

      entry->capability.t = epCap->t;
      if(epCap->t == T_H245Capability_receiveAudioCapability)
         entry->capability.u.receiveAudioCapability = audioCap;
      else if(epCap->t == T_H245Capability_transmitAudioCapability)
         entry->capability.u.transmitAudioCapability = audioCap;
      else if(epCap->t == T_H245Capability_receiveAndTransmitAudioCapability)
         entry->capability.u.receiveAndTransmitAudioCapability = audioCap;
     
      entry->capabilityTableEntryNumber = i+1;
      dListAppend(pctxt , &(termCap->capabilityTable), entry);
   }
   /*TODO:Add Video and Data capabilities, if required*/


   /* Define capability descriptior */
   capDesc = (H245CapabilityDescriptor*)
             ASN1MALLOC(pctxt, sizeof(H245CapabilityDescriptor));
   memset(capDesc, 0, sizeof(H245CapabilityDescriptor));
   capDesc->m.simultaneousCapabilitiesPresent = 1;
   capDesc->capabilityDescriptorNumber = 1;
   dListInit(&(capDesc->simultaneousCapabilities));
   /* Add Alternative Capability Set.
      TODO: Right now all capabilities are added in separate
            alternate capabilities set. Need a way for application
            developer to specify the alternative capability sets.
   */
   for(j=0; j<i; j++)
   {
      altSet = (H245AlternativeCapabilitySet*)
               ASN1MALLOC(pctxt, sizeof(H245AlternativeCapabilitySet));
      memset(altSet, 0, sizeof(H245AlternativeCapabilitySet));
      altSet->n = 1;
      altSet->elem[0] = j+1;
  
      dListAppend(pctxt, &(capDesc->simultaneousCapabilities), altSet);
   }

   dListInit(&(termCap->capabilityDescriptors));
   dListAppend(pctxt, &(termCap->capabilityDescriptors), capDesc);
  
   ooSendH245Msg(call, ph245msg);
   OOTRACEINFO3("Built terminal capability set message (%s, %s)\n",
                 call->callType, call->callToken);
   return OO_OK;
}


ASN1UINT ooGenerateStatusDeterminationNumber()
{
   ASN1UINT statusDeterminationNumber;

   srand((unsigned)time( NULL ) );
   statusDeterminationNumber = rand()%16777215;
   return statusDeterminationNumber;
}
/* TODO: Should Send MasterSlave Release when no response from
         Remote endpoint after MasterSlaveDetermination sent within
         timeout.
*/
int ooHandleMasterSlave(ooCallData *call, void * pmsg,
                          int msgType)
{
   H245MasterSlaveDetermination *masterSlave;
   H245MasterSlaveDeterminationAck *masterSlaveAck;
   ASN1UINT statusDeterminationNumber;

   switch(msgType)
   {
      case OOMasterSlaveDetermination:
         OOTRACEINFO3("Master Slave Determination received (%s, %s)\n",
                       call->callType, call->callToken);
        
         masterSlave = (H245MasterSlaveDetermination*)pmsg;
        
         if(masterSlave->terminalType < gH323ep.termType)
         {
            ooSendMasterSlaveDeterminationAck(call, "slave");
            call->masterSlaveState =  OO_MasterSlave_Master;
            OOTRACEINFO3("MasterSlaveDetermination done - Master(%s, %s)\n",
                             call->callType, call->callToken);
            return OO_OK;
         }
         if(masterSlave->terminalType > gH323ep.termType)
         {
            ooSendMasterSlaveDeterminationAck(call, "master");
            call->masterSlaveState =  OO_MasterSlave_Slave;
            OOTRACEINFO3("MasterSlaveDetermination done - Slave(%s, %s)\n",
                             call->callType, call->callToken);
            return OO_OK;
         }
         /* Since term types are same, master slave determination will
            be done based on statusdetermination number
         */
        
         OOTRACEDBGA3("Determining master-slave based on StatusDetermination"
                      "Number (%s, %s)\n", call->callType, call->callToken);
         if(call->masterSlaveState == OO_MasterSlave_DetermineSent)
            statusDeterminationNumber = call->statusDeterminationNumber;
         else
            statusDeterminationNumber = ooGenerateStatusDeterminationNumber();
        
                 if(masterSlave->statusDeterminationNumber <
                       statusDeterminationNumber)
         {
            ooSendMasterSlaveDeterminationAck(call, "slave");
            call->masterSlaveState =  OO_MasterSlave_Master;
            OOTRACEINFO3("MasterSlaveDetermination done - Master(%s, %s)\n",
                             call->callType, call->callToken);
            return OO_OK;
         }
         if(masterSlave->statusDeterminationNumber >
                         statusDeterminationNumber)
         {
            ooSendMasterSlaveDeterminationAck(call, "master");
            call->masterSlaveState =  OO_MasterSlave_Slave;
            OOTRACEINFO3("MasterSlaveDetermination done - Slave(%s, %s)\n",
                             call->callType, call->callToken);
            return OO_OK;
         }
                 if(masterSlave->statusDeterminationNumber ==
                         statusDeterminationNumber)
         {
         /* TODO: Should Send MasterSlaveDeterminationReject_cause IdenticalNumbers*/        
            OOTRACEERR3("ERROR:MasterSlaveDetermination failed- identical "
                        "numbers (%s, %s)\n", call->callType, call->callToken);
         }
         break;
      case OOMasterSlaveAck:
         masterSlaveAck = (H245MasterSlaveDeterminationAck*)pmsg;
         if(call->masterSlaveState == OO_MasterSlave_DetermineSent)
         {
            if(masterSlaveAck->decision.t ==
               T_H245MasterSlaveDeterminationAck_decision_master)
            {
               ooSendMasterSlaveDeterminationAck(call, "slave");
               call->masterSlaveState =  OO_MasterSlave_Master;
               OOTRACEINFO3("MasterSlaveDetermination done - Master(%s, %s)\n",
                             call->callType, call->callToken);
            }
            else
            {
               ooSendMasterSlaveDeterminationAck(call, "master");
               call->masterSlaveState = OO_MasterSlave_Slave;
               OOTRACEINFO3("MasterSlaveDetermination done - Slave(%s, %s)\n",
                             call->callType, call->callToken);
            }
         }
        
         if(call->capabilityExchangeState == OO_TermCapExchangeDone)
         {
            /*Since Cap exchange and MasterSlave Procedures are done */
            ooOpenLogicalChannels(call);
         }
                 else
            OOTRACEDBGC1("Not opening logical channels as Cap exchange remaining\n");

         break;
       default:
          OOTRACEWARN3("Warn:Unhandled Master Slave message received - %s - "
                       "%s\n", call->callType, call->callToken);
   }
   return OO_OK;     
}

int ooSendMasterSlaveDetermination(ooCallData *call)
{
   int ret;
   H245Message* ph245msg=NULL;
   H245RequestMessage *request;
   OOCTXT *pctxt=NULL;
   H245MasterSlaveDetermination* pMasterSlave;

   /* Check whether Master Slave Determination already in progress */
   if(call->masterSlaveState != OO_MasterSlave_Idle)
   {
      OOTRACEINFO3("MasterSlave determination already in progress (%s, %s)\n",
                   call->callType, call->callToken);
      return OO_FAILED;
   }

   ret = ooCreateH245Message(&ph245msg,
                   T_H245MultimediaSystemControlMessage_request);
   if(ret != OO_OK)
   {
      OOTRACEERR3("Error: creating H245 message - MasterSlave Determination "
                  "(%s, %s)\n", call->callType, call->callToken);
      return OO_FAILED;
   }
   ph245msg->msgType = OOMasterSlaveDetermination;
   request = ph245msg->h245Msg.u.request;
   request->t = T_H245RequestMessage_masterSlaveDetermination;
   request->u.masterSlaveDetermination = (H245MasterSlaveDetermination*)
            ASN1MALLOC(ph245msg->pctxt, sizeof(H245MasterSlaveDetermination));

  
   pMasterSlave = request->u.masterSlaveDetermination;
   memset(pMasterSlave, 0, sizeof(H245MasterSlaveDetermination));  
   pMasterSlave->terminalType = gH323ep.termType;
   pMasterSlave->statusDeterminationNumber =
                       ooGenerateStatusDeterminationNumber();
   call->statusDeterminationNumber = pMasterSlave->statusDeterminationNumber;
#ifndef _COMPACT
   dListInit(&(pMasterSlave->extElem1));
#endif /* !_COMPACT */  

   OOTRACEINFO3("Built MasterSlave Determination (%s, %s)\n", call->callType,
                 call->callToken);
   ooSendH245Msg(call, ph245msg);
   call->masterSlaveState = OO_MasterSlave_DetermineSent;
   return OO_OK;
}

int ooSendMasterSlaveDeterminationAck(ooCallData* call,
                                      char * status)
{
   int ret=0;
   H245ResponseMessage * response=NULL;
   H245Message *ph245msg=NULL;
   OOCTXT *pctxt=NULL;

   ret = ooCreateH245Message(&ph245msg,
                      T_H245MultimediaSystemControlMessage_response);
   if(ret != OO_OK)
   {
      OOTRACEERR3("Error:H245 message creation failed for - MasterSlave "
                  "Determination Ack (%s, %s)\n",call->callType, call->callToken);
      return OO_FAILED;
   }
   ph245msg->msgType = OOMasterSlaveAck;
   response = ph245msg->h245Msg.u.response;
   memset(response, 0, sizeof(H245ResponseMessage));
   pctxt = ph245msg->pctxt;
   response->t = T_H245ResponseMessage_masterSlaveDeterminationAck;
   response->u.masterSlaveDeterminationAck = (H245MasterSlaveDeterminationAck*)
                   ASN1MALLOC(pctxt, sizeof(H245MasterSlaveDeterminationAck));
   memset(response->u.masterSlaveDeterminationAck, 0,
                             sizeof(response->u.masterSlaveDeterminationAck));
   if(!strcmp("master", status))
      response->u.masterSlaveDeterminationAck->decision.t =
                         T_H245MasterSlaveDeterminationAck_decision_master;
   else
      response->u.masterSlaveDeterminationAck->decision.t =
                         T_H245MasterSlaveDeterminationAck_decision_slave;
  
#ifndef _COMPACT
   dListInit(&(response->u.masterSlaveDeterminationAck->extElem1));
#endif /* !_COMPACT */
   ooSendH245Msg(call, ph245msg);
   OOTRACEINFO3("Built MasterSlave determination Ack (%s, %s)\n", call->callType,
                 call->callToken);

   return OO_OK;
}

int ooHandleOpenLogicalChannel(ooCallData* call,
                                 H245OpenLogicalChannel *olc)
{

   H245OpenLogicalChannel_forwardLogicalChannelParameters *flcp =
    &(olc->forwardLogicalChannelParameters);

   /* Check whether channel type is supported. Only supported channel
      type for now is g711ulaw audio channel.
   */
   switch(flcp->dataType.t)
   {
   case T_H245DataType_nonStandard:
      OOTRACEWARN3("Warn:Media channel data type "
                   "'T_H245DataType_nonStandard' not supported (%s, %s)\n",
                   call->callType, call->callToken);
      break;
   case T_H245DataType_nullData:
      OOTRACEWARN3("Warn:Media channel data type "
                   "'T_H245DataType_nullData' not supported (%s, %s)\n",
                   call->callType, call->callToken);
      break;
   case T_H245DataType_videoData:
      OOTRACEWARN3("Warn:Media channel data type "
                   "'T_H245DataType_videoData' not supported (%s, %s)\n",
                   call->callType, call->callToken);
      break;
   case T_H245DataType_audioData:
      ooHandleOpenLogicalAudioChannel(call, olc);
      break;
   case T_H245DataType_data:
      OOTRACEWARN3("Warn:Media channel data type "
                   "'T_H245DataType_data' not supported (%s, %s)\n",
                   call->callType, call->callToken);
      break;
   case T_H245DataType_encryptionData:
      OOTRACEWARN3("Warn:Media channel data type "
                   "'T_H245DataType_encryptionData' not supported (%s, %s)\n",
                   call->callType, call->callToken);
      break;
   case T_H245DataType_h235Control:
      OOTRACEWARN3("Warn:Media channel data type "
                   "'T_H245DataType_h235Control' not supported (%s, %s)\n",
                   call->callType, call->callToken);
      break;
   case T_H245DataType_h235Media:
      OOTRACEWARN3("Warn:Media channel data type "
                   "'T_H245DataType_h235Media' not supported (%s, %s)\n",
                   call->callType, call->callToken);
      break;
   case T_H245DataType_multiplexedStream:
      OOTRACEWARN3("Warn:Media channel data type "
                   "'T_H245DataType_multiplexedStream' not supported (%s, %s)\n",
                   call->callType, call->callToken);
      break;
   case T_H245DataType_redundancyEncoding:
      OOTRACEWARN3("Warn:Media channel data type "
                   "'T_H245DataType_redundancyEncoding' not supported (%s, %s)\n",
                   call->callType, call->callToken);
      break;
   case T_H245DataType_multiplePayloadStream:
      OOTRACEWARN3("Warn:Media channel data type "
                   "'T_H245DataType_multiplePayloadStream' not supported (%s, %s)\n",
                   call->callType, call->callToken);
      break;
   case T_H245DataType_fec:
      OOTRACEWARN3("Warn:Media channel data type 'T_H245DataType_fec' not "
                   "supported (%s, %s)\n", call->callType, call->callToken);
      break;
   case T_H245DataType_extElem1:
      OOTRACEWARN3("Warn:Media channel data type 'T_H245DataType_extElem1' "
                   "not supported (%s, %s)\n", call->callType, call->callToken);
      break;
   default:
      OOTRACEERR3("ERROR:Unknown media channel data type (%s, %s)\n",
                   call->callType, call->callToken);
   }
  
   return OO_OK;
}      


int ooHandleOpenLogicalAudioChannel(ooCallData *call, H245OpenLogicalChannel*olc)
{
   int ret=0;
   H245Message *ph245msg=NULL;
   H245ResponseMessage *response;
   H245OpenLogicalChannelAck *olcAck;
   ooH323EpCapability *epCap=NULL;
   H245H2250LogicalChannelAckParameters *h2250lcap;
   OOCTXT *pctxt;
   H245UnicastAddress *unicastAddrs, *unicastAddrs1;
   H245UnicastAddress_iPAddress *iPAddress, *iPAddress1;
   char hexip[20];
   int addr_part1, addr_part2, addr_part3, addr_part4;

  
   H245OpenLogicalChannel_forwardLogicalChannelParameters *flcp =
    &(olc->forwardLogicalChannelParameters);
   if(call->noOfLogicalChannels>=5)
   {
      OOTRACEERR3("ERROR:Can not open more than 5 logical channels per call "
                  "(%s, %s)\n", call->callType, call->callToken);
      return OO_FAILED;
   }
   if(!(epCap=ooIsAudioCapabilitySupported(flcp->dataType.u.audioData->t, OO_RX_CAP)))
   {
      OOTRACEERR3("ERROR:OpenLogicalChannel - audio capability not supported "
                  "(%s, %s)\n", call->callType, call->callToken);
      return OO_FAILED;
   }
   /* Generate an Ack for the open channel request */
   ret = ooCreateH245Message(&ph245msg,T_H245MultimediaSystemControlMessage_response);
   if(ret != OO_OK)
   {
      OOTRACEERR3("Error: H245 message creation failed for - OpenLogicalChannel"
                  "Ack (%s, %s)\n", call->callType, call->callToken);
      return OO_FAILED;
   }

   ph245msg->msgType = OOOpenLogicalChannelAck;
   response = ph245msg->h245Msg.u.response;
   pctxt = ph245msg->pctxt;
   memset(response, 0, sizeof(H245ResponseMessage));
   response->t = T_H245ResponseMessage_openLogicalChannelAck;
   response->u.openLogicalChannelAck = (H245OpenLogicalChannelAck*)
                   ASN1MALLOC(pctxt, sizeof(H245OpenLogicalChannelAck));  
   olcAck = response->u.openLogicalChannelAck;
   memset(olcAck, 0, sizeof(H245OpenLogicalChannelAck));
   olcAck->forwardLogicalChannelNumber = olc->forwardLogicalChannelNumber;

#ifndef _COMPACT
   dListInit(&(olcAck->extElem1));
#endif /* !_COMPACT */

   olcAck->m.forwardMultiplexAckParametersPresent = 1;
   olcAck->forwardMultiplexAckParameters.t =
     T_H245OpenLogicalChannelAck_forwardMultiplexAckParameters_h2250LogicalChannelAckParameters;
   olcAck->forwardMultiplexAckParameters.u.h2250LogicalChannelAckParameters =
                      (H245H2250LogicalChannelAckParameters*)ASN1MALLOC(pctxt,
                      sizeof(H245H2250LogicalChannelAckParameters));
   h2250lcap =
      olcAck->forwardMultiplexAckParameters.u.h2250LogicalChannelAckParameters;
   memset(h2250lcap, 0, sizeof(H245H2250LogicalChannelAckParameters));

#ifndef _COMPACT
   dListInit(&(h2250lcap->extElem1));
#endif /* !_COMPACT */

   h2250lcap->m.mediaChannelPresent = 1;
   h2250lcap->m.mediaControlChannelPresent = 1;
   h2250lcap->m.sessionIDPresent = 1;
   h2250lcap->sessionID = 1;
  
   h2250lcap->mediaChannel.t =
                         T_H245TransportAddress_unicastAddress;
   h2250lcap->mediaChannel.u.unicastAddress =  (H245UnicastAddress*)
                         ASN1MALLOC(pctxt, sizeof(H245UnicastAddress));

   unicastAddrs = h2250lcap->mediaChannel.u.unicastAddress;
   memset(unicastAddrs, 0, sizeof(H245UnicastAddress));
   unicastAddrs->t = T_H245UnicastAddress_iPAddress;
   unicastAddrs->u.iPAddress = (H245UnicastAddress_iPAddress*)
               ASN1MALLOC(pctxt, sizeof(H245UnicastAddress_iPAddress));
   iPAddress = unicastAddrs->u.iPAddress;
   memset(iPAddress, 0, sizeof(H245UnicastAddress_iPAddress));

#ifndef _COMPACT
   dListInit(&(iPAddress->extElem1));
#endif /* !_COMPACT */


   sscanf(call->localIP, "%d.%d.%d.%d", &addr_part1, &addr_part2, &addr_part3,
                                  &addr_part4);
   sprintf(hexip, "%x %x %x %x", addr_part1, addr_part2,
                                     addr_part3, addr_part4);
   sscanf(hexip, "%x %x %x %x", &(iPAddress->network.data[0]),
                                &(iPAddress->network.data[1]),
                                &(iPAddress->network.data[2]),
                                &(iPAddress->network.data[3]));
   iPAddress->network.numocts = 4;
   iPAddress->tsapIdentifier = call->localRtpPort;

   /* media contrcol channel */
   h2250lcap->mediaControlChannel.t =
                                 T_H245TransportAddress_unicastAddress;
   h2250lcap->mediaControlChannel.u.unicastAddress =  (H245UnicastAddress*)
                         ASN1MALLOC(pctxt, sizeof(H245UnicastAddress));

   unicastAddrs1 = h2250lcap->mediaControlChannel.u.unicastAddress;
   memset(unicastAddrs1, 0, sizeof(H245UnicastAddress));
   unicastAddrs1->t = T_H245UnicastAddress_iPAddress;
   unicastAddrs1->u.iPAddress = (H245UnicastAddress_iPAddress*)
               ASN1MALLOC(pctxt, sizeof(H245UnicastAddress_iPAddress));
   iPAddress1 = unicastAddrs1->u.iPAddress;
   memset(iPAddress1, 0, sizeof(H245UnicastAddress_iPAddress));

#ifndef _COMPACT
   dListInit(&(iPAddress1->extElem1));
#endif /* !_COMPACT */

   sscanf(hexip, "%x %x %x %x", &(iPAddress1->network.data[0]),
                                &(iPAddress1->network.data[1]),
                                &(iPAddress1->network.data[2]),
                                &(iPAddress1->network.data[3]));
   iPAddress1->network.numocts = 4;
   iPAddress1->tsapIdentifier = call->localRtcpPort;

   ret = ooAddNewLogicalChannel(call,olc->forwardLogicalChannelNumber,
                                h2250lcap->sessionID, "audio", "receive", epCap);
   if(ret != OO_OK)
   {
      OOTRACEERR3("ERROR:Failed to add new logical channel entry to call "
                  "(%s, %s)\n", call->callType, call->callToken);
      return OO_FAILED;
   }

   ooSendH245Msg(call, ph245msg);
   OOTRACEINFO3("Built OpenLogicalChannelAck (%s, %s)\n", call->callType,
                 call->callToken);
   if(epCap->startReceiveChannel)
   {
      epCap->startReceiveChannel(call);     
      OOTRACEINFO3("Receive channel of type audio started (%s, %s)\n",
                    call->callType, call->callToken);
   }
   else{
          OOTRACEERR3("ERROR:No callback registered to start receive audio channel"
                   " (%s, %s)\n", call->callType, call->callToken);
      return OO_FAILED;
   }
   return OO_OK;
}

int ooOnReceivedOpenLogicalChannelAck(ooCallData *call,
                                      H245OpenLogicalChannelAck *olcAck)
{
   char remoteip[20];
   ooLogicalChannel *pLogicalChannel;
   H245H2250LogicalChannelAckParameters *h2250lcap;
   H245UnicastAddress *unicastAddr;
   H245UnicastAddress_iPAddress *iPAddress;
   H245UnicastAddress *unicastAddr1;
   H245UnicastAddress_iPAddress *iPAddress1;

   if(!((olcAck->m.forwardMultiplexAckParametersPresent == 1) &&
        (olcAck->forwardMultiplexAckParameters.t ==
         T_H245OpenLogicalChannelAck_forwardMultiplexAckParameters_h2250LogicalChannelAckParameters)))
   {
      OOTRACEERR3("Error: Processing open logical channel ack - LogicalChannel"
                  "Ack parameters absent (%s, %s)\n", call->callType, call->callToken);
      return OO_OK;  /* should send CloseLogicalChannel request */
   }

   h2250lcap =
      olcAck->forwardMultiplexAckParameters.u.h2250LogicalChannelAckParameters;
   /* Extract media channel address */
   if(h2250lcap->m.mediaChannelPresent != 1)
   {
      OOTRACEERR3("Error: Processing OpenLogicalChannelAck - media channel "
                  "absent (%s, %s)\n", call->callType, call->callToken);
      return OO_FAILED;
   }
   if(h2250lcap->mediaChannel.t != T_H245TransportAddress_unicastAddress)
   {
      OOTRACEERR3("Error: Processing OpenLogicalChannelAck - media channel "
                  "address type is not unicast (%s, %s)\n", call->callType,
                  call->callToken);
      return OO_FAILED;
   }
  
   unicastAddr = h2250lcap->mediaChannel.u.unicastAddress;
   if(unicastAddr->t != T_H245UnicastAddress_iPAddress)
   {
      OOTRACEERR3("Error: Processing OpenLogicalChannelAck - media channel "
                  "address type is not IP (%s, %s)\n", call->callType,
                   call->callToken);
      return OO_FAILED;
   }
   iPAddress = unicastAddr->u.iPAddress;
  
   sprintf(remoteip,"%d.%d.%d.%d", iPAddress->network.data[0],
                                  iPAddress->network.data[1],
                                  iPAddress->network.data[2],
                                  iPAddress->network.data[3]);
   call->remoteRtpPort = iPAddress->tsapIdentifier;
  
   /* Extract media control channel address */
   if(h2250lcap->m.mediaControlChannelPresent != 1)
   {
      OOTRACEERR3("Error: Processing OpenLogicalChannelAck - Missing media "
                  "control channel (%s, %s)\n", call->callType, call->callToken);
      return OO_FAILED;
   }
   if(h2250lcap->mediaControlChannel.t !=
                                     T_H245TransportAddress_unicastAddress)
   {
      OOTRACEERR3("Error: Processing OpenLogicalChannelAck - media control "
                  "channel addres type is not unicast (%s, %s)\n",
                   call->callType, call->callToken);
      return OO_FAILED;
   }
  
   unicastAddr1 = h2250lcap->mediaControlChannel.u.unicastAddress;
   if(unicastAddr1->t != T_H245UnicastAddress_iPAddress)
   {
      OOTRACEERR3("Error: Processing OpenLogicalChannelAck - media control "
                  "channel address type is not IP (%s, %s)\n", call->callType,
                   call->callToken);
      return OO_FAILED;
   }
   iPAddress1 = unicastAddr1->u.iPAddress;
   call->remoteRtcpPort = iPAddress1->tsapIdentifier;

   /* Set remote destination address for rtp session */
   strcpy(call->remoteIP, remoteip);
  
   /* Start channel here */
   pLogicalChannel = ooFindLogicalChannelByLogicalChannelNo(call,olcAck->forwardLogicalChannelNumber);
   if(!pLogicalChannel)
   {
      OOTRACEERR4("ERROR:Logical channel %d not found in the channel list for call"
                  " (%s, %s)\n", olcAck->forwardLogicalChannelNumber,
                  call->callType, call->callToken);
      return OO_FAILED;
   }
   if(pLogicalChannel->chanCap->startTransmitChannel)
   {
      pLogicalChannel->chanCap->startTransmitChannel(call);
      OOTRACEINFO4("TransmitLogical Channel of type %s started (%s, %s)\n",
                   pLogicalChannel->type,call->callType, call->callToken);
   }
   else{
      OOTRACEERR3("ERROR:No callback registered for starting transmit channel "
                  "(%s, %s)\n", call->callType, call->callToken);
      return OO_FAILED;
   }
   return OO_OK;
}

/**
 * Currently only disconnect end session command is supported.
 **/
int ooSendEndSessionCommand(ooCallData *call)
{
   int ret;
   H245CommandMessage * command;
   OOCTXT *pctxt;
   H245Message *ph245msg=NULL;
   ret = ooCreateH245Message(&ph245msg,
                      T_H245MultimediaSystemControlMessage_command);
   if(ret != OO_OK)
   {
      OOTRACEERR3("Error: H245 message creation failed for - End Session "
                  "Command (%s, %s)\n", call->callType, call->callToken);
      return OO_FAILED;
   }
   ph245msg->msgType = OOEndSessionCommand;

   command = ph245msg->h245Msg.u.command;
   pctxt = ph245msg->pctxt;
   memset(command, 0, sizeof(H245CommandMessage));
   command->t = T_H245CommandMessage_endSessionCommand;
   command->u.endSessionCommand = (H245EndSessionCommand*) ASN1MALLOC(pctxt,
                                  sizeof(H245EndSessionCommand));
   memset(command->u.endSessionCommand, 0, sizeof(H245EndSessionCommand));
   command->u.endSessionCommand->t = T_H245EndSessionCommand_disconnect;
   ooSendH245Msg(call, ph245msg);
   OOTRACEINFO3("Sending EndSession Command (%s, %s)\n", call->callType,
                call->callToken);
   return OO_OK;
}


int ooHandleH245Command(ooCallData *call,
                        H245CommandMessage *command)
{
   int ret;
   switch(command->t)
   {
      case T_H245CommandMessage_endSessionCommand:
         OOTRACEINFO3("Received EndSession command (%s, %s)\n",
                       call->callType, call->callToken);
         ooCloseH245Session(call);        
         ret = ooSendReleaseComplete(call);
         if(ret != OO_OK)
         {
            OOTRACEERR1("Error: Sending release complete\n");
            return OO_FAILED;
         }
         break;
      case T_H245CommandMessage_sendTerminalCapabilitySet:
         OOTRACEWARN3("Warning: Received command Send terminal capability set "
                      "- Not handled (%s, %s)\n", call->callType, call->callToken);
         break;
      case T_H245CommandMessage_flowControlCommand:
         OOTRACEWARN3("Warning: Flow control command received - Not handled "
                      "(%s, %s)\n", call->callType, call->callToken);
         break;
      default:
         OOTRACEWARN3("Warning: Unhandled H245 command message received "
                      "(%s, %s)\n", call->callType, call->callToken);
   }
   return OO_OK;
}


int ooOnReceivedTerminalCapabilitySetAck(ooCallData* call)
{
   if(call->capabilityExchangeState == OO_RemoteTermCapSetAcked)
      call->capabilityExchangeState = OO_TermCapExchangeDone;
   else
      call->capabilityExchangeState = OO_LocalTermCapSetAcked;
  
   if(call->capabilityExchangeState == OO_TermCapExchangeDone &&
      (call->masterSlaveState == OO_MasterSlave_Master ||
       call->masterSlaveState == OO_MasterSlave_Slave))
      ooOpenLogicalChannels(call);
   else{
      if(call->capabilityExchangeState == OO_TermCapExchangeDone)
         OOTRACEINFO1("Not opening channels - master slave remaining\n");
      else
         OOTRACEINFO1("Not opening channels - cap exchange remaining\n");
   }
  
   return OO_OK;
}

int ooCloseAllLogicalChannels(ooCallData *call)
{
   ooLogicalChannel *temp;
   int outgoing=0;
   temp = call->logicalChans;
   while(temp)
   {
      /* Sending closelogicalchannel only for outgoing channels*/
      if(!strcmp(temp->dir, "transmit"))
      {
       /* outgoing = 1;*/
         ooSendCloseLogicalChannel(call, temp);
      }
      else{
         ooSendRequestCloseLogicalChannel(call, temp);
      }
      temp = temp->next;
   }
/*
   if(!outgoing)
      ooSendEndSessionCommand(call);*/
   return OO_OK;
}

int ooSendCloseLogicalChannel(ooCallData *call, ooLogicalChannel *logicalChan)
{
   int ret = OO_OK;
   H245Message *ph245msg = NULL;
   OOCTXT *pctxt;
   H245RequestMessage *request;
   H245CloseLogicalChannel* clc;

   ret = ooCreateH245Message(&ph245msg, T_H245MultimediaSystemControlMessage_request);
   if(ret != OO_OK)
   {
      OOTRACEERR3("ERROR:Failed to create H245 message for closeLogicalChannel"
                  " message (%s, %s)\n", call->callType, call->callToken);
      return OO_FAILED;
   }
   ph245msg->msgType = OOCloseLogicalChannel;
   pctxt = ph245msg->pctxt;
   request = ph245msg->h245Msg.u.request;

   request->t = T_H245RequestMessage_closeLogicalChannel;
   request->u.closeLogicalChannel = (H245CloseLogicalChannel*)ASN1MALLOC(pctxt,
                                     sizeof(H245CloseLogicalChannel));
   if(!request->u.closeLogicalChannel)
   {
      OOTRACEERR3("ERROR:Memory allocation for CloseLogicalChannel failed "
                  "(%s, %s)\n", call->callType, call->callToken);
      freeContext(pctxt);
      ASN1CRTFREE0(pctxt);
      return OO_FAILED;
   }
   clc = request->u.closeLogicalChannel;
   memset(clc, 0, sizeof(H245CloseLogicalChannel));

   clc->forwardLogicalChannelNumber = logicalChan->channelNo;
   clc->source.t = T_H245CloseLogicalChannel_source_user;
   clc->m.reasonPresent = 1;
   clc->reason.t = T_H245CloseLogicalChannel_reason_unknown;
#ifndef _COMPACT
   dListInit(&clc->extElem1);
#endif
   ooSendH245Msg(call, ph245msg);
   OOTRACEINFO4("Sending close logical channel for %d (%s, %s)\n",
                 logicalChan->channelNo, call->callType, call->callToken);
   return OO_OK;
}

/*TODO: Need to pass reason as a parameter */
int ooSendRequestCloseLogicalChannel(ooCallData *call, ooLogicalChannel *logicalChan)
{
   int ret = OO_OK;
   H245Message *ph245msg = NULL;
   OOCTXT *pctxt;
   H245RequestMessage *request;
   H245RequestChannelClose *rclc;

   ret = ooCreateH245Message(&ph245msg, T_H245MultimediaSystemControlMessage_request);
   if(ret != OO_OK)
   {
      OOTRACEERR3("ERROR:Failed to create H245 message for "
                  "requestCloseLogicalChannel message (%s, %s)\n",
                   call->callType, call->callToken);
      return OO_FAILED;
   }
   ph245msg->msgType = OORequestCloseLogicalChannel;
   pctxt = ph245msg->pctxt;
   request = ph245msg->h245Msg.u.request;

   request->t = T_H245RequestMessage_requestChannelClose;
   request->u.requestChannelClose = (H245RequestChannelClose*)ASN1MALLOC(pctxt,
                                     sizeof(H245RequestChannelClose));
   if(!request->u.requestChannelClose)
   {
      OOTRACEERR3("ERROR:Memory allocation for RequestCloseLogicalChannel "
                  " failed (%s, %s)\n", call->callType, call->callToken);
      freeContext(pctxt);
      ASN1CRTFREE0(pctxt);
      return OO_FAILED;
   }

   rclc = request->u.requestChannelClose;
   memset(rclc, 0, sizeof(H245RequestChannelClose));
   rclc->forwardLogicalChannelNumber = logicalChan->channelNo;
  
   rclc->m.reasonPresent = 1;
   rclc->reason.t = T_H245CloseLogicalChannel_reason_unknown;
#ifndef _COMPACT
   dListInit(&rclc->extElem1);
#endif
   ooSendH245Msg(call, ph245msg);
   OOTRACEINFO4("Sending RequestCloseChannel for %d (%s, %s)\n",
                 logicalChan->channelNo, call->callType, call->callToken);
   return OO_OK;
}

int ooOnReceivedRequestChannelClose(ooCallData *call, H245RequestChannelClose *rclc)
{
   int ret=0;
   H245Message *ph245msg=NULL;
   H245ResponseMessage *response = NULL;
   OOCTXT *pctxt=NULL;
   H245RequestChannelCloseAck *rclcAck;
   ooLogicalChannel * lChannel=NULL;
   /* Send Ack: TODO: Need to send reject, if doesn't exist
   */
   lChannel = ooFindLogicalChannelByLogicalChannelNo(call,
                                        rclc->forwardLogicalChannelNumber);
   if(!lChannel)
   {
      OOTRACEERR4("ERROR:Channel %d requested to be closed not found "
                  "(%s, %s)\n", rclc->forwardLogicalChannelNumber,
                  call->callType, call->callToken);
      return OO_FAILED;
   }
   else{
      if(strcmp(lChannel->dir, "transmit"))
      {
         OOTRACEERR4("ERROR:Channel %d requested to be closed, Not a forward "
                     "channel (%s, %s)\n", rclc->forwardLogicalChannelNumber,
                     call->callType, call->callToken);
         return OO_FAILED;
      }
   }
   ret = ooCreateH245Message(&ph245msg, T_H245MultimediaSystemControlMessage_response);
   if(ret != OO_OK)
   {
      OOTRACEERR3("ERROR:Memory allocation for RequestChannelCloseAck message "
                  "failed (%s, %s)\n", call->callType, call->callToken);
      return OO_FAILED;
   }
   pctxt = ph245msg->pctxt;
   response = ph245msg->h245Msg.u.response;
   response->t = T_H245ResponseMessage_requestChannelCloseAck;
   response->u.requestChannelCloseAck = (H245RequestChannelCloseAck*)ASN1MALLOC
                                   (pctxt, sizeof(H245RequestChannelCloseAck));
   if(!response->u.requestChannelCloseAck)
   {
      OOTRACEERR3("ERROR:Failed to allocate memory for RequestChannelCloseAck "
                  "message (%s, %s)\n", call->callType, call->callToken);
      return OO_FAILED;
   }
   rclcAck = response->u.requestChannelCloseAck;
   memset(rclcAck, 0, sizeof(H245RequestChannelCloseAck));
   rclcAck->forwardLogicalChannelNumber = rclc->forwardLogicalChannelNumber;
#ifndef _COMPACT
   dListInit(&rclcAck->extElem1);
#endif
   ooSendH245Msg(call, ph245msg);
   OOTRACEINFO3("Sending RequestCloseChannelAck message (%s, %s)\n",
                 call->callType, call->callToken);
  
  
   /* Send Close Logical Channel*/
   ret = ooSendCloseLogicalChannel(call, lChannel);
   if(ret != OO_OK)
   {
      OOTRACEERR3("ERROR:Failed to build CloseLgicalChannel message (%s, %s)\n",
                   call->callType, call->callToken);
      return OO_FAILED;
   }

   return OO_OK;
}
/****/
int ooOnReceivedCloseLogicalChannel(ooCallData *call, H245CloseLogicalChannel* clc)
{
   int ret=0;
   ooH323EpCapability *epCap=NULL;
   ooLogicalChannel *pLogicalChannel=NULL;
   H245Message *ph245msg = NULL;
   OOCTXT *pctxt = NULL;
   H245CloseLogicalChannelAck * clcAck;
   H245ResponseMessage *response;

   pLogicalChannel = ooFindLogicalChannelByLogicalChannelNo(call,clc->forwardLogicalChannelNumber);
   if(!pLogicalChannel)
   {
      OOTRACEERR4("ERROR:Invalid logical Channel No %d in received "
                  "CloseLogicalChannel (%s, %s)\n",
                  clc->forwardLogicalChannelNumber, call->callType, call->callToken);
      return OO_FAILED;
   }
  
   epCap = (ooH323EpCapability*) pLogicalChannel->chanCap;
   if(!strcmp(pLogicalChannel->dir, "receive"))
   {
      if(epCap->stopReceiveChannel)
      {
         epCap->stopReceiveChannel(call);
         OOTRACEINFO4("Stopped Receive channel %d (%s, %s)\n",
              clc->forwardLogicalChannelNumber, call->callType, call->callToken);
      }
      else{
         OOTRACEERR3("ERROR: No callback registered to stop receive channel "
                       "(%s, %s)\n", call->callType, call->callToken);
      }
   }
   else
   {
      if(epCap->stopTransmitChannel)
      {
         epCap->stopTransmitChannel(call);
         OOTRACEINFO4("Stopped Transmit channel %d (%s, %s)\n",
              clc->forwardLogicalChannelNumber, call->callType, call->callToken);
      }
      else{
         OOTRACEERR3("ERROR:No callback registered to stop transmit channel "
                     "(%s, %s)\n", call->callType, call->callToken);
      }
   }
   ooRemoveLogicalChannel(call, clc->forwardLogicalChannelNumber);
   /* TODO: SendCloseLogicalChannelAck */
   ret = ooCreateH245Message(&ph245msg, T_H245MultimediaSystemControlMessage_response);
   if(ret != OO_OK)
   {
      OOTRACEERR3("ERROR:Failed to create H245 message for "
                  "closeLogicalChannelAck (%s, %s)\n", call->callType,
                  call->callToken);
      return OO_FAILED;
   }
   pctxt = ph245msg->pctxt;
   response = ph245msg->h245Msg.u.response;
   response->t = T_H245ResponseMessage_closeLogicalChannelAck;
   response->u.closeLogicalChannelAck = (H245CloseLogicalChannelAck*)
                         ASN1MALLOC(pctxt, sizeof(H245CloseLogicalChannelAck));
   clcAck = response->u.closeLogicalChannelAck;
   if(!clcAck)
   {
      OOTRACEERR3("ERROR:Failed to allocate memory for closeLogicalChannelAck "
                  "(%s, %s)\n", call->callType, call->callToken);
      return OO_OK;
   }
   memset(clcAck, 0, sizeof(H245CloseLogicalChannelAck));
   clcAck->forwardLogicalChannelNumber = clc->forwardLogicalChannelNumber;
#ifndef _COMPACT
   dListInit(&(clcAck->extElem1));
#endif
   ooSendH245Msg(call, ph245msg);
   OOTRACEINFO3("Sending CloseLogicalChannelAck message (%s, %s)\n",
                 call->callType, call->callToken);
  
   if(call->noOfLogicalChannels == 0 && call->callState == OO_CALL_CLEARING)
   {
      ooSendEndSessionCommand(call);
   }
   return OO_OK;
}

int ooOnReceivedCloseChannelAck(ooCallData* call, H245CloseLogicalChannelAck* clcAck)
{
   int ret = OO_OK;
   ooLogicalChannel *pLogicalChannel = NULL;
   ooH323EpCapability *epCap=NULL;
   pLogicalChannel = ooFindLogicalChannelByLogicalChannelNo(call,clcAck->forwardLogicalChannelNumber);
   if(!pLogicalChannel)
   {
      OOTRACEERR4("ERROR:Invalid logical Channel No %d in received "
                  "CloseLogicalChannelAck (%s, %s)\n",
                  clcAck->forwardLogicalChannelNumber, call->callType, call->callToken);
      return OO_FAILED;
   }

   epCap = (ooH323EpCapability*) pLogicalChannel->chanCap;
   if(!strcmp(pLogicalChannel->dir, "receive"))
   {
      if(epCap->stopReceiveChannel)
      {
         epCap->stopReceiveChannel(call);
         OOTRACEINFO4("Stopped Receive channel %d (%s, %s)\n",
              clcAck->forwardLogicalChannelNumber, call->callType, call->callToken);
      }
      else{
         OOTRACEERR3("ERROR:No callback registered for stopReceiveChannel "
                     "(%s, %s)\n", call->callType, call->callToken);
      }
   }
   else
   {
      if(epCap->stopTransmitChannel)
      {
         epCap->stopTransmitChannel(call);
         OOTRACEINFO4("Stopped Transmit channel %d (%s, %s)\n",
              clcAck->forwardLogicalChannelNumber, call->callType, call->callToken);
      }
      else{
         OOTRACEERR3("ERROR:No callback registered for stopTransmitChannel "
                     "(%s, %s)\n", call->callType, call->callToken);
      }
   }
   ooRemoveLogicalChannel(call, clcAck->forwardLogicalChannelNumber);
   if(call->noOfLogicalChannels == 0 && call->callState == OO_CALL_CLEARING)
   {
      ooSendEndSessionCommand(call);
   }

   return OO_OK;
}
/* TODO: TerminalCapabilityReject/TerminalCapabilityRelease
         OpenLogicalChannelReject/OpenLogicalChannelRelease
         OpenLogicalChannelConfirm/*/
int ooHandleH245Message(ooCallData *call, H245Message * pmsg)
{
   H245Message *pH245 = (H245Message*)pmsg;
   /* There are four major types of H.245 messages that can be received.
      Request/Response/Command/Indication. Each one of them need to be
      handled separately.
   */  
   H245RequestMessage *request;
   H245ResponseMessage *response;
   H245CommandMessage *command;

   switch(pH245->h245Msg.t)
   {
     /* H.245 Request message is received */
      case (T_H245MultimediaSystemControlMessage_request):
         request = pH245->h245Msg.u.request;
         switch(request->t)
         {
         case T_H245RequestMessage_terminalCapabilitySet:
           
            ooOnReceivedTerminalCapabilitySet(call, pH245);
            break;
         case T_H245RequestMessage_masterSlaveDetermination:
            ooHandleMasterSlave(call,
                                  request->u.masterSlaveDetermination,
                                  OOMasterSlaveDetermination);
            break;
        case T_H245RequestMessage_openLogicalChannel:
            ooHandleOpenLogicalChannel(call,
                                       request->u.openLogicalChannel);
            break;
        case T_H245RequestMessage_closeLogicalChannel:
            OOTRACEINFO4("Received close logical Channel - %d (%s, %s)\n",
               request->u.closeLogicalChannel->forwardLogicalChannelNumber,
               call->callType, call->callToken);
            ooOnReceivedCloseLogicalChannel(call, request->u.closeLogicalChannel);
            break;
        case T_H245RequestMessage_requestChannelClose:
            OOTRACEINFO4("Received RequestChannelClose - %d (%s, %s)\n",
               request->u.requestChannelClose->forwardLogicalChannelNumber,
               call->callType, call->callToken);
            ooOnReceivedRequestChannelClose(call,
                                            request->u.requestChannelClose);
            break;
        default:
            ;
         }
         break;
      /* H.245 Response message is received */
      case (T_H245MultimediaSystemControlMessage_response):
         response = pH245->h245Msg.u.response;
         switch(response->t)
         {
         case T_H245ResponseMessage_masterSlaveDeterminationAck:
            ooHandleMasterSlave(call,
                                response->u.masterSlaveDeterminationAck,
                                  OOMasterSlaveAck);
            /* Send terminal capability set message for G.711 transmit/
               receive capability.
            */
    /*        ooSendG711CallTermCapMsg(myContext);*/
            break;
         case T_H245ResponseMessage_terminalCapabilitySetAck:
            ooOnReceivedTerminalCapabilitySetAck(call);
            break;
         case T_H245ResponseMessage_openLogicalChannelAck:
            ooOnReceivedOpenLogicalChannelAck(call,
                                           response->u.openLogicalChannelAck);
            break;
         case T_H245ResponseMessage_closeLogicalChannelAck:
            OOTRACEINFO4("CloseLogicalChannelAck received for %d (%s, %s)\n",
               response->u.closeLogicalChannelAck->forwardLogicalChannelNumber,
               call->callType, call->callToken);
            ooOnReceivedCloseChannelAck(call, response->u.closeLogicalChannelAck);
            break;
         case T_H245ResponseMessage_requestChannelCloseAck:
             OOTRACEINFO4("RequestChannelCloseAck received - %d (%s, %s)\n",
               response->u.requestChannelCloseAck->forwardLogicalChannelNumber,
               call->callType, call->callToken);
             break;
         default:
            ;
         }
         break;
      /* H.245 command message is received */
      case (T_H245MultimediaSystemControlMessage_command):
         command = pH245->h245Msg.u.command;
         ooHandleH245Command(call, command);
         break;
      /* H.245 Indication message received */
      /*case (T_H245MultimediaSystemControlMessage_indication):
         break;
      case (T_H245MultimediaSystemControlMessage_extElem1):
         break;*/
      default:
        ;
   }

   return OO_OK;
}

int ooOnReceivedTerminalCapabilitySet(ooCallData *call, H245Message *pmsg)
{
   int ret = 0;
   OOCTXT *pctxt=NULL;
   if(call->remoteTermCapSeqNo >=
      pmsg->h245Msg.u.request->u.terminalCapabilitySet->sequenceNumber)
   {
      OOTRACEINFO4("Rejecting TermCapSet message with SeqNo %d, as already "
                   "acknowledged message with this SeqNo (%s, %s)\n",
                                   call->remoteTermCapSeqNo, call->callType, call->callToken);
/*      ooH245RejectTerminalCapabilitySet(call, pmsg);*/
      return OO_OK;
   }
   call->remoteTermCapSeqNo = pmsg->h245Msg.u.request->u.terminalCapabilitySet->sequenceNumber;
   if(call->remoteTermCapSet)
   {
      pctxt = call->remoteTermCapSet->pctxt;
      freeContext(pctxt);
      ASN1CRTFREE0(pctxt);
   }
  
   call->remoteTermCapSet = pmsg;

   ooH245AcknowledgeTerminalCapabilitySet(call);  
  
   if(call->capabilityExchangeState == OO_LocalTermCapSetAcked)
      call->capabilityExchangeState = OO_TermCapExchangeDone;
   else{
      call->capabilityExchangeState = OO_RemoteTermCapSetAcked;
      return OO_OK;
   }

   /* Check whether MasterSlave Procedure has finished */
   if(call->masterSlaveState != OO_MasterSlave_Master &&
      call->masterSlaveState != OO_MasterSlave_Slave)
      return OO_OK;

   /* As both MasterSlave and TerminalCapabilitySet procedures have finished,
      OpenLogicalChannels*/
   ret = ooOpenLogicalChannels(call);
  
   return OO_OK;
}

int ooH245RejectTerminalCapabilitySet(ooCallData *call, H245Message* pmsg)
{
   return OO_OK;
}

int ooH245AcknowledgeTerminalCapabilitySet(ooCallData *call)
{
   H245Message *ph245msg=NULL;
   H245ResponseMessage * response=NULL;
   OOCTXT *pctxt=NULL;
   int ret = ooCreateH245Message(&ph245msg,
                      T_H245MultimediaSystemControlMessage_response);
   if(ret != OO_OK)
   {
      OOTRACEERR1("ERROR:H245 message creation failed for - "
                           "TerminalCapability Set Ack\n");
      return OO_FAILED;
   }
   ph245msg->msgType = OOTerminalCapabilitySetAck;
   response = ph245msg->h245Msg.u.response;
   memset(response, 0, sizeof(H245ResponseMessage));
   pctxt = ph245msg->pctxt;
   response->t = T_H245ResponseMessage_terminalCapabilitySetAck;
  
   response->u.terminalCapabilitySetAck = (H245TerminalCapabilitySetAck*)
                   ASN1MALLOC(pctxt, sizeof(H245TerminalCapabilitySetAck));

   memset(response->u.terminalCapabilitySetAck, 0,
                                 sizeof(H245TerminalCapabilitySetAck));
   response->u.terminalCapabilitySetAck->sequenceNumber = call->remoteTermCapSet->h245Msg.u.request->u.terminalCapabilitySet->sequenceNumber;
#ifndef _COMPACT
   dListInit(&(response->u.terminalCapabilitySetAck->extElem1));  
#endif /* !_COMPACT */
   ooSendH245Msg(call, ph245msg);
   OOTRACEINFO3("Built TerminalCapabilitySet Ack (%s, %s)\n",
                 call->callType, call->callToken);
   return OO_OK;
}

int ooOpenLogicalChannels(ooCallData *call)
{
   int ret=0;
   OOTRACEINFO3("Opening logical channels (%s, %s)\n", call->callType,
                 call->callToken);  
   OOTRACEINFO3("Call audiochannel %d call active %d\n",call->audioChannel, call->isAudioActive);
   if(call->audioChannel && !call->isAudioActive)
   {
      ret = ooOpenLogicalAudioChannel(call);
      call->isAudioActive = 1;
   }
   else
      OOTRACEINFO1("Opps problem with audio channel \n");
   if(call->videoChannel)
   {
       ;     
   }
   if(call->dataChannel)
   {
      ;
   }
   return OO_OK;
}

int ooOpenLogicalAudioChannel(ooCallData *call)
{
   H245CapabilityTableEntry *capTableEntry;
   H245Capability *h245Cap;
   ooH323EpCapability *epCap;
   H245AudioCapability *audioCapLocal, *audioCapRemote;
   DListNode *curNode1 = NULL, *curNode2=NULL;
   H245Message  *h245TermCapMsg;
   H245RequestMessage *request;
   H245TerminalCapabilitySet *termCapSet;
   int i=0, j=0, found=0;
   /* Check whether local endpoint has audio capability */
   if(gH323ep.audioCaps.count == 0)
   {
      OOTRACEERR3("ERROR:Local endpoint does not have any audio capabilities"
                  " (%s, %s)\n", call->callType, call->callToken);
      return OO_FAILED;
   }
  
   /* Check whether capability table is present in remote endpoints CapSet */
   h245TermCapMsg = call->remoteTermCapSet;
   request = h245TermCapMsg->h245Msg.u.request;
   termCapSet = request->u.terminalCapabilitySet;
   if(termCapSet->m.capabilityTablePresent == 0)
   {
      OOTRACEERR3("ERROR:No CapabilityTable Present in remote Terminal "
                  "Capability Set (%s, %s)\n", call->callType, call->callToken);
      return OO_FAILED;
   }
  
   /* Go through local endpoints capabilities sequentially, and find out the
      first one which has a match in the remote endpoints receive capabilities.
   */
   for(i=0; i<(int) gH323ep.audioCaps.count; i++)
   {
      /* Retrieve local capability */
      curNode1 = dListFindByIndex (&gH323ep.audioCaps, i) ;
      epCap = (ooH323EpCapability*)curNode1->data;
      audioCapLocal = (H245AudioCapability*) epCap->cap;
     
      /* Serach for a match in remote capabilities */
      for(j = 0; j<(int) termCapSet->capabilityTable.count; j++)
      {
         curNode2 = dListFindByIndex (&termCapSet->capabilityTable, j) ;
         capTableEntry = (H245CapabilityTableEntry *) curNode2->data;
        
                 if(!capTableEntry->m.capabilityPresent)
            continue;
         else
            h245Cap = &(capTableEntry->capability);

        
         /* Only audio Capabilities */
         if(h245Cap->t == T_H245Capability_receiveAudioCapability)
            audioCapRemote = h245Cap->u.receiveAudioCapability;
         else if(h245Cap->t == T_H245Capability_receiveAndTransmitAudioCapability)
            audioCapRemote = h245Cap->u.receiveAndTransmitAudioCapability;
         else
            continue;
         /* Check for a match */
        
         if(audioCapLocal->t == audioCapRemote->t)
         {
            found = 1;
            break;
         }
       
          }
      if(found) break;
   }
  
   if(!found)
   {
      OOTRACEERR3("ERROR:Incompatible audio capabilities - Can not open "
                  "audio channel (%s, %s)\n", call->callType, call->callToken);
      return OO_FAILED;
   }
   switch(audioCapLocal->t)
   {
   case T_H245AudioCapability_g711Ulaw64k:
      ooOpenG711ULaw64KChannel(call);
      break;
   default:
      OOTRACEERR3("ERROR:Unknown Audio Capability type (%s, %s)\n",
                   call->callType, call->callToken);
   }
 
    return OO_OK;
}

int ooOpenG711ULaw64KChannel(ooCallData* call)
{
   int ret;
   H245Message *ph245msg = NULL;
   H245RequestMessage * request;
   DListNode *curNode1 = NULL;
   OOCTXT *pctxt;
   ooH323EpCapability* epCap=NULL;
   H245OpenLogicalChannel_forwardLogicalChannelParameters *flcp;
   H245AudioCapability *audioCap, *audioCapLocal;
   H245H2250LogicalChannelParameters *h2250lcp;
   H245UnicastAddress *unicastAddrs;
   H245UnicastAddress_iPAddress *iPAddress;
   char hexip[20];
   int addr_part1, addr_part2, addr_part3, addr_part4;
  
   if(call->noOfLogicalChannels >=5)
   {
      OOTRACEERR3("ERROR:Can not open more than 5 logical channels per call "
                  "(%s, %s)\n", call->callType, call->callToken);
      return OO_FAILED;
   }
   ret = ooCreateH245Message(&ph245msg,
                      T_H245MultimediaSystemControlMessage_request);
   if(ret != OO_OK)
   {
      OOTRACEERR3("Error: H245 message creation failed for - Open G711 ulaw "
                  "channel (%s, %s)\n", call->callType, call->callToken);
      return OO_FAILED;
   }

   ph245msg->msgType = OOOpenLogicalChannel;
   request = ph245msg->h245Msg.u.request;
   pctxt = ph245msg->pctxt;
   memset(request, 0, sizeof(H245RequestMessage));

   request->t = T_H245RequestMessage_openLogicalChannel;
   request->u.openLogicalChannel = (H245OpenLogicalChannel*)
                     ASN1MALLOC(pctxt, sizeof(H245OpenLogicalChannel));
   memset(request->u.openLogicalChannel, 0,
                                     sizeof(request->u.openLogicalChannel));
   request->u.openLogicalChannel->forwardLogicalChannelNumber = call->logicalChanNoCur++;
   if(call->logicalChanNoCur > call->logicalChanNoMax)
      call->logicalChanNoCur = call->logicalChanNoBase;
  
#ifndef _COMPACT
   dListInit(&(request->u.openLogicalChannel->extElem1));
#endif /* !_COMPACT */
   /* Populate H245OpenLogicalChannel_ForwardLogicalChannel Parameters*/
   flcp = &(request->u.openLogicalChannel->forwardLogicalChannelParameters);
   flcp->m.portNumberPresent = 0;
   flcp->m.forwardLogicalChannelDependencyPresent = 0;
   flcp->m.replacementForPresent = 0;

#ifndef _COMPACT
   dListInit(&(flcp->extElem1));
#endif /* !_COMPACT */
   /* data type of channel is audio */
   flcp->dataType.t = T_H245DataType_audioData;

   /* set audio capability for channel */
   audioCap = (H245AudioCapability*) ASN1MALLOC(pctxt,
                                     sizeof(H245AudioCapability));
   memset(audioCap, 0, sizeof(audioCap));
   epCap = ooIsAudioCapabilitySupported(T_H245AudioCapability_g711Ulaw64k, OO_TX_CAP);

   if(!epCap)
   {
      OOTRACEERR3("ERROR: No transmit audio capability with g711ulaw64k found"
                  " (%s, %s)\n", call->callType, call->callToken);
      freeContext(pctxt);
      ASN1CRTFREE0(pctxt);
      return OO_FAILED;
   }
   else
      audioCapLocal = (H245AudioCapability*) epCap->cap;

   ooCopyAudioCapability(audioCapLocal, audioCap);
  
  
   flcp->dataType.u.audioData = audioCap;
   flcp->multiplexParameters.t =
      T_H245OpenLogicalChannel_forwardLogicalChannelParameters_multiplexParameters_h2250LogicalChannelParameters;
   flcp->multiplexParameters.u.h2250LogicalChannelParameters =
                         (H245H2250LogicalChannelParameters*)ASN1MALLOC(pctxt,
                          sizeof(H245H2250LogicalChannelParameters));

   h2250lcp = flcp->multiplexParameters.u.h2250LogicalChannelParameters;
   memset(h2250lcp, 0, sizeof(h2250lcp));

#ifndef _COMPACT
   dListInit(&(h2250lcp->extElem1));
#endif /* !_COMPACT */

   h2250lcp->sessionID = 1;
   h2250lcp->mediaGuaranteedDelivery = 0;
   h2250lcp->silenceSuppression = 0;
   h2250lcp->m.mediaControlChannelPresent = 1;
  
   h2250lcp->mediaControlChannel.t =
                                 T_H245TransportAddress_unicastAddress;
   h2250lcp->mediaControlChannel.u.unicastAddress =  (H245UnicastAddress*)
                         ASN1MALLOC(pctxt, sizeof(H245UnicastAddress));

   unicastAddrs = h2250lcp->mediaControlChannel.u.unicastAddress;
   memset(unicastAddrs, 0, sizeof(unicastAddrs));
   unicastAddrs->t = T_H245UnicastAddress_iPAddress;
   unicastAddrs->u.iPAddress = (H245UnicastAddress_iPAddress*)
               ASN1MALLOC(pctxt, sizeof(H245UnicastAddress_iPAddress));
   iPAddress = unicastAddrs->u.iPAddress;
   memset(iPAddress, 0, sizeof(iPAddress));

#ifndef _COMPACT
   dListInit(&(iPAddress->extElem1));
#endif /* !_COMPACT */

   sscanf(call->localIP, "%d.%d.%d.%d", &addr_part1, &addr_part2, &addr_part3,
                                  &addr_part4);
   sprintf(hexip, "%x %x %x %x", addr_part1, addr_part2, addr_part3,
                                 addr_part4);
   sscanf(hexip, "%x %x %x %x", &(iPAddress->network.data[0]),
                                &(iPAddress->network.data[1]),
                                &(iPAddress->network.data[2]),
                                &(iPAddress->network.data[3]));
   iPAddress->network.numocts = 4;
   iPAddress->tsapIdentifier = call->localRtcpPort;
   ooSendH245Msg(call, ph245msg);
   OOTRACEINFO3("Built OpenLogicalChannel (%s, %s)\n", call->callType,
                call->callToken);
  
   ret = ooAddNewLogicalChannel(call,
                   request->u.openLogicalChannel->forwardLogicalChannelNumber,
                   h2250lcp->sessionID, "audio", "transmit", epCap);
   if(ret!=OO_OK)
   {
      OOTRACEERR3("ERROR:Failed to add new logical channel entry (%s, %s)\n",
                  call->callType, call->callToken);
      return OO_FAILED;
   }
  
   return OO_OK;
}

int ooAddFastStartToSetup(ooCallData *call, H225Setup_UUIE *setup)
{
   return OO_OK;
}

int ooBuildOpenLogicalChannelAudio(ooCallData *call, H245OpenLogicalChannel *olc, ooH323EpCapability *epCap, OOCTXT*pctxt)
{
   int reverse=0, forward=0;
   H245AudioCapability *audioCap;
   H245OpenLogicalChannel_forwardLogicalChannelParameters *flcp;
   H245OpenLogicalChannel_reverseLogicalChannelParameters *rlcp;
   H245H2250LogicalChannelParameters *pH2250lcp1, *pH2250lcp2;
   H245UnicastAddress *pUnicastAddrs;
   H245UnicastAddress_iPAddress *pIpAddrs;
   int addr_part1, addr_part2, addr_part3, addr_part4;
   char hexip[20];
  
   if(epCap->t == T_H245Capability_receiveAudioCapability)
      reverse = 1;
   else if(epCap->t == T_H245Capability_transmitAudioCapability)
      forward = 1;
   else if(epCap->t == T_H245Capability_receiveAndTransmitAudioCapability)
   {
      reverse = 1;
      forward = 1;
   }

   audioCap = (H245AudioCapability*)epCap->cap;
   olc->forwardLogicalChannelNumber = call->logicalChanNoCur++;
   if(call->logicalChanNoCur > call->logicalChanNoMax)
      call->logicalChanNoCur = call->logicalChanNoBase;

   sscanf(call->localIP, "%d.%d.%d.%d", &addr_part1, &addr_part2, &addr_part3, &addr_part4);
   sprintf(hexip, "%x %x %x %x", addr_part1, addr_part2, addr_part3, addr_part4);
   if(forward)
   {
      flcp = &(olc->forwardLogicalChannelParameters);
      memset(flcp, 0, sizeof(H245OpenLogicalChannel_forwardLogicalChannelParameters));
      flcp->dataType.t = T_H245DataType_audioData;
      /* Note we don't allocate memory and copy capability, we just set the pointer.
         This is ok as end point capability will be valid till the end of the application,
         and since we have single threaded app, we are safe. */
      flcp->dataType.u.audioData = audioCap;
     
          flcp->multiplexParameters.t = T_H245OpenLogicalChannel_forwardLogicalChannelParameters_multiplexParameters_h2250LogicalChannelParameters;
      pH2250lcp1 = (H245H2250LogicalChannelParameters*)ASN1MALLOC(pctxt, sizeof(H245H2250LogicalChannelParameters));
      memset(pH2250lcp1, 0, sizeof(H245H2250LogicalChannelParameters));
      flcp->multiplexParameters.t = T_H245OpenLogicalChannel_forwardLogicalChannelParameters_multiplexParameters_h2250LogicalChannelParameters;
     
      flcp->multiplexParameters.u.h2250LogicalChannelParameters = pH2250lcp1;
         
      pH2250lcp1->sessionID = 1;
      pH2250lcp1->m.mediaControlChannelPresent = 1;

      pH2250lcp1->mediaControlChannel.t =
                                 T_H245TransportAddress_unicastAddress;
      pUnicastAddrs = (H245UnicastAddress*) ASN1MALLOC(pctxt, sizeof(H245UnicastAddress));
      memset(pUnicastAddrs, 0, sizeof(H245UnicastAddress));
      pH2250lcp1->mediaControlChannel.u.unicastAddress =  pUnicastAddrs;

     
     
          pUnicastAddrs->t = T_H245UnicastAddress_iPAddress;
      pIpAddrs = (H245UnicastAddress_iPAddress*) ASN1MALLOC(pctxt, sizeof(H245UnicastAddress_iPAddress));
      memset(pIpAddrs, 0, sizeof(H245UnicastAddress_iPAddress));

      pUnicastAddrs->u.iPAddress = pIpAddrs;
     

#ifndef _COMPACT
      dListInit(&(pIpAddrs->extElem1));
#endif /* !_COMPACT */
     
      sscanf(hexip, "%x %x %x %x", &(pIpAddrs->network.data[0]),
                                &(pIpAddrs->network.data[1]),
                                &(pIpAddrs->network.data[2]),
                                &(pIpAddrs->network.data[3]));
      pIpAddrs->network.numocts = 4;
      pIpAddrs->tsapIdentifier = call->localRtcpPort;

   }

   if(reverse)
   {
      olc->m.reverseLogicalChannelParametersPresent = 1;
      rlcp = &(olc->reverseLogicalChannelParameters);
      memset(rlcp, 0, sizeof(H245OpenLogicalChannel_reverseLogicalChannelParameters));
      rlcp->dataType.t = T_H245DataType_audioData;
      /* Note we don't allocate memory and copy capability, we just set the pointer.
         This is ok as end point capability will be valid till the end of the application,
         and since we have single threaded app, we are safe. */
      rlcp->dataType.u.audioData = audioCap;
     
          rlcp->multiplexParameters.t = T_H245OpenLogicalChannel_reverseLogicalChannelParameters_multiplexParameters_h2250LogicalChannelParameters;
      pH2250lcp2 = (H245H2250LogicalChannelParameters*) ASN1MALLOC(pctxt, sizeof(H245H2250LogicalChannelParameters));
      rlcp->multiplexParameters.u.h2250LogicalChannelParameters = pH2250lcp2;
      memset(pH2250lcp2, 0, sizeof(H245H2250LogicalChannelParameters));
      pH2250lcp2->sessionID = 1;
      pH2250lcp2->m.mediaChannelPresent = 1;

      pH2250lcp2->mediaChannel.t =
                                 T_H245TransportAddress_unicastAddress;
      pUnicastAddrs = (H245UnicastAddress*) ASN1MALLOC(pctxt, sizeof(H245UnicastAddress));
     
      memset(pUnicastAddrs, 0, sizeof(H245UnicastAddress));
      pH2250lcp2->mediaChannel.u.unicastAddress =  pUnicastAddrs;

     
      pUnicastAddrs->t = T_H245UnicastAddress_iPAddress;
      pIpAddrs = (H245UnicastAddress_iPAddress*) ASN1MALLOC(pctxt, sizeof(H245UnicastAddress_iPAddress));
      memset(pIpAddrs, 0, sizeof(H245UnicastAddress_iPAddress));
      pUnicastAddrs->u.iPAddress = pIpAddrs;     

#ifndef _COMPACT
      dListInit(&(pIpAddrs->extElem1));
#endif /* !_COMPACT */

      sscanf(hexip, "%x %x %x %x", &(pIpAddrs->network.data[0]),
                                &(pIpAddrs->network.data[1]),
                                &(pIpAddrs->network.data[2]),
                                &(pIpAddrs->network.data[3]));
      pIpAddrs->network.numocts = 4;
      pIpAddrs->tsapIdentifier = call->localRtpPort;
   }
  
   return OO_OK;
}

