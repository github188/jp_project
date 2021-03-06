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
#ifndef __APP_COMMON_SQLCOMMIT_H__
#define __APP_COMMON_SQLCOMMIT_H__

#include <string>

#include "../../corelib/database/common/sqloperation.h"
#include "../../corelib/database/databaseoracle/ocilib.h"

namespace app {

namespace CD = corelib::database;
class SqlCommit : public CD::SqlOperation
{
	private:

		SqlCommit(const SqlCommit&);
		SqlCommit& operator= (const SqlCommit&);

	public:

		SqlCommit();
		~SqlCommit();

	public:

		virtual void Execute(CD::Database *db);

	private:
};

}
#endif /* end of file */