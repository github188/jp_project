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
#ifndef __APP_COMMON_CONTROLLIST_H__
#define __APP_COMMON_CONTROLLIST_H__

#include <regex.h>
#include <string>
#include <map>

#include "eprotocol.h"
#include "../../library/concurrent/lock.h"

namespace app {

/* support two bytes for single chinese charactor */
#define CHINESE_CHARACTORS_IN_BYTES 3

typedef regex_t REGEX_T;

class HpRegExpressTransfer
{
	public:
		HpRegExpressTransfer();

		HpRegExpressTransfer(const char *pcpattern);

		~HpRegExpressTransfer();

	public:
		int32 setpattern(const char *pcpattern);

		bool search(const char *pcStr);

	private:
		int32 generatereg(const char *pcpattern);

		int32 destroyreg();

	private:
		REGEX_T reg;
};

////////////////////////////////////////////////////////////////////////////////
class ControlList
{
	private:

		ControlList(const ControlList&);
		ControlList& operator= (const ControlList&);

	public:

		ControlList();
		~ControlList();

	public:

		void configUpdate(ST_CONTROLLIST_CONFIG_INFO &info);
		void configDrop();
		bool check(ST_CAR_TRAP_INFO &trap);

		size_t getControlListSize() const {return concretelist.size() + abstractlist.size();};

	private:

		library::Lock m_lock;
		std::map<std::string, ST_CONTROLLIST_CONFIG_INFO> concretelist;
		std::map<std::string, ST_CONTROLLIST_CONFIG_INFO> abstractlist;
};

}

#endif /* end of file */