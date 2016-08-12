/***************************************************************************
 *   Copyright (C) 2015 by root   				   						   *
 *   ysgen0217@163.com   							   					   *
 *                                                                         *
 *   \file circularbuffer.cpp from libsockets-2.2.9						   *
 *	 \date  2004-02-13													   *
 *	 \author grymse@alhem.net											   *
 *
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
#ifndef __LIBRARY_BUFFER_CIRCULARBUFFER_H__
#define __LIBRARY_BUFFER_CIRCULARBUFFER_H__

#include <string>
#include <stdlib.h>

namespace library {

/* inherit from libSockets*/
class CircularBuffer
{
	private:

		CircularBuffer(const CircularBuffer&) {};

		CircularBuffer& operator= (const CircularBuffer&) {return *this;};

	public:

		CircularBuffer(size_t capacity);

		~CircularBuffer();

	public:

		/** append l bytes from p to buffer */
		bool write(const char* p, size_t l);

		/** copy l bytes from buffer to dest */
		bool read(char *dest, size_t l);

		/** skip l bytes from buffer */
		bool remove(size_t l);

		/* copy l bytes from buffer with not change any buffer's property */
		bool peek(char *dest, size_t l);

		/** read l bytes from buffer, returns as string. */
		std::string readString(size_t l);

		/** total buffer length */
		size_t getLength();
		
		/** pointer to circular buffer beginning */
		const char *getStart();
		
		/** return number of bytes from circular buffer beginning to buffer physical end */
		size_t getL();
		
		/** return free space in buffer, number of bytes until buffer overrun */
		size_t space();

		/** return total number of bytes written to this buffer, ever */
		unsigned long long byteCounter(bool clear = false);

	private:

		char *buf;
		size_t m_max; //capacity
		size_t m_q; // actual data size stored in the buffer
		size_t m_b; // head
		size_t m_t; // tail
		unsigned long long m_count; //total bytes count which has been stored.
};

}

#endif /* end of file */