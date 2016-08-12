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
#include <arpa/inet.h>
#include <string.h>

#include "remotebytearraybuffer.h"
#include "../util/log.h"

using namespace library;

typedef struct TWO_INTS
{
	int32 v1;
	int32 v2;
} TWO_INTS;

union TRANSFER
{
	uint64 val;
	TWO_INTS ints;
};

RemoteByteArrayBuffer::RemoteByteArrayBuffer(int32 capacity) : ByteBuffer(capacity),
				_array(new uint8[capacity]), _offset(0), _length(capacity), _own(true)
{
}

RemoteByteArrayBuffer::RemoteByteArrayBuffer(std::vector<uint8> &buffer) : ByteBuffer((int32)buffer.size()), 
				_array(&buffer[0]), _offset(0), _length((int32)buffer.size()), _own(false)
{
}

RemoteByteArrayBuffer::RemoteByteArrayBuffer(uint8* array, int32 size, int32 offset, int32 length, bool own) :
				ByteBuffer(length), _array(array), _offset(offset), _length(length), _own(own)
{
	if (offset < 0 || offset > size) {
		Log(LOG_ERROR, "Offset parameter if out of bounds, %d", offset);
	}

	if (length < 0 || offset + length > size) {
		Log(LOG_ERROR, "length parameter if out of bounds, %d", length);
	}
}

RemoteByteArrayBuffer::RemoteByteArrayBuffer(const RemoteByteArrayBuffer& other) : ByteBuffer(other), 
				_array(other._array), _offset(other._offset), _length(other._length), _own(false)
{
}

RemoteByteArrayBuffer::~RemoteByteArrayBuffer()
{
	if (_own) delete [] _array;
	_array = NULL;
}

uint8* RemoteByteArrayBuffer::array()
{
	return this->_array;
}

const uint8* RemoteByteArrayBuffer::array() const
{
	return this->_array;
}

int32 RemoteByteArrayBuffer::arrayOffset() const
{
	return this->_offset;
}

RemoteByteArrayBuffer& RemoteByteArrayBuffer::compact()
{
	for (int32 ix = 0; ix < this->remaining(); ++ix) {
		this->putChar(ix, this->get(this->position() + ix));
	}

	this->position(this->limit() - this->position());
	this->limit(this->capacity());
	this->_markSet = FALSE;

	return *this;
}

RemoteByteArrayBuffer* RemoteByteArrayBuffer::duplicate()
{
	return new RemoteByteArrayBuffer(*this);
}

uint8 RemoteByteArrayBuffer::get() const
{
	return this->get(this->_position++);
}

uint8 RemoteByteArrayBuffer::get(int32 index) const
{
	if(index >= this->limit()) {
    	Log(LOG_ERROR, "RemoteByteArrayBuffer::get - Not enough data to fill request.");
		return -1;
    }

    return _array[_offset + index];
}

char RemoteByteArrayBuffer::getChar()
{
	return (char) this->get();
}

char RemoteByteArrayBuffer::getChar(int32 index) const
{
	return (char) this->get(index);
}

double RemoteByteArrayBuffer::getDouble()
{
	double value = 0;

	value = this->getDouble(this->position());
	this->_position += sizeof(double);

	return value;
}

double RemoteByteArrayBuffer::getDouble(int32 index) const
{
	double value = 0;
	TRANSFER data;
	
	if ((index + (int32)sizeof(double)) > this->limit()) {
		Log(LOG_ERROR, "RemoteByteArrayBuffer::getDouble - Not enough data to fill request.");
		return -1;
	}

	memcpy(&value, _array + _offset + index, sizeof(double));
	data.val = (int64) value;
	data.ints.v1 = ntohl(data.ints.v1);
	data.ints.v2 = ntohl(data.ints.v2);
	value = data.val;

	return value;
}

float RemoteByteArrayBuffer::getFloat()
{
	float value = 0;
	
	value = this->getFloat(this->position());
	this->_position += sizeof(float);

	return value;
}

float RemoteByteArrayBuffer::getFloat(int32 index) const
{
	float value = 0;
	
	if ((index + (int32)sizeof(float)) > this->limit()) {
		Log(LOG_ERROR, "RemoteByteArrayBuffer::getFloat - Not enough data to fill request.");
		return -1;
	}

	memcpy(&value, _array + _offset + index, sizeof(float));

	return ntohl(value);
}

int64 RemoteByteArrayBuffer::getLong()
{
	int64 value = 0;
	
	value = this->getLong(this->position());
	this->_position += sizeof(int64);

	return value;
}

