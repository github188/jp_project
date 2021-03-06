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
#include "../common/exceptiondefines.h"
#include "../exception/sysexception.h"
#include "platform_lin.h"
#include "mutex.h"

using namespace library;
using namespace platform;

Mutex::Mutex()
{
	try {
		PlatformAPI::createmutex(&monitor);
		PlatformAPI::createcondition(&cond);
	}
	EXCEPTION_CATCH_RETHROW(SysOsException)
}

Mutex::~Mutex()
{
	PlatformAPI::destroymutex(monitor);
	PlatformAPI::destroycondition(cond);
}

void Mutex::lock()
{
	try {
		PlatformAPI::lockmutex(monitor);
	}
	EXCEPTION_CATCH_RETHROW(SysOsException)
}

bool Mutex::trylock()
{
	bool blocked = FALSE;
	
	try {
		blocked = PlatformAPI::trylockmutex(monitor);
	}
	EXCEPTION_CATCH_RETHROW(SysOsException)

	return blocked;
}

bool Mutex::trylock(struct timeval stTimeout)
{
	bool blocked = FALSE;
	
	try {
		blocked = PlatformAPI::trylockmutex(monitor, stTimeout);
	}
	EXCEPTION_CATCH_RETHROW(SysOsException)

	return blocked;
}

void Mutex::unlock()
{
	try {
		PlatformAPI::unlockmutex(monitor);
	}
	EXCEPTION_CATCH_RETHROW(SysOsException)
}

void Mutex::wait()
{
	PlatformAPI::waitoncondition(cond, monitor);
}

bool Mutex::wait(int64 mills, int32 nases)
{
	bool timed = FALSE;
	
	timed = PlatformAPI::waitoncondition(cond, monitor, mills, nases);

	return timed;
}

void Mutex::notify()
{
	PlatformAPI::notify(cond);
}

void Mutex::notifyall()
{
	PlatformAPI::notifyall(cond);
}
