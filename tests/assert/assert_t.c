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
#include <panic.h>
#include <string.h>

u32 line_no = 0;
bool passed = false;

void assertion_test(void *d) {
	line_no = __LINE__ + 1;
	assert(d != NULL); // allow caller to decide between failing and succeeding
	passed = true;
}

CAMAIN0() {
	error err;

	// assertion success
	passed = false;
	assert(!recover(assertion_test, &line_no, &err));
	assert(passed);

	// assertion failure
	passed = false;
	assert(recover(assertion_test, NULL, &err));
	assert(!passed);
	assert(err.type == ERR_STATIC);
	char expected[1024] = {__FILE__ ":"};
	mutable_string target = expected + strlen(expected);
	showui(target, expected + (sizeof(expected) / sizeof(char)) - target, line_no);
	strapnd(expected, sizeof(expected) / sizeof(char), ": assertion 'd != NULL' failed.");
	assert(streq(expected, err.str));

	return 0;
}
