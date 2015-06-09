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

#include <assert.h>
#include <system.h>
#include <string.h>

#define TOSTR_I(x) #x
#define TOSTR(x) TOSTR_I(x)

CAMAIN0() {
	assert(streq(TOSTR(LIBCA_MAJOR) "." TOSTR(LIBCA_MINOR) "." TOSTR(LIBCA_PATCH), LIBCA_VERSION));

	assert(LIBCA_MAJOR == 0); // not yet ready for release
	assert(LIBCA_MINOR == 1); // these tests are designed for this version

	return 0;
}
