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
#ifndef __CORELIB_PROTOCOL_DATASTRUCTURE_H__
#define __CORELIB_PROTOCOL_DATASTRUCTURE_H__

#include <string>

namespace corelib {
namespace protocol {

class DataStructure
{
	private:

		DataStructure(const DataStructure&);

		DataStructure& operator= (const DataStructure&);
		
	public:

		DataStructure();

		virtual ~DataStructure();

	public:

		virtual std::string getDataStructureType() const = 0;

		virtual void setSessionId(const std::string &sid);

		virtual std::string getSessionId() const;

		virtual void copyDataStructure(const DataStructure *src);

		virtual bool equals(const DataStructure *value) const;

	protected:

		std::string sessionId;
};

}
}

#endif /* end of file */