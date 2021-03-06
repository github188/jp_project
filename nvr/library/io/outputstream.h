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
#ifndef __LIBRARY_IO_OUTPUTSTREAM_H__
#define __LIBRARY_IO_OUTPUTSTREAM_H__

#include "../../common/basetype.h"
#include "../common/synchronize.h"
#include "../concurrent/lock.h"

namespace library {

class OutputStream : public Synchronize
{
	public:

		OutputStream();

		virtual ~OutputStream();

	public:

		virtual int32 write(const uint8 *buffer, int32 size, bool bSyncWriting = true);

		virtual int32 write(const uint8 *buffer, int32 size, int32 offset, int32 length, bool bSyncWriting = true);

		virtual void flush();

		virtual void close();

		virtual bool isclosed();
		
	public:

		virtual void lock() {mutex.lock();};

		virtual bool trylock() {return mutex.trylock();};

		virtual void unlock() {mutex.unlock();};

		virtual void wait() {mutex.wait();};

		virtual bool wait(int64 mills, int32 nases) {return mutex.wait(mills, nases);};

		virtual void notify() {mutex.notify();};

		virtual void notifyall() {mutex.notifyall();};

	protected:

		virtual int32 doWriteByteArray(const uint8 *buffer, int32 size, int32 offset, int32 length, bool bSyncWriting) = 0;

	private:

		Lock mutex;
};

}

#endif /* end of file */