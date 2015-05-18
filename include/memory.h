/*
 * Copyright (c) 2015 Colby Skeggs
 * This file is part of libca, and is licensed under the MIT license.
 * See LICENSE.txt for details.
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
void *memcmp(const void *lhs, const void *rhs, ulen count);
void *memchr(const void *ptr, u8 chr, ulen count);

#endif
