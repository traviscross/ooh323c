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

#ifndef OOPLAYER_H
#define OOPLAYER_H
#include "ootypes.h"
#include "ooLogChan.h"
#include "H323-MESSAGES.h"

struct OOH323CallData;

/**
 * This is the callback function registered with the stack component. This
 * function is called by the stack whenever there is user activity on stdin.
 * The function handles the user commands to make/accept/reject/hang calls.
 *
 * @return      0, on success
 *             -1, on failure.
 */
void* osEpHandleCommand(void*);

int osEpStartTransmitChannel
   (struct OOH323CallData *call, OOLogicalChannel *pChannel);

int osEpStopTransmitChannel
   (struct OOH323CallData *call, OOLogicalChannel *pChannel);

#endif

