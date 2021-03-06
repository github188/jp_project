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
#ifndef __LIBRARY_UTIL_QUEUE_H__
#define __LIBRARY_UTIL_QUEUE_H__

namespace library {

template <typename T> class Queue
{
	private:

		Queue(const Queue&);
		Queue& operator= (const Queue&);

	public:

		Queue() {};

		//! Destroy a Queue
		virtual ~Queue() {};

		/**
	     * Add an object to this Queue.
	     *
	     * @param item value to be added to the Queue
	     * 
	     * @post If no exception is thrown, a copy of <i>item</i> will have been added to the Queue.
	     */
	    virtual void add(const T& item) = 0;

		/**
	     * Add an object to this Queue.
	     *
	     * @param item value to be added to the Queue
	     * @param timeout maximum amount of time (milliseconds) this method may block
	     *        the calling thread.
	     *
	     * @return 
	     *   - <em>true</em> if a copy of <i>item</i> can be added before <i>timeout</i> 
	     *     milliseconds elapse.
	     *   - <em>false</em> otherwise.
	     *
	     * @post If this function returns true a copy of <i>item</i> will have been added to the Queue.
	     */
	    virtual bool add(const T& item, unsigned long timeout) = 0;

		/**
	     * Retrieve and remove a value from this Queue.
	     *
	     * @return <em>T</em> next available value
	     * 
	     * @post The value returned will have been removed from the Queue.
	     */
	    virtual T next() = 0;

		/**
	     * Retrieve and remove a value from this Queue.
	     *
	     * @param timeout maximum amount of time (milliseconds) this method may block
	     *        the calling thread.
	     *
	     * @return <em>T</em> next available value
	     * 
	     * @post The value returned will have been removed from the Queue.
	     */
	    virtual T next(unsigned long timeout) = 0;

		/**
	     * Count the values present in this Queue. 
	     *
	     * @return <em>size_t</em> number of elements available in the Queue. 
	     */
	    virtual size_t size() = 0;

		/**
	     * Count the values present in this Queue. 
	     *
	     * @param timeout maximum amount of time (milliseconds) this method may block
	     *        the calling thread.
	     *
	     * @return <em>size_t</em> number of elements available in the Queue. 
	     */
	    virtual size_t size(unsigned long timeout) = 0;

		/**
	     * Test whether any values are available in this Queue.
	     *
	     * @return 
	     *  - <em>true</em> if there are no values available.
	     *  - <em>false</em> if there <i>are</i> values available.
	     */
	    virtual bool empty() {

	        return size() == 0;

	    }

		/**
	     * Test whether any values are available in this Queue.
	     *
	     * @param timeout maximum amount of time (milliseconds) this method may block
	     *        the calling thread.
	     *
	     * @return 
	     *  - <em>true</em> if there are no values available.
	     *  - <em>false</em> if there <i>are</i> values available.
	     */
	    virtual bool empty(unsigned long timeout) {

	        return size(timeout) == 0;
			
	    }
};

}

#endif /* end of file */