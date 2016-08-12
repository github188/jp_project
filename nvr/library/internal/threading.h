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
#ifndef __LIBRARY_INTERNAL_THREADING_H__
#define __LIBRARY_INTERNAL_THREADING_H__

#include <vector>
#include <string>

#include "../../common/basetype.h"
#include "../common/threadhandle.h"
#include "platformdefs.h"
#include "ThreadList.h"

using namespace platform;
namespace library {

class Threading
{
	private:
		
		Threading() {};

		Threading(const Threading& src) {};

		Threading& operator=(const Threading& src);
		
	public:

		virtual ~Threading();

		static void initialize();

		static void shutdown();

	public:
		
		static ST_ThreadHandle* createnewthread(std::string &name, int32 priority, int64 stacksize, void *threadarg);

		static void destroythread(ST_ThreadHandle *thread);

		static void detachthread(ST_ThreadHandle *hanlde);

		static void jointhread(ST_ThreadHandle *hanlde);

		static m_thread_t getcurrentthread(ST_ThreadHandle *thread);

		static uint64 getcurrentthreadid(ST_ThreadHandle *thread);

		static int32 getPriority(ST_ThreadHandle *thread);

		static void setPriority(ST_ThreadHandle *thread, int32 priority);

		static int64 getstacksize(ST_ThreadHandle *thread);

		static void setstacksize(ST_ThreadHandle *thread, int64 stacksize);

		static int32 getthreadstate(ST_ThreadHandle *thread);

		static std::string getthreadname(ST_ThreadHandle *thread);

		static std::string getcurrentthreadname(uint64 id);

		static void setthreadname(ST_ThreadHandle *thread, const std::string &name);

		static bool isalive(ST_ThreadHandle *thread);

		static void start(ST_ThreadHandle *thread);

        static void yeild();

	private:

		/** list for save threads */
		static ThreadList threadlist;
		
		/** priority map */
		static std::vector<int> priorityMapping;
};

}

#endif /* end of file */