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
#ifndef __CORELIB_PROTOCOL_INTERACTFORMATPROPERTY_H__
#define __CORELIB_PROTOCOL_INTERACTFORMATPROPERTY_H__

#include <string>
#include <map>

namespace corelib {

namespace protocol {

/**
 * Protocol properties:
 * user add the property for a concrete protocol
 */

class FormatProperty
{
	private:

		FormatProperty(const FormatProperty &);

		FormatProperty &operator= (const FormatProperty &);

	public:

		FormatProperty();

		virtual ~FormatProperty();

	public:

		/**
		 * add a property to the interactformat.
		 *
		 * @param name property name.
		 * @param value property value.
		 *
		 * @return void.
		 */
		virtual void addProperty(std::string &name, std::string &value);

		/**
		 * get property with a name setted
		 *
		 * @param name property name.
		 *
		 * @param return property value with the name setted by user.
		 *
		 */
		virtual std::string getProperty(std::string &name);

		/**
		 * remove property with a name setted.
		 *
		 * @param name property name.
		 *
		 * @return the value removed from interactformat.
		 *
		 */
		virtual std::string removeProperty(std::string &name);

	private:

		std::map<std::string, std::string> propertyMap;
};

}

}

#endif /* end of file */