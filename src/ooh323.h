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
 * @file ooh323.h
 * This file contains functions to support H.225 messages.
 */
#ifndef _OOH323HDR_H_
#define _OOH323HDR_H_

#include "ooasn1.h"
#include "ootypes.h"
#include "oo.h"
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
 * @addtogroup q931
 * @{
 */
/**
 * This function is used to process a received SETUP message.
 * @param call     Pointer to call for which SETUP message is received.
 * @param q931Msg  Pointer to the received SETUP message.
 *
 * @return         OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooOnReceivedSetup(ooCallData *call, Q931Message *q931Msg);

/**
 * This function is used to process a received CONNECT message.
 * It creates H.245 negotiation channel, and starts TCS and MSD
 * procedures.
 * @param call      Pointer to call for which CONNECT message is received.
 * @param q931Msg   Pointer to the received q931Msg
 *
 * @return          OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooOnReceivedSignalConnect(ooCallData* call, Q931Message *q931Msg);

/**
 * This function is used to handle received H.2250 messages. It
 * calls helper functions based on the type of message received.
 * @param call       Pointer to the call for which a H.2250 message is received
 * @param q931Msg    Pointer to the received q931Msg
 *
 * @return           OO_OK, on success. OO_FAILED, on failure
 */
EXTERN int ooHandleH2250Message(ooCallData *call, Q931Message *q931Msg);

/**
 * This function is used to process a received Facility message.
 * @param call       Handle to the call for which message has been received.
 * @param pQ931Msg   Pointer the the received Facility message.
 *
 * @return           OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooOnReceivedFacility(ooCallData *call, Q931Message * pQ931Msg);

/**
 * This function is used to process tunneled H245 messages
 * @param call       Handle to the call
 * @param pH323UUPdu Pointer to the pdu containing tunneled messages.
 *
 * @return           OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooHandleTunneledH245Messages(ooCallData *call,
                                        H225H323_UU_PDU * pH323UUPdu);

/**
 * This is a helper function used to handle an startH245 Facility message.
 * @param call       Handle to the call
 * @param facility   Pointer to the facility message.
 */
EXTERN int ooHandleStartH245FacilityMessage(ooCallData *call,
                                            H225Facility_UUIE *facility);

/**
 * This function is used to retrieve the aliases from Sequence of alias
 * addresses.
 * @param call       Handle to the call.
 * @param pAddresses Pointer to the sequence of alias addresses.
 * @param remote     Indicates whether aliases are for remote host.
 *
 * @return           OO_OK, on success. OO_FAILED, on failure.
 */
EXTERN int ooRetrieveAliases(ooCallData *call,
                             H225_SeqOfH225AliasAddress *pAddresses,
                             int remote);
/**
 * @}
 */
#ifdef __cplusplus
}
#endif

#endif

