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
#ifndef __LIBRARY_CONCURRENT_POOLTHREAD_H__
#define __LIBRARY_CONCURRENT_POOLTHREAD_H__

#include <string>

#include "../common/synchronize.h"
#include "../common/runnable.h"
#include "lock.h"

namespace library {

class ThreadPool;
class Thread;
class PoolThread : public Synchronize, public Runnable
{
	public:

		PoolThread(ThreadPool *pool, const char *name = "POOLT");

		PoolThread(ThreadPool *pool, std::string &name);

		virtual ~PoolThread();

	public:

		/* true for idle */
		bool isIdle() const;

		/* start the thread */
		void start();

		/* exit the thread */
		void close();

		/**
		 * set a new task to thread and wake up if it is wait.
		 *
		 * @return true for setting ok, flase for thread has already setted.
		 */
		bool setTask(Runnable *r);

	public:

		virtual void lock() {threadlock.lock();};

		virtual bool trylock() {return threadlock.trylock();};

		virtual void unlock() {threadlock.unlock();};

		virtual void wait() {threadlock.wait();};

		virtual bool wait(int64 mills, int32 nases) {
			return threadlock.wait(mills, nases);
		};

		virtual void notify() {threadlock.notify();};

		virtual void notifyall() {threadlock.notifyall();};

		virtual void run();

	private:

		ThreadPool *threadpool;
		Thread *thread;
		Runnable *task;
		Lock threadlock;
		bool go; //make the thread exit deleting the pool.
};

}

#endif /* end of file */