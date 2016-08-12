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
#ifndef __CORELIB_DATABASE_COMMON_FIELD_H__
#define __CORELIB_DATABASE_COMMON_FIELD_H__

#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../../common/basetype.h"

namespace corelib {
namespace database {

class Field
{
	private:

		Field& operator= (const Field&);

	public:

		enum DataTypes {

			DB_TYPE_UNKNOWN = 0x00,
			DB_TYPE_STRING	= 0x01,
			DB_TYPE_INTEGER	= 0x02,
			DB_TYPE_FLOAT	= 0x03
		};

		Field();
		Field(const Field&);
		Field(const char *value, const char *name, enum DataTypes type);
		~Field();

	public:

		void setName(const char *name);
		void setType(enum DataTypes type) {m_type = type;};
		void setValue(const char *value);

		const char *getName() const {return m_name;};
		enum DataTypes getType() const {return m_type;};
		const char *getString() const {return m_value;};
		std::string getCppString() const {return m_value ? m_value : "";};

		float toFloat() const {return m_value ? static_cast<float>(atof(m_value)) : 0;};
		bool toBoolean() const {return m_value ? atoi(m_value) > 0 : false;};
		int32 toInt32() const {return m_value ? static_cast<int32>(atol(m_value)) : 0;};
		uint32 toUint32() const {return m_value ? static_cast<uint32>(atol(m_value)) : 0;};
		int16 toInt16() const {return m_value ? static_cast<int16>(atol(m_value)) : 0;};
		uint16 toUint16() const {return m_value ? static_cast<uint16>(atol(m_value)) : 0;};
		int8 toInt8() const {return m_value ? static_cast<int8>(atol(m_value)) : 0;};
		uint8 toUint8() const {return m_value ? static_cast<uint8>(atol(m_value)) : 0;};
		uint64 toUint64() const {
			if (m_value) {
				uint64 value;
				sscanf(m_value, "%llu", &value);
				return value;
			}
			
			return 0;
		}

		int64 toInt64() const {
			if (m_value) {
				int64 value;
				sscanf(m_value, "%lld", &value);
				return value;
			}
			
			return 0;
		}

		std::string toString() const {return m_value ? m_value : "";};
		const char* toCString() const {return getString();};

	private:

		char *m_value;
		char *m_name;
		enum DataTypes m_type;
};

}
}

#endif /* end of file */