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
#include "file.h"
#include <stdio.h>
#include <stdlib.h>
#include "filestreamhandler.h"

using namespace library;

FileStreamHandler::FileStreamHandler(std::string &fileName, size_t size) : Handler(),
m_file(NULL), m_size(size), m_fileName(fileName)
{
	m_file = new File(m_fileName, "w+");
}

FileStreamHandler::~FileStreamHandler()
{
	if (m_file != NULL) {
		delete m_file;
		m_file = NULL;
	}
}

void FileStreamHandler::flush()
{
}

void FileStreamHandler::publish(const char * data, int32 size)
{
	if (m_file->size()/1024 >= (off_t)m_size) {
		char cmd[1024] = {0};
		sprintf(cmd, "echo 0 > %s", m_fileName.c_str());
		system(cmd);
		m_file->reset_write();
	}
	
	m_file->fwrite(data, 1, size);
}