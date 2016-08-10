/***************************************************************************
 *   Copyright (C) 2016 by going
 *	 Created on: 2016年2月23日
 *	 Author: going   				   						   *
 *   going369@qq.com   							   					   *
 *   threadpool.h                                                                      *
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


#ifndef THREADPOOL_THREADPOOL_H_
#define THREADPOOL_THREADPOOL_H_


#include <pthread.h>
#include <semaphore.h>
#include <iostream>
#include <vector>

using namespace std;
/*
WorkerThread class
This class needs to be sobclassed by the user.
*/
class WorkerThread{
public:

    unsigned virtual executeThis()
	{
		return 0;
	}

    WorkerThread(){}
    virtual ~WorkerThread(){}
};

/*
ThreadPool class manages all the ThreadPool related activities. This includes keeping track of idle threads and synchronizations between all threads.
*/
class ThreadPool{
public:
    ThreadPool();
    ThreadPool(int maxThreadsTemp);
    virtual ~ThreadPool();

	void destroyPool(int maxPollSecs);

    bool assignWork(WorkerThread *worker);
    bool fetchWork(WorkerThread **worker);

	void initializeThreads();

    static void *threadExecute(void *param); // pthread_create()调用的函数必须为静态的
    static pthread_mutex_t mutexSync;
    static pthread_mutex_t mutexWorkCompletion;//工作完成个数互斥量


private:
    int maxThreads;

    pthread_cond_t  condCrit;
    sem_t availableWork;
    sem_t availableThreads;

    vector<WorkerThread *> workerQueue;

    int topIndex;
    int bottomIndex;
	int incompleteWork;
    int queueSize;
};







#endif /* THREADPOOL_THREADPOOL_H_ */
