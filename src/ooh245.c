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

#include "ooh245.h"
#include "ooCalls.h"
#include "printHandler.h"
#include "ooh323ep.h"
#include "ooCapability.h"

#ifdef _WIN32
#include <stdlib.h>
#include <time.h>
#endif

/** Global endpoint structure */
extern ooEndPoint gH323ep;

static ASN1OBJID gh245ProtocolID = {
  6, { 0, 0, 8, 245, 0, 8 }
};

int ooCreateH245Message(H245Message **pph245msg, int type)
{
   OOCTXT* pctxt = &gH323ep.msgctxt;

   *pph245msg = (H245Message*) memAlloc (pctxt, sizeof(H245Message));

   if(!(*pph245msg))
   {
      OOTRACEERR1("ERROR:Failed to allocate memory for h245 message\n");
      return OO_FAILED;
   }
   else
   {
      (*pph245msg)->h245Msg.t = type;
      switch(type)
      {
         case  T_H245MultimediaSystemControlMessage_request:
            (*pph245msg)->h245Msg.u.request = (H245RequestMessage*)
               memAllocZ (pctxt, sizeof(H245RequestMessage));

            /*Check for successful mem allocation, and if successful initialize
              mem to zero*/
            if(!(*pph245msg)->h245Msg.u.request)
            {
               OOTRACEERR1("ERROR:Memory allocation for H.245 request"
                                     " message failed\n");
               return OO_FAILED;
            }
            break;

         case T_H245MultimediaSystemControlMessage_response:
            (*pph245msg)->h245Msg.u.response = (H245ResponseMessage*)
               memAllocZ (pctxt, sizeof(H245ResponseMessage));

            /*Check for successful mem allocation, and if successful initialize
              mem to zero*/
            if(!(*pph245msg)->h245Msg.u.response)
            {
               OOTRACEERR1("ERROR:Memory allocation for H.245 response"
                                     " message failed\n");
               return OO_FAILED;
            }
            break;

         case T_H245MultimediaSystemControlMessage_command:
            (*pph245msg)->h245Msg.u.command = (H245CommandMessage*)
               memAllocZ (pctxt, sizeof(H245CommandMessage));

            /*Check for successful mem allocation, and if successful initialize
              mem to zero*/
            if(!(*pph245msg)->h245Msg.u.command)
            {
               OOTRACEERR1("ERROR:Memory allocation for H.245 command"
                                     " message failed\n");
               return OO_FAILED;
            }
            break;

         case T_H245MultimediaSystemControlMessage_indication:
            (*pph245msg)->h245Msg.u.indication = (H245IndicationMessage*)
               memAllocZ (pctxt, sizeof(H245IndicationMessage));

            /*Check for successful mem allocation, and if successful initialize
              mem to zero*/
            if(!(*pph245msg)->h245Msg.u.indication == 0)
            {
               OOTRACEERR1("ERROR:Memory allocation for H.245 indication"
                                     " message failed\n");
               return OO_FAILED;
            }
            break;

         default:
            OOTRACEERR1("ERROR: H245 message type not supported\n");
      }

      return OO_OK;
   }
}

int ooFreeH245Message(ooCallData *call, H245Message *pmsg)
{
  /* In case of tunneling, memory is freed when corresponding Q931 message is freed.*/
   if (0 != pmsg) {
     if(!call->isTunnelingActive)
         memReset (&gH323ep.msgctxt);
   }
   return OO_OK;
}

#ifndef _COMPACT
static void ooPrintH245Message
(ooCallData* call, ASN1OCTET* msgbuf, ASN1UINT msglen)
{
   OOCTXT ctxt;
   H245MultimediaSystemControlMessage mmMsg;
   int ret;

   initContext (&ctxt);

   setPERBuffer (&ctxt, msgbuf, msglen, TRUE);

   initializePrintHandler(&printHandler, "Sending H.245 Message");

   /* Add event handler to list */
   rtAddEventHandler (&ctxt, &printHandler);
   ret = asn1PD_H245MultimediaSystemControlMessage(&ctxt, &mmMsg);
   if(ret != ASN_OK)
   {
      OOTRACEERR3("Error decoding H245 message (%s, %s)\n",
                  call->callType, call->callToken);
      OOTRACEERR1 (errGetText (&ctxt));
   }
   finishPrint();
   freeContext(&ctxt);  
}
#endif


int ooEncodeH245Message(ooCallData *call, H245Message *ph245Msg, char *msgbuf, int size)
{
   int len=0, encodeLen=0, i=0;
   int stat=0;
   ASN1OCTET* encodePtr=NULL;
   H245MultimediaSystemControlMessage *multimediaMsg;
   OOCTXT *pctxt = &gH323ep.msgctxt;
   multimediaMsg = &(ph245Msg->h245Msg);

   if(!msgbuf || size<200)
   {
      OOTRACEERR3("Error: Invalid message buffer/size for "
                  "ooEncodeH245Message. (%s, %s)\n",
                   call->callType, call->callToken);
      return OO_FAILED;
   }

   msgbuf[i++] = ph245Msg->msgType;
   /* This will contain the total length of the encoded message */
   msgbuf[i++] = 0;
   msgbuf[i++] = 0;
  
   if(!call->isTunnelingActive)
   {
      /* Populate message buffer to be returned */
      len =  4;
      msgbuf[i++] = 3; /* TPKT version */
      msgbuf[i++] = 0; /* TPKT resevred */
      /* 1st octet of length, will be populated once len is determined */
      msgbuf[i++] = 0;
      /* 2nd octet of length, will be populated once len is determined */
      msgbuf[i++] = 0;
   }
  
   setPERBuffer (pctxt, msgbuf+i, (size-i), TRUE);

   stat = asn1PE_H245MultimediaSystemControlMessage (&gH323ep.msgctxt,
                                                            multimediaMsg);

   if (stat != ASN_OK) {
      OOTRACEERR3 ("ERROR: H245 Message encoding failed (%s, %s)\n",
                   call->callType, call->callToken);
      OOTRACEERR1 (errGetText (&gH323ep.msgctxt));
      return OO_FAILED;
   }
  
   encodePtr = encodeGetMsgPtr(pctxt, &encodeLen);
   len +=encodeLen;
   msgbuf[1] = (len>>8);
   msgbuf[2] = len;
   if(!call->isTunnelingActive)
   {
      msgbuf[5] = len>>8;
      msgbuf[6] = len;
   }
#ifndef _COMPACT
   ooPrintH245Message (call, encodePtr, encodeLen);
#endif
   return OO_OK;
}

