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
#ifndef __LIBRARY_NET_TCPSOCKETIMPL_H__
#define __LIBRARY_NET_TCPSOCKETIMPL_H__

#include "socketimpl.h"

namespace library {

class TcpsocketImpl;
class Tcpsocket : public SocketImpl
{
	private:

		Tcpsocket(const Tcpsocket&);

		Tcpsocket& operator= (const Tcpsocket&);

	public:

		Tcpsocket();

		virtual ~Tcpsocket();
		
	public:

		virtual int32 create();

		virtual int32 bind(const std::string &ipaddress, int32 port);

		virtual int32 listen(int32 backlog);

		virtual int32 accept(SocketImpl *socket);

		virtual int32 connect(const std::string& hostname, int port, int timeout);

		virtual InputStream *getInputStream();

		virtual OutputStream *getOutputStream();

		virtual int32 available();

		virtual void close();

		virtual void shutdownInput();

		virtual void shutdownOutput();

		virtual int32 getOption(int32 option) const;

		virtual int32 setOption(int32 option, int32 value);

		virtual std::string getLocalAddress();

		virtual std::string getRemoteAddress();

		virtual uint16 getLocalPort();

		virtual uint16 getRemotePort();

		virtual int32 getSocketDescriptor() const;

		virtual void clone(SocketImpl *src);

	private:

		int32 setSocketBlock(bool bBlocked);

	public: //streams

		/**
		 * read length by bytes from the peer.
		 * if return -1, the socket is closed.
		 * if return greater than 0, then success.
		 */
		virtual int32 read(uint8* buffer, int32 size, int32 offset, int32 length);

		/* @param return the successfully bytes writing to the peer. if the return 
		 *               value is less than length Or the resturn is -1, the outputstream is closed.
		 *
		 * success: the return value equal to length;
		 * fail: the return value is less than length;
		 */
		virtual int32 write(const uint8* buffer, int32 size, int32 offset, int32 length);

		virtual int32 write1(const uint8* buffer, int32 size, int32 offset, int32 length);

		virtual bool isConnected() const;

		virtual bool isclose();

	private:

		TcpsocketImpl *impl;
};

}

#endif /* end of file */
