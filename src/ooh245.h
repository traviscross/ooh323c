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

/**
 * @file ooh245.h
 * This file contains functions to support H245 negotiations.
 */
#ifndef _OOH245HDR_H_
#define _OOH245HDR_H_

#include "ooasn1.h"
#include "ootypes.h"
#include "oochannels.h"
#include "ootrace.h"

#include "ooq931.h"
#include "MULTIMEDIA-SYSTEM-CONTROL.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef EXTERN
#ifdef _WIN32
#define EXTERN __declspec(dllexport)
#else
#define EXTERN
#endif /* _WIN32 */
#endif /* EXTERN */

/**
 * @defgroup h245 H.245 Message Handling
 * @{
 */
/**
 * Creates an outgoing H245 message of the type specified by the type
 * argument for the Application context.
 *
 * @param msg       A pointer to pointer to message which will be assigned to
 *                  allocated memory.
 * @param type      Type of the message to be created.
 *                  (Request/Response/Command/Indication)
 *
 * @return          Completion status of operation: 0 (OO_OK) = success,
 *                  negative return value is error.        
 */
EXTERN int ooCreateH245Message(H245Message **msg, int type);

/**
 * Frees up the memory used by the H245 message.
 *
 * @param call      Handle to the call
 * @param pmsg      Pointer to an H245 message structure.
 *
 * @return          OO_OK, on success. OO_FAILED, on failure        
 */
EXTERN int ooFreeH245Message(ooCallData *call, H245Message *pmsg);

/**
 * This function is used to retrieve an H.245 message enqueued in the outgoing
 * queue.
 * @param call      Pointer to the call for which message has to be retrieved.
 * @param msgbuf    Pointer to a buffer in which the message will be returned.
 * @param len       Pointer to an int variable which will contain length of
 *                  the message data after returning.
 * @param msgType   Pointer to an int variable, which will contain message type
 *                  on return from the function.
 *
 * @return          OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooGetOutgoingH245Msgbuf(ooCallData *call, ASN1OCTET *msgbuf,
                                   int *len, int *msgType);

/**
 * This function is used to send out a treminal capability set message.
 * @param call      Pointer to a call, for which TerminalCapabilitySet message
 *                  has to be sent.
 *
 * @return          OO_OK, on success. OO_FAILED, on failure. 
 */
EXTERN int ooSendTermCapMsg(ooCallData *call);

/**
 * This function is used to generate a random status determination number
 * for MSD procedure.
 * @param None
 *
 * @return          Generated status determination number.
 */
EXTERN ASN1UINT ooGenerateStatusDeterminationNumber();

/**
 * This fuction is used to handle received MasterSlaveDetermination procedure
 * messages.
 * @param call       Pointer to the call for which a message is received.
 * @param pmsg       Pointer to MSD message
 * @param msgType    Message type indicating whether received message is MSD,
 *                   MSDAck, MSDReject etc...
 *
 * @return           OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooHandleMasterSlave(ooCallData *call, void * pmsg, int msgType);

/**
 * This function is used to send MSD message.
 * @param call       Pointer to call for which MasterSlaveDetermination has to
 *                   be sent.
 *
 * @return           OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooSendMasterSlaveDetermination(ooCallData *call);

/**
 * This function is used to send a MasterSlaveDeterminationAck message.
 * @param call        Pointer to call for which MasterSlaveDeterminationAck has
 *                    to be sent.
 * @param status      Result of the determination process(Master/Slave as it
 *                    applies to remote endpoint)
 *
 * @return            OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooSendMasterSlaveDeterminationAck(ooCallData* call, char * status);

/**
 * This function is used to send a MasterSlaveDeterminationReject message.
 * @param call        Pointer to call for which message is to be sent.
 * @return            OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooSendMasterSlaveDeterminationReject (ooCallData* call);


/**
 * This function is used to handle MasterSlaveReject message. If number of
 * retries is less than max allowed, then it restarts the
 * MasterSlaveDetermination procedure.
 * @param call        Handle to the call for which MasterSlaveReject is
 *                    received.
 * @param reject      Poinetr to the received reject message.
 *
 * @return            OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooHandleMasterSlaveReject
   (ooCallData *call, H245MasterSlaveDeterminationReject* reject);

/**
 * This function is used to handle received OpenLogicalChannel message.
 * @param call        Pointer to call for which OpenLogicalChannel message is
 *                    received.
 * @param olc         Pointer to the received OpenLogicalChannel message.
 *
 * @return            OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooHandleOpenLogicalChannel
                   (ooCallData* call, H245OpenLogicalChannel *olc);

/**
 * This function is used to handle a received OpenLogicalChannel message which
 * is trying to open a audio channel.
 * @param call        Pointer to cll for which OLC was received.
 * @param olc         The received OpenLogicalChannel message.
 *
 * @return            OO_OK, on success. OO_FAILED, on failure.        
 */
