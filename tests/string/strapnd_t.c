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

/* Tests for strapnd */

// mutable_string strapnd(mutable_string restrict dest, ulen max, string restrict src);

string strings[] = {"", "hello", "beardie weirdie", "embedded\0null", "embedded\1\nonnull", "embedded\nnewline", "__  __  ", "\1", NULL};

void check_t(mutable_string buffer, ulen size, string a, string b) {
	strmov(buffer, size, a);
	strapnd_t(buffer, size, b);
	assert(strneq(buffer, a, strlen(a)));
	if (size - strlen(a) > strlen(b)) { // not truncated
		assert(streq(buffer + strlen(a), b));
		assert(buffer[strlen(a) + strlen(b)] == '\0');
	} else { // truncated
		assert(!streq(buffer + strlen(a), b));
		assert(buffer[size - 1] == '\0');
	}
	assert(strneq(buffer + strlen(a), b, size - strlen(a) - 1));
}

struct params {
	mutable_string buffer;
	ulen size;
	string source;
};

void doappend(void *paramr) {
	struct params *params = (struct params *) paramr;
	strapnd(params->buffer, params->size, params->source);
}

void check(mutable_string buffer, ulen size, string a, string b) {
	strmov(buffer, size, a);
	if (size - strlen(a) > strlen(b)) { // not truncated
		strapnd(buffer, size, b); // this fails! FIRST TRY!!!
		assert(strneq(buffer, a, strlen(a)));
		assert(streq(buffer + strlen(a), b));
		assert(buffer[strlen(a) + strlen(b)] == '\0');
		assert(strneq(buffer + strlen(a), b, size - strlen(a)));
	} else { // truncated
		error err;
		struct params params = {buffer, size, b};
		assert(recover(doappend, &params, &err));
	}
}

void check_zeroes() {
	char test[] = "hi\0\7\0";

	strapnd_t(test, 0, "hello");
	assert(streq(test, "hi"));
	assert(test[3] == 7);
	assert(test[4] == 0);

	strapnd_t(test, 0, "");
	assert(streq(test, "hi"));
	assert(test[3] == 7);
	assert(test[4] == 0);

	struct params params = {test, 0, "hello"};
	error err;
	assert(recover(doappend, &params, &err));
	assert(streq(test, "hi"));
	assert(test[3] == 7);
	assert(test[4] == 0);

	params.source = "";
	assert(recover(doappend, &params, &err));
	assert(streq(test, "hi"));
	assert(test[3] == 7);
	assert(test[4] == 0);
}

void check_initial_overflow() {
	char test[] = "helloworld";
	struct params params = {test, 10, ""}; // test is completely filled: no null terminator in the size given
	error err;
	assert(recover(doappend, &params, &err)); // should fail: a completely-filled input buffer is invalid
	params.source = "hello";
	assert(recover(doappend, &params, &err)); // should fail regardless of the length of the source string.
}

CAMAIN0() {
	check_zeroes();

	check_initial_overflow();

	for (string *alpha = strings; *alpha != NULL; alpha++) {
		for (string *beta = strings; *beta != NULL; beta++) {
			string a = *alpha, b = *beta;
			ulen sla = strlen(a), slb = strlen(b);
			for (ulen n = sla + slb / 2 + 2; n < sla + slb + 15; n++) {
				char bufa[n];
				bufa[n - 1] = '\7';
				check(bufa, n - 1, a, b);
				check_t(bufa, n - 1, a, b);
				assert(bufa[n - 1] == '\7');
			}
		}
	}
	return 0;
}
