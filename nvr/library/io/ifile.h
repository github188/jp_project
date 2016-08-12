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
#ifndef __LIBRARY_IO_IFILE_H__
#define __LIBRARY_IO_IFILE_H__

#include <string>
#include <cstdio>

namespace library {

class IFile
{
	private:

		IFile(const IFile &);
		IFile& operator= (const IFile&);

	public:

		IFile() {};
		virtual ~IFile() {};

		virtual bool fopen(const std::string&, const std::string&) = 0;
		virtual void fclose()const = 0;

		virtual size_t fread(char *, size_t, size_t)const = 0;
		virtual size_t fwrite(const char *, size_t, size_t) = 0;

		virtual char *fgets(char *, int) const = 0;
		virtual void fprintf(const char *format, ...) = 0;

		virtual off_t size() const = 0;
		virtual bool eof() const = 0;

		virtual void reset_read() const = 0;
		virtual void reset_write() = 0;

		virtual const std::string& path() const = 0;
};

}

#endif /* end of file */