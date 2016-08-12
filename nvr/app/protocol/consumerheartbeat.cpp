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
#include "consumerheartbeat.h"

//using namespace library;
using namespace corelib;
using namespace corelib::protocol;
//using namespace corelib::MTP_NETWORK;
using namespace app;

const std::string ConsumerHeartBeatInfo::ID_CONSUMER_INFO = "6.9001";
////////////////////////////////////////////////////////////////////////////////
ConsumerHeartBeatInfo::ConsumerHeartBeatInfo()
{
}

ConsumerHeartBeatInfo::~ConsumerHeartBeatInfo()
{
}

std::string ConsumerHeartBeatInfo::getDataStructureType() const
{
	return ConsumerHeartBeatInfo::ID_CONSUMER_INFO;
}

void ConsumerHeartBeatInfo::copyDataStructure(const DataStructure *src)
{
	if (this == src) {
        return;
    }

	const ConsumerHeartBeatInfo *carInfo = dynamic_cast<const ConsumerHeartBeatInfo *>(src);
	this->status = carInfo->status;
	this->specialNumber = carInfo->specialNumber;
}

bool ConsumerHeartBeatInfo::equals(const DataStructure *value) const
{
	if (this == value) return true;

	const ConsumerHeartBeatInfo *carInfo = dynamic_cast<const ConsumerHeartBeatInfo *>(value);
	if (this->status != carInfo->status) return false;
	if (this->specialNumber != carInfo->specialNumber) return false;

	return true;
}

void ConsumerHeartBeatInfo::setStatus(uint32 status)
{
	this->status = status;
}

uint32 ConsumerHeartBeatInfo::getStatus()const
{
	return this->status;
}

void ConsumerHeartBeatInfo::setSpecialNumber(uint32 specialNumber)
{
	this->specialNumber = specialNumber;
}

uint32 ConsumerHeartBeatInfo::getSpecialNumber()const
{
	return this->specialNumber;
}