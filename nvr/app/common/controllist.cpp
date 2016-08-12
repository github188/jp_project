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
#include "controllist.h"
#include "../../library/util/log.h"
#include "../../library/util/system.h"
#include "../../library/concurrent/reentrant.h"

using namespace library;
using namespace app;

HpRegExpressTransfer::HpRegExpressTransfer()
{
}

HpRegExpressTransfer::HpRegExpressTransfer(const char *pcpattern)
{
	generatereg(pcpattern);
}

HpRegExpressTransfer::~HpRegExpressTransfer()
{
	destroyreg();
}

int32 HpRegExpressTransfer::setpattern(const char *pcpattern)
{
	if (pcpattern == NULL) return STATUS_ERROR;
	
	return generatereg(pcpattern);
}

bool HpRegExpressTransfer::search(const char *pcStr)
{
	int32 iRet = STATUS_ERROR;
    const size_t nmatch = 1;
	regmatch_t pmatch[1];
	
	if (pcStr == NULL) return false;

	iRet = ::regexec(&reg, pcStr, nmatch, pmatch, 0);
	if (iRet == REG_NOMATCH)
	{
		return false;
	}
	else
	{
		Log(LOG_INFO, "Matched for regex.");
		return true;
	}

	return true;
}

int32 HpRegExpressTransfer::generatereg(const char *pcpattern)
{
	char tbuf[MAX_BUFFER_SIZE_01k] = {0};
	const char *pcstart = "^[^\\x00-\\xff]";
	const char *pcleast = "[a-z0-9A-Z]+";
	const char *pcsingl = "[a-z0-9A-Z]";
	int32 iRet = STATUS_ERROR;
	const char *pctemp = NULL;
	bool bspec = false;
	uint32 i = 0;
	
	if (pcpattern == NULL) return STATUS_ERROR;

	pctemp = pcpattern;
	for (i = 0; i < ::strlen(pcpattern); ++i)
	{
		if (pctemp[i] == '*')
		{
			if (i == 0)
			{
				bspec = true;
				::strncat(tbuf, pcstart, ::strlen(pcstart));
			}
			else
			{
				if (i < CHINESE_CHARACTORS_IN_BYTES)
				{
					if (!bspec)
						tbuf[::strlen(tbuf)] +=  pctemp[i];
					else
						::strncat(tbuf, pcleast, ::strlen(pcleast));
				}
				else
				{
					::strncat(tbuf, pcleast, ::strlen(pcleast));
				}
			}
		}
		else
		{
			if (i < CHINESE_CHARACTORS_IN_BYTES)
			{
				if (!bspec)
					tbuf[::strlen(tbuf)] +=  pctemp[i];
				else
					::strncat(tbuf, pcsingl, ::strlen(pcsingl));
			}
			else
			{
				if (pctemp[i] == '?')
					::strncat(tbuf, pcsingl, ::strlen(pcsingl));
				else
					tbuf[::strlen(tbuf)] +=  pctemp[i];
			}
		}
	}
	Log(LOG_DEBUG, "regex express: %s-%s", pcpattern, tbuf);

	iRet = ::regcomp(&reg, tbuf, (REG_ICASE | REG_NOSUB | REG_NEWLINE | REG_EXTENDED));
	if (iRet != STATUS_SUCCESS)
	{
		Log(LOG_ERROR, "formula regex express:%s error.", pcpattern);
		return STATUS_ERROR;
	}

	return STATUS_SUCCESS;
} 

int32 HpRegExpressTransfer::destroyreg()
{
	::regfree(&reg);

	return STATUS_SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////
ControlList::ControlList()
{
	concretelist.clear();
	abstractlist.clear();
}

ControlList::~ControlList()
{
	concretelist.clear();
	abstractlist.clear();
}

void ControlList::configUpdate(ST_CONTROLLIST_CONFIG_INFO &info)
{
	std::map<std::string, ST_CONTROLLIST_CONFIG_INFO>::iterator iter;
	char *posq = NULL, *posa = NULL;

	Synchronization autolock(m_lock);

	std::string hphm = info.hphm;
	posq = ::strchr(info.hphm, '?');
	posa = ::strchr(info.hphm, '*');
	if (posq == NULL && posa == NULL)
	{
		iter = concretelist.find(hphm);
		if (iter == concretelist.end())
			concretelist.insert(std::pair<std::string, ST_CONTROLLIST_CONFIG_INFO>(hphm, info));
		else
			iter->second = info;
	}
	else
	{
		iter = abstractlist.find(hphm);
		if (iter == abstractlist.end())
			abstractlist.insert(std::pair<std::string, ST_CONTROLLIST_CONFIG_INFO>(hphm, info));
		else
			iter->second = info;
	}
}

void ControlList::configDrop()
{
	concretelist.clear();
	abstractlist.clear();
}

bool ControlList::check(ST_CAR_TRAP_INFO &trap)
{
	std::map<std::string, ST_CONTROLLIST_CONFIG_INFO>::iterator it;
	ST_CONTROLLIST_CONFIG_INFO stControlConfig;
	char chphm[MAX_BUFFER_SIZE_032] = {0};
	std::string hphm;

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
	Log(LOG_DEBUG, "check[%s] for set control.", chphm);

	memset(&stControlConfig, 0, sizeof(ST_CONTROLLIST_CONFIG_INFO));
	hphm = chphm;
	it = concretelist.find(hphm);
	if (it != concretelist.end())
	{
		/* alarm happens */
		stControlConfig = it->second;

		strncpy(trap.bkxxbh, stControlConfig.bkxxbh, sizeof(stControlConfig.bkxxbh));
		strncpy(trap.bjlx, "下陣御少", strlen("下陣御少"));
		strncpy(trap.lxdh, stControlConfig.lxdh, sizeof(stControlConfig.lxdh));
		strncpy(trap.dxfsbs, stControlConfig.dxfsbs, sizeof(stControlConfig.dxfsbs));
		
		Log(LOG_INFO, "control alarm[%s] happens", chphm);
		return true;
	}

	it = abstractlist.begin();
	for (; it != abstractlist.end(); ++it)
	{
		std::string hphm = it->first;
		HpRegExpressTransfer hpregex(hphm.c_str());
		bool bright = false;

		bright = hpregex.search(chphm);
		if (bright)
		{
			/* alarm happens */
			stControlConfig = it->second;
			
			strncpy(trap.bkxxbh, stControlConfig.bkxxbh, sizeof(stControlConfig.bkxxbh));
			strncpy(trap.bjlx, "下陣御少", strlen("下陣御少"));
			strncpy(trap.lxdh, stControlConfig.lxdh, sizeof(stControlConfig.lxdh));
			strncpy(trap.dxfsbs, stControlConfig.dxfsbs, sizeof(stControlConfig.dxfsbs));
			
			Log(LOG_INFO, "control alarm[%s] happens", chphm);
			return true;
		}
	}

	return false;
}