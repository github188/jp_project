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
#include <memory>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>

#include "../../library/buffer/remotebytearraybuffer.h"
#include "../../library/buffer/localbytearraybuffer.h"
#include "../../corelib/protocol/datastructure.h"
#include "../../library/concurrent/reentrant.h"
#include "../../corelib/network/connection.h"
#include "../protocol/consumerregisterinfo.h"
#include "../protocol/cardescriptioninfo.h"
#include "../common/zhuhaiprotocolconfig.h"
#include "../../corelib/protocol/format.h"
#include "../protocol/consumerheartbeat.h"
#include "../protocol/carbypassdescinfo.h"
#include "../../library/util/log.h"
#include "dataforward.h"

using namespace library;
using namespace corelib;
using namespace corelib::protocol;
using namespace corelib::MTP_NETWORK;
using namespace app;

////////////////////////////////////////////////////////////////////////////////
class InvalidConsumerCleaner
{
	public:

		InvalidConsumerCleaner(std::list<std::string> &ol) : objectList(ol) {
			removeList.clear();
		};
		
		~InvalidConsumerCleaner() {
			
			while (removeList.size() > 0) {
				
				std::list<std::string>::iterator it = removeList.begin();
				for (std::list<std::string>::iterator i = objectList.begin(); i != objectList.end(); ++i){
					if (*i == *it) {
						objectList.erase(i);
						break;
					}
				}
				removeList.erase(it);
			}
		};

	public:

		void add(std::string& consumerId) {
			removeList.push_back(consumerId);
		};

	private:

		std::list<std::string> &objectList; // checked list
		std::list<std::string> removeList; // removed node container
};


/////////////////////////////////////////////////////////////////////////////////
DataForward::DataForward(ConnectionManager &manager) : wireformat_(NULL), connectionManager_(manager)
{
	consumerList_.clear();
}

DataForward::~DataForward()
{
	consumerList_.clear();
}

void DataForward::onStart(CM::Connection *connection)
{
	if (connection == NULL) return;

	connection->setDisableInputBuffer(false);
	connection->setFlushoutBeforeClose(false);
	connection->setDeleteByHandler(true);
	
	connectionManager_.addConnection(connection);
}

void DataForward::onCommand(CM::Connection *connection, const char *data, int32 len)
{
	if (connection == NULL || data == NULL || len == 0) return;

	if (this->wireformat_ != NULL) {

		NETBUFFER netByteBuffer((uint8*)data, len, 0, len);
		std::auto_ptr<DataStructure> datastructure(wireformat_->unmarshal(connection, netByteBuffer));
		if (datastructure.get() == NULL) return;
		
		std::string messageType = datastructure->getDataStructureType();
		if (messageType == CarDescriptionInfo::ID_CAR_INFO || messageType == CarBypassDescInfo::ID_CARBYPASS_INFO) {
			static uint64 carInfoCount = 1;
			int32 pt = 0, dt = 0;
			sscanf(messageType.c_str(), "%d.%d", &pt, &dt);
			forwardMessage(dt, data, len);
			Log(LOG_DEBUG, "DataForward::onCommand - total count of carinfo: %llu", carInfoCount++);
		} else if (messageType == ConsumerHeartBeatInfo::ID_CONSUMER_INFO) {
			ConsumerHeartBeatInfo *heartBeatInfo = static_cast<ConsumerHeartBeatInfo *>(datastructure.get());
			uint32 id = heartBeatInfo->getSpecialNumber();
			if (id != 0xefeffefe) {
				Log(LOG_ERROR, "DataForward::onCommand - heart beat identity<0x%x> invalid.", id);
				connection->shutdown();
				return;
			}

			std::string peerIp = connection->getRemoteIpAddress();
			uint16 port = connection->getRemotePort();
			Log(LOG_DEBUG, "DataForward::onCommand - consumer<%s:%d> heart beat.", peerIp.c_str(), port);
		} else if (messageType == ConsumerRegisterInfo::ID_CONSUMER_REGISTER_INFO) {
			Synchronization autolock(mutex_);
			addToConsumerList(datastructure.get());
		} else {
			Log(LOG_DEBUG, "DataForward::onCommand - unknown message type: %s.", messageType.c_str());
		}
	} else {
		Log(LOG_WARN, "DataForward::onCommand - wireformat null.");
	}
}

