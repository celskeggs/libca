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

#include <system.h>
#include <assert.h>
#include <string.h>
#include <alloc.h>

/*
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
*/

struct params {
	u8 e;
	string buffer;
};

void panicdispatch(void *v) {
	struct params *params = (struct params *) v;
	string buffer = params->buffer;
	switch (params->e) {
	case 0: parseu64(buffer, NULL); break;
	case 1: parseu32(buffer, NULL); break;
	case 2: parseu16(buffer, NULL); break;
	case 3: parseu8(buffer, NULL); break;
	case 4: parsei64(buffer, NULL); break;
	case 5: parsei32(buffer, NULL); break;
	case 6: parsei16(buffer, NULL); break;
	case 7: parsei8(buffer, NULL); break;
	default: panic_static("panicdispatch nope");
	}
}

void attemptu(u64 i) {
	char buffer[64];
	struct params params = {8, buffer};
	error err;
	showui(buffer, sizeof(buffer) / sizeof(char), i);
	assert(parseu64(buffer, NULL) == i);
	string out;
	u32 i32 = parseu32(buffer, &out);
	if (i > U32_MAX) {
		assert(out == NULL || *out != '\0'); // should not be properly parsed!
		params.e = 1;
		assert(recover(panicdispatch, &params, &err));
	} else {
		assert(out != NULL && *out == '\0');
		assert(i32 == i);
		parseu32(buffer, NULL); // require no panic
	}
	u16 i16 = parseu16(buffer, &out);
	if (i > U16_MAX) {
		assert(out == NULL || *out != '\0'); // should not be properly parsed!
		params.e = 2;
		assert(recover(panicdispatch, &params, &err));
	} else {
		assert(out != NULL && *out == '\0');
		assert(i16 == i);
		parseu16(buffer, NULL); // require no panic
	}
	u8 i8 = parseu8(buffer, &out);
	if (i > U8_MAX) {
		assert(out == NULL || *out != '\0'); // should not be properly parsed!
		params.e = 3;
		assert(recover(panicdispatch, &params, &err));
	} else {
		assert(out != NULL && *out == '\0');
		assert(i8 == i);
		parseu8(buffer, NULL); // require no panic
	}
}

void attempti(i64 i) {
	char buffer[64];
	struct params params = {8, buffer};
	error err;
	showsi(buffer, sizeof(buffer) / sizeof(char), i);
	assert(parsei64(buffer, NULL) == i);
	string out;
	i32 i32 = parsei32(buffer, &out);
	if (i < I32_MIN || i > I32_MAX) {
		assert(out == NULL || *out != '\0'); // should not be properly parsed!
		params.e = 5;
		assert(recover(panicdispatch, &params, &err));
	} else {
		assert(out != NULL && *out == '\0');
		assert(i32 == i);
		parsei32(buffer, NULL); // require no panic
	}
	i16 i16 = parsei16(buffer, &out);
	if (i < I16_MIN || i > I16_MAX) {
		assert(out == NULL || *out != '\0'); // should not be properly parsed!
		params.e = 6;
		assert(recover(panicdispatch, &params, &err));
	} else {
		assert(out != NULL && *out == '\0');
		assert(i16 == i);
		parsei16(buffer, NULL); // require no panic
	}
	i8 i8 = parsei8(buffer, &out);
	if (i < I8_MIN || i > I8_MAX) {
		assert(out == NULL || *out != '\0'); // should not be properly parsed!
		params.e = 7;
		assert(recover(panicdispatch, &params, &err));
	} else {
		assert(out != NULL && *out == '\0');
		assert(i8 == i);
		parsei8(buffer, NULL); // require no panic
	}
}

void iterate_recurse(u64 num, u8 remain) {
	if (remain == 0) {
		attemptu(num);
		attempti(num);
		return;
	} else if (remain == 4) { // 32 bits ready
		attemptu((u32) num);
		attempti((u32) num);
	} else if (remain == 6) { // 16 bits ready
		attemptu((u16) num);
		attempti((u16) num);
	} else if (remain == 7) { // 8 bits ready
		attemptu((u8) num);
		attempti((i8) num);
	}
	static const u8 x[] = {1, 182, 42, 0, 13, 255, 114};
	for (u8 i=0; i<sizeof(x) / sizeof(u8); i++) {
		iterate_recurse((num << 8) | x[i], remain - 1);
	}
}

CAMAIN0() {
	struct params params;
	error err; // TODO: properly deallocate this if necessary.

	assert(parseu8("0", NULL) == 0);
	assert(parseu16("0", NULL) == 0);
	assert(parseu32("0", NULL) == 0);
	assert(parseu64("0", NULL) == 0);
	assert(parsei8("0", NULL) == 0);
	assert(parsei16("0", NULL) == 0);
	assert(parsei32("0", NULL) == 0);
	assert(parsei64("0", NULL) == 0);

	assert(parseu8("177", NULL) == 177);
	string out;
	parseu8("177", &out);
	assert(out != NULL && *out == 0);

	parseu64("18446744073709551616", &out);
	assert(out == NULL || *out != 0);

	params.e = 0;
	params.buffer = "18446744073709551616";
	assert(recover(panicdispatch, &params, &err));

	parsei64("18446744073709551616", &out);
	assert(out == NULL || *out != 0);

	params.e = 4;
	params.buffer = "18446744073709551616";
	assert(recover(panicdispatch, &params, &err));

	parsei64("-18446744073709551616", &out);
	assert(out == NULL || *out != 0);

	params.e = 4;
	params.buffer = "-18446744073709551616";
	assert(recover(panicdispatch, &params, &err));

	char buf[64];
	showui(buf, 64, 0);
	assert(streq(buf, "0"));
	showsi(buf, 64, 0);
	assert(streq(buf, "0"));
	showui(buf, 64, 187583475);
	assert(streq(buf, "187583475"));
	showsi(buf, 64, 11834);
	assert(streq(buf, "11834"));
	showsi(buf, 64, -11834);
	assert(streq(buf, "-11834"));

	iterate_recurse(0, 8);

	return 0;
}
