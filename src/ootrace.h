/*
 * Copyright (C) 2004-2009 by Objective Systems, Inc.
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
 * @file ootrace.h
 * This file defines the trace functionality
 */
#include "ooCommon.h"
#ifndef _OOTRACE_H_
#define _OOTRACE_H_


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

#define OOTRACEERR1(a)        ooTrace(__FILE__, __LINE__, OOTRCLVLERR,a)
#define OOTRACEERR2(a,b)      ooTrace(__FILE__, __LINE__, OOTRCLVLERR,a,b)
#define OOTRACEERR3(a,b,c)    ooTrace(__FILE__, __LINE__, OOTRCLVLERR,a,b,c)
#define OOTRACEERR4(a,b,c,d)  ooTrace(__FILE__, __LINE__, OOTRCLVLERR,a,b,c,d)
#define OOTRACEWARN1(a)       ooTrace(__FILE__, __LINE__, OOTRCLVLWARN,a)
#define OOTRACEWARN2(a,b)     ooTrace(__FILE__, __LINE__, OOTRCLVLWARN,a,b)
#define OOTRACEWARN3(a,b,c)   ooTrace(__FILE__, __LINE__, OOTRCLVLWARN,a,b,c)
#define OOTRACEWARN4(a,b,c,d) ooTrace(__FILE__, __LINE__, OOTRCLVLWARN,a,b,c,d)
#define OOTRACEINFO1(a)       ooTrace(__FILE__, __LINE__, OOTRCLVLINFO, a)
#define OOTRACEINFO2(a,b)     ooTrace(__FILE__, __LINE__, OOTRCLVLINFO,a,b)
#define OOTRACEINFO3(a,b,c)   ooTrace(__FILE__, __LINE__, OOTRCLVLINFO,a,b,c)
#define OOTRACEINFO4(a,b,c,d) ooTrace(__FILE__, __LINE__, OOTRCLVLINFO,a,b,c,d)
#define OOTRACEINFO5(a,b,c,d,e) ooTrace(__FILE__, __LINE__, OOTRCLVLINFO,a,b,c,d,e)
#define OOTRACEINFO6(a,b,c,d,e,f) ooTrace(__FILE__, __LINE__, OOTRCLVLINFO,a,b,c,d,e, f)
#ifndef _COMPACT
#define OOTRACEDBGA1(a)       ooTrace(__FILE__, __LINE__, OOTRCLVLDBGA,a)
#define OOTRACEDBGA2(a,b)     ooTrace(__FILE__, __LINE__, OOTRCLVLDBGA,a,b)
#define OOTRACEDBGA3(a,b,c)   ooTrace(__FILE__, __LINE__, OOTRCLVLDBGA,a,b,c)
#define OOTRACEDBGA4(a,b,c,d) ooTrace(__FILE__, __LINE__, OOTRCLVLDBGA,a,b,c,d)
#define OOTRACEDBGA5(a,b,c,d,e) ooTrace(__FILE__, __LINE__, OOTRCLVLDBGA,a,b,c,d,e)
#define OOTRACEDBGB1(a)       ooTrace(__FILE__, __LINE__, OOTRCLVLDBGB,a)
#define OOTRACEDBGB2(a,b)     ooTrace(__FILE__, __LINE__, OOTRCLVLDBGB,a,b)
#define OOTRACEDBGB3(a,b,c)   ooTrace(__FILE__, __LINE__, OOTRCLVLDBGB,a,b,c)
#define OOTRACEDBGB4(a,b,c,d) ooTrace(__FILE__, __LINE__, OOTRCLVLDBGB,a,b,c,d)
#define OOTRACEDBGC1(a)       ooTrace(__FILE__, __LINE__, OOTRCLVLDBGC,a)
#define OOTRACEDBGC2(a,b)     ooTrace(__FILE__, __LINE__, OOTRCLVLDBGC,a,b)
#define OOTRACEDBGC3(a,b,c)   ooTrace(__FILE__, __LINE__, OOTRCLVLDBGC,a,b,c)
#define OOTRACEDBGC4(a,b,c,d) ooTrace(__FILE__, __LINE__, OOTRCLVLDBGC,a,b,c,d)
#define OOTRACEDBGC5(a,b,c,d,e) ooTrace(__FILE__, __LINE__, OOTRCLVLDBGC,a,b,c,d,e)
#else
#define OOTRACEDBGA1(a)
#define OOTRACEDBGA2(a,b)
#define OOTRACEDBGA3(a,b,c)
#define OOTRACEDBGA4(a,b,c,d)
#define OOTRACEDBGA5(a,b,c,d,e)
#define OOTRACEDBGB1(a)
#define OOTRACEDBGB2(a,b)
#define OOTRACEDBGB3(a,b,c)
#define OOTRACEDBGB4(a,b,c,d)
#define OOTRACEDBGC1(a)
#define OOTRACEDBGC2(a,b)
#define OOTRACEDBGC3(a,b,c)
#define OOTRACEDBGC4(a,b,c,d)
#define OOTRACEDBGC5(a,b,c,d,e)
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifndef EXTERN
#ifdef MAKE_DLL
#define EXTERN __declspec(dllexport)
#else
#define EXTERN
#endif /* MAKE_DLL */
#endif /* EXTERN */

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
 * This function is used to write messages to the trace file.
 *
 * @param traceLevel  Trace level for the message.
 * @param fmtspec     Printf style format spec.
 * @param ...         Printf style variable list of arguments
 *
 * @return            - none
 */
EXTERN void ooTrace(char *file, long line, OOUINT32 traceLevel, const char * fmtspec, ...);

void ooTraceHexDump
(OOUINT32 traceLevel, const OOUCHAR* data, OOUINT32 numocts);

void ooChangeIPToNWOrder(char * internetIP, char* networkIP);

int ooLogAsn1Error(int stat, const char * fname, int lno);

#ifdef __cplusplus
}
#endif
#endif
