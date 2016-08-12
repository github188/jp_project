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

#include "devicetimesyncinfo.h"
#include "../../library/util/log.h"
#include "devicetimesyncinfomarshaller.h"
#include "../../corelib/protocol/format.h"
#include "../../corelib/protocol/datastructure.h"

using namespace library;
using namespace corelib::protocol;
using namespace corelib::MTP_NETWORK;
using namespace app;

DeviceTimeSyncInfoMarshaller::DeviceTimeSyncInfoMarshaller()
{
}

DeviceTimeSyncInfoMarshaller::~DeviceTimeSyncInfoMarshaller()
{
}

CP::DataStructure* DeviceTimeSyncInfoMarshaller::createObject()const
{
	return new DeviceTimeSyncInfo();
}

std::string DeviceTimeSyncInfoMarshaller::getDataStructureType()const
{
	return DeviceTimeSyncInfo::ID_DEVICE_TIMESYNC_INFO;
}

void DeviceTimeSyncInfoMarshaller::unmarshal(CP::Format *, CP::DataStructure *dataStructure, library::ByteBuffer &bytebuffer)
{
	if (dataStructure == NULL) {
		Log(LOG_ERROR, "DeviceTimeSyncInfoMarshaller::unmarshal - dataStructure null.");
		return;
	}

	DeviceTimeSyncInfo *info = dynamic_cast<DeviceTimeSyncInfo *>(dataStructure);
	uint8 tempbuffer[64] = {0};

	bytebuffer.getRawBytes(tempbuffer, 19);
	info->setTimeString(std::string((char*)tempbuffer));
}

void DeviceTimeSyncInfoMarshaller::marshal(CP::Format *, CP::DataStructure *dataStructure, library::ByteBuffer &bytebuffer)
{
	if (dataStructure == NULL) {
		Log(LOG_ERROR, "DeviceHeartBeatMarshaller::marshal - input null.");
		return;
	}

	DeviceTimeSyncInfo *info = dynamic_cast<DeviceTimeSyncInfo *>(dataStructure);

	bytebuffer.putRawBytes((const uint8*)(info->getTimeString().c_str()), 19);

	return;
}