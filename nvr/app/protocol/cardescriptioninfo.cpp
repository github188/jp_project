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
#include "../../library/util/log.h"
#include "cardescriptioninfo.h"

using namespace library;
using namespace app;

const std::string CarDescriptionInfo::ID_CAR_INFO = "2.1101";
////////////////////////////////////////////////////////////////////////////////
CarDescriptionInfo::CarDescriptionInfo()
{
	pictures.clear();
}

CarDescriptionInfo::~CarDescriptionInfo()
{
	std::vector<ST_PICTURE_INFO *>::iterator iter;

	iter = pictures.begin();
	for (; iter != pictures.end(); ++iter) {

		ST_PICTURE_INFO *picture = *iter;
		if (picture != NULL) delete picture;
		picture = NULL;
	}

	pictures.clear();
}

std::string CarDescriptionInfo::getDataStructureType() const
{
	return CarDescriptionInfo::ID_CAR_INFO;
}

void CarDescriptionInfo::copyDataStructure(const DataStructure *src)
{
	// Protect against invalid self assignment.
    if (this == src) {
        return;
    }
	
	const CarDescriptionInfo *carInfo = dynamic_cast<const CarDescriptionInfo *>(src);
	DataStructure::copyDataStructure(src);
	this->setSessionId(carInfo->getSessionId());
	this->setXxbh(carInfo->getXxbh());
	this->setKkbh(carInfo->getKkbh());
	this->setSbbh(carInfo->getSbbh());
	this->setJgsj(carInfo->getJgsj());
	this->setCdbh(carInfo->getCdbh());
	this->setHphm(carInfo->getHphm());
	this->setHpys(carInfo->getHpys());
	this->setCwhphm(carInfo->getCwhphm());
	this->setCwhpys(carInfo->getCwhpys());
	this->setHpyz(carInfo->getHpyz());
	this->setClsd(carInfo->getClsd());
	this->setClxs(carInfo->getClxs());
	this->setClcd(carInfo->getClcd());
	this->setXszt(carInfo->getXszt());
	this->setClpp(carInfo->getClpp());
	this->setClwx(carInfo->getClwx());
	this->setCsys(carInfo->getCsys());
	this->setCb(carInfo->getCb());
	this->setCllx(carInfo->getCllx());
	this->setHpzl(carInfo->getHpzl());
	this->setSsqy(carInfo->getSsqy());
	this->setTxsl(carInfo->getTxsl());
}

bool CarDescriptionInfo::equals(const DataStructure *value) const
{
	if (this == value) return true;

	const CarDescriptionInfo *info = dynamic_cast<const CarDescriptionInfo *>(value);
	if (this->xxbh != info->getXxbh()) return false;
	if (this->kkbh != info->getKkbh()) return false;
	if (this->sbbh != info->getKkbh()) return false;
	if (this->jgsj != info->getJgsj()) return false;
	if (this->cdbh != info->getCdbh()) return false;
	if (this->hphm != info->getHphm()) return false;
	if (this->hpys != info->getHpys()) return false;
	if (this->cwhphm != info->getCwhphm()) return false;
	if (this->cwhpys != info->getCwhpys()) return false;
	if (this->hpyz != info->getHpyz()) return false;
	if (this->clsd != info->getClsd()) return false;
	if (this->clxs != info->getClxs()) return false;
	if (this->cscd != info->getClcd()) return false;
	if (this->xszt != info->getXszt()) return false;
	if (this->clpp != info->getClpp()) return false;
	if (this->clwx != info->getClwx()) return false;
	if (this->csys != info->getCsys()) return false;
	if (this->cb != info->getCb()) return false;
	if (this->cllx != info->getCllx()) return false;
	if (this->hpzl != info->getHpzl()) return false;
	if (this->ssqy != info->getSsqy()) return false;
	if (this->txsl != info->getTxsl()) return false;

	return true;
}

void CarDescriptionInfo::setXxbh(const std::string &str)
{
	this->xxbh = trimSuffixSpace(str);
}

std::string CarDescriptionInfo::getXxbh()const
{
	return this->xxbh;
}

void CarDescriptionInfo::setKkbh(const std::string &str)
{
	this->kkbh = trimSuffixSpace(str);
}

std::string CarDescriptionInfo::getKkbh()const
{
	return this->kkbh;
}

void CarDescriptionInfo::setSbbh(const std::string &str)
{
	this->sbbh = trimSuffixSpace(str);
}

