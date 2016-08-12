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
#include "consumerregisterinfo.h"

using namespace corelib;
using namespace corelib::protocol;
using namespace app;

const std::string ConsumerRegisterInfo::ID_CONSUMER_REGISTER_INFO = "6.9003";
////////////////////////////////////////////////////////////////////////////////
ConsumerRegisterInfo::ConsumerRegisterInfo()
{
}

ConsumerRegisterInfo::~ConsumerRegisterInfo()
{
}

std::string ConsumerRegisterInfo::getDataStructureType() const
{
	return ConsumerRegisterInfo::ID_CONSUMER_REGISTER_INFO;
}

void ConsumerRegisterInfo::copyDataStructure(const DataStructure *src)
{
	if (this == src) return;

	const ConsumerRegisterInfo* registerInfo = dynamic_cast<const ConsumerRegisterInfo*> (src);

	this->status = registerInfo->status;
	this->RegisterType = registerInfo->RegisterType;
}

bool ConsumerRegisterInfo::equals(const DataStructure *value) const
{
	if (this == value) return true;

	const ConsumerRegisterInfo* registerInfo = dynamic_cast<const ConsumerRegisterInfo*> (value);

	if (this->status != registerInfo->status) return false;
	if (this->RegisterType != registerInfo->RegisterType) return false;

	return true;
}

void ConsumerRegisterInfo::setRegisterType(uint32 registerType)
{
	this->RegisterType = registerType;
}

uint32 ConsumerRegisterInfo::getRegisterType()const
{
	return this->RegisterType;
}

void ConsumerRegisterInfo::setStatus(uint32 status)
{
	this->status = status;
}

uint32 ConsumerRegisterInfo::getStatus()const
{
	return this->status;
}