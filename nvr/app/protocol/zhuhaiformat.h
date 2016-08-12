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
#ifndef __APP_COMMON_ZHINTERACTFORMAT_H__
#define __APP_COMMON_ZHINTERACTFORMAT_H__

#include <string>
#include <map>

#include "../../library/buffer/bytebuffer.h"
#include "../../corelib/protocol/format.h"

namespace corelib{
namespace protocol {
	class DataStructure;
	class DataStructureMarshaller;
}
}

namespace app {

namespace CP = corelib::protocol;
namespace CM = corelib::MTP_NETWORK;
class ZhuHaiFormat: public CP::Format
{
	typedef std::map<std::string, CP::DataStructureMarshaller*> DSMarshallerMap;
	
	public:

		ZhuHaiFormat(uint8 version);

		~ZhuHaiFormat();

	public:

		virtual void setVersion(uint8 version) {this->version = version;};

		virtual int32  getVersion()const {return this->version;};

		virtual void marshal(CP::DataStructure *command, library::ByteBuffer &netByteBuffer);

		virtual CP::DataStructure* unmarshal(CM::Connection*, library::ByteBuffer &netByteBuffer);

	public:

		void addMarshaller(const std::string &type, CP::DataStructureMarshaller *marshaller);

	private:

		void initFormatProperty();

	private:

		uint8  version;
		uint16 headSeparator;
		uint16 tailSeparator;
		std::string digitString;
		DSMarshallerMap marshallers;
};

}

#endif /* end of file */