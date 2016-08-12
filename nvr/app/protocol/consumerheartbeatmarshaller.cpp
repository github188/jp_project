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
#include "../../corelib/protocol/datastructure.h"
#include "../../corelib/protocol/format.h"
#include "consumerheartbeatmarshaller.h"
#include "../../library/util/log.h"
#include "consumerheartbeat.h"

using namespace library;
using namespace corelib::protocol;
using namespace corelib::MTP_NETWORK;
using namespace app;

ConsumerHeartBeatInfoMarshaller::ConsumerHeartBeatInfoMarshaller()
{
}

ConsumerHeartBeatInfoMarshaller::~ConsumerHeartBeatInfoMarshaller()
{
}

DataStructure* ConsumerHeartBeatInfoMarshaller::createObject() const
{
	return new ConsumerHeartBeatInfo();
}

std::string ConsumerHeartBeatInfoMarshaller::getDataStructureType() const
{
	return ConsumerHeartBeatInfo::ID_CONSUMER_INFO;
}

void ConsumerHeartBeatInfoMarshaller::unmarshal(CP::Format*, CP::DataStructure *dataStructure, library::ByteBuffer &bytebuffer)
{
	if (dataStructure == NULL) {
		Log(LOG_ERROR, "ConsumerHeartBeatInfoMarshaller::unmarshal - datastructure null.");
		return ;
	}

	ConsumerHeartBeatInfo *info = dynamic_cast<ConsumerHeartBeatInfo *>(dataStructure);
	info->setStatus(bytebuffer.getInt());
	info->setSpecialNumber(bytebuffer.getInt());
}

void ConsumerHeartBeatInfoMarshaller::marshal(CP::Format*, CP::DataStructure *dataStructure, library::ByteBuffer &bytebuffer)
{
	if (dataStructure == NULL) {
		Log(LOG_ERROR, "ConsumerHeartBeatInfoMarshaller::marshal - input null.");
		return;
	}

	ConsumerHeartBeatInfo *info = dynamic_cast<ConsumerHeartBeatInfo *>(dataStructure);
	
	bytebuffer.putInt((int32)info->getStatus());
	bytebuffer.putInt((int32)info->getSpecialNumber());

	return;
}