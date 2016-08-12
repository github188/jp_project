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
#include <string.h>
#include <errno.h>

#include "../../library/util/log.h"
#include "connectionhandlerepoll.h"
#include "connection.h"
#include "tcpserver.h"
#include "ioevent.h"

using namespace library;
using namespace corelib;
using namespace corelib::MTP_NETWORK;

ConnectionHandlerEp::ConnectionHandlerEp(IOHandler *iohandler, bool bDel) : ConnectionHandler(iohandler, bDel), m_epoll(-1)
{
	m_epoll = epoll_create(FD_SETSIZE);
	if (m_epoll == -1) {
		Log(LOG_ERROR, "ConnectionHandlerEp::ConnectionHandlerEp - error: %s", strerror(errno));
	}
}

ConnectionHandlerEp::~ConnectionHandlerEp()
{
	if (m_epoll != -1) {
		close(m_epoll);
	}
}

void ConnectionHandlerEp::IConnectionHandler_Add(IOEvent *e, bool bRead, bool bWrite)
{
	struct epoll_event stat;
	SOCKET s = getSocketId(e);

	stat.data.ptr = e;
	stat.events = (bRead ? EPOLLIN : 0) | (bWrite ? EPOLLOUT : 0);
	if (epoll_ctl(m_epoll, EPOLL_CTL_ADD, s, &stat) == -1) {
		Log(LOG_ERROR, "ConnectionHandlerEp::IConnectionHandler_Add - EPOLL_CTL_ADD, %s.", strerror(errno));
	}
}

void ConnectionHandlerEp::IConnectionHandler_Mod(IOEvent *e, bool bRead, bool bWrite)
{
	struct epoll_event stat;
	SOCKET s = getSocketId(e);
	
	stat.data.ptr = e;
	stat.events = (bRead ? EPOLLIN : 0) | (bWrite ? EPOLLOUT : 0);
	if (epoll_ctl(m_epoll, EPOLL_CTL_MOD, s, &stat) == -1) {
		Log(LOG_ERROR, "ConnectionHandlerEp::IConnectionHandler_Mod - EPOLL_CTL_MOD, %s.", strerror(errno));
	}
}

void ConnectionHandlerEp::IConnectionHandler_Del(IOEvent *e)
{
	struct epoll_event stat;
	SOCKET s = getSocketId(e);

	stat.events = 0;
	stat.data.ptr = e;
	if (epoll_ctl(m_epoll, EPOLL_CTL_DEL, s, &stat) == -1) {
		Log(LOG_ERROR, "ConnectionHandlerEp::IConnectionHandler_Del - EPOLL_CTL_DEL, %s.", strerror(errno));
	}
}

int32 ConnectionHandlerEp::IConnectionHandler_Select(struct timeval *tsel)
{
	int32 n = -1;

	n = epoll_wait(m_epoll, m_events, MAX_EVENTS_EP_WAIT, tsel ? tsel->tv_sec * 1000 + tsel->tv_usec / 1000 : -1);
	if (n == -1) {
		Log(LOG_ERROR, "ConnectionHandlerEp::IConnectionHandler_Select - epoll_wait, %s.", strerror(errno));
	} else {
		if (n > 0) {
			for (int32 x = 0; x < n; x++) {
				IOEvent *p = static_cast<IOEvent *>(m_events[x].data.ptr);
				std::string identifier = getUniqueIdentifier(p);
				
				if ((m_events[x].events & EPOLLIN) || (m_events[x].events & EPOLLHUP))
				{
					p->handle(identifier, SO_READ);
				}

				if (m_events[x].events & EPOLLOUT)
				{
					p->handle(identifier, SO_WRITE);
				}

				if (m_events[x].events & EPOLLERR)
				{
					;
				}
			}
		}
	}

	return n;
}