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
#include <string.h>
#include "circularbuffer.h"

using namespace library;

CircularBuffer::CircularBuffer(size_t capacity) : buf(new char[2 * capacity]),
				m_max(capacity), m_q(0), m_b(0), m_t(0), m_count(0)
{
}

CircularBuffer::~CircularBuffer()
{
	delete []buf;
}

bool CircularBuffer::write(const char *s, size_t l)
{
	if (m_q + l > m_max)
	{
		return false; // overflow
	}
	m_count += (unsigned long long)l;
	if (m_t + l > m_max) // block crosses circular border
	{
		size_t l1 = m_max - m_t; // size left until circular border crossing
		// always copy full block to buffer(buf) + top pointer(m_t)
		// because we have doubled the buffer size for performance reasons
		memcpy(buf + m_t, s, l);
		memcpy(buf, s + l1, l - l1);
		m_t = l - l1;
		m_q += l;
	}
	else
	{
		memcpy(buf + m_t, s, l);
		memcpy(buf + m_max + m_t, s, l);
		m_t += l;
		if (m_t >= m_max)
			m_t -= m_max;
		m_q += l;
	}
	return true;
}

bool CircularBuffer::read(char *s, size_t l)
{
	if (l > m_q)
	{
		return false; // not enough chars
	}
	if (m_b + l > m_max) // block crosses circular border
	{
		size_t l1 = m_max - m_b;
		if (s)
		{
			memcpy(s, buf + m_b, l1);
			memcpy(s + l1, buf, l - l1);
		}
		m_b = l - l1;
		m_q -= l;
	}
	else
	{
		if (s)
		{
			memcpy(s, buf + m_b, l);
		}
		m_b += l;
		if (m_b >= m_max)
			m_b -= m_max;
		m_q -= l;
	}
	if (!m_q)
	{
		m_b = m_t = 0;
	}
	return true;
}

bool CircularBuffer::remove(size_t l)
{
	return read(NULL, l);
}

bool CircularBuffer::peek(char *s, size_t l)
{
	if (l > m_q)
	{
		return false; // not enough chars
	}
	
	if (m_b + l > m_max) // block crosses circular border
	{
		size_t l1 = m_max - m_b;
		if (s)
		{
			memcpy(s, buf + m_b, l1);
			memcpy(s + l1, buf, l - l1);
		}
	}
	else
	{
		if (s)
		{
			memcpy(s, buf + m_b, l);
		}
	}

	return true;
}

size_t CircularBuffer::getLength()
{
	return m_q;
}

const char *CircularBuffer::getStart()
{
	return buf + m_b;
}

size_t CircularBuffer::getL()
{
	return (m_b + m_q > m_max) ? m_max - m_b : m_q;
}

size_t CircularBuffer::space()
{
	return m_max - m_q;
}

unsigned long long CircularBuffer::byteCounter(bool clear)
{
	if (clear)
	{
		unsigned long x = m_count;
		m_count = 0;
		return x;
	}
	return m_count;
}

std::string CircularBuffer::readString(size_t l)
{
	char *sz = new char[l + 1];
	if (!read(sz, l)) // failed, debug printout in Read() method
	{
		delete[] sz;
		return "";
	}
	sz[l] = 0;
	std::string tmp = sz;
	delete[] sz;
	return tmp;
}