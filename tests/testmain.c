/*  This program is a set of unit tests for libca implementations.
    Copyright (C) 2015 Colby Skeggs

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

#include <caversion.h>
#include <system.h>
#include <construct.h>

#if LIBCA_MAJOR != 0 || LIBCA_MINOR != 1
# error This version of the test harness is designed for libca 0.1.*
#endif

CAMAIN(count, args) {
	stream manifest = openfq(get_argument_opt(0, "./manifest"), FILE_READ, FILE_TEXTUAL);
	char line[1024];
	u32 tests = 0;
	u32 successes = 0;
	while (readlnq(manifest, line, sizeof(line))) {
		if (line[0] == '\0') { continue; }
		tests++;
		if (system_check(line)) {
			successes++;
		} else {
			writesf(stdout, "Failed test: %s\n", line);
		}
	}
	writesf(stdout, "Test summary: passed %d of %d (%d%%)", successes, tests, (u32) (100 * successes / (float) tests));
	return tests != successes;
}
