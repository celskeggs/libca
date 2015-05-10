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
