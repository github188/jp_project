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
#include <stdio.h>

#include "sqlstring.h"

using namespace corelib;
using namespace corelib::database;

SqlString::SqlString()
{
}

SqlString::~SqlString()
{
}

void SqlString::append(int32 i)
{
	char buf[24] = {0};

	/* int32 len = */snprintf(buf, sizeof(buf), "%d", i);
	sql_.append(buf);
}
		
void SqlString::append(uint32 i)
{
	char buf[24] = {0};

	/* int32 len = */snprintf(buf, sizeof(buf), "%u", i);
	sql_.append(buf);
}

void SqlString::append(long i)
{
	char buf[24] = {0};

	/* int32 len = */snprintf(buf, sizeof(buf), "%ld", i);
	sql_.append(buf);
}

void SqlString::append(unsigned long i)
{
	char buf[24] = {0};

	/* int32 len = */snprintf(buf, sizeof(buf), "%lu", i);
	sql_.append(buf);
}

void SqlString::append(int64 i)
{
	char buf[24] = {0};

	/* int32 len = */snprintf(buf, sizeof(buf), "%lld", i);
	sql_.append(buf);
}

void SqlString::append(uint64 i)
{
	char buf[24] = {0};

	/* int32 len = */snprintf(buf, sizeof(buf), "%llu", i);
	sql_.append(buf);
}

void SqlString::append(const std::string &str)
{
	sql_.append(str);
}