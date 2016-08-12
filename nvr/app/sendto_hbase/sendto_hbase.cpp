/***************************************************************************
 *   Copyright (C) 2016 by going
 *	 Created on: 2016年2月16日
 *	 Author: going   				   						   *
 *   going369@qq.com   							   					   *
 *   sengto_hbase.cpp                                                                      *
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
#include "sendto_hbase.h"
#include "../common/sqlcartrap.h"
#include "../../library/util/log.h"
#include "../../library/util/system.h"
#include "../../corelib/protocol/format.h"
#include "../protocol/carbypassdescinfo.h"
#include "../../corelib/network/connection.h"
#include "../protocol/consumerregisterinfo.h"
#include "../../corelib/database/common/field.h"
#include "../../corelib/database/common/sqlstring.h"
#include "../../corelib/database/common/queryresult.h"
#include "../../library/buffer/localbytearraybuffer.h"
#include "../../corelib/database/common/sqloperation.h"
#include "../../corelib/database/common/databaseimpl.h"
#include "../../library/buffer/remotebytearraybuffer.h"
#include "myoracle.h"
using namespace library;
using namespace corelib;
using namespace corelib::database;
using namespace corelib::protocol;
using namespace corelib::MTP_NETWORK;
using namespace app;
extern ST_TRAPAPP_CONFIG_INFO g_trapapp_config;
extern unsigned short datastore_port;
extern std::string datastore_ip;


ToHbase::ToHbase()
{
	// config trap
	trapConfigInit();
	Oracle * ora_ptr = new Oracle("10.42.181.34:1521/SICSDB", "si01", "jp2011");
	OCI_Resultset * rs_config = NULL;
	std::string fx_buf,sb_buf;
	ora_ptr->execute("select direction_number,sbbh from deviceinfo_tab" );
	while((rs_config = ora_ptr->result()) != NULL){
		
		if( (OCI_GetString(rs_config, 1) != NULL) && (OCI_GetString(rs_config, 2) != NULL)){
			dex[OCI_GetString(rs_config, 2)] = OCI_GetString(rs_config, 1);
		}	
	}
	delete ora_ptr;
	// start mqservice
}

ToHbase::~ToHbase()
{
}

void ToHbase::onStart(CM::Connection *connection)
{
	if (connection == NULL) return;

	connection->setDeleteByHandler(false); // client handler is deleted by user

	// send consumer register information
	ConsumerRegisterInfo registerInfo;

	registerInfo.setRegisterType(9005);
	registerInfo.setStatus(0);

	NETBUFFER netByteBuffer(1024);
	if (this->wireformat_ != NULL) {
		wireformat_->marshal(&registerInfo, netByteBuffer);
		connection->send((const char *)netByteBuffer.array(), netByteBuffer.limit());
	}
}

void ToHbase::onCommand(CM::Connection *connection, const char *data, int32 len)
{
	if (connection == NULL || data == NULL || len == 0) return;

	if (this->wireformat_ != NULL) {

		NETBUFFER netByteBuffer((uint8*)data, len, 0, len);
		DataStructure* datastructure(wireformat_->unmarshal(connection, netByteBuffer));
		if (datastructure == NULL) return;

		std::string messageType = datastructure->getDataStructureType();
		if (messageType == CarBypassDescInfo::ID_CARBYPASS_INFO) {
			checkAndNotify(static_cast<CarBypassDescInfo *>(datastructure));
		} else {
			Log(LOG_DEBUG, "ToHbase::onCommand - message<%s> unknown.", messageType.c_str());
		}

		delete datastructure;
	} else {
		Log(LOG_WARN, "ToHbase::onCommand - wireformat null.");
	}
}

void ToHbase::onEnd(CM::Connection *connection)
{
	// do nothing
}

void ToHbase::checkAndNotify(CarBypassDescInfo *carInfo)
{
	if (carInfo == NULL) return;

	ST_CAR_TRAP_INFO trap;

	memset(&trap, 0, sizeof(ST_CAR_TRAP_INFO));
	trap.stcarinfo = carInfo->getAll();

	GZ_VEHICLE_COMMON_INFO *GZ_protocol = (GZ_VEHICLE_COMMON_INFO *)malloc(sizeof(GZ_VEHICLE_COMMON_INFO));

//	show_zh_pages((ST_VEHICLE_COMMON_INFO *)(&trap.stcarinfo));
	zh_to_gz_protocol((ST_VEHICLE_COMMON_INFO *)(&trap.stcarinfo), GZ_protocol);
//	printf("beore send+++++++++++++++\n");
	do_send(GZ_protocol);
//	printf("++++++++++guangzhou++++++++++++++\n");
//	show_gz_pages(GZ_protocol);
//	printf("++++++++++zhuhai++++++++++++++\n");
//	show_zh_pages((ST_VEHICLE_COMMON_INFO *)(&trap.stcarinfo));

	free(GZ_protocol);

}

void ToHbase::trapConfigInit()
{

}

void ToHbase::setWireFormat(CP::Format *format)
{
	this->wireformat_ = format;
}

void ToHbase::zh_to_gz_protocol(ST_VEHICLE_COMMON_INFO *zh, GZ_VEHICLE_COMMON_INFO * gz)
{

//	g2u(zh->plateFNo, PLATE_NO_SIZE, gz->plateFNo, PLATE_NO_SIZE);
//	g2u(zh->plateTNo, PLATE_NO_SIZE, gz->plateTNo, PLATE_NO_SIZE);

	memcpy(gz->area, zh->area, sizeof(gz->area));
	memcpy(gz->arrivetime, zh->arrivetime, sizeof(gz->arrivetime));
	memcpy(gz->blockSn, zh->blockSn, sizeof(gz->blockSn));
	memcpy(gz->devSn, zh->devSn, sizeof(gz->devSn));
	memcpy(gz->laneSn, zh->laneSn, sizeof(gz->laneSn));
	memcpy(gz->plateType, zh->plateType, sizeof(gz->plateType));
	memcpy(gz->ptime, zh->ptime, sizeof(gz->ptime));
	gz->ptime[4] = '/';
	gz->ptime[7] = '/';
	gz->ptime[10] = '/';
	gz->ptime[13] = '/';
	gz->ptime[16] = '/';
	gz->ptime[19] = '/';

	memcpy(gz->runState, zh->runState, sizeof(gz->runState));
	memcpy(&gz->stPictPaths, &zh->stPictPaths, sizeof(gz->stPictPaths));
	memcpy(gz->vehiBrand, zh->vehiBrand, sizeof(gz->vehiBrand));
	memcpy(gz->vehiCode, zh->vehiCode, sizeof(gz->vehiCode));
	memcpy(gz->vehiShape, zh->vehiShape, sizeof(gz->vehiShape));
	memcpy(gz->vehiSn, zh->vehiSn, sizeof(zh->vehiSn));
	memset(&gz->vehiSn[15], 48, 7);
	memcpy(gz->vehiType, zh->vehiType, sizeof(gz->vehiType));
	//memcpy(&gz->fSpeed, &zh->fSpeed, sizeof(float));
	gz->fSpeed = zh->fSpeed;
	gz->fSpeedLim = zh ->fSpeedLim;
	gz->ftEquel[0] = zh->ftEquel[0];
	gz->plateFCol[0] = zh->plateFCol[0];
	gz->plateTCol[0] = zh->plateTCol[0];
	gz->vehiCol[0] = zh->vehiCol[0];
	gz->vehiLen = htonl(zh->vehiLen);
	strncpy(gz->plateFNo,zh->plateFNo, 15);
	if(zh->plateFNo != NULL){
		char tran_buf[255];
		memset(tran_buf,0, 255);
		g2u(zh->plateFNo, PLATE_NO_SIZE, tran_buf, 255);
		strncpy(gz->plateFNo,tran_buf, 15);
	}


	gz->score = 0;
	char buf[30];
	sprintf(buf,"000000000000000000000");
	memcpy(gz->cllx, buf, sizeof(gz->cllx));
	memcpy(gz->cltz, buf, sizeof(gz->cltz));
	char fxbh_buf[18];
		strncpy(fxbh_buf,gz->devSn,sizeof(gz->devSn));
		sbbh_str.clear();
		sbbh_str = fxbh_buf;
		fxbh(sbbh_str,dex,gz);
		printf("buf = %s\n", sbbh_str.c_str());


}
long long count = 0;

void ToHbase::do_send(GZ_VEHICLE_COMMON_INFO *data)
{
	if((data->stPictPaths.picPath[0][0] > 0) || (data->stPictPaths.picPath[1][0] > 0)){
		struct Encode_page page;
		Encode encode_gz("guangzhou", data, 1101);
		page = encode_gz.Do_encode();
/*
		printf("buf_lenth = %d\n", page.offset);
		printf("head = 0x%hhx\n", page.ptr[0]);
		printf("page = %.32s\n",page.ptr+2);
		printf("tail = 0x%hhx\n", page.ptr[page.offset-1]);
*/
		int32 nwrite = send_data(page.ptr, page.offset);
		Log(LOG_INFO, "send %d bytes.---count :: %d", nwrite, ++count);
	}

	//show_gz_pages((GZ_VEHICLE_COMMON_INFO *)&page.ptr[2 + 32 + 2 + 2 + 4]);
}

