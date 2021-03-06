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
#include "devicetimesyncinfo.h"

using namespace app;

const std::string DeviceTimeSyncInfo::ID_DEVICE_TIMESYNC_INFO = "5.1401";
////////////////////////////////////////////////////////////////////////////////
DeviceTimeSyncInfo::DeviceTimeSyncInfo()
{
}

DeviceTimeSyncInfo::~DeviceTimeSyncInfo()
{
}

std::string DeviceTimeSyncInfo::getDataStructureType()const
{
	return DeviceTimeSyncInfo::ID_DEVICE_TIMESYNC_INFO;
}

void DeviceTimeSyncInfo::copyDataStructure(const DataStructure *src)
{
	if (this == src) {
        return;
    }

	const DeviceTimeSyncInfo *timeSyncInfo = dynamic_cast<const DeviceTimeSyncInfo *>(src);
	DataStructure::copyDataStructure(src);
	this->setTimeString(timeSyncInfo->getTimeString());
}

bool DeviceTimeSyncInfo::equals(const DataStructure *value) const
{
	if (this == value) return true;

	const DeviceTimeSyncInfo *timeSyncInfo = dynamic_cast<const DeviceTimeSyncInfo *>(value);
	if (this->timestring_ != timeSyncInfo->timestring_) return false;

	return true;
}

void DeviceTimeSyncInfo::updateTimeString()
{
	char cTime[128] = {0};
	struct tm *pstTm;
	time_t timep;

	timep = time(NULL);
	pstTm = gmtime(&timep); /* 获取GMT时间: 换算成北京时间需要 + 8 */
	sprintf(cTime, "%04d-%02d-%02d %02d:%02d:%02d", pstTm->tm_year + 1900, 
			pstTm->tm_mon + 1, pstTm->tm_mday, pstTm->tm_hour + 8, pstTm->tm_min,
			pstTm->tm_sec);

	this->timestring_.append(cTime);
}