/***************************************************************************
 *   Copyright (C) 2015 by Y.S.G   										   *
 *   ysgen0217@163.com   												   *
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
#ifndef __LIBRARY_COMMON_HANDLER_H__
#define __LIBRARY_COMMON_HANDLER_H__

#include "../../common/basetype.h"

namespace library {

class Handler
{
	public:

		Handler() {};

		virtual ~Handler() {};

	public:

		/**
         * Flush the Handler's output, clears any buffers.
         */
        virtual void flush() = 0;

		/**
         * Publish the Log Record to this Handler
         */
        virtual void publish(const char *data, int32 size ) = 0;
};

}

#endif /* end of file */