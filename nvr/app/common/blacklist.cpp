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
#include <string.h>
#include "blacklist.h"
#include "../../library/util/log.h"
#include "../../library/util/system.h"
#include "../../library/concurrent/reentrant.h"

using namespace library;
using namespace app;

BlackList::BlackList()
{
	_blackList.clear();
}

BlackList::~BlackList()
{
	_blackList.clear();
}

void BlackList::configUpdate(ST_BALCKLIST_CONFIG_INFO &info)
{
	std::map<std::string, ST_BALCKLIST_CONFIG_INFO>::iterator iter;

	Synchronization autolock(m_lock);
	
	iter = _blackList.find(info.hphm);
	if (iter == _blackList.end()) {
		_blackList.insert(std::pair<std::string, ST_BALCKLIST_CONFIG_INFO>(info.hphm, info));
	}
}

void BlackList::configDrop()
{
	_blackList.clear();
}

bool BlackList::check(ST_CAR_TRAP_INFO &trap)
{
	std::map<std::string, ST_BALCKLIST_CONFIG_INFO>::iterator iter;
	char chphm[MAX_BUFFER_SIZE_064] = {0};
	ST_BALCKLIST_CONFIG_INFO blackConfig;
	std::string hphm;

	Synchronization autolock(m_lock);

	memset(trap.bkxxbh, 0, sizeof(trap.bkxxbh));
	memset(trap.bjlx, 0, sizeof(trap.bjlx));
	memset(trap.lxdh, 0, sizeof(trap.lxdh));
	memset(trap.dxfsbs, 0, sizeof(trap.dxfsbs));

	if (::strlen(trap.stcarinfo.urlInfo.url[0]) > 0 || ::strlen(trap.stcarinfo.urlInfo.url[1]) > 0)
		::memcpy(chphm, trap.stcarinfo.plateFNo, sizeof(trap.stcarinfo.plateFNo));
	else if (::strlen(trap.stcarinfo.urlInfo.url[2]) > 0 || ::strlen(trap.stcarinfo.urlInfo.url[3]) > 0)
		::memcpy(chphm, trap.stcarinfo.plateTNo, sizeof(trap.stcarinfo.plateTNo));
	else
		::memcpy(chphm, trap.stcarinfo.plateFNo, sizeof(trap.stcarinfo.plateFNo));

	///! transfer @param 'hphm' to gb2312
	char in[MAX_BUFFER_SIZE_128] = {0}, out[MAX_BUFFER_SIZE_128] = {0};
	memset(in, 0, sizeof(in)); memset(out, 0, sizeof(out));
	memcpy(in, chphm, sizeof(chphm));
	int32 iRet = System::u2g(in, sizeof(in), out, sizeof(out));
	if (iRet < 0 || strlen(out) <= 0) {
		;
	} else {
		memset(chphm, 0, sizeof(chphm));
		memcpy(chphm, out, strlen(out));
	}
	Log(LOG_DEBUG, "check[%s] for set blacklist.", chphm);
	
	hphm = chphm;
	iter = _blackList.find(hphm);
	if (iter != _blackList.end()) {
		memset(&blackConfig, 0, sizeof(ST_BALCKLIST_CONFIG_INFO));
		blackConfig = iter->second;
		strncpy(trap.bkxxbh, blackConfig.bkxxbh, sizeof(blackConfig.bkxxbh));
		strncpy(trap.bjlx, "黑名单告警", strlen("黑名单告警"));
		return true;
	}

	return false;
}