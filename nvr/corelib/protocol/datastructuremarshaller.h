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
#ifndef __CORELIB_PROTOCOL_DATASTRUCTUREMARSHALLER_H__
#define __CORELIB_PROTOCOL_DATASTRUCTUREMARSHALLER_H__

#include <string>

#include "../../library/buffer/bytebuffer.h"

namespace library {

	class ByteBuffer;
}

namespace corelib {
namespace MTP_NETWORK {

	class Request;
}
}

namespace corelib {
namespace protocol {

class Format;
class DataStructure;
class DataStructureMarshaller
{
	private:

		DataStructureMarshaller(const DataStructureMarshaller&);

		DataStructureMarshaller& operator= (const DataStructureMarshaller&);

	public:

		DataStructureMarshaller();

		virtual ~DataStructureMarshaller();

	public:

		/* create a object for the certain type */
		virtual DataStructure* createObject() const = 0;

		/* return the datatype of datastructure */
		virtual std::string getDataStructureType() const = 0;

		/**
		 * decode from the request to a datastructure
		 *
		 * @param format the protocol for decoding.
		 * @param dataStructure the result for decoding.
		 * @param bytebuffer input packet for decoding.
		 *
		 * @param return void.
		 */
		virtual void unmarshal(Format *format, DataStructure *dataStructure, library::ByteBuffer &bytebuffer) = 0;

		/**
		 * @param format protocol for encoding.
		 * @param dataStructure the input datastructure for encoding.
		 * @param bytebuffer the marshall result.
		 *
		 * @param return void.
		 */
		virtual void marshal(Format *format, DataStructure *dataStructure, library::ByteBuffer &bytebuffer) = 0;
};

}

}

#endif /* end of file */