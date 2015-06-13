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
#include <assert.h>
#include <alloc.h>
#include <string.h>

static void *received;

void receive(void *recv) {
	received = recv;
}

void fail_s(void *recv) {
	panic_static((string) recv);
}

void fail_d(void *recv) {
	panic((string) recv);
}

void use_recover_with_null_cb(void *unused) {
	error err;
	recover(NULL, NULL, &err);
}

void use_recover_with_null_err_triggered(void *unused) {
	recover(fail_s, "ignored failure", NULL);
}

void use_recover_with_null_err_untriggered(void *unused) {
	recover(receive, "ignored failure", NULL);
}

CAMAIN0() {
	error err;
	u8 x;

	// check reception of live pointer; check error reporting on success
	received = NULL;
	err.type = ERR_STATIC;
	assert(!recover(receive, &x, &err));
	assert(received == &x);
	assert(err.type == ERR_NONE);

	// check reception of NULL; check error reporting on success
	err.type = ERR_STATIC;
	assert(!recover(receive, NULL, &err));
	assert(received == NULL);
	assert(err.type == ERR_NONE);

	// check static panicing
	string pst = "pass-through string";
	assert(recover(fail_s, (void*) pst, &err));
	assert(err.type == ERR_STATIC);
	assert(err.str == pst); // must be EXACTLY THE SAME STRING

	// check static panicing with NULL pointer
	assert(recover(fail_s, NULL, &err));
	assert(err.type == ERR_STATIC);
	assert(streq(err.str, "paniced with NULL message"));

	// check dynamic panicing
	string ecs = "eventually-copied string";
	assert(recover(fail_d, (void*) ecs, &err));
	assert(err.type == ERR_MALLOCED);
	assert(err.str != ecs); // must not have simply reused the string
	assert(streq(err.str, ecs)); // must still be equal
	free((void*) err.str); // this might check to make sure that malloc was used.
	err.str = NULL;

	// check dynamic panicing with NULL pointer - should be converted to a static failure
	assert(recover(fail_d, NULL, &err));
	assert(err.type == ERR_STATIC);
	assert(streq(err.str, "paniced with NULL message"));

	// check edge cases of recover() itself
	assert(recover(use_recover_with_null_cb, NULL, &err));
	assert(err.type == ERR_STATIC);
	assert(streq(err.str, "recover() called with NULL callback"));
	assert(recover(use_recover_with_null_err_untriggered, NULL, &err));
	assert(err.type == ERR_STATIC);
	assert(streq(err.str, "recover() called with NULL error return target"));
	assert(recover(use_recover_with_null_err_triggered, NULL, &err));
	assert(err.type == ERR_STATIC);
	assert(streq(err.str, "recover() called with NULL error return target"));

	// exit() is checked as part of system tests

	return 0;
}
