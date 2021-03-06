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
#ifndef __LIBRARY_COMMON_PARADEF_H__
#define __LIBRARY_COMMON_PARADEF_H__

#define FILE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH)

#define SEMAPHORE_NAME  "www.gzjp.cn"

/* 1ms */
#define TIME_TICKS                  1000

/* buffer size def */
#define MAX_BUFFER_SIZE_016         16

#define MAX_BUFFER_SIZE_032         32

#define MAX_BUFFER_SIZE_064         64

#define MAX_BUFFER_SIZE_128         128

#define MAX_BUFFER_SIZE_256         256

#define MAX_BUFFER_SIZE_384         384

#define MAX_BUFFER_SIZE_512         512

#define MAX_BUFFER_SIZE_01k         (1 * 1024)

#define MAX_BUFFER_SIZE_02k         (2 * 1024)

#define MAX_BUFFER_SIZE_03k			(3 * 1024)

#define MAX_BUFFER_SIZE_04k         (4 * 1024)

#define MAX_BUFFER_SIZE_05k         (5 * 1024)

#define MAX_BUFFER_SIZE_08k         (8 * 1024)

#define MAX_BUFFER_SIZE_10k         (10 * 1024)

#define MAX_BUFFER_SIZE_20k         (20 * 1024)

#define MAX_BUFFER_SIZE_40k         (40 * 1024)

#define MAX_BUFFER_SIZE_50k         (50 * 1024)

#define MAX_BUFFER_SIZE_100k        (100 * 1024)

#define MAX_BUFFER_SIZE_300k        (100 * 1024)

#define MAX_BUFFER_SIZE_500k        (500 * 1024)

#define MAX_BUFFER_SIZE_700k        (700 * 1024)

#define MAX_BUFFER_SIZE_01m         (1024 * 1024)

#define MAX_BUFFER_SIZE_02m         (2* 1024 * 1024)

#define MAX_BUFFER_SIZE_03m         (3* 1024 * 1024)

#define MAX_BUFFER_SIZE_04m         (4* 1024 * 1024)

#define MAX_BUFFER_SIZE_08m         (8* 1024 * 1024)

#define MAX_BUFFER_SIZE_16m         (16* 1024 * 1024)

////////////////////////////////////////////////////////////////////////////////
#if(!defined(_ZH_PLATFORM_))
#define NETBUFFER					RemoteByteArrayBuffer
#else
#define NETBUFFER					LocalByteArrayBuffer
#endif

#endif