EXTERN int ooHandleOpenLogicalAudioChannel
                      (ooCallData *call, H245OpenLogicalChannel*olc);

/**
 * This function is used to build and send OpenLogicalChannelReject message.
 * @param call        Pointer to call for which OLCReject has to be sent.
 * @param channelNum  LogicalChannelNumber to be rejected.
 * @param cause       Cause of rejection.
 *
 * @return            OO_OK, on success. OO_FAILED, on failure.
 */
int ooSendOpenLogicalChannelReject
   (ooCallData *call, ASN1UINT channelNum, ASN1UINT cause);

/**
 * This function is used to handle a received OpenLogicalChannelAck message.
 * @param call         Pointer to call for which OLCAck is received
 * @param olcAck       Pointer to received olcAck message.
 *
 * @return             OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooOnReceivedOpenLogicalChannelAck(ooCallData *call,
                                            H245OpenLogicalChannelAck *olcAck);


/**
 * This function is used to handle the received OpenLogicalChannelReject
 * message.
 * @param call         Handle to the call for which the message is received.
 * @param olcRejected  Pointer to received OpenLogicalChannelReject message.
 *
 * @return             OO_OK, on success. OO_FAILED, on failure.
 */
int ooOnReceivedOpenLogicalChannelRejected(ooCallData *call,
                                    H245OpenLogicalChannelReject *olcRejected);
/**
 * This message is used to send an EndSession command. It builds a EndSession
 * command message and queues it into the calls outgoing queue.
 * @param call          Pointer to call for which EndSession command has to be
 *                      sent.
 * @return              OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooSendEndSessionCommand(ooCallData *call);

/**
 * This function is used to handle a received H245Command message.
 * @param call          Pointer to call for which an H245Command is received.
 * @param command       Pointer to a command message.
 *
 * @return              OO_OK, on success. OO_FAILED, on failure
 */
EXTERN int ooHandleH245Command(ooCallData *call, H245CommandMessage *command);

/**
 * This function is called on receiving a TreminalCapabilitySetAck message.
 * If the MasterSlaveDetermination process is also over, this function
 * initiates the process of opening logical channels.
 * @param call          Pointer to call for which TCSAck is received.
 *
 * @return              OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooOnReceivedTerminalCapabilitySetAck(ooCallData* call);

/**
 * This function is called to close all the open logical channels. It sends
 * CloseLogicalChannel message for all the forward channels and sends
 * RequestCloseLogicalChannel message for all the reverse channels.
 * @param call          Pointer to call for which logical channels have to be
 *                      closed.
 *
 * @return              OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooCloseAllLogicalChannels(ooCallData *call);


/**
 * This function is used to send out a CloseLogicalChannel message for a particular
 * logical channel.
 * @param call           Pointer to a call, to which logical channel to be closed belongs.
 * @param logicalChan    Pointer to the logical channel to be closed.
 *
 * @return               OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooSendCloseLogicalChannel(ooCallData *call, ooLogicalChannel *logicalChan);

/**
 * This function is used to process a received closeLogicalChannel request. It closes the
 * logical channel and removes the logical channel entry from the list. It also, sends
 * closeLogicalChannelAck message to the remote endpoint.
 * @param call           Pointer to call for which CloseLogicalChannel message is received.
 * @param clc            Pointer to received CloseLogicalChannel message.
 *
 * @return               OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooOnReceivedCloseLogicalChannel(ooCallData *call,
                                           H245CloseLogicalChannel* clc);

/**
 * This function is used to process a received CloseLogicalChannelAck message. It closes the
 * channel and removes it from the list of active logical channels.
 * @param call           Pointer to call for which CLCAck message is received.
 * @param clcAck         Pointer to the received CloseLogicalChannelAck message.
 *
 * @return               OO_OK, on success. OO_FAILED, on failure
 */
