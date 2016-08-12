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
#ifndef __LIBRARY_IO_TCPINPUTSTREAM_H__
#define __LIBRARY_IO_TCPINPUTSTREAM_H__

#include "inputstream.h"

namespace library {

class Tcpsocket;
class TcpInputStream : public InputStream
{
	private:

		TcpInputStream(const TcpInputStream&);

		TcpInputStream& operator= (const TcpInputStream&);
		
	public:

		TcpInputStream(Tcpsocket *tcpsocket);

		virtual ~TcpInputStream();

	public:

		virtual void close();

		/**
		 * indicate the inputstream has something to read or not.
		 * @param return
		 *   true for something to read.
		 *   false for nothing to read.
		 */
		virtual bool avail();

		virtual bool isclosed();

	protected:

		virtual int32 doReadByteArray(uint8 *buffer, int32 size, int32 offset, int32 length);

	private:

		//socket handler obj
		Tcpsocket *socket;

		//indicate this stream is close or not
		volatile bool closed;
		
};

}

#endif /* end of file */