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
 * @file oo.h
 * This file defines the trace functionality
 */


#ifndef _OO_H_
#define _OO_H_
#include "ootypes.h"

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
 * This function is used to write the messages to the trace file.
 *
 * @param fmtspec   Printf style format spec.
 * @param ...       Printf style variable list of arguments             
 *
 * @return          - none
 */
EXTERN void ooTrace(const char * fmtspec, ...);

#ifdef __cplusplus
}
#endif
#endif