int64 RemoteByteArrayBuffer::getLong(int32 index) const
{
	int64 value = 0;
	TRANSFER data;
	
	if ((index + (int32)sizeof(int64)) > this->limit()) {
		Log(LOG_ERROR, "RemoteByteArrayBuffer::getLong - Not enough data to fill request.");
		return -1;
	}

	memcpy(&value, _array + _offset + index, sizeof(int64));
	data.val = value;
	data.ints.v1 = ntohl(data.ints.v1);
	data.ints.v2 = ntohl(data.ints.v2);
	value = data.val;

	return value;
}

int32 RemoteByteArrayBuffer::getInt()
{
	int32 value = 0;
	
	value = this->getInt(this->position());
	this->_position += sizeof(int32);

	return value;
}

int32 RemoteByteArrayBuffer::getInt(int32 index) const
{
	int32 value = 0;
	
	if ((index + (int32)sizeof(int32)) > this->limit()) {
		Log(LOG_ERROR, "RemoteByteArrayBuffer::getInt - Not enough data to fill request.");
		return -1;
	}

	memcpy(&value, _array + _offset + index, sizeof(int32));

	return ntohl(value);
}

int16 RemoteByteArrayBuffer::getShort()
{
	int16 value = 0;

	value = this->getShort(this->position());
	this->_position += sizeof(int16);

	return value;
}

int16 RemoteByteArrayBuffer::getShort(int32 index) const
{
	int16 value = 0;
	
	if ((index + (int32)sizeof(int16)) > this->limit()) {
		Log(LOG_ERROR, "RemoteByteArrayBuffer::getShort - Not enough data to fill request.");
		return -1;
	}

	memcpy(&value, _array + _offset + index, sizeof(int16));

	return ntohs(value);
}

uint8* RemoteByteArrayBuffer::getRawBytes(uint8* value, int32 length)
{
	if (value == NULL || length <= 0) return value;

	this->getRawBytes(this->position(), value, length);
	this->_position += length;

	return value;
}

uint8* RemoteByteArrayBuffer::getRawBytes(int32 index, uint8* value, int32 length)const
{
	if (value == NULL || length <= 0) return value;
	
	if ((index + length) > this->limit()) {
		Log(LOG_ERROR, "RemoteByteArrayBuffer::getRawBytes - Not enough data to fill request.");
		return value;
	}

	memcpy(value, _array + _offset + index, length);

	return value;
}

int32 RemoteByteArrayBuffer::put(int32 index, const char* value, int32 length)
{
	if (!(this->_own)) {
		if (index + length > this->limit()) {
			Log(LOG_ERROR, "RemoteByteArrayBuffer::put - Not enough data to fill request.");
			return -1;
		}
	} else {
		// auto resize if owns buffer
		if (index + length > this->limit()) {
			int32 lot = length > MAX_BUFFER_SIZE_064 ? length : MAX_BUFFER_SIZE_064;
			int32 newlength = this->limit() + lot * 2;
			Log(LOG_INFO, "RemoteByteArrayBuffer::put - Not enough space, auto resize to %llu.", newlength);
			
			this->capacity(newlength);
			this->limit(newlength);
			uint8 *temp = new uint8[newlength];
			memcpy(temp, this->_array + _offset, this->_position);
			delete []this->_array;
			this->_array = temp;
		}
	}

	memcpy(this->_array + index + _offset, value, length);
	
	return 0;
}

RemoteByteArrayBuffer& RemoteByteArrayBuffer::putChar(char value)
{
	int32 index = this->_position;

	int32 iRet = this->put(index, (const char*)&value, sizeof(value));
	if (iRet == 0)
		this->_position += sizeof(value);
	else
		Log(LOG_ERROR, "RemoteByteArrayBuffer::putChar(1).");

	return *this;
}

RemoteByteArrayBuffer& RemoteByteArrayBuffer::putChar(int32 index, char value)
{
	int32 iRet = this->put(index, (const char*)&value, sizeof(value));
	if (iRet == -1) {
		Log(LOG_ERROR, "RemoteByteArrayBuffer::putChar(1, 1).");
	}
	return *this;
}

RemoteByteArrayBuffer& RemoteByteArrayBuffer::putDouble(double value)
{
	int32 index = this->_position;
	TRANSFER data;

	data.val = (int64) value;
	data.ints.v1 = htonl(data.ints.v1);
	data.ints.v2 = htonl(data.ints.v2);
	value = data.val;

	int32 iRet = this->put(index, (const char*)&value, sizeof(value));
	if (iRet == 0)
		this->_position += sizeof(value);
	else
		Log(LOG_ERROR, "RemoteByteArrayBuffer::putDouble(1).");

	return *this;
}

