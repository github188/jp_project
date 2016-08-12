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
#include <errno.h>
#include "../util/log.h"
#include "tcpsocket.h"
#include "socket.h"

using namespace library;

Socket::Socket() : closed(false), bounded(false), connected(false), impl(NULL)
{
	impl = new Tcpsocket();
	impl->create();
}

Socket::Socket(SocketImpl *socketImpl) : closed(false), bounded(false), connected(false), impl(socketImpl)
{
}

Socket::Socket(const std::string &remoteIpaddress, int16 remotePort) : closed(false), bounded(false), connected(false), impl(NULL)
{
	std::string localIpAddress = "0.0.0.0";
	uint16 localPort = 0;
	
	impl = new Tcpsocket();
	this->initSocketImpl(remoteIpaddress, remotePort, localIpAddress, localPort);
}

Socket::Socket(const std::string &remoteIpaddress, int16 remotePort, const std::string &localIpaddress, int16 localPort) : closed(false), 
				bounded(false), connected(false), impl(NULL)
{
	impl = new Tcpsocket();
	this->initSocketImpl(remoteIpaddress, remotePort, localIpaddress, localPort);
}

Socket::~Socket()
{
	this->close();

	if (impl) delete impl;
	impl = NULL;
}

int32 Socket::bind(const std::string &localIpaddress, uint16 localPort)
{
	if (isClosed()) return -1;
	if (isBound()) return 0;

	int32 iRet = this->impl->bind(localIpaddress, localPort);
	if (iRet == -1) {

		Log(LOG_INFO, "Socket bind: %s-%d error, errstr: %s.", localIpaddress.c_str(), localPort, 
			strerror(errno));
		return -1;
	}
	this->bounded = true;

	return 0;
}

int32 Socket::connect(const std::string &remoteIpaddress, uint16 remotePort)
{
	if (isClosed()) return -1;
	if (isConnected()) return 0;

	int32 iRet = this->impl->connect(remoteIpaddress, remotePort, -1);
	if (iRet == -1) {

		Log(LOG_INFO, "Socket connect:%s-%d eror.", remoteIpaddress.c_str(), remotePort);
		return -1;
	}
	this->connected = true;

	return 0;
}

int32 Socket::connect(const std::string &remoteIpaddress, uint16 remotePort, int32 timeout)
{
	if (isClosed()) return -1;
	if (isConnected()) return 0;

	int32 iRet = this->impl->connect(remoteIpaddress, remotePort, timeout);
	if (iRet == -1) {

		Log(LOG_INFO, "Socket connect:%s-%d eror.", remoteIpaddress.c_str(), remotePort);
		return -1;
	}
	this->connected = true;

	return 0;
}

int32 Socket::reConnect(const std::string &remoteIpaddress, uint16 remotePort)
{
	// delete the old record
	if (!isClosed()) {
		
		this->close();
		if (impl) delete impl;
		impl = NULL;
	}

	std::string localIpAddress = "0.0.0.0";
	uint16 localPort = 0;
	
	impl = new Tcpsocket();
	this->initSocketImpl(remoteIpaddress, remotePort, localIpAddress, localPort);

	return this->connected ? 0 : -1;
}

int32 Socket::reConnect(const std::string &remoteIpaddress, uint16 remotePort, int32 timeout)
{
	// delete the old record
	if (!isClosed()) {
		
		this->close();
		if (impl) delete impl;
		impl = NULL;
	}

	std::string localIpAddress = "0.0.0.0";
	uint16 localPort = 0;
	
	impl = new Tcpsocket();
	this->initSocketImpl(remoteIpaddress, remotePort, localIpAddress, localPort);

	return this->connected ? 0 : -1;
}

int32 Socket::accepted()
{
	this->closed = false;
	this->bounded = true;
	this->connected = true;

	return 0;
}

int32 Socket::setReuseAddr(bool resue)
{
	if (isClosed()) return -1;

	return this->impl->setOption(SOCKET_OPTION_REUSEADDR, resue);
}

int32 Socket::setSoLinger(bool state, int32 timeout)
{
	if (isClosed()) return -1;

	return this->impl->setOption(SOCKET_OPTION_LINGER, timeout);
}

int32 Socket::setKeepAlive(bool keepAlive)
{
	if (isClosed()) return -1;

	return this->impl->setOption(SOCKET_OPTION_KEEPALIVE, keepAlive);
}

int32 Socket::setReceiveBufferSize(int32 size)
{
	if (isClosed()) return -1;

	return this->impl->setOption(SOCKET_OPTION_RCVBUF, size);
}

int32 Socket::setSendBufferSize(int32 size)
{
	if (isClosed()) return -1;

	return this->impl->setOption(SOCKET_OPTION_SNDBUF, size);
}

int32 Socket::setSoTimeout(int32 timeout)
{
	if (isClosed()) return -1;

	return this->impl->setOption(SOCKET_OPTION_TIMEOUT, timeout);
}

int32 Socket::setTcpNoDelay(bool value)
{
	if (isClosed()) return -1;

	return this->impl->setOption(SOCKET_OPTION_TCP_NODELAY, value);
}

InputStream* Socket::getInputStream()
{
	if (isClosed()) return NULL;

	return this->impl->getInputStream();
}

OutputStream* Socket::getOutputStream()
{
	if (isClosed()) return NULL;

	return this->impl->getOutputStream();
}

std::string Socket::getLocalAddress()const
{
	if (!isClosed()&& (isBound()|| isConnected()))
		return this->impl->getLocalAddress();
	else
		return "";
}

std::string Socket::getRemoteAddress() const
{
	if (!isClosed() && isConnected())
		return this->impl->getRemoteAddress();
	else
		return "";
}

uint16 Socket::getLocalPort() const
{
	if (isClosed() || !isBound()) return -1;

	return this->impl->getLocalPort();
}

uint16 Socket::getRemotePort() const
{
	if (isClosed() || !isConnected()) return -1;

	return this->impl->getRemotePort();
}

int32 Socket::getSocketDescriptor()const
{
	return this->impl->getSocketDescriptor();
}

int32 Socket::close()
{
	if (this->closed) return TRUE;

	this->impl->close();
		
	this->closed = true;
	this->bounded = false;
	this->connected = false;

	Log(LOG_DEBUG, "Socket::close - socket is closed.");
	return 0;
}

void Socket::initSocketImpl(const std::string &remoteIpaddress, uint16 remotePort, const std::string &localIpaddress, uint16 localPort)
{
	int32 iRet = -1;
	
	if (impl == NULL) return;
	if ((localIpaddress == "0.0.0.0") && (localPort != 0)) return;
	if ((localPort == 0) && (localIpaddress != "0.0.0.0")) return;

	this->impl->create();
	if (localPort != 0) {
		iRet = this->impl->bind(localIpaddress, localPort);
		if (iRet == -1) {

			Log(LOG_INFO, "Socket initSocketImpl bind: %s-%d error.", localIpaddress.c_str(), localPort);
			return;
		}
		this->bounded = true;
	}

	//default is using blocking mode to connect.
	iRet = this->impl->connect(remoteIpaddress, remotePort, -1);
	if (iRet == -1) {
		this->impl->close();
		Log(LOG_INFO, "Socket initSocketImpl connect:%s-%d eror.", remoteIpaddress.c_str(), remotePort);
		return;
	}
	this->connected = true;

	return;
}