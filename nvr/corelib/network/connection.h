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
#ifndef __CORELIB_NETWORK_CONNECTION_H__
#define __CORELIB_NETWORK_CONNECTION_H__

#include <string>

#include "../../library/util/idgenerator.h"
#include "../../library/concurrent/lock.h"
#include "../protocol/datastructure.h"
#include "connectionhandler.h"
#include "socketcommondef.h"
#include "ioevent.h"

namespace library {

class Socket;
class InputStream;
class OutputStream;
}

namespace corelib {
namespace protocol {
	class DataStructure;
}
}

namespace corelib {
namespace MTP_NETWORK {

class ConnectionInfo;
class SimpleConnection;
class Transport;
class IOHandler;
////////////////////////////////////////////////////////////////////////////////
//NOTE: Connection is not thread safe, user should take @ConnectionManager to keep
//      thread safe
class Connection : public IOEvent
{
	friend class ConnectionInfo;
	private:

		Connection(const Connection &);
		Connection& operator= (const Connection &);

	public:

		Connection(uint64 connId, library::Socket *socket, bool own = true);
		Connection(uint64 connId, ConnectionHandler*, library::Socket *socket, bool own = true);
		virtual ~Connection();

	public: //inheritDoc

		virtual void handle(std::string& socketId, SOCKETOP op);

	public: //client

		int32 bind(const std::string &localIpaddress, int16 localPort);
		int32 connect(const std::string &remoteIpaddress, int16 remotePort);
		int32 connect(const std::string &remoteIpaddress, int16 remotePort, int32 timeout);
		int32 reConnect(const std::string &remoteIpaddress, int16 remotePort);
		int32 reConnect(const std::string &remoteIpaddress, int16 remotePort, int32 timeout);

	public:

		bool send(const char *format, ...); //with synchrosize mode
		bool send(const std::string &str, bool synwriting = true);
		bool send(const char* buf, size_t len, bool synwriting = true);
		size_t readInput(char *buf, size_t size);
		size_t peekInput(char *buf, size_t size);

		size_t getInputLength();
		size_t getOutputLength();

		uint64 getBytesReceived(bool clear = false);
		uint64 getBytesSended(bool clear = false);

	public:

		std::string uniqueIdentifier()const;
		uint16 getLocalPort()const;
		uint16 getRemotePort()const;
		std::string getLocalIpAddress()const;
		std::string getRemoteIpAddress() const;
		int32 getSocketDescriptor()const;

	public:

		void setCloseAndDelete(bool value = false);
		bool closeAndDelete();

		void setFlushoutBeforeClose(bool value = false);
		bool flushoutBeforeClose();

		void setDeleteByHandler(bool value = true);
		bool deleteByHandler();

		void setDisableInputBuffer(bool value = false);
		bool disableInputBuffer();

		void setCurrentTime();
		bool isTimedOut(time_t now);

 		void setIohandler(IOHandler *handler);

		void shutdown();
		bool isClosed();
		bool isInputClosed();
		bool isOutputClosed();

		uint64 getSeqNum() {return seqNumber_++;};

	private:
		
 		void init(uint64 connId, Transport *transport, library::Socket *socket);
		void finish();

		void handleRead();
		void handleWrite();

		int32 readData(char *data, int32 len);
		int32 writeData(const char* data, int32 len, bool synwriting);
		void Buffer(const char* buf, size_t len);

		void close();
		void closeInput();
		void closeOutput();

	private:

		bool ownedSocket;
		library::Lock mutex_;
		IOHandler *iohandler_;
		library::Socket *socket_;
		ConnectionInfo *connInfo_;
		ConnectionHandler *connectionHandler_;

		//
		uint64 seqNumber_;
};

}
}

#endif /* end of file */
