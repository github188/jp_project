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
#ifndef __LIBRARY_NET_SERVERSOCKET_H__
#define __LIBRARY_NET_SERVERSOCKET_H__

#include "../../library/net/socketimpl.h"
#include "../../common/basetype.h"
#include <string>

namespace library {

class Socket;
class ServerSocket
{
	private:

		ServerSocket(const ServerSocket&);

		ServerSocket& operator= (const ServerSocket&);
		
	public:

		ServerSocket();
		
		ServerSocket(uint16 port);

		ServerSocket(uint16 port, uint32 backlog);

		ServerSocket(uint16 port, uint32 backlog, const char *ipaddress);

		ServerSocket(uint16 port, uint32 backlog, std::string &ipaddress);

		virtual ~ServerSocket();

	public:

		void bind(std::string ip, uint16 port);

		void bind(std::string ip, uint16 port, int32 backlog);

		/* if status return -1. then it indicates that serversocket is closed */
		Socket* accept(int32 &status);

		void setTimeOut(int32 timeOut); // timeout in millseconds

		void setReuseAddr(bool reuse);

		void setSendBufferSize(int32 sendBufferSize);

		void setRecvBufferSize(int32 recvBufferSize);

		int32 getSocketDescriptor() const {
			return this->impl->getSocketDescriptor();
		};

		void close();

		bool isClosed()const {return closed;};

	private:

		int32 doAccept(Socket*);

		void setupSocketImpl(int16 port, int32 backlog, const char *ip);

		int32 getDefaultBacklog();

	private:

		uint16 port;
		bool closed;
		bool bounded;
		int32 backlog;
		SocketImpl* impl;
		std::string bindaddr;
};

}

#endif /* end of file */
