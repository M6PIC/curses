/*	$NetBSD: printw.c,v 1.22 2011/07/17 20:54:34 joerg Exp $	*/

/*
 * Copyright (c) 1981, 1993, 1994
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <stdarg.h>
#include <stdlib.h>

#include "curses.h"
#include "curses_private.h"

/*
 * printw and friends.
 */

static int __winwrite (void *, const char *, int);

/*
 * printw --
 *	Printf on the standard screen.
 */
int
printw(const char *fmt,...)
{
	va_list ap;
	int     ret;

	va_start(ap, fmt);
	ret = vw_printw(stdscr, fmt, ap);
	va_end(ap);
	return (ret);
}
/*
 * wprintw --
 *	Printf on the given window.
 */
int
wprintw(WINDOW *win, const char *fmt,...)
{
	va_list ap;
	int     ret;

	va_start(ap, fmt);
	ret = vw_printw(win, fmt, ap);
	va_end(ap);
	return (ret);
}
/*
 * mvprintw, mvwprintw --
 *	Implement the mvprintw commands.  Due to the variable number of
 *	arguments, they cannot be macros.  Sigh....
 */
int
mvprintw(int y, int x, const char *fmt,...)
{
	va_list ap;
	int     ret;

	if (move(y, x) != OK)
		return (ERR);
	va_start(ap, fmt);
	ret = vw_printw(stdscr, fmt, ap);
	va_end(ap);
	return (ret);
}

int
mvwprintw(WINDOW * win, int y, int x, const char *fmt,...)
{
	va_list ap;
	int     ret;

	if (wmove(win, y, x) != OK)
		return (ERR);

	va_start(ap, fmt);
	ret = vw_printw(win, fmt, ap);
	va_end(ap);
	return (ret);
}
/*
 * Internal write-buffer-to-window function.
 */
static int
__winwrite(cookie, buf, n)
	void   *cookie;
	const char *buf;
	int     n;
{
	WINDOW *win;
	int     c;

	for (c = n, win = cookie; --c >= 0;)
	{
		if (waddch(win, (chtype) (*buf++ & __CHARTEXT)) == ERR)
			return (-1);
	}
	return (n);
}
/*
 * vw_printw --
 *	This routine actually executes the printf and adds it to the window.
 */
int
vw_printw(WINDOW *win, const char *fmt, va_list ap)
{
	/*FILE   *f;

	if ((f = funopen(win, NULL, __winwrite, NULL, NULL)) == NULL)
		return (ERR);
	(void) vfprintf(f, fmt, ap);
	return (fclose(f) ? ERR : OK);*/

	va_list aq;
	va_copy(aq, ap); // Save va_list

	size_t needed = vsnprintf(NULL, 0, fmt, aq);
	char *buf = malloc(needed);
	if (!buf) return ERR;
	vsnprintf(buf, needed, fmt, ap);
	__winwrite(win, buf, needed);
	free(buf);

	va_end(aq);

	return OK;

}

//__strong_alias(vwprintw, vw_printw)
