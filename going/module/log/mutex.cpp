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

#include "platform_lin.h"
#include "mutex.h"

using namespace library;
using namespace platform;

Mutex::Mutex()
{

		PlatformAPI::createmutex(&monitor);

}

Mutex::~Mutex()
{
	PlatformAPI::destroymutex(monitor);

}

void Mutex::lock()
{

		PlatformAPI::lockmutex(monitor);

}

bool Mutex::trylock()
{
	bool blocked = FALSE;
	

		blocked = PlatformAPI::trylockmutex(monitor);

	return blocked;
}

bool Mutex::trylock(struct timeval stTimeout)
{
	bool blocked = FALSE;
	

		blocked = PlatformAPI::trylockmutex(monitor, stTimeout);


	return blocked;
}

void Mutex::unlock()
{

		PlatformAPI::unlockmutex(monitor);

}


