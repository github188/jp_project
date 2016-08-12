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
#include "field.h"

using namespace corelib;
using namespace corelib::database;

Field::Field() : m_value(NULL), m_name(NULL), m_type(DB_TYPE_UNKNOWN)
{
}

Field::Field(const Field &f)
{
	const char *value = NULL, *name = NULL;
	
	value = f.getString();
	name = f.getName();

	if (value && (m_value = new char[strlen(value) + 1]))
		strcpy(m_value, value);
	else
		m_value = NULL;

	if (name && (m_name = new char[strlen(name) + 1]))
		strcpy(m_name, name);
	else
		m_name = NULL;

	m_type = f.getType();
}

Field::Field(const char *value, const char *name, enum Field::DataTypes type) : m_type(type)
{
	if (value && (m_value = new char[strlen(value) + 1]))
		strcpy(m_value, value);
	else
		m_value = NULL;

	if (name && (m_name = new char[strlen(name) + 1]))
		strcpy(m_name, name);
	else
		m_name = NULL;
}

Field::~Field()
{
	delete []m_value;
	delete []m_name;
}

void Field::setName(const char *name)
{
	if (m_name != NULL) delete []m_name;

	if (name) {
		m_name = new char[strlen(name) + 1];
        strcpy(m_name, name);
		return;
	}

	m_name = NULL;
}

void Field::setValue(const char *value)
{
	if (m_value != NULL) delete []m_value;

	if (value) {
		m_value = new char[strlen(value) + 1];
        strcpy(m_value, value);

		return;
	}

	m_value = NULL;
}