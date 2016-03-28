/*   $NetBSD: insstr.c,v 1.3 2009/07/22 16:57:15 roy Exp $ */

/*
 * Copyright (c) 2005 The NetBSD Foundation Inc.
 * All rights reserved.
 *
 * This code is derived from code donated to the NetBSD Foundation
 * by Ruibiao Qiu <ruibiao@arl.wustl.edu,ruibiao@gmail.com>.
 *
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *	notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *	notice, this list of conditions and the following disclaimer in the
 *	documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the NetBSD Foundation nor the names of its
 *	contributors may be used to endorse or promote products derived
 *	from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE NETBSD FOUNDATION, INC. AND
 * CONTRIBUTORS ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <string.h>
#include <stdlib.h>

#include "curses.h"
#include "curses_private.h"

#ifndef _CURSES_USE_MACROS

/*
 * insstr --
 *	insert a multi-byte character string into the current window
 */
int
insstr(const char *str)
{
	return winsstr(stdscr, str);
}

/*
 * insnstr --
 *	insert a multi-byte character string into the current window
 *	with at most n characters
 */
int
insnstr(const char *str, int n)
{
	return winsnstr(stdscr, str, n);
}

/*
 * mvinsstr --
 *	  Do an insert-string on the line at (y, x).
 */
int
mvinsstr(int y, int x, const char *str)
{
	return mvwinsstr(stdscr, y, x, str);
}

/*
 * mvinsnstr --
 *	  Do an insert-n-string on the line at (y, x).
 */
int
mvinsnstr(int y, int x, const char *str, int n)
{
	return mvwinsnstr(stdscr, y, x, str, n);
}

/*
 * mvwinsstr --
 *	  Do an insert-string on the line at (y, x) in the given window.
 */
int
mvwinsstr(WINDOW *win, int y, int x, const char *str)
{
	if (wmove(win, y, x) == ERR)
		return ERR;

	return winsstr(stdscr, str);
}

/*
 * mvwinsnstr --
 *	  Do an insert-n-string on the line at (y, x) in the given window.
 */
int
mvwinsnstr(WINDOW *win, int y, int x, const char *str, int n)
{
	if (wmove(win, y, x) == ERR)
		return ERR;

	return winsnstr(stdscr, str, n);
}

#endif

/*
 * winsstr --
 *	Do an insert-string on the line, leaving (cury, curx) unchanged.
 *	No wrapping.
 */
int
winsstr(WINDOW *win, const char *str)
{
	return winsnstr( win, str, -1 );
}

/*
 * winsnstr --
 *	Do an insert-n-string on the line, leaving (cury, curx) unchanged.
 *	Performs wrapping.
 */
int
winsnstr(WINDOW *win, const char *str, int n)
{
	__LDATA	*end, *temp1, *temp2;
	const char *scp;
	int len, x;
	__LINE *lnp;
	nschar_t *np, *tnp;

	/* find string length */
	if ( n > 0 )
		for ( scp = str, len = 0; n-- && *scp++; ++len );
	else
		for ( scp = str, len = 0; *scp++; ++len );

	/* move string */
	end = &win->alines[win->cury]->line[win->curx];
	if ( len < win->maxx - win->curx ) {
		temp1 = &win->alines[win->cury]->line[win->maxx - 1];
		temp2 = temp1 - len;
		while (temp2 >= end) {
			np = temp1->nsp;
			if (np){
				while ( np ) {
					tnp = np->next;
					free( np );
					np = tnp;
				}
				temp1->nsp = NULL;
			}
			(void) memcpy(temp1, temp2, sizeof(__LDATA));
			temp1--, temp2--;
		}
	}

	for ( scp = str, temp1 = end, x = win->curx;
			*scp && x < len + win->curx && x < win->maxx;
			scp++, temp1++, x++ ) {
		temp1->ch = (wchar_t)*scp & __CHARTEXT;
		temp1->attr = win->wattr;
		SET_WCOL( *temp1, 1 );
	}
	lnp = win->alines[ win->cury ];
	lnp->flags |= __ISDIRTY;
	if ( win->ch_off < *lnp->firstchp )
		*lnp->firstchp = win->ch_off;
	if ( win->ch_off + win->maxx - 1 > *lnp->lastchp )
		*lnp->lastchp = win->ch_off + win->maxx - 1;
	__touchline(win, (int)win->cury, (int)win->curx, (int)win->maxx - 1);
	return OK;
}
