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