int ooSendTermCapMsg(ooCallData *call)
{
   int ret;
   H245RequestMessage * request;
   OOCTXT *pctxt;
   ooH323EpCapability *epCap;
   H245TerminalCapabilitySet *termCap;
   H245AudioCapability *audioCap;
   H245AudioTelephonyEventCapability *ateCap=NULL;
   H245CapabilityTableEntry *entry;
   H245AlternativeCapabilitySet *altSet;
   DListNode * curNode=NULL;
   H245CapabilityDescriptor *capDesc;
   H245Message *ph245msg=NULL;

   int i=0, j=0, k=0;

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
   pctxt = &gH323ep.msgctxt;
   ph245msg->msgType = OOTerminalCapabilitySet;
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

   /* Add audio Capabilities */

   dListInit(&(termCap->capabilityTable));
   for(k=0; k<(int)call->capPrefs.index; k++)
   {
      epCap = gH323ep.myCaps;
      while(epCap) {
          if(epCap->cap == call->capPrefs.order[k])
            break;
         epCap = epCap->next;
      }
      if(!epCap)
      {
         OOTRACEWARN4("WARN:Preferred capability %d not supported.(%s, %s)\n",
                     call->capPrefs.order[k],call->callType, call->callToken);
         continue;
      }

      /* Create audio capability. If capability supports receive, we only add
         it as receive capability in TCS. However, if it supports only
         transmit, we add it as transmit capability in TCS.
      */
      if((epCap->dir & OORX))
      {
         OOTRACEDBGC4("Sending receive capability %d in TCS.(%s, %s)\n",
                       epCap->cap, call->callType, call->callToken);
         audioCap = ooCreateAudioCapability(epCap, pctxt, OORX);
         if(!audioCap)
         {
            OOTRACEWARN3("WARN:Failed to create audio capability (%s, %s)",
                        call->callType, call->callToken);
            continue;
         }
      }else if(epCap->dir & OOTX)
      {
         OOTRACEDBGC4("Sending transmit capability %d in TCS.(%s, %s)\n",
                       epCap->cap, call->callType, call->callToken);
         audioCap = ooCreateAudioCapability(epCap, pctxt, OOTX);
         if(!audioCap)
         {
            OOTRACEWARN3("WARN:Failed to create audio capability (%s, %s)",
                        call->callType, call->callToken);
            continue;
         }    
      }else{
         OOTRACEWARN3("Warn:Capability is not RX/TX/RXANDTX. Symmetric "
                      "capabilities are not supported.(%s, %s)\n",
                      call->callType, call->callToken);
          continue;
      }
      /* Add  Capabilities to Capability Table */
      entry = (H245CapabilityTableEntry*) ASN1MALLOC(pctxt,
                      sizeof(H245CapabilityTableEntry));
      memset(entry, 0, sizeof(H245CapabilityTableEntry));
      entry->m.capabilityPresent = 1;
      if((epCap->dir & OORX))
      {
         entry->capability.t = T_H245Capability_receiveAudioCapability;
         entry->capability.u.receiveAudioCapability = audioCap;
      }else{
         entry->capability.t = T_H245Capability_transmitAudioCapability;
         entry->capability.u.transmitAudioCapability = audioCap;
      }
      entry->capabilityTableEntryNumber = i+1;
      dListAppend(pctxt , &(termCap->capabilityTable), entry);
      i++;
   }
   /* Add dtmf capability, if any */
   if(call->dtmfmode & OO_CAP_DTMF_RFC2833)
   {
      ateCap = (H245AudioTelephonyEventCapability*)ooCreateDTMFCapability(
                                                   OO_CAP_DTMF_RFC2833, pctxt);
      if(!ateCap)
      {
         OOTRACEWARN3("WARN:Failed to add RFC2833 cap to TCS(%s, %s)\n",
                     call->callType, call->callToken);
      }else {
         entry = (H245CapabilityTableEntry*) ASN1MALLOC(pctxt,
                      sizeof(H245CapabilityTableEntry));
         if(!entry)
         {
            OOTRACEERR3("Error:Failed to allocate memory for new capability "
                        "table entry. (%s, %s)\n", call->callType,
                        call->callToken);
            ooFreeH245Message(call, ph245msg);
            return OO_FAILED;
         }
           
         memset(entry, 0, sizeof(H245CapabilityTableEntry));
         entry->m.capabilityPresent = 1;

         entry->capability.t = T_H245Capability_receiveRTPAudioTelephonyEventCapability;
         entry->capability.u.receiveRTPAudioTelephonyEventCapability = ateCap;
     
         entry->capabilityTableEntryNumber = i+1;
         dListAppend(pctxt , &(termCap->capabilityTable), entry);

         i++;
      }
   }     
   /*TODO:Add Video and Data capabilities, if required*/
   if(i==0)
   {
      OOTRACEERR3("Error:No capabilities found to send in TCS message."
                  " (%s, %s)\n", call->callType, call->callToken);
      ooFreeH245Message(call,ph245msg);
      return OO_FAILED;
   }
          
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

   OOTRACEDBGA3("Built terminal capability set message (%s, %s)\n",
                 call->callType, call->callToken);
   ret = ooSendH245Msg(call, ph245msg);
   if(ret != OO_OK)
   {
      OOTRACEERR3("Error:Failed to enqueue TCS message to outbound queue. (%s, %s)\n",
                   call->callType, call->callToken);
   }else
      call->localTermCapState = OO_LocalTermCapSetSent;
  
   ooFreeH245Message(call,ph245msg);

   return ret;
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
            ooSendMasterSlaveDeterminationReject (call);

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
        
         if(call->localTermCapState == OO_LocalTermCapSetAckRecvd &&
            call->remoteTermCapState == OO_RemoteTermCapSetAckSent)
         {
            /*Since Cap exchange and MasterSlave Procedures are done */
            if(!call->logicalChans)
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
   OOCTXT *pctxt=&gH323ep.msgctxt;
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
            ASN1MALLOC(pctxt, sizeof(H245MasterSlaveDetermination));

  
   pMasterSlave = request->u.masterSlaveDetermination;
   memset(pMasterSlave, 0, sizeof(H245MasterSlaveDetermination));  
   pMasterSlave->terminalType = gH323ep.termType;
   pMasterSlave->statusDeterminationNumber =
                       ooGenerateStatusDeterminationNumber();
   call->statusDeterminationNumber = pMasterSlave->statusDeterminationNumber;

   OOTRACEDBGA3("Built MasterSlave Determination (%s, %s)\n", call->callType,
                 call->callToken);
   ret = ooSendH245Msg(call, ph245msg);

   if(ret != OO_OK)
   {
      OOTRACEERR3("Error:Failed to enqueue MasterSlaveDetermination message to outbound queue. (%s, %s)\n",
                      call->callType, call->callToken);
   }else
      call->masterSlaveState = OO_MasterSlave_DetermineSent;
  
   ooFreeH245Message(call, ph245msg);

   return ret;
}

int ooSendMasterSlaveDeterminationAck(ooCallData* call,
                                      char * status)
{
   int ret=0;
   H245ResponseMessage * response=NULL;
   H245Message *ph245msg=NULL;
   OOCTXT *pctxt=&gH323ep.msgctxt;

   ret = ooCreateH245Message(&ph245msg,
                      T_H245MultimediaSystemControlMessage_response);
   if(ret != OO_OK)
   {
      OOTRACEERR3("Error:H245 message creation failed for - MasterSlave "
                  "Determination Ack (%s, %s)\n",call->callType,
                  call->callToken);
      return OO_FAILED;
   }
   ph245msg->msgType = OOMasterSlaveAck;
   response = ph245msg->h245Msg.u.response;
   memset(response, 0, sizeof(H245ResponseMessage));
   response->t = T_H245ResponseMessage_masterSlaveDeterminationAck;
   response->u.masterSlaveDeterminationAck = (H245MasterSlaveDeterminationAck*)
                   ASN1MALLOC(pctxt, sizeof(H245MasterSlaveDeterminationAck));
   memset(response->u.masterSlaveDeterminationAck, 0,
                             sizeof(H245MasterSlaveDeterminationAck));
   if(!strcmp("master", status))
      response->u.masterSlaveDeterminationAck->decision.t =
                         T_H245MasterSlaveDeterminationAck_decision_master;
   else
      response->u.masterSlaveDeterminationAck->decision.t =
                         T_H245MasterSlaveDeterminationAck_decision_slave;
  
   OOTRACEDBGA3("Built MasterSlave determination Ack (%s, %s)\n",
                call->callType, call->callToken);
   ret = ooSendH245Msg(call, ph245msg);
   if(ret != OO_OK)
   {
      OOTRACEERR3("Error:Failed to enqueue MasterSlaveDeterminationAck message to outbound queue."
                  " (%s, %s)\n", call->callType, call->callToken);
   }
  
   ooFreeH245Message(call, ph245msg);
   return ret;
}

int ooSendMasterSlaveDeterminationReject (ooCallData* call)
{
   int ret=0;
   H245ResponseMessage* response=NULL;
   H245Message *ph245msg=NULL;
   OOCTXT *pctxt=&gH323ep.msgctxt;

   ret = ooCreateH245Message
      (&ph245msg, T_H245MultimediaSystemControlMessage_response);

   if (ret != OO_OK) {
      OOTRACEERR3("Error:H245 message creation failed for - MasterSlave "
                  "Determination Reject (%s, %s)\n",call->callType,
                  call->callToken);
      return OO_FAILED;
   }
   ph245msg->msgType = OOMasterSlaveReject;
   response = ph245msg->h245Msg.u.response;

   response->t = T_H245ResponseMessage_masterSlaveDeterminationReject;

   response->u.masterSlaveDeterminationReject =
      (H245MasterSlaveDeterminationReject*)
      memAlloc (pctxt, sizeof(H245MasterSlaveDeterminationReject));

   response->u.masterSlaveDeterminationReject->cause.t =
      T_H245MasterSlaveDeterminationReject_cause_identicalNumbers;

   OOTRACEDBGA3 ("Built MasterSlave determination reject (%s, %s)\n",
                 call->callType, call->callToken);

   ret = ooSendH245Msg (call, ph245msg);

   if (ret != OO_OK) {
      OOTRACEERR3
         ("Error:Failed to enqueue MasterSlaveDeterminationReject "
          "message to outbound queue.(%s, %s)\n", call->callType,
          call->callToken);
   }
  
   ooFreeH245Message (call, ph245msg);

   return ret;
}

int ooHandleOpenLogicalChannel(ooCallData* call,
                                 H245OpenLogicalChannel *olc)
{

   H245OpenLogicalChannel_forwardLogicalChannelParameters *flcp =
    &(olc->forwardLogicalChannelParameters);
  
   if(!call->logicalChans)
      ooOpenLogicalChannels(call);
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
                  "'T_H245DataType_multiplexedStream' not supported(%s, %s)\n",
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
   default:
      OOTRACEERR3("ERROR:Unknown media channel data type (%s, %s)\n",
                   call->callType, call->callToken);
   }
  
   return OO_OK;
}      


