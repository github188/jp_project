/***************************************************************************
 *   Copyright (C) 2015 by Y.S.G   										   *
 *   ysgen0217@163.com   												   *
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
#ifndef __LIBRARY_UTIL_SYSTEM_H__
#define __LIBRARY_UTIL_SYSTEM_H__

#include "../exception/nullpointerexception.h"
#include <cstddef>

namespace library {

class System
{
	public:

		~System() {};

	protected:

		System() {};

	public:

		/**
         * Copies the number of elements specified by length from the source array starting at
         * the given source offset specified by srcPos to the dest array starting at the given
         * destination offset given by destPos.
         *
         * @param src
         *      The source array to copy from.
         * @param srcPos
         *      The position in the array to start copying from.
         * @param dest
         *      The destination array to copy to.
         * @param destPos
         *      The position in the destination array to start writing at.
         * @param length
         *      The number of elements to copy from src to dest.
         *
         * @throws NullPointerException if src or dest are NULL.
         */
         
		template <class E>
		static void arraycopy(const E *src, std::size_t srcPos, E *dst, std::size_t dstPos, std::size_t length)
		{
			if (src == NULL)
			{
				throw NullPointerException(__FILE__, __LINE__, "given src pointer is NUll");
			}

			if (dst == NULL)
			{
				throw NullPointerException(__FILE__, __LINE__, "given dst pointer is NUll");
			}

			for (std::size_t i = 0; i < length; ++i)
			{
				dst[dstPos + i] = src[srcPos + i];
			}
		}

		/**
         * Returns the current time in milliseconds. Note that while the unit of time of
         * the return value is a millisecond, the granularity of the value depends on the
         * underlying operating system and may be larger. For example, many operating
         * systems measure time in units of tens of milliseconds.
         *
         * See the description of the class Date for a discussion of slight discrepancies
         * that may arise between "computer time" and coordinated universal time (UTC).
         *
         * @returns the difference, measured in milliseconds, between the current time
         *          and midnight, January 1, 1970 UTC.
         */
        static long long currentTimeMillis();

		/**
		 * get host name.
		 *
		 * @param return host machine name, if fail, return "".
		 */
		static std::string gethostname();

		static std::string gethostbyname(std::string &hostname);

		static const std::string FromUtf8(const std::string& str);

		static const std::string ToUtf8(const std::string& str);

		static int32 u2g(char *inbuf, int32 inlen, char *outbuf, int32 outlen);
		static int32 g2u(char *inbuf, size_t inlen,char *outbuf, size_t outlen);

	private:

		static int32 charsetConvert(const char *myCharset, const char *toCharset,
				char *inbuf, int32 inlen, char *outbuf, int32 outlen);
};

}

#endif /* end of file */