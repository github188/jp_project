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
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>
#include <iconv.h>
#include <netdb.h>
#include <errno.h>
#include "system.h"

using namespace library;

namespace library {

static void *memchr(const void *s, int c, size_t n)
{
    const char *cp;

    for (cp = (const char *)s; n > 0; n--, cp++) {
        if (*cp == c)
            return (char *) cp; /* Casting away the const here */
    }

    return NULL;
}

}

////////////////////////////////////////////////////////////////////////////////
long long System::currentTimeMillis()
{
#ifdef _WIN32

    /* Number of micro-seconds between the beginning of the Windows epoch
     * (Jan. 1, 1601) and the Unix epoch (Jan. 1, 1970)
     */
    static const unsigned long long DELTA_EPOCH_IN_USEC = 116444736000000000ULL;

    unsigned long long time = 0;
    ::GetSystemTimeAsFileTime( (FILETIME*)&time );
    return ( time - DELTA_EPOCH_IN_USEC ) / 10000;

#else

    struct timeval tv;
    gettimeofday( &tv, NULL );
    return ( ( (long long)tv.tv_sec * 1000000 ) + tv.tv_usec ) / 1000;

#endif
}

std::string System::gethostname()
{
	char hostname[256] = {0};

	int32 ret = ::gethostname(hostname, sizeof(hostname));
	if (ret == -1) {
		return "";
	} else if (!memchr(hostname, '\0', sizeof(hostname))) { /* buffer too small */
		return "";
	}

	return std::string(hostname);
}

/**
 * struct  hostent
 *  {
 *   char *h_name;           // 正式的主机名称
 *   char **h_aliases;       // 指向主机名称的其他别名
 *   int h_addrtype;         // 地址的型态, 通常是AF_INET
 *   int h_length;           // 地址的长度
 *   char **h_addr_list;     // 从域名服务器取得该主机的所有地址
 *  };
 *
 * NOTE: this function is not thread safe.
 */
std::string System::gethostbyname(std::string &hostname)
{
	struct hostent *host = NULL;
	char ip[256] = {0};
	struct in_addr addr;
	
	if (hostname.size() <= 0) return "";

	host = ::gethostbyname(hostname.c_str());
	if (host == NULL) return "";

	memcpy(&addr, host->h_addr_list[0], 4);
	inet_ntop(AF_INET, &addr, ip, sizeof(ip));

	return std::string(ip);
}

const std::string System::FromUtf8(const std::string& str)
{
	if (!str.size())
		return "";
	std::string r;
	for (size_t i = 0; i < str.size(); ++i)
	{
		if (i < str.size() - 1 && (str[i] & 0xe0) == 0xc0 && (str[i + 1] & 0xc0) == 0x80)
		{
			int c1 = str[i] & 0x1f;
			int c2 = str[++i] & 0x3f;
			int c = (c1 << 6) + c2;
			r += (char)c;
		}
		else
		{
			r += str[i];
		}
	}
	return r;
}

// 110yyyxx 10xxxxxx	

const std::string System::ToUtf8(const std::string& str)
{
	if (str.empty())
		return "";
	std::string r;
	for (size_t i = 0; i < str.size(); ++i)
	{
		if (((unsigned)str[i] & 0x80) == 0x80)
		{
			r += (str[i] >> 6) | 0xc0;
			r += (str[i] & 0x3f) | 0x80;
		}
		else
		{
			r += str[i];
		}
	}
	return r;
}

int32 System::u2g(char *inbuf, int32 inlen, char *outbuf, int32 outlen)
{
	return charsetConvert("utf-8", "gb2312", inbuf, inlen, outbuf, outlen);
}

int32 System::g2u(char *inbuf, size_t inlen,char *outbuf, size_t outlen)
{
	return charsetConvert("gb2312", "utf-8", inbuf, inlen, outbuf, outlen); 
}

int32 System::charsetConvert(const char *myCharset, const char *toCharset, char *inbuf,
							int32 inlen, char *outbuf, int32 outlen)
{
	char **pin = &inbuf;
	char **pout = &outbuf;
	iconv_t cd;

	if (inbuf == NULL || outbuf == NULL) return -1;
	if (strlen(inbuf) <= 0) return -1;
	
	cd = iconv_open(toCharset, myCharset);
	if (cd == 0)
	{
		iconv_close(cd);
		return -1;
	}
	::memset(outbuf,0,outlen); 
	iconv(cd, pin, (size_t*)&inlen, pout, (size_t*)&outlen);
//	if (iconv(cd, pin, (size_t*)&inlen, pout, (size_t*)&outlen) == -1)
//	{
//		iconv_close(cd);
//		return -1;
//	}
	iconv_close(cd);
	
	return 0;
}