int ooHandleOpenLogicalAudioChannel(ooCallData *call,
                                    H245OpenLogicalChannel*olc)
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
   ooLogicalChannel *pLogicalChannel = NULL;
  
   H245OpenLogicalChannel_forwardLogicalChannelParameters *flcp =
    &(olc->forwardLogicalChannelParameters);
   if(call->noOfLogicalChannels>=5)
   {
      OOTRACEERR3("ERROR:Can not open more than 5 logical channels per call "
                  "(%s, %s)\n", call->callType, call->callToken);
      return OO_FAILED;
   }
   if(!(epCap=ooIsAudioDataTypeSupported(call, flcp->dataType.u.audioData,
                                     OORX)))
   {
      OOTRACEERR3("ERROR:OpenLogicalChannel - audio capability not supported "
                  "(%s, %s)\n", call->callType, call->callToken);
      return OO_FAILED;
   }
   /* Generate an Ack for the open channel request */
   ret = ooCreateH245Message(&ph245msg,
                             T_H245MultimediaSystemControlMessage_response);
   if(ret != OO_OK)
   {
      OOTRACEERR3("Error: H245 message creation failed for - "
                  "OpenLogicalChannel Ack (%s, %s)\n", call->callType,
                  call->callToken);
      return OO_FAILED;
   }

   ph245msg->msgType = OOOpenLogicalChannelAck;
   response = ph245msg->h245Msg.u.response;
   pctxt = &gH323ep.msgctxt;
   memset(response, 0, sizeof(H245ResponseMessage));
   response->t = T_H245ResponseMessage_openLogicalChannelAck;
   response->u.openLogicalChannelAck = (H245OpenLogicalChannelAck*)
                   ASN1MALLOC(pctxt, sizeof(H245OpenLogicalChannelAck));  
   olcAck = response->u.openLogicalChannelAck;
   memset(olcAck, 0, sizeof(H245OpenLogicalChannelAck));
   olcAck->forwardLogicalChannelNumber = olc->forwardLogicalChannelNumber;

   olcAck->m.forwardMultiplexAckParametersPresent = 1;
   olcAck->forwardMultiplexAckParameters.t =
     T_H245OpenLogicalChannelAck_forwardMultiplexAckParameters_h2250LogicalChannelAckParameters;
   olcAck->forwardMultiplexAckParameters.u.h2250LogicalChannelAckParameters =
                      (H245H2250LogicalChannelAckParameters*)ASN1MALLOC(pctxt,
                      sizeof(H245H2250LogicalChannelAckParameters));
   h2250lcap =
      olcAck->forwardMultiplexAckParameters.u.h2250LogicalChannelAckParameters;
   memset(h2250lcap, 0, sizeof(H245H2250LogicalChannelAckParameters));

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

   pLogicalChannel = ooAddNewLogicalChannel(call,
                        olc->forwardLogicalChannelNumber, h2250lcap->sessionID,
                        "audio", "receive", epCap);
   if(!pLogicalChannel)
   {
      OOTRACEERR3("ERROR:Failed to add new logical channel entry to call "
                  "(%s, %s)\n", call->callType, call->callToken);
      return OO_FAILED;
   }
   ooConvertIpToNwAddr(call->localIP, iPAddress->network.data);

   iPAddress->network.numocts = 4;
   iPAddress->tsapIdentifier = pLogicalChannel->localRtpPort;

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

   ooConvertIpToNwAddr(call->localIP, iPAddress1->network.data);

   iPAddress1->network.numocts = 4;
   iPAddress1->tsapIdentifier = pLogicalChannel->localRtcpPort;

   OOTRACEDBGA3("Built OpenLogicalChannelAck (%s, %s)\n", call->callType,
                 call->callToken);
   ret = ooSendH245Msg(call, ph245msg);
   if(ret != OO_OK)
   {
      OOTRACEERR3("Error:Failed to enqueue OpenLogicalChannelAck message to outbound queue. (%s, %s)\n",
                  call->callType, call->callToken);
   }
   ooFreeH245Message(call, ph245msg);


   if(epCap->startReceiveChannel)
   {
      epCap->startReceiveChannel(call, pLogicalChannel);     
      OOTRACEINFO5("Receive channel of type audio started at %s:%d(%s, %s)\n",
                    call->localIP, pLogicalChannel->localRtpPort,
                    call->callType, call->callToken);
   }
   else{
      OOTRACEERR3("ERROR:No callback registered to start receive audio "
                  "channel (%s, %s)\n", call->callType, call->callToken);
      return OO_FAILED;
   }
   return ret;
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
                  "Ack parameters absent (%s, %s)\n", call->callType,
                  call->callToken);
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

   /* Set remote destination address for rtp session */
   strcpy(call->remoteIP, remoteip);
  
   /* Start channel here */
   pLogicalChannel = ooFindLogicalChannelByLogicalChannelNo(call,olcAck->forwardLogicalChannelNumber);
   if(!pLogicalChannel)
   {
      OOTRACEERR4("ERROR:Logical channel %d not found in the channel list for "
                  "call (%s, %s)\n", olcAck->forwardLogicalChannelNumber,
                  call->callType, call->callToken);
      return OO_FAILED;
   }
   /* Populate ports for channel */
   pLogicalChannel->remoteRtpPort = iPAddress->tsapIdentifier;
   pLogicalChannel->remoteRtcpPort = iPAddress1->tsapIdentifier;

   if(pLogicalChannel->chanCap->startTransmitChannel)
   {
      pLogicalChannel->chanCap->startTransmitChannel(call, pLogicalChannel);
      OOTRACEINFO4("TransmitLogical Channel of type %s started (%s, %s)\n",
                   pLogicalChannel->type,call->callType, call->callToken);
   }
   else{
      OOTRACEERR3("ERROR:No callback registered for starting transmit channel "
                  "(%s, %s)\n", call->callType, call->callToken);
      return OO_FAILED;
   }
   pLogicalChannel->state = OO_LOGICALCHAN_ESTABLISHED;
   return OO_OK;
}

