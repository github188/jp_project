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
#include <iostream>

#include "standardoutputstream.h"
#include "../util/log.h"

using namespace library;

StandardOutputStream::StandardOutputStream()
{
}

StandardOutputStream::~StandardOutputStream()
{
}

void StandardOutputStream::flush()
{
	std::cout.flush();
}

void StandardOutputStream::close()
{
	std::cout.flush();
}

int32 StandardOutputStream::doWriteByteArray(const uint8 *buffer, int32 size, int32 offset, int32 length, bool bSyncWriting)
{
	if (buffer == NULL) {
		
		Log(LOG_ERROR, "StandardOutputStream buffer is null.");
		return 0;
	}

	if (size <= 0 || length <= 0 || offset < 0) {
		
		Log(LOG_ERROR, "StandardOutputStream input parameter is InValid: size: %d, length: %d", size, length);
		return 0;
	}

	if (length - offset > size) {
		
		Log(LOG_ERROR, "StandardOutputStream write too much data: %d", length - offset);
		return 0;
	}

	for(int32 i = 0; i < length; ++i) {
        std::cout << buffer[i+offset];
    }

	return length;
}