int32 ToHbase::send_data(char *pRetVal, uint32 len)
{
	int fd = socket( AF_INET, SOCK_STREAM, 0);
	if (fd < 0){
		perror("fail to socket");
		return -1;
	}
	sockaddr_in client;
	bzero( &client, sizeof(client));
	client.sin_family = AF_INET;
	client.sin_port = htons(datastore_port);
	inet_pton(AF_INET, datastore_ip.c_str() ,&client.sin_addr);
	//inet_pton(AF_INET, "127.0.0.1" ,&client.sin_addr);
	int32 ret;

	socklen_t add_len = sizeof(client);
	while((ret = connect( fd, (sockaddr *)&client, add_len)) == -1){
			Log(LOG_INFO, "fail to connet --- reconnet-----");
			sleep(5);
		}

	ret = send(fd, pRetVal, len, 0);
	if(ret != len){
			perror("fail to send");
			close(fd);
		}
	close(fd);
	return ret;
}


void ToHbase::show_zh_pages(ST_VEHICLE_COMMON_INFO * zh)
{
		printf("XXBH = %.15s\n", zh->vehiSn);
		printf("KKBH = %.18s\n", zh->blockSn);
		printf("SBBH = %.18s\n", zh->devSn);
		//printf("FXBH = %s\n", zh->ptime);
		printf("JGSJ = %.24s\n", zh->ptime);
		printf("CDBH = %.10s\n", zh->laneSn);
		//printf("CDLX = %s\n", zh->);
		printf("HPHM = %.15s\n", zh->plateFNo);
		printf("HPYS = %c\n", zh->plateFCol[0]);
		printf("CWHPHM = %.15s\n", zh->plateTNo);
		printf("CWHPYS = %c\n", zh->plateTCol[0]);
		printf("HPYZ = %c\n", zh->ftEquel[0]);
		printf("CLSD = %f\n", zh->fSpeed);
		printf("CLXS = %f\n", zh->fSpeedLim);
		printf("CSCD = %u\n", zh->vehiLen);
		printf("XSZT = %.4s\n", zh->runState);
		//printf("WFZT = %s\n", zh->);
		printf("CLPP = %.4s\n", zh->vehiBrand);
		printf("CLWX = %.4s\n", zh->vehiShape);
		printf("CSYS = %.5s\n", zh->vehiCol);
		printf("CB = %.2s\n", zh->vehiCode);
		printf("CLLX = %.4s\n", zh->vehiType);
		printf("HPZL = %.2s\n", zh->plateType);
		printf("SSDQ = %.10s\n", zh->area);
		printf("DDSJ = %.32s\n", zh->arrivetime);
		printf("TPSL = %u\n", zh->stPictPaths.dwPicNum);
		printf("TPLJ1 = %s\n", zh->stPictPaths.picPath[0]);
		printf("TPLJ2 = %s\n", zh->stPictPaths.picPath[1]);
		printf("TPLJ3= %s\n", zh->stPictPaths.picPath[2]);
		printf("TPLJ4 = %s\n", zh->stPictPaths.picPath[3]);


}

