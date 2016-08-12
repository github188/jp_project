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
#ifndef __APP_COMMON_ZHUHAIDATAREQUESTLENGTHCODEC_H__
#define __APP_COMMON_ZHUHAIDATAREQUESTLENGTHCODEC_H__

#include "../../common/basetype.h"

namespace corelib {
namespace MTP_NETWORK {

	class Connection;
}
}

namespace app {

namespace CM = corelib::MTP_NETWORK;
class ZhuhaiDataRequestLengthCodec
{
	private:

		ZhuhaiDataRequestLengthCodec(const ZhuhaiDataRequestLengthCodec&);
		ZhuhaiDataRequestLengthCodec& operator= (const ZhuhaiDataRequestLengthCodec&);

	public:

		ZhuhaiDataRequestLengthCodec();
		~ZhuhaiDataRequestLengthCodec();

	public:

		int32 decode(CM::Connection *connection);
};

}
#endif /* end of file */