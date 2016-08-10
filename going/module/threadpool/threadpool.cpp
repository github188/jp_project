/***************************************************************************
 *   Copyright (C) 2016 by going
 *	 Created on: 2016��2��23��
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


//��ʼ����ľ�̬��Ա����������ͺ�������static���ݳ�Ա�������ඨ������ⲿ���壬����ͬ���ݳ�Ա�����ù��캯����ʼ��
//Ӧ���ڶ���ʱ���г�ʼ����ע���Ƕ��壬�������Ӧ�÷��ڰ�����ķ�������Ա����������ļ��С�
//ע����̬��Ա����ֻ��ʹ�þ�̬�������Ǿ�̬û�����ƣ���̬�����������ⲿ����ͳ�ʼ����û��ʼ����ΪĬ����ֵ
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
   workerQueue.resize(maxThreads, NULL);//����������С��Ȼ����Ĭ�Ϲ��캯����ʼ���µĿռ�
   topIndex = 0;
   bottomIndex = 0;
   incompleteWork = 0;
   sem_init(&availableWork, 0, 0); //���������ź�������ʾ�Ѿ�������еĹ�������ʼʱû�й���
   sem_init(&availableThreads, 0, queueSize);  //�����߳��ź�����һ��ʼ����quisize���߳̿���ʹ��
   pthread_mutex_unlock(&mutexSync);
}

//����pthread_create()���߳���������threadExecute����ľ�̬��������Ϊpthread_create()��������������Ϊ��̬����
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
	//��Ϊ����vector��clear���������ͷ��ڴ�(����Ϊ�Ż�Ч����������)��clearʵ����������Ϊvector������������ж��������������(����еĻ�),
	//Ȼ���ʼ��size��Щ������������ð����еĶ�������ˡ�����
    //�����ͷ��ڴ�����vector��������������еģ�����һ������vector���������纯������)������������������ж���ᱻ������
	//Ȼ������allocator�е�deallocate�������ն�������ڴ档����
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

//�������ﵽtop��Ȼ��֪ͨ��������Ҫִ�С�
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

//���Ѿ�������ŵ���������󣬾ͻ��ܵ�֪ͨ��Ȼ��ӵײ����߹�������workerArg�з���
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

//ÿ���߳����еľ�̬����ʵ�壬executeThis �������ᱻ�̳��۴�д��֮��ʵ�־����̵߳Ĺ�����
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




