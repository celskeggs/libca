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
#include <assert.h>
#include <system.h>

void check(string str) {
	mutable_string mut = strdup(str);
	assert(streq(mut, str));
	assert(mut != str);
	free(mut);
}

CAMAIN0() {
	check("");
	check("hello world");
	check("what");
	check("THIS IS A TESTY TESTER TESTING");
	check("inline\0zero");
	check("_______________________________________________________________________________");
	char test[100000];
	memset(test, '\1', sizeof(test) - 1);
	test[sizeof(test) - 1] = '\0';
	assert(test[0] == '\1');
	check(test);
	return 0;
}
