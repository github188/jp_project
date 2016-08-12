/***************************************************************************
 *   Copyright (C) 2016 by going
 *	 Created on: 2016Äê2ÔÂ16ÈÕ
 *	 Author: going   				   						   *
 *   going369@qq.com   							   					   *
 *   sendto_hbase.h                                                                      *
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



#ifndef __APP_SENDTOHBASE_H__
#define __APP_SENDTOHBASE_H__

#include <string>
#include "../../corelib/network/listener.h"
#include "../../common/gzconstproto.h"
#include "../../common/zzconstproto.h"
#include "include/encode.h"
#include <arpa/inet.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <strings.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <iconv.h>
#include <map>
#define PLATE_NO_SIZE 15

namespace corelib {
namespace MTP_NETWORK {

	class Connection;
}

namespace protocol {

	class Format;
}
}

namespace app {

namespace CP = corelib::protocol;
namespace CM = corelib::MTP_NETWORK;


class CarBypassDescInfo;
class ToHbase : public CM::Listener
{
	private:

		ToHbase(const ToHbase&);
		ToHbase& operator= (const ToHbase&);

	public:

		ToHbase();
		~ToHbase();
		
	public: //inheritDoc

		virtual void onStart(CM::Connection *connection);
		virtual void onCommand(CM::Connection *connection, const char *data, int32 len);
		virtual void onEnd(CM::Connection *connection);

	public:

		//

		int code_convert(char *from_charset,char *to_charset,char *inbuf,int inlen,char *outbuf,int outlen);
		int u2g(char *inbuf,int inlen,char *outbuf,int outlen);
		int g2u(char *inbuf,size_t inlen,char *outbuf,size_t outlen);

		void zh_to_gz_protocol(ST_VEHICLE_COMMON_INFO *zh, GZ_VEHICLE_COMMON_INFO * gz);


		void do_send(GZ_VEHICLE_COMMON_INFO *data);

		int send_data(char *pRetVal, uint32 len);


		void show_zh_pages(ST_VEHICLE_COMMON_INFO * zh);
		void show_gz_pages(GZ_VEHICLE_COMMON_INFO * gz);

		//protocol
		void setWireFormat(CP::Format *format);

		void fxbh(std::string sbbh, std::map<std::string, std::string> &index, GZ_VEHICLE_COMMON_INFO * gz);

	private:

		//init for whitelist/blacklist/controllist
		void trapConfigInit();



		void checkAndNotify(CarBypassDescInfo *carInfo);

	private:

		std::map<std::string, std::string> dex;
		CP::Format *wireformat_;
		std::string sbbh_str;

};

}

#endif /* end of file */




