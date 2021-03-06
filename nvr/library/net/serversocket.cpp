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
#include "serversocket.h"
#include "../util/log.h"
#include "tcpsocket.h"
#include "socket.h"

using namespace library;

ServerSocket::ServerSocket() : port(0), closed(FALSE), bounded(FALSE), backlog(0), impl(NULL), bindaddr()
{
	impl = new Tcpsocket();
	impl->create();
}

ServerSocket::ServerSocket(uint16 port) : port(port), closed(FALSE), bounded(FALSE), backlog(0), impl(NULL), bindaddr()
{
	impl = new Tcpsocket();
	this->setupSocketImpl(port, getDefaultBacklog(), "0.0.0.0");
}

ServerSocket::ServerSocket(uint16 port, uint32 backlog) : port(port), closed(FALSE), bounded(FALSE), backlog(backlog), impl(NULL), bindaddr()
{
	impl = new Tcpsocket();
	this->setupSocketImpl(port, backlog, "0.0.0.0");
}

ServerSocket::ServerSocket(uint16 port, uint32 backlog, const char *ipaddress) : port(port), closed(FALSE), bounded(FALSE), backlog(backlog), impl(NULL), bindaddr()
{
	impl = new Tcpsocket();
	this->setupSocketImpl(port, backlog, ipaddress);
}

ServerSocket::ServerSocket(uint16 port, uint32 backlog, std::string &ipaddress) : port(port), closed(FALSE), backlog(backlog), impl(NULL), bindaddr()
{
	impl = new Tcpsocket();
	this->setupSocketImpl(port, backlog, ipaddress.c_str());
}

ServerSocket::~ServerSocket()
{
	this->close();
	if (impl) delete impl;
	impl = NULL;

	Log(LOG_INFO, "%s - %d", __FUNCTION__, __LINE__);
}

void ServerSocket::bind(std::string ip, uint16 port)
{
	if (isClosed()) return;
	
	if (this->bounded) {

		Log(LOG_DEBUG, "server already bounded.");
		return;
	}
	
	this->bind(ip, port, getDefaultBacklog());
}

void ServerSocket::bind(std::string ip, uint16 port, int32 backlog)
{
	if (isClosed()) return;
	
	if (this->bounded) {

		Log(LOG_DEBUG, "server already bounded.");
		return;
	}
	
	int32 iRet = this->impl->bind(ip, port);
	if (iRet == -1) {
		
		Log(LOG_ERROR, "bind to %s:%d error", ip.c_str(), port);
		return;
	}
	this->bounded = TRUE;
	this->impl->listen(backlog);

	return;
}

Socket* ServerSocket::accept(int32 &status)
{
	if (isClosed()) {

		status = -1;
		return NULL;
	}
	
	/* create a client socket and impl accept */
	Socket *socket = new Socket(new Tcpsocket());
	int32 iRet = this->doAccept(socket);
	if (iRet == -1) {

		status = -1;
		delete socket;
		Log(LOG_INFO, "ServerSocket accept error.");
		return NULL;
	}
	socket->accepted();
	status = 0;

	return socket;
}

void ServerSocket::setTimeOut(int32 timeOut)
{
	if (isClosed()) return;

	this->impl->setOption(SOCKET_OPTION_TIMEOUT, timeOut);
}

void ServerSocket::setReuseAddr(bool reuse)
{
	if (isClosed()) return;

	this->impl->setOption(SOCKET_OPTION_REUSEADDR, reuse);
}

void ServerSocket::setSendBufferSize(int32 sendBufferSize)
{
	if (isClosed()) return;

	this->impl->setOption(SOCKET_OPTION_SNDBUF, sendBufferSize);
}

void ServerSocket::setRecvBufferSize(int32 recvBufferSize)
{
	if (isClosed()) return;

	this->impl->setOption(SOCKET_OPTION_RCVBUF, recvBufferSize);
}

void ServerSocket::close()
{
	if (isClosed()) return;

	this->impl->close();
	this->closed = TRUE;

	Log(LOG_DEBUG, "ServerSocket::close - server is closed.");
}

int32 ServerSocket::doAccept(Socket *socket)
{
	if (socket == NULL) return -1;
	if (isClosed()) return -1;

	return this->impl->accept(socket->impl);
}

void ServerSocket::setupSocketImpl(int16 port, int32 backlog, const char *ip)
{
	std::string ipaddress = "0.0.0.0";
	
	if (this->impl == NULL) return;
	if (this->bounded) {

		Log(LOG_DEBUG, "server already bounded.");
		return;
	}
	
	if (isClosed()) {

		Log(LOG_WARN, "server is closed.");
		return;
	}

	this->impl->create();
	
	if (ip != NULL) ipaddress = ip;
	int32 iRet = this->impl->bind(ipaddress, port);
	if (iRet == -1) {
		Log(LOG_ERROR, "bind to %s:%d error", ip, port);
		return;
	}
	this->bounded = TRUE;
	this->impl->listen(backlog);
	
	Log(LOG_DEBUG, "serversocket %s:%d start.", ipaddress.c_str(), port);

	return;
}

int32 ServerSocket::getDefaultBacklog()
{
	return 50;
}