RemoteByteArrayBuffer& RemoteByteArrayBuffer::putDouble(int32 index, double value)
{
	TRANSFER data;

	data.val = (int64) value;
	data.ints.v1 = htonl(data.ints.v1);
	data.ints.v2 = htonl(data.ints.v2);
	value = data.val;
	
	int32 iRet = this->put(index, (const char*)&value, sizeof(value));
	if (iRet == -1) {
		Log(LOG_ERROR, "RemoteByteArrayBuffer::putDouble(1, 1).");
	}
	return *this;
}

RemoteByteArrayBuffer& RemoteByteArrayBuffer::putFloat(float value)
{
	int32 index = this->_position;

	value = htonl((int32) value);
	int32 iRet = this->put(index, (const char*)&value, sizeof(value));
	if (iRet == 0)
		this->_position += sizeof(value);
	else
		Log(LOG_ERROR, "RemoteByteArrayBuffer::putFloat(1).");

	return *this;
}

RemoteByteArrayBuffer& RemoteByteArrayBuffer::putFloat(int32 index, float value)
{
	value = htonl((int32) value);
	int32 iRet = this->put(index, (const char*)&value, sizeof(value));
	if (iRet == -1) {
		Log(LOG_ERROR, "RemoteByteArrayBuffer::putFloat(1, 1).");
	}
	return *this;
}

RemoteByteArrayBuffer& RemoteByteArrayBuffer::putLong(int64 value)
{
	int32 index = this->_position;
	TRANSFER data;

	data.val = value;
	data.ints.v1 = htonl(data.ints.v1);
	data.ints.v2 = htonl(data.ints.v2);
	value = data.val;

	int32 iRet = this->put(index, (const char*)&value, sizeof(value));
	if (iRet == 0)
		this->_position += sizeof(value);
	else
		Log(LOG_ERROR, "RemoteByteArrayBuffer::putLong(1).");

	return *this;
}

RemoteByteArrayBuffer& RemoteByteArrayBuffer::putLong(int32 index, int64 value)
{
	TRANSFER data;

	data.val = value;
	data.ints.v1 = htonl(data.ints.v1);
	data.ints.v2 = htonl(data.ints.v2);
	value = data.val;
	
	int32 iRet = this->put(index, (const char*)&value, sizeof(value));
	if (iRet == -1) {
		Log(LOG_ERROR, "RemoteByteArrayBuffer::putLong(1, 1).");
	}
	return *this;
}

RemoteByteArrayBuffer& RemoteByteArrayBuffer::putInt(int32 value)
{
	int32 index = this->_position;

	value = htonl((int32) value);
	int32 iRet = this->put(index, (const char*)&value, sizeof(value));
	if (iRet == 0)
		this->_position += sizeof(value);
	else
		Log(LOG_ERROR, "RemoteByteArrayBuffer::putInt(1).");

	return *this;
}

RemoteByteArrayBuffer& RemoteByteArrayBuffer::putInt(int32 index, int32 value)
{
	value = htonl((int32) value);
	int32 iRet = this->put(index, (const char*)&value, sizeof(value));
	if (iRet == -1) {
		Log(LOG_ERROR, "RemoteByteArrayBuffer::putInt(1, 1).");
	}
	return *this;
}

RemoteByteArrayBuffer& RemoteByteArrayBuffer::putShort(int16 value)
{
	int32 index = this->_position;

	value = htons((int32) value);
	int32 iRet = this->put(index, (const char*)&value, sizeof(value));
	if (iRet == 0)
		this->_position += sizeof(value);
	else
		Log(LOG_ERROR, "RemoteByteArrayBuffer::putShort(1).");

	return *this;
}

RemoteByteArrayBuffer& RemoteByteArrayBuffer::putShort(int32 index, int16 value)
{
	value = htons((int32) value);
	int32 iRet = this->put(index, (const char*)&value, sizeof(value));
	if (iRet == -1) {
		Log(LOG_ERROR, "RemoteByteArrayBuffer::putShort(1, 1).");
	}
	return *this;
}

RemoteByteArrayBuffer& RemoteByteArrayBuffer::putRawBytes(const uint8* value, int32 length)
{
	int32 index = this->_position;
	
	if (value == NULL || length <= 0) return *this;

	int32 iRet = this->put(index, (const char*)value, length);
	if (iRet == 0)
		this->_position += length;
	else
		Log(LOG_ERROR, "RemoteByteArrayBuffer::putRawBytes(1, 1).");

	return *this;
}

RemoteByteArrayBuffer& RemoteByteArrayBuffer::putRawBytes(int32 index, const uint8* value, int32 length)
{
	if (value == NULL || length <= 0) return *this;
	
	int32 iRet = this->put(index, (const char*)&value, length);
	if (iRet == -1) {
		Log(LOG_ERROR, "RemoteByteArrayBuffer::putRawBytes(1, 1, 1).");
	}
	return *this;
}