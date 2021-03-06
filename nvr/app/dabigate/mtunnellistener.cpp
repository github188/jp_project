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
#include "mtunnellistener.h"
#include "../../library/util/log.h"
#include "../../library/util/system.h"
#include "../protocol/deviceheartbeat.h"
#include "../../corelib/protocol/format.h"
#include "../protocol/devicetimesyncinfo.h"
#include "../common/zhuhaiprotocolconfig.h"
#include "../../corelib/network/connection.h"
#include "../../library/concurrent/reentrant.h"
#include "../../corelib/protocol/datastructure.h"
#include "../../library/buffer/localbytearraybuffer.h"
#include "../../library/buffer/remotebytearraybuffer.h"

using namespace library;
using namespace corelib;
using namespace corelib::database;
using namespace corelib::protocol;
using namespace corelib::MTP_NETWORK;
using namespace app;

MTunnelListener::MTunnelListener(DbService &dbService) : m_dbService(dbService), 
wireformat_(NULL), timeInSecs_(0)
{
}

MTunnelListener::~MTunnelListener()
{
}

void MTunnelListener::onStart(CM::Connection *connection)
{
	if (connection == NULL) return;

	connection->setDisableInputBuffer(false);
	connection->setFlushoutBeforeClose(false);
	connection->setDeleteByHandler(true);
	
	connectionManager_.addConnection(connection);
}

void MTunnelListener::onCommand(CM::Connection *connection, const char *data, int32 len)
{	
	if (connection == NULL || data == NULL || len == 0) return;

	static uint64 u64c = 1;
	if (this->wireformat_ != NULL) {

		NETBUFFER netByteBuffer((uint8*)data, len, 0, len);
		DataStructure* datastructure(wireformat_->unmarshal(connection, netByteBuffer));
		if (datastructure == NULL) return;
		
		std::string messageType = datastructure->getDataStructureType();
		if (messageType == DeviceHeartBeatInfo::ID_DEVICE_HEARTBEAT_INFO) {
			std::string deviceSn;

			DeviceHeartBeatInfo *info = dynamic_cast<DeviceHeartBeatInfo *>(datastructure);
			deviceSn.append(info->getDepartment());
			deviceSn.append(info->getDeviceId());
			deviceSn.append(info->getSerialNumber());
			m_dbService.insertDevHeartBeat(deviceSn);

			Log(LOG_DEBUG, "MTunnelListener::onCommand - Heart beat count: %llu", u64c++);
		} else {
			Log(LOG_DEBUG, "MTunnelListener::onCommand - message<%s> unknown.", messageType.c_str());
		}

		delete datastructure;
	} else {
		Log(LOG_WARN, "MTunnelListener::onCommand - wireformat null.");
	}

#if(defined(_ZH_PLATFORM_))
	//send timeSync
	uint64 now = System::currentTimeMillis()/1000;
	if (now - timeInSecs_ >= 15) {
		DeviceTimeSyncInfo timeSyncInfo;
		NETBUFFER nb(MAX_BUFFER_SIZE_01k);
		
		timeSyncInfo.updateTimeString();
		if (wireformat_ != NULL) {
			wireformat_->marshal(&timeSyncInfo, nb);
		}
		Log(LOG_DEBUG, "MTunnelListener::onCommand - broadcast time sync: %s.", timeSyncInfo.getTimeString().c_str());
		connectionManager_.dispatchAll((const char *)nb.array(), nb.limit());
		
		timeInSecs_ = System::currentTimeMillis()/1000;
	}
#endif
}

void MTunnelListener::onEnd(CM::Connection *connection)
{
	if (connection == NULL) return;

	connection->setDeleteByHandler(true);
	connectionManager_.delConnection(connection);
}

void MTunnelListener::setWireFormat(CP::Format *format)
{
	wireformat_ = format;
}