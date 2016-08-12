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
#ifndef __APP_COMMON_ZHUHAIPROTOCOLCONFIG_H__
#define __APP_COMMON_ZHUHAIPROTOCOLCONFIG_H__

namespace app {

// protocol related define
#define HEAD_SEPA       					0xAAAA

#define TAIL_SEPA       					0x5555

#define DIGIT_LETTER    					"JINPENG2011.KT-ITMS-CX.ZHU.HAI.KA.KOU.201309302O112O1220130O0O01"

// protocol version define
#define PROT_VERSION    					1

#define PROT_TYPE    						2

// fixed size define
#define FIX_SPEC_HEAD_SEPARATOR_SIZE 		2

#define FIX_SPEC_DIGIT_LETTER_SIZE 			64

#define FIX_SPEC_PROTOCOL_VERSION_SIZE 		1

#define FIX_SPEC_PROTOCOL_TYPE_SIZE 		1

#define FIX_SPEC_PROTOCOL_DATATYPE_SIZE 	2

#define FIX_SPEC_PROTOCOL_DATALEN_SIZE 		4

#define FIX_SPEC_CHECK_SUM_SIZE 			2

#define FIX_SPEC_TAIL_SEPARATOR_SIZE 		2

#define FIX_SPEC_HEAD_SIZE 				   (FIX_SPEC_HEAD_SEPARATOR_SIZE + \
											FIX_SPEC_DIGIT_LETTER_SIZE + \
											FIX_SPEC_PROTOCOL_VERSION_SIZE + \
											FIX_SPEC_PROTOCOL_TYPE_SIZE + \
											FIX_SPEC_PROTOCOL_DATATYPE_SIZE + \
											FIX_SPEC_PROTOCOL_DATALEN_SIZE)

// protocol specific posiztion define
#define FIX_SPEC_DATA_LEN_POS 			   (FIX_SPEC_HEAD_SEPARATOR_SIZE + \
										    FIX_SPEC_DIGIT_LETTER_SIZE + \
										    FIX_SPEC_PROTOCOL_VERSION_SIZE + \
										    FIX_SPEC_PROTOCOL_TYPE_SIZE + \
										    FIX_SPEC_PROTOCOL_DATATYPE_SIZE)

#define FIX_SPEC_PRO_SIZE 				   (FIX_SPEC_HEAD_SEPARATOR_SIZE + \
										    FIX_SPEC_DIGIT_LETTER_SIZE + \
										    FIX_SPEC_PROTOCOL_VERSION_SIZE + \
										    FIX_SPEC_PROTOCOL_TYPE_SIZE + \
										    FIX_SPEC_PROTOCOL_DATATYPE_SIZE + \
										    FIX_SPEC_PROTOCOL_DATALEN_SIZE +\
										    FIX_SPEC_CHECK_SUM_SIZE +\
										    FIX_SPEC_TAIL_SEPARATOR_SIZE)


////////////////////////////////////////////////////////////////////////////////
#define STATUS_HEART_BEAT_MSG_ID            1001

#define STATUS_VECHILE_SENSOR_MSG_ID        1002

#define STATUS_SNAP_SHOT_MSG_ID             1003

#define STATUS_SEARCH_UNIT_MSG_ID           1004

#define VEHICLE_INTERAVE_MSG_ID             1101

#define TIME_SYNCHRONIZATION_MSG_ID         1401

#define PROCESS_HEART_BEAT_MSG_ID           9001

#define PROCESS_HEART_BEAT_RSP_MSG_ID		9002

#define PROCESS_DATA_REGISTER_MSG_ID        9003

#define PROCESS_DATA_REGISTER_RSP_MSG_ID    9004

#define PROCESS_CAR_INFO_FORWARD_MSG_ID     9005

#define PROCESS_RAW_BYTES_FORWARD_MSG_ID	9006

#define PROCESS_DEVICE_HEART_FORWARD_MSG_ID	9007

}

#endif /* end of file */