void DataForward::forwardMessage(CP::DataStructure *ds)
{
	int32 pt = 0, dt = 0;
	std::string messageType = ds->getDataStructureType();
	sscanf(messageType.c_str(), "%d.%d", &pt, &dt);
	forwardMessage(dt, ds);
}

void DataForward::forwardMessage(int32 type, CP::DataStructure *ds)
{
	NETBUFFER netByteBuffer(MAX_BUFFER_SIZE_03m);

	wireformat_->marshal(ds, netByteBuffer);
	forwardMessage(type, (const char*)netByteBuffer.array(), netByteBuffer.limit());
}

void DataForward::forwardMessage(int32 type, const char *rawData, int32 len)
{
	if (len == 0 || rawData == NULL) return;

	Synchronization autolock(mutex_);
	
	switch (type)
	{
	case PROCESS_CAR_INFO_FORWARD_MSG_ID:
	case VEHICLE_INTERAVE_MSG_ID:
		doForwardingVehicleData(type, rawData, len);
		break;
	default:
		Log(LOG_DEBUG, "DataForward::forwardMessage - unknown data type: %d.", type);
		break;
	}
}

void DataForward::doForwardingVehicleData(int32 registerType, const char *rawData, int32 len)
{
	static uint64 forwardConunt = 1;
	
	if (rawData == NULL) return;

	REGIST_TYPE rt = (REGIST_TYPE)registerType;
	std::map<REGIST_TYPE, CONSUMERS>::iterator iter = consumerList_.find(rt);
	if (iter != consumerList_.end()) {

		CONSUMERS::iterator it = iter->second.begin();
		InvalidConsumerCleaner cleaner(iter->second);
		for (; it != iter->second.end(); ++it) {
			std::string sessionId = *it;
			bool bSucced = connectionManager_.dispatch(sessionId, rawData, len);
			if (!bSucced) {
				// delete the invalid consumers
				cleaner.add(sessionId);
			}
			Log(LOG_DEBUG, "DataForward::doForwardingVehicleData - forward %d to %s, len: %u", registerType, sessionId.c_str(), len);
		}

		Log(LOG_DEBUG, "DataForward::doForwardingVehicleData - statistic forwarding of %d, total count: %llu", registerType, forwardConunt++);
	} else {
		Log(LOG_INFO, "DataForward::doForwardingVehicleData - no users added in.");
	}
}

void DataForward::addToConsumerList(DataStructure *ds)
{
	if (ds == NULL) return;

	std::string sessionId = ds->getSessionId();
	ConsumerRegisterInfo *registerInfo = static_cast<ConsumerRegisterInfo *>(ds);
	REGIST_TYPE registerType = registerInfo->getRegisterType();

	std::map<REGIST_TYPE, CONSUMERS>::iterator iter = consumerList_.find(registerType);
	if (iter != consumerList_.end()) {
		iter->second.push_back(sessionId); // should check if exist? - need not!
		Log(LOG_DEBUG, "DataForward::addToConsumerList - session <%s> register message <%d>.", sessionId.c_str(), registerType);
	} else {
		CONSUMERS consumer;
		consumer.push_back(sessionId);
		consumerList_.insert(std::pair<REGIST_TYPE, CONSUMERS>(registerType, consumer));
		Log(LOG_DEBUG, "DataForward::addToConsumerList - session <%s> register message <%d>.", sessionId.c_str(), registerType);
	}
}

void DataForward::onEnd(CM::Connection *connection)
{
	if (connection == NULL) return;

	std::string session = connection->uniqueIdentifier();
	std::map<REGIST_TYPE, CONSUMERS>::iterator iter = consumerList_.begin();
	for (; iter != consumerList_.end(); ++iter) {
		CONSUMERS &ol = iter->second;
		CONSUMERS::iterator it = std::find(ol.begin(), ol.end(), session);
		if (it != ol.end()) {
			ol.erase(it);
			Log(LOG_DEBUG, "DataForward::onEnd - erase consumer: %s", session.c_str());
		}
	}

	connection->setDeleteByHandler(true);
	connectionManager_.delConnection(connection);
}

void DataForward::setWireFormat(CP::Format *format)
{
	this->wireformat_ = format;
}
