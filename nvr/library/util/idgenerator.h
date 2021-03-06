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
#ifndef __LIBRARY_UTIL_IDGENERATOR_H__
#define __LIBRARY_UTIL_IDGENERATOR_H__

#include "../../library/util/singleton.h"
#include "../../common/basetype.h"

namespace library {

//Random int32 data generator.
class IDGenerator
{
	public:

		IDGenerator();
		
		/* seed must different: current time is ok. */
		IDGenerator(int64 seed);

		IDGenerator(const IDGenerator&);

		IDGenerator& operator= (const IDGenerator&);

		~IDGenerator();

	public:

		IDGenerator& operator++ (); //prefix ++

		IDGenerator& operator-- (); //prefix ++

		// invaild for '++++'
		const IDGenerator operator++ (int32); //postfix ++

		const IDGenerator operator-- (int32); //postfix ++

		IDGenerator& operator+= (int32 unit);

		IDGenerator& operator-= (int32 unit);

		uint64 increase(int32 unit = 1);

		uint64 getId()const {return this->randomId;};		

	private:

		uint64 randomId;
};

/////////////////////////////// the global instance/////////////////////////////
typedef Singleton<IDGenerator> IDGeneratorMgr;
////////////////////////////////////////////////////////////////////////////////

}

#endif /* end of file */
