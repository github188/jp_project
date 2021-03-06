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
#ifndef __CORELIB_NETWORK_TCPSERVER_H__
#define __CORELIB_NETWORK_TCPSERVER_H__

#include "../../common/basetype.h"
#include "socketcommondef.h"
#include "ioevent.h"

namespace library {

	class ServerSocket;
}

namespace corelib {
namespace MTP_NETWORK {

class ConnectionHandler;
class Connection;
class IOHandler;
class TcpServer : public IOEvent
{
	public:

		TcpServer(ConnectionHandler *handler);

		~TcpServer();

	public:

		virtual void handle(std::string& socketId, SOCKETOP op);

	public:

		void bind(uint16 port, int32 timeOutInMillsec = -1);

		void bind(uint16 port, const char *ipaddress, int32 timeOutInMillsec = -1);

		int32 getSocketDescriptor()const;

		void setCloseAndDelete(bool value = false);

		bool closeAndDelete();

		std::string uniqueIdentifier();

	protected:
		
		void doAccept();

	private:

		std::string id_;
		bool closeAndDelete_;
		ConnectionHandler *connHandler_;
		library::ServerSocket *_serversocket;
};

}
}
#endif /* end of file */