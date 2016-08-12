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
#include "../util/log.h"
#include "databuffer.h"

using namespace library;

DataBuffer::DataBuffer(int32 capacity) : _position(0), _capacity(capacity), _limit(capacity),
				_mark(0), _markSet(false)
{
}

DataBuffer::DataBuffer(const DataBuffer &other) : _position(other._position), _capacity(other._capacity),
				_limit(other._limit), _mark(other._mark), _markSet(other._markSet)
{
}

DataBuffer& DataBuffer::capacity(int32 newCapacity)
{
	if (this->_limit > newCapacity || this->_position > newCapacity) {
		Log(LOG_ERROR, "New Capacity is less than limit/position.");
		return *this;
	}
	
	this->_capacity = newCapacity;
	if(this->_markSet && (this->_mark > newCapacity)) {
        this->_mark = 0;
        this->_markSet = false;
    }

	return *this;
}

DataBuffer& DataBuffer::position(int32 newPosition)
{
	if (newPosition < 0 || newPosition > this->_limit) {
		Log(LOG_ERROR, "New Position is greater than set limit");
		return *this;
	}

	this->_position = newPosition;
	if (this->_markSet && (this->_mark > newPosition)) {
		this->_mark = 0;
		this->_markSet = false;
	}

	return *this;
}

DataBuffer& DataBuffer::limit(int32 newLimit)
{
	if (newLimit < 0 || newLimit > this->capacity()) {
		Log(LOG_ERROR, "new limit is larger than the capacity.");
		return *this;
	}

	this->_limit = newLimit;
	if (this->_position > newLimit) {
		this->_position = newLimit;
	}

	if(this->_markSet && (this->_mark > newLimit)) {
        this->_mark = 0;
        this->_markSet = false;
    }

    return *this;
}

DataBuffer& DataBuffer::mark()
{
	this->_mark = this->_position;
    this->_markSet = true;

    return *this;
}

DataBuffer& DataBuffer::reset()
{
	if (!this->_markSet) {
		Log(LOG_ERROR, "Data buffer has not been Marked.");
	}
	this->_position = this->_mark;

	return *this;
}

DataBuffer& DataBuffer::clear()
{
	this->_position = 0;
    this->_mark = 0;
    this->_markSet = false;
    this->_limit = this->capacity();
    return *this;
}

DataBuffer& DataBuffer::flip()
{
    this->_limit = this->_position;
    this->_position = 0;
    this->_mark = 0;
    this->_markSet = false;
    return *this;
}

DataBuffer& DataBuffer::rewind()
{
    this->_position = 0;
    this->_mark = 0;
    this->_markSet = false;
    return *this;
}
