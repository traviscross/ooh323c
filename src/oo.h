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


/* tracing */
#ifdef _OOWARNING
#define TRACELVL 2
#endif
#ifdef _OOINFO
#define TRACELVL 3
#endif
#ifdef _OODEBUGA
#define TRACELVL 4
#endif
#ifdef _OODEBUGB
#define TRACELVL 5
#endif
#ifdef _OODEBUGC
#define TRACELVL 6
#endif

/* Ensure we always log error messages */
#ifndef TRACELVL
#define TRACELVL 1
#endif

#define OOTRACEERR1(a)                ooTrace(a)
#define OOTRACEERR2(a,b)        ooTrace(a,b)
#define OOTRACEERR3(a,b,c)    ooTrace(a,b,c)
#define OOTRACEERR4(a,b,c,d)  ooTrace(a,b,c,d)
#define OOTRACEWARN1(a)       if(TRACELVL > 1) ooTrace(a)
#define OOTRACEWARN2(a,b)     if(TRACELVL > 1) ooTrace(a,b)
#define OOTRACEWARN3(a,b,c)   if(TRACELVL > 1) ooTrace(a,b,c)
#define OOTRACEWARN4(a,b,c,d) if(TRACELVL > 1) ooTrace(a,b,c,d)
#define OOTRACEINFO1(a)               if(TRACELVL > 2) ooTrace(a)
#define OOTRACEINFO2(a,b)     if(TRACELVL > 2) ooTrace(a,b)
#define OOTRACEINFO3(a,b,c)   if(TRACELVL > 2) ooTrace(a,b,c)
#define OOTRACEINFO4(a,b,c,d) if(TRACELVL > 2) ooTrace(a,b,c,d)
#define OOTRACEINFO5(a,b,c,d,e) if(TRACELVL > 2) ooTrace(a,b,c,d,e)
#define OOTRACEDBGA1(a)       if(TRACELVL > 3) ooTrace(a)
#define OOTRACEDBGA2(a,b)     if(TRACELVL > 3) ooTrace(a,b)
#define OOTRACEDBGA3(a,b,c)   if(TRACELVL > 3) ooTrace(a,b,c)
#define OOTRACEDBGA4(a,b,c,d) if(TRACELVL > 3) ooTrace(a,b,c,d)
#define OOTRACEDBGB1(a)       if(TRACELVL > 4) ooTrace(a)
#define OOTRACEDBGB2(a,b)     if(TRACELVL > 4) ooTrace(a,b)
#define OOTRACEDBGB3(a,b,c)   if(TRACELVL > 4) ooTrace(a,b,c)
#define OOTRACEDBGB4(a,b,c,d) if(TRACELVL > 4) ooTrace(a,b,c,d)
#define OOTRACEDBGC1(a)       if(TRACELVL > 5) ooTrace(a)
#define OOTRACEDBGC2(a,b)     if(TRACELVL > 5) ooTrace(a,b)
#define OOTRACEDBGC3(a,b,c)   if(TRACELVL > 5) ooTrace(a,b,c)
#define OOTRACEDBGC4(a,b,c,d) if(TRACELVL > 5) ooTrace(a,b,c,d)

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

EXTERN int ooLogAsn1Error(int stat, const char * fname, int lno);
#ifdef __cplusplus
}
#endif
#endif
