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
#include "../protocol/formatfactory.h"
#include "../../library/util/log.h"
#include "activecorelib.h"

using namespace library;
using namespace corelib;
using namespace corelib::protocol;

void ActiveCorelib::initializeCoreLibrary()
{
	Log(LOG_INFO, "ActiveCorelib::initializeCoreLibrary - init corelibrary.");

	FormatFactory::initialize();
}

void ActiveCorelib::shutdownCoreLibrary()
{
	Log(LOG_INFO, "ActiveCorelib::initializeCoreLibrary - shutdown corelibrary.");

	FormatFactory::shutdown();
}