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

void check(string s) {
	systemq(s);
	assert(system_check(s));
	u32 out = system(s);
	assert(SYS_EXIT_NORMAL(out));
	assert(SYS_EXITED(out));
	assert(SYS_CODE(out) == 0);
}

void check_nosh(string s, string *args, ulen count) {
	systemq_nosh(s, args, count);
	assert(system_check_nosh(s, args, count));
	u32 out = system_nosh(s, args, count);
	assert(SYS_EXIT_NORMAL(out));
	assert(SYS_EXITED(out));
	assert(SYS_CODE(out) == 0);
}

CAMAIN(argc, argv) {
	assert(argc == 2);
	check("true");
	check_nosh("/bin/true", NULL, 0);
	check("echo CheckableString");
	check("echo CheckableString | cmp - ./system_success_expected");
	check_nosh("/bin/echo", (string[]) {"CheckableString"}, 1);
	check("exit 0");
	char temp[1024];
	strmov(temp, sizeof(temp) / sizeof(char), argv[1]); // TESTBUILDDIR
	strapnd(temp, sizeof(temp) / sizeof(char), "/system_success_target");
	check_nosh(temp, (string[]) {"Parameter 4", "Parameter\1With Embedded Binary", "Parameter With\nEmbedded Newline", "", "Fifth"}, 5);
	return 0;
}
