/*
 * Copyright (C) 2004 by Objective Systems, Inc.
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
#ifndef _G711_H_
#define _G711_H_

static int search(int val,  /* changed from "short" *drago* */
                  int* table,
                  int size);

int linear2alaw(int pcm_val);

int alaw2linear(int a_val);

int linear2ulaw( int pcm_val);

int ulaw2linear( int u_val);

#if 0

/* A-law to u-law conversion */
static int alaw2ulaw (int aval);

static int ulaw2alaw (int uval);

#endif


#endif
