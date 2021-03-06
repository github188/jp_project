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
#ifndef __APP_PROTOCOL_CONSUMERHEARTBEAT_H__
#define __APP_PROTOCOL_CONSUMERHEARTBEAT_H__

#include "../../common/basetype.h"
#include "../../corelib/protocol/datastructure.h"

namespace app {

using corelib::protocol::DataStructure;
class ConsumerHeartBeatInfo : public DataStructure
{
	public:

		static const std::string ID_CONSUMER_INFO;
			
	private:
		
		ConsumerHeartBeatInfo(const ConsumerHeartBeatInfo&);

		ConsumerHeartBeatInfo& operator= (const ConsumerHeartBeatInfo&);

	public:

		ConsumerHeartBeatInfo();

		~ConsumerHeartBeatInfo();

	public:

		virtual std::string getDataStructureType() const;

		virtual void copyDataStructure(const DataStructure *src);

		virtual bool equals(const DataStructure *value) const;

	public:

		void setStatus(uint32 status);

		uint32 getStatus()const;

		void setSpecialNumber(uint32 specialNumber);

		uint32 getSpecialNumber()const;

	private:

		uint32 status;
		uint32 specialNumber;
};

}
#endif /* end of file */
