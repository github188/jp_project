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
#ifndef __LIBRARY_IO_TCPOUTPUTSTREAM_H__
#define __LIBRARY_IO_TCPOUTPUTSTREAM_H__

#include "outputstream.h"

namespace library {

class Tcpsocket;
class TcpOutputStream : public OutputStream
{
	private:

		TcpOutputStream(const TcpOutputStream&);
		TcpOutputStream& operator= (const TcpOutputStream&);
		
	public:

		TcpOutputStream(Tcpsocket *tcpsocket);

		virtual ~TcpOutputStream();

	public:

		//flush out the bytes to dest.
		virtual void flush();

		//close the outputstream
		virtual void close();

		//check the stream is closed or not. true for close.
		virtual bool isclosed();

	protected:

		//write a array of bytes to buffer. @param size is the buffer length
		//@offset is the pos the data to fill. @length the bytes needed to write.
		virtual int32 doWriteByteArray(const uint8 *buffer, int32 size, int32 offset, int32 length, bool bSyncWriting);

	private:

		//socket handler obj
		Tcpsocket *socket;

		//indicate this stream is close or not
		bool closed;
};

}

#endif