int ooOnReceivedOpenLogicalChannelRejected(ooCallData *call,
                                     H245OpenLogicalChannelReject *olcReject)
{
   switch(olcReject->cause.t)
   {
   case T_H245OpenLogicalChannelReject_cause_unspecified:
      OOTRACEINFO4("Open logical channel %d rejected - unspecified (%s, %s)\n",
                   olcReject->forwardLogicalChannelNumber, call->callType,
                   call->callToken);
      break;
   case T_H245OpenLogicalChannelReject_cause_unsuitableReverseParameters:
      OOTRACEINFO4("Open logical channel %d rejected - "
                   "unsuitableReverseParameters (%s, %s)\n",
                   olcReject->forwardLogicalChannelNumber, call->callType,
                   call->callToken);
      break;
   case T_H245OpenLogicalChannelReject_cause_dataTypeNotSupported:
      OOTRACEINFO4("Open logical channel %d rejected - dataTypeNotSupported"
                   "(%s, %s)\n", olcReject->forwardLogicalChannelNumber,
                   call->callType, call->callToken);
      break;
   case T_H245OpenLogicalChannelReject_cause_dataTypeNotAvailable:
      OOTRACEINFO4("Open logical channel %d rejected - dataTypeNotAvailable"
                   "(%s, %s)\n", olcReject->forwardLogicalChannelNumber,
                   call->callType, call->callToken);
      break;
   case T_H245OpenLogicalChannelReject_cause_unknownDataType:
      OOTRACEINFO4("Open logical channel %d rejected - unknownDataType"
                   "(%s, %s)\n", olcReject->forwardLogicalChannelNumber,
                   call->callType, call->callToken);
      break;
   case T_H245OpenLogicalChannelReject_cause_dataTypeALCombinationNotSupported:
      OOTRACEINFO4("Open logical channel %d rejected - "
                   "dataTypeALCombinationNotSupported(%s, %s)\n",
                   olcReject->forwardLogicalChannelNumber,
                   call->callType, call->callToken);
      break;
   case T_H245OpenLogicalChannelReject_cause_multicastChannelNotAllowed:
       OOTRACEINFO4("Open logical channel %d rejected - "
                    "multicastChannelNotAllowed (%s, %s)\n",
                    olcReject->forwardLogicalChannelNumber,
                    call->callType, call->callToken);
      break;
   case T_H245OpenLogicalChannelReject_cause_insufficientBandwidth:
      OOTRACEINFO4("Open logical channel %d rejected - insufficientBandwidth"
                   "(%s, %s)\n", olcReject->forwardLogicalChannelNumber,
                   call->callType, call->callToken);
      break;
   case T_H245OpenLogicalChannelReject_cause_separateStackEstablishmentFailed:
      OOTRACEINFO4("Open logical channel %d rejected - "
                    "separateStackEstablishmentFailed (%s, %s)\n",
                    olcReject->forwardLogicalChannelNumber,
                    call->callType, call->callToken);
      break;
   case T_H245OpenLogicalChannelReject_cause_invalidSessionID:
      OOTRACEINFO4("Open logical channel %d rejected - "
                    "invalidSessionID (%s, %s)\n",
                    olcReject->forwardLogicalChannelNumber,
                    call->callType, call->callToken);
      break;
   case T_H245OpenLogicalChannelReject_cause_masterSlaveConflict:
      OOTRACEINFO4("Open logical channel %d rejected - "
                    "invalidSessionID (%s, %s)\n",
                    olcReject->forwardLogicalChannelNumber,
                    call->callType, call->callToken);
      break;
   case T_H245OpenLogicalChannelReject_cause_waitForCommunicationMode:
      OOTRACEINFO4("Open logical channel %d rejected - "
                    "waitForCommunicationMode (%s, %s)\n",
                    olcReject->forwardLogicalChannelNumber,
                    call->callType, call->callToken);
      break;
   case T_H245OpenLogicalChannelReject_cause_invalidDependentChannel:
      OOTRACEINFO4("Open logical channel %d rejected - "
                    "invalidDependentChannel (%s, %s)\n",
                    olcReject->forwardLogicalChannelNumber,
                    call->callType, call->callToken);
      break;
   case T_H245OpenLogicalChannelReject_cause_replacementForRejected:
      OOTRACEINFO4("Open logical channel %d rejected - "
                    "replacementForRejected (%s, %s)\n",
                    olcReject->forwardLogicalChannelNumber,
                    call->callType, call->callToken);
      break;
   default:
      OOTRACEERR4("Error: OpenLogicalChannel %d rejected - "
                  "invalid cause(%s, %s)\n",
                   olcReject->forwardLogicalChannelNumber,
                    call->callType, call->callToken);
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
   pctxt = &gH323ep.msgctxt;
   memset(command, 0, sizeof(H245CommandMessage));
   command->t = T_H245CommandMessage_endSessionCommand;
   command->u.endSessionCommand = (H245EndSessionCommand*) ASN1MALLOC(pctxt,
                                  sizeof(H245EndSessionCommand));
   memset(command->u.endSessionCommand, 0, sizeof(H245EndSessionCommand));
   command->u.endSessionCommand->t = T_H245EndSessionCommand_disconnect;
   OOTRACEDBGA3("Built EndSession Command (%s, %s)\n", call->callType,
                call->callToken);
   ret = ooSendH245Msg(call, ph245msg);
   if(ret != OO_OK)
   {
      OOTRACEERR3("Error:Failed to enqueue EndSession message to outbound queue.(%s, %s)\n",
                   call->callType, call->callToken);
   }
   ooFreeH245Message(call, ph245msg);
   return ret;
}


int ooHandleH245Command(ooCallData *call,
                        H245CommandMessage *command)
{
   OOTRACEDBGC3("Handling H.245 command message. (%s, %s)\n", call->callType,
                 call->callToken);
   switch(command->t)
   {
      case T_H245CommandMessage_endSessionCommand:
         OOTRACEINFO3("Received EndSession command (%s, %s)\n",
                       call->callType, call->callToken);
         ooCloseH245Connection(call);
         call->callState = OO_CALL_CLEAR_ENDSESSION;
         break;
      case T_H245CommandMessage_sendTerminalCapabilitySet:
         OOTRACEWARN3("Warning: Received command Send terminal capability set "
                      "- Not handled (%s, %s)\n", call->callType,
                      call->callToken);
         break;
      case T_H245CommandMessage_flowControlCommand:
         OOTRACEWARN3("Warning: Flow control command received - Not handled "
                      "(%s, %s)\n", call->callType, call->callToken);
         break;
      default:
         OOTRACEWARN3("Warning: Unhandled H245 command message received "
                      "(%s, %s)\n", call->callType, call->callToken);
   }
   OOTRACEDBGC3("Handling H.245 command message done. (%s, %s)\n",
                 call->callType, call->callToken);  
   return OO_OK;
}


int ooOnReceivedTerminalCapabilitySetAck(ooCallData* call)
{
   call->localTermCapState = OO_LocalTermCapSetAckRecvd;
   if(call->remoteTermCapState != OO_RemoteTermCapSetAckSent)
      return OO_OK;
  
   if(call->masterSlaveState == OO_MasterSlave_Master ||
       call->masterSlaveState == OO_MasterSlave_Slave)
   {
      if(!call->logicalChans)
         ooOpenLogicalChannels(call);
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
      if(temp->state == OO_LOGICALCHAN_ESTABLISHED)
      {
         /* Sending closelogicalchannel only for outgoing channels*/
         if(!strcmp(temp->dir, "transmit"))
         {
            ooSendCloseLogicalChannel(call, temp);
         }
         else{
            ooSendRequestCloseLogicalChannel(call, temp);
         }
      }
      temp = temp->next;
   }
   return OO_OK;
}

int ooSendCloseLogicalChannel(ooCallData *call, ooLogicalChannel *logicalChan)
{
   int ret = OO_OK, error=0;
   H245Message *ph245msg = NULL;
   OOCTXT *pctxt;
   H245RequestMessage *request;
   H245CloseLogicalChannel* clc;
  
   ret = ooCreateH245Message(&ph245msg,
                             T_H245MultimediaSystemControlMessage_request);
   if(ret != OO_OK)
   {
      OOTRACEERR3("ERROR:Failed to create H245 message for closeLogicalChannel"
                  " message (%s, %s)\n", call->callType, call->callToken);
      return OO_FAILED;
   }
   ph245msg->msgType = OOCloseLogicalChannel;
   pctxt = &gH323ep.msgctxt;
   request = ph245msg->h245Msg.u.request;

   request->t = T_H245RequestMessage_closeLogicalChannel;
   request->u.closeLogicalChannel = (H245CloseLogicalChannel*)ASN1MALLOC(pctxt,
                                     sizeof(H245CloseLogicalChannel));
   if(!request->u.closeLogicalChannel)
   {
      OOTRACEERR3("ERROR:Memory allocation for CloseLogicalChannel failed "
                  "(%s, %s)\n", call->callType, call->callToken);
      ooFreeH245Message(call, ph245msg);
      return OO_FAILED;
   }
   clc = request->u.closeLogicalChannel;
   memset(clc, 0, sizeof(H245CloseLogicalChannel));

   clc->forwardLogicalChannelNumber = logicalChan->channelNo;
   clc->source.t = T_H245CloseLogicalChannel_source_lcse;
   clc->m.reasonPresent = 1;
   clc->reason.t = T_H245CloseLogicalChannel_reason_unknown;

   OOTRACEDBGA4("Built close logical channel for %d (%s, %s)\n",
                 logicalChan->channelNo, call->callType, call->callToken);
   ret = ooSendH245Msg(call, ph245msg);
   if(ret != OO_OK)
   {
     OOTRACEERR3("Error:Failed to enqueue CloseLogicalChannel to outbound queue.(%s, %s)\n", call->callType,
                 call->callToken);
     error++;
   }
   ooFreeH245Message(call, ph245msg);
  
   /* Stop the media transmission */
   OOTRACEINFO4("Closing logical channel %d (%s, %s)\n",
                clc->forwardLogicalChannelNumber, call->callType,
                call->callToken);
   ret = ooClearLogicalChannel(call, clc->forwardLogicalChannelNumber);
   if(ret != OO_OK)
   {
      OOTRACEERR4("ERROR:Failed to close logical channel %d (%s, %s)\n",
         clc->forwardLogicalChannelNumber, call->callType, call->callToken);
      return OO_FAILED;
   }
   if(error) return OO_FAILED;
   return ret;
}

/*TODO: Need to pass reason as a parameter */
int ooSendRequestCloseLogicalChannel(ooCallData *call,
                                     ooLogicalChannel *logicalChan)
{
   int ret = OO_OK;
   H245Message *ph245msg = NULL;
   OOCTXT *pctxt;
   H245RequestMessage *request;
   H245RequestChannelClose *rclc;

   ret = ooCreateH245Message(&ph245msg,
                             T_H245MultimediaSystemControlMessage_request);
   if(ret != OO_OK)
   {
      OOTRACEERR3("ERROR:Failed to create H245 message for "
                  "requestCloseLogicalChannel message (%s, %s)\n",
                   call->callType, call->callToken);
      return OO_FAILED;
   }
   ph245msg->msgType = OORequestChannelClose;
   pctxt = &gH323ep.msgctxt;
   request = ph245msg->h245Msg.u.request;

   request->t = T_H245RequestMessage_requestChannelClose;
   request->u.requestChannelClose = (H245RequestChannelClose*)ASN1MALLOC(pctxt,
                                     sizeof(H245RequestChannelClose));
   if(!request->u.requestChannelClose)
   {
      OOTRACEERR3("ERROR:Memory allocation for RequestCloseLogicalChannel "
                  " failed (%s, %s)\n", call->callType, call->callToken);
      ooFreeH245Message(call, ph245msg);
      return OO_FAILED;
   }

   rclc = request->u.requestChannelClose;
   memset(rclc, 0, sizeof(H245RequestChannelClose));
   rclc->forwardLogicalChannelNumber = logicalChan->channelNo;
  
   rclc->m.reasonPresent = 1;
   rclc->reason.t = T_H245RequestChannelClose_reason_unknown;

   OOTRACEDBGA4("Built RequestCloseChannel for %d (%s, %s)\n",
                 logicalChan->channelNo, call->callType, call->callToken);
   ret = ooSendH245Msg(call, ph245msg);
   if(ret != OO_OK)
   {
     OOTRACEERR3("Error:Failed to enqueue the RequestCloseChannel to outbound"
                 " queue (%s, %s)\n", call->callType,
                 call->callToken);
   }
   ooFreeH245Message(call, ph245msg);

   return ret;
}

int ooOnReceivedRequestChannelClose(ooCallData *call,
                                    H245RequestChannelClose *rclc)
{
   int ret=0, error=0;
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
   ret = ooCreateH245Message(&ph245msg,
                             T_H245MultimediaSystemControlMessage_response);
   if(ret != OO_OK)
   {
      OOTRACEERR3("ERROR:Memory allocation for RequestChannelCloseAck message "
                  "failed (%s, %s)\n", call->callType, call->callToken);
      return OO_FAILED;
   }
   pctxt = &gH323ep.msgctxt;
   ph245msg->msgType = OORequestChannelCloseAck;
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

   OOTRACEDBGA3("Built RequestCloseChannelAck message (%s, %s)\n",
                 call->callType, call->callToken);
   ret = ooSendH245Msg(call, ph245msg);
   if(ret != OO_OK)
   {
      OOTRACEERR3("Error:Failed to enqueue RequestCloseChannelAck to outbound queue. (%s, %s)\n", call->callType,
                 call->callToken);
      error++;
   }

   ooFreeH245Message(call, ph245msg);
  
   /* Send Close Logical Channel*/
   ret = ooSendCloseLogicalChannel(call, lChannel);
   if(ret != OO_OK)
   {
      OOTRACEERR3("ERROR:Failed to build CloseLgicalChannel message(%s, %s)\n",
                   call->callType, call->callToken);
      return OO_FAILED;
   }

   if(error) return OO_FAILED;

   return ret;
}
/****/
int ooOnReceivedCloseLogicalChannel(ooCallData *call,
                                    H245CloseLogicalChannel* clc)
{
   int ret=0;
   ooH323EpCapability *epCap=NULL;
   ooLogicalChannel *pLogicalChannel=NULL;
   H245Message *ph245msg = NULL;
   OOCTXT *pctxt = NULL;
   H245CloseLogicalChannelAck * clcAck;
   H245ResponseMessage *response;
  
   OOTRACEINFO4("Closing logical channel number %d (%s, %s)\n",
      clc->forwardLogicalChannelNumber, call->callType, call->callToken);
  
   ret = ooClearLogicalChannel(call, clc->forwardLogicalChannelNumber);
   if(ret != OO_OK)
   {
      OOTRACEERR4("ERROR:Failed to close logical channel %d (%s, %s)\n",
         clc->forwardLogicalChannelNumber, call->callType, call->callToken);
      return OO_FAILED;
   }

   ret = ooCreateH245Message(&ph245msg,
                              T_H245MultimediaSystemControlMessage_response);
   if(ret != OO_OK)
   {
      OOTRACEERR3("ERROR:Failed to create H245 message for "
                  "closeLogicalChannelAck (%s, %s)\n", call->callType,
                  call->callToken);
      return OO_FAILED;
   }
   pctxt = &gH323ep.msgctxt;
   ph245msg->msgType = OOCloseLogicalChannelAck;
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

   OOTRACEDBGA3("Built CloseLogicalChannelAck message (%s, %s)\n",
                 call->callType, call->callToken);
   ret = ooSendH245Msg(call, ph245msg);
   if(ret != OO_OK)
   {
     OOTRACEERR3("Error:Failed to enqueue CloseLogicalChannelAck message to outbound queue.(%s, %s)\n",
                 call->callType, call->callToken);
   }

   ooFreeH245Message(call, ph245msg);
   return ret;
}

int ooOnReceivedCloseChannelAck(ooCallData* call,
                                H245CloseLogicalChannelAck* clcAck)
{
   int ret = OO_OK;
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
  
   OOTRACEDBGC3("Handling H245 message. (%s, %s)\n", call->callType,
                 call->callToken);
  
   switch(pH245->h245Msg.t)
   {
     /* H.245 Request message is received */
      case (T_H245MultimediaSystemControlMessage_request):
         request = pH245->h245Msg.u.request;
         switch(request->t)
         {
         case T_H245RequestMessage_terminalCapabilitySet:
           
            ooOnReceivedTerminalCapabilitySet(call, pH245);
            if(call->localTermCapState == OO_LocalTermCapExchange_Idle)
               ooSendTermCapMsg(call);
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
            ooOnReceivedCloseLogicalChannel(call,
                                            request->u.closeLogicalChannel);
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
            break;
         case T_H245ResponseMessage_terminalCapabilitySetAck:
            ooOnReceivedTerminalCapabilitySetAck(call);
            break;
         case T_H245ResponseMessage_openLogicalChannelAck:
            ooOnReceivedOpenLogicalChannelAck(call,
                                           response->u.openLogicalChannelAck);
            break;
         case T_H245ResponseMessage_openLogicalChannelReject:
            OOTRACEINFO3("Open Logical Channel Reject received (%s, %s)\n",
                          call->callType, call->callToken);
            ooOnReceivedOpenLogicalChannelRejected(call,
                                     response->u.openLogicalChannelReject);
            break;
         case T_H245ResponseMessage_closeLogicalChannelAck:
            OOTRACEINFO4("CloseLogicalChannelAck received for %d (%s, %s)\n",
               response->u.closeLogicalChannelAck->forwardLogicalChannelNumber,
               call->callType, call->callToken);
            ooOnReceivedCloseChannelAck(call,
                                        response->u.closeLogicalChannelAck);
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
      case (T_H245MultimediaSystemControlMessage_indication):
         break;
      default:
        ;
   }
   OOTRACEDBGC3("Finished handling H245 message. (%s, %s)\n",
                 call->callType, call->callToken);
   return OO_OK;
}

int ooOnReceivedTerminalCapabilitySet(ooCallData *call, H245Message *pmsg)
{
  int ret = 0,k;
   OOCTXT *pctxt=NULL;
   H245TerminalCapabilitySet *tcs=NULL;
   DListNode *pNode=NULL;
   H245CapabilityTableEntry *capEntry = NULL;

   tcs =  pmsg->h245Msg.u.request->u.terminalCapabilitySet;
   if(call->remoteTermCapSeqNo >= tcs->sequenceNumber)
   {
      OOTRACEINFO4("Rejecting TermCapSet message with SeqNo %d, as already "
                   "acknowledged message with this SeqNo (%s, %s)\n",
                   call->remoteTermCapSeqNo, call->callType, call->callToken);
      return OO_OK;
   }
 
   if(!tcs->m.capabilityTablePresent)
   {
     OOTRACEWARN3("Warn:Ignoring TCS as no capability table present(%s, %s)\n",                   call->callType, call->callToken);
     return OO_OK;
   }
   call->remoteTermCapSeqNo = tcs->sequenceNumber;

   for(k=0; k<(int)tcs->capabilityTable.count; k++)
   {
      pNode = dListFindByIndex(&tcs->capabilityTable, k);
      if(pNode)
      {
         capEntry = (H245CapabilityTableEntry*) pNode->data;
         if(capEntry->m.capabilityPresent){
            ret =  ooAddRemoteCapability(call, &capEntry->capability);
            if(ret != OO_OK)
            {
               OOTRACEERR4("Error:Failed to process remote capability in "
                           "capability table at index %d. (%s, %s)\n",
                            k, call->callType, call->callToken);
            }
         }
      }
      pNode = NULL;
      capEntry=NULL;
   }

   /* Update remoteTermCapSetState */
   call->remoteTermCapState = OO_RemoteTermCapSetRecvd;

   ooH245AcknowledgeTerminalCapabilitySet(call);  

   if(call->remoteTermCapState != OO_RemoteTermCapSetAckSent ||
      call->localTermCapState  != OO_LocalTermCapSetAckRecvd)
      return OO_OK;

   /* Check whether MasterSlave Procedure has finished */
   if(call->masterSlaveState != OO_MasterSlave_Master &&
      call->masterSlaveState != OO_MasterSlave_Slave)
      return OO_OK;

   /* As both MasterSlave and TerminalCapabilitySet procedures have finished,
      OpenLogicalChannels*/
   if(!call->logicalChans)
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
   pctxt = &gH323ep.msgctxt;
   response->t = T_H245ResponseMessage_terminalCapabilitySetAck;
  
   response->u.terminalCapabilitySetAck = (H245TerminalCapabilitySetAck*)
                   ASN1MALLOC(pctxt, sizeof(H245TerminalCapabilitySetAck));

   memset(response->u.terminalCapabilitySetAck, 0,
                                 sizeof(H245TerminalCapabilitySetAck));
   response->u.terminalCapabilitySetAck->sequenceNumber = call->remoteTermCapSeqNo;

   OOTRACEDBGA3("Built TerminalCapabilitySet Ack (%s, %s)\n",
                 call->callType, call->callToken);
   ret = ooSendH245Msg(call, ph245msg);

   if(ret != OO_OK)
   {
     OOTRACEERR3("Error:Failed to enqueue TCSAck to outbound queue. (%s, %s)\n", call->callType, call->callToken);
   }else
      call->remoteTermCapState = OO_RemoteTermCapSetAckSent;

   ooFreeH245Message(call, ph245msg);
   return ret;
}

int ooOpenLogicalChannels(ooCallData *call)
{
   int ret=0;
   OOTRACEINFO3("Opening logical channels (%s, %s)\n", call->callType,
                 call->callToken);

   /* Audio channels */
   if(gH323ep.callMode == OO_CALLMODE_AUDIOCALL ||
      gH323ep.callMode == OO_CALLMODE_AUDIOTX)
   {
      if(!call->isAudioActive)
      {
         ret = ooOpenLogicalAudioChannel(call);
         call->isAudioActive = 1;
      }
   }
  
   return OO_OK;
}

int ooOpenLogicalAudioChannel(ooCallData *call)
{
   ooH323EpCapability *epCap=NULL, *repCap=NULL;
   int k=0;

   /* Check whether local endpoint has audio capability */
   if(gH323ep.myCaps == 0)
   {
      OOTRACEERR3("ERROR:Local endpoint does not have any audio capabilities"
                  " (%s, %s)\n", call->callType, call->callToken);
      return OO_FAILED;
   }
  
   /* Go through local endpoints capabilities sequentially, and find out the
      first one which has a match in the remote endpoints receive capabilities.
      TODO: Capability matching should find first preffered capability of
      master that slave supports.
   */
   OOTRACEINFO3("Looking for matching audio capabilities. (%s, %s)\n",
                 call->callType, call->callToken);

   for(k=0; k<call->capPrefs.index; k++)
   {
      epCap = gH323ep.myCaps;
      while(epCap){
         if(epCap->cap == call->capPrefs.order[k] &&
            ((epCap->dir & OOTX)))
            break;
         epCap = epCap->next;
      }
      if(!epCap)
      {
         OOTRACEDBGA4("Prefereed capability %d is not a local transmit "
                      "capability(%s, %s)\n", call->capPrefs.order[k],
                      call->callType, call->callToken);
         continue;
      }
      repCap = call->remoteCaps;
      while(repCap)
      {
         if(ooCheckCompatibility(call, epCap, repCap))
            break;
         repCap = repCap->next;
      }
      if(repCap) break;

   }
  
   if(!repCap)
   {
      OOTRACEERR3("ERROR:Incompatible audio capabilities - Can not open "
                  "audio channel (%s, %s)\n", call->callType, call->callToken);
      return OO_FAILED;
   }
   switch(epCap->cap)
   {
   case OO_G711ALAW64K:
   case OO_G711ALAW56K:
   case OO_G711ULAW64K:
   case OO_G711ULAW56K:
      ooOpenG711Channel(call, epCap);
      break;
   case OO_GSMFULLRATE:
   case OO_GSMHALFRATE:
   case OO_GSMENHANCEDFULLRATE:
   default:
      OOTRACEERR3("ERROR:Unknown Audio Capability type (%s, %s)\n",
                   call->callType, call->callToken);
   }
   return OO_OK;
}

int ooOpenG711Channel(ooCallData* call, ooH323EpCapability *epCap)
{
   int ret;
   H245Message *ph245msg = NULL;
   H245RequestMessage * request;
   DListNode *curNode1 = NULL;
   OOCTXT *pctxt;
   H245OpenLogicalChannel_forwardLogicalChannelParameters *flcp;
   H245AudioCapability *audioCap;
   H245H2250LogicalChannelParameters *h2250lcp;
   H245UnicastAddress *unicastAddrs;
   H245UnicastAddress_iPAddress *iPAddress;
   /*   char hexip[20];
        int addr_part1, addr_part2, addr_part3, addr_part4;*/
   ooLogicalChannel *pLogicalChannel = NULL;
  
  
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
   pctxt = &gH323ep.msgctxt;
   memset(request, 0, sizeof(H245RequestMessage));

   request->t = T_H245RequestMessage_openLogicalChannel;
   request->u.openLogicalChannel = (H245OpenLogicalChannel*)
                     ASN1MALLOC(pctxt, sizeof(H245OpenLogicalChannel));
   memset(request->u.openLogicalChannel, 0,
                                     sizeof(H245OpenLogicalChannel));
   request->u.openLogicalChannel->forwardLogicalChannelNumber = call->logicalChanNoCur++;
   if(call->logicalChanNoCur > call->logicalChanNoMax)
      call->logicalChanNoCur = call->logicalChanNoBase;
  
   pLogicalChannel = ooAddNewLogicalChannel(call,
                   request->u.openLogicalChannel->forwardLogicalChannelNumber,
                   1, "audio", "transmit", epCap);
  
   if(!pLogicalChannel)
   {
      OOTRACEERR3("ERROR:Failed to add new logical channel entry (%s, %s)\n",
                  call->callType, call->callToken);
      ooFreeH245Message(call, ph245msg);
      return OO_FAILED;
   }
   /* Populate H245OpenLogicalChannel_ForwardLogicalChannel Parameters*/
   flcp = &(request->u.openLogicalChannel->forwardLogicalChannelParameters);
   flcp->m.portNumberPresent = 0;
   flcp->m.forwardLogicalChannelDependencyPresent = 0;
   flcp->m.replacementForPresent = 0;

   /* data type of channel is audio */
   flcp->dataType.t = T_H245DataType_audioData;

   /* set audio capability for channel */
   audioCap = ooCreateAudioCapability(epCap,pctxt, OOTX);
   if(!audioCap)
   {
      OOTRACEERR3("Error:Failed to create duplicate audio capability in "
                  "ooOpenG711Channel. (%s, %s)\n", call->callType,
                  call->callToken);
      ooFreeH245Message(call, ph245msg);
      return OO_FAILED;
   }
  
   flcp->dataType.u.audioData = audioCap;
   flcp->multiplexParameters.t =
      T_H245OpenLogicalChannel_forwardLogicalChannelParameters_multiplexParameters_h2250LogicalChannelParameters;
   flcp->multiplexParameters.u.h2250LogicalChannelParameters =
                         (H245H2250LogicalChannelParameters*)ASN1MALLOC(pctxt,
                          sizeof(H245H2250LogicalChannelParameters));

   h2250lcp = flcp->multiplexParameters.u.h2250LogicalChannelParameters;
   memset(h2250lcp, 0, sizeof(H245H2250LogicalChannelParameters));

   h2250lcp->sessionID = 1;
   h2250lcp->mediaGuaranteedDelivery = 0;
   h2250lcp->silenceSuppression = 0;
   h2250lcp->m.mediaControlChannelPresent = 1;
  
   h2250lcp->mediaControlChannel.t =
                                 T_H245TransportAddress_unicastAddress;
   h2250lcp->mediaControlChannel.u.unicastAddress =  (H245UnicastAddress*)
                         ASN1MALLOC(pctxt, sizeof(H245UnicastAddress));

   unicastAddrs = h2250lcp->mediaControlChannel.u.unicastAddress;
   memset(unicastAddrs, 0, sizeof(H245UnicastAddress));
   unicastAddrs->t = T_H245UnicastAddress_iPAddress;
   unicastAddrs->u.iPAddress = (H245UnicastAddress_iPAddress*)
               ASN1MALLOC(pctxt, sizeof(H245UnicastAddress_iPAddress));
   iPAddress = unicastAddrs->u.iPAddress;
   memset(iPAddress, 0, sizeof(H245UnicastAddress_iPAddress));

   ooConvertIpToNwAddr(pLogicalChannel->localIP, iPAddress->network.data);

   iPAddress->network.numocts = 4;
   iPAddress->tsapIdentifier = pLogicalChannel->localRtcpPort;
   pLogicalChannel->state = OO_LOGICALCHAN_PROPOSED;
   OOTRACEDBGA3("Built OpenLogicalChannel (%s, %s)\n", call->callType,
                call->callToken);
   ret = ooSendH245Msg(call, ph245msg);
   if(ret != OO_OK)
   {
     OOTRACEERR3("Error:Failed to enqueue OpenLogicalChannel to outbound queue. (%s, %s)\n", call->callType,
                 call->callToken);
   }
   ooFreeH245Message(call, ph245msg);
 
    return ret;
}

int ooAddFastStartToSetup(ooCallData *call, H225Setup_UUIE *setup)
{
   return OO_OK;
}
/* Used to build Audio OLCs for fast connect. Keep in mind that forward and
   reverse
   are always with respect to the endpoint which proposes channels */
int ooBuildOpenLogicalChannelAudio
   (ooCallData *call, H245OpenLogicalChannel *olc, ooH323EpCapability *epCap,
    OOCTXT*pctxt, int dir)
{
   int reverse=0, forward=0;
   H245AudioCapability *audioCap=NULL;
   H245OpenLogicalChannel_forwardLogicalChannelParameters *flcp=NULL;
   H245OpenLogicalChannel_reverseLogicalChannelParameters *rlcp=NULL;
   H245H2250LogicalChannelParameters *pH2250lcp1=NULL, *pH2250lcp2=NULL;
   H245UnicastAddress *pUnicastAddrs=NULL, *pUniAddrs=NULL;
   H245UnicastAddress_iPAddress *pIpAddrs=NULL, *pUniIpAddrs=NULL;

   ooLogicalChannel *pLogicalChannel = NULL;
   int outgoing=0;

   if(!strcmp(call->callType, "outgoing"))  
      outgoing = 1;
  
   if(dir & OORX)
   {
      OOTRACEDBGA3("Building OpenLogicalChannel for Receive Audio Capability "
                   "(%s, %s)\n", call->callType, call->callToken);
      pLogicalChannel = ooAddNewLogicalChannel(call,
                                 olc->forwardLogicalChannelNumber, 1, "audio",
                                 "receive", epCap);
      if(outgoing)
         reverse = 1;
      else
         forward = 1;
   }
   else if(dir & OOTX)
   {
      OOTRACEDBGA3("Building OpenLogicalChannel for transmit Audio Capability "
                   "(%s, %s)\n", call->callType, call->callToken);
      pLogicalChannel = ooAddNewLogicalChannel(call,
                                  olc->forwardLogicalChannelNumber, 1, "audio",
                                  "transmit", epCap);
      if(outgoing)
         forward = 1;
      else
         reverse = 1;
   }
   else if(dir & OORXTX)
   {
      OOTRACEDBGA3("Building OpenLogicalChannel for ReceiveAndTransmit Audio "
                   "Capability (%s, %s)\n", call->callType, call->callToken);
      reverse = 1;
      forward = 1;
      OOTRACEERR3("Symmetric capability is not supported as of now (%s, %s)\n",
                   call->callType, call->callToken);
      return OO_FAILED;
   }

   if(forward)
   {
      OOTRACEDBGC3("Building forward olc. (%s, %s)\n", call->callType,
                    call->callToken);
      flcp = &(olc->forwardLogicalChannelParameters);
      memset(flcp, 0,
             sizeof(H245OpenLogicalChannel_forwardLogicalChannelParameters));
      flcp->dataType.t = T_H245DataType_audioData;
      flcp->dataType.u.audioData = ooCreateAudioCapability(epCap, pctxt, dir);
     
      flcp->multiplexParameters.t = T_H245OpenLogicalChannel_forwardLogicalChannelParameters_multiplexParameters_h2250LogicalChannelParameters;
      pH2250lcp1 = (H245H2250LogicalChannelParameters*)ASN1MALLOC(pctxt,
                                    sizeof(H245H2250LogicalChannelParameters));
      memset(pH2250lcp1, 0, sizeof(H245H2250LogicalChannelParameters));
      flcp->multiplexParameters.t = T_H245OpenLogicalChannel_forwardLogicalChannelParameters_multiplexParameters_h2250LogicalChannelParameters;
     
      flcp->multiplexParameters.u.h2250LogicalChannelParameters = pH2250lcp1;
         
      pH2250lcp1->sessionID = 1;
      if(!outgoing)
      {
         pH2250lcp1->m.mediaChannelPresent = 1;
         pH2250lcp1->mediaChannel.t =
                                    T_H245TransportAddress_unicastAddress;
         pUniAddrs = (H245UnicastAddress*) ASN1MALLOC(pctxt,
                                                   sizeof(H245UnicastAddress));
         memset(pUniAddrs, 0, sizeof(H245UnicastAddress));
         pH2250lcp1->mediaChannel.u.unicastAddress =  pUniAddrs;
         pUniAddrs->t = T_H245UnicastAddress_iPAddress;
         pUniIpAddrs = (H245UnicastAddress_iPAddress*) ASN1MALLOC(pctxt,
                                         sizeof(H245UnicastAddress_iPAddress));
         memset(pUniIpAddrs, 0, sizeof(H245UnicastAddress_iPAddress));
         pUniAddrs->u.iPAddress = pUniIpAddrs;
    
         ooConvertIpToNwAddr(pLogicalChannel->localIP,
                                                pUniIpAddrs->network.data);

         pUniIpAddrs->network.numocts = 4;
         pUniIpAddrs->tsapIdentifier = pLogicalChannel->localRtpPort;
      }
      pH2250lcp1->m.mediaControlChannelPresent = 1;
      pH2250lcp1->mediaControlChannel.t =
                                 T_H245TransportAddress_unicastAddress;
      pUnicastAddrs = (H245UnicastAddress*) ASN1MALLOC(pctxt,
                                                   sizeof(H245UnicastAddress));
      memset(pUnicastAddrs, 0, sizeof(H245UnicastAddress));
      pH2250lcp1->mediaControlChannel.u.unicastAddress =  pUnicastAddrs;
      pUnicastAddrs->t = T_H245UnicastAddress_iPAddress;
      pIpAddrs = (H245UnicastAddress_iPAddress*) ASN1MALLOC(pctxt,
                                         sizeof(H245UnicastAddress_iPAddress));
      memset(pIpAddrs, 0, sizeof(H245UnicastAddress_iPAddress));
      pUnicastAddrs->u.iPAddress = pIpAddrs;
    
       ooConvertIpToNwAddr(pLogicalChannel->localIP, pIpAddrs->network.data);

      pIpAddrs->network.numocts = 4;
      pIpAddrs->tsapIdentifier = pLogicalChannel->localRtcpPort;
      if(!outgoing)
      {
         if(epCap->startReceiveChannel)
         {  
            epCap->startReceiveChannel(call, pLogicalChannel);     
            OOTRACEINFO3("Receive channel of type audio started (%s, %s)\n",
                          call->callType, call->callToken);
         }
         else{
            OOTRACEERR3("ERROR:No callback registered to start receive audio"
                       " channel (%s, %s)\n", call->callType, call->callToken);
            return OO_FAILED;
         }
      }
   }

   if(reverse)
   {
      OOTRACEDBGC3("Building reverse olc. (%s, %s)\n", call->callType,
                    call->callToken);
      olc->forwardLogicalChannelParameters.dataType.t =
                                                      T_H245DataType_nullData;
      olc->forwardLogicalChannelParameters.multiplexParameters.t =
         T_H245OpenLogicalChannel_forwardLogicalChannelParameters_multiplexParameters_none;
      olc->m.reverseLogicalChannelParametersPresent = 1;
      rlcp = &(olc->reverseLogicalChannelParameters);
      memset(rlcp, 0, sizeof(H245OpenLogicalChannel_reverseLogicalChannelParameters));
      rlcp->dataType.t = T_H245DataType_audioData;
 
      rlcp->dataType.u.audioData = ooCreateAudioCapability(epCap, pctxt, dir);

      rlcp->m.multiplexParametersPresent = 1;
      rlcp->multiplexParameters.t = T_H245OpenLogicalChannel_reverseLogicalChannelParameters_multiplexParameters_h2250LogicalChannelParameters;
      pH2250lcp2 = (H245H2250LogicalChannelParameters*) ASN1MALLOC(pctxt, sizeof(H245H2250LogicalChannelParameters));
      rlcp->multiplexParameters.u.h2250LogicalChannelParameters = pH2250lcp2;
      memset(pH2250lcp2, 0, sizeof(H245H2250LogicalChannelParameters));
      pH2250lcp2->sessionID = 1;
      if(outgoing)
      {
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

         ooConvertIpToNwAddr(pLogicalChannel->localIP, pIpAddrs->network.data);

         pIpAddrs->network.numocts = 4;
         pIpAddrs->tsapIdentifier = pLogicalChannel->localRtpPort;
      }
      pH2250lcp2->m.mediaControlChannelPresent = 1;
      pH2250lcp2->mediaControlChannel.t =
                                 T_H245TransportAddress_unicastAddress;
      pUniAddrs = (H245UnicastAddress*) ASN1MALLOC(pctxt, sizeof(H245UnicastAddress));
     
      memset(pUniAddrs, 0, sizeof(H245UnicastAddress));
      pH2250lcp2->mediaControlChannel.u.unicastAddress =  pUniAddrs;

     
      pUniAddrs->t = T_H245UnicastAddress_iPAddress;
      pUniIpAddrs = (H245UnicastAddress_iPAddress*) ASN1MALLOC(pctxt, sizeof(H245UnicastAddress_iPAddress));
      memset(pUniIpAddrs, 0, sizeof(H245UnicastAddress_iPAddress));
      pUniAddrs->u.iPAddress = pUniIpAddrs;

      ooConvertIpToNwAddr(pLogicalChannel->localIP, pUniIpAddrs->network.data);
      pUniIpAddrs->network.numocts = 4;
      pUniIpAddrs->tsapIdentifier = pLogicalChannel->localRtcpPort;
         
      /*
         In case of fast start, the local endpoint need to be ready to
         receive all the media types proposed in the fast connect, before
         the actual call is established.
      */
      if(outgoing)
      {
         if(epCap->startReceiveChannel)
         {  
            epCap->startReceiveChannel(call, pLogicalChannel);     
            OOTRACEINFO3("Receive channel of type audio started (%s, %s)\n",
                          call->callType, call->callToken);
         }
         else{
            OOTRACEERR3("ERROR:No callback registered to start receive audio "
                        "channel (%s, %s)\n", call->callType, call->callToken);
            return OO_FAILED;
         }  
      }
   }

   /* State of logical channel. for out going calls, as we are sending setup,
      state of all channels are proposed, for incoming calls, state is
      established. */
   if(!outgoing)
      pLogicalChannel->state = OO_LOGICALCHAN_ESTABLISHED;
   else
      pLogicalChannel->state = OO_LOGICALCHAN_PROPOSED;
  
   return OO_OK;
}



