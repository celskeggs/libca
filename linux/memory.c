/*
    This file is part of libca/linux. See LICENSE.txt for details.

    libca/linux is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    libca/linux is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with libca/linux.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <memory.h>

void *memset(void *buf, u8 chr, ulen count) {
	u8 *u8b = (u8*) buf;
	for (ulen i = 0; i < count; i++) {
		u8b[i] = chr;
	}
	return buf;
}
void *memcpy(void *restrict dst, const void *restrict src, ulen count) {
	const u8 *u8s = (const u8*) src;
	u8 *u8d = (u8*) dst;
	for (ulen i = 0; i < count; i++) {
		u8d[i] = u8s[i];
	}
	return u8d;
}

/* TODO
// _c functions are check functions: they panic if constraints (like non-NULLity) aren't met. Also make sure that no more than the maximum amount is modified - also panic-ing.
void *memset_c(void *dest, ulen max, u8 chr, ulen count);
void *memcpy_c(void *restrict dst, ulen max, const void *restrict src, ulen count);
void *memmove(void *restrict dst, const void *restrict src, ulen count);
void *memmove_c(void *restrict dst, ulen max, const void *restrict src, ulen count);
void *memcmp(const void *lhs, const void *rhs, ulen count);
void *memchr(const void *ptr, u8 chr, ulen count);
*/
