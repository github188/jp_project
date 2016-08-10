/***************************************************************************
 *   Copyright (C) 2016 by going
 *	 Created on: 2016Äê2ÔÂ18ÈÕ
 *	 Author: going   				   						   *
 *   going369@qq.com   							   					   *
 *   xmldemo.cpp                                                                      *
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
#include "tinyxml.h"
#include "tinystr.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <string>
#include <iostream>
bool initAppsConfig(std::string configFile)
{
	TiXmlElement *pRoot = NULL;
	bool bloadok = false;
	TiXmlDocument doc;
	std::string buf;
	short num;
	if (access(configFile.c_str(), 0) == -1) {
		printf("==================================================\n");
		printf("===== !!! test Config File Not Exist !!! =====\n");
		printf("==================================================\n");
		return false;
	}

	bloadok = doc.LoadFile(configFile.c_str());
	if (!bloadok) {
		printf("==================================================\n");
		printf("===== !!! Load test Config File Fail !!! =====\n");
		printf("==================================================\n");
		return false;
	}

	pRoot = doc.RootElement();
	TiXmlHandle handle(pRoot);

	TiXmlElement *pElem = NULL;
	pElem = handle.FirstChild("connection").FirstChild("lConnectionPort").ToElement();
	num = (short)atoi(pElem->GetText());
	std::cout << "lConnectionPort = " <<num <<"\n";
	pElem = handle.FirstChild("connection").FirstChild("sConnectionPort").ToElement();
	num = (short)atoi(pElem->GetText());
	std::cout << "sConnectionPort = " <<num <<"\n";
	//sending service
	pElem = handle.FirstChild("sendingService").FirstChild("enable").ToElement();
	num = atoi(pElem->GetText()) > 0 ? true : false;
	std::cout << "enable = " <<num <<"\n";

	pElem = handle.FirstChild("sendingService").FirstChild("serverIp").ToElement();
	buf = pElem->GetText();
	std::cout << "serverIp = " << buf <<"\n";

	pElem = handle.FirstChild("sendingService").FirstChild("serverPort").ToElement();
	num = (short)atoi(pElem->GetText());
	std::cout << "serverPort = " <<num <<"\n";

	//picture store

	pElem = handle.FirstChild("pictureStore").FirstChild("enable").ToElement();
	num = atoi(pElem->GetText()) > 0 ? true : false;
	std::cout << "enable = " <<num <<"\n";

	pElem = handle.FirstChild("pictureStore").FirstChild("threadNum").ToElement();
	num = (short)atoi(pElem->GetText());
	std::cout << "sConnectionPort = " <<num <<"\n";

	pElem = handle.FirstChild("pictureStore").FirstChild("urlPrefix").ToElement();
	buf = pElem->GetText();
	std::cout << "urlPrefix = " << buf <<"\n";

	pElem = handle.FirstChild("pictureStore").FirstChild("pictureRoot").ToElement();
	buf = pElem->GetText();
	std::cout << "pictureRoot = " << buf <<"\n";
/*
	pElem = handle.FirstChild("pictureStore").FirstChild("direntNum").ToElement();
	g_dabigate_config.pictureConfig.direntNum = (uint32)atoi(pElem->GetText());
	for (uint32 i = 0; i < g_dabigate_config.pictureConfig.direntNum; ++i) {
		char tmp[32] = {0};
		std::string dir;

		dir.clear();
		sprintf(tmp, "dirent%d", i + 1);
		pElem = handle.FirstChild("pictureStore").FirstChild(tmp).ToElement();
		dir = pElem->GetText();
		g_dabigate_config.pictureConfig.direntVec.push_back(dir);
	}
	pElem = handle.FirstChild("pictureStore").FirstChild("direntSkipOffSize").ToElement();
	g_dabigate_config.pictureConfig.direntSkipOffSize = (uint32)atoi(pElem->GetText());

	//database
	pElem = handle.FirstChild("dbConf").FirstChild("enable").ToElement();
	g_dabigate_config.dbconfig.enable = atoi(pElem->GetText()) > 0 ? true : false;
	pElem = handle.FirstChild("dbConf").FirstChild("dbIp").ToElement();
	g_dabigate_config.dbconfig.dbIp = pElem->GetText();
	pElem = handle.FirstChild("dbConf").FirstChild("dbPort").ToElement();
	g_dabigate_config.dbconfig.dbPort = atoi(pElem->GetText());
	pElem = handle.FirstChild("dbConf").FirstChild("dbUser").ToElement();
	g_dabigate_config.dbconfig.dbUser = pElem->GetText();
	pElem = handle.FirstChild("dbConf").FirstChild("dbPass").ToElement();
	g_dabigate_config.dbconfig.dbPass = pElem->GetText();
	pElem = handle.FirstChild("dbConf").FirstChild("dbSid").ToElement();
	g_dabigate_config.dbconfig.dbSid= pElem->GetText();

	//logmy
	pElem = handle.FirstChild("logMy").FirstChild("logWhere").ToElement();
	g_dabigate_config.logMyConfig.logWhere = (uint16)atoi(pElem->GetText());
	pElem = handle.FirstChild("logMy").FirstChild("logLevel").ToElement();
	g_dabigate_config.logMyConfig.logLevel = (uint16)atoi(pElem->GetText());
	pElem = handle.FirstChild("logMy").FirstChild("logFileSize").ToElement();
	g_dabigate_config.logMyConfig.logFileSize = (uint16)atoi(pElem->GetText());
	pElem = handle.FirstChild("logMy").FirstChild("logFileName").ToElement();
	g_dabigate_config.logMyConfig.logFileName = pElem->GetText();
*/
	return true;
}

////////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{

	if (!initAppsConfig(std::string("test.xml"))) {
		return -1;
	}

}




