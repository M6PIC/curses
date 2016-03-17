/*	$NetBSD: endian.h,v 1.30 2016/02/27 21:37:35 christos Exp $	*/

/*
 * Copyright (c) 1987, 1991, 1993
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
 *
 *	@(#)endian.h	8.1 (Berkeley) 6/11/93
 */

#ifndef _SYS_ENDIAN_H_
#define _SYS_ENDIAN_H_

/*
 * Routines to encode/decode big- and little-endian multi-octet values
 * to/from an octet stream.
 */

#define __GEN_ENDIAN_ENC(bits, endian) \
static inline void \
endian ## bits ## enc(void *dst, uint ## bits ## _t u) \
{ \
	u = hto ## endian ## bits (u); \
	__builtin_memcpy(dst, &u, sizeof(u)); \
}

__GEN_ENDIAN_ENC(16, be)
__GEN_ENDIAN_ENC(32, be)
__GEN_ENDIAN_ENC(64, be)
__GEN_ENDIAN_ENC(16, le)
__GEN_ENDIAN_ENC(32, le)
__GEN_ENDIAN_ENC(64, le)
#undef __GEN_ENDIAN_ENC

#define __GEN_ENDIAN_DEC(bits, endian) \
static inline uint ## bits ## _t \
endian ## bits ## dec(const void *buf) \
{ \
	uint ## bits ## _t u; \
	__builtin_memcpy(&u, buf, sizeof(u)); \
	return endian ## bits ## toh (u); \
}

__GEN_ENDIAN_DEC(16, be)
__GEN_ENDIAN_DEC(32, be)
__GEN_ENDIAN_DEC(64, be)
__GEN_ENDIAN_DEC(16, le)
__GEN_ENDIAN_DEC(32, le)
__GEN_ENDIAN_DEC(64, le)
#undef __GEN_ENDIAN_DEC

#endif /* !_SYS_ENDIAN_H_ */
