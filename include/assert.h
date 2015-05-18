/*
 * Copyright (c) 2015 Colby Skeggs
 * This file is part of libca, and is licensed under the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _ASSERT_H
#define _ASSERT_H

#include <catype.h>
#include <panic.h>

#ifdef NDEBUG
#define assert(ignore) ((void) 0)
#else
#define _assert_fail(expr, file, line) panic_static(#file ":" #line ": assertion '" #expr "' failed."))
#define assert(expr) (expr ? ((void) 0) : _assert_fail(expr, __FILE__, __LINE__)
#endif

#endif
