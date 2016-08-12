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
#ifndef __CORELIB_NETWORK_CONNECTIONEPOLL_H__
#define __CORELIB_NETWORK_CONNECTIONEPOLL_H__

#include <sys/epoll.h>

#include "connectionhandler.h"

#define MAX_EVENTS_EP_WAIT 100

namespace corelib {
namespace MTP_NETWORK {

class ConnectionHandlerEp : public ConnectionHandler
{
	private:

		ConnectionHandlerEp(const ConnectionHandlerEp&);

		ConnectionHandlerEp& operator= (const ConnectionHandlerEp&);

	public:

		ConnectionHandlerEp(IOHandler *iohandler, bool bDel = true);

		~ConnectionHandlerEp();

	public:

		virtual void IConnectionHandler_Add(IOEvent *, bool bRead, bool bWrite);
		virtual void IConnectionHandler_Mod(IOEvent *, bool bRead, bool bWrite);
		virtual void IConnectionHandler_Del(IOEvent *);

	protected:

		virtual int32 IConnectionHandler_Select(struct timeval *);

	private:

		int32 m_epoll;
		struct epoll_event m_events[MAX_EVENTS_EP_WAIT];
};

}
}

#endif /* end of file */