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
#include "threadpool.h"
#include "reentrant.h"

using namespace library;

ThreadPool::ThreadPool(uint32 poolNum) : taskList()
{
	for (uint32 i = 0; i < poolNum; ++i) {
		taskList.add(new PoolThread(this));
	}
}

ThreadPool::ThreadPool(uint32 poolNum, std::string &name) : taskList()
{
	for (uint32 i = 0; i < poolNum; ++i) {
		taskList.add(new PoolThread(this, name));
	}
}

ThreadPool::~ThreadPool() {
	
	taskList.clearAll();
}

/* task will be waiting for running unless it gets a idle thread. */
void ThreadPool::excute(Runnable *r)
{
	if (r == NULL) return;

	lock();

	PoolThread *thread = NULL;
	while (thread == NULL)
	{
		ListCursor<PoolThread> cursor(&taskList);
		for (; cursor.get(); cursor.next())
		{
			thread = cursor.get();
			if (thread->isIdle()) {
								
				/**
				 * unlock it for other user may excute the runnable task.
				 * the pool may be excute tasks unsequenced even if task
				 * was setted before.
				 */
				unlock();
				if (thread->setTask(r)) {
					// task was assigned ok.
					return;
				} else {
					// task could not be assigned, continue to find a idle thread
					thread = NULL;
					lock();
				}
			}
			thread = NULL;
		}

		//threads are all busy, wait a while for excuting.
		if (!thread) {
			wait(1, 0);
		}
	}
	
	unlock();
}

void ThreadPool::poolexit()
{
	taskList.clearAll();
}