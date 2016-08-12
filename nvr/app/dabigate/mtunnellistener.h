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
#ifndef __APP_DABIGATE_MTUNNELLISTENER_H__
#define __APP_DABIGATE_MTUNNELLISTENER_H__

#include "../common/dbservice.h"
#include "../../corelib/network/listener.h"
#include "../../corelib/network/connectionmanager.h"

namespace corelib {
namespace protocol {

	class DataStructure;
	class Format;
}
}

namespace app {

namespace CP = corelib::protocol;
namespace CM = corelib::MTP_NETWORK;
class MTunnelListener : public CM::Listener
{
	private:

		MTunnelListener(const MTunnelListener&);
		MTunnelListener& operator= (const MTunnelListener&);

	public:

		MTunnelListener(DbService &dbService);
		~MTunnelListener();

	public: //inheritDoc

		virtual void onStart(CM::Connection *connection);
		virtual void onCommand(CM::Connection *connection, const char *data, int32 len);
		virtual void onEnd(CM::Connection *connection);

	public:

		void setWireFormat(CP::Format *format);

	private:

		uint64 timeInSecs_;
		DbService &m_dbService;
		CP::Format *wireformat_;		
		CM::ConnectionManager connectionManager_;
};

}

#endif /* end of file */