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
#include <sstream>

#include "../../library/net/serversocket.h"
#include "../../library/util/idgenerator.h"
#include "../../library/net/socket.h"
#include "../../library/util/log.h"
#include "connectionhandler.h"
#include "connection.h"
#include "tcpserver.h"
#include "iohandler.h"

using namespace library;
using namespace corelib;
using namespace corelib::MTP_NETWORK;

TcpServer::TcpServer(ConnectionHandler *handler) : IOEvent(SERVER), closeAndDelete_(false), 
				connHandler_(handler), _serversocket(NULL)
{
}

TcpServer::~TcpServer()
{
	delete _serversocket;
}

void TcpServer::handle(std::string& socketId, SOCKETOP op)
{
	if (this->id_ != socketId) {
		Log(LOG_WARN, "TcpServer::handle - not me.");
		return;
	}

	if (op & SO_READ) {
		doAccept();
	} else if (op & SO_CLOSE) {
		this->_serversocket->close();
	} else {
		;
	}
}

void TcpServer::bind(uint16 port, int32 timeOutInMillsec)
{
	_serversocket = new ServerSocket();
	_serversocket->setSendBufferSize(MAX_BUFFER_SIZE_08m); //be careful
	_serversocket->setRecvBufferSize(MAX_BUFFER_SIZE_08m); //be careful
	_serversocket->setReuseAddr(true);
	_serversocket->setTimeOut(timeOutInMillsec);
	_serversocket->bind("", port);

	std::ostringstream oss;
	IDGenerator *idGenerator = IDGeneratorMgr::getInstance();
	oss << "server: " << port << " - " << idGenerator->increase();
	this->id_ = oss.str();

	Log(LOG_DEBUG, "TTTTT server(%d): %s start TTTTT", getSocketDescriptor(), id_.c_str());
}

void TcpServer::bind(uint16 port, const char *ipaddress, int32 timeOutInMillsec)
{
	_serversocket = new ServerSocket();
	_serversocket->setSendBufferSize(MAX_BUFFER_SIZE_08m); //be careful
	_serversocket->setRecvBufferSize(MAX_BUFFER_SIZE_08m); //be careful
	_serversocket->setReuseAddr(true);
	_serversocket->setTimeOut(timeOutInMillsec);
	_serversocket->bind(std::string(ipaddress), port);

	std::ostringstream oss;
	IDGenerator *idGenerator = IDGeneratorMgr::getInstance();
	oss << "server: " << port << " - " << idGenerator->increase();
	this->id_ = oss.str();

	Log(LOG_DEBUG, "server(%d): %s start...", getSocketDescriptor(), id_.c_str());
}

int32 TcpServer::getSocketDescriptor()const
{
	return this->_serversocket->getSocketDescriptor();
}

void TcpServer::setCloseAndDelete(bool value)
{
	closeAndDelete_ = value;
}

bool TcpServer::closeAndDelete()
{
	return closeAndDelete_;
}

void TcpServer::doAccept()
{
	if (this->_serversocket == NULL) {
		Log(LOG_ERROR, "TcpServer::doAccept - server socket is null.");
		return ;
	}

	// accept a socket
	int32 status = -1;
	Socket* socket = this->_serversocket->accept(status);
	if (socket == NULL || status == -1) {
		if (socket != NULL) delete socket;
		Log(LOG_ERROR, "TcpServer::doAccept - create socket error.");
		return ;
	}

	// create connection
	IDGenerator *idGenerator = IDGeneratorMgr::getInstance();
	Connection *connection = new Connection(idGenerator->increase(), connHandler_, socket, true);
	this->connHandler_->add(connection);
	
	return ;
}

std::string TcpServer::uniqueIdentifier()
{
	return this->id_;
}
