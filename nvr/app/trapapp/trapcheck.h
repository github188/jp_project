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
#ifndef __APP_TRAPAPP_TRAPCHECK_H__
#define __APP_TRAPAPP_TRAPCHECK_H__

#include <string>
#include "../common/mqservice.h"
#include "../common/whitelist.h"
#include "../common/blacklist.h"
#include "../common/controllist.h"
#include "../../corelib/network/listener.h"
#include "../../corelib/database/databaseoracle/databaseoracle.h"

namespace corelib {
namespace MTP_NETWORK {

	class Connection;
}

namespace protocol {

	class Format;
}
}

namespace app {

namespace CP = corelib::protocol;
namespace CM = corelib::MTP_NETWORK;

using corelib::database::DatatbaseOracle;
using corelib::database::SqlResultQueue;
using corelib::database::QueryResult;
class CarBypassDescInfo;
class TrapCheck : public CM::Listener 
{
	private:

		TrapCheck(const TrapCheck&);
		TrapCheck& operator= (const TrapCheck&);

	public:

		TrapCheck(DatatbaseOracle &dbEngine);
		~TrapCheck();

	public: //inheritDoc

		virtual void onStart(CM::Connection *connection);
		virtual void onCommand(CM::Connection *connection, const char *data, int32 len);
		virtual void onEnd(CM::Connection *connection);

	public:

		//command from mq to update whitelist/blacklist/controllist
		void onUpdateTrapConfig(std::string cmd);

		//protocol
		void setWireFormat(CP::Format *format);

	private:

		//init for whitelist/blacklist/controllist
		void trapConfigInit();

		void getWhiteList();
		void handleWhiteList(QueryResult *result);
		
		void getBlackList();
		void handleBlackList(QueryResult *result);
		
		void getControlList();
		void handleControlList(QueryResult *result);

		void getDevConfigInfo();
		void handleDevConfigInfo(QueryResult *result);
		
		void checkAndNotify(CarBypassDescInfo *carInfo);

	private:

		WhiteList m_white;
		BlackList m_black;
		ControlList m_control;
		MQService m_mqService;
		CP::Format *wireformat_;
		DatatbaseOracle &m_dbEngine;
};

}

#endif /* end of file */