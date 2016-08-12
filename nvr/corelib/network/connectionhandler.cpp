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
#include <errno.h>

#include "../../library/util/log.h"
#include "connectionhandler.h"
#include "connection.h"
#include "tcpserver.h"
#include "iohandler.h"
#include "ioevent.h"

using namespace library;
using namespace corelib;
using namespace corelib::MTP_NETWORK;

ConnectionHandler::ConnectionHandler(IOHandler *iohandler, bool bDel) : bDeleteIohandler(bDel), 
				iohandler_(iohandler), m_maxsock(-1)
{
	m_connections.clear();
	m_add.clear();
	m_delete.clear();
}

ConnectionHandler::~ConnectionHandler()
{
	if (bDeleteIohandler) delete iohandler_;
	iohandler_ = NULL;
}

void ConnectionHandler::add(IOEvent *e)
{
	if (e == NULL) return;

	if (e->getRoler() == CONNECTION) {
		Connection *p = static_cast<Connection *> (e);
		p->setIohandler(this->iohandler_);
		if (this->iohandler_ != NULL) this->iohandler_->OnConnectionOpened(p);
	}

	std::string identifier = getUniqueIdentifier(e);
	m_add.insert(std::pair<std::string, IOEvent *>(identifier, e));
}

void ConnectionHandler::remove(IOEvent *e)
{
	if (e == NULL) return;
	
	for (connection_m::iterator it = m_connections.begin(); it != m_connections.end(); ++it) {

		if (it->second == e) {
			Log(LOG_INFO, "ConnectionHandler::removeConnection - m_connections socket remove.");
			m_connections.erase(it);
		}
	}

	for (connection_m::iterator it2 = m_add.begin(); it2 != m_add.end(); ++it2) {

		if (it2->second == e) {
			Log(LOG_INFO, "ConnectionHandler::removeConnection - m_add socket remove.");
			m_add.erase(it2);
		}
	}

	for (connection_m::iterator it3 = m_delete.begin(); it3 != m_delete.end(); ++it3) {

		if (it3->second == e) {
			Log(LOG_INFO, "ConnectionHandler::removeConnection - m_delete socket remove.");
			m_delete.erase(it3);
		}
	}
}

void ConnectionHandler::IConnectionHandler_Add(IOEvent *e, bool bRead, bool bWrite)
{
	set(e, bRead, bWrite);
}

void ConnectionHandler::IConnectionHandler_Mod(IOEvent *e, bool bRead, bool bWrite)
{
	set(e, bRead, bWrite);
}

void ConnectionHandler::IConnectionHandler_Del(IOEvent *e)
{
	set(e, false, false);
}

void ConnectionHandler::set(IOEvent *e, bool bRead, bool bWrite)
{
	SOCKET s = getSocketId(e);
	if (s >= 0) {
		
		if (bRead) {
			if (!FD_ISSET(s, &m_rfds)) {
				FD_SET(s, &m_rfds);
			}
		} else {
			FD_CLR(s, &m_rfds);
		}

		if (bWrite) {
			if (!FD_ISSET(s, &m_wfds)) {
				FD_SET(s, &m_wfds);
			}
		} else {
			FD_CLR(s, &m_wfds);
		}

		bool bException = true;
		if (bException) {
			if (!FD_ISSET(s, &m_efds)) {
				FD_SET(s, &m_efds);
			}
		} else {
			FD_CLR(s, &m_efds);
		}
	}
}

int32 ConnectionHandler::select()
{
	return select(NULL);
}

int32 ConnectionHandler::select(int32 sec, int32 usec)
{
	struct timeval tv;
	tv.tv_sec = sec;
	tv.tv_usec = usec;
	return select(&tv);
}

int32 ConnectionHandler::select(struct timeval *tsel)
{
	if (m_add.size() > 0) {
		addIncoming();
	}

	checkTimeOut();

	checkClose();

	flushDeleteConnection();

	IConnectionHandler_Select(tsel);

	return 0;
}

