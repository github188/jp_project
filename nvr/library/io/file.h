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
#ifndef __LIBRARY_IO_FILE_H__
#define __LIBRARY_IO_FILE_H__

#include <stdio.h>
#include "ifile.h"
#include "../../common/basetype.h"

namespace library {

class File : public IFile
{
	private:

		File(const File&);
		File& operator= (const File&);

	public:

		File();
		File(FILE *);
		File(const std::string& path, const std::string& mode);
		~File();

	public:

		bool fopen(const std::string& path, const std::string& mode);
		void fclose() const;

		size_t fread(char *, size_t, size_t) const;
		size_t fwrite(const char *, size_t, size_t);

		char *fgets(char *, int) const;
		void fprintf(const char *format, ...);

		off_t size() const;
		bool eof() const;

		void reset_read() const;
		void reset_write();

		const std::string& path() const;

	private:

		int32 getPathString(const char *filename, std::string &pathInfo);

		int32 createDirent(const char *pcPath);

		bool isExsit(const char * pcName);

	private:

		std::string m_path;
		std::string m_mode;
		mutable FILE *m_fil;
		bool m_b_close;
		mutable long m_rptr;
		long m_wptr;
};

}
#endif /* end of file */