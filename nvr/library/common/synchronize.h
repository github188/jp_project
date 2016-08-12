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
#ifndef __LIBRARY_SYNCHRONIZE_H__
#define __LIBRARY_SYNCHRONIZE_H__

#include "../../common/basetype.h"

namespace library {

class Synchronize
{
	public:
		
		virtual ~Synchronize() {};

	protected:
		
		Synchronize() {};

	public:
		/**
		 * Enter a critical section.
		 */
		virtual void lock() = 0;

		/**
		 * Try to enter a critical section.
		 *
		 * @return
		 *    TRUE if the attempt was successful, FALSE otherwise.
		 */
		virtual bool trylock() = 0;

		/**
		 * Try to enter a critical section.
		 *
		 * @return
		 *    TRUE if the attempt was successful, FALSE otherwise.
		 */
		virtual bool trylock(struct timeval stTimeout) {return false;};

		/**
		 * Leave a critical section.
		 */
		virtual void unlock() = 0;

		/**
		 * Causes current thread to wait until another thread 
		 * invokes the notify() method or the notifyAll() 
		 * method for this object.
		 */
		virtual void wait() = 0;

		/**
		 * Causes current thread to wait until either another 
		 * thread invokes the notify() method or the notifyAll() 
		 * method for this object, or a specified amount of time 
		 * has elapsed.
		 *
		 * @param mills 
		 *    milliseconds part of timeout.
		 * @param nases
		 *    naseconds part of timeout.
		 * @param
		 *    return TRUE if timeout occured, FALSE otherwise.
		 */
		virtual bool wait(int64 mills, int32 nases) = 0;

		/**
		 * Wakes up a single thread that is waiting on this 
		 * object's monitor.
		 */
		virtual void notify() = 0;
		
		/**
		 * Wakes up all threads that are waiting on this object's 
		 * monitor.
		 */
		virtual void notifyall() = 0;
};

}

#endif /* end of file */