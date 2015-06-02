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

#ifndef _MEMORY_H
#define _MEMORY_H

#include <catype.h>

void *memset(void *buf, u8 chr, ulen count);
// _c functions are check functions: they panic if constraints (like non-NULLity) aren't met. Also make sure that no more than the maximum amount is modified - also panic-ing.
void *memset_c(void *dest, ulen max, u8 chr, ulen count);
void *memcpy(void *restrict dst, const void *restrict src, ulen count);
void *memcpy_c(void *restrict dst, ulen max, const void *restrict src, ulen count);
void *memmove(void *restrict dst, const void *restrict src, ulen count);
void *memmove_c(void *restrict dst, ulen max, const void *restrict src, ulen count);
i16 memcmp(const void *lhs, const void *rhs, ulen count);
bool memeq(const void *lhs, const void *rhs, ulen count);
void *memchr(const void *ptr, u8 chr, ulen count);

#endif
