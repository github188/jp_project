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
#include "mutex.h"
#include "ThreadList.h"

using namespace platform;
using namespace library;

ThreadList::ThreadList() : _list(), mutex(NULL)
{
	mutex = new Mutex();
}

ThreadList::~ThreadList()
{
	if (mutex) delete mutex;
	mutex = NULL;
}

void ThreadList::add(ST_ThreadHandle * handle)
{
	mutex->lock();

	_list.add(handle);

	mutex->unlock();
};

void ThreadList::remove(ST_ThreadHandle *handle)
{
	ST_ThreadHandle *thread = NULL;
	
	mutex->lock();

	thread = _list.remove(handle);
	
	mutex->unlock();
}

void ThreadList::getListRef(ListCursor<ST_ThreadHandle> &cusor)
{
	cusor.init(&_list);
}

int32 ThreadList::size() const
{
	return _list.size();
}
