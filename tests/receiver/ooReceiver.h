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

#ifndef H323EP_H
#define H323EP_H
#include "ootypes.h"
#include "H323-MESSAGES.h"

/**
 * This is the callback function registered with the stack component. This
 * function is called by the stack whenever there is a user activity on stdin.
 * The function handles the user commands to make/accpet/reject/hang calls
 *
 * @return      0, on success
 *             -1, on failure.
 */
void* osEpHandleCommand(void*);
int osEpStartReceiveChannel(ooCallData *call, ooLogicalChannel *pChannel);
int osEpStopReceiveChannel(ooCallData *call, ooLogicalChannel *pChannel);
int osEpOnIncomingCall(ooCallData* call );
#endif

