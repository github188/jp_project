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
#include "queuedthreadpool.h"
#include "../util/log.h"
#include "reentrant.h"

using namespace library;

QueuedThreadPool::QueuedThreadPool(std::string &poolName, uint32 poolNum, std::string &poolThreadname) :
				Thread(poolName), ThreadPool(poolNum, poolThreadname), queue(), threadlock(), go(TRUE)
{
	queue.clearAll();
	Log(LOG_DEBUG, "QueuedThreadPool: %s  start.", poolName.c_str());
}

QueuedThreadPool::QueuedThreadPool(std::string &poolName, uint32 poolNum) : Thread(poolName),
				ThreadPool(poolNum), queue(), threadlock(), go(TRUE)
{
	queue.clearAll();
	Log(LOG_DEBUG, "QueuedThreadPool: %s  start.", poolName.c_str());
}

QueuedThreadPool::~QueuedThreadPool() {
	
	lock();
	queue.clearAll();
	go = FALSE;
	notify();
	unlock();

	poolexit();

	/**
	 * wait for QueuedThreadPool thread exit before all attribute destroys
	 * 
	 * timewait = 150 because every poolthread will wait 100 millsecs a loop
	 */
	Thread::sleep(150);
	
	Log(LOG_DEBUG, "QueuedThreadPool stoped.");
}

void QueuedThreadPool::excute(Runnable *r)
{
	/**
	 * here add a lock for:
	 * this method is called by user, while QueuedThreadPool is running, 
	 * so, to avoid the resource competition, there add a lock to block it.
	 */
	lock();

	assign(r);

	unlock();
}

void QueuedThreadPool::idleNotification()
{
	lock();
	notify();
	unlock();
	ThreadPool::idleNotification();
}

void QueuedThreadPool::run()
{
	/* lock for the same purpose as method excute. */
	lock();
	while (go)
	{
		Runnable *r = queue.removeFirst();
		if (r) {
			assign(r);
		}
		
		//pool will be waiting a while if there exist no queued task.
		wait(100, 0);
	}
	unlock();
}

void QueuedThreadPool::assign(Runnable *r)
{
	if (r == NULL) return;
	
	ListCursor<PoolThread> cursor(&taskList);
	PoolThread *thread = NULL;
	for (; cursor.get(); cursor.next())
	{
		thread = cursor.get();
		if (thread && thread->isIdle()) {
			
			Log(LOG_DEBUG, "get a idle poolthread.");
			if (thread->setTask(r)) {

				break;
			} else {
				thread = NULL;
			}
		} else {

			thread = NULL;
		}
	}

	if (thread == NULL)
	{
		queue.add(r);
		notify();
	}
}