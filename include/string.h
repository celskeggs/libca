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

#ifndef _STRING_H
#define _STRING_H

#include <catype.h>

// max includes the null terminator.
mutable_string strmov(mutable_string restrict dest, ulen max, string restrict src); // panic on overflow
mutable_string strmov_t(mutable_string restrict dest, ulen max, string restrict src); // truncate on overflow
mutable_string strapnd(mutable_string restrict dest, ulen max, string restrict src); // panic on overflow
mutable_string strapnd_t(mutable_string restrict dest, ulen max, string restrict src); // truncate on overflow

ulen strlen(string str);
ulen strnlen(string str, ulen max);
i16 strcmp(string lhs, string rhs);
i16 strncmp(string lhs, string rhs, ulen max);
bool streq(string lhs, string rhs);
bool strneq(string lhs, string rhs, ulen max);
string strchr(string str, u8 chr);
string strrchr(string str, u8 chr);
string strstr(string str, string substr);

mutable_string strdup(string str);
mutable_string strndup(string str, ulen count);

typedef struct {
	ulen count;
	string *strings;
} strsplit_out;
strsplit_out *strsplit(string str, string delimiter, bool compress);
void strsplit_free(strsplit_out *struc);

// if remain is NULL, then will panic if invalid. otherwise, will be set to point to the first unconsumed character.
u8 parseu8(string data, string *remain);
u16 parseu16(string data, string *remain);
u32 parseu32(string data, string *remain);
u64 parseu64(string data, string *remain);
i8 parsei8(string data, string *remain);
i16 parsei16(string data, string *remain);
i32 parsei32(string data, string *remain);
i64 parsei64(string data, string *remain);

u8 showui(mutable_string out, ulen max, u64 number);
u8 showsi(mutable_string out, ulen max, i64 number);

#endif
