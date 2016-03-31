/*	$NetBSD: border.c,v 1.15 2013/05/05 14:23:16 jdc Exp $	*/

/*
 * Copyright (c) 2000 The NetBSD Foundation, Inc.
 * All rights reserved.
 *
 * This code is derived from software contributed to The NetBSD Foundation
 * by Julian Coleman.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE NETBSD FOUNDATION, INC. AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdlib.h>
#include <string.h>

#include "curses.h"
#include "curses_private.h"

#ifndef _CURSES_USE_MACROS

/*
 * border --
 *	Draw a border around stdscr using the specified
 *	delimiting characters.
 */
int
border(chtype left, chtype right, chtype top, chtype bottom, chtype topleft,
       chtype topright, chtype botleft, chtype botright)
{
	return wborder(stdscr, left, right, top, bottom, topleft, topright,
	    botleft, botright);
}

#endif

/*
 * wborder --
 *	Draw a border around the given window using the specified delimiting
 *	characters.
 */
int
wborder(WINDOW *win, chtype left, chtype right, chtype top, chtype bottom,
	chtype topleft, chtype topright, chtype botleft, chtype botright)
{
	cchar_t ls, rs, ts, bs, tl, tr, bl, br;
	cchar_t *lsp, *rsp, *tsp, *bsp, *tlp, *trp, *blp, *brp;

#define S(in, out, def) \
	if (in & __CHARTEXT) { \
		__cursesi_chtype_to_cchar(in, &out); \
	} else { \
		memcpy(&out, def, sizeof(cchar_t)); \
		out.attributes |= in & __ATTRIBUTES; \
	} \
	out##p = &out;

	S(left, ls, WACS_VLINE);
	S(right, rs, WACS_VLINE);
	S(top, ts, WACS_HLINE);
	S(bottom, bs, WACS_HLINE);
	S(topleft, tl, WACS_ULCORNER);
	S(topright, tr, WACS_URCORNER);
	S(botleft, bl, WACS_LLCORNER);
	S(botright, br, WACS_LRCORNER);
#undef S
	return wborder_set(win, lsp, rsp, tsp, bsp, tlp, trp, blp, brp);
}

int border_set(const cchar_t *ls, const cchar_t *rs, const cchar_t *ts,
	   const cchar_t *bs, const cchar_t *tl, const cchar_t *tr,
	   const cchar_t *bl, const cchar_t *br)
{
	return wborder_set(stdscr, ls, rs, ts, bs, tl, tr, bl, br);
}

