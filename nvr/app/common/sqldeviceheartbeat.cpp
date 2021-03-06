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
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include "sqldeviceheartbeat.h"
#include "../../library/util/log.h"
#include "../../library/util/system.h"
#include "../../corelib/database/databaseoracle/myoracle.h"
#include "../../corelib/database/databaseoracle/databaseoracle.h"

using namespace library;
using namespace corelib::database;
using namespace app;

SqlDeviceHeartBeat::SqlDeviceHeartBeat(std::list<std::string> &dl) : deviceList_(dl)
{
	m_device.resize(18);
}

SqlDeviceHeartBeat::~SqlDeviceHeartBeat()
{
}

void SqlDeviceHeartBeat::Execute(CD::Database *db)
{
	if (db == NULL) return;

	DatatbaseOracle *mydb = static_cast<DatatbaseOracle*>(db);
	while (deviceList_.size() > 0) {
		m_device = deviceList_.front();
		deviceList_.pop_front();
		mydb->execute(formatDeviceHeartbeat().c_str());
	}
	mydb->execute("commit");
}

std::string SqlDeviceHeartBeat::formatDeviceHeartbeat()
{
	std::string sql;

	sql = "INSERT INTO devicestate_tab(ZTXH, SBBH, SCSJ, DQZT, GZSJ, GZZT1, GZZT2, GZZT3, GZZT4, ZTSJ) VALUES (";
	sql += "seq_devicestate_tab.nextval, ";
	sql += m_device;
	sql += ", sysdate, '0', sysdate, NULL, NULL, NULL, NULL, sysdate)";

	return sql;
}