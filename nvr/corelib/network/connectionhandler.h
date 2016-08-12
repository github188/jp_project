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
#ifndef __CORELIB_NETWORK_CONNECTIONHANDLER_H__
#define __CORELIB_NETWORK_CONNECTIONHANDLER_H__

#include <list>
#include <map>
#include <string>

#include "../../common/basetype.h"
#include "../../library/concurrent/lock.h"

namespace corelib {
namespace MTP_NETWORK {

class IOHandler;
class IOEvent;
class ConnectionHandler
{
	private:

		ConnectionHandler(const ConnectionHandler&);
		ConnectionHandler& operator= (const ConnectionHandler&);

	protected:

		typedef int32 SOCKET;
		typedef std::map<std::string, IOEvent*> connection_m;

	public:

		ConnectionHandler(IOHandler *iohandler, bool bDel = true);
		virtual ~ConnectionHandler();

	public:

		void add(IOEvent*);
		void remove(IOEvent *);

		virtual void IConnectionHandler_Add(IOEvent *, bool bRead, bool bWrite);
		virtual void IConnectionHandler_Mod(IOEvent *, bool bRead, bool bWrite);
		virtual void IConnectionHandler_Del(IOEvent *);

		/** This method will not return until an event has been detected. */
		int32 select();		
		int32 select(int32 sec, int32 usec);
		int32 select(struct timeval *tsel);

		IOHandler *getIohandler() {return this->iohandler_;};

	private:

		void rebuildFdset();
		void set(IOEvent *, bool, bool);

	protected:

		virtual int32 IConnectionHandler_Select(struct timeval *);
		void addIncoming();
		void checkTimeOut();
		void checkClose();
		void deleteConnection(IOEvent *);
		void flushDeleteConnection();
		int32 getSocketId(IOEvent *);
		std::string getUniqueIdentifier(IOEvent *e);

	protected:

		bool bDeleteIohandler;
		IOHandler *iohandler_;
		connection_m m_connections; // active connection map
		connection_m m_add; //< connection to be added to sockets map
		connection_m m_delete; //connection to be deleted

	private:

		fd_set m_rfds;
		fd_set m_wfds;
		fd_set m_efds;
		SOCKET m_maxsock;
};

}
}

#endif /* end of file */