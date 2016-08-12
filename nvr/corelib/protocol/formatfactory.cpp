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
#include "formatfactory.h"
#include "format.h"

using namespace corelib;
using namespace corelib::protocol;

namespace {

	FormatFactory* theOnlyInstance;
}

////////////////////////////////////////////////////////////////////////////////
FormatFactory::FormatFactory() : formats()
{
}

FormatFactory::~FormatFactory()
{
}

void FormatFactory::initialize()
{
	theOnlyInstance = new FormatFactory();
}

void FormatFactory::shutdown()
{
	if (theOnlyInstance) delete theOnlyInstance;
	theOnlyInstance = NULL;
}

FormatFactory& FormatFactory::getInstance()
{
	return *theOnlyInstance;
}

Format *FormatFactory::findFormat(const char *name)
{
	std::map<std::string, Format* >::iterator iter;
	std::string n = name;

	iter = formats.find(n);
	if (iter != formats.end()) {

		return iter->second;
	}
	
	return NULL;
}

void FormatFactory::registerFormat(const char *name, Format *format)
{
	std::map<std::string, Format*>::iterator iter;
	std::string n = name;

	iter = formats.find(n);
	if (iter == formats.end()) {
		formats.insert(std::pair<std::string, Format*>(n, format));
	}
	
	return;
}

void FormatFactory::unRegisterAllFormat()
{
	std::map<std::string, Format*>::iterator iter;

	iter = formats.begin();
	for (; iter != formats.end(); ++iter)
	{
		Format* format = iter->second;

		if (format) delete format;
		format = NULL;
	}

	formats.clear();
}