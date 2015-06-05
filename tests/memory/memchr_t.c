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
#include <memory.h>
#include <alloc.h>

/* Tests for memchr */

void check(const u8 *data, u8 v, ulen c) {
	const u8 *found = memchr(data, v, c);
	if (found == NULL) {
		for (ulen i = 0; i < c; i++) {
			assert(data[i] != v);
		}
	} else {
		assert(*found == v);
		assert(found >= data && found < data + c);
		for (const u8 *cur = data; cur < found; cur++) {
			assert(*cur != v);
		}
		check(found + 1, v, data + c - found);
	}
}

CAMAIN0() {
	u8 data[4096];
	for (ulen i = 0; i < 4096; i++) {
		data[i] = (i & 255) ^ (i >> 8);
	}
	for (u8 i = 0; i < 255; i++) {
		check(data, i, 4096);
	}
	check(data, 255, 4096);
	return 0;
}
