/*	$NetBSD: line.c,v 1.6 2010/02/23 19:48:26 drochner Exp $	*/

/*-
 * Copyright (c) 1998-1999 Brett Lymn
 *						 (blymn@baea.com.au, brett_lymn@yahoo.com.au)
 * All rights reserved.
 *
 * This code has been donated to The NetBSD Foundation by the Author.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *
 */

#include <string.h>

#include "curses.h"
#include "curses_private.h"

/*
 * hline --
 *	Draw a horizontal line of character c on stdscr.
 */
int
hline(chtype ch, int count)
{
	return whline(stdscr, ch, count);
}

/*
 * mvhline --
 *	Move to location (y, x) and draw a horizontal line of character c
 *	on stdscr.
 */
int
mvhline(int y, int x, chtype ch, int count)
{
	return mvwhline(stdscr, y, x, ch, count);
}

/*
 * mvwhline --
 *	Move to location (y, x) and draw a horizontal line of character c
 *	in the given window.
 */
int
mvwhline(WINDOW *win, int y, int x, chtype ch, int count)
{
	if (wmove(win, y, x) == ERR)
		return ERR;

	return whline(win, ch, count);
}

/*
 * whline --
 *	Draw a horizontal line of character c in the given window moving
 *	towards the rightmost column.  At most count characters are drawn
 *	or until the edge of the screen, whichever comes first.
 */
int
whline(WINDOW *win, chtype ch, int count)
{
	cchar_t cch, *cchp;

	if (ch & __CHARTEXT) {
		__cursesi_chtype_to_cchar(ch, &cch);
		cchp = & cch;
	} else
		cchp = WACS_HLINE;

	return whline_set(win, cchp, count);
}

/*
 * vline --
 *	Draw a vertical line of character ch on stdscr.
 */
int
vline(chtype ch, int count)
{
	return wvline(stdscr, ch, count);
}

/*
 * mvvline --
 *	Move to the given location an draw a vertical line of character ch.
 */
int
mvvline(int y, int x, chtype ch, int count)
{
	return mvwvline(stdscr, y, x, ch, count);
}

/*
 * mvwvline --
 *	Move to the given location and draw a vertical line of character ch
 *	on the given window.
 */
int
mvwvline(WINDOW *win, int y, int x, chtype ch, int count)
{
	if (wmove(win, y, x) == ERR)
		return ERR;

	return wvline(win, ch, count);
}

/*
 * wvline --
 *	Draw a vertical line of character ch in the given window moving
 *	towards the bottom of the screen.  At most count characters are drawn
 *	or until the edge of the screen, whichever comes first.
 */
int
wvline(WINDOW *win, chtype ch, int count)
{
	cchar_t cch, *cchp;

	if (ch & __CHARTEXT) {
		__cursesi_chtype_to_cchar(ch, &cch);
		cchp = & cch;
	} else
		cchp = WACS_VLINE;

	return wvline_set(win, cchp, count);
}

int hline_set(const cchar_t *wch, int n)
{
	return whline_set( stdscr, wch, n );
}

int mvhline_set(int y, int x, const cchar_t *wch, int n)
{
	return mvwhline_set( stdscr, y, x, wch, n );
}

int mvwhline_set(WINDOW *win, int y, int x, const cchar_t *wch, int n)
{
	if ( wmove( win, y , x ) == ERR )
		return ERR;

	return whline_set( win, wch, n );
}

int whline_set(WINDOW *win, const cchar_t *wch, int n)
{
	int ocurx, wcn, i, cw;
	cchar_t cc;

	cw = wcwidth( wch->vals[ 0 ]);
	if (cw < 0)
		cw = 1;
	if ( ( win->maxx - win->curx ) < cw )
		return ERR;
	wcn = min( n, ( win->maxx - win->curx ) / cw );
	ocurx = win->curx;

	memcpy( &cc, wch, sizeof( cchar_t ));
	if (!(wch->vals[ 0 ]))
		cc.vals[ 0 ] |= WACS_HLINE->vals[0];
	for (i = 0; i < wcn; i++ ) {
		mvwadd_wch(win, win->cury, ocurx + i * cw, &cc);
	}
		
	wmove(win, win->cury, ocurx);
	return OK;
}

int vline_set(const cchar_t *wch, int n)
{
	return wvline_set( stdscr, wch, n );
}

int mvvline_set(int y, int x, const cchar_t *wch, int n)
{
	return mvwvline_set( stdscr, y, x, wch, n );
}

int mvwvline_set(WINDOW *win, int y, int x, const cchar_t *wch, int n)
{
	if ( wmove( win, y, x ) == ERR )
		return ERR;

	return wvline_set( win, wch, n );
}

int wvline_set(WINDOW *win, const cchar_t *wch, int n)
{
	int ocury, ocurx, wcn, i;
	cchar_t cc;

	wcn = min( n, win->maxy - win->cury);
	ocury = win->cury;
	ocurx = win->curx;

	memcpy( &cc, wch, sizeof( cchar_t ));
	if (!(wch->vals[ 0 ]))
		cc.vals[ 0 ] |= WACS_VLINE->vals[0];
	for (i = 0; i < wcn; i++) {
		mvwadd_wch(win, ocury + i, ocurx, &cc);
	}
	wmove(win, ocury, ocurx);
	return OK;
}
