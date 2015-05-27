/*
 * Copyright (c) 2015 Colby Skeggs
 * This file is part of libca, and is licensed under the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _STRING_H
#define _STRING_H

#include <catype.h>

// max includes the null terminator.
mutable_string strmov(mutable_string restrict dest, ulen max, string restrict src); // panic on overflow
mutable_string strmov_t(mutable_string restrict dest, ulen max, string restrict src); // truncate on overflow
mutable_string strapnd(mutable_string restrict dest, ulen max, string restrict src);
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

typedef struct {
	ulen count;
	string *strings;
} strsplit_out;
strsplit_out *strsplit(string str, string delimiter, bool compress);
void strsplit_free(strsplit_out *struc);

// if remain is NULL, then will panic if invalid. otherwise, will be set to point to the first unconsumed character.
u8 parseu8(string data, string *remain, u8 base);
u16 parseu16(string data, string *remain, u8 base);
u32 parseu32(string data, string *remain, u8 base);
u64 parseu64(string data, string *remain, u8 base);
i8 parsei8(string data, string *remain, u8 base);
i16 parsei16(string data, string *remain, u8 base);
i32 parsei32(string data, string *remain, u8 base);
i64 parsei64(string data, string *remain, u8 base);

u8 showui(mutable_string out, ulen max, u64 number);
u8 showsi(mutable_string out, ulen max, i64 number);

#endif
