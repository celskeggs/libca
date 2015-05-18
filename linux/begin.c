/*
    This file is part of libca/linux. See LICENSE.txt for details.

    libca/linux is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    libca/linux is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with libca/linux.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <system.h>
#include <panic.h>
#include <libsyscall.h>

static string program_reference;
static u32 argument_count;
static string *arguments;
static string *environment;

u32 get_argument_count(void) {
	return argument_count;
}
string get_argument(u32 id) {
	if (id >= argument_count) {
		panic_static("get_argument: attempt to get out-of-range argument");
	}
	return arguments[id];
}
string get_argument_opt(u32 id, string defarg) {
	return id < argument_count ? arguments[id] : defarg;
}
string *get_arguments(void) {
	return arguments;
}
string get_program_reference(void) {
	return program_reference;
}

LSC_MAIN(argc, argv, env) {
	if (argc < 1 || (argc & 0xFFFFFFFF) != argc) {
		panic_static("LSC_MAIN: invalid argc state on program entrance");
	}
	argument_count = argc - 1;
	program_reference = argv[0];
	arguments = ((string*) argv) + 1;
	environment = (string*) env;
	return camain(argument_count, arguments);
}
