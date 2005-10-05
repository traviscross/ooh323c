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
 * @file ooStackCmds.h
 * This file contains stack commands which an user application can use to make
 * call, hang call etc.
 */

#ifndef OO_MUTEX_H
#define OO_MUTEX_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_PTHREAD_H
#include <pthread.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifndef EXTERN
#if defined (MAKE_DLL)
#define EXTERN __declspec(dllexport)
#else
#define EXTERN
#endif /* MAKE_DLL */
#endif /* EXTERN */



/* Define common mutex type */
#ifdef _WIN32
#define OO_MUTEX CRITICAL_SECTION
#else
#define OO_MUTEX pthread_mutex_t
#endif



/**
 * Mutex to protect ooGenerateCallReference function.
 * This is required as this function will be called by
 * multiple threads trying to place calls using stack commands
 */
OO_MUTEX gCallRefMutex;

/**
 * Mutex to protect access to global call token variables.
 * This is required as these variables will be used by
 * multiple threads trying to place calls using stack commands
 */
OO_MUTEX gCallTokenMutex;

/**
 * Mutex to protect access to stack commands list
 */
OO_MUTEX gCmdMutex;

EXTERN void ooMutexInitCmdMutex(void);
EXTERN void ooMutexAcquireCmdMutex(void);
EXTERN void ooMutexReleaseCmdMutex(void);
EXTERN void ooMutexDestroyCmdMutex(void);

EXTERN void ooMutexInitCallRefMutex(void);
EXTERN void ooMutexAcquireCallRefMutex(void);
EXTERN void ooMutexReleaseCallRefMutex(void);
EXTERN void ooMutexDestroyCallRefMutex(void);

EXTERN void ooMutexInitCallTokenMutex(void);
EXTERN void ooMutexAcquireCallTokenMutex(void);
EXTERN void ooMutexReleaseCallTokenMutex(void);
EXTERN void ooMutexDestroyCallTokenMutex(void);


/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
