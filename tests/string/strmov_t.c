/*
    This file is part of libca/tests. See LICENSE.txt for details.

    libca/tests is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    libca/tests is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with libca/tests.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <system.h>
#include <assert.h>
#include <string.h>

// temporary:
#include <stream.h>

/* Tests for strmov */

// mutable_string strmov(mutable_string restrict dest, ulen max, string restrict src);

CAMAIN0() {
	writes(stderr, "strmov_t incomplete.\n");
	return 0;
}
