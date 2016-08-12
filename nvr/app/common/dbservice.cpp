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
#include "dbservice.h"
#include "eprotocol.h"
#include "sqlcommit.h"
#include "sqlcarrecord.h"
#include "sqldeviceheartbeat.h"
#include "../../library/util/log.h"
#include "../../library/util/system.h"
#include "../../library/concurrent/thread.h"
#include "../../corelib/database/common/field.h"
#include "../../corelib/database/common/sqlstring.h"
#include "../../corelib/database/common/queryresult.h"
#include "../../corelib/database/common/databaseimpl.h"

using namespace library;
using namespace corelib::database;
using namespace app;

extern ST_DABIGATE_CONFIG_INFO g_dabigate_config;
extern std::map<std::string, ST_SYS_SETTING_INFO> g_cjmap;
extern std::map<std::string, ST_CAR_BRAND_INFO> g_cbmap;
extern std::map<std::string, ST_DEVICE_INFO> g_devinfomap;

DbService::DbService(DatatbaseOracle &dbEngine) : m_resultQueue(NULL), m_thread(NULL), 
m_dbEngine(dbEngine)
{
	m_resultQueue = new SqlResultQueue;
	dbEngine.setResultQueue(m_resultQueue);
	
	m_thread = new Thread(this, "DBASE");
	m_thread->start();
}

DbService::~DbService()
{
	delete m_thread;
	m_thread = NULL;

	delete m_resultQueue;
	m_resultQueue = NULL;
}

void DbService::run()
{
	uint64 before = System::currentTimeMillis();
	uint64 current = before;
	while (1) {

		if (!carDescriptionList.empty() || !devHeartBeatList_.empty()) {

			if (!carDescriptionList.empty()) {
				handleCarRecords();
				flushTimedoutXxbh();
			}
			if (!devHeartBeatList_.empty())
				handleDeviceHeartBeatInfo();
		} else {
			Thread::sleep(50);
		}

		// every 5 seconds do a committing
		current = System::currentTimeMillis();
		if ((current - before) >= 5000) {
			m_dbEngine.execute(new SqlCommit());
			before = System::currentTimeMillis();
		}
	}
}

void DbService::handleCarRecords()
{
	CAR_INFO_LIST tmpList;
	
	m_lock.lock();
	tmpList = carDescriptionList;
	carDescriptionList.clear();
	m_lock.unlock();

	std::list<ST_CAR_RECORD_INFO> carList;
	for (CAR_INFO_LIST::iterator it = tmpList.begin(); it != tmpList.end(); ++it)
	{
		std::string xxbh = generateXxbh(*it);
		std::map<std::string, uint64>::iterator iter = xxbhIn30s.find(xxbh);
		ST_CAR_RECORD_INFO r;

		memset(&r, 0, sizeof(r));
		if (iter != xxbhIn30s.end()) {
			r.action = UPDATE;
		} else {
			r.action = INSERT;
			xxbhIn30s.insert(std::pair<std::string, uint64>(xxbh, System::currentTimeMillis()/1000));
		}
		r.record = *it;

		carList.push_back(r);
	}
	
	if (tmpList.size() > 0) m_dbEngine.execute(new SqlCarRecord(carList));
}

void DbService::handleDeviceHeartBeatInfo()
{
	static uint64 before = System::currentTimeMillis()/1000;
	uint64 current = System::currentTimeMillis()/1000;

	DEVICE_HEARTBEAT_LIST tmpList;

	if (!devHeartBeatList_.empty()) {
		if (current - before < 2 * 60) {
			return;
		} else {
			before = current;
		}
	}

	m_lock.lock();
	tmpList = devHeartBeatList_;
	devHeartBeatList_.clear();
	m_lock.unlock();

	tmpList.sort();
	tmpList.unique();
	if (tmpList.size() > 0) m_dbEngine.execute(new SqlDeviceHeartBeat(tmpList));
}

void DbService::init()
{
	if (g_dabigate_config.dbconfig.enable) {
		getCjConfigInfo();
		getCbConfigInfo();
		getDevConfigInfo();

		while (m_resultQueue->empty())
		{
			Thread::sleep(5);
		}
		m_resultQueue->Update();
	}
}

void DbService::insert(ST_CAR_DESCRIPTION_INFO* carInfo)
{
	m_lock.lock();

	carDescriptionList.push_back(carInfo);

	m_lock.unlock();
}

void DbService::insertDevHeartBeat(std::string deviceSn)
{
	m_lock.lock();
	devHeartBeatList_.push_back(deviceSn);
	m_lock.unlock();
}

void DbService::getCjConfigInfo()
{
	SqlString sql;

	sql << "SELECT * FROM sys_setting_tab WHERE settingname='VNP'";
	m_dbEngine.asyncQuery(this, &DbService::handleCjConfigInfo, sql.preview().c_str());
}