void ToHbase::show_gz_pages(GZ_VEHICLE_COMMON_INFO * gz)
{
		printf("XXBH = %.22s\n", gz->vehiSn);
		printf("KKBH = %.18s\n", gz->blockSn);
		printf("SBBH = %.18s\n", gz->devSn);
		printf("FXBH = %.10s\n", gz->fxbh);
		printf("JGSJ = %.23s\n", gz->ptime);
		printf("CDBH = %.2s\n", gz->laneSn);
		printf("CDLX = %.2s\n", gz->laneType);
		printf("HPHM = %.15s\n", gz->plateFNo);
		printf("HPYS = %c\n", gz->plateFCol[0]);
		printf("CWHPHM = %.15s\n", gz->plateTNo);
		printf("CWHPYS = %c\n", gz->plateTCol[0]);
		printf("HPYZ = %c\n", gz->ftEquel[0]);
		printf("CLSD = %f\n", gz->fSpeed);
		printf("CLXS = %f\n", gz->fSpeedLim);
		printf("CSCD = %u\n", gz->vehiLen);
		printf("XSZT = %.4s\n", gz->runState);
		printf("WFZT = %.4s\n", gz->wfzt);
		printf("CLPP = %.3s\n", gz->vehiBrand);
		printf("CLWX = %.3s\n", gz->vehiShape);
		printf("CSYS = %c\n", gz->vehiCol[0]);
		printf("CB = %.2s\n", gz->vehiCode);
		printf("CLLX = %.3s\n", gz->vehiType);
		printf("HPZL = %.2s\n", gz->plateType);
		printf("SSDQ = %.10s\n", gz->area);
		printf("DDSJ = %.32s\n", gz->arrivetime);
		printf("TPSL = %u\n", gz->stPictPaths.dwPicNum);
		printf("TPLJ1 = %s\n", gz->stPictPaths.picPath[0]);
		printf("TPLJ2 = %s\n", gz->stPictPaths.picPath[1]);
		printf("TPLJ3= %s\n", gz->stPictPaths.picPath[2]);
		printf("TPLJ4 = %s\n", gz->stPictPaths.picPath[3]);


}
iconv_t cd = 0;
int ToHbase::code_convert(char *from_charset,char *to_charset,char *inbuf,int inlen,char *outbuf,int outlen)
{
	char **pin = &inbuf;
	char **pout = &outbuf;
        if(cd == 0)
		cd = iconv_open(to_charset,from_charset);
	if (cd==0) return -1;
	memset(outbuf,0,outlen);
	if (iconv(cd, pin, (size_t*)&inlen, pout, (size_t*)&outlen)==-1) return -1;
	iconv_close(cd);

	return 0;
}

//UNICODE码转为GB2312码
int ToHbase::u2g(char *inbuf,int inlen,char *outbuf,int outlen)
{
	return code_convert("utf-8","gb2312",inbuf,inlen,outbuf,outlen);
}

//GB2312码转为UNICODE码
int ToHbase::g2u(char *inbuf,size_t inlen,char *outbuf,size_t outlen)
{
	return code_convert("gb2312","utf-8",inbuf,inlen,outbuf,outlen);
}

void ToHbase::fxbh(std::string sbbh, std::map<std::string, std::string> &index, GZ_VEHICLE_COMMON_INFO * gz)
{
	std::map <std::string, std::string>::iterator it;
	it = index.find(sbbh);
	//cout<<"+++++++"<<index[it->first]<<endl;
	if(it != index.end()){
		std::string find_buf = index[it->first];
		strncpy(gz->fxbh, find_buf.c_str(), sizeof(gz->fxbh));
	}
}	





