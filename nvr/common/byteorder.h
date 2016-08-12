/***************************************************************************
 *   Copyright (C) 2015 by root   				   						   *
 *   ysgen0217@163.com   							   					   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "basetype.h"

#if defined(__i386__)||defined(WIN32)

#define XHTONS
#define XHTONL
#define XHTONLL

#else /* big end */

inline uint16 XHTONS(uint16 i16) {
		return ((i16 << 8) | (i16 >> 8));
}
inline uint32 XHTONL(uint32 i32) {
		return ((uint32(XHTONS(i32)) << 16) | XHTONS(i32>>16));
}
inline uint64 XHTONLL(uint64 i64) {
		return ((uint64(XHTONL((uint32)i64)) << 32) |XHTONL((uint32(i64>>32))));
}

#endif /* __i386__ */

#define XNTOHS XHTONS
#define XNTOHL XHTONL
#define XNTOHLL XHTONLL

static uint16 xhtons(uint16 i16) { return XHTONS(i16); }
		
static uint32 xhtonl(uint32 i32) { return XHTONL(i32); }

static uint64 xhtonll(uint64 i64) { return XHTONLL(i64); }

static uint16 xntohs(uint16 i16)  { return XNTOHS(i16); }

static uint32 xntohl(uint32 i32)  { return XNTOHL(i32); }

static uint64 xntohll(uint64 i64)  { return XNTOHLL(i64); }