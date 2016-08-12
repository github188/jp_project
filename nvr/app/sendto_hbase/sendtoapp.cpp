/***************************************************************************
 *   Copyright (C) 2016 by going
 *	 Created on: 2016Äê2ÔÂ16ÈÕ
 *	 Author: going   				   						   *
 *   going369@qq.com   							   					   *
 *   sendto_hbase.cpp                                                                      *
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <netinet/in.h>

#include "sendto_hbase.h"
#include "../../library/util/log.h"
#include "../thirdlib/xml/tinystr.h"
#include "../thirdlib/xml/tinyxml.h"
#include "../protocol/zhuhaiformat.h"
#include "../../library/util/system.h"
#include "../common/defaultIOHandler.h"
#include "../../library/util/singleton.h"
#include "../protocol/consumerheartbeat.h"
#include "../../library/util/logmanager.h"
#include "../../app/protocol/zhuhaiformat.h"
#include "../../corelib/network/connection.h"
#include "../../library/util/activelibrary.h"
#include "../../corelib/network/ioconnector.h"
#include "../../corelib/common/activecorelib.h"
#include "../../corelib/protocol/formatfactory.h"
#include "../../corelib/network/connectionmanager.h"
#include "../../corelib/network/connectionhandler.h"
#include "../../library/buffer/localbytearraybuffer.h"
#include "../../library/buffer/remotebytearraybuffer.h"

using namespace library;
using namespace corelib;
using namespace corelib::protocol;
using namespace corelib::MTP_NETWORK;
using namespace app;

std::map<std::string, ST_DEVICE_INFO> g_devinfomap;
ST_TRAPAPP_CONFIG_INFO g_trapapp_config;
unsigned short datastore_port;
std::string datastore_ip;
bool initAppsConfig(std::string configFile)
{
	TiXmlElement *pRoot = NULL;
	bool bloadok = false;
	TiXmlDocument doc;

	if (access(configFile.c_str(), 0) == -1) {
		printf("==================================================\n");
		printf("===== !!! Tohbase Config File Not Exist !!! ======\n");
		printf("==================================================\n");
		return false;
	}

	bloadok = doc.LoadFile(configFile.c_str());
	if (!bloadok) {
		printf("==================================================\n");
		printf("===== !!! Togbase Trapapp Config File Fail !!! ======\n");
		printf("==================================================\n");
		return false;
	}

	pRoot = doc.RootElement();
	TiXmlHandle handle(pRoot);

	TiXmlElement *pElem = NULL;
	pElem = handle.FirstChild("connection").FirstChild("server").ToElement();
	g_trapapp_config.serverIp = pElem->GetText();
	pElem = handle.FirstChild("connection").FirstChild("serverPort").ToElement();
	g_trapapp_config.serverPort = (uint16)atoi(pElem->GetText());

//tohbase
	pElem = handle.FirstChild("datastore").FirstChild("datastore_ip").ToElement();
	datastore_ip = pElem->GetText();
	pElem = handle.FirstChild("datastore").FirstChild("datastore_port").ToElement();
	datastore_port = (uint16)atoi(pElem->GetText());


	//logmy
	pElem = handle.FirstChild("logMy").FirstChild("logWhere").ToElement();
	g_trapapp_config.logMyConfig.logWhere = (uint16)atoi(pElem->GetText());
	pElem = handle.FirstChild("logMy").FirstChild("logLevel").ToElement();
	g_trapapp_config.logMyConfig.logLevel = (uint16)atoi(pElem->GetText());
	pElem = handle.FirstChild("logMy").FirstChild("logFileSize").ToElement();
	g_trapapp_config.logMyConfig.logFileSize = (uint16)atoi(pElem->GetText());
	pElem = handle.FirstChild("logMy").FirstChild("logFileName").ToElement();
	g_trapapp_config.logMyConfig.logFileName = pElem->GetText();

	return true;
}

bool sendHearbeat(Connection *connection)
{
	if (connection == NULL) return false;

	ConsumerHeartBeatInfo heartBeat;
	heartBeat.setStatus(0);
	heartBeat.setSpecialNumber(0xefeffefe);
	NETBUFFER netBuffer(1024);

	Format *format = FormatFactory::getInstance().findFormat("zhuhai");
	format->marshal(&heartBeat, netBuffer);

	bool bSendOk = connection->send((const char *)netBuffer.array(), netBuffer.limit());
	if (bSendOk) {
		Log(LOG_DEBUG, "trapapp::sendHearbeat - send heartbeat OK.");
	} else {
		Log(LOG_ERROR, "trapapp::sendHearbeat - send heartbeat FAIL.");
	}

	return bSendOk;
}

int main(int argc, char **argv)
{
	/* daemonize */
	::daemon(1,1);

	system("rm -f tohbase.log");
	signal(SIGPIPE, SIG_IGN);
	if (!initAppsConfig(std::string("tohbaseConfig.xml"))) {
		return -1;
	}

	DefaultLog::initialize(g_trapapp_config.logMyConfig.logLevel, g_trapapp_config.logMyConfig.logWhere,
						   g_trapapp_config.logMyConfig.logFileName, g_trapapp_config.logMyConfig.logFileSize);

	ActiveLibrary::initializeLibrary();
	ActiveCorelib::initializeCoreLibrary();

	Log(LOG_INFO, "**************************************************");
	Log(LOG_INFO, "**************************************************");
	Log(LOG_INFO,  "*   tohbase Build Time: %s %s     *", __DATE__, __TIME__);
	Log(LOG_INFO, "**************************************************");
	Log(LOG_INFO, "**************************************************");

	FormatFactory::getInstance().registerFormat("zhuhai", new ZhuHaiFormat((uint8)1));
	//DatatbaseOracle oracle(g_trapapp_config.dbconfig.dbIp, g_trapapp_config.dbconfig.dbPort,
//					   g_trapapp_config.dbconfig.dbUser, g_trapapp_config.dbconfig.dbPass,
//						   g_trapapp_config.dbconfig.dbSid);
	//if (!oracle.dbInitialize()) {
		//Log(LOG_DEBUG, "Main: trapapp init database error.");
	//}
	ToHbase to_hbase;
	to_hbase.setWireFormat(FormatFactory::getInstance().findFormat("zhuhai"));
	ConnectionHandler selector(new DefaultIOHandler(&to_hbase), true);
	IOConnector connector(&selector);
	Connection *client = connector.createConnection(g_trapapp_config.serverIp, g_trapapp_config.serverPort);

	uint64 before = System::currentTimeMillis()/1000;
	uint64 now = before;
	while (1)
	{
		selector.select(0, 500000);

		now = System::currentTimeMillis()/1000;
		if (now - before >= 10) {
			bool bSucced = sendHearbeat(client);
			if (!bSucced) {
				if (client != NULL) client->setDeleteByHandler(true);
				client = connector.createConnection(g_trapapp_config.serverIp, g_trapapp_config.serverPort);
				if (client != NULL) client->setDeleteByHandler(false);
			}
			before = now;
		}
	}

	ActiveCorelib::shutdownCoreLibrary();
	ActiveLibrary::shutdownLibrary();

	return 0;
}






