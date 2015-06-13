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
#include <string.h>
#include <stream.h>
#include <alloc.h>

void qback(void *s) {
	systemq((string) s);
}

struct qbnp {
	string cmd;
	string *args;
	ulen n;
};

void qback_nosh(void *s) {
	struct qbnp *q = (struct qbnp*) s;
	systemq_nosh(q->cmd, q->args, q->n);
}

void check(string s, u8 exact) {
	error err;
	assert(recover(qback, (void*) s, &err));
	assert(err.type == ERR_MALLOCED);
	free((void*) err.str); // TODO: check actual string?

	assert(!system_check(s));
	u32 out = system(s);
	assert(!SYS_EXIT_NORMAL(out));
	assert(SYS_EXITED(out));
	assert(SYS_CODE(out) == exact);
}

void check_nosh(string s, string *args, ulen count, u8 exact) {
	error err;
	struct qbnp qb = {s, args, count};
	assert(recover(qback_nosh, &qb, &err));
	assert(err.type == ERR_MALLOCED);
	free((void*) err.str); // TODO: check actual string?

	assert(!system_check_nosh(s, args, count));
	u32 out = system_nosh(s, args, count);
	assert(!SYS_EXIT_NORMAL(out));
	assert(SYS_EXITED(out));
	assert(SYS_CODE(out) == exact);
}

CAMAIN(argc, argv) {
	assert(argc == 2);
	check("false", 1);
	check_nosh("/bin/false", NULL, 0, 1);
	check("echo CheckableString >/dev/full 2>/dev/null", 1);
	writes(stderr, "The following three errors from 'rm' are intentional.\n");
	check_nosh("/bin/rm", (string[]) {"/"}, 1, 1);
	for (u8 i = 1; i != 0; i++) {
		char cmd[64] = {"exit "};
		showui(cmd + 5, 64 - 5, i);
		check(cmd, i);
	}
	char temp[1024];
	strmov(temp, sizeof(temp) / sizeof(char), argv[1]); // TESTBUILDDIR
	strapnd(temp, sizeof(temp) / sizeof(char), "/system_failure_target");
	for (u8 i = 1; i != 0; i++) {
		char arg[64];
		showui(arg, 64, i);
		check_nosh(temp, (string[]) {"--retval", arg}, 2, i);
	}
	return 0;
}
