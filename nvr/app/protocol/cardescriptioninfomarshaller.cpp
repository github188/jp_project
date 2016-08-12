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
#include "../../library/buffer/remotebytearraybuffer.h"
#include "../../corelib/protocol/datastructure.h"
#include "../../corelib/protocol/format.h"
#include "cardescriptioninfomarshaller.h"
#include "../../library/util/log.h"
#include "cardescriptioninfo.h"


using namespace library;
using namespace corelib;
using namespace corelib::MTP_NETWORK;
using namespace corelib::protocol;
using namespace app;

CarDescriptionInfoMarshaller::CarDescriptionInfoMarshaller()
{
}

CarDescriptionInfoMarshaller::~CarDescriptionInfoMarshaller()
{
}

DataStructure* CarDescriptionInfoMarshaller::createObject() const
{
	return new CarDescriptionInfo();
}

std::string CarDescriptionInfoMarshaller::getDataStructureType() const
{
	return CarDescriptionInfo::ID_CAR_INFO;
}

void CarDescriptionInfoMarshaller::unmarshal(Format *, DataStructure *dataStructure, ByteBuffer &bytebuffer)
{
	uint8 tempbuffer[64] = {0};
	
	if (dataStructure == NULL) return;

	CarDescriptionInfo *info = dynamic_cast<CarDescriptionInfo *>(dataStructure);

	bytebuffer.getRawBytes(tempbuffer, 15);
	info->setXxbh(std::string((char*)tempbuffer));
	
	memset(tempbuffer, 0, sizeof(tempbuffer));
	bytebuffer.getRawBytes(tempbuffer, 18);
	info->setKkbh(std::string((char*)tempbuffer));

	memset(tempbuffer, 0, sizeof(tempbuffer));
	bytebuffer.getRawBytes(tempbuffer, 18);
	info->setSbbh(std::string((char*)tempbuffer));

	memset(tempbuffer, 0, sizeof(tempbuffer));
	bytebuffer.getRawBytes(tempbuffer, 24);
	info->setJgsj(std::string((char*)tempbuffer));

	memset(tempbuffer, 0, sizeof(tempbuffer));
	bytebuffer.getRawBytes(tempbuffer, 10);
	info->setCdbh(std::string((char*)tempbuffer));

	memset(tempbuffer, 0, sizeof(tempbuffer));
	bytebuffer.getRawBytes(tempbuffer, 15);
	info->setHphm(std::string((char*)tempbuffer));

	memset(tempbuffer, 0, sizeof(tempbuffer));
	bytebuffer.getRawBytes(tempbuffer, 1);
	info->setHpys(std::string((char*)tempbuffer));

	memset(tempbuffer, 0, sizeof(tempbuffer));
	bytebuffer.getRawBytes(tempbuffer, 15);
	info->setCwhphm(std::string((char*)tempbuffer));

	memset(tempbuffer, 0, sizeof(tempbuffer));
	bytebuffer.getRawBytes(tempbuffer, 1);
	info->setCwhpys(std::string((char*)tempbuffer));

	memset(tempbuffer, 0, sizeof(tempbuffer));
	bytebuffer.getRawBytes(tempbuffer, 1);
	info->setHpyz(std::string((char*)tempbuffer));

#if(defined(_ZH_PLATFORM_))
	float clsd = bytebuffer.getFloat();
#else
	float clsd = (float)bytebuffer.getInt();
#endif
	info->setClsd(clsd);

#if(defined(_ZH_PLATFORM_))
	float clxs = bytebuffer.getFloat();
#else
	float clxs = (float)bytebuffer.getInt();
#endif
	info->setClxs(clxs);

	int32 clcd = bytebuffer.getInt();
	info->setClcd(clcd);

	memset(tempbuffer, 0, sizeof(tempbuffer));
	bytebuffer.getRawBytes(tempbuffer, 4);
	info->setXszt(std::string((char*)tempbuffer));

	memset(tempbuffer, 0, sizeof(tempbuffer));
	bytebuffer.getRawBytes(tempbuffer, 4);
	info->setClpp(std::string((char*)tempbuffer));

	memset(tempbuffer, 0, sizeof(tempbuffer));
	bytebuffer.getRawBytes(tempbuffer, 4);
	info->setClwx(std::string((char*)tempbuffer));

	memset(tempbuffer, 0, sizeof(tempbuffer));
	bytebuffer.getRawBytes(tempbuffer, 5);
	info->setCsys(std::string((char*)tempbuffer));

	memset(tempbuffer, 0, sizeof(tempbuffer));
	bytebuffer.getRawBytes(tempbuffer, 2);
	info->setCb(std::string((char*)tempbuffer));

	memset(tempbuffer, 0, sizeof(tempbuffer));
	bytebuffer.getRawBytes(tempbuffer, 4);
	info->setCllx(std::string((char*)tempbuffer));

	memset(tempbuffer, 0, sizeof(tempbuffer));
	bytebuffer.getRawBytes(tempbuffer, 2);
	info->setHpzl(std::string((char*)tempbuffer));

	memset(tempbuffer, 0, sizeof(tempbuffer));
	bytebuffer.getRawBytes(tempbuffer, 10);
	info->setSsqy(std::string((char*)tempbuffer));

	int32 txsl = bytebuffer.getInt();
	info->setTxsl(txsl);

	for (int32 i = 0; i < info->getTxsl(); ++i) {

		int32 len = bytebuffer.getInt();
		if (len >= MAX_BUFFER_SIZE_02m) {

			Log(LOG_WARN, "CarDescriptionInfoMarshaller::unmarshal - picture size: %llu too big.", len);
			continue;
		}
		
		ST_PICTURE_INFO *newPicture = new ST_PICTURE_INFO;
		newPicture->index = i + 1;
		newPicture->len = len;
		bytebuffer.getRawBytes(newPicture->data, len);
		info->setPicture(newPicture);
	}
}

