/***************************************************************************
 *   Copyright (C) 2016 by going
 *	 Created on: 2016Äê2ÔÂ17ÈÕ
 *	 Author: going   				   						   *
 *   going369@qq.com   							   					   *
 *   encode.h                                                                      *
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


#ifndef INCLUDE_ENCODE_H_
#define INCLUDE_ENCODE_H_
#include "../../../common/gzconstproto.h"
#include "../../../common/zzconstproto.h"
#include "../../../common/basetype.h"

struct Encode_page{
	uint32 offset;
	char * ptr;
};
class Encode{
public:
	Encode(const char * s, void * page_ptr, uint16 _type);
	~Encode();
public:
	struct Encode_page Do_encode();
	int32 VehicleInfo_filter(void * pcData);
	void Get_memory(uint32 datalen);
	void Free_memory(void * page);
	void Head(uint8 head);
	void DigitLetter(const char * digit);
	void ProtTV();
	void Type(uint16 type);
	void Bodydata();
	void Length(uint32 datelen);
	void Tail(uint8 tail);
private:
	struct Encode_page encode_str;
	uint32 datalen;
	uint16 type;
	char buf[MAX_BUFFER_SIZE_50k];
};




#endif /* INCLUDE_ENCODE_H_ */
