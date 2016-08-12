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
#include <netinet/tcp.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

#include "../internal/platform_lin.h"
#include "../io/tcpoutputstream.h"
#include "../io/tcpinputstream.h"
#include "../util/log.h"
#include "tcpsocket.h"

using namespace platform;
using namespace library;

namespace library {
	
class TcpsocketImpl
{
	public:

		struct sockaddr_in localAddress;
		struct sockaddr_in remoteAddress;
		TcpInputStream *inputstream;
		TcpOutputStream *outputstream;
		bool inputshutdown;
		bool outputshutdown;
		bool closed;
		bool connected;
		int32 timeout; //millsecs
		int32 soLinger;
		int32 sendbuffersize;
		int32 recvbuffersize;
		bool enableNagle;
		bool enableKeepAlive;
		int32 socketHandler;

	public:

		TcpsocketImpl() : inputstream(NULL), outputstream(NULL), inputshutdown(FALSE),
						  outputshutdown(FALSE), closed(FALSE), connected(FALSE), timeout(-1), 
						  soLinger(-1), sendbuffersize(8092), recvbuffersize(8092), enableNagle(FALSE),
						  enableKeepAlive(TRUE), socketHandler(-1) {
		};

		~TcpsocketImpl() {};
};

}

////////////////////////////////////////////////////////////////////////////////
Tcpsocket::Tcpsocket() : impl(new TcpsocketImpl())
{
}

Tcpsocket::~Tcpsocket()
{
	this->close();
	
	if (this->impl->inputstream != NULL) {
		delete this->impl->inputstream;
		this->impl->inputstream = NULL;
	}

	if (this->impl->outputstream != NULL) {
		delete this->impl->outputstream;
		this->impl->outputstream = NULL;
	}
	
	if (this->impl) delete this->impl;
	this->impl = NULL;
}

int32 Tcpsocket::create()
{
	int32 iRet = -1;
	
	iRet = PlatformAPI::createsocket(this->impl->socketHandler, AF_INET, SOCK_STREAM, IPPROTO_TCP);
//	Log(LOG_DEBUG, "socket id = %d", this->impl->socketHandler);

	return iRet;
}

int32 Tcpsocket::bind(const std::string &ipaddress, int32 port)
{
	uint32 addr = INADDR_ANY;
	sockaddr_in *sa = NULL;

	if (!ipaddress.empty()) {
		addr = ::inet_addr(ipaddress.c_str());
	}
	
	memset(&this->impl->localAddress, 0, sizeof(sockaddr_in));
	sa = &this->impl->localAddress;
	sa->sin_family = AF_INET;
	sa->sin_addr.s_addr = addr;
	sa->sin_port = htons(port);
	
	return PlatformAPI::bindsocket(this->impl->socketHandler, (const struct sockaddr *)sa, sizeof(sockaddr));
}

int32 Tcpsocket::listen(int32 backlog)
{
	return PlatformAPI::listensocket(this->impl->socketHandler, backlog);
}

int32 Tcpsocket::accept(SocketImpl *socket)
{
	socklen_t len = sizeof(struct sockaddr);
	struct sockaddr_in *sa = NULL;
	int32 iRet = -1;
	
	if (socket == NULL) return -1;
	
	Tcpsocket *tcpsocket = dynamic_cast<Tcpsocket *>(socket);
	sa = &(tcpsocket->impl->remoteAddress);

	// set block mode
	if (this->impl->timeout > 0) {
		setSocketBlock(false);
	} else {
		setSocketBlock(true);
	}

	::memset(sa, 0, sizeof(struct sockaddr_in));
	iRet = PlatformAPI::acceptsocket(this->impl->socketHandler, tcpsocket->impl->socketHandler, (struct sockaddr *)sa, &len, this->impl->timeout);
	if (iRet < 0) {
		Log(LOG_WARN, "accept error.");
		return -1;
	}

	//clone configuration of server.
	tcpsocket->clone(this);
	tcpsocket->impl->connected = TRUE;
		
	Log(LOG_DEBUG, "socket(%d), accept client: %s:%d.", tcpsocket->impl->socketHandler, inet_ntoa(sa->sin_addr), ntohs(sa->sin_port));

	return 0;
}

