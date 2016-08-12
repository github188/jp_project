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
#ifndef __LIBRARY_BUFFER_REMOTEBYTEARRAYBUFFER_H__
#define __LIBRARY_BUFFER_REMOTEBYTEARRAYBUFFER_H__

#include <string>
#include <vector>

#include "bytebuffer.h"

namespace library {

class RemoteByteArrayBuffer : public ByteBuffer
{
	private:

		RemoteByteArrayBuffer& operator= (const RemoteByteArrayBuffer&);

	public:

		RemoteByteArrayBuffer(int32 capacity);

		RemoteByteArrayBuffer(std::vector<uint8> &buffer);

		RemoteByteArrayBuffer(uint8* array, int32 size, int32 offset, int32 length, bool own = false);

		RemoteByteArrayBuffer(const RemoteByteArrayBuffer& other);

		virtual ~RemoteByteArrayBuffer();

	public: //@inheritDoc

		virtual uint8* array();

		virtual const uint8* array() const;

		virtual int32 arrayOffset() const;

		virtual RemoteByteArrayBuffer& compact();

        virtual RemoteByteArrayBuffer* duplicate();

		virtual uint8 get() const;

		virtual uint8 get(int32 index) const;

		virtual char getChar();

		virtual char getChar(int32 index) const;

		virtual double getDouble();

		virtual double getDouble( int32 index ) const;

		virtual float getFloat();

		virtual float getFloat(int32 index) const;

		virtual int64 getLong();

		virtual int64 getLong(int32 index) const;

		virtual int32 getInt();

		virtual int32 getInt(int32 index) const;

		virtual int16 getShort();

		virtual int16 getShort(int32 index) const;

		virtual uint8* getRawBytes(uint8* value, int32 length);

		virtual uint8* getRawBytes(int32 index, uint8* value, int32 length) const;

        virtual int32 put(int32 index, const char* value, int32 length);

		virtual RemoteByteArrayBuffer& putChar(char value);

		virtual RemoteByteArrayBuffer& putChar(int32 index, char value);

		virtual RemoteByteArrayBuffer& putDouble(double value);

		virtual RemoteByteArrayBuffer& putDouble(int32 index, double value);

		virtual RemoteByteArrayBuffer& putFloat(float value);

		virtual RemoteByteArrayBuffer& putFloat(int32 index, float value);

		virtual RemoteByteArrayBuffer& putLong(int64 value);

		virtual RemoteByteArrayBuffer& putLong(int32 index, int64 value);

		virtual RemoteByteArrayBuffer& putInt(int32 value);

		virtual RemoteByteArrayBuffer& putInt(int32 index, int32 value);

		virtual RemoteByteArrayBuffer& putShort(int16 value);

		virtual RemoteByteArrayBuffer& putShort(int32 index, int16 value);

		virtual RemoteByteArrayBuffer& putRawBytes(const uint8* value, int32 length);

		virtual RemoteByteArrayBuffer& putRawBytes(int32 index, const uint8* value, int32 length);

	private:

		// Buffer to read and write to, may be shared with other instances.
		uint8* _array;

		// Offset into the array that we are to start from
		int32 _offset;

		// The number of bytes we are limited to.
		int32 _length;

		// Whether this object owns the buffer
        bool _own;
};

}

#endif /* end of file */
