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

#include "ooCommon.h"
#ifndef _OO_H_
#define _OO_H_


/* tracing */
#define OOTRCLVLERR  1
#define OOTRCLVLWARN 2
#define OOTRCLVLINFO 3
#define OOTRCLVLDBGA 4
#define OOTRCLVLDBGB 5
#define OOTRCLVLDBGC 6

#ifdef _OOWARNING
#define TRACELVL OOTRCLVLWARN
#endif
#ifdef _OOINFO
#define TRACELVL OOTRCLVLINFO
#endif
#ifdef _OODEBUGA
#define TRACELVL OOTRCLVLDBGA
#endif
#ifdef _OODEBUGB
#define TRACELVL OOTRCLVLDBGB
#endif
#ifdef _OODEBUGC
#define TRACELVL OOTRCLVLDBGC
#endif

/* Ensure we always log error messages */
#ifndef TRACELVL
#define TRACELVL 1
#endif

#define OOTRACEERR1(a)        ooTrace(OOTRCLVLERR,a)
#define OOTRACEERR2(a,b)      ooTrace(OOTRCLVLERR,a,b)
#define OOTRACEERR3(a,b,c)    ooTrace(OOTRCLVLERR,a,b,c)
#define OOTRACEERR4(a,b,c,d)  ooTrace(OOTRCLVLERR,a,b,c,d)
#define OOTRACEWARN1(a)       ooTrace(OOTRCLVLWARN,a)
#define OOTRACEWARN2(a,b)     ooTrace(OOTRCLVLWARN,a,b)
#define OOTRACEWARN3(a,b,c)   ooTrace(OOTRCLVLWARN,a,b,c)
#define OOTRACEWARN4(a,b,c,d) ooTrace(OOTRCLVLWARN,a,b,c,d)
#define OOTRACEINFO1(a)       ooTrace(OOTRCLVLINFO, a)
#define OOTRACEINFO2(a,b)     ooTrace(OOTRCLVLINFO,a,b)
#define OOTRACEINFO3(a,b,c)   ooTrace(OOTRCLVLINFO,a,b,c)
#define OOTRACEINFO4(a,b,c,d) ooTrace(OOTRCLVLINFO,a,b,c,d)
#define OOTRACEINFO5(a,b,c,d,e) ooTrace(OOTRCLVLINFO,a,b,c,d,e)
#define OOTRACEDBGA1(a)       ooTrace(OOTRCLVLDBGA,a)
#define OOTRACEDBGA2(a,b)     ooTrace(OOTRCLVLDBGA,a,b)
#define OOTRACEDBGA3(a,b,c)   ooTrace(OOTRCLVLDBGA,a,b,c)
#define OOTRACEDBGA4(a,b,c,d) ooTrace(OOTRCLVLDBGA,a,b,c,d)
#define OOTRACEDBGA5(a,b,c,d,e) ooTrace(OOTRCLVLDBGA,a,b,c,d,e)
#define OOTRACEDBGB1(a)       ooTrace(OOTRCLVLDBGB,a)
#define OOTRACEDBGB2(a,b)     ooTrace(OOTRCLVLDBGB,a,b)
#define OOTRACEDBGB3(a,b,c)   ooTrace(OOTRCLVLDBGB,a,b,c)
#define OOTRACEDBGB4(a,b,c,d) ooTrace(OOTRCLVLDBGB,a,b,c,d)
#define OOTRACEDBGC1(a)       ooTrace(OOTRCLVLDBGC,a)
#define OOTRACEDBGC2(a,b)     ooTrace(OOTRCLVLDBGC,a,b)
#define OOTRACEDBGC3(a,b,c)   ooTrace(OOTRCLVLDBGC,a,b,c)
#define OOTRACEDBGC4(a,b,c,d) ooTrace(OOTRCLVLDBGC,a,b,c,d)

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
 * This function is used to retireve the description text for a code.
 * @param code     Code for which description is required.
 *
 * @return         The text description string.
 */
EXTERN char * ooGetText(int code);

/**
 * This function is used to set the trace level.
 * @param traceLevel  New trace level. Various values are: OOTRCLVLERR,
 *                    OOTRCLVLWARN, OOTRCLVLINFO, OOTRCLVLDBGA, OOTRCLVLDBGB,
 *                    OOTRCLVLDBGC
 *
 * @return            None
 */
EXTERN void ooSetTraceThreshold(OOUINT32 traceLevel);

/**
 * This function is used to write the messages to the trace file.
 *
 * @param traceLevel  Trace level for the message.
 * @param fmtspec     Printf style format spec.
 * @param ...         Printf style variable list of arguments             
 *
 * @return            - none
 */
EXTERN void ooTrace(OOUINT32 traceLevel, const char * fmtspec, ...);

/**
 * Helper function for the trace function. This function performs actual
 * writing to file.
 * @param logMessage  Log message to be writted to file.
 *
 * @return            - none
 */
void ooTraceLogMessage(const char * logMessage);


int ooLogAsn1Error(int stat, const char * fname, int lno);
#ifdef __cplusplus
}
#endif
#endif
