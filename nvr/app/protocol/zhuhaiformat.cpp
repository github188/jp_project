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
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sstream>
#include <vector>
#include <memory>
#include <iostream>

#include "../../corelib/protocol/datastructuremarshaller.h"
#include "../../corelib/protocol/datastructure.h"
#include "../../corelib/network/connection.h"
#include "../common/zhuhaiprotocolconfig.h"
#include "../common/zhuhaiprotocolconfig.h"
#include "../../library/util/log.h"
#include "zhuhaiformat.h"

#include "cardescriptioninfomarshaller.h"
#include "cardescriptioninfo.h"
#include "consumerheartbeatmarshaller.h"
#include "consumerheartbeat.h"
#include "consumerregisterinfomarshaller.h"
#include "consumerregisterinfo.h"
#include "carbypassdescinfomarshaller.h"
#include "carbypassdescinfo.h"
#include "deviceheartbeatmarshaller.h"
#include "deviceheartbeat.h"
#include "devicetimesyncinfomarshaller.h"
#include "devicetimesyncinfo.h"

using namespace library;
using namespace corelib;
using namespace corelib::protocol;
using namespace corelib::MTP_NETWORK;
using namespace app;

////////////////////////////////////////////////////////////////////////////////

static int32 getIdFromString(const char *data, char separator, int32 &first, int32 &second)
{
	if (data == NULL) return -1;
	sscanf(data, "%d.%d", &first, &second);
	return 0;
}

////////////////////////////////////////////////////////////////////////////////
ZhuHaiFormat::ZhuHaiFormat(uint8 version) : version(version)
{
	this->initFormatProperty();
}

ZhuHaiFormat::~ZhuHaiFormat()
{
}

void ZhuHaiFormat::marshal(DataStructure *command, ByteBuffer &netByteBuffer)
{
	int32 dataLength = 0;
	
	if (command == NULL) return;

	std::string type = command->getDataStructureType();
	
	// head separator
	netByteBuffer.putShort(headSeparator);

	// digit number
	netByteBuffer.put((uint8*)digitString.c_str(), (int32)digitString.size(), 0, (int32)digitString.size());

	// version
	netByteBuffer.putChar((char)version);

	//protocol type
	int32 protype = 0, datatype = 0;
	getIdFromString(type.c_str(), '.', protype, datatype);
	if (protype == -1) {
		netByteBuffer.putChar((char)2);
	} else {
		netByteBuffer.putChar((uint8)protype);
	}

	// data type
	netByteBuffer.putShort((int16)datatype);

	// data length
	netByteBuffer.putInt(dataLength);

	// data
	DataStructureMarshaller *marshaller = marshallers[type];
	marshaller->marshal(this, command, netByteBuffer);
	
	// check sum
	netByteBuffer.putShort((int16) 0);

	// tail separator
	netByteBuffer.putShort(tailSeparator);

	//end pack
	netByteBuffer.flip();

	// update data length
	dataLength = netByteBuffer.limit() - FIX_SPEC_PRO_SIZE;
	netByteBuffer.putInt(FIX_SPEC_DATA_LEN_POS, dataLength);
}

DataStructure* ZhuHaiFormat::unmarshal(Connection *connection, ByteBuffer &netByteBuffer)
{	
	// check head separator
	uint16 headVal = netByteBuffer.getShort();
//	Log(LOG_DEBUG, "headVal = 0x%x", headVal);
	if (headVal != headSeparator) {
		Log(LOG_ERROR, "ZhuHaiFormat::unmarshal - packet head 0x%x invalid.", headVal);
		return NULL;
	}

	// check digit number
	uint8 digitNumber[128] = {0};
	netByteBuffer.getRawBytes(digitNumber, FIX_SPEC_DIGIT_LETTER_SIZE);
//	Log(LOG_DEBUG, "digitNumber = %s", digitNumber);
	if (memcmp(digitNumber, digitString.c_str(), FIX_SPEC_DIGIT_LETTER_SIZE)) {
		Log(LOG_ERROR, "ZhuHaiFormat::unmarshal - digit number %s invalid.", digitNumber);
		return NULL;
	}

	// check head version
	uint8 ver = netByteBuffer.getChar();
//	Log(LOG_DEBUG, "ver = %d", ver);
	if (ver != version) {
		Log(LOG_ERROR, "ZhuHaiFormat::unmarshal - version %d invalid.", ver);
		return NULL;
	}

	uint8 proType = netByteBuffer.getChar();
//	Log(LOG_DEBUG, "proType = %d", proType);
	uint16 datatype = netByteBuffer.getShort();
//	Log(LOG_DEBUG, "datatype = %d", datatype);

	uint32 dataLength = netByteBuffer.getInt();
	if (dataLength >= MAX_BUFFER_SIZE_02m) {
		Log(LOG_ERROR, "ZhuHaiFormat::unmarshal - dataLength %d too long.", dataLength);
		return NULL;
	}

	char marshaltype[8] = {0};
	sprintf(marshaltype, "%d.%d", proType, datatype);
	DataStructureMarshaller *mashaller = marshallers[marshaltype];
	if (mashaller == NULL) {
		Log(LOG_ERROR, "ZhuHaiFormat::unmarshal - data type %s invalid.", marshaltype);
		return NULL;
	}
	
	std::auto_ptr<DataStructure> data(mashaller->createObject());
	if (connection != NULL) {
		data->setSessionId(connection->uniqueIdentifier());
	}
	mashaller->unmarshal(this, data.get(), netByteBuffer);

	uint16 checksum = netByteBuffer.getShort();
	//Log(LOG_DEBUG, "ZhuHaiFormat::unmarshal - chech sum: %d", checksum);
	
	uint16 tailVal = netByteBuffer.getShort();
	if (tailVal != tailSeparator) {
		Log(LOG_ERROR, "ZhuHaiFormat::unmarshal - tail 0x%x invalid.", tailVal);
		return NULL;
	}

	return data.release();
}

void ZhuHaiFormat::addMarshaller(const std::string &type, DataStructureMarshaller *marshaller)
{
	std::map<std::string, DataStructureMarshaller*>::iterator iter;

	iter = marshallers.find(type);
	if (iter == marshallers.end()) {

		marshallers[type] = marshaller;
	}
}

void ZhuHaiFormat::initFormatProperty()
{
	headSeparator = HEAD_SEPA;
	tailSeparator = TAIL_SEPA;
	digitString = DIGIT_LETTER;

	addMarshaller(CarDescriptionInfo::ID_CAR_INFO, new CarDescriptionInfoMarshaller());
	addMarshaller(ConsumerHeartBeatInfo::ID_CONSUMER_INFO, new ConsumerHeartBeatInfoMarshaller());
	addMarshaller(ConsumerRegisterInfo::ID_CONSUMER_REGISTER_INFO, new ConsumerRegisterInfoMarshaller());
	addMarshaller(CarBypassDescInfo::ID_CARBYPASS_INFO, new CarBypassDescInfoMarshaller());
	addMarshaller(DeviceHeartBeatInfo::ID_DEVICE_HEARTBEAT_INFO, new DeviceHeartBeatMarshaller());
	addMarshaller(DeviceTimeSyncInfo::ID_DEVICE_TIMESYNC_INFO, new DeviceTimeSyncInfoMarshaller());
}
