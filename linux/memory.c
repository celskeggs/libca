#include <memory.h>

void *memset(void *buf, u8 chr, ulen count) {
	u8 *u8b = (u8*) buf;
	// TODO: what if count is UINT_MAX?
	for (ulen i = 0; i < count; i++) {
		u8b[i] = chr;
	}
	return buf;
}
void *memcpy(void *restrict dst, const void *restrict src, ulen count) {
	const u8 *u8s = (const u8*) src;
	u8 *u8d = (u8*) dst;
	// TODO: what if count is UINT_MAX?
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
