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
#ifndef __APP_COMMON_SQLDEVICEHEARTBEAT_H__
#define __APP_COMMON_SQLDEVICEHEARTBEAT_H__

#include <list>
#include <string>

#include "eprotocol.h"
#include "../../corelib/database/common/sqloperation.h"
#include "../../corelib/database/databaseoracle/ocilib.h"

namespace app {
namespace CD = corelib::database;
class SqlDeviceHeartBeat : public CD::SqlOperation
{
	private:

		SqlDeviceHeartBeat(const SqlDeviceHeartBeat&);
		SqlDeviceHeartBeat& operator= (const SqlDeviceHeartBeat&);

	public:

		SqlDeviceHeartBeat(std::list<std::string> &dl);
		~SqlDeviceHeartBeat();

	public:

		virtual void Execute(CD::Database *db);

	private:

		std::string formatDeviceHeartbeat();
		
	private:

		std::string m_device;
		std::list<std::string> deviceList_;
};

}

#endif /* end of file */