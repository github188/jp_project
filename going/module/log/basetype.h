/***************************************************************************
 *   Copyright (C) 2015 by Y.S.G   										   *
 *   ysgen0217@163.com   												   *
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
#ifndef __LIBRARY_COMMON_BASETYPE_H__
#define __LIBRARY_COMMON_BASETYPE_H__

#include <assert.h>
#include <sys/types.h>
#include <inttypes.h>
#include <stddef.h>

typedef char int8;

typedef unsigned char uint8;

typedef const char cchar;

typedef const unsigned char cuchar;

typedef short int16;

typedef unsigned short uint16;

typedef int int32;

typedef unsigned int uint32;

typedef long long int64;

typedef unsigned long long uint64;

typedef uintptr_t uspointer;

typedef intptr_t spointer;

typedef size_t  msize_t;

typedef ssize_t mssize_t;

const signed int STATUS_ERROR = 0xFFFFFFFF;

const int STATUS_SUCCESS = 0;

#ifndef FALSE
#define FALSE 0u

#define TRUE 1u
#endif

#define ASSERT(assertion) { if ( !(assertion) ) {fprintf( stderr, "\n%s:%i ASSERTION FAILED:\n  %s\n", __FILE__, __LINE__, #assertion ); assert( #assertion &&0 ); } }

#endif
