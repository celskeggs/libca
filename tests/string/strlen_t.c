/*
    This file is part of libca/tests. See LICENSE.txt for details.

    libca/tests is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    libca/tests is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with libca/tests.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <system.h>
#include <assert.h>
#include <string.h>

/* Tests for strlen, strnlen */

void check(string str, ulen len) {
	ulen real_len = strlen(str);
	assert(real_len == len);
	for (ulen i=0; i < 1000; i++) {
		ulen short_len = strnlen(str, i);
		if (real_len <= i) {
			assert(short_len == real_len);
		} else {
			assert(short_len == i);
		}
	}
	if (len != 0) {
		assert(strnlen(str, real_len - 1) == real_len - 1);
	}
	assert(strnlen(str, real_len) == real_len);
	assert(strnlen(str, real_len + 1) == real_len);
	assert(strnlen(str, real_len + 1000) == real_len);
}

CAMAIN0() {
	check("", 0);
	check("four", 4);
	check("Hello, World!", 13);
	check("Null\0Embedded", 4);
	check("Non-null\1Embedded", 17);
	check("Newline\nEmbedded", 16);
	check("Not-quite-null\\0...", 19);
	check("\1\1\1", 3);
	check("\0hello\0", 0);
	check("0123456789", 10);
	return 0;
}
