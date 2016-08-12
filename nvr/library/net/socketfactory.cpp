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
#include "socketfactory.h"
#include "socket.h"

using namespace library;

namespace {
	SocketFactory *theOnlyInstance;
}

////////////////////////////////////////////////////////////////////////////////
SocketFactory::SocketFactory()
{
}

SocketFactory::~SocketFactory()
{
}

void SocketFactory::initialize()
{
	theOnlyInstance = new SocketFactory();
}

void SocketFactory::shutdown()
{
	if (theOnlyInstance) delete theOnlyInstance;
	theOnlyInstance = NULL;
}

SocketFactory& SocketFactory::getInstance()
{
	return *theOnlyInstance;
}

Socket* SocketFactory::createSocket()
{
	return new Socket();
}

Socket* SocketFactory::createSocket(SocketImpl *socketImpl)
{
	if (socketImpl == NULL) return NULL;
	
	return new Socket(socketImpl);
}

Socket* SocketFactory::createSocket(std::string &remoteIpaddress, int16 remotePort)
{
	return new Socket(remoteIpaddress, remotePort);
}

Socket* SocketFactory::createSocket(std::string &remoteIpaddress, int16 remotePort, std::string &localIpaddress, int16 localPort)
{
	return new Socket(remoteIpaddress, remotePort, localIpaddress, localPort);
}