int32 ConnectionHandler::IConnectionHandler_Select(struct timeval *tsel)
{
	fd_set rfds = m_rfds;
	fd_set wfds = m_wfds;
	fd_set efds = m_efds;
	int32 n;

	n  = ::select(int32 (m_maxsock + 1), &rfds, &wfds, &efds, tsel);
	if (n == -1) { //error on select
		int32 err = errno;
		/*
			EBADF  An invalid file descriptor was given in one of the sets.
			EINTR  A non blocked signal was caught.
			EINVAL n is negative. Or struct timeval contains bad time values (<0).
			ENOMEM select was unable to allocate memory for internal tables.
		*/
		switch(err) {
			case EBADF:
				rebuildFdset();
				break;
			case EINTR:
				break;
			case EINVAL:
				Log(LOG_ERROR, "ConnectionHandler::IConnectionHandler_Select - timeval is bad, %s.", strerror(err));
				break;
			case ENOMEM:
				Log(LOG_ERROR, "ConnectionHandler::IConnectionHandler_Select - %s.", strerror(err));
				break;
		}

//		Log(LOG_WARN, "ConnectionHandler::IConnectionHandler_Select - select %s.", strerror(err));
	} else {

		if (n > 0) {
			for (connection_m::iterator it = m_connections.begin(); it != m_connections.end(); ++it) {
				std::string identifier = it->first;
				IOEvent *e = it->second;
				SOCKET s = getSocketId(e);
				
				if (FD_ISSET(s, &rfds))
				{
					e->handle(identifier, SO_READ);
					//if (e->getRoler() == CONNECTION) {
					//	this->iohandler_->OnConnectionReceive(static_cast<Connection *>(e));
					//}
				}

				if (FD_ISSET(s, &wfds))
				{
					e->handle(identifier, SO_WRITE);
				}

				if (FD_ISSET(s, &efds))
				{
					;//e->handle(s, SO_e);
				}
			}
		}
	}

	return n;
}

void ConnectionHandler::rebuildFdset()
{
	fd_set rfds;
	fd_set wfds;
	fd_set efds;
	
	// rebuild fd_set's from active sockets list (m_sockets) here
	FD_ZERO(&rfds);
	FD_ZERO(&wfds);
	FD_ZERO(&efds);

	for (connection_m::iterator it = m_connections.begin(); it != m_connections.end(); ++it) {
		
		SOCKET s = -1;
		
		IOEvent *e = it->second;
		s = getSocketId(e);
		if (s >= 0) {
			fd_set fds;
			struct timeval tv;

			FD_ZERO(&fds);
			FD_SET(s, &fds);
			tv.tv_sec = 0;
			tv.tv_usec = 0;
			int32 n = ::select((int32)s + 1, &fds, NULL, NULL, &tv);
			if (n == -1 && errno == EBADF) {
//				Log(LOG_WARN, "ConnectionHandler::rebuildFdset - select, bad fd:%d in m_connections.", s);
				deleteConnection(e);
			} else {
				Log(LOG_DEBUG, "ConnectionHandler::rebuildFdset - select, add fd:%d in m_connections.", s);
				if (FD_ISSET(s, &m_rfds))
					FD_SET(s, &rfds);
				if (FD_ISSET(s, &m_wfds))
					FD_SET(s, &wfds);
				if (FD_ISSET(s, &m_efds))
					FD_SET(s, &efds);
			}
		} else {
//			Log(LOG_DEBUG, "ConnectionHandler::rebuildFdset - bad fd:%d in m_connections.", s);
			deleteConnection(e);
		}
	}

	m_rfds = rfds;
	m_wfds = wfds;
	m_efds = efds;
}

void ConnectionHandler::addIncoming()
{
	while (m_add.size() > 0) {

		// one loop for max 6000 connection
		if (m_connections.size() >= 6000){
			Log(LOG_WARN, "ConnectionHandler::addIncoming - socket max limit: 6000 reached.");
			break;
		}

		connection_m::iterator it = m_add.begin();
		IOEvent *e = it->second;
		std::string identifier;
		SOCKET s = -1;
		bool bCloseAndDelete = false;
		
		if (e->getRoler() == SERVER) {
			TcpServer *server = static_cast<TcpServer *>(e);
			identifier = server->uniqueIdentifier();
			s = server->getSocketDescriptor();
			bCloseAndDelete = server->closeAndDelete();
		} else {
			Connection *p = static_cast<Connection *>(e);
			identifier = p->uniqueIdentifier();
			s = p->getSocketDescriptor();
			bCloseAndDelete = p->closeAndDelete();
		}
		
		if (s == -1) {
			Log(LOG_ERROR, "ConnectionHandler::addIncoming - INVALID socket.");
			deleteConnection(e);
			m_add.erase(it);
			continue;
		}

		connection_m::iterator it2;
		if ((it2 = m_connections.find(identifier)) != m_connections.end()) {

			IOEvent* found = it2->second;
			if (e == found) {
				Log(LOG_WARN, "ConnectionHandler::addIncoming - same connection.");
				// same - ignore
			} else {
				// other close
				Log(LOG_ERROR, "ConnectionHandler::addIncoming - >< connection.");
				deleteConnection(e);
			}

			m_add.erase(it);
			continue;
		}

		if (bCloseAndDelete) {
			Log(LOG_ERROR, "ConnectionHandler::addIncoming - CloseAndDelete Connection.");
			m_connections[identifier] = e;
		} else {
			if (e->getRoler() != SERVER) {
				Connection *p = static_cast<Connection *>(e);
				bool bWrite = p ? p->getOutputLength() != 0 : false;
				if (p->isInputClosed()) {
					IConnectionHandler_Add(e, false, bWrite);
				} else {
					IConnectionHandler_Add(e, true, bWrite);
				}
			} else {
				IConnectionHandler_Add(e, true, false);
			}
			
			m_maxsock = (s > m_maxsock) ? s : m_maxsock;
			m_connections[identifier] = e;
		}

		m_add.erase(it);
	}
}

