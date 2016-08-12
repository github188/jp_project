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
#include "../../common/paradef.h"
#include "sysexception.h"
#include <stdarg.h>

using namespace library;
using namespace platform;

SysOsException::SysOsException() : Exception()
{
}

SysOsException::SysOsException(const Exception &ex)
{
	*(Exception*) this = ex;
}

SysOsException::SysOsException(const SysOsException & ex)
{
	*(Exception*) this = ex;
}

SysOsException::SysOsException(const char * file, const int32 linenumber, int32 codeid) :
			Exception()
{
	char exceptiondes[MAX_BUFFER_SIZE_128] = {0};
	
	::snprintf(exceptiondes, sizeof(exceptiondes) - 1, "exception code: [%d]", codeid);
	setmessage(exceptiondes);
	
	// Set the first mark for this exception.
    setmark(file, linenumber);
}

SysOsException::SysOsException(const char * file, const int32 linenumber, const char * msg,...) :
			Exception()
{
	va_list vargs;
    va_start(vargs, msg);
    buildmessage(msg, vargs);
    va_end(vargs);

    // Set the first mark for this exception.
    setmark(file, linenumber);
}

SysOsException::~SysOsException()throw()
{
}