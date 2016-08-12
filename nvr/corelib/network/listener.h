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
#ifndef __CORELIB_NETWORK_LISTENER_H__
#define __CORELIB_NETWORK_LISTENER_H__

#include "../../common/basetype.h"

namespace corelib {

namespace protocol {
	class DataStructure;
}

namespace MTP_NETWORK {

class Connection;
class Request;
class Listener
{
	private:

		Listener(const Listener&);

		Listener& operator= (const Listener&);

	public:

		Listener();

		virtual ~Listener();

	public:

//		virtual void onCommand(Connection *connection, Request*) = 0;

		virtual void onStart(Connection *connection)= 0;

		virtual void onCommand(Connection *connection, const char *, int32 len) = 0;

		virtual void onEnd(Connection *connection) = 0;

//		virtual void onSuccess(Connection *connection, corelib::protocol::DataStructure *) = 0;
};

}
}

#endif /* end of file */