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
#include <memory.h>

/* Tests for memcmp, memeq */

struct params {
	void *data;
	ulen max;
	u8 v;
	ulen n;
};

void cb(void *pv) {
	struct params *p = (struct params *) pv;
	memset_c(p->data, p->max, p->v, p->n);
}

void check(u8 *data, u8 v, ulen n) { // data is guaranteed to be at least n + 16 wide
	data[1] = v + 16;
	data[n] = v + 15;

	data[0] = v + 17;
	data[n + 1] = v + 14;

	assert(memset(data + 1, v, n) == data + 1);

	assert(data[0] == (u8) (v + 17));
	assert(data[n + 1] == (u8) (v + 14));

	for (ulen i = 1; i < n + 1; i++) { // make sure that everything got changed
		assert(data[i] == v);
	}

	for (ulen k = 0; k < n + 16; k++) {
		assert(memset(data + 1, v + 15, n) == data + 1); // clear body so that we can redo it
		if (k < n) {
			struct params p = {data + 1, k, v, n};
			error e;
			assert(recover(cb, &p, &e));
			for (ulen i = 1; i < n + 1; i++) { // make sure that nothing got touched
				assert(data[i] == (u8) (v + 15));
			}
		} else {
			assert(memset_c(data + 1, k, v, n) == data + 1);
			for (ulen i = 1; i < n + 1; i++) { // make sure that everything got changed
				assert(data[i] == v);
			}
		}
		assert(data[0] == (u8) (v + 17));
		assert(data[n + 1] == (u8) (v + 14));
	}
}

CAMAIN0() {
	u8 test[2];
	assert(memset(test, 7, 2));
	assert(test[0] == 7 && test[1] == 7);
	assert(memset_c(test, 2, 8, 2));
	assert(test[0] == 8 && test[1] == 8);
	struct params p = {test, 2, 9, 2};
	error err;
	assert(!recover(cb, &p, &err));
	assert(test[0] == 9 && test[1] == 9);
	p.v = 10;
	p.data = NULL;
	assert(recover(cb, &p, &err));
	assert(test[0] == 9 && test[1] == 9);

	u8 data[200 + 16]; // needs to be at least max(n) + 16
	for (u8 i = 0; i < 255; i++) {
		for (ulen n = 0; n < 200; n++) {
			check(data, i, n);
		}
	}
	for (ulen n = 0; n < 200; n++) {
		check(data, 255, n);
	}
	return 0;
}
