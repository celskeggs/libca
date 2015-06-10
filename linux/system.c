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
#include <panic.h>
#include <string.h>
#include <assert.h>
#include <libsyscall.h>

#define SYSTEMQ_PREFIX "command failed: "

u32 system_nosh(string command, string *args, ulen count) {
	assert(command != NULL);
	string nargs[count + 2];
	nargs[0] = command;
	for (ulen i = 0; i < count; i++) {
		assert(args[i] != NULL);
		nargs[i + 1] = args[i];
	}
	nargs[count + 1] = NULL;
	u64 pid = lsc_fork();
	if (pid == 0) { // child
		lsc_execve(command, nargs, get_environment());
		lsc_exit(127);
	}
	u32 status;
	i64 out;
restart:
	out = lsc_wait4(pid, &status, 0, NULL);
	if (out < 0) {
		if (out == -4) { // EINTR
			goto restart;
		}
		return 0x200;
	}
	if ((status & 0x7F) == 0) {
		return 0x100 | ((status & 0xff00) >> 8);
	} else if ((status & 0x7F) != 0x7F) {
		return (status & 0x7F);
	} else { // indicates WIFCONTINUED
		return 0x200; // TODO: handle this better?
	}
}
u32 system(string command) {
	assert(command != NULL);
	return system_nosh("/bin/sh", (string[]) {"-c", command}, 2);
}

bool system_check_nosh(string command, string *args, ulen count) {
	return SYS_EXIT_NORMAL(system_nosh(command, args, count));
}
bool system_check(string command) {
	return SYS_EXIT_NORMAL(system(command));
}

void systemq_nosh(string command, string *args, ulen count) {
	if (!system_check_nosh(command, args, count)) {
		char built[strlen(command) + 1 + strlen(SYSTEMQ_PREFIX)];
		strmov(built, sizeof(built) / sizeof(char), SYSTEMQ_PREFIX);
		strapnd(built, sizeof(built) / sizeof(char), command);
		panic(built);
	}
}
void systemq(string command) {
	if (!system_check(command)) {
		char built[strlen(command) + 1 + strlen(SYSTEMQ_PREFIX)];
		strmov(built, sizeof(built) / sizeof(char), SYSTEMQ_PREFIX);
		strapnd(built, sizeof(built) / sizeof(char), command);
		panic(built);
	}
}