int32 Tcpsocket::connect(const std::string &hostname, int32 port, int32 timeout)
{
	int32 iRet = -1;
	
	if (this->impl == NULL) return -1;
	
	if (port < 0 || port > 65535) return -1;
	
	if (hostname.size() <= 0) return -1;

	/* user's setting is first */
	if (timeout > 0) {
		setSocketBlock(false);
	} else {
		setSocketBlock(true);
	}

	struct sockaddr_in *sa = NULL;
	sa = &this->impl->remoteAddress;
	sa->sin_family = AF_INET;
	sa->sin_addr.s_addr = inet_addr(hostname.c_str());
	sa->sin_port = htons(port);

	iRet = PlatformAPI::connectsocket(this->impl->socketHandler, (const struct sockaddr *)sa, sizeof(struct sockaddr), timeout);
	
	//restore the old flags
	if (this->impl->timeout > 0) {
		setSocketBlock(false);
	} else {
		setSocketBlock(true);
	}

	if (iRet == -1) {
		
		Log(LOG_INFO, "connect %s:%d error.", hostname.c_str(), port);
		return iRet;
	}

	Log(LOG_INFO, "client(%d) connect to server %s:%d success.", this->impl->socketHandler, hostname.c_str(), port);
	return iRet;
}

InputStream* Tcpsocket::getInputStream()
{
	if (this->impl->socketHandler < 0 || this->isclose()) {
		this->impl->inputshutdown = TRUE;
		return NULL;
	}
	
	if (this->impl->inputshutdown) {
		return NULL;
	}

	if (this->impl->inputstream == NULL) {
		this->impl->inputshutdown = FALSE;
		this->impl->inputstream = new TcpInputStream(this);
	}

	return this->impl->inputstream;
}

OutputStream *Tcpsocket::getOutputStream()
{
	if (this->impl->socketHandler == 0 || this->isclose()) {
		this->impl->outputshutdown = TRUE;
		return NULL;
	}
	
	if (this->impl->outputshutdown) {
		return NULL;
	}

	if (this->impl->outputstream == NULL) {
		this->impl->outputshutdown = FALSE;
		this->impl->outputstream = new TcpOutputStream(this);
	}

	return this->impl->outputstream;
}

int32 Tcpsocket::available()
{
	if (this->impl->socketHandler < 0 || this->isclose()) {
		return -1;
	}
	
	if (this->impl->inputshutdown) {
		return -1;
	}

	fd_set rd;
    FD_ZERO(&rd);
	struct timeval tv;

	tv.tv_sec = 0;
	tv.tv_usec = this->impl->timeout;
    FD_SET(this->impl->socketHandler, &rd);
	int32 returnCode = ::select(this->impl->socketHandler + 1, &rd, NULL, NULL, &tv);
	if (returnCode == -1) {
		return -1;
	}
	
    return (returnCode == 0) ? 0 : 1;
}

void Tcpsocket::close()
{
	if (this->impl->closed) return;

	if (this->impl->inputstream) {
		this->impl->inputstream->close();
		this->impl->inputshutdown = TRUE;
	}

	if (this->impl->outputstream) {
		this->impl->outputstream->close();
		this->impl->outputshutdown = TRUE;
	}

	PlatformAPI::closesocket(this->impl->socketHandler);
	this->impl->connected = FALSE;
	this->impl->closed = TRUE;
}

void Tcpsocket::shutdownInput()
{
	if (this->isclose()) {
		this->impl->inputshutdown = TRUE;
		return ;
	}
	
	if (this->impl->inputshutdown || this->impl->inputstream == NULL) {
		this->impl->inputshutdown = TRUE;
		return ;
	}

	PlatformAPI::closesocketinput(this->impl->socketHandler);
	this->impl->inputshutdown = TRUE;
}

void Tcpsocket::shutdownOutput()
{
	if (this->isclose()) {
		this->impl->outputshutdown = TRUE;
		return ;
	}
	if (this->impl->outputshutdown || this->impl->outputstream == NULL) {
		this->impl->outputshutdown = TRUE;
		return ;
	}

	PlatformAPI::closesocketoutput(this->impl->socketHandler);
	this->impl->outputshutdown = TRUE;
}

