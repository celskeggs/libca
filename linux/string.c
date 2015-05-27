/*
    This file is part of libca/linux. See LICENSE.txt for details.

    libca/linux is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    libca/linux is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with libca/linux.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <string.h>
#include <memory.h>
#include <alloc.h>
#include <panic.h>

ulen strlen(string str) {
	ulen out = 0;
	while (str[out]) {
		out++;
	}
	return out;
}
ulen strnlen(string str, ulen max) {
	ulen out = 0;
	while (out < max && str[out]) {
		out++;
	}
	return out;
}
mutable_string strdup(string str) {
	ulen count = strlen(str) + 1;
	mutable_string out = (mutable_string) malloc(count);
	memcpy(out, str, count);
	return out;
}

i16 strncmp(string lhs, string rhs, ulen max) {
	if (max == 0) {
		return 0;
	}
	string maxl = lhs + max;
	while (*lhs == *rhs && lhs < maxl - 1) {
		if (!*lhs) {
			return 0;
		}
		lhs++;
		rhs++;
	}
	return ((i16) *lhs) - ((i16) *rhs);
}

i16 strcmp(string lhs, string rhs) {
	while (*lhs == *rhs) {
		if (!*lhs) {
			return 0;
		}
		lhs++;
		rhs++;
	}
	return ((i16) *lhs) - ((i16) *rhs);
}

bool strneq(string lhs, string rhs, ulen n) {
	return strncmp(lhs, rhs, n) == 0;
}

bool streq(string lhs, string rhs) {
	return strcmp(lhs, rhs) == 0;
}

mutable_string strmov(mutable_string restrict dest, ulen max, string restrict src) {
	if (max == 0) {
		panic_static("strmov passed zero-length buffer");
	}
	mutable_string orig = dest;
	string end = dest + max - 1; // leave an extra space for the ending '\0'
	while (*src) {
		if (dest >= end) {
			panic_static("strmov overflow");
		}
		*dest++ = *src++;
	}
	*dest = 0;
	return orig;
}

mutable_string strmov_t(mutable_string restrict dest, ulen max, string restrict src) {
	if (max == 0) {
		return dest; // do nothing!
	}
	mutable_string orig = dest;
	string end = dest + max - 1; // leave an extra space for the ending '\0'
	while (dest < end && *src) {
		*dest++ = *src++;
	}
	*dest = 0;
	return orig;
}

/* TODO
mutable_string strapnd(mutable_string restrict dest, ulen max, string restrict src);
string strchr(string str, u8 chr);
string strrchr(string str, u8 chr);
string strstr(string str, string substr);

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
*/
