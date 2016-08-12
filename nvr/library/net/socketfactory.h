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
#ifndef __LIBRARY_NET_SOCKETFACTORY_H__
#define __LIBRARY_NET_SOCKETFACTORY_H__

#include "../../common/basetype.h"
#include <string>

namespace library {

class Socket;
class SocketImpl;
class SocketFactory
{
	private:

		SocketFactory();

		SocketFactory(const SocketFactory&);

		SocketFactory& operator= (const SocketFactory&);

	public:

		~SocketFactory();

	public:

		static void initialize();

		static void shutdown();

		static SocketFactory& getInstance();

	public:

		Socket* createSocket();

		Socket* createSocket(SocketImpl *socketImpl);

		Socket* createSocket(std::string &remoteIpaddress, int16 remotePort);

		Socket* createSocket(std::string &remoteIpaddress, int16 remotePort, std::string &localIpaddress, int16 localPort);
};


}

#endif /* end of file */
