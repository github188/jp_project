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
#include <sstream>

#include "bytebuffer.h"
#include "../util/log.h"

using namespace library;

ByteBuffer::ByteBuffer(int32 capacity) : DataBuffer(capacity)
{
}

ByteBuffer::~ByteBuffer()
{
}

ByteBuffer& ByteBuffer::get(std::vector<uint8> &buffer)
{
	if (!buffer.empty()) {

		this->get(&buffer[0], (int32)buffer.size(), 0, (int32)buffer.size());
	}

	return *this;
}

ByteBuffer& ByteBuffer::get(uint8 *buffer, int32 size, int32 offset, int32 length)
{
	if (length == 0) {
		return *this;
	}

	if (buffer == NULL) {
		Log(LOG_ERROR, "ByteBuffer::get - Passed Buffer is Null.");
		return *this;
	}

	if (size < 0 || offset < 0 || length < 0 || offset + length > size) {
		Log(LOG_ERROR, "Arguments violate array bounds.");
		return *this;
	}

//	if (length > this->remaining()) {
//		Log(LOG_ERROR, "ByteBuffer::get - Not Enough Data to Fill Request.");
//		return *this;
//	}

	if (length > this->remaining()) {
		Log(LOG_DEBUG, "ByteBuffer::get - less bytes(%d) getted than required(%d).", this->remaining(), length);
	}
	length = length >= this->remaining() ? this->remaining() : length;

	// read length bytes starting from the offset
//    for( int ix = 0; ix < length; ++ix ) {
//        buffer[ix + offset] = this->get();
// 	  }

	this->getRawBytes(buffer + offset, length);

    return *this;
}

ByteBuffer& ByteBuffer::put(ByteBuffer& src)
{
	if (this == &src) {
		return *this;
	}

	if (src.remaining() > this->remaining()) {
		Log(LOG_ERROR, "ByteBuffer::put - Not enough space remaining to put src.");
		return *this;
	}

	while (src.remaining()) {
		this->putChar(src.get());
	}

	return *this;
}

ByteBuffer& ByteBuffer::put(const uint8* buffer, int32 size, int32 offset, int32 length)
{
	if (length == 0) {
		return *this;
	}

	if (buffer == NULL) {
		Log(LOG_ERROR, "ByteBuffer::put - Passed Buffer is Null.");
		return *this;
	}

	if (size < 0 || offset < 0 || length < 0 || offset + length > size) {
		Log(LOG_ERROR, "Arguments violate array bounds.");
		return *this;
	}

	if (length > this->remaining()) {
		Log(LOG_ERROR, "ByteBuffer::put - Not enough space remaining to store requested data.");
		return *this;
	}

//	for (int32 ix = 0; ix < length; ++ix) {
//		this->putChar(buffer[ix + offset]);
//	}
	this->putRawBytes(buffer + offset, length);

	return *this;
}

ByteBuffer& ByteBuffer::put(std::vector<uint8> &buffer)
{
	this->put(&buffer[0], (int32)buffer.size(), 0, (int32)buffer.size());

	return *this;
}

std::string ByteBuffer::toString() const
{
	std::ostringstream stream;
    stream << "ByteBuffer";
    stream << ", status: capacity=" << this->capacity();
    stream << " position=" << this->position();
    stream << " limit=" << this->limit();
	
    return stream.str();
}