void ConnectionHandler::checkTimeOut()
{
	time_t now = time(NULL);

	connection_m::iterator it = m_connections.begin();
	for (; it != m_connections.end(); ++it) {

		IOEvent *e = it->second;
		if (e->getRoler() == SERVER)
			continue;

		Connection* p = static_cast<Connection *> (e);
		std::string identifier = p->uniqueIdentifier();
		if (p->isTimedOut(now)) {
			this->iohandler_->OnConnectionTimedOut(p);
			p->handle(identifier, SO_TIMED);
		}
	}
}

void ConnectionHandler::checkClose()
{
	connection_m::iterator it = m_connections.begin();
	for (; it != m_connections.end(); ++it) {

		IOEvent *e = it->second;
		if (e->getRoler() == SERVER) {
			TcpServer *server = static_cast<TcpServer *>(e);
			if (server->closeAndDelete()) {
				deleteConnection(e);
			}
			continue;
		}

		//connection
		Connection *p = static_cast<Connection *>(e);
		if (p->closeAndDelete()) {
			if (!p->flushoutBeforeClose())
			{
				deleteConnection(e);
			}
			else // graceful tcp close
			{
				// no data
				if (p->getOutputLength()== 0) {
					deleteConnection(e);
					continue;
				}
			}
			IConnectionHandler_Del(e);
		}
	}
}

void ConnectionHandler::deleteConnection(IOEvent * e)
{
	if (e == NULL) return;

	std::string identifier = getUniqueIdentifier(e);
	connection_m::iterator it = m_delete.find(identifier);
	if (it == m_delete.end())
		m_delete.insert(std::pair<std::string, IOEvent*>(identifier, e));
}

// Connection always deleted by ConnectionHandler, user should set the 'deleteByHandler' when close
void ConnectionHandler::flushDeleteConnection()
{
	for (connection_m::iterator it = m_delete.begin(); it != m_delete.end(); ) {
		std::string identifier = it->first;
		IOEvent *e = it->second;
		bool bErased = false;

		connection_m::iterator it2 = m_connections.find(identifier);
		if (it2 != m_connections.end()) {
			m_connections.erase(it2);
		}

		//tcpserver handler must be deleted by user
		if (e->getRoler() == SERVER) {
			e->handle(identifier, SO_CLOSE);
			bErased = true;
		} else {
			Connection *p = static_cast<Connection *> (e);
			e->handle(identifier, SO_CLOSE);
			Log(LOG_INFO, "-----------------------------------------------------------------");
			this->iohandler_->OnConnectionClosed(p);
			if (p->deleteByHandler()) {
				delete p;
				bErased = true;
			}
		}

		if (bErased) {
			m_delete.erase(it++);
		} else {
			it++;
		}
	}

	m_maxsock = 0;
	connection_m::iterator i = m_connections.begin();
	for (; i != m_connections.end(); ++i) {
		SOCKET s = getSocketId(i->second);
		m_maxsock = s > m_maxsock ? s : m_maxsock;
	}
}

int32 ConnectionHandler::getSocketId(IOEvent *e)
{
	if (e == NULL) return -1;

	SOCKET s = -1;
	if (e->getRoler() == SERVER) {
		TcpServer *server = static_cast<TcpServer *>(e);
		s = server->getSocketDescriptor();
	} else {
		Connection *p = static_cast<Connection *> (e);
		s = p->getSocketDescriptor();
	}

	return s;
}

std::string ConnectionHandler::getUniqueIdentifier(IOEvent *e)
{
	if (e == NULL) return "";

	std::string identifier;
	if (e->getRoler() == SERVER) {
		TcpServer *server = static_cast<TcpServer *>(e);
		identifier = server->uniqueIdentifier();
	} else {
		Connection *p = static_cast<Connection *> (e);
		identifier = p->uniqueIdentifier();
	}

	return identifier;
}