int32 Tcpsocket::getOption(int32 option) const
{
	socklen_t len;
	
	if (this->impl->closed) {
		return -1;
	}

	if (option == SOCKET_OPTION_TIMEOUT) {
		
		return this->impl->timeout;
	} else if (option == SOCKET_OPTION_LINGER) {

		return this->impl->soLinger;
	} else if (option == SOCKET_OPTION_REUSEADDR) {

		int32 retValue = -1;
		if ( -1 == PlatformAPI::getsocketoption(this->impl->socketHandler, SOL_SOCKET, SO_REUSEADDR, &retValue, &len))
			return -1;
		else
			return retValue;
	} else if (option == SOCKET_OPTION_SNDBUF) {

		int32 retValue = -1;
		if ( -1 == PlatformAPI::getsocketoption(this->impl->socketHandler, SOL_SOCKET, SO_SNDBUF, &retValue, &len))
			return -1;
		else {
			this->impl->sendbuffersize = retValue;
			return retValue;
		}
	} else if (option == SOCKET_OPTION_RCVBUF) {

		int32 retValue = -1;
		if ( -1 == PlatformAPI::getsocketoption(this->impl->socketHandler, SOL_SOCKET, SO_RCVBUF, &retValue, &len))
			return -1;
		else {
			this->impl->recvbuffersize = retValue;
			return retValue;
		}
	} else if (option == SOCKET_OPTION_TCP_NODELAY) {

		int32 retValue = -1;
		if ( -1 == PlatformAPI::getsocketoption(this->impl->socketHandler, IPPROTO_TCP, TCP_NODELAY, &retValue, &len))
			return -1;
		else {
			this->impl->enableNagle = retValue;
			return retValue;
		}
	} else if (option == SOCKET_OPTION_KEEPALIVE) {

		int32 retValue = -1;
		if ( -1 == PlatformAPI::getsocketoption(this->impl->socketHandler, SOL_SOCKET, SO_KEEPALIVE, &retValue, &len))
			return -1;
		else {

			this->impl->enableKeepAlive = retValue;
			return retValue;
		}
	}

	Log(LOG_INFO, "Get Socket Option is not valid for this Socket type.");
	return -1;
}

int32 Tcpsocket::setOption(int32 option, int32 value)
{
	if (this->impl->closed) {
		return -1;
	}

	if (option == SOCKET_OPTION_TIMEOUT) {
		
		if (value > 0) {			
			setSocketBlock(false);
		} else {
			setSocketBlock(true);
		}
		this->impl->timeout = value;

		return 0;
	} else if (option == SOCKET_OPTION_LINGER) {

		struct linger li;
		
		this->impl->soLinger = value;
		bool on = value > 0 ? 1 : 0;
		li.l_onoff = on;
		li.l_linger = 30; //default max linger time is 30 secs.
		
		return PlatformAPI::setsocketoption(this->impl->socketHandler, SOL_SOCKET, SO_LINGER, &li, sizeof(struct linger));
	} else if (option == SOCKET_OPTION_REUSEADDR) {

		return PlatformAPI::setsocketoption(this->impl->socketHandler, SOL_SOCKET, SO_REUSEADDR, (void *)&value, sizeof(int32));
	} else if (option == SOCKET_OPTION_SNDBUF) {

		this->impl->sendbuffersize = value;
		return PlatformAPI::setsocketoption(this->impl->socketHandler, SOL_SOCKET, SO_SNDBUF, (void *)&value, sizeof(int32));
	} else if (option == SOCKET_OPTION_RCVBUF) {

		this->impl->recvbuffersize = value;
		return PlatformAPI::setsocketoption(this->impl->socketHandler, SOL_SOCKET, SO_RCVBUF, (void *)&value, sizeof(int32));
	} else if (option == SOCKET_OPTION_TCP_NODELAY) {

		this->impl->enableNagle = value;
		return PlatformAPI::setsocketoption(this->impl->socketHandler, IPPROTO_TCP, TCP_NODELAY, (void *)&value, sizeof(int32));
	} else if (option == SOCKET_OPTION_KEEPALIVE) {

		this->impl->enableKeepAlive = value;
		return PlatformAPI::setsocketoption(this->impl->socketHandler, SOL_SOCKET, SO_KEEPALIVE, (void *)&value, sizeof(int32));
	}

	Log(LOG_INFO, "Set Socket Option is not valid for this Socket type.");
	return -1;
}

std::string Tcpsocket::getLocalAddress()
{
	if (!(this->impl->closed)) {

		struct sockaddr_in sa;
		socklen_t len = sizeof(sa);
//		uint16 port;

		memset(&sa, 0, sizeof(sa));
		if (-1 == ::getsockname(this->impl->socketHandler, (struct sockaddr*)&sa, &len))
			return std::string("0.0.0.0");

		this->impl->localAddress = sa;

		struct in_addr addr;
		addr.s_addr = (in_addr_t) sa.sin_addr.s_addr;

		return std::string(::inet_ntoa(addr));
	}

	return std::string("0.0.0.0");
}

std::string Tcpsocket::getRemoteAddress()
{
	struct in_addr addr;

	addr.s_addr = (in_addr_t)this->impl->remoteAddress.sin_addr.s_addr;
	
	return std::string(::inet_ntoa(addr));
}

uint16 Tcpsocket::getLocalPort()
{
	getLocalAddress();
	return ntohs(this->impl->localAddress.sin_port);
}

