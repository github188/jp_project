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
#include "../../library/concurrent/reentrant.h"
#include "../../library/util/log.h"
#include "connectionmanager.h"
#include "connection.h"

using namespace library;
using namespace corelib;
using namespace corelib::MTP_NETWORK;

////////////////////////////////////////////////////////////////////////////////
ConnectionManager::ConnectionManager() : mutex(), connectionMap()
{
}

ConnectionManager::~ConnectionManager()
{
}

void ConnectionManager::addConnection(Connection *connection)
{
	std::map<std::string, Connection *>::iterator iter;
	
	Synchronization autolock(mutex);
	
	if (connection == NULL) {
		Log(LOG_INFO, "ConnectionManager::addConnection - input is null.");
		return;
	}

	if (connection->isClosed()) {
		Log(LOG_INFO, "ConnectionManager::addConnection - connection is closed.");
		return;
	}

	std::string connId = connection->uniqueIdentifier();
	iter = connectionMap.find(connId);
	if (iter != connectionMap.end()) {
		Log(LOG_INFO, "ConnectionManager::addConnection - connection already in.");
		return;
	}

	connectionMap.insert(std::pair<std::string, Connection *>(connId, connection));
}

Connection* ConnectionManager::delConnection(std::string &connId)
{
	std::map<std::string, Connection *>::iterator iter;
	Connection *connection = NULL;
	
	Synchronization autolock(mutex);

	iter = connectionMap.find(connId);
	if (iter == connectionMap.end()) {
		Log(LOG_INFO, "ConnectionManager::delConnection - connection is removed.");
		return NULL;
	}

	connection = iter->second;
	connectionMap.erase(iter);

	return connection;
}

Connection* ConnectionManager::delConnection(Connection *connection)
{
	std::map<std::string, Connection *>::iterator iter;
	
	Synchronization autolock(mutex);
	
	if (connection == NULL) {
		Log(LOG_INFO, "ConnectionManager::delConnection - input is null.");
		return NULL;
	}

	std::string connId = connection->uniqueIdentifier();
	iter = connectionMap.find(connId);
	if (iter == connectionMap.end()) {
		Log(LOG_INFO, "ConnectionManager::delConnection - connection is removed.");
		return NULL;
	}

	connectionMap.erase(iter);

	return connection;
}

bool ConnectionManager::findConnectionIfExisted(std::string &connId)
{
	std::map<std::string, Connection *>::iterator iter;

	Synchronization autolock(mutex);
	
	iter = connectionMap.find(connId);
	if (iter == connectionMap.end())
		return false;

	return true;
}

Connection *ConnectionManager::getConnection(std::string &connId)
{
	std::map<std::string, Connection *>::iterator iter;
	
	Synchronization autolock(mutex);

	iter = connectionMap.find(connId);
	if (iter != connectionMap.end())
		return iter->second;

	return NULL;
}

void ConnectionManager::flushClosedConnection()
{
	std::map<std::string, Connection *>::iterator iter;
	Connection *connection = NULL;
	
	Synchronization autolock(mutex);

	iter = connectionMap.begin();
	for ( ; iter != connectionMap.end(); ) {

		connection = iter->second;
		if (connection && connection->isClosed()) {
			connectionMap.erase(iter++);
			iter = connectionMap.begin();
		} else {
			++iter;
		}
		connection = NULL;
	}
}

bool ConnectionManager::dispatch(std::string &connId, const char* data, int32 len)
{
	Synchronization autolock(mutex);
	
	if (data == NULL || len <= 0) {
		Log(LOG_INFO, "ConnectionManager::dispatch - input is invalid.");
		return false;
	}

	std::map<std::string, Connection *>::iterator iter;
	Connection *connection = NULL;
	bool succeed = false;

	iter = connectionMap.find(connId);
	if (iter != connectionMap.end()) {
		connection = iter->second;
		if (connection) {
			succeed = connection->send(data, len, false);
			if (connection->getOutputLength() > MAX_BUFFER_SIZE_16m) {
				Log(LOG_WARN, "%s receive too slow, remain <%llu> bytes", connId.c_str(), connection->getOutputLength());
				connection->setCloseAndDelete(true);
				return false;
			}
		}
	}

	return succeed;
}

void ConnectionManager::dispatchAll(const char* data, int32 len)
{
	Synchronization autolock(mutex);
	
	if (data == NULL || len <= 0) {
		Log(LOG_INFO, "ConnectionManager::dispatchAll - input is invalid.");
		return;
	}

	std::map<std::string, Connection *>::iterator iter;
	Connection *connection = NULL;

	iter = connectionMap.begin();
	for (; iter != connectionMap.end(); ++iter) {
		connection = iter->second;
		if (connection) {
			connection->send(data, len, false);
		}
		connection = NULL;
	}
}