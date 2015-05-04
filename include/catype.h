#ifndef _CATYPE_H
#define _CATYPE_H

#include <caversion.h>

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <limits.h>

#if CHAR_BITS != 8
# error libca expects 8-bit characters!
#endif

#ifndef NULL
# define NULL ((void*) 0)
#endif

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef uintptr_t uptr;
typedef uintptr_t uidx; // array index
typedef uintptr_t ulen; // array length

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef intptr_t iptr;
typedef intptr_t iidx; // array index
typedef intptr_t ilen; // array length

typedef const char *string;
typedef char *mutable_string;

#endif
