/***************************************************************************
 *   Copyright (C) 2016 by going
 *	 Created on: 2016Äê2ÔÂ17ÈÕ
 *	 Author: going   				   						   *
 *   going369@qq.com   							   					   *
 *   encode.cpp                                                                      *
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
#include "./include/encode.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>
Encode::Encode(const char * s, void * page_ptr, uint16 _type)
{	//printf("ok, encode\n");
	VehicleInfo_filter(page_ptr);
	type = _type;

	Get_memory(datalen);
}
Encode::~Encode()
{
	//printf("by, encode\n");
	Free_memory(encode_str.ptr);
}
void Encode::Get_memory(uint32 datelen)
{

	encode_str.ptr = (char *)malloc(GZ_FIX_SPEC_PRO_SIZE + datalen);
	if(encode_str.ptr == NULL){
		perror("fail to malloc");
	}
	memset(encode_str.ptr, 0, GZ_FIX_SPEC_PRO_SIZE + datalen);

}

void Encode::Free_memory(void * page)
{
	free(page);
}

struct Encode_page Encode::Do_encode()
{
	encode_str.offset = 0;
	Head(0xaa);
	//printf("head = ox%hhx\n", encode_str.ptr[0]);
//	printf("offser = %d\n", encode_str.offset);
	Head(0xaa);
//	printf("offser = %d\n", encode_str.offset);
	DigitLetter(GZ_DIGIT_LETTER);

//	printf("offser = %d\n", encode_str.offset);
	ProtTV();
//	printf("offser = %d\n", encode_str.offset);
	Type(type);
//	printf("offser = %d\n", encode_str.offset);

	Length(datalen);
//	printf("offser = %d\n", encode_str.offset);


	Bodydata();
	//printf("offser = %d\n", encode_str.offset);
	Tail(0x55);
	Tail(0x55);
//	printf("offser = %d\n", encode_str.offset);
	return encode_str;

}

void Encode::Head(uint8 head)
{

	if(encode_str.ptr != NULL){
		memcpy(&encode_str.ptr[encode_str.offset], &head, 1);
		encode_str.offset++;
	}

}
void Encode::DigitLetter(const char * digit)
{
	uint32 len = strlen(digit);

	if(encode_str.ptr != NULL){
		memcpy(encode_str.ptr + encode_str.offset, digit, len);
		encode_str.offset += len;
		}
}
void Encode::ProtTV()
{
	uint8 version = 1;
	if(encode_str.ptr != NULL){
		memcpy(encode_str.ptr + encode_str.offset, &version, 1);
		encode_str.offset++;
		memcpy(encode_str.ptr + encode_str.offset, &version, 1);
		encode_str.offset++;
		}
}
void Encode::Type(uint16 type)
{
	if(encode_str.ptr != NULL){
		memcpy(encode_str.ptr + encode_str.offset, &type, sizeof(uint16));
		encode_str.offset += sizeof(uint16);
		}

}

void Encode::Length(uint32 datalen)
{
	//datalen = htons(datalen);
	if(encode_str.ptr != NULL){
		memcpy(encode_str.ptr + encode_str.offset, &datalen, sizeof(uint32));
		encode_str.offset += sizeof(uint32);
		}
}

void Encode::Bodydata()
{
	if(encode_str.ptr != NULL){
		memcpy(encode_str.ptr + encode_str.offset, buf, datalen);
		encode_str.offset += datalen;
		}
}

void Encode::Tail(uint8 tail){
	if(encode_str.ptr != NULL){
		memcpy(encode_str.ptr + encode_str.offset, &tail, 1);
		encode_str.offset++;
	}
}

int32 Encode::VehicleInfo_filter(void * pcData)
{
	GZ_VEHICLE_COMMON_INFO *pstDbInfo = NULL;

	uint32 offset = 0;
	memset(&buf[offset], 0, MAX_BUFFER_SIZE_50k);

	if (pcData == NULL) return STATUS_ERROR;

	pstDbInfo = (GZ_VEHICLE_COMMON_INFO *)pcData;
	memcpy(&buf[offset], pstDbInfo->vehiSn, sizeof(pstDbInfo->vehiSn));
	offset += sizeof(pstDbInfo->vehiSn);
	memcpy(&buf[offset], pstDbInfo->blockSn, sizeof(pstDbInfo->blockSn));
	offset += sizeof(pstDbInfo->blockSn);
	memcpy(&buf[offset], pstDbInfo->devSn, sizeof(pstDbInfo->devSn));
	offset += sizeof(pstDbInfo->devSn);
	memcpy(&buf[offset], pstDbInfo->fxbh, sizeof(pstDbInfo->fxbh));
	offset += sizeof(pstDbInfo->fxbh);
	memcpy(&buf[offset], pstDbInfo->ptime, sizeof(pstDbInfo->ptime));
	offset += sizeof(pstDbInfo->ptime);
	memcpy(&buf[offset], pstDbInfo->laneSn, sizeof(pstDbInfo->laneSn));
	offset += sizeof(pstDbInfo->laneSn);
	memcpy(&buf[offset], pstDbInfo->laneType, sizeof(pstDbInfo->laneType));
	offset += sizeof(pstDbInfo->laneType);
	memcpy(&buf[offset], pstDbInfo->plateFNo, sizeof(pstDbInfo->plateFNo));
	offset += sizeof(pstDbInfo->plateFNo);
	memcpy(&buf[offset], pstDbInfo->plateFCol, sizeof(pstDbInfo->plateFCol));
	offset += sizeof(pstDbInfo->plateFCol);
	memcpy(&buf[offset], pstDbInfo->plateTNo, sizeof(pstDbInfo->plateTNo));
	offset += sizeof(pstDbInfo->plateFNo);
	memcpy(&buf[offset], pstDbInfo->plateTCol, sizeof(pstDbInfo->plateTCol));
	offset += sizeof(pstDbInfo->plateTCol);
	memcpy(&buf[offset], pstDbInfo->ftEquel, sizeof(pstDbInfo->ftEquel));
	offset += sizeof(pstDbInfo->ftEquel);
//	Uint32 speed = htonl(pstDbInfo->fSpeed);
	float speed = pstDbInfo->fSpeed;
	memcpy(&buf[offset], &speed, sizeof(speed));
	offset += sizeof(speed);
	memcpy(&buf[offset], &(pstDbInfo->fSpeedLim), sizeof(pstDbInfo->fSpeedLim));
	offset += sizeof(pstDbInfo->fSpeedLim);
//	Uint32 vl = htonl(pstDbInfo->vehiLen);
	uint32 vl = pstDbInfo->vehiLen;
	memcpy(&buf[offset], &vl, sizeof(uint32));
	offset += sizeof(uint32);
	memcpy(&buf[offset], &(pstDbInfo->runState), sizeof(pstDbInfo->runState));
	offset += sizeof(pstDbInfo->runState);
	memcpy(&buf[offset], &(pstDbInfo->wfzt), sizeof(pstDbInfo->wfzt));
	offset += sizeof(pstDbInfo->wfzt);
	memcpy(&buf[offset], &(pstDbInfo->vehiBrand), sizeof(pstDbInfo->vehiBrand));
	offset += sizeof(pstDbInfo->vehiBrand);
	memcpy(&buf[offset], &(pstDbInfo->vehiShape), sizeof(pstDbInfo->vehiShape));
	offset += sizeof(pstDbInfo->vehiShape);
	memcpy(&buf[offset], &(pstDbInfo->vehiCol), sizeof(pstDbInfo->vehiCol));
	offset += sizeof(pstDbInfo->vehiCol);
	memcpy(&buf[offset], &(pstDbInfo->vehiType), sizeof(pstDbInfo->vehiType));
	offset += sizeof(pstDbInfo->vehiType);
	memcpy(&buf[offset], &(pstDbInfo->plateType), sizeof(pstDbInfo->plateType));
	offset += sizeof(pstDbInfo->plateType);
	
	vl = pstDbInfo->score;
	memcpy(&buf[offset], &vl, sizeof(uint32));
	offset += sizeof(uint32);


	memcpy(&buf[offset], &(pstDbInfo->cllx), sizeof(pstDbInfo->cllx));
	offset += sizeof(pstDbInfo->cllx);


	memcpy(&buf[offset], &(pstDbInfo->cltz), sizeof(pstDbInfo->cltz));
	offset += sizeof(pstDbInfo->cltz);

//	Uint32 picNum = htonl(pstDbInfo->stPictPaths.dwPicNum);
	pstDbInfo->stPictPaths.dwPicNum = 8;
	uint32 picNum = pstDbInfo->stPictPaths.dwPicNum;
	memcpy(&buf[offset], &picNum, sizeof(uint32));
	offset += sizeof(uint32);

	uint32 ll = 0;
	/* set the pictures to 8 */
	for (uint32 j = 0; j < pstDbInfo->stPictPaths.dwPicNum; j++)
	{
//		Size_t pl = ::strlen(pstDbInfo->stPictPaths.picPath[j]);
		size_t pl = 200;
		ll = pl;
//		ll = htonl(ll);
//		memcpy(&buf[offset], &ll, sizeof(Uint32));
//		offset += sizeof(Uint32);
		if (pl > 0)
		{
			memcpy(&buf[offset], pstDbInfo->stPictPaths.picPath[j], pl);
			offset += pl;
		}
	}

	datalen = offset;


	return 0;
}




