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
#ifndef __APP_COMMON_EPROTOCOL_H__
#define __APP_COMMON_EPROTOCOL_H__

#include <vector>
#include <string>

#include "../../common/paradef.h"
#include "../../common/basetype.h"

#define MAX_PIC_NUM_PER_PASS 8

namespace app {

typedef enum {
	INSERT = 0,
	UPDATE = 1,
} ACTION;

struct ST_CAR_DESCRIPTION_INFO;
typedef struct ST_CAR_RECORD_INFO {

	ST_CAR_DESCRIPTION_INFO *record;
	ACTION action;
} ST_CAR_RECORD_INFO;

typedef struct ST_SYS_SETTING_INFO
{
	char mapKey[MAX_BUFFER_SIZE_032];
	char dspVal[MAX_BUFFER_SIZE_256];
} ST_SYS_SETTING_INFO;

typedef struct ST_CAR_BRAND_INFO
{
	char brandId[MAX_BUFFER_SIZE_032];
	char brandValue[MAX_BUFFER_SIZE_512];
} ST_CAR_BRAND_INFO;

typedef struct ST_DEVICE_INFO
{
    char devSn[MAX_BUFFER_SIZE_256];
    float lattitude;
    float longitude;
//  char ipAddr[MAX_BUFFER_SIZE_256];
//  char devType[MAX_BUFFER_SIZE_032];
//  char devState[MAX_BUFFER_SIZE_032];
//  char port[MAX_BUFFER_SIZE_032];
//  char userName[MAX_BUFFER_SIZE_128];
//  char password[MAX_BUFFER_SIZE_128];
    char bkkbh[MAX_BUFFER_SIZE_128];
    char manufactory[MAX_BUFFER_SIZE_064];
    char department[MAX_BUFFER_SIZE_128];
    char devName[MAX_BUFFER_SIZE_256];
    char direction[MAX_BUFFER_SIZE_064];
    char devLen[MAX_BUFFER_SIZE_064];
    char kkbh[MAX_BUFFER_SIZE_128];
    char kkmc[MAX_BUFFER_SIZE_256];
    char dwbh[MAX_BUFFER_SIZE_032];
    char dwmc[MAX_BUFFER_SIZE_128];
    char qymc[MAX_BUFFER_SIZE_256];
    char qydm[MAX_BUFFER_SIZE_128];
    char directionSn[MAX_BUFFER_SIZE_064];
} ST_DEVICE_INFO;

////////////////////////////////////////////////////////////////////////////////
typedef struct ST_PICTURE_INFO
{
	uint8		index;
	uint32		len;
	uint8		data[MAX_BUFFER_SIZE_02m];
} ST_PICTURE_INFO;

typedef struct ST_PICTURES_INFO
{
	uint32 num;
	ST_PICTURE_INFO data[MAX_PIC_NUM_PER_PASS];
} ST_PICTURES_INFO;

typedef struct ST_PICTURE_URL_INFO
{
	uint32 urlNum;
    char url[MAX_PIC_NUM_PER_PASS][MAX_BUFFER_SIZE_512]; // http://xx.xx.xx:808x/Image/xxxx/xx.jpg
} ST_PICTURE_URL_INFO;

typedef struct ST_CAR_DESCRIPTION_INFO
{
	char vehiSn[15];
    char blockSn[18];
    char devSn[18];
    char ptime[24];
    char laneSn[10];
    char plateFNo[15];
    char plateFCol[1];
    char plateTNo[15];
    char plateTCol[1];
    char ftEquel[1];
    float fSpeed;
    float fSpeedLim;
    uint32 vehiLen;
    char runState[4];
    char vehiBrand[4];
    char vehiShape[4];
    char vehiCol[5];
    char vehiCode[2];
    char vehiType[4];
    char plateType[2];
    char area[10];
	char arrivetime[32];
	ST_PICTURE_URL_INFO urlInfo;
} ST_CAR_DESCRIPTION_INFO;

typedef struct ST_BALCKLIST_CONFIG_INFO
{
	char bkxxbh[MAX_BUFFER_SIZE_128];
	char hphm[MAX_BUFFER_SIZE_128];
	char cpys[MAX_BUFFER_SIZE_032];
	char csys[MAX_BUFFER_SIZE_032];
	char cllx[MAX_BUFFER_SIZE_128];
	char hpzl[MAX_BUFFER_SIZE_256];
	char clpp[MAX_BUFFER_SIZE_512];
	char cljs[MAX_BUFFER_SIZE_01k];
	char bjjb[MAX_BUFFER_SIZE_064];
	char llsj[MAX_BUFFER_SIZE_128];
} ST_BALCKLIST_CONFIG_INFO;

typedef struct ST_WHITELIST_CONFIG_INFO
{
	char hphm[MAX_BUFFER_SIZE_128];
	char cpys[MAX_BUFFER_SIZE_032];
	char csys[MAX_BUFFER_SIZE_032];
	char cllx[MAX_BUFFER_SIZE_128];
	char hpzl[MAX_BUFFER_SIZE_256];
	char clpp[MAX_BUFFER_SIZE_512];
	char cljs[MAX_BUFFER_SIZE_01k];
	uint32 shzj;
	char llsj[MAX_BUFFER_SIZE_128];
} ST_WHITELIST_CONFIG_INFO;

typedef struct ST_CONTROLLIST_CONFIG_INFO
{
	char bkxxbh[MAX_BUFFER_SIZE_128];
	char hphm[MAX_BUFFER_SIZE_064];
	char hpys[MAX_BUFFER_SIZE_016];
	char clpp[MAX_BUFFER_SIZE_016];
	char clwx[MAX_BUFFER_SIZE_032];
	char csys[MAX_BUFFER_SIZE_032];
	char cllx[MAX_BUFFER_SIZE_032];
	char hpzl[MAX_BUFFER_SIZE_032];
	char bkkssj[MAX_BUFFER_SIZE_032];
	char bkjssj[MAX_BUFFER_SIZE_032];
	char bkzt[MAX_BUFFER_SIZE_032];
	char bkfw[MAX_BUFFER_SIZE_128];
	char lxdh[MAX_BUFFER_SIZE_032];
	char dxfsbs[MAX_BUFFER_SIZE_016];
} ST_CONTROLLIST_CONFIG_INFO;

typedef struct ST_CAR_TRAP_INFO
{
	ST_CAR_DESCRIPTION_INFO stcarinfo;
	char bkxxbh[MAX_BUFFER_SIZE_064];
	char bjlx[MAX_BUFFER_SIZE_064];
	char lxdh[MAX_BUFFER_SIZE_032];
	char dxfsbs[MAX_BUFFER_SIZE_016]; 
} ST_CAR_TRAP_INFO;

////////////////////////////////////////////////////////////////////////////////
typedef struct ST_LOGMY_INFO
{
	uint16 logWhere;  //0: stdout; 1: to file; 2: to file and stdout
	uint16 logLevel;  //0: debug above; 2: info above; 3: warn above; 4: error level
	uint32 logFileSize; //size in kb
	std::string logFileName;
} ST_LOGMY_INFO;

typedef struct ST_SENDING_SERVICE_INFO
{
	bool enable;
	uint16 serverPort;
	std::string serverIp;
} ST_SENDING_SERVICE_INFO;

typedef struct ST_PICTURE_STORE_INFO
{
	bool pictureStoreEnable;
	uint32 threadNum;
	std::string urlPrefix;
	std::string pictureRoot;
	uint32 direntNum;
	uint64 direntSkipOffSize;
	std::vector<std::string> direntVec;
} ST_PICTURE_STORE_INFO;

typedef struct ST_DATABASE_CONFIG_INFO
{
	bool enable;
	uint16 dbPort;
	std::string dbIp;
	std::string dbUser;
	std::string dbPass;
	std::string dbSid;
} ST_DATABASE_CONFIG_INFO;

typedef struct ST_DABIGATE_CONFIG_INFO
{
	uint16 lConnectionPort;
	uint16 sConnectionPort;
	ST_LOGMY_INFO logMyConfig;
	ST_DATABASE_CONFIG_INFO dbconfig;
	ST_PICTURE_STORE_INFO pictureConfig;
	ST_SENDING_SERVICE_INFO sendingService;
} ST_DABIGATE_CONFIG_INFO;

typedef struct ST_FWDSERVER_CONFIG_INFO
{
	uint16 serverPort;
	ST_LOGMY_INFO logMyConfig;
} ST_FWDSERVER_CONFIG_INFO;

typedef struct ST_ACTIVEMQ_CONFIG_INFO
{
	bool enable;
	std::string broker;
	std::string producer;
	std::string consumer;
} ST_ACTIVEMQ_CONFIG_INFO;

typedef struct ST_TRAPAPP_CONFIG_INFO
{
	std::string serverIp;
	uint16 serverPort;
	ST_LOGMY_INFO logMyConfig;
	ST_DATABASE_CONFIG_INFO dbconfig;
	ST_ACTIVEMQ_CONFIG_INFO reportMQ;
	ST_ACTIVEMQ_CONFIG_INFO commandMQ;
	ST_ACTIVEMQ_CONFIG_INFO realtimeMQ;
} ST_TRAPAPP_CONFIG_INFO;

}

#endif /* end of file */