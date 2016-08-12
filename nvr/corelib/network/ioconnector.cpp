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
#include "../../library/util/idgenerator.h"
#include "../../library/concurrent/thread.h"
#include "../../library/net/socket.h"
#include "../../library/util/log.h"
#include "ioconnector.h"
#include "connection.h"

using namespace library;
using namespace corelib;
using namespace corelib::MTP_NETWORK;

IOConnector::IOConnector() : connectionHandler_(NULL)
{
}

IOConnector::IOConnector(ConnectionHandler *handler) : connectionHandler_(handler)
{
}

IOConnector::~IOConnector()
{	
}

Connection *IOConnector::createConnection(const char *ip, uint16 port, int32 timeout)
{
	Socket* clientSocket = doConnect(std::string(ip), port, timeout);
	if (clientSocket != NULL) {
		return doCreateConnection(clientSocket);
	}

	return NULL;
}

Connection *IOConnector::createConnection(std::string &ip, uint16 port, int32 timeout)
{
	Socket* clientSocket =	doConnect(ip, port, timeout);
	if (clientSocket != NULL) {
		return doCreateConnection(clientSocket);
	}
	
	return NULL;
}

Socket* IOConnector::doConnect(std::string ip, uint16 port, int32 timeout)
{
	Socket* socket = new Socket();
	socket->setSendBufferSize(MAX_BUFFER_SIZE_08m); //be careful
	socket->setReceiveBufferSize(MAX_BUFFER_SIZE_08m); //be careful
	socket->setTcpNoDelay(true);
	socket->setKeepAlive(true);
	socket->setSoTimeout(timeout);
	int32 iRet = socket->connect(ip, port, timeout);
	if (iRet != 0) {
		socket->close();
		delete socket;
		return NULL;
	}

	return socket;
}

Connection *IOConnector::doCreateConnection(Socket* socket)
{	
	IDGenerator *idGenerator = IDGeneratorMgr::getInstance();
	Connection* conn = new Connection(idGenerator->increase(), connectionHandler_, socket, true);
	if (connectionHandler_ != NULL) connectionHandler_->add(conn);

	return conn;
}