EXTERN int ooOnReceivedCloseChannelAck(ooCallData* call,
                                           H245CloseLogicalChannelAck* clcAck);

/**
 * This function is used to handle received H245 message. Based on the type of message received,
 * it calls helper functions to process those messages.
 * @param call           Pointer to call for which a message is received.
 * @param pmsg           Pointer to the received H245 message.
 *
 * @return               OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooHandleH245Message(ooCallData *call, H245Message * pmsg);

/**
 * This function is used to process received TCS message. It builds TCSAck message and queues it
 * into the calls outgoing queue. Also, starts Logical channel opening procedure if TCS and MSD
 * procedures have finished.
 * @param call           Pointer to call for which TCS is received.
 * @param pmsg           Pointer to the received message.
 *
 * @return               OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooOnReceivedTerminalCapabilitySet(ooCallData *call, H245Message *pmsg);

/**
 * This function is used to send a TCSAck message to remote endpoint.
 * @param call           Pointer to call on which TCSAck has to be sent.
 *
 * @return               OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooH245AcknowledgeTerminalCapabilitySet(ooCallData *call);

/**
 * This function is used to start OpenLogicalChannel procedure for all the
 * channels to be opened for the call.
 * @param call            Pointer to call for which logical channels have to be opened.
 *
 * @return                OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooOpenLogicalChannels(ooCallData *call);

/**
 * This function is used to send OpenLogicalChannel message for audio channel.
 * It uses the first capability match in the local and remote audio capabilities
 * for the audio channel and calls corresponding helper function.
 * @param call            Pointer to call for which audio channel ahs to be opened.
 *
 * @return                OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooOpenLogicalAudioChannel(ooCallData *call);

/**
 * This function is used to build a OpenLogicalChannel message using
 * G711ULaw64K capability
 * @param call            Pointer to call for which OpenLogicalChannel message have
 *                        to be built.
 * @param epCap           Pointer to G711ULaw capability
 *
 * @return                OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooOpenG711Channel(ooCallData* call, ooH323EpCapability *epCap);

/**
 * This function is used to request a remote end point to close a logical
 * channel.
 * @param call            Pointer to call for which the logical channel has to
 *                        be closed.
 * @param logicalChan     Pointer to the logical channel structure which needs
 *                        to be closed.
 *
 * @return                OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooSendRequestCloseLogicalChannel(ooCallData *call,
                                            ooLogicalChannel *logicalChan);

/**
 * This function is used to send a RequestChannelCloseRelease message when the
 * corresponding timer has expired.
 * @param call            Handle to the call
 * @param channelNum      Channel number.
 *
 * @return                OO_OK, on success. OO_FAILED, otherwise.
 */
int ooSendRequestChannelCloseRelease(ooCallData *call, int channelNum);

/**
 * This function handles the received RequestChannelClose message, verifies
 * that the requested channel is forward channel. It sends an acknowledgement
 * for the message followed by CloseLogicalChannel message.
 * @param call             Pointer to the call for which RequestChannelClose is
 *                         received.
 * @param rclc             Pointer to the received message.
 *
 * @return                 OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooOnReceivedRequestChannelClose(ooCallData *call,
                                           H245RequestChannelClose *rclc);

/**
 * This function is used to handle a received RequestChannelCloseReject
 * response message.
 * @param call             Handle to the call.
 * @param rccReject        Pointer to the received reject response message.
 *
 * @return                 OO_OK, on success. OO_FAILED, on failure.
 */
int ooOnReceivedRequestChannelCloseReject
   (ooCallData *call, H245RequestChannelCloseReject *rccReject);

