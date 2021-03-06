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
#include "picturepainer.h"
#include "../../library/io/file.h"
#include "../../library/util/log.h"

using namespace library;
using namespace app;

PicturePainer::PicturePainer(std::string &path, ST_PICTURES_INFO *pictures) : m_path(path), m_pictures(pictures)
{
}

PicturePainer::~PicturePainer()
{
	if (m_pictures != NULL) delete m_pictures;
}

void PicturePainer::run()
{
	if (m_pictures != NULL) {
		for (uint32 i = 0; i < m_pictures->num; ++i) {
			std::string fileName;
			char d[16] = {0};

			fileName.clear();
			fileName += m_path;

			sprintf(d, "%02d.jpg", (uint32)m_pictures->data[i].index);
			fileName += d;

			//Log(LOG_DEBUG, "PicturePainer::run - picture name: %s", fileName.c_str());
			File *file = new File(fileName, "w+");
			size_t size = file->fwrite((const char*)(m_pictures->data[i].data), 1, m_pictures->data[i].len);
			if (size != m_pictures->data[i].len) {
				Log(LOG_ERROR, "PicturePainer::run - write: %s, Fail!!!", fileName.c_str());
			}
			delete file; file = NULL;
		}

		delete m_pictures;
		m_pictures = NULL;
	}
}