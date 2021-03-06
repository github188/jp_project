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
#include <exception>

#include "../../corelib/network/connection.h"
#include "../../corelib/network/listener.h"
#include "../../library/util/log.h"
#include "defaultIOHandler.h"

using namespace library;
using namespace corelib;
using namespace corelib::MTP_NETWORK;
using namespace app;

DefaultIOHandler::DefaultIOHandler() : IOHandler(), codec_(), listener_(NULL)
{
}

DefaultIOHandler::DefaultIOHandler(Listener *listener) : IOHandler(), codec_(), listener_(listener)
{
}

DefaultIOHandler::~DefaultIOHandler()
{
}

void DefaultIOHandler::OnConnectionOpened(CM::Connection *connection)
{
	//default configuration
	std::string connId = connection->uniqueIdentifier();
	connection->setDisableInputBuffer(false);
	connection->setFlushoutBeforeClose(false);
	connection->setDeleteByHandler(true);

	// user can reset it
	if (this->listener_ != NULL) {
		this->listener_->onStart(connection);
	}

	Log(LOG_DEBUG, "DefaultIOHandler::OnConnectionOpened - %s opened.", connId.c_str());
}

void DefaultIOHandler::OnConnectionReceive(CM::Connection *connection)
{
	if (connection == NULL) return;

	std::string connId = connection->uniqueIdentifier();
	int32 dataRequestLength = codec_.decode(connection);

	do {
		if (dataRequestLength < 0) {
			connection->shutdown();
			connection->setDeleteByHandler(true);
			if (this->listener_ != NULL) {
				this->listener_->onEnd(connection);
			}
			Log(LOG_DEBUG, "DefaultIOHandler::OnConnectionReceive - %s OnConnectionReceive error.", connId.c_str());
		} else if (dataRequestLength > 0) {

			int32 remainBytes = connection->getInputLength();
			if (remainBytes < dataRequestLength) return;
			
			char *bytes = NULL;
			try {
				bytes = new char[dataRequestLength + 1];
			} catch (const std::bad_alloc& e) {
				Log(LOG_ERROR, "DefaultIOHandler::OnConnectionReceive - ++ error for new operator ++.");
				return;
			}
			
			bytes[dataRequestLength] = '\0';
			connection->readInput(bytes, dataRequestLength);
			
			if (this->listener_ != NULL) {
				this->listener_->onCommand(connection, bytes, dataRequestLength);
			}

			delete []bytes;
		} else {
			return;
		}
	} while ((dataRequestLength = codec_.decode(connection)) > 0);
}

void DefaultIOHandler::OnConnectionReceiveRawData(CM::Connection *connection, char *data, size_t len)
{
	std::string connId = connection->uniqueIdentifier();
	Log(LOG_DEBUG, "DefaultIOHandler::OnConnectionReceiveRawData - %s receive raw bytes.", connId.c_str());
}

void DefaultIOHandler::OnConnectionDisConnect(CM::Connection *connection, CM::SHUT_REASON reason)
{
	std::string connId = connection->uniqueIdentifier();
	if (this->listener_ != NULL) {
		this->listener_->onEnd(connection);
	}
	Log(LOG_DEBUG, "DefaultIOHandler::OnConnectionDisConnect - %s disconnected, reason: %d.", connId.c_str(), reason);
}

void DefaultIOHandler::OnConnectionTimedOut(CM::Connection *connection)
{
	std::string connId = connection->uniqueIdentifier();
	connection->shutdown();
	connection->setDeleteByHandler(true);
	if (this->listener_ != NULL) {
		this->listener_->onEnd(connection);
	}
	Log(LOG_DEBUG, "DefaultIOHandler::OnConnectionTimedOut - %s time out.", connId.c_str());
}

void DefaultIOHandler::OnConnectionClosed(CM::Connection *connection)
{
	if (this->listener_ != NULL) {
		this->listener_->onEnd(connection);
	}
}