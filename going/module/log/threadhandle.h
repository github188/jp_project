/***************************************************************************
 *   Copyright (C) 2015 by root   				   *
 *   ysgen0217@163.com   							   *
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
#ifndef __LIBRARY_COMMON_THREADHANDLE_H__
#define __LIBRARY_COMMON_THREADHANDLE_H__

#include "platformdefs.h"

namespace library {

class Thread;

typedef void* (*threadingTask)(void *);
typedef struct ST_ThreadHandle
{
	platform::m_thread_t threadhandle;
	std::string threadname;
	uint64 stacksize;
	Thread* thread;
	uint64 threadId;
	int32 priority;
	int32 state;

	/** indicate whether this thread can be suspended */
	bool bsuspend;
	bool canceled;
	bool osthread;

	threadingTask threadMain;
	platform::m_condition_t cond;
	platform::m_mutex_t mutex;
	
} ST_ThreadHandle;

}

#endif /* end of file */
