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
#ifndef __APP_PROTOCOL_DEVICEHEARBEAT_H__
#define __APP_PROTOCOL_DEVICEHEARBEAT_H__

#include "../common/eprotocol.h"
#include "../../common/basetype.h"
#include "../../corelib/protocol/datastructure.h"

namespace app {
using corelib::protocol::DataStructure;
class DeviceHeartBeatInfo : public DataStructure
{
	public:

		static const std::string ID_DEVICE_HEARTBEAT_INFO;
			
	private:
		
		DeviceHeartBeatInfo(const DeviceHeartBeatInfo&);
		DeviceHeartBeatInfo& operator= (const DeviceHeartBeatInfo&);

	public:

		DeviceHeartBeatInfo();
		~DeviceHeartBeatInfo();

	public:

		virtual std::string getDataStructureType() const;
		virtual void copyDataStructure(const DataStructure *src);
		virtual bool equals(const DataStructure *value) const;

	public:

		std::string getDeviceId()const {return deviceId_;};
		void setDeviceId(const std::string str) {deviceId_ = str;};
		virtual void setDeviceId(const char *str, int32 len) {this->deviceId_.append(str, len);};

		std::string getDepartment()const {return department_;};
		void setDepartment(const std::string str) {department_ = str;};
		virtual void setDepartment(const char *str, int32 len) {this->department_.append(str, len);};

		std::string getSerialNumber()const {return serialNumber_;};
		void setSerialNumber(const std::string str) {serialNumber_ = str;};
		virtual void setSerialNumber(const char *str, int32 len) {this->serialNumber_.append(str, len);};
		
	private:

		std::string deviceId_;
		std::string department_;
		std::string serialNumber_;
};

}

#endif /* end of file */