/*
 * Copyright (c) 2015 Colby Skeggs
 * This file is part of libca, and is licensed under the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _CLOCK_H
#define _CLOCK_H

#include <catype.h>

enum clock_type {
	CL_PROGRAM,
	CL_UNIX,
	CL_MONOTONIC,
	CL_SYSTEM // for example, /proc/uptime
};

struct clock_result {
	i64 seconds;
	u32 nanos;
};

// panics if the clock is unavailable
struct clock_result get_clock(enum clock_type type);

struct clock_struct {
	i8 sec, min, hour, day, month;
	i32 year;
};

void normalize_clock_struct(struct clock_struct *cstr);
i64 make_clock(struct clock_struct cstr); // TODO: utc versus local

#define DEFAULT_CLOCK_FORMAT "%c"

void format_clock_struct(string format, mutable_string out, ulen max, struct clock_struct cstr);
void format_clock(string format, mutable_string out, ulen max, i64 clock);
struct clock_struct utc_clock(i64 clock);
struct clock_struct local_clock(i64 clock);

#endif