uint16 Tcpsocket::getRemotePort()
{
	return ntohs(this->impl->remoteAddress.sin_port);
}

int32 Tcpsocket::getSocketDescriptor() const
{
	if (this->impl->closed) return -1;
	
	return this->impl->socketHandler;
}

void Tcpsocket::clone(SocketImpl *src)
{
	if (src == NULL) return;

	Tcpsocket *tcpsocket = dynamic_cast<Tcpsocket *>(src);
	this->setOption(SOCKET_OPTION_TIMEOUT, tcpsocket->impl->timeout);
	this->setOption(SOCKET_OPTION_LINGER, tcpsocket->impl->soLinger);
	this->setOption(SOCKET_OPTION_SNDBUF, tcpsocket->impl->sendbuffersize);
	this->setOption(SOCKET_OPTION_RCVBUF, tcpsocket->impl->recvbuffersize);
	this->setOption(SOCKET_OPTION_TCP_NODELAY, tcpsocket->impl->enableNagle);
	this->setOption(SOCKET_OPTION_KEEPALIVE, tcpsocket->impl->enableKeepAlive);
}

int32 Tcpsocket::read(uint8* buffer, int32 size, int32 offset, int32 length)
{
	if (this->impl->closed) return -1;
	if (this->impl->inputshutdown) return -1;
	if (length == 0) return 0;
	if (buffer == NULL || size < 0) return 0;
	if (offset > size || length > size - offset) return 0;

	int32 result = -1;
	result = PlatformAPI::read(this->impl->socketHandler, (char *)(buffer + offset), length, this->impl->timeout);
	if (result <= 0) {
		
		this->impl->inputshutdown = true;
		Log(LOG_INFO, "socket inputstream is closed. error: %s.", strerror(errno));
//		return -1;
	}
	if (this->impl->closed) return -1;
	
	return result;
}

// write in block mode
int32 Tcpsocket::write(const uint8* buffer, int32 size, int32 offset, int32 length)
{
	if (length == 0) return 0;
	if (this->impl->closed) return -1;
	if (this->impl->outputshutdown) return -1;
	if (buffer == NULL || size < 0 || offset < 0) return 0;
	if (offset > size || length > size - offset) return 0;

	int32 remaining = length;
	int32 result = -1;

	const uint8 *lbuffer = buffer + offset;
	while (remaining > 0 && !(this->impl->closed) && !(this->impl->outputshutdown)) {

		result = PlatformAPI::write(this->impl->socketHandler, (char *)lbuffer, remaining, this->impl->timeout);
		if (result == -1) {

			this->impl->outputshutdown = TRUE;
			Log(LOG_INFO, "socket write error, peer is closed.");
			return -1;
			//break;
		}

		// move us to next position to write, or maybe end.
		lbuffer += result;
		remaining = remaining - result;
	}

	return (length - remaining);
}

// write in unblock mode
int32 Tcpsocket::write1(const uint8* buffer, int32 size, int32 offset, int32 length)
{
	if (length == 0) return 0;
	if (this->impl->closed) return -1;
	if (this->impl->outputshutdown) return -1;
	if (buffer == NULL || size < 0 || offset < 0) return 0;
	if (offset > size || length > size - offset) return 0;

	int32 remaining = length;
	int32 result = -1;

	const uint8 *lbuffer = buffer + offset;
	if (remaining > 0 && !(this->impl->closed) && !(this->impl->outputshutdown)) {

		result = PlatformAPI::write(this->impl->socketHandler, (char *)lbuffer, remaining, this->impl->timeout);
		if (result == -1) {
			Log(LOG_INFO, "socket write error, peer is closed ?");
		}
	}

	return result;
}

bool Tcpsocket::isConnected() const
{
	return this->impl->connected;
}

bool Tcpsocket::isclose()
{
	if (this->impl->closed) {
		return TRUE;
	}

	return FALSE;
}

int32 Tcpsocket::setSocketBlock(bool bBlocked)
{
	int32 toflags = 0;
	int32 flags = 0;

	flags = ::fcntl(this->impl->socketHandler, F_GETFL,0);
	if (bBlocked) {
		
		toflags = flags & ~O_NONBLOCK;
//		Log(LOG_DEBUG, "set socket BLOCK.");
	} else {

		toflags = flags | O_NONBLOCK;
//		Log(LOG_DEBUG, "set socket unBLOCK.");
	}

	if (-1 == ::fcntl(this->impl->socketHandler, F_SETFL, toflags)) {

		Log(LOG_WARN, "set socket BLOCK error.");
		return flags;
	}

	return flags;
}