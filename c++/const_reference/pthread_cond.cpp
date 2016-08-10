/***************************************************************************
 *   Copyright (C) 2016 by going
 *   Created on: 2016年4月24日
 *   Author: going                                           *
 *   going369@163.com                                                   *                                                                  *
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
#include<stdio.h>
#include<unistd.h>
#include<pthread.h>

#define SIZE 4

struct product

{

        pthread_mutex_t mutex;

        pthread_cond_t notfull;

        pthread_cond_t notempty;

        int pos;//指向buf中最后一个存放了数据的位置

        int buf[SIZE];//存放数据的缓冲区

};

void init(struct product *t)

{

        pthread_mutex_init(&t->mutex,NULL);//初始化互斥锁

        pthread_cond_init(&t->notfull,NULL);//初始化互斥锁，这时notfull处于unsignal状态

        pthread_cond_init(&t->notempty,NULL); //初始化互斥锁，这时notempty处于unsignal状态

        t->pos =-1;

}

//存入函数

void put(struct product *t,int data)

{



        pthread_mutex_lock(&t->mutex);//枷锁

        if((t->pos+1)>(SIZE-1)) //判断pos位置，当下标越界时挂起，等待未满信号继续

                pthread_cond_wait(&t->notfull,&t->mutex);

        t->pos = t->pos +1;

        t->buf[t->pos] = data;

        printf("Thread %d put a data:%d to pos%d\n",pthread_self(),data,t->pos);

        pthread_cond_signal(&t->notempty);//此处必然处于缓冲区有数据状态，改变取数线程条件变量为signaled

        pthread_mutex_unlock(&t->mutex);//解锁

}

//读取函数

void get(struct product *t)

{

        int rtntemp;

        pthread_mutex_lock(&t->mutex);

        if(t->pos<0) //判断好pos位置，适时挂起

                pthread_cond_wait(&t->notempty,&t->mutex);

        printf("Thread %d get a data:%d from pos%d\n",pthread_self(),t->buf[t->pos],t->pos);

        rtntemp = t->buf[t->pos];

        t->pos = t->pos -1;

        pthread_cond_signal(&t->notfull);

        pthread_mutex_unlock(&t->mutex);

}

struct product pdt;

void *putter(void *)

{

        int n;

        for(n=0;n<8;n++){

                put(&pdt,n);
          //      sleep(1);
        }

}



void *getter(void *)

{

        int n;

        for(n=0;n<8;n++){

                get(&pdt);
           //     sleep(1);
        }

}



int main(void)

{

        pthread_t pt1,pt2,gt1,gt2;

        void *retval;

        init(&pdt);

          //创建4个线程，两个放两个取

        pthread_create(&pt1,NULL,putter,0);

        pthread_create(&pt2,NULL,putter,0);

        pthread_create(&gt1,NULL,getter,0);

        pthread_create(&gt2,NULL,getter,0);



      //主线程等待这四个线程结束

        pthread_join(pt1,&retval);

        pthread_join(pt2,&retval);

        pthread_join(gt1,&retval);

        pthread_join(gt2,&retval);

        return 0;

}




