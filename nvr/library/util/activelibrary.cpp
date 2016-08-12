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
#include <time.h>

#include "../internal/threading.h"
#include "activelibrary.h"
#include "idgenerator.h"
#include "logmanager.h"
#include "logentry.h"
#include "log.h"

using namespace library;

void ActiveLibrary::initializeLibrary()
{
	Log(LOG_INFO, "ActiveLibrary::initializeLibrary - init library.");
	
	LogManagerMgr::getInstance();
	IDGeneratorMgr::getInstance();
	Threading::initialize();
}

void ActiveLibrary::shutdownLibrary()
{
	Log(LOG_INFO, "ActiveLibrary::initializeLibrary - shutdown library.");
	
	Threading::shutdown();
	LogManagerMgr::getInstance()->shutdownAll();
	DefaultLog::shutdown();
}