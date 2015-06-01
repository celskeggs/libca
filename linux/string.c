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

#include <string.h>
#include <memory.h>
#include <alloc.h>
#include <panic.h>
#include <assert.h>

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

mutable_string strapnd(mutable_string restrict dest, ulen max, string restrict src) {
	if (max == 0) {
		panic_static("strapnd passed zero-length buffer");
	}
	mutable_string orig = dest;
	string end = dest + max;
	while (dest < end && *dest) {
		dest++;
	}
	if (dest >= end) {
		panic_static("strapnd passed overflowed buffer");
	} else { // dest points to the terminator
		end--; // leave a space for the ending '\0'
		while (*src) {
			if (dest >= end) {
				panic_static("strapnd overflow");
			}
			*dest++ = *src++;
		}
		*dest = 0;
	}
	return orig;
}

mutable_string strapnd_t(mutable_string restrict dest, ulen max, string restrict src) {
	if (max == 0) {
		return dest; // do nothing!
	}
	mutable_string orig = dest;
	string end = dest + max;
	while (dest < end && *dest) {
		dest++;
	}
	if (dest < end) {
		strmov_t(dest, end - dest, src);
	}
	return orig;
}

u64 parseu64(string data, string *remain) {
	if (remain == NULL) {
		if (*data == 0) {
			panic_static("parsenum got no number");
		}
		if (*data == '-') {
			panic_static("parsenum got a negative number");
		}
		string str;
		u64 out = parseu64(data, &str);
		if (str == NULL) {
			panic_static("parsenum found a too-large number");
		} else if (*str != '\0') {
			if (str == data) {
				panic_static("parsenum did not find a number");
			} else if (*str >= '0' && *str <= '9') {
				panic_static("parsenum found a too-long number");
			} else {
				panic_static("parsenum found garbage after a number");
			}
		}
		return out;
	}
	u64 out = 0;
	while (*data >= '0' && *data <= '9') {
		if (__builtin_mul_overflow(out, 10, &out) || __builtin_add_overflow(out, *data - '0', &out)) {
			*remain = NULL;
			return out;
		}
		data++;
	}
	*remain = data;
	return out;
}
u32 parseu32(string data, string *remain) {
	u64 out = parseu64(data, remain);
	if (out > U32_MAX) {
		if (remain == NULL) {
			panic_static("parsenum found a too-large number");
		} else {
			*remain = NULL;
		}
	}
	return out;
}
u16 parseu16(string data, string *remain) {
	u64 out = parseu64(data, remain);
	if (out > U16_MAX) {
		if (remain == NULL) {
			panic_static("parsenum found a too-large number");
		} else {
			*remain = NULL;
		}
	}
	return out;
}
u8 parseu8(string data, string *remain) {
	u64 out = parseu64(data, remain);
	if (out > U8_MAX) {
		if (remain == NULL) {
			panic_static("parsenum found a too-large number");
		} else {
			*remain = NULL;
		}
	}
	return out;
}

i64 parsei64(string data, string *remain) {
	bool isneg = (*data == '-');
	u64 out = parseu64(isneg ? data + 1 : data, remain);
	if (out > I64_MAX && !(out == ((u64) I64_MAX) + 1 && isneg)) {
		if (remain == NULL) {
			panic_static("parsenum found a too-large number");
		} else {
			*remain = NULL;
			return out;
		}
	}
	return isneg ? -out : out;
}
i32 parsei32(string data, string *remain) {
	bool isneg = (*data == '-');
	u64 out = parseu64(isneg ? data + 1 : data, remain);
	if (out > I32_MAX && !(out == ((u32) I32_MAX) + 1 && isneg)) {
		if (remain == NULL) {
			panic_static("parsenum found a too-large number");
		} else {
			*remain = NULL;
			return out;
		}
	}
	return isneg ? -out : out;
}
i16 parsei16(string data, string *remain) {
	bool isneg = (*data == '-');
	u64 out = parseu64(isneg ? data + 1 : data, remain);
	if (out > I16_MAX && !(out == ((u16) I16_MAX) + 1 && isneg)) {
		if (remain == NULL) {
			panic_static("parsenum found a too-large number");
		} else {
			*remain = NULL;
			return out;
		}
	}
	return isneg ? -out : out;
}
i8 parsei8(string data, string *remain) {
	bool isneg = (*data == '-');
	u64 out = parseu64(isneg ? data + 1 : data, remain);
	if (out > I8_MAX && !(out == ((u8) I8_MAX) + 1 && isneg)) {
		if (remain == NULL) {
			panic_static("parsenum found a too-large number");
		} else {
			*remain = NULL;
			return out;
		}
	}
	return isneg ? -out : out;
}

static inline u8 numlen(u64 number) { // assumes that number > 0
	u8 out = 0;
	while (number > 0) {
		number /= 10;
		out++;
	}
	return out;
}

u8 showui(mutable_string out, ulen max, u64 number) {
	if (max < 2) { // one for null terminator and one for output character
		panic_static("showint buffer is too short");
	} else if (number == 0) {
		*out++ = '0';
		*out = '\0';
		return 1;
	}
	// TODO: optimize this so that we don't have to do the same divisions twice
	u8 needed = numlen(number);
	if (max < needed + 1) {
		panic_static("showint buffer is too short");
	}
	string orig = out;
	out += needed;
	*out = '\0';
	while (number > 0) {
		*--out = (number % 10) + '0';
		number /= 10;
	}
	assert(out == orig);
	return needed;
}

u8 showsi(mutable_string out, ulen max, i64 number) {
	if (max < 2) { // one for null terminator and one for output character
		panic_static("showint buffer is too short");
	}
	if (number < 0) {
		*out = '-';
		return showui(out + 1, max - 1, -number) + 1;
	} else {
		return showui(out, max, number);
	}
}

string strchr(string str, u8 chr) {
	while (*str && *str != chr) {
		str++;
	}
	return *str || chr == 0 ? str : NULL;
}
string strrchr(string str, u8 chr) {
	if (chr == 0) {
		while (*str) {
			str++;
		}
		return str;
	}
	string out = NULL;
	while (*str) {
		if (*str == chr) {
			out = str;
		}
		str++;
	}
	return out;
}
string strstr(string str, string substr) {
	char first = *substr;
	if (first == 0) {
		return str;
	}
	ulen count = strlen(substr);
	while (*str) {
		if (*str == first && strneq(str + 1, substr + 1, count - 1)) {
			return str;
		}
		str++;
	}
	return NULL;
}

/* TODO
typedef struct {
	ulen count;
	string *strings;
} strsplit_out;
strsplit_out *strsplit(string str, string delimiter, bool compress);
void strsplit_free(strsplit_out *struc);
*/