/**
 * This function is used to handle a received RequestChannelCloseAck
 * response message.
 * @param call             Handle to the call.
 * @param rccAck           Pointer to the received ack response message.
 *
 * @return                 OO_OK, on success. OO_FAILED, on failure.
 */
int ooOnReceivedRequestChannelCloseAck
   (ooCallData *call, H245RequestChannelCloseAck *rccAck);

/**
 * Builds an OLC with an audio capability passed as parameter.
 * @param call             Handle to call for which OLC has to be built.
 * @param olc              Pointer to an OLC structure which will be populated.
 * @param epCap            Pointer to the capability which will be used to build OLC.
 * @param pctxt            Pointer to an OOCTXT structure which will be used to allocate
 *                         additional memory for OLC.
 *
 * @return                 OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooBuildOpenLogicalChannelAudio(ooCallData *call,
                                          H245OpenLogicalChannel *olc,
                                          ooH323EpCapability *epCap,
                                          OOCTXT*pctxt, int dir);

int ooEncodeH245Message
      (ooCallData *call, H245Message *ph245Msg, char *msgbuf, int size);

/**
 * This function is used to send a master-slave determination release message.
 * @param call             Handle to call, for which MSDRelease message has
 *                         to be sent.
 *
 * @return                 OO_OK, on success. OO_FAILED, on failure.
 */
int ooSendMasterSlaveDeterminationRelease(ooCallData * call);

/**
 * This function is used to send a terminal capability set reject message
 * to the remote endpoint.
 * @param call             Handle to the call for which reject message has to
 *                         be sent.
 * @param seqNo            Sequence number of the TCS message to be rejected.
 * @param cause            Cause for rejecting a TCS message.
 *
 * @return                 OO_OK, on success; OO_FAILED, otherwise.
 */
int ooSendTerminalCapabilitySetReject
    (ooCallData *call, int seqNo, ASN1UINT cause);

/**
 * This function is used to send a TerminalCapabilitySetRelease message after
 * capability exchange timer has expired.
 * @param call            Handle to call for which release message has to be
 *                        sent.
 *
 * @return                OO_OK, on success; OO_FAILED, on failure.
 */
int ooSendTerminalCapabilitySetRelease(ooCallData * call);

/**
 * This is a callback function for handling an expired master-slave
 * determination timer.
 * @param data             Callback data registered at the time of creation of
 *                         the timer.
 *
 * @return                 OO_OK, on success. OO_FAILED, otherwise.
 */
int ooMSDTimerExpired(void *data);

/**
 * This is a callback function for handling an expired capability exchange
 * timer.
 * @param data             Callback data registered at the time of creation of
 *                         the timer.
 *
 * @return                 OO_OK, on success. OO_FAILED, otherwise.
 */
int ooTCSTimerExpired(void *data);

/**
 * This is a callback function for handling an expired OpenLogicalChannel
 * timer.
 * @param data             Callback data registered at the time of creation of
 *                         the timer.
 *
 * @return                 OO_OK, on success. OO_FAILED, otherwise.
 */
int ooOpenLogicalChannelTimerExpired(void *pdata);

/**
 * This is a callback function for handling an expired CloseLogicalChannel
 * timer.
 * @param data             Callback data registered at the time of creation of
 *                         the timer.
 *
 * @return                 OO_OK, on success. OO_FAILED, otherwise.
 */
int ooCloseLogicalChannelTimerExpired(void *pdata);

/**
 * This is a callback function for handling an expired RequestChannelClose
 * timer.
 * @param data             Callback data registered at the time of creation of
 *                         the timer.
 *
 * @return                 OO_OK, on success. OO_FAILED, otherwise.
 */
int ooRequestChannelCloseTimerExpired(void *pdata);

/**
 * This is a callback function for handling an expired EndSession timer.
 * @param data             Callback data registered at the time of creation of
 *                         the timer.
 *
 * @return                 OO_OK, on success. OO_FAILED, otherwise.
 */
int ooSessionTimerExpired(void *pdata);
/**
 * @}
 */
#ifdef __cplusplus
}
#endif

#endif
