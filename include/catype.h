/*
    Copyright 2015 Colby Skeggs

    This file is part of libca. See LICENSE.txt for details.

    libca is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    libca is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with libca.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _CATYPE_H
#define _CATYPE_H

#ifndef __GNUC__
# warning libca has only been tested on gcc!
#endif

#include <caversion.h>

#define bool _Bool
#define true 1
#define false 0

#define offsetof(type, member)  __builtin_offsetof(type, member)

#if !(defined(__CHAR_BIT__)) || __CHAR_BIT__ != 8
# error 8-bit bytes required!
#endif

#define NULL ((void*) 0)

#if !defined(__INT8_TYPE__) || !defined(__INT16_TYPE__) || !defined(__INT32_TYPE__) || !defined(__INT64_TYPE__)
# error Required types were not defined! Maybe this only works on gcc?
#endif

typedef __INT8_TYPE__ i8;
typedef __INT16_TYPE__ i16;
typedef __INT32_TYPE__ i32;
typedef __INT64_TYPE__ i64;

#if !defined(__UINT8_TYPE__) || !defined(__UINT16_TYPE__) || !defined(__UINT32_TYPE__) || !defined(__UINT64_TYPE__)
# error Required types were not defined! Maybe this only works on gcc?
#endif

typedef __UINT8_TYPE__ u8;
typedef __UINT16_TYPE__ u16;
typedef __UINT32_TYPE__ u32;
typedef __UINT64_TYPE__ u64;

#if !defined(__UINTPTR_TYPE__) || !defined(__INTPTR_TYPE__) || !defined(__PTRDIFF_TYPE__) || !defined(__SIZE_TYPE__)
# error Required types were not defined! Maybe this only works on gcc?
#endif

typedef __UINTPTR_TYPE__ uptr;
typedef __INTPTR_TYPE__ iptr;
typedef __PTRDIFF_TYPE__ iptrd; // pointer difference
typedef __SIZE_TYPE__ ulen; // array length

typedef const char *string;
typedef char *mutable_string;

#endif
