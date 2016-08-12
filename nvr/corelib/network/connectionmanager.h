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
#ifndef __CORELIB_NETWORK_CONNECTIONMANAGER_H__
#define __CORELIB_NETWORK_CONNECTIONMANAGER_H__

#include <string>
#include <map>

#include "../../library/concurrent/lock.h"

namespace corelib {
namespace protocol {
	class DataStructure;
}
}

namespace corelib {
namespace MTP_NETWORK {

class Connection;
class Request;
class ConnectionManager
{
	private:

		ConnectionManager(const ConnectionManager&);

		ConnectionManager& operator= (const ConnectionManager&);

	public:

		ConnectionManager();

		~ConnectionManager();

	public:

		void addConnection(Connection *connection);

		Connection* delConnection(std::string &connId);

		Connection* delConnection(Connection *connection);

		bool findConnectionIfExisted(std::string &connId);

		Connection *getConnection(std::string &connId);

		void flushClosedConnection();

	public:

		bool dispatch(std::string &connId, const char* data, int32 len);

		void dispatchAll(const char* data, int32 len);

	private:

		library::Lock mutex;
		std::map<std::string, Connection *> connectionMap; //ConnId <-> Connection
};

}
}

#endif /* end of file */