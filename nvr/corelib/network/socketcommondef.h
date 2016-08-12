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
#ifndef __CORELIB_NETWORK_SOCKETCOMMON_H__
#define __CORELIB_NETWORK_SOCKETCOMMON_H__

namespace corelib {
namespace MTP_NETWORK {

typedef enum {

	/* indicate socket is readable */
	SO_READ = 1,

	/* indicate there is some data to write */
	SO_WRITE = 2,

	/* indicate socket is timeout */
	SO_TIMED = 4,

	/* indicate socekt is closed */
	SO_CLOSE = 8
	
} SOCKETOP;

}
}

#endif /* end of file */