int wborder_set(WINDOW *win, const cchar_t *ls, const cchar_t *rs,
		const cchar_t *ts, const cchar_t *bs,
		const cchar_t *tl, const cchar_t *tr,
		const cchar_t *bl, const cchar_t *br)
{
	int	 endy, endx, i, j, cw, pcw, tlcw, blcw, trcw, brcw;
	unsigned int k;
	cchar_t left, right, bottom, top, topleft, topright, botleft, botright;
	nschar_t *np, *tnp;

	if ( ls && wcwidth( ls->vals[ 0 ]))
		memcpy( &left, ls, sizeof( cchar_t ));
	else
		memcpy( &left, WACS_VLINE, sizeof( cchar_t ));
	if ( rs && wcwidth( rs->vals[ 0 ]))
		memcpy( &right, rs, sizeof( cchar_t ));
	else
		memcpy( &right, WACS_VLINE, sizeof( cchar_t ));
	if ( ts && wcwidth( ts->vals[ 0 ]))
		memcpy( &top, ts, sizeof( cchar_t ));
	else
		memcpy( &top, WACS_HLINE, sizeof( cchar_t ));
	if ( bs && wcwidth( bs->vals[ 0 ]))
		memcpy( &bottom, bs, sizeof( cchar_t ));
	else
		memcpy( &bottom, WACS_HLINE, sizeof( cchar_t ));
	if ( tl && wcwidth( tl->vals[ 0 ]))
		memcpy( &topleft, tl, sizeof( cchar_t ));
	else
		memcpy( &topleft, WACS_ULCORNER, sizeof( cchar_t ));
	if ( tr && wcwidth( tr->vals[ 0 ]))
		memcpy( &topright, tr, sizeof( cchar_t ));
	else
		memcpy( &topright, WACS_URCORNER, sizeof( cchar_t ));
	if ( bl && wcwidth( bl->vals[ 0 ]))
		memcpy( &botleft, bl, sizeof( cchar_t ));
	else
		memcpy( &botleft, WACS_LLCORNER, sizeof( cchar_t ));
	if ( br && wcwidth( br->vals[ 0 ]))
		memcpy( &botright, br, sizeof( cchar_t ));
	else
		memcpy( &botright, WACS_LRCORNER, sizeof( cchar_t ));

	/* Merge window attributes */
	left.attributes |= (left.attributes & __COLOR) ?
		(win->wattr & ~__COLOR) : win->wattr;
	left.attributes |= (left.attributes & __COLOR) ?
		(win->battr & ~__COLOR) : win->battr;
	right.attributes |= (right.attributes & __COLOR) ?
		(win->wattr & ~__COLOR) : win->wattr;
	right.attributes |= (right.attributes & __COLOR) ?
		(win->battr & ~__COLOR) : win->battr;
	top.attributes |= (top.attributes & __COLOR) ?
		(win->wattr & ~__COLOR) : win->wattr;
	top.attributes |= (top.attributes & __COLOR) ?
		(win->battr & ~__COLOR) : win->battr;
	bottom.attributes |= (bottom.attributes & __COLOR) ?
		(win->wattr & ~__COLOR) : win->wattr;
	bottom.attributes |= (bottom.attributes & __COLOR) ?
		(win->battr & ~__COLOR) : win->battr;
	topleft.attributes |= (topleft.attributes & __COLOR) ?
		(win->wattr & ~__COLOR) : win->wattr;
	topleft.attributes |= (topleft.attributes & __COLOR) ?
		(win->battr & ~__COLOR) : win->battr;
	topright.attributes |= (topright.attributes & __COLOR) ?
		(win->wattr & ~__COLOR) : win->wattr;
	topright.attributes |= (topright.attributes & __COLOR) ?
		(win->battr & ~__COLOR) : win->battr;
	botleft.attributes |= (botleft.attributes & __COLOR) ?
		(win->wattr & ~__COLOR) : win->wattr;
	botleft.attributes |= (botleft.attributes & __COLOR) ?
		(win->battr & ~__COLOR) : win->battr;
	botright.attributes |= (botright.attributes & __COLOR) ?
		(win->wattr & ~__COLOR) : win->wattr;
	botright.attributes |= (botright.attributes & __COLOR) ?
		(win->battr & ~__COLOR) : win->battr;

	endx = win->maxx - 1;
	endy = win->maxy - 1;

	/* Sides */
	for (i = 1; i < endy; i++) {
		/* left border */
		cw = wcwidth( left.vals[ 0 ]);
		if (cw < 0)
			cw = 1;
		for ( j = 0; j < cw; j++ ) {
			win->alines[i]->line[j].ch = left.vals[ 0 ];
			win->alines[i]->line[j].attr = left.attributes;
			np = win->alines[i]->line[j].nsp;
			if (np) {
				while ( np ) {
					tnp = np->next;
					free( np );
					np = tnp;
				}
				win->alines[i]->line[j].nsp = NULL;
			}
			if ( j )
				SET_WCOL( win->alines[i]->line[j], -j );
			else {
				SET_WCOL( win->alines[i]->line[j], cw );
				if ( left.elements > 1 ) {
					for (k = 1; k < left.elements; k++) {
						np = (nschar_t *)malloc(sizeof(nschar_t));
						if (!np)
							return ERR;
						np->ch = left.vals[ k ];
						np->next = win->alines[i]->line[j].nsp;
						win->alines[i]->line[j].nsp
							= np;
					}
				}
			}
		}
		for ( j = cw; WCOL( win->alines[i]->line[j]) < 0; j++ ) {
			win->alines[i]->line[j].ch = ( wchar_t )btowc(win->bch);
			if (_cursesi_copy_nsp(win->bnsp,
					      &win->alines[i]->line[j]) == ERR)
				return ERR;
			SET_WCOL( win->alines[i]->line[j], 1 );
		}
		/* right border */
		cw = wcwidth( right.vals[ 0 ]);
		if (cw < 0)
			cw = 1;
		pcw = WCOL( win->alines[i]->line[endx - cw]);
		for ( j = endx - cw + 1; j <= endx; j++ ) {
			win->alines[i]->line[j].ch = right.vals[ 0 ];
			win->alines[i]->line[j].attr = right.attributes;
			np = win->alines[i]->line[j].nsp;
			if (np) {
				while ( np ) {
					tnp = np->next;
					free( np );
					np = tnp;
				}
				win->alines[i]->line[j].nsp = NULL;
			}
			if ( j == endx - cw + 1 ) {
				SET_WCOL( win->alines[i]->line[j], cw );
				if ( right.elements > 1 ) {
					for (k = 1; k < right.elements; k++) {
						np = (nschar_t *)malloc(sizeof(nschar_t));
						if (!np)
							return ERR;
						np->ch = right.vals[ k ];
						np->next = win->alines[i]->line[j].nsp;
						win->alines[i]->line[j].nsp
							= np;
					}
				}
			} else
				SET_WCOL( win->alines[i]->line[j],
					endx - cw + 1 - j );
		}
		if ( pcw != 1 ) {
			k = pcw < 0 ? endx -cw + pcw : endx - cw;
			for ( j = endx - cw; j >= (signed)k; j-- ) {
				win->alines[i]->line[j].ch
					= (wchar_t)btowc(win->bch);
				if (_cursesi_copy_nsp(win->bnsp,
					       &win->alines[i]->line[j]) == ERR)
					return ERR;
				win->alines[i]->line[j].attr = win->battr;
				SET_WCOL( win->alines[i]->line[j], 1 );
			}
		}
	}
	tlcw = wcwidth( topleft.vals[ 0 ]);
	if (tlcw < 0)
		tlcw = 1;
	blcw = wcwidth( botleft.vals[ 0 ]);
	if (blcw < 0)
		blcw = 1;
	trcw = wcwidth( topright.vals[ 0 ]);
	if (trcw < 0)
		trcw = 1;
	brcw = wcwidth( botright.vals[ 0 ]);
	if (brcw < 0)
		brcw = 1;
	/* upper border */
	cw = wcwidth( top.vals[ 0 ]);
	if (cw < 0)
		cw = 1;
	for (i = tlcw; i <= min( endx - cw, endx - trcw ); i += cw ) {
		for ( j = 0; j < cw; j++ ) {
			win->alines[ 0 ]->line[i + j].ch = top.vals[ 0 ];
			win->alines[ 0 ]->line[i + j].attr = top.attributes;
			np = win->alines[ 0 ]->line[i + j].nsp;
			if (np) {
				while ( np ) {
					tnp = np->next;
					free( np );
					np = tnp;
				}
				win->alines[ 0 ]->line[i + j].nsp = NULL;
			}
			if ( j )
				SET_WCOL( win->alines[ 0 ]->line[ i + j ], -j );
			else {
				SET_WCOL( win->alines[ 0 ]->line[ i + j ], cw );
				if ( top.elements > 1 ) {
					for ( k = 1; k < top.elements; k++ ) {
						np = (nschar_t *)malloc(sizeof(nschar_t));
						if (!np)
							return ERR;
						np->ch = top.vals[ k ];
						np->next = win->alines[0]->line[i + j].nsp;
						win->alines[0]->line[i + j].nsp
							= np;
					}
				}
			}
		}
	}
	while ( i <= endx - trcw ) {
		win->alines[0]->line[i].ch =
			( wchar_t )btowc(( int ) win->bch );
		if (_cursesi_copy_nsp(win->bnsp,
				      &win->alines[0]->line[i]) == ERR)
			return ERR;
		win->alines[ 0 ]->line[ i ].attr = win->battr;
		SET_WCOL( win->alines[ 0 ]->line[ i ], 1 );
		i++;
	}
	/* lower border */
	for (i = blcw; i <= min( endx - cw, endx - brcw ); i += cw ) {
		for ( j = 0; j < cw; j++ ) {
			win->alines[ endy ]->line[i + j].ch = bottom.vals[ 0 ];
			win->alines[endy]->line[i + j].attr = bottom.attributes;
			np = win->alines[ endy ]->line[i + j].nsp;
			if (np) {
				while ( np ) {
					tnp = np->next;
					free( np );
					np = tnp;
				}
				win->alines[ endy ]->line[i + j].nsp = NULL;
			}
			if ( j )
				SET_WCOL( win->alines[endy]->line[i + j], -j);
			else {
				SET_WCOL( win->alines[endy]->line[i + j], cw );
				if ( bottom.elements > 1 ) {
					for ( k = 1; k < bottom.elements;
							k++ ) {
						if ( !( np = ( nschar_t *)malloc( sizeof( nschar_t ))))
							return ERR;
						np->ch = bottom.vals[ k ];
						np->next = win->alines[endy]->line[i + j].nsp;
						win->alines[endy]->line[i + j].nsp = np;
					}
				}
			}
		}
	}
	while ( i <= endx - brcw ) {
		win->alines[endy]->line[i].ch =
			(wchar_t)btowc((int) win->bch );
		if (_cursesi_copy_nsp(win->bnsp,
				      &win->alines[endy]->line[i]) == ERR)
			return ERR;
		win->alines[ endy ]->line[ i ].attr = win->battr;
		SET_WCOL( win->alines[ endy ]->line[ i ], 1 );
		i++;
	}

	/* Corners */
	if (!(win->maxx == LINES && win->maxy == COLS &&
		(win->flags & __SCROLLOK) && (win->flags & __SCROLLWIN))) {
		for ( i = 0; i < tlcw; i++ ) {
			win->alines[ 0 ]->line[i].ch = topleft.vals[ 0 ];
			win->alines[ 0 ]->line[i].attr = topleft.attributes;
			np = win->alines[ 0 ]->line[i].nsp;
			if (np) {
				while ( np ) {
					tnp = np->next;
					free( np );
					np = tnp;
				}
				win->alines[ 0 ]->line[i].nsp = NULL;
			}
			if ( i )
				SET_WCOL( win->alines[ 0 ]->line[ i ], -i );
			else {
				SET_WCOL( win->alines[ 0 ]->line[ i ], tlcw );
				if ( topleft.elements > 1 ) {
					for ( k = 1; k < topleft.elements;
							k++ ) {
						np = (nschar_t *)malloc(sizeof(nschar_t));
						if (!np)
							return ERR;
						np->ch = topleft.vals[ k ];
						np->next = win->alines[ 0 ]->line[i].nsp;
						win->alines[ 0 ]->line[i].nsp
							= np;
					}
				}
			}
		}
		for ( i = endx - trcw + 1; i <= endx; i++ ) {
			win->alines[ 0 ]->line[i].ch = topright.vals[ 0 ];
			win->alines[ 0 ]->line[i].attr = topright.attributes;
			np = win->alines[ 0 ]->line[i].nsp;
			if (np) {
				while ( np ) {
					tnp = np->next;
					free( np );
					np = tnp;
				}
				win->alines[ 0 ]->line[i].nsp = NULL;
			}
			if ( i == endx - trcw + 1 ) {
				SET_WCOL( win->alines[ 0 ]->line[ i ], trcw );
				if ( topright.elements > 1 ) {
					for ( k = 1; k < topright.elements;
							k++ ) {
						np = (nschar_t *)malloc(sizeof(nschar_t));
						if (!np)
							return ERR;
						np->ch = topright.vals[ k ];
						np->next = win->alines[0]->line[i].nsp;
						win->alines[ 0 ]->line[i].nsp
							= np;
					}
				}
			} else
				SET_WCOL( win->alines[ 0 ]->line[ i ],
					  endx - trcw + 1 - i );
		}
		for ( i = 0; i < blcw; i++ ) {
			win->alines[ endy ]->line[i].ch = botleft.vals[ 0 ];
			win->alines[ endy ]->line[i].attr = botleft.attributes;
			np = win->alines[ endy ]->line[i].nsp;
			if (np) {
				while ( np ) {
					tnp = np->next;
					free( np );
					np = tnp;
				}
				win->alines[ endy ]->line[i].nsp = NULL;
			}
			if ( i )
				SET_WCOL( win->alines[endy]->line[i], -i );
			else {
				SET_WCOL( win->alines[endy]->line[i], blcw );
				if ( botleft.elements > 1 ) {
					for ( k = 1; k < botleft.elements;
							k++ ) {
						np = (nschar_t *)malloc(sizeof(nschar_t));
						if (!np)
							return ERR;
						np->ch = botleft.vals[ k ];
						np->next = win->alines[endy]->line[i].nsp;
						win->alines[endy]->line[i].nsp
							= np;
					}
				}
			}
		}
		for ( i = endx - brcw + 1; i <= endx; i++ ) {
			win->alines[ endy ]->line[i].ch = botright.vals[ 0 ];
			win->alines[ endy ]->line[i].attr = botright.attributes;
			np = win->alines[ endy ]->line[i].nsp;
			if (np) {
				while ( np ) {
					tnp = np->next;
					free( np );
					np = tnp;
				}
				win->alines[ endy ]->line[i].nsp = NULL;
			}
			if ( i == endx - brcw + 1 ) {
				SET_WCOL( win->alines[ endy ]->line[ i ],
					  brcw );
				if ( botright.elements > 1 ) {
					for ( k = 1; k < botright.elements; k++ ) {
						np = (nschar_t *)malloc(sizeof(nschar_t));
						if (!np)
							return ERR;
						np->ch = botright.vals[ k ];
						np->next = win->alines[endy]->line[i].nsp;
						win->alines[endy]->line[i].nsp
							= np;
					}
				}
			} else
				SET_WCOL( win->alines[ endy ]->line[ i ],
					endx - brcw + 1 - i );
		}
	}
	__touchwin(win);
	return (OK);
}
