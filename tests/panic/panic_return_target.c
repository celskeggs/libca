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
#include <panic.h>
#include <stream.h>
#include <string.h>

CAMAIN(argc, argv) {
	if (argc == 1) {
		if (streq(argv[0], "STATIC")) {
			panic_static("I have failed.");
		} else if (streq(argv[0], "DYNAMIC")) {
			panic("I have failed.");
		}
	}
	writes(stderr, "Failed to fail.");
	writes(stdout, "Failed to fail.");
	return 0; // not the expected result, so this is how we signal real failure
}
