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
#include "../exception/nullpointerexception.h"
#include "../common/exceptiondefines.h"
#include "../concurrent/thread.h"
#include "../common/exception.h"
#include "../util/logmanager.h"
#include "platform_lin.h"
#include "threading.h"
#include <memory>

#include "../util/log.h"

using namespace library;
using namespace platform;

static void threadexit(ST_ThreadHandle *self, bool destroy = FALSE)
{
	PlatformAPI::lockmutex(self->mutex);

	self->state = Thread::FINISHED;
	m_thread_t handle = self->threadhandle;
	PlatformAPI::notifyall(self->cond);

	PlatformAPI::unlockmutex(self->mutex);

	if (destroy == TRUE)
	{
		delete self;
	}
	Log(LOG_DEBUG, "%p thread exit.", self);
	LogManagerMgr::getInstance()->shutdownSelf();
	PlatformAPI::detachthread(handle);
	PlatformAPI::exitthread();
}

static void* threadStarter(void* t)
{
	Thread *thread = (Thread*)t;

	thread->run();

	return t;
}

static void* threadEntryMethod(void* arg)
{
	ST_ThreadHandle *thread = (ST_ThreadHandle *)arg;

	try {
		
		PlatformAPI::lockmutex(thread->mutex);
		if (thread->bsuspend == TRUE)
		{
			PlatformAPI::waitoncondition(thread->cond, thread->mutex);
		}
		PlatformAPI::unlockmutex(thread->mutex);
//		Log(LOG_DEBUG, "thread run: %llu, %p", pthread_self(), thread);
	} 
	EXCEPTION_CATCH_NOTHROW (Exception)
	
	if (thread->canceled == TRUE)
	{
		threadexit(thread, thread->osthread);
		return NULL;
	}
	
	thread->state = Thread::RUNNING;

	thread->threadMain(thread->thread);

	threadexit(thread, thread->osthread);
	return NULL;
}

static void createthreadinstance(ST_ThreadHandle* thread, long long stackSize, int priority,
                              bool suspended, threadingTask threadMain, void* threadArg)
{
	thread->threadMain = threadMain;
	thread->bsuspend = suspended;
	
	PlatformAPI::createnewthread(&thread->threadhandle, threadEntryMethod, thread, 
								 priority, stackSize, &thread->threadId);
}

static void initthreadhandle(ST_ThreadHandle *thread)
{
	if (thread == NULL) return;

	thread->state = Thread::IDLE;
	thread->stacksize = -1;
	thread->priority = Thread::NORM_PRIORITY;
	thread->bsuspend = TRUE;
	thread->canceled = FALSE;
	thread->osthread = FALSE;
	thread->threadMain = NULL;
	PlatformAPI::createmutex(&thread->mutex);
	PlatformAPI::createcondition(&thread->cond);
}

////////////////////////////////////////////////////////////////////////////////
ThreadList Threading::threadlist;
std::vector<int> Threading::priorityMapping;

////////////////////////////////////////////////////////////////////////////////
Threading::~Threading()
{
}

void Threading::initialize()
{
	/* init logmanager instance */
	LogManagerMgr::getInstance();
	priorityMapping.clear();

	// Figure out what the OS level thread priority mappings are for the Thread
    // classes generic priority value range.
    PlatformAPI::initprioritymapping(Thread::MAX_PRIORITY + 1, priorityMapping);
}

void Threading::shutdown()
{
	ListCursor<ST_ThreadHandle> cursor;

	threadlist.getListRef(cursor);
	for (; cursor.get(); cursor.next())
	{
		ST_ThreadHandle *thread = NULL;

		thread = cursor.get();
		destroythread(thread);
	}
	threadlist.clear();
}

ST_ThreadHandle* Threading::createnewthread(std::string &name, int32 priority, int64 stacksize, void *threadarg)
{
	ST_ThreadHandle *thread = NULL;
	
	if (threadarg == NULL) {
        throw NullPointerException(__FILE__, __LINE__, "One or more arguments was NULL");
    }
	
	std::auto_ptr<ST_ThreadHandle> handle(new ST_ThreadHandle());
	initthreadhandle(handle.get());
	thread = handle.release();
	
	thread->threadname = name;
	thread->stacksize = stacksize;
	thread->priority = priority;
	thread->thread = (Thread *)threadarg;

	/* add to list */
	threadlist.add(thread);

	if (priority < Thread::MIN_PRIORITY || priority > Thread::MAX_PRIORITY) {
		throw NullPointerException(__FILE__, __LINE__, "thread priority error");
	}

	createthreadinstance(thread, stacksize, priorityMapping[priority], TRUE, threadStarter, threadarg);

	return thread;
}

