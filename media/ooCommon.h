/*
 * Copyright (C) 2004-2010 by Objective Systems, Inc.
 *
 * This software is furnished under an open source license and may be
 * used and copied only in accordance with the terms of this license.
 * The text of the license may generally be found in the root
 * directory of this installation in the LICENSE.txt file.  It
 * can also be viewed online at the following URL:
 *
 *   http://www.obj-sys.com/open/license.html
 *
 * Any redistributions of this file including modified versions must
 * maintain this copyright notice.
 *
 *****************************************************************************/

#ifndef _OOCOMMON_H_
#define _OOCOMMON_H_

/**
 * @file ooCommon.h
 * This file contains common helper functions.
 */

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include "ooSock.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef EXTERN
#ifdef _WIN32
#define EXTERN __declspec(dllexport)
#else
#define EXTERN
#endif /* _WIN32 */
#endif

/**
 * @defgroup mediacommoin Media API Internal Common Helper Functions
 * @{
 */
#define OORTPPACKETDATASIZE 240 /* Send Receive Packet Data Size*/
#define MAXLOGMSGLEN 1024
#define OOLOG2(a,b)               ooLog(a,b)
#define OOLOG3(a,b,c)             ooLog(a,b,c)
#define OOLOG4(a,b,c,d)           ooLog(a,b,c,d)
#define OOLOG5(a,b,c,d,e)         ooLog(a,b,c,d,e)
#define OOLOG9(a,b,c,d,e,f,g,h,i) ooLog(a,b,c,d,e,f,g,h,i)

FILE *fpLog;

/**
 * This function logs a trace message into a log file.
 * @param level    Log level(Currently not used)
 * @param fmtspec  Format specification for the log message.
 * @param ...      Variable number of arguments representing the message.
 */
void ooLog(int level, const char * fmtspec,...);

/**
 * Platform independent sleep function.
 * @param milliseconds    Sleep time in milliseconds.
 */
void ooSleep(int milliseconds);

/**
 * @}
 */
#ifdef __cplusplus
}
#endif

#endif
