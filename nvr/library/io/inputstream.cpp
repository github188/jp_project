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
#include "../util/log.h"
#include "inputstream.h"

using namespace library;

InputStream::InputStream()
{
}

InputStream::~InputStream()
{
}

int32 InputStream::read(uint8 *buffer, int32 size)
{
	if (buffer == NULL) return 0;
	if (size <= 0) return 0;
	
	return doReadByteArray(buffer, size, 0, size);
}

int32 InputStream::read(uint8 *buffer, int32 size, int32 offset, int32 length)
{
	if (buffer == NULL) {

		Log(LOG_ERROR, "buffer is null.");
		return 0;
	}
	
	if (size <= 0 || offset < 0 || length < 0) {

		Log(LOG_ERROR, "size OR offset OR length is negative.");
		return 0;
	}
	if ((length - offset) > size) {

		Log(LOG_ERROR, "length: %d out of bound.", length);
		return 0;
	}
	
	return doReadByteArray(buffer, size, offset, length);
}

void InputStream::close()
{
}

bool InputStream::avail()
{
	return FALSE;
}

bool InputStream::isclosed()
{
	return FALSE;
}