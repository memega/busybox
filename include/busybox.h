/* vi: set sw=4 ts=4: */
/*
 * Busybox main internal header file
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 * Based in part on code from sash, Copyright (c) 1999 by David I. Bell 
 * Permission has been granted to redistribute this code under the GPL.
 *
 */
#ifndef	_BB_INTERNAL_H_
#define	_BB_INTERNAL_H_    1

#include "config.h"

#include <stdio.h>
#include <stdarg.h>
#include <sys/stat.h>
#include <sys/types.h>

#define BB_BANNER "BusyBox v" BB_VER " (" BB_BT ")"

#ifdef DMALLOC
#include <dmalloc.h>
#endif

#include <features.h>

/* Pull in the utility routines from libbb */
#include "libbb.h"

enum Location {
	_BB_DIR_ROOT = 0,
	_BB_DIR_BIN,
	_BB_DIR_SBIN,
	_BB_DIR_USR_BIN,
	_BB_DIR_USR_SBIN
};

enum SUIDRoot {
	_BB_SUID_NEVER = 0,
	_BB_SUID_MAYBE,
	_BB_SUID_ALWAYS
};

struct BB_applet {
	const char *name;
	int (*main) (int argc, char **argv);
	enum Location location:4;
	enum SUIDRoot need_suid:4;
};

/* From busybox.c */
extern const struct BB_applet applets[];

/* Automagically pull in all the applet function prototypes and
 * applet usage strings.  These are all of the form:
 *		extern int foo_main(int argc, char **argv);
 *		extern const char foo_usage[];
 * These are all autogenerated from the set of currently defined applets. 
 */
#define PROTOTYPES
#include "applets.h"
#undef PROTOTYPES

#ifdef CONFIG_FEATURE_BUFFERS_GO_ON_STACK
#define RESERVE_CONFIG_BUFFER(buffer,len)           char buffer[len]
#define RESERVE_CONFIG_UBUFFER(buffer,len) unsigned char buffer[len]
#define RELEASE_CONFIG_BUFFER(buffer)      ((void)0)
#else
#ifdef CONFIG_FEATURE_BUFFERS_GO_IN_BSS
#define RESERVE_CONFIG_BUFFER(buffer,len)  static          char buffer[len]
#define RESERVE_CONFIG_UBUFFER(buffer,len) static unsigned char buffer[len]
#define RELEASE_CONFIG_BUFFER(buffer)      ((void)0)
#else
#define RESERVE_CONFIG_BUFFER(buffer,len)           char *buffer=xmalloc(len)
#define RESERVE_CONFIG_UBUFFER(buffer,len) unsigned char *buffer=xmalloc(len)
#define RELEASE_CONFIG_BUFFER(buffer)      free (buffer)
#endif
#endif


/* Bit map related macros -- libc5 doens't provide these... sigh.  */
#ifndef setbit
#define NBBY            CHAR_BIT
#define setbit(a,i)     ((a)[(i)/NBBY] |= 1<<((i)%NBBY))
#define clrbit(a,i)     ((a)[(i)/NBBY] &= ~(1<<((i)%NBBY)))
#define isset(a,i)      ((a)[(i)/NBBY] & (1<<((i)%NBBY)))
#define isclr(a,i)      (((a)[(i)/NBBY] & (1<<((i)%NBBY))) == 0)
#endif

#ifndef RB_POWER_OFF
/* Stop system and switch power off if possible.  */
#define RB_POWER_OFF   0x4321fedc
#endif

/* Try to pull in PATH_MAX */
#include <limits.h>

/* for PATH_MAX on systems that don't have it in limits.h */
#include <sys/param.h>
#ifndef PATH_MAX
#define  PATH_MAX         256
#endif

#endif							/* _BB_INTERNAL_H_ */
