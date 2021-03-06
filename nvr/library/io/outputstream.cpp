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
#include "outputstream.h"

using namespace library;

OutputStream::OutputStream() : mutex()
{
}

OutputStream::~OutputStream()
{
}

int32 OutputStream::write(const uint8 *buffer, int32 size, bool bSyncWriting)
{
	if (buffer == NULL) return 0;
	if (size <= 0) return 0;

	return this->doWriteByteArray(buffer, size, 0, size, bSyncWriting);
}

int32 OutputStream::write(const uint8 *buffer, int32 size, int32 offset, int32 length, bool bSyncWriting)
{
	if (buffer == NULL) {

		Log(LOG_ERROR, "outputstream buffer is null.");
		return 0;
	}
	if (size <= 0 || offset < 0 || length < 0) {

		Log(LOG_ERROR, "outputstream size | offset | length is negative.");
		return 0;
	}
	
	if ((length - offset) >= size) {

		Log(LOG_ERROR, "outputstream length: %d out of bound.", length);
		return 0;
	}

	return this->doWriteByteArray(buffer, size, offset, length, bSyncWriting);
}

//do nothing
void OutputStream::flush()
{
}

void OutputStream::close()
{
}

bool OutputStream::isclosed()
{
	return FALSE;
}