void DbService::handleCjConfigInfo(QueryResult *result)
{
	if (result == NULL) return;

	ST_SYS_SETTING_INFO stCj;
	do
	{
		::memset(&stCj, 0, sizeof(ST_SYS_SETTING_INFO));
		strncpy(stCj.dspVal, (*result)[1].toCString(), (*result)[1].toString().size());
		strncpy(stCj.mapKey, (*result)[4].toCString(), (*result)[4].toString().size());
		
		g_cjmap.insert(std::pair<std::string, ST_SYS_SETTING_INFO>(stCj.mapKey, stCj));
	} while (result->nextRow());

	Log(LOG_DEBUG, "%u CJ info record readed from database.", g_cjmap.size());
}

void DbService::getCbConfigInfo()
{
	SqlString sql;

	sql << "SELECT * FROM sys_setting_tab WHERE settingname='CarBrand'";
	m_dbEngine.asyncQuery(this, &DbService::handleCbConfigInfo, sql.preview().c_str());
}

void DbService::handleCbConfigInfo(QueryResult *result)
{
	if (result == NULL) return;

	ST_CAR_BRAND_INFO stCb;

	do
	{
		memset(&stCb, 0, sizeof(ST_CAR_BRAND_INFO));
		strncpy(stCb.brandId, (*result)[1].toCString(), (*result)[1].toString().size());
		strncpy(stCb.brandValue, (*result)[4].toCString(), (*result)[4].toString().size());
		
		g_cbmap.insert(std::pair<std::string, ST_CAR_BRAND_INFO>(stCb.brandId, stCb));
	} while (result->nextRow());

	Log(LOG_DEBUG, "%u CB info record readed from database.", g_cbmap.size());
}

void DbService::getDevConfigInfo()
{
	SqlString sql;

	sql << "SELECT * FROM v_deviceview";
	m_dbEngine.asyncQuery(this, &DbService::handleDevConfigInfo, sql.preview().c_str());
}

void DbService::handleDevConfigInfo(QueryResult *result)
{
	if (result == NULL) return;

	ST_DEVICE_INFO stDevInfo;
	do
	{
		::memset(&stDevInfo, 0, sizeof(ST_DEVICE_INFO));

		strncpy(stDevInfo.devSn, (*result)[0].toCString(), (*result)[0].toString().size());
		stDevInfo.lattitude = (*result)[1].toFloat();
		stDevInfo.longitude = (*result)[2].toFloat();
		strncpy(stDevInfo.bkkbh, (*result)[9].toCString(), (*result)[9].toString().size());
		strncpy(stDevInfo.manufactory, (*result)[10].toCString(), (*result)[10].toString().size());
		strncpy(stDevInfo.department, (*result)[11].toCString(), (*result)[11].toString().size());
		strncpy(stDevInfo.devName, (*result)[12].toCString(), (*result)[12].toString().size());
		strncpy(stDevInfo.direction, (*result)[13].toCString(), (*result)[13].toString().size());
		strncpy(stDevInfo.devLen, (*result)[14].toCString(), (*result)[14].toString().size());
		strncpy(stDevInfo.kkbh, (*result)[15].toCString(), (*result)[15].toString().size());
		strncpy(stDevInfo.kkmc, (*result)[16].toCString(), (*result)[16].toString().size());
		strncpy(stDevInfo.dwbh, (*result)[17].toCString(), (*result)[17].toString().size());
		strncpy(stDevInfo.dwmc, (*result)[18].toCString(), (*result)[18].toString().size());
		strncpy(stDevInfo.qymc, (*result)[19].toCString(), (*result)[19].toString().size());
		strncpy(stDevInfo.qydm, (*result)[20].toCString(), (*result)[20].toString().size());
		strncpy(stDevInfo.directionSn, (*result)[21].toCString(), (*result)[21].toString().size());
		
		g_devinfomap.insert(std::pair<std::string, ST_DEVICE_INFO>(stDevInfo.devSn, stDevInfo));
	} while (result->nextRow());

	Log(LOG_DEBUG, "%u device info record readed from database.", g_devinfomap.size());
}

std::string DbService::generateXxbh(ST_CAR_DESCRIPTION_INFO* carInfo)
{
	char buf[MAX_BUFFER_SIZE_064] = {0};
	std::string xxbh;
	
	if (carInfo == NULL) return "";

	xxbh.clear();
	memset(buf, 0, sizeof(buf));
	memcpy(buf, carInfo->blockSn, sizeof(carInfo->blockSn));
	xxbh += buf;
	
	memset(buf, 0, sizeof(buf));
	memcpy(buf, carInfo->vehiSn, sizeof(carInfo->vehiSn));
	xxbh += buf;

	return xxbh;
}

void DbService::flushTimedoutXxbh()
{
	static uint64 before = System::currentTimeMillis()/1000;
	uint64 current = System::currentTimeMillis()/1000;

	if (current - before < 10) {
		return;
	} else {
		before = current;
	}

	std::map<std::string, uint64>::iterator iter = xxbhIn30s.begin();
	for (; iter != xxbhIn30s.end();)
	{
		if (current - iter->second >= 30)
			xxbhIn30s.erase(iter++);
		else
			iter++;
	}
}