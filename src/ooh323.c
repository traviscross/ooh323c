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

#include "ootypes.h"
#include "ooq931.h"
#include "oo.h"
#include "oochannels.h"
#include "ooh245.h"
#include "ooCalls.h"


int ooOnReceivedSetup(ooCallData *call, Q931Message *q931Msg)
{
   H225Setup_UUIE *setup=NULL;
   call->callReference = q931Msg->callReference;

   if(!q931Msg->userInfo)
   {
      OOTRACEERR3("ERROR:No User-User IE in received SETUP message (%s, %s)\n",
                  call->callType, call->callToken);
      return OO_FAILED;
   }
   setup = q931Msg->userInfo->h323_uu_pdu.h323_message_body.u.setup;
   if(!setup)
   {
      OOTRACEERR3("Error: Setup UUIE not found in received setup message - %s "
                  "%s\n", call->callType, call->callToken);
      return OO_FAILED;
   }
   memcpy(call->callIdentifier.guid.data, setup->callIdentifier.guid.data,
          setup->callIdentifier.guid.numocts);
   call->callIdentifier.guid.numocts = setup->callIdentifier.guid.numocts;
   /* check for fast start - H245 Tunneling */
   return OO_OK;
}

int ooOnReceivedSignalConnect(ooCallData* call, Q931Message *q931Msg)
{
   int ret;
   H225TransportAddress *h245Address;
   H225Connect_UUIE *connect;
   if(!q931Msg->userInfo)
   {
      OOTRACEERR3("Error: UUIE not found in received H.225 Connect message"
                  " (%s, %s)\n", call->callType, call->callToken);
      return OO_FAILED;
   }
   /* Retrieve the connect message from the user-user IE */
   connect = q931Msg->userInfo->h323_uu_pdu.h323_message_body.u.connect;
   if(connect == NULL)
   {
      OOTRACEERR3("Error: Received Connect message does not have Connect UUIE"
                  " (%s, %s)\n", call->callType, call->callToken);
      return OO_FAILED;
   }
   /* Retrieve the H.245 control channel address from the connect msg */
   if(connect->m.h245AddressPresent == 0)
   {
      OOTRACEERR3("Error: Received Connect message does not have H.245 address"
                  " (%s, %s)\n", call->callType, call->callToken);
      return OO_FAILED;
   }
   h245Address = &(connect->h245Address);
   if(h245Address->t != T_H225TransportAddress_ipAddress)
   {
      OOTRACEERR3("Error: Unknown H245 address type in received Connect message"
                  " (%s, %s)", call->callType, call->callToken);
      return OO_FAILED;
   }
   sprintf(call->remoteIP, "%d.%d.%d.%d",
           h245Address->u.ipAddress->ip.data[0],
           h245Address->u.ipAddress->ip.data[1],
           h245Address->u.ipAddress->ip.data[2],
           h245Address->u.ipAddress->ip.data[3]);
   call->remoteH245Port = h245Address->u.ipAddress->port;
   /* Create an H.245 connection.
   */
   if(ooCreateH245Connection(call)== OO_FAILED)
   {
      OOTRACEERR3("Error: H.245 channel creation failed (%s, %s)\n",
                  call->callType, call->callToken);
      return OO_FAILED;
   }

   /* Send Master Slave determination Message */
  
   ret = ooSendTermCapMsg(call);
   if(ret != OO_OK)
   {
      OOTRACEERR3("Error: Sending TerminalCapabilitySet message (%s, %s)\n",
                  call->callType, call->callToken);
      return OO_FAILED;
   }

   ret = ooSendMasterSlaveDetermination(call);
   if(ret != OO_OK)
   {
      OOTRACEERR3("Error: Sending MasterSlaveDetermination message (%s, %s)\n",
                  call->callType, call->callToken);
      return OO_FAILED;
   }
   return OO_OK; 
}

int ooHandleH2250Message(ooCallData *call, Q931Message *q931Msg)
{
   int ret=0;
   int type = q931Msg->messageType;
   switch(type)
   {
      case Q931SetupMsg: /* Setup message is received */
         OOTRACEINFO3("Received SETUP message (%s, %s\n", call->callType,
                       call->callToken);
         ooOnReceivedSetup(call, q931Msg);
         ooSendCallProceeding(call);/* Send call proceeding message*/
         ooSendAlerting(call); /* Send alerting message */
         ooCreateH245Listener(call);
         ooSendConnect(call); /* Send connect message - call accepted */

         /* Free up the mem used by the received message, as it's processing
            is done.
         */
         ooFreeQ931Message(q931Msg);
         break;
      case Q931CallProceedingMsg:/* Call proceeding message is received */
         OOTRACEINFO3("H.225 Call Proceeding message received (%s, %s)\n",
                      call->callType, call->callToken);
         ooFreeQ931Message(q931Msg);
         break;
      case Q931AlertingMsg:/* Alerting message received */
         OOTRACEINFO3("H.225 Alerting message received (%s, %s)\n",
                      call->callType, call->callToken);
         ooFreeQ931Message(q931Msg);
         break;
      case Q931ConnectMsg:/* Connect message received */
         OOTRACEINFO3("H.225 Connect message received (%s, %s)\n",
                      call->callType, call->callToken);        
         ooOnReceivedSignalConnect(call, q931Msg);
         ooFreeQ931Message(q931Msg);
         break;
      /*case Q931InformationMsg:
         OOTRACEINFO2(myContext, "H.225 Information msg received\n");
         ooFreeQ931Message(myContext, q931Msg);
         break;*/
      case Q931ReleaseCompleteMsg:/* Release complete message received */
         OOTRACEINFO3("H.225 Release Complete message received (%s, %s)\n",
                      call->callType, call->callToken);
         ooClearCall(call, OO_NORMAL);
         ooFreeQ931Message(q931Msg);
         break;
/*      case Q931FacilityMsg:
         OOTRACEINFO2(myContext, "H.225 Facility message Received\n");
         ooFreeQ931Message(myContext, q931Msg);
         break;
      case Q931ProgressMsg:
         OOTRACEINFO2(myContext, "H.225 Progress message received\n");
         ooFreeQ931Message(myContext, q931Msg);
         break;
      case Q931StatusMsg:
         OOTRACEINFO2(myContext, "H.225 Status message received\n");
         ooFreeQ931Message(myContext, q931Msg);
         break;
      case Q931StatusEnquiryMsg:
         OOTRACEINFO2(myContext, "H.225 Status Inquiry message Received\n");
         ooFreeQ931Message(myContext, q931Msg);
         break;
      case Q931SetupAckMsg:
         OOTRACEINFO2(myContext, "H.225 Setup Ack message received\n");
         ooFreeQ931Message(myContext, q931Msg);
         break;
      case Q931NotifyMsg:
         OOTRACEINFO2(myContext, "H.225 Notify message Received\n");
         ooFreeQ931Message(myContext, q931Msg);
 */        break;
      default:
         OOTRACEWARN3("Invalid H.225 message type received (%s, %s)\n",
                      call->callType, call->callToken);
         ooFreeQ931Message(q931Msg);
   }
   return OO_OK;
}