void CarDescriptionInfoMarshaller::marshal(Format *, DataStructure *dataStructure, library::ByteBuffer &bytebuffer)
{
	if (dataStructure == NULL) return;

	CarDescriptionInfo &info = dynamic_cast<CarDescriptionInfo &>(*dataStructure);
	
	bytebuffer.putRawBytes((const uint8*)(info.getXxbh().c_str()), 15);
	bytebuffer.putRawBytes((const uint8*)(info.getKkbh().c_str()), 18);
	bytebuffer.putRawBytes((const uint8*)(info.getSbbh().c_str()), 18);
	bytebuffer.putRawBytes((const uint8*)(info.getJgsj().c_str()), 24);
	bytebuffer.putRawBytes((const uint8*)(info.getCdbh().c_str()), 10);
	bytebuffer.putRawBytes((const uint8*)(info.getHphm().c_str()), 15);
	bytebuffer.putRawBytes((const uint8*)(info.getHpys().c_str()), 1);
	bytebuffer.putRawBytes((const uint8*)(info.getCwhphm().c_str()), 15);
	bytebuffer.putRawBytes((const uint8*)(info.getCwhpys().c_str()), 1);
	bytebuffer.putRawBytes((const uint8*)(info.getHpyz().c_str()), 1);
	bytebuffer.putFloat(info.getClsd());
	bytebuffer.putFloat(info.getClxs());
	bytebuffer.putInt(info.getClcd());
	bytebuffer.putRawBytes((const uint8*)(info.getXszt().c_str()), 4);
	bytebuffer.putRawBytes((const uint8*)(info.getClpp().c_str()), 4);
	bytebuffer.putRawBytes((const uint8*)(info.getClwx().c_str()), 4);
	bytebuffer.putRawBytes((const uint8*)(info.getCsys().c_str()), 5);
	bytebuffer.putRawBytes((const uint8*)(info.getCb().c_str()), 2);
	bytebuffer.putRawBytes((const uint8*)(info.getCllx().c_str()), 4);
	bytebuffer.putRawBytes((const uint8*)(info.getHpzl().c_str()), 2);
	bytebuffer.putRawBytes((const uint8*)(info.getSsqy().c_str()), 10);
	bytebuffer.putInt(info.getTxsl());

	for (int32 i = 0; i < info.getTxsl(); ++i) {

		uint8 data[MAX_BUFFER_SIZE_02m] = {0};
		uint32 len = sizeof(data) - 1;
		
		info.getPicture(i, data, len);
		bytebuffer.putInt(len);
		bytebuffer.putRawBytes(data, len);
	}
}
