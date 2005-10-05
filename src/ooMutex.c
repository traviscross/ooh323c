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
#include "ooMutex.h"

void ooMutexInitCmdMutex(void)
{

#ifdef USE_THREADS
#ifdef _WIN32
   InitializeCriticalSection(&gCmdMutex);
#else
   pthread_mutex_init(&gCmdMutex, 0);
#endif
#endif

}

void ooMutexAcquireCmdMutex()
{

#ifdef USE_THREADS
#ifdef _WIN32
   EnterCriticalSection(&gCmdMutex);
#else
   pthread_mutex_lock(&gCmdMutex);
#endif
#endif

}

void ooMutexReleaseCmdMutex()
{

#ifdef USE_THREADS
#ifdef _WIN32
   LeaveCriticalSection(&gCmdMutex);
#else
   pthread_mutex_unlock(&gCmdMutex);
#endif
#endif

}

void ooMutexDestroyCmdMutex()
{

#ifdef USE_THREADS
#ifdef _WIN32
  DeleteCriticalSection(&gCmdMutex);
#endif
#endif

}

void ooMutexInitCallRefMutex(void)
{

#ifdef USE_THREADS
#ifdef _WIN32
   InitializeCriticalSection(&gCallRefMutex);
#else
   pthread_mutex_init(&gCallRefMutex, 0);
#endif
#endif

}

void ooMutexAcquireCallRefMutex(void)
{

#ifdef USE_THREADS
#ifdef _WIN32
   EnterCriticalSection(&gCallRefMutex);
#else
   pthread_mutex_lock(&gCallRefMutex);
#endif
#endif

}

void ooMutexReleaseCallRefMutex(void)
{

#ifdef USE_THREADS
#ifdef _WIN32
   LeaveCriticalSection(&gCallRefMutex);
#else
   pthread_mutex_unlock(&gCallRefMutex);
#endif
#endif

}

void ooMutexDestroyCallRefMutex()
{

#ifdef USE_THREADS
#ifdef _WIN32
   DeleteCriticalSection(&gCallRefMutex);
#endif
#endif

}

void ooMutexInitCallTokenMutex(void)
{

#ifdef USE_THREADS
#ifdef _WIN32
   InitializeCriticalSection(&gCallTokenMutex);
#else
   pthread_mutex_init(&gCallTokenMutex, 0);
#endif
#endif

}

void ooMutexAcquireCallTokenMutex(void)
{

#ifdef USE_THREADS
#ifdef _WIN32
   EnterCriticalSection(&gCallTokenMutex);
#else
   pthread_mutex_lock(&gCallTokenMutex);
#endif
#endif

}

void ooMutexReleaseCallTokenMutex(void)
{

#ifdef USE_THREADS
#ifdef _WIN32
   LeaveCriticalSection(&gCallTokenMutex);
#else
   pthread_mutex_unlock(&gCallTokenMutex);
#endif
#endif

}

void ooMutexDestroyCallTokenMutex(void)
{

#ifdef USE_THREADS
#ifdef _WIN32
   DeleteCriticalSection(&gCallTokenMutex);
#endif
#endif

}
