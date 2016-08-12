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
#ifndef __APP_COMMON_MQSERVICE_H__
#define __APP_COMMON_MQSERVICE_H__

#include <decaf/lang/Thread.h>
#include <decaf/lang/Runnable.h>
#include <decaf/util/concurrent/CountDownLatch.h>
#include <decaf/lang/Long.h>
#include <decaf/util/Date.h>
#include <activemq/core/ActiveMQConnectionFactory.h>
#include <activemq/util/Config.h>
#include <activemq/library/ActiveMQCPP.h>
#include <cms/Connection.h>
#include <cms/Session.h>
#include <cms/TextMessage.h>
#include <cms/BytesMessage.h>
#include <cms/MapMessage.h>
#include <cms/ExceptionListener.h>
#include <cms/MessageListener.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <memory>

#include <map>
#include <list>
#include <string>
#include "mqcallback.h"
#include "../common/eprotocol.h"
#include "../../library/util/lockqueue.h"
#include "../../library/common/runnable.h"

namespace library {

	class Thread;
}

namespace app {

class IMQCallback;

class CmsMQClient
{
	public:

		CmsMQClient(const std::string& brokerURI, const std::string& prodestURI,
					const std::string& condestURI, bool useTopic = false, 
					bool clientAck = false);
		~CmsMQClient();

	public:

		void setMessageListener(cms::MessageListener *listener);
		void sendMessage(std::string &msg);

	private:

		void CmsMqClientInit();
		void cleanup();

	private:

		cms::Connection* connection;
        cms::Session* session;
        cms::Destination* prodest;
		cms::Destination* condest;
        cms::MessageProducer* producer;
        cms::MessageConsumer* consumer;
        bool bUseTopic;
        bool bClientAck;
        std::string brokerURI;
        std::string prodestURI;
		std::string condestURI;
		bool m_bInitOk;

		cms::MessageListener *m_listener;
};

////////////////////////////////////////////////////////////////////////////////
class MQService : public library::Runnable, public cms::MessageListener
{
	typedef library::LockedQueue<std::string> MESSAGELIST;
	
	private:

		MQService(const MQService&);
		MQService& operator= (const MQService&);

	public:

		MQService();
		~MQService();

	public: //inheritDoc

		virtual void run();
		virtual bool bNeedDestroyed() const {return false;};

	public:

		void start();
		void stop();

		void add(ST_CAR_TRAP_INFO *trap);
		void add(ST_CAR_DESCRIPTION_INFO &carInfo);
		void onMessage(const cms::Message* message);

		template<class Class>
		void bindHandler(std::string id, Class *object, void (Class::*method)(std::string));

	private:

		void mqServiceInit();
		std::string assembleTrapInfo(ST_CAR_TRAP_INFO *psttrap);
		std::string assembleCarDescStringInfo(ST_CAR_DESCRIPTION_INFO &carInfo);
		void update(std::string &command);

	private:

		bool m_go;
		MESSAGELIST m_sendList;
		MESSAGELIST m_realtimeList;
		library::Thread *m_thread;
		CmsMQClient *m_MqSendClient;
		CmsMQClient *m_MqRecvClient;
		CmsMQClient *m_MqRealtimeClient;
		ST_ACTIVEMQ_CONFIG_INFO m_sendConfig;
		ST_ACTIVEMQ_CONFIG_INFO m_recvConfig;
		ST_ACTIVEMQ_CONFIG_INFO m_realtimeConfig;
		std::map<std::string, IMQCallback*> m_callback;
};

template<class Class>
void MQService::bindHandler(std::string id, Class *object, void (Class::*method)(std::string))
{
	std::map<std::string, IMQCallback*>::iterator it;
	
	it = m_callback.find(id);
	if (it == m_callback.end()) {
		m_callback.insert(std::pair<std::string, IMQCallback*>(id, new MQCallback<Class>(object, method, std::string(""))));
	}
}

}

#endif /* end of file */