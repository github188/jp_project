/*
 * base_type.h
 *
 *  Created on: 2016��1��28��
 *      Author: going
 */

#ifndef BASE_TYPE_H_
#define BASE_TYPE_H_



typedef char int8;

typedef unsigned char uint8;

typedef const char cchar;

typedef const unsigned char cuchar;

typedef short int16;

typedef unsigned short uint16;

typedef int int32;

typedef unsigned int uint32;

typedef long long int64;

typedef unsigned long long uint64;

//typedef uintptr_t Spointer;

//typedef intptr_t Uspointer;

//typedef unsigned int Bool;

//typedef float Float;

//typedef size_t  Size_t;

//typedef ssize_t Ssize_t;

const signed int STATUS_ERROR = 0xFFFFFFFF;

const signed int STATUS_LOW_ERROR = -2;

const int STATUS_SUCCESS = 0;

#ifndef FALSE
#define FALSE 0u

#define TRUE 1u
#endif



#endif /* BASE_TYPE_H_ */
