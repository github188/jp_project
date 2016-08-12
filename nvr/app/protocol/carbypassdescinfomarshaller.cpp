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
#include "carbypassdescinfomarshaller.h"
#include "../../library/util/system.h"
#include "../../library/util/log.h"
#include "carbypassdescinfo.h"

using namespace library;
using namespace corelib;
using namespace corelib::MTP_NETWORK;
using namespace corelib::protocol;
using namespace app;

CarBypassDescInfoMarshaller::CarBypassDescInfoMarshaller()
{
}

CarBypassDescInfoMarshaller::~CarBypassDescInfoMarshaller()
{
}

CP::DataStructure* CarBypassDescInfoMarshaller::createObject() const
{
	return new CarBypassDescInfo();
}

std::string CarBypassDescInfoMarshaller::getDataStructureType() const
{
	return CarBypassDescInfo::ID_CARBYPASS_INFO;
}

void CarBypassDescInfoMarshaller::unmarshal(CP::Format *format, CP::DataStructure *dataStructure, ByteBuffer &bytebuffer)
{
	uint8 tempbuffer[64] = {0};
	
	if (dataStructure == NULL) return;

	CarBypassDescInfo *info = dynamic_cast<CarBypassDescInfo *>(dataStructure);

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

	float clsd = bytebuffer.getFloat();
	info->setClsd(clsd);

	float clxs = bytebuffer.getFloat();
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

	ST_PICTURE_URL_INFO urlInfo;
	memset(&urlInfo, 0, sizeof(ST_PICTURE_URL_INFO));
	urlInfo.urlNum = txsl;
	
	for (int32 i = 0; i < info->getTxsl(); ++i) {

		int32 len = bytebuffer.getInt();
		if (len >= MAX_BUFFER_SIZE_512) {

			Log(LOG_WARN, "CarBypassDescInfoMarshaller::unmarshal - url size: %llu too big.", len);
			continue;
		}
		bytebuffer.getRawBytes((uint8*)urlInfo.url[i], len);
		
		//Log(LOG_DEBUG, "CarBypassDescInfoMarshaller::unmarshal - url[%d]: %s", i + 1, urlInfo.url[i]);
	}
	info->setUrls(urlInfo);
}

void CarBypassDescInfoMarshaller::marshal(CP::Format *format, CP::DataStructure *dataStructure, ByteBuffer &bytebuffer)
{
	char buf[128] = {0}, in[256] = {0}, out[256] = {0};
	
	if (dataStructure == NULL) return;

	CarBypassDescInfo &info = dynamic_cast<CarBypassDescInfo &>(*dataStructure);
	
	bytebuffer.putRawBytes((const uint8*)(info.getXxbh().c_str()), 15);
	bytebuffer.putRawBytes((const uint8*)(info.getKkbh().c_str()), 18);
	bytebuffer.putRawBytes((const uint8*)(info.getSbbh().c_str()), 18);
	bytebuffer.putRawBytes((const uint8*)(info.getJgsj().c_str()), 24);
	bytebuffer.putRawBytes((const uint8*)(info.getCdbh().c_str()), 10);

int32 iRet = -1;
#if(defined(_ZH_PLATFORM_))
	//transfer to gb2312
	memset(buf, 0, sizeof(buf));
	memset(in, 0, sizeof(in));
	memset(out, 0, sizeof(out));

	if (strlen(info.getHphm().c_str()) > 0)
		memcpy(in, info.getHphm().c_str(), strlen(info.getHphm().c_str()));
	iRet = System::u2g(in, sizeof(in), out, sizeof(out));
	if (iRet < 0 || strlen(out) <= 0) {
		bytebuffer.putRawBytes((const uint8*)(info.getHphm().c_str()), 15);
	} else {
		bytebuffer.putRawBytes((const uint8*)(out), 15);
	}
#else
	bytebuffer.putRawBytes((const uint8*)(info.getHphm().c_str()), 15);
#endif

	bytebuffer.putRawBytes((const uint8*)(info.getHpys().c_str()), 1);

#if(defined(_ZH_PLATFORM_))
	//transfer to gb2312
	memset(buf, 0, sizeof(buf));
	memset(in, 0, sizeof(in));
	memset(out, 0, sizeof(out));

	if (strlen(info.getCwhphm().c_str()) > 0)
		memcpy(in, info.getCwhphm().c_str(), strlen(info.getCwhphm().c_str()));
	iRet = System::u2g(in, sizeof(in), out, sizeof(out));
	if (iRet < 0 || strlen(out) <= 0) {
		bytebuffer.putRawBytes((const uint8*)(info.getHphm().c_str()), 15);
	} else {
		bytebuffer.putRawBytes((const uint8*)(out), 15);
	}
#else
	bytebuffer.putRawBytes((const uint8*)(info.getCwhphm().c_str()), 15);
#endif
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


	ST_PICTURE_URL_INFO urlInfo;
	memset(&urlInfo, 0, sizeof(ST_PICTURE_URL_INFO));
	urlInfo = info.getUrls();
	for (int32 i = 0; i < info.getTxsl(); ++i) {
		int32 len = strlen(urlInfo.url[i]);	
		bytebuffer.putInt(len);
		bytebuffer.putRawBytes((const uint8*)(urlInfo.url[i]), len);
	}
}