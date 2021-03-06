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
#include "deviceheartbeat.h"

using namespace app;

const std::string DeviceHeartBeatInfo::ID_DEVICE_HEARTBEAT_INFO = "1.1001";
////////////////////////////////////////////////////////////////////////////////
DeviceHeartBeatInfo::DeviceHeartBeatInfo()
{
}

DeviceHeartBeatInfo::~DeviceHeartBeatInfo()
{
}

std::string DeviceHeartBeatInfo::getDataStructureType()const
{
	return DeviceHeartBeatInfo::ID_DEVICE_HEARTBEAT_INFO;
}

void DeviceHeartBeatInfo::copyDataStructure(const DataStructure *src)
{
	if (this == src) {
        return;
    }

	const DeviceHeartBeatInfo *heartBeatInfo = dynamic_cast<const DeviceHeartBeatInfo *>(src);
	DataStructure::copyDataStructure(src);
	this->setDeviceId(heartBeatInfo->getDeviceId());
	this->setDepartment(heartBeatInfo->getDepartment());
	this->setSerialNumber(heartBeatInfo->getSerialNumber());
}

bool DeviceHeartBeatInfo::equals(const DataStructure *value) const
{
	if (this == value) {
        return true;
    }

	const DeviceHeartBeatInfo *heartBeatInfo = dynamic_cast<const DeviceHeartBeatInfo *>(value);
	if (this->deviceId_ != heartBeatInfo->deviceId_) return false;
	if (this->department_ != heartBeatInfo->department_) return false;
	if (this->serialNumber_ != heartBeatInfo->serialNumber_) return false;

	return true;
}