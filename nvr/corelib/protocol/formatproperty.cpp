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
#include "formatproperty.h"

using namespace corelib;
using namespace corelib::protocol;

FormatProperty::FormatProperty() : propertyMap()
{
	propertyMap.clear();
}

FormatProperty::~FormatProperty()
{
	propertyMap.clear();
}

void FormatProperty::addProperty(std::string &name, std::string &value)
{
	std::map<std::string, std::string>::iterator iter;

	iter = propertyMap.find(name);
	if (iter == propertyMap.end()) {

		propertyMap.insert(std::pair<std::string, std::string>(name, value));
	} else {

		propertyMap[name] = value;
	}
}

std::string FormatProperty::getProperty(std::string &name)
{
	std::map<std::string, std::string>::iterator iter;

	iter = propertyMap.find(name);
	if (iter == propertyMap.end()) {

		return "";
	} else {

		return iter->second;
	}

	return "";
}

std::string FormatProperty::removeProperty(std::string &name)
{
	std::map<std::string, std::string>::iterator iter;
	std::string value;

	iter = propertyMap.find(name);
	if (iter == propertyMap.end()) {

		propertyMap.erase(iter);
		value = iter->second;
	}

	return value;
}