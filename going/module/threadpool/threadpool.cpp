/***************************************************************************
 *   Copyright (C) 2016 by going
 *	 Created on: 2016年2月23日
 *	 Author: going   				   						   *
 *   going369@qq.com   							   					   *
 *   threadpool.cpp                                                                      *
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

#include <stdlib.h>
#include "threadpool.h"
using namespace std;


//初始化类的静态成员必须加上类型和作用域，static数据成员必须在类定义体的外部定义，不像不同数据成员可以用构造函数初始化
//应该在定义时进行初始化，注意是定义，这个定义应该放在包含类的非内联成员函数定义的文件中。
//注：静态成员函数只能使用静态变量，非静态没有限制，静态变量必须在外部定义和初始化，没初始化就为默认数值
pthread_mutex_t ThreadPool::mutexSync = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t ThreadPool::mutexWorkCompletion = PTHREAD_MUTEX_INITIALIZER;


ThreadPool::ThreadPool()
{
	ThreadPool(2);
}

ThreadPool::ThreadPool(int maxThreads)
{
   if (maxThreads < 1)
       maxThreads=1;

   pthread_mutex_lock(&mutexSync);
   this->maxThreads = maxThreads;
   this->queueSize = maxThreads;
   workerQueue.resize(maxThreads, NULL);//调整容器大小，然后用默认构造函数初始化新的空间
   topIndex = 0;
   bottomIndex = 0;
   incompleteWork = 0;
   sem_init(&availableWork, 0, 0); //工作队列信号量，表示已经加入队列的工作，初始时没有工作
   sem_init(&availableThreads, 0, queueSize);  //空闲线程信号量，一开始就有quisize个线程可以使用
   pthread_mutex_unlock(&mutexSync);
}

//调用pthread_create()让线程跑起来，threadExecute是类的静态函数，因为pthread_create()第三个参数必须为静态函数
void ThreadPool::initializeThreads()
{
   for(int i = 0; i<maxThreads; ++i)
	{
		pthread_t tempThread;
		pthread_create(&tempThread, NULL, ThreadPool::threadExecute, (void*)this );
   }
}

ThreadPool::~ThreadPool()
{
	//因为对于vector，clear并不真正释放内存(这是为优化效率所做的事)，clear实际所做的是为vector中所保存的所有对象调用析构函数(如果有的话),
	//然后初始化size这些东西，让你觉得把所有的对象清除了。。。
    //真正释放内存是在vector的析构函数里进行的，所以一旦超出vector的作用域（如函数返回)，首先它所保存的所有对象会被析构，
	//然后会调用allocator中的deallocate函数回收对象本身的内存。。。
    workerQueue.clear();
}


void ThreadPool::destroyPool(int maxPollSecs = 2)
{
	while(incompleteWork>0 )
	{
	    //cout << "Work is still incomplete=" << incompleteWork << endl;
		sleep(maxPollSecs);
	}
	cout << "All Done!! Wow! That was a lot of work!" << endl;
	sem_destroy(&availableWork);
	sem_destroy(&availableThreads);
    pthread_mutex_destroy(&mutexSync);
    pthread_mutex_destroy(&mutexWorkCompletion);

}

//分配人物到top，然后通知有任务需要执行。
bool ThreadPool::assignWork(WorkerThread *workerThread)
{
    pthread_mutex_lock(&mutexWorkCompletion);
    incompleteWork++;
		//cout << "assignWork...incomapleteWork=" << incompleteWork << endl;
	pthread_mutex_unlock(&mutexWorkCompletion);
	sem_wait(&availableThreads);
	pthread_mutex_lock(&mutexSync);
    //workerVec[topIndex] = workerThread;
    workerQueue[topIndex] = workerThread;
    //cout << "Assigning Worker[" << workerThread->id << "] Address:[" << workerThread << "] to Queue index [" << topIndex << "]" << endl;
	if(queueSize !=1 )
		topIndex = (topIndex+1) % (queueSize-1);
    sem_post(&availableWork);
	pthread_mutex_unlock(&mutexSync);
	return true;
}

//当已经有人物放到队列里面后，就会受到通知，然后从底部拿走工作，在workerArg中返回
bool ThreadPool::fetchWork(WorkerThread **workerArg)
{
	sem_wait(&availableWork);

	pthread_mutex_lock(&mutexSync);
	WorkerThread * workerThread = workerQueue[bottomIndex];
    workerQueue[bottomIndex] = NULL;
	*workerArg = workerThread;
	if(queueSize !=1 )
		bottomIndex = (bottomIndex+1) % (queueSize-1);
	sem_post(&availableThreads);
	pthread_mutex_unlock(&mutexSync);
    return true;
}

//每个线程运行的静态函数实体，executeThis 方法将会被继承累从写，之后实现具体线程的工作。
void *ThreadPool::threadExecute(void *param)
{
	WorkerThread *worker = NULL;
	while(((ThreadPool *)param)->fetchWork(&worker))
	{
		if(worker)
        {
			worker->executeThis();
            //cout << "worker[" << worker->id << "]\tdelete address: [" << worker << "]" << endl;
            delete worker;
            worker = NULL;
        }

		pthread_mutex_lock( &(((ThreadPool *)param)->mutexWorkCompletion) );
        //cout << "Thread " << pthread_self() << " has completed a Job !" << endl;
	 	((ThreadPool *)param)->incompleteWork--;
		pthread_mutex_unlock( &(((ThreadPool *)param)->mutexWorkCompletion) );
	}
	return 0;
}




