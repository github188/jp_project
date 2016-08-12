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
#ifndef __APP_PROTOCOL_CARDESCRIPTIONINFOMARSHALLER_H__
#define __APP_PROTOCOL_CARDESCRIPTIONINFOMARSHALLER_H__

#include "../../corelib/protocol/datastructuremarshaller.h"
#include "../../library/buffer/bytebuffer.h"

namespace corelib {
namespace protocol {

	class Format;
	class DataStructure;
}
}

namespace app {

namespace CM = corelib::MTP_NETWORK;
namespace CP = corelib::protocol;
class CarDescriptionInfoMarshaller : public corelib::protocol::DataStructureMarshaller
{
	public:

		CarDescriptionInfoMarshaller();

		virtual ~CarDescriptionInfoMarshaller();

	public:

		virtual CP::DataStructure* createObject() const;

		virtual std::string getDataStructureType() const;

		virtual void unmarshal(CP::Format *format, CP::DataStructure *dataStructure, library::ByteBuffer &bytebuffer);

		virtual void marshal(CP::Format *format, CP::DataStructure *dataStructure, library::ByteBuffer &bytebuffer);
};

}

#endif /* end of file */
