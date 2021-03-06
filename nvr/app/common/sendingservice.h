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
#ifndef __APP_COMMON_SENDINGSERVICE_H__
#define __APP_COMMON_SENDINGSERVICE_H__

#include "../common/eprotocol.h"
#include "../../library/common/runnable.h"
#include "../../library/concurrent/lock.h"
#include "../../library/buffer/bytebuffer.h"
#include "../../corelib/network/ioconnector.h"

namespace corelib {
namespace MTP_NETWORK {

	class Connection;
}

namespace protocol {
	class Format;
}

}

namespace library {

	class Thread;
}

namespace app {

namespace CM = corelib::MTP_NETWORK;
namespace CP = corelib::protocol;
class SendingService : public library::Runnable
{
	typedef std::list<ST_CAR_DESCRIPTION_INFO> SENDLIST;
	private:

		SendingService(const SendingService&);
		SendingService& operator= (const SendingService&);

	public:

		SendingService();
		~SendingService();

	public:

		virtual void run();

	public:

		void send(ST_CAR_DESCRIPTION_INFO* carInfo);

	private:

		void configInit();
		int32 doSend(ST_CAR_DESCRIPTION_INFO *carInfo);
		
	private:

		bool m_enable;
		uint16 m_serverport;
		std::string m_serverIp;
		CP::Format *m_wireFormat;
		CM::IOConnector m_connector;
		CM::Connection *m_clientHandler;

		SENDLIST m_sendList;
		library::Lock m_lock;		
		library::Thread *m_thread;
};

}

#endif /* end of file */