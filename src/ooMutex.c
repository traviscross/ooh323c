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
#ifdef USE_THREADS
#include "ooMutex.h"


/**
 * Mutex to protect access to global call token variables.
 * This is required as these variables will be used by
 * multiple threads trying to place calls using stack commands
 */
static OO_MUTEX gCallTokenMutex;

/**
 * Mutex to protect access to stack commands list
 */
static OO_MUTEX gCmdMutex;

static void ooInitMutex (OO_MUTEX* pmutex)
{
#ifdef _WIN32
   InitializeCriticalSection (pmutex);
#else
   pthread_mutex_init (pmutex, 0);
#endif
}

static void ooAcquireMutex (OO_MUTEX* pmutex)
{
#ifdef _WIN32
   EnterCriticalSection (pmutex);
#else
   pthread_mutex_lock (pmutex);
#endif
}

static void ooReleaseMutex (OO_MUTEX* pmutex)
{
#ifdef _WIN32
   LeaveCriticalSection (pmutex);
#else
   pthread_mutex_unlock (pmutex);
#endif
}

void ooMutexInitCmdMutex(void)
{
   ooInitMutex (&gCmdMutex);
}

void ooMutexAcquireCmdMutex()
{
   ooAcquireMutex (&gCmdMutex);
}

void ooMutexReleaseCmdMutex()
{
   ooReleaseMutex (&gCmdMutex);
}

void ooMutexDestroyCmdMutex()
{
#ifdef _WIN32
  DeleteCriticalSection(&gCmdMutex);
#endif
}

void ooMutexInitCallTokenMutex(void)
{
   ooInitMutex (&gCallTokenMutex);
}

void ooMutexAcquireCallTokenMutex(void)
{
   ooAcquireMutex (&gCallTokenMutex);
}

void ooMutexReleaseCallTokenMutex(void)
{
   ooReleaseMutex (&gCallTokenMutex);
}

void ooMutexDestroyCallTokenMutex(void)
{
#ifdef _WIN32
   DeleteCriticalSection(&gCallTokenMutex);
#endif
}
#endif
