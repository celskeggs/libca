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
#include <string.h>
#include <assert.h>

CAMAIN(argc, argv) {
	assert(argc == 5);
	string strs[] = {"Parameter 4", "Parameter\1With Embedded Binary", "Parameter With\nEmbedded Newline", "", "Fifth"};
	for (ulen i = 0; i < 5; i++) {
		assert(streq(argv[i], strs[i]));
	}
	return 0;
}
