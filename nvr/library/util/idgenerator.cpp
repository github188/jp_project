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
#include <stdlib.h>
#include <time.h>

#include "idgenerator.h"

using namespace library;

////////////////////////////////////////////////////////////////////////////////
IDGenerator::IDGenerator() : randomId(0)
{
	::srand((int64)time(NULL));

	this->randomId = (10000.0 * rand()/(RAND_MAX + 1.0));
}

IDGenerator::IDGenerator(int64 seed) : randomId(0)
{
	::srand(seed);

	this->randomId = (10000.0 * rand()/(RAND_MAX + 1.0));
}

IDGenerator::IDGenerator(const IDGenerator &src)
{
	this->randomId = src.randomId;
}

IDGenerator& IDGenerator::operator= (const IDGenerator &src)
{
	if (this == &src) {
		return *this;
	}

	this->randomId = src.randomId;

	return *this;
}

IDGenerator::~IDGenerator()
{
}

IDGenerator& IDGenerator::operator++ ()
{
	increase(1);

	return *this;
}

IDGenerator& IDGenerator::operator-- ()
{
	increase(-1);

	return *this;
}

const IDGenerator IDGenerator::operator++ (int32)
{
	IDGenerator ret(*this);

	++*this;

	return ret;
}

const IDGenerator IDGenerator::operator-- (int32)
{
	IDGenerator ret(*this);

	--*this;

	return ret;
}

IDGenerator& IDGenerator::operator+= (int32 unit)
{
	increase(unit);

	return *this;
}

IDGenerator& IDGenerator::operator-= (int32 unit)
{
	increase(unit);

	return *this;
}

uint64 IDGenerator::increase(int32 unit)
{
	this->randomId += unit;

	return this->randomId;
}