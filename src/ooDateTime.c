/*
 * Copyright (C) 2004-2005 by Objective Systems, Inc.
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

#include "ooCommon.h"

#if defined(_WIN32) || defined(_MSC_VER) || defined(__MINGW32__)
#include <SYS\TIMEB.H>
int gettimeofday (struct timeval *tv, struct timezone *tz)
{
   struct _timeb currSysTime;
   _ftime(&currSysTime);
        
   tv->tv_sec = currSysTime.time;
   tv->tv_usec = currSysTime.millitm * 1000;

   return 0;
}
#endif

int ooGetTimeOfDay (struct timeval *tv, struct timezone *tz)
{
   return gettimeofday (tv, tz);
}