void Threading::destroythread(ST_ThreadHandle *thread)
{
	if (thread == NULL) return;

	if (!thread->osthread)
	{
		// If the thread was created but never started then we need to wake it
		// up from the suspended state so that it can terminate, we mark it
		// canceled to ensure it doesn't call its runnable.
		if (thread->state == Thread::IDLE)
		{
			PlatformAPI::lockmutex(thread->mutex);
			if (thread->state == Thread::IDLE && thread->bsuspend == TRUE)
			{
				thread->canceled = TRUE;
				thread->bsuspend = FALSE;
				PlatformAPI::notifyall(thread->cond);
			}
			PlatformAPI::unlockmutex(thread->mutex);
		}

    	Threading::jointhread(thread);
		delete thread;
//		Log(LOG_DEBUG, "1  thread = %p", thread);
	}
	else
	{
		Threading::detachthread(thread);
	}

	/* delete thread handle and remove from list */
//	Log(LOG_DEBUG, "2  thread = %p", thread);
	threadlist.remove(thread);
//	Log(LOG_DEBUG, "3  thread = %p", thread);
}

void Threading::detachthread(ST_ThreadHandle *thread)
{
	PlatformAPI::lockmutex(thread->mutex);
	thread->osthread = TRUE;
	PlatformAPI::unlockmutex(thread->mutex);

	PlatformAPI::detachthread(thread->threadhandle);
}

void Threading::jointhread(ST_ThreadHandle *thread)
{
	PlatformAPI::jointhread(thread->threadhandle);
}

m_thread_t Threading::getcurrentthread(ST_ThreadHandle *thread)
{
	return thread->threadhandle;
}

uint64 Threading::getcurrentthreadid(ST_ThreadHandle *thread)
{
	return thread->threadId;
}

int32 Threading::getPriority(ST_ThreadHandle *thread)
{
	return thread->priority;
}

void Threading::setPriority(ST_ThreadHandle *thread, int32 priority)
{
	PlatformAPI::setPriority(thread->threadhandle, priorityMapping[priority]);
	thread->priority = priority;
}

int64 Threading::getstacksize(ST_ThreadHandle *thread)
{
	return thread->stacksize;
}

void Threading::setstacksize(ST_ThreadHandle *thread, int64 stacksize)
{
	PlatformAPI::setstacksize(thread->threadhandle, stacksize);
	thread->stacksize = stacksize;
}

int32 Threading::getthreadstate(ST_ThreadHandle *thread)
{
	return (Thread::State) thread->state;
}

std::string Threading::getthreadname(ST_ThreadHandle *thread)
{
	return thread->threadname;
}

std::string Threading::getcurrentthreadname(uint64 id)
{
	ListCursor<ST_ThreadHandle> cursor;
	ST_ThreadHandle *handle = NULL;
	
	threadlist.getListRef(cursor);
	for (; cursor.get(); cursor.next())
	{
		handle = cursor.get();
		if (handle && handle->threadId == id)
			break;

		handle = NULL;
	}

	if (handle) return handle->threadname;
	return "";
}

void Threading::setthreadname(ST_ThreadHandle *thread, const std::string &name)
{
	thread->threadname = name;
}

bool Threading::isalive(ST_ThreadHandle *thread)
{
	return thread->state == Thread::RUNNING;
}

void Threading::start(ST_ThreadHandle *thread)
{
	try {
		
		if (thread == NULL) return;

		if (thread->state > Thread::IDLE) return;

		PlatformAPI::lockmutex(thread->mutex);

		thread->state = Thread::RUNNING;

		if (thread->bsuspend == TRUE)
		{
			thread->bsuspend = FALSE;
			PlatformAPI::notifyall(thread->cond);
		}

		PlatformAPI::unlockmutex(thread->mutex);

	}
	EXCEPTION_CATCH_NOTHROW (Exception)
}

void Threading::yeild()
{
	PlatformAPI::yeild();
}