std::string CarDescriptionInfo::getSbbh()const
{
	return this->sbbh;
}

void CarDescriptionInfo::setJgsj(const std::string &str)
{
	this->jgsj = trimSuffixSpace(str);
}

std::string CarDescriptionInfo::getJgsj()const
{
	return this->jgsj;
}

void CarDescriptionInfo::setCdbh(const std::string &str)
{
	this->cdbh = trimSuffixSpace(str);
}

std::string CarDescriptionInfo::getCdbh()const
{
	return this->cdbh;
}

void CarDescriptionInfo::setHphm(const std::string &str)
{
	this->hphm = trimSuffixSpace(str);
}

std::string CarDescriptionInfo::getHphm()const
{
	return this->hphm;
}

void CarDescriptionInfo::setHpys(const std::string &str)
{
	this->hpys = trimSuffixSpace(str);
}

std::string CarDescriptionInfo::getHpys()const
{
	return this->hpys;
}

void CarDescriptionInfo::setCwhphm(const std::string &str)
{
	this->cwhphm = trimSuffixSpace(str);
}

std::string CarDescriptionInfo::getCwhphm()const
{
	return this->cwhphm;
}

void CarDescriptionInfo::setCwhpys(const std::string &str)
{
	this->cwhpys = trimSuffixSpace(str);
}

std::string CarDescriptionInfo::getCwhpys()const
{
	return this->cwhpys;
}

void CarDescriptionInfo::setHpyz(const std::string &str)
{
	this->hpyz = trimSuffixSpace(str);
}

std::string CarDescriptionInfo::getHpyz()const
{
	return this->hpyz;
}

void CarDescriptionInfo::setClsd(float speed)
{
	this->clsd = speed;
}

float CarDescriptionInfo::getClsd()const
{
	return this->clsd;
}

void CarDescriptionInfo::setClxs(float limitSpeed)
{
	this->clxs = limitSpeed;
}

float CarDescriptionInfo::getClxs()const
{
	return this->clxs;
}

void CarDescriptionInfo::setClcd(int32 clcd)
{
	this->cscd = clcd;
}

int32 CarDescriptionInfo::getClcd()const
{
	return this->cscd;
}

void CarDescriptionInfo::setXszt(const std::string &str)
{
	this->xszt = trimSuffixSpace(str);
}

std::string CarDescriptionInfo::getXszt()const
{
	return this->xszt;
}

void CarDescriptionInfo::setClpp(const std::string &str)
{
	this->clpp = trimSuffixSpace(str);
}

std::string CarDescriptionInfo::getClpp()const
{
	return this->clpp;
}

void CarDescriptionInfo::setClwx(const std::string &str)
{
	this->clwx = trimSuffixSpace(str);
}

std::string CarDescriptionInfo::getClwx()const
{
	return this->clwx;
}

void CarDescriptionInfo::setCsys(const std::string &str)
{
	this->csys = trimSuffixSpace(str);
}

std::string CarDescriptionInfo::getCsys()const
{
	return this->csys;
}

void CarDescriptionInfo::setCb(const std::string &str)
{
	this->cb = trimSuffixSpace(str);
}

std::string CarDescriptionInfo::getCb()const
{
	return this->cb;
}

void CarDescriptionInfo::setCllx(const std::string &str)
{
	this->cllx = trimSuffixSpace(str);
}

std::string CarDescriptionInfo::getCllx()const
{
	return this->cllx;
}

void CarDescriptionInfo::setHpzl(const std::string &str)
{
	this->hpzl = trimSuffixSpace(str);
}

std::string CarDescriptionInfo::getHpzl()const
{
	return this->hpzl;
}

void CarDescriptionInfo::setSsqy(const std::string &str)
{
	this->ssqy = trimSuffixSpace(str);
}

std::string CarDescriptionInfo::getSsqy()const
{
	return this->ssqy;
}

void CarDescriptionInfo::setTxsl(int32 num)
{
	this->txsl = num;
}

int32 CarDescriptionInfo::getTxsl()const
{
	return this->txsl;
}

void CarDescriptionInfo::setPicture(ST_PICTURE_INFO *picture)
{
	this->pictures.push_back(picture);
}

void CarDescriptionInfo::getPicture(int32 index, uint8 *data, uint32 &len)
{
	if (data == NULL || (index + 1) > getPictureNumber()) {
		len = 0;
		return;
	}

	ST_PICTURE_INFO *picture = this->pictures[index];
	if (picture != NULL) {
		
		if (picture->len > len) {
			Log(LOG_ERROR, "CarDescriptionInfo::getPicture - buffer too small.");
			return;
		}
		len = picture->len;
		memcpy(data, picture->data, len);
	}

	return;
}

int32 CarDescriptionInfo::getPictureNumber() const
{
	return this->pictures.size();
}

void CarDescriptionInfo::getCarInfo(ST_CAR_DESCRIPTION_INFO &carInfo)
{
	strncpy(carInfo.vehiSn, xxbh.c_str(), sizeof(carInfo.vehiSn));
	strncpy(carInfo.blockSn, kkbh.c_str(), sizeof(carInfo.blockSn));
	strncpy(carInfo.devSn, sbbh.c_str(), sizeof(carInfo.devSn));
	strncpy(carInfo.ptime, jgsj.c_str(), sizeof(carInfo.ptime));
	strncpy(carInfo.laneSn, cdbh.c_str(), sizeof(carInfo.laneSn));
	strncpy(carInfo.plateFNo, hphm.c_str(), sizeof(carInfo.plateFNo));
	strncpy(carInfo.plateFCol, hpys.c_str(), sizeof(carInfo.plateFCol));
	strncpy(carInfo.plateTNo, cwhphm.c_str(), sizeof(carInfo.plateTNo));
	strncpy(carInfo.plateTCol, cwhpys.c_str(), sizeof(carInfo.plateTCol));
	strncpy(carInfo.ftEquel, hpyz.c_str(), sizeof(carInfo.ftEquel));
	carInfo.fSpeed = clsd;
	carInfo.fSpeedLim = clxs;
	carInfo.vehiLen = cscd;
	strncpy(carInfo.runState, xszt.c_str(), sizeof(carInfo.runState));
	strncpy(carInfo.vehiBrand, clpp.c_str(), sizeof(carInfo.vehiBrand));
	strncpy(carInfo.vehiShape, clwx.c_str(), sizeof(carInfo.vehiShape));
	strncpy(carInfo.vehiCol, csys.c_str(), sizeof(carInfo.vehiCol));
	strncpy(carInfo.vehiCode, cb.c_str(), sizeof(carInfo.vehiCode));
	strncpy(carInfo.vehiType, cllx.c_str(), sizeof(carInfo.vehiType));
	strncpy(carInfo.plateType, hpzl.c_str(), sizeof(carInfo.plateType));
	strncpy(carInfo.area, ssqy.c_str(), sizeof(carInfo.area));
	carInfo.urlInfo.urlNum = pictures.size();

	/* 添加解包时间 */
	char cTime[32] = {0};
	struct tm *pstTm;
	time_t timep;

	timep = time(NULL);
	pstTm = gmtime(&timep); /* 获取GMT时间: 换算成北京时间需要 + 8 */
	sprintf(cTime, "%04d-%02d-%02d %02d:%02d:%02d", pstTm->tm_year + 1900, 
			pstTm->tm_mon + 1, pstTm->tm_mday, pstTm->tm_hour + 8, pstTm->tm_min,
			pstTm->tm_sec);
	::memcpy(carInfo.arrivetime, cTime, ::strlen(cTime));
}

void CarDescriptionInfo::getPictures(ST_PICTURES_INFO &picts)
{
	std::vector<ST_PICTURE_INFO *>::iterator iter;

	picts.num = pictures.size();
	
	iter = pictures.begin();
	for (int32 i = 0; iter != pictures.end(); ++iter, ++i)
	{
		picts.data[i] = **iter;
	}
}

std::string CarDescriptionInfo::trimSuffixSpace(char *data, int32 size)
{
	if (data == NULL || size == 0) return std::string("");

	char *p = data + (size - 1);
	while (p && *p == ' ') {
		*p = '\0';
		p--;
	}

	return std::string(data);
}

std::string CarDescriptionInfo::trimSuffixSpace(const std::string &data)
{
	char buffer[MAX_BUFFER_SIZE_04k] = {0}; // large enough for a collum
	
	if (data.size() <= 0 || data.size() > sizeof(buffer)) {
		Log(LOG_WARN, "CarDescriptionInfo::trimSuffixSpace - data size too small/big: %u", data.size());
		return std::string("");
	}
	
	memcpy(buffer, data.c_str(), data.size());
	char *p = buffer + (data.size() - 1);
	while (p && *p == ' ') {
		*p = '\0';
		p--;
	}

	return std::string(buffer);
}