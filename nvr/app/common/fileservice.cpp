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
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/statfs.h>

#include "fileservice.h"
#include "picturepainer.h"
#include "../../library/util/log.h"
#include "../protocol/cardescriptioninfo.h"
#include "../../library/concurrent/threadpool.h"

using namespace library;
using namespace corelib;
using namespace corelib::protocol;
using namespace app;

extern ST_DABIGATE_CONFIG_INFO g_dabigate_config;

////////////////////////////////////////////////////////////////////////////////
FileService::FileService(DbService &dbservice) : m_dbservice(dbservice), threadpool_(NULL)
{
	serviceInit();
}

FileService::~FileService()
{
	delete threadpool_;
	threadpool_ = NULL;
}

void FileService::run()
{
}

void FileService::add(CarDescriptionInfo *carInfo)
{
	if (carInfo == NULL) return;

	ST_PICTURES_INFO *pictures = new ST_PICTURES_INFO;
	memset(pictures, 0, sizeof(ST_PICTURES_INFO));

	ST_CAR_DESCRIPTION_INFO *carDesInfo = new ST_CAR_DESCRIPTION_INFO;
	memset(carDesInfo, 0, sizeof(ST_CAR_DESCRIPTION_INFO));

	carInfo->getPictures(*pictures);
	carInfo->getCarInfo(*carDesInfo);

	// file
	std::string currentStoreDir = getStoreDir();
	std::string urlPostfix = generateStorePath(carDesInfo);
	if (storeRoot_[storeRoot_.size() - 1] != '/') storeRoot_ += "/";

	// url assemble
//	for (uint32 n = 0; n < carDesInfo->urlInfo.urlNum; ++n)
//	{
//		char d[16] = {0};
//		std::string url;
//		
//		if (pictures->data[n].len <= 0) continue;
//
//		sprintf(d, "%02d.jpg", n + 1);
//		url += urlHeader_;
//		if (url[url.size() -1] != '/') url += "/";
//		url += currentStoreDir; url += "/";
//		url += urlPostfix;
//		url += d;
//		strncpy(carDesInfo->urlInfo.url[n], url.c_str(), MAX_BUFFER_SIZE_512);
//		//Log(LOG_DEBUG, "url: %s", carDesInfo->urlInfo.url[n]);
//	}
//////////////////////////////////////////////////////////////
	for (uint32 n = 0; n < carDesInfo->urlInfo.urlNum; ++n)
	{
		char d[16] = {0};
		std::string url;
		
		if (pictures->data[n].len <= 0) continue;

		sprintf(d, "%02d.jpg", pictures->data[n].index);
		url += urlHeader_;
		if (url[url.size() -1] != '/') url += "/";
		url += currentStoreDir; url += "/";
		url += urlPostfix;
		url += d;
		strncpy(carDesInfo->urlInfo.url[n], url.c_str(), MAX_BUFFER_SIZE_512);
//		Log(LOG_DEBUG, "url[%d]: %s", pictures->data[n].index, carDesInfo->urlInfo.url[n]);
	}

	if (g_dabigate_config.pictureConfig.pictureStoreEnable) {
		std::string filePrefix = storeRoot_;
		filePrefix += currentStoreDir;
		filePrefix += "/";
		filePrefix += urlPostfix;
		//NOTE: @param pictures will be deleted by PicturePainer
		threadpool_->excute(new PicturePainer(filePrefix, pictures));
	} else {
		delete pictures;
	}

	if (g_dabigate_config.sendingService.enable) {
		m_sending.send(carDesInfo);
	}

	//NOTE: @param carDesInfo will be deleted by dbService
	if (g_dabigate_config.dbconfig.enable) {
		m_dbservice.insert(carDesInfo);
	} else {
		delete carDesInfo;
	}
}

void FileService::serviceInit()
{
	threadpool_ = new ThreadPool(g_dabigate_config.pictureConfig.threadNum);
	urlHeader_ = g_dabigate_config.pictureConfig.urlPrefix;
	storeRoot_ = g_dabigate_config.pictureConfig.pictureRoot;
	storeDirections = g_dabigate_config.pictureConfig.direntVec;
	alarmSize_ = g_dabigate_config.pictureConfig.direntSkipOffSize;

//	Log(LOG_DEBUG, "urlPrefix: %s", urlHeader_.c_str());
//	Log(LOG_DEBUG, "pictureRoot: %s", storeRoot_.c_str());
//	Log(LOG_DEBUG, "diskRemainSize: %llu", alarmSize_);
	for (uint32 i = 0; i < storeDirections.size(); ++i) {
		Log(LOG_DEBUG, "dirent[%d]: %s", i, storeDirections[i].c_str());
	}
}

std::string FileService::generateStorePath(ST_CAR_DESCRIPTION_INFO *info)
{
	if (info == NULL) return "";

	char filePath[MAX_BUFFER_SIZE_384] = {0};
	char blockSn[MAX_BUFFER_SIZE_064] = {0};
	int32 iIntArry[10] = {0};
	
	int32 iRet = sscanf(info->ptime, "%04d-%02d-%02d %02d:%02d:%02d.%03d", 
						&iIntArry[0], &iIntArry[1], &iIntArry[2], &iIntArry[3], 
						&iIntArry[4], &iIntArry[5], &iIntArry[6]);
	if (iRet != 7)
	{
		Log(LOG_ERROR, "time format: time:%s errno:%s.", info->ptime, ::strerror(errno));
		return "";
	}

	strncpy(blockSn, info->blockSn, sizeof(info->blockSn));
	sprintf(filePath, "%04d/%02d/%02d/%02d/%s/%02d%02d%02d%03d", iIntArry[0], 
			iIntArry[1], iIntArry[2], iIntArry[3], blockSn, iIntArry[3], iIntArry[4],
			iIntArry[5], iIntArry[6]);

	return filePath;
}

std::string FileService::getStoreDir()
{
	static uint32 index = 0;
	uint64 remainSpace = 0;
	
	if (storeDirections.size() <= 0) return "";

	remainSpace = getDiskRemainSpace(storeRoot_, storeDirections[index]);
	while (remainSpace <= alarmSize_) // 2G remains
	{
		index = (index + 1) % storeDirections.size();
		remainSpace = getDiskRemainSpace(storeRoot_, storeDirections[index]);
	}

	return storeDirections[index];
}

uint64 FileService::getDiskRemainSpace(std::string &rootPrefix, std::string &dir)
{
	struct statfs stDiskInfo;
	uint64 freeSize = 0;
	std::string fullpath;

	fullpath.clear();
	::memset(&stDiskInfo, 0, sizeof(struct statfs));

	fullpath += rootPrefix;
	fullpath += dir;

	int32 iRet = ::statfs(fullpath.c_str(), &stDiskInfo);
	if (iRet < 0)
	{
		Log(LOG_ERROR, "check %s, error.", fullpath.c_str());
		return 0;
	}

	freeSize = stDiskInfo.f_bsize * stDiskInfo.f_bavail;
	freeSize = freeSize/(1024 * 1024); //MB
	Log(LOG_INFO, " check %s, freeSize = %ul ", fullpath.c_str(), freeSize);

	return freeSize;
}