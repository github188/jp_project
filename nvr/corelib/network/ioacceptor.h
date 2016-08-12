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
#ifndef __CORELIB_NETWORK_IOACCEPTOR_H__
#define __CORELIB_NETWORK_IOACCEPTOR_H__

#include <string>
#include <map>

#include "../../library/common/runnable.h"
#include "socketcommondef.h"

namespace library {

	class Thread;
}

namespace corelib {
namespace MTP_NETWORK{

class ConnectionHandler;
class TcpServer;
class IOHandler;
class IOAcceptor : public library::Runnable
{
	private:

		IOAcceptor(const IOAcceptor&);

		IOAcceptor& operator= (const IOAcceptor&);

	public:

		IOAcceptor(ConnectionHandler *selector);

		IOAcceptor(IOHandler *handler, bool bDelete = true);

		virtual ~IOAcceptor();

	public: //@inheritDoc

		virtual void run();

		virtual bool bNeedDestroyed()const {return false;};

	public:

		void start();

		void stop();

		void bind(uint16 port, int32 timeOutInMillsec = -1);

		void bind(uint16 port, const char *ipaddress, int32 timeOutInMillsec = -1);

		void setConnectionHandler(ConnectionHandler *selector, bool own = true);

	private:

		bool go;
		TcpServer *_server;
		library::Thread *thread;
		bool bOwnConnectionHandler_;
		ConnectionHandler *selector_;
};

}
}

#endif /* end of file */
