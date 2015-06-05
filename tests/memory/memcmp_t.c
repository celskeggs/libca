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

/* Tests for memcmp, memeq */

void checkeqr(void *a, void *b, ulen x) {
	assert(memeq(a, b, x));
	assert(memcmp(a, b, x) == 0);
	if (x > 0) {
		checkeqr(a + 1, b + 1, x - 1);
		checkeqr(a, b, x - 1);
	}
}

void checkeq(void *a, void *b, ulen x) {
	u8 an[x], bn[x];
	memcpy(an, a, x);
	memcpy(bn, b, x);
	checkeqr(a, a, x);
	checkeqr(a, b, x);
	checkeqr(a, an, x);
	checkeqr(a, bn, x);
	checkeqr(b, a, x);
	checkeqr(b, b, x);
	checkeqr(b, an, x);
	checkeqr(b, bn, x);
	checkeqr(an, a, x);
	checkeqr(an, b, x);
	checkeqr(an, an, x);
	checkeqr(an, bn, x);
	checkeqr(bn, a, x);
	checkeqr(bn, b, x);
	checkeqr(bn, an, x);
	checkeqr(bn, bn, x);
}

void checklt(void *a, void *b, ulen x) {
	assert(!memeq(a, b, x));
	assert(memcmp(a, b, x) < 0);
	assert(memcmp(b, a, x) > 0);
	if (x > 0 && *(u8*)a == *(u8*)b) {
		checklt(a + 1, b + 1, x - 1);
	}
}

CAMAIN0() {
	checkeq("abc", "abc", 4);
	checkeq("abcaoeu", "abcasdf", 4);
	checklt("abcaoeu", "abcasdf", 5);
	void *chunks[] = {"\0\0\0\0", "\0\0\0\1", "\0\0\0C", "\0\0\0c", "!!xx", "!!xy", "!!yx", "ao\0c", "ao\0v", "aoeu", "aplm", "axdv", "mrrg", "z\6ZZ", "z\7\7\7", "zZZZ", NULL};
	for (void **cur = chunks; *cur != NULL; cur++) {
		for (void **other = cur + 1; *other != NULL; other++) {
			checklt(*cur, *other, 4);
		}
	}
	return 0;
}
