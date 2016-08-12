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
#ifndef __CORELIB_PROTOCOL_FORMATFACTORY_H__
#define __CORELIB_PROTOCOL_FORMATFACTORY_H__

#include <string>
#include <map>

namespace corelib {
namespace protocol {

class Format;
class FormatFactory
{
	private:

		FormatFactory();

		FormatFactory(const FormatFactory&);

		FormatFactory& operator= (const FormatFactory&);

	public:

		~FormatFactory();

	public:

		static void initialize();

		static void shutdown();

		static FormatFactory& getInstance();

	public:

		Format* findFormat(const char *name);

		void registerFormat(const char *name, Format *format);

		void unRegisterAllFormat();

	private:

		std::map<std::string, Format* > formats;
};

}

}

#endif /* end of file */