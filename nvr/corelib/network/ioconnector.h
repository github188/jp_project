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
#ifndef __CORELIB_NETWORK_IOCONNECTOR_H__
#define __CORELIB_NETWORK_IOCONNECTOR_H__

#include <string>

#include "connectionhandler.h"
#include "socketcommondef.h"

namespace library {

	class Socket;
}

namespace corelib {
namespace MTP_NETWORK {

class ClientConnectionHandler;
class ConnectionHandler;
class Connection;
class IOConnector
{
	private:

		IOConnector(const IOConnector&);

		IOConnector& operator= (const IOConnector&);

	public:

		IOConnector();
		
		IOConnector(ConnectionHandler *handler);

		~IOConnector();

	public:

		Connection *createConnection(const char *ip, uint16 port, int32 timeout = -1);

		Connection *createConnection(std::string &ip, uint16 port, int32 timeout = -1);

	public:

		void setConnectionHandler(ConnectionHandler *h) {connectionHandler_ = h;};

	private:

		library::Socket* doConnect(std::string ip, uint16 port, int32 timeout);

		Connection *doCreateConnection(library::Socket *socket);

	private:

		ConnectionHandler *connectionHandler_;
};

}
}

#endif /* end of file */