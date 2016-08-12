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
#ifndef __CORELIB_NETWORK_IOEVENT_H__
#define __CORELIB_NETWORK_IOEVENT_H__

#include <string>
#include <map>

#include "../../common/basetype.h"
#include "socketcommondef.h"

namespace corelib {
namespace MTP_NETWORK {

enum ROLER {

	SERVER = 0,
	CONNECTION = 1,
};

/* base class for event. */
class IOEvent
{
	public:

		IOEvent(ROLER roler);

		virtual ~IOEvent();

	public:

		virtual void handle(std::string& socketId, SOCKETOP op);

		virtual ROLER getRoler();

	protected:

		ROLER roler_;
};

}
}

#endif /* end of file */
