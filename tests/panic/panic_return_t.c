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
#include <string.h>
#include <stream.h>

#define BUFSIZ 1024

CAMAIN(count, args) { // args: { srcdir, builddir }
	if (count < 2) {
		writes(stderr, "bad arguments to test!\n");
		return 1;
	}

	// with static
	char temp[BUFSIZ];
	strmov(temp, BUFSIZ, args[1]);
	strapnd(temp, BUFSIZ, "/panic_return_target STATIC 2>");
	strapnd(temp, BUFSIZ, args[1]);
	strapnd(temp, BUFSIZ, "/panic_return_output");
	if (system_check(temp)) {
		writes(stderr, "panic doesn't work!\n");
		return 2;
	}
	strmov(temp, BUFSIZ, "cmp ");
	strapnd(temp, BUFSIZ, args[0]);
	strapnd(temp, BUFSIZ, "/panic_return_expected ");
	strapnd(temp, BUFSIZ, args[1]);
	strapnd(temp, BUFSIZ, "/panic_return_output");
	if (!system_check(temp)) {
		writes(stderr, "panic printed the wrong output!\n");
		return 3;
	}

	// without static
	strmov(temp, BUFSIZ, args[1]);
	strapnd(temp, BUFSIZ, "/panic_return_target DYNAMIC 2>");
	strapnd(temp, BUFSIZ, args[1]);
	strapnd(temp, BUFSIZ, "/panic_return_output");
	if (system_check(temp)) {
		writes(stderr, "panic doesn't work!\n");
		return 2;
	}
	strmov(temp, BUFSIZ, "cmp ");
	strapnd(temp, BUFSIZ, args[0]);
	strapnd(temp, BUFSIZ, "/panic_return_expected ");
	strapnd(temp, BUFSIZ, args[1]);
	strapnd(temp, BUFSIZ, "/panic_return_output");
	if (!system_check(temp)) {
		writes(stderr, "panic printed the wrong output!\n");
		return 3;
	}

	return 0;
}
