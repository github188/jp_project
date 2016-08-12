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
#include "../../library/buffer/remotebytearraybuffer.h"
#include "../../library/buffer/localbytearraybuffer.h"
#include "../../corelib/network/connection.h"
#include "zhuhaidatarequestlengthcodec.h"
#include "../../library/util/log.h"
#include "zhuhaiprotocolconfig.h"

using namespace library;
using namespace corelib;
using namespace corelib::MTP_NETWORK;
using namespace app;


ZhuhaiDataRequestLengthCodec::ZhuhaiDataRequestLengthCodec()
{
}

ZhuhaiDataRequestLengthCodec::~ZhuhaiDataRequestLengthCodec()
{
}

int32 ZhuhaiDataRequestLengthCodec::decode(Connection *connection)
{
	int32 totalLength = 0;
	uint32 dataLen = 0;
	
	if (connection == NULL) return 0;

	int32 inputDataLength = connection->getInputLength();
	if (inputDataLength < FIX_SPEC_HEAD_SIZE) {
		return 0;
	}

	char header[FIX_SPEC_HEAD_SIZE + 4]= {0};
	connection->peekInput(header, FIX_SPEC_HEAD_SIZE);
	NETBUFFER buffer1((uint8*)header, sizeof(header), 0, FIX_SPEC_HEAD_SIZE);
	ByteBuffer &buffer = buffer1;

	uint16 head = buffer.getShort();
	if (head != HEAD_SEPA) {
		Log(LOG_ERROR, "ZhuhaiDataRequestLengthCodec::decode - packet head invalid: 0x%x", head);	
		return -1;
	}
	
	uint8 digit[FIX_SPEC_DIGIT_LETTER_SIZE + 1] = {0};
	buffer.getRawBytes(digit, FIX_SPEC_DIGIT_LETTER_SIZE);
	if (memcmp(digit, DIGIT_LETTER, FIX_SPEC_DIGIT_LETTER_SIZE)) {
		Log(LOG_ERROR, "ZhuhaiDataRequestLengthCodec::decode - packet digit number invalid: %s", digit);	
		return -1;
	}
	
	uint8 version = buffer.getChar();
	if (version != PROT_VERSION) {
		Log(LOG_ERROR, "ZhuhaiDataRequestLengthCodec::decode - packet version invalid: %d", version);	
		return -1;
	}
	
	uint8 type = buffer.getChar();
//	if (type != PROT_TYPE) {
//		Log(LOG_ERROR, "ZhuhaiDataRequestLengthCodec::decode - packet type invalid: %d", type);
//		return -1;
//	}

	uint16 datatype = buffer.getShort();
	dataLen = (uint32)buffer.getInt();
	if (dataLen > 2 * 1024 * 1024 - 10) {
		Log(LOG_ERROR, "ZhuhaiDataRequestLengthCodec::decode - packet is too big: %d", dataLen);	
		return -1;
	}
	
	totalLength = FIX_SPEC_PRO_SIZE + dataLen;
	
	return totalLength;
}