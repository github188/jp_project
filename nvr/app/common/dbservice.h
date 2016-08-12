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
#ifndef __APP_COMMON_DBSERVICE_H__
#define __APP_COMMON_DBSERVICE_H__

#include <list>

#include "../common/eprotocol.h"
#include "../../library/common/runnable.h"
#include "../../library/concurrent/lock.h"
#include "../../corelib/database/databaseoracle/databaseoracle.h"

namespace library {

	class Thread;
}

namespace app {

using corelib::database::DatatbaseOracle;
using corelib::database::SqlResultQueue;
using corelib::database::QueryResult;
class DbService : public library::Runnable
{
	typedef std::list<ST_CAR_DESCRIPTION_INFO*> CAR_INFO_LIST;
	typedef std::list<std::string> DEVICE_HEARTBEAT_LIST;
	private:

		DbService(const DbService&);
		DbService& operator= (const DbService&);

	public:

		DbService(DatatbaseOracle &dbEngine);
		~DbService();

	public: //inheritDoc

		virtual void run();
		virtual bool bNeedDestroyed() const {return false;};

	public:

		void init();
		void insert(ST_CAR_DESCRIPTION_INFO* carInfo);
		void insertDevHeartBeat(std::string deviceSn);

	private:

		void getCjConfigInfo();
		void handleCjConfigInfo(QueryResult *result);
		void getCbConfigInfo();
		void handleCbConfigInfo(QueryResult *result);
		void getDevConfigInfo();
		void handleDevConfigInfo(QueryResult *result);
		void handleCarRecords();
		void handleDeviceHeartBeatInfo();

		std::string generateXxbh(ST_CAR_DESCRIPTION_INFO* carInfo);
		void flushTimedoutXxbh();
		
	private:

		library::Lock m_lock;
		SqlResultQueue *m_resultQueue;
		library::Thread *m_thread;
		DatatbaseOracle &m_dbEngine;

		std::map<std::string, uint64> xxbhIn30s;

		//car records
		CAR_INFO_LIST carDescriptionList;

		//dev heart beat
		DEVICE_HEARTBEAT_LIST devHeartBeatList_;
};

}

#endif /* end of file */