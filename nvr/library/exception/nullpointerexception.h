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
#ifndef __LIBRARY_EXCEPTION_NULLPOINTEREXCEPTION_H__
#define __LIBRARY_EXCEPTION_NULLPOINTEREXCEPTION_H__

#include "../common/exception.h"

namespace library {

class NullPointerException : public Exception
{
	public:

		NullPointerException();

		NullPointerException(const Exception& ex);

		NullPointerException(const NullPointerException& ex);

		NullPointerException(const char* file, const int lineNumber, const char* msg, ...);

		~NullPointerException()throw();

		virtual NullPointerException* clone() const {
            return new NullPointerException(*this);
        }
};

}

#endif /* end of file */
