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
#ifndef __CORELIB_DATABASE_DATABASEORACLE_MYORACLE_H__
#define __CORELIB_DATABASE_DATABASEORACLE_MYORACLE_H__

#include <string>
#include "ocilib.h"
#include "../../../common/basetype.h"

namespace corelib {
namespace database {

class QueryResult;
class MyOracle
{
	private:

		MyOracle(const MyOracle&);
		MyOracle& operator= (const MyOracle&);

	public:

		MyOracle();
		~MyOracle();

	public:

		static void Initialize();
		void shutdown();

		bool connectDb(std::string &host, uint16 port, std::string &sid, std::string &user, std::string &passwd, uint32 mode);
		QueryResult* query(const char *sql);
		bool execute(const char *sql);

		//!binding operation
		OCI_Statement * prepare(std::string sql);
		bool execute(OCI_Statement *stmt);
		bool commit();
		
	private:

		static size_t db_count;
		OCI_Statement *m_stmt;
		OCI_Connection *m_oracle; // used to initialize database library when start
};

}
}

#endif /* end of file */