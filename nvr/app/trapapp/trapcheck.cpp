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
#include "trapcheck.h"
#include "../common/sqlcartrap.h"
#include "../../library/util/log.h"
#include "../../library/util/system.h"
#include "../../corelib/protocol/format.h"
#include "../protocol/carbypassdescinfo.h"
#include "../../corelib/network/connection.h"
#include "../protocol/consumerregisterinfo.h"
#include "../../corelib/database/common/field.h"
#include "../../corelib/database/common/sqlstring.h"
#include "../../corelib/database/common/queryresult.h"
#include "../../library/buffer/localbytearraybuffer.h"
#include "../../corelib/database/common/sqloperation.h"
#include "../../corelib/database/common/databaseimpl.h"
#include "../../library/buffer/remotebytearraybuffer.h"

using namespace library;
using namespace corelib;
using namespace corelib::database;
using namespace corelib::protocol;
using namespace corelib::MTP_NETWORK;
using namespace app;

extern ST_TRAPAPP_CONFIG_INFO g_trapapp_config;
extern std::map<std::string, ST_DEVICE_INFO> g_devinfomap;

TrapCheck::TrapCheck(DatatbaseOracle &dbEngine) : wireformat_(NULL), m_dbEngine(dbEngine)
{
	// config trap
	trapConfigInit();

	// start mqservice
	m_mqService.bindHandler(std::string("C05"), this, &TrapCheck::onUpdateTrapConfig);
	m_mqService.bindHandler(std::string("C02"), this, &TrapCheck::onUpdateTrapConfig);
	m_mqService.start();
}

TrapCheck::~TrapCheck()
{
}

void TrapCheck::onStart(CM::Connection *connection)
{
	if (connection == NULL) return;

	connection->setDeleteByHandler(false); // client handler is deleted by user
	
	// send consumer register information
	ConsumerRegisterInfo registerInfo;

	registerInfo.setRegisterType(9005);
	registerInfo.setStatus(0);

	NETBUFFER netByteBuffer(1024);
	if (this->wireformat_ != NULL) {
		wireformat_->marshal(&registerInfo, netByteBuffer);
		connection->send((const char *)netByteBuffer.array(), netByteBuffer.limit());
	}
}

void TrapCheck::onCommand(CM::Connection *connection, const char *data, int32 len)
{
	if (connection == NULL || data == NULL || len == 0) return;

	static uint64 u64c = 1;
	if (this->wireformat_ != NULL) {

		NETBUFFER netByteBuffer((uint8*)data, len, 0, len);
		DataStructure* datastructure(wireformat_->unmarshal(connection, netByteBuffer));
		if (datastructure == NULL) return;
		
		std::string messageType = datastructure->getDataStructureType();
		if (messageType == CarBypassDescInfo::ID_CARBYPASS_INFO) {
			checkAndNotify(static_cast<CarBypassDescInfo *>(datastructure));
		} else {
			Log(LOG_DEBUG, "TrapCheck::onCommand - message<%s> unknown.", messageType.c_str());
		}

		delete datastructure;
		Log(LOG_DEBUG, "TrapCheck::onCommand - statistic of packet: %llu", u64c++);
	} else {
		Log(LOG_WARN, "TrapCheck::onCommand - wireformat null.");
	}
}

void TrapCheck::onEnd(CM::Connection *connection)
{
	// do nothing
}

void TrapCheck::checkAndNotify(CarBypassDescInfo *carInfo)
{
	if (carInfo == NULL) return;

	ST_CAR_TRAP_INFO trap;

	memset(&trap, 0, sizeof(ST_CAR_TRAP_INFO));
	trap.stcarinfo = carInfo->getAll();

	// check
	bool bInWhite = m_white.check(trap);
	if (!bInWhite) {
		bool bInBlack = m_black.check(trap);
		if (bInBlack) {
			m_dbEngine.execute(new SqlCarTrap(trap));
			m_mqService.add(&trap);
		}
	} else {
		m_dbEngine.execute(new SqlCarTrap(trap));
		m_mqService.add(&trap);
	}
	
	bool bInControl = m_control.check(trap);
	if (bInControl) {
		m_dbEngine.execute(new SqlCarTrap(trap));
		m_mqService.add(&trap);
	}

	//real time update
	m_mqService.add(trap.stcarinfo);
}

void TrapCheck::trapConfigInit()
{
	if (g_trapapp_config.dbconfig.enable) {
		getWhiteList();
		getBlackList();
		getControlList();
		getDevConfigInfo();
	}
}

void TrapCheck::onUpdateTrapConfig(std::string cmd)
{
	getWhiteList();
	getBlackList();
	getControlList();
}

void TrapCheck::setWireFormat(CP::Format *format)
{
	this->wireformat_ = format;
}

void TrapCheck::getWhiteList()
{
	handleWhiteList(m_dbEngine.query("SELECT * FROM whitelist_tab"));
}

void TrapCheck::handleWhiteList(QueryResult *result)
{
	if (result == NULL) return;

	m_white.configDrop();

	ST_WHITELIST_CONFIG_INFO whiteList;
	do
	{
		::memset(&whiteList, 0, sizeof(ST_WHITELIST_CONFIG_INFO));
		
		strncpy(whiteList.hphm, (*result)[1].toCString(), (*result)[1].toString().size());
		strncpy(whiteList.cpys, (*result)[2].toCString(), (*result)[2].toString().size());
		strncpy(whiteList.csys, (*result)[3].toCString(), (*result)[3].toString().size());
		strncpy(whiteList.cllx, (*result)[4].toCString(), (*result)[4].toString().size());
		strncpy(whiteList.hpzl, (*result)[5].toCString(), (*result)[5].toString().size());
		strncpy(whiteList.clpp, (*result)[6].toCString(), (*result)[6].toString().size());
		strncpy(whiteList.cljs, (*result)[7].toCString(), (*result)[7].toString().size());
		whiteList.shzj = (*result)[8].toUint32();
		strncpy(whiteList.llsj, (*result)[9].toCString(), (*result)[9].toString().size());

		m_white.configUpdate(whiteList);
	} while (result->nextRow());

	Log(LOG_DEBUG, "TrapCheck::handleWhiteList - get %d whiteList config item.", m_white.getWhiteListSize());
}

void TrapCheck::getBlackList()
{
	handleBlackList(m_dbEngine.query("SELECT * FROM blacklist_tab"));
}

void TrapCheck::handleBlackList(QueryResult *result)
{
	if (result == NULL) return;

	m_black.configDrop();

	ST_BALCKLIST_CONFIG_INFO blackList;
	do
	{
		::memset(&blackList, 0, sizeof(ST_BALCKLIST_CONFIG_INFO));

		strncpy(blackList.bkxxbh, (*result)[0].toCString(), (*result)[0].toString().size());
		strncpy(blackList.hphm, (*result)[1].toCString(), (*result)[1].toString().size());
		strncpy(blackList.cpys, (*result)[2].toCString(), (*result)[2].toString().size());
		strncpy(blackList.csys, (*result)[3].toCString(), (*result)[3].toString().size());
		strncpy(blackList.cllx, (*result)[4].toCString(), (*result)[4].toString().size());
		strncpy(blackList.hpzl, (*result)[5].toCString(), (*result)[5].toString().size());
		strncpy(blackList.clpp, (*result)[6].toCString(), (*result)[6].toString().size());
		strncpy(blackList.cljs, (*result)[7].toCString(), (*result)[7].toString().size());
		strncpy(blackList.bjjb, (*result)[8].toCString(), (*result)[8].toString().size());
		strncpy(blackList.llsj, (*result)[9].toCString(), (*result)[9].toString().size());

		m_black.configUpdate(blackList);

	} while (result->nextRow());

	Log(LOG_DEBUG, "TrapCheck::handleBlackList - get %d blacklist config item.", m_black.getBlackListSize());
}

void TrapCheck::getControlList()
{
	handleControlList(m_dbEngine.query("SELECT * FROM setcontrol_tab t WHERE t.bkkssj <= (SELECT SYSDATE FROM dual) AND t.bklen >= (SELECT SYSDATE FROM dual) AND t.BKZT = '0'"));
}

void TrapCheck::handleControlList(QueryResult *result)
{
	if (result == NULL) return;

	m_control.configDrop();

	ST_CONTROLLIST_CONFIG_INFO controllist;
	do
	{
		::memset(&controllist, 0, sizeof(ST_CONTROLLIST_CONFIG_INFO));
		
		strncpy(controllist.bkxxbh, (*result)[0].toCString(), (*result)[0].toString().size());
		strncpy(controllist.hphm, (*result)[1].toCString(), (*result)[1].toString().size());
		strncpy(controllist.hpys, (*result)[2].toCString(), (*result)[2].toString().size());
		strncpy(controllist.clpp, (*result)[3].toCString(), (*result)[3].toString().size());
		strncpy(controllist.clwx, (*result)[4].toCString(), (*result)[4].toString().size());
		strncpy(controllist.csys, (*result)[5].toCString(), (*result)[5].toString().size());
		strncpy(controllist.cllx, (*result)[6].toCString(), (*result)[6].toString().size());
		strncpy(controllist.hpzl, (*result)[7].toCString(), (*result)[7].toString().size());
		strncpy(controllist.bkfw, (*result)[12].toCString(), (*result)[12].toString().size());
		strncpy(controllist.bkkssj, (*result)[14].toCString(), (*result)[14].toString().size());
		strncpy(controllist.bkjssj, (*result)[16].toCString(), (*result)[16].toString().size());
		strncpy(controllist.bkzt, (*result)[33].toCString(), (*result)[33].toString().size());
		strncpy(controllist.lxdh, (*result)[28].toCString(), (*result)[28].toString().size());
		strncpy(controllist.dxfsbs, (*result)[25].toCString(), (*result)[25].toString().size());

		m_control.configUpdate(controllist);
	} while (result->nextRow());

	Log(LOG_DEBUG, "TrapCheck::handleControlList - get %d controllist config item.", m_control.getControlListSize());
}

void TrapCheck::getDevConfigInfo()
{
	handleDevConfigInfo(m_dbEngine.query("SELECT * FROM v_deviceview"));
}

void TrapCheck::handleDevConfigInfo(QueryResult *result)
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
