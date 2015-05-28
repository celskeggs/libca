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

/* Tests for strmov */

// mutable_string strmov(mutable_string restrict dest, ulen max, string restrict src);

void check_t(ulen count, string in) {
	char out[count + 1]; // +1 so that we can check that it does not get touched.

	if (strlen(in) < count) {
		out[strlen(in)] = '\1';
		out[strlen(in) + 1] = '\2';
		out[count] = '\3';
	} else {
		out[count] = '\3';
	}
	assert(out[count] == '\3');
	strmov_t(out, count, in);
	assert(out[count] == '\3');
	if (strlen(in) < count) {
		assert(out[strlen(in)] == 0);
		assert(out[strlen(in) + 1] == '\2' || strlen(in) + 1 == count);
		assert(streq(in, out));
		assert(strcmp(in, out) == 0);
	} else {
		assert(out[count - 1] == '\0');
	}
	assert(strneq(in, out, count - 1));
	assert(strncmp(in, out, count - 1) == 0);
}

struct check_param {
	ulen count;
	string in;
	mutable_string out;
};

void check_s(void *paramr) {
	struct check_param *params = (struct check_param *) paramr;
	strmov(params->out, params->count, params->in); // this line should panic!
	// went fine... not good.
}

void check_zero() {
	char raw;
	for (u16 i=0; i<256; i++) {
		raw = i;
		strmov_t(&raw, 0, "this is a long string");
		assert(raw == (char) i);
		strmov_t(&raw, 0, "");
		assert(raw == (char) i);
	}
	for (u16 i=0; i<256; i++) {
		raw = i;
		struct check_param params = { 0, "another string", &raw };
		error err; // TODO: should this be ignored?
		assert(recover(check_s, &params, &err));
		assert(raw == (char) i);
		params.in = "";
		assert(recover(check_s, &params, &err));
		assert(raw == (char) i);
	}
}

void check_p(ulen count, string in) {
	char out[count + 1]; // +1 so that we can check that it does not get touched.
	if (strlen(in) < count) {
		out[strlen(in)] = '\1';
		out[strlen(in) + 1] = '\2';
		out[count] = '\3';
		strmov(out, count, in);
		assert(out[strlen(in)] == 0);
		assert(out[strlen(in) + 1] == '\2' || strlen(in) + 1 == count);
		assert(out[count] == '\3');
		assert(streq(in, out));
		assert(strcmp(in, out) == 0);
	} else {
		struct check_param params = { count, in, out };
		error err; // TODO: should this be ignored?
		assert(recover(check_s, &params, &err));
	}
}

void checkall(string in) {
	for (ulen i = 1; i < 1000; i++) {
		check_t(i, in);
		check_p(i, in);
	}
}

CAMAIN0() {
	check_zero();
	checkall("");
	checkall("hello world");
	checkall("whatever...");
	checkall("nope nope\0halfway through");
	checkall("abc\1\2\3\4\5\6\7hahahahah");
	return 0;
}
