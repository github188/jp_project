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
#ifndef __APP_COMMON_BLACKLIST_H__
#define __APP_COMMON_BLACKLIST_H__

#include <map>
#include <string>
#include "eprotocol.h"
#include "../../library/concurrent/lock.h"

namespace app {

class BlackList
{
	private:

		BlackList(const BlackList&);
		BlackList& operator= (const BlackList&);

	public:

		BlackList();
		~BlackList();

	public:

		void configUpdate(ST_BALCKLIST_CONFIG_INFO &info);
		void configDrop();
		bool check(ST_CAR_TRAP_INFO &trap);

		size_t getBlackListSize() const {return _blackList.size();};

	private:

		library::Lock m_lock;
		std::map<std::string, ST_BALCKLIST_CONFIG_INFO> _blackList;
};

}

#endif /* end of file */