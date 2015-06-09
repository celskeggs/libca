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
#include <alloc.h>

/* Tests for memcpy, memcpy_c */

struct params {
	void *buf;
	ulen buflen;
	const void *from;
	ulen fromlen;
};

static void cb(void *pr) {
	struct params *p = (struct params *) pr;
	memcpy_c(p->buf, p->buflen, p->from, p->fromlen);
}

void check(u8 *buf, ulen buflen, const u8 *sample, ulen samplelen) {
	assert(buflen >= 2 + samplelen);
	u8 startbyte = samplelen ? sample[0] + 42 : 67;
	u8 midbyte = samplelen ? sample[0] + 31 : 46;
	u8 endbyte = samplelen ? sample[samplelen - 1] + 142 : 167;
	buf[0] = startbyte;
	buf[1] = midbyte;
	buf[samplelen + 1] = endbyte;

	assert(memcpy(buf + 1, sample, samplelen) == buf + 1);
	assert(buf[0] == startbyte);
	assert(buf[samplelen + 1] == endbyte);
	assert(memeq(buf + 1, sample, samplelen));

	for (ulen tbuflen = 0; tbuflen <= buflen - 2; tbuflen++) {
		buf[0] = startbyte;
		buf[1] = midbyte;
		buf[samplelen + 1] = endbyte;

		if (tbuflen < samplelen) {
			struct params p = {buf + 1, tbuflen, sample, samplelen};
			error err;
			assert(recover(cb, &p, &err));
			assert(samplelen == 0 || buf[1] == midbyte);
		} else {
			assert(memcpy_c(buf + 1, tbuflen, sample, samplelen) == buf + 1);
			assert(memeq(buf + 1, sample, samplelen));
		}

		assert(buf[0] == startbyte);
		assert(buf[samplelen + 1] == endbyte);
	}
}

void checkranges(u8 *buf) {
	u8 *interesting_cases[] = {buf, buf + 9, buf + 10, buf + 11, buf + 19, buf + 20, buf + 21, buf + 25, NULL};

	u8 *usedbuffer = buf + 10;
	ulen usedbufferlen = 10;

	u32 n = 0;

	for (u8 **start = interesting_cases; *start != NULL; start++) {
		for (u8 **end = start + 1; *end != NULL; end++) {
			n++;
			u8 *realsource = *start;
			u8 *realend = *end;
			ulen reallen = *end - realsource;
			if (realend <= usedbuffer || realsource >= usedbuffer + usedbufferlen) {
				// should not overlap, so assume no exception
				assert(memcpy_c(usedbuffer, usedbufferlen, realsource, reallen) == usedbuffer);
			} else {
				// should overlap, so assume exception
				struct params p = {usedbuffer, usedbufferlen, realsource, reallen};
				error err;
				assert(recover(cb, &p, &err));
			}
		}
	}
}

CAMAIN0() {
	u8 buf[1024];
	const u8 sourcedata[] = {225, 82, 132, 187, 51, 102, 12, 85, 86, 68, 83, 64, 7, 241, 224, 194, 108, 69, 170, 99, 92, 27, 222, 132, 135, 172, 170, 98, 135, 76, 110, 88, 224, 203, 116, 111, 113, 153, 180, 78, 231, 72, 179, 58, 176, 75, 241, 208, 128, 124, 207, 136, 53, 89, 149, 174, 193, 9, 139, 55, 232, 112, 237, 54, 50, 39, 2, 95, 227, 128, 238, 84, 131, 83, 105, 178, 195, 219, 186, 202, 184, 52, 131, 27, 156, 35, 208, 29, 36, 68, 110, 134, 3, 246, 250, 179, 210, 97, 166, 0};
	const u8 *end = sourcedata + sizeof(sourcedata) / sizeof(u8);
	for (const u8 *cur = sourcedata; cur < end; cur++) {
		for (const u8 *extent = cur; extent < end; extent++) {
			check(buf, sizeof(buf) / sizeof(u8), cur, extent - cur);
		}
	}

	// checking for memcpy_c checking for nullity
	struct params params = {NULL, 10, sourcedata, 16};
	error err;
	assert(recover(cb, &params, &err));
	params.from = NULL;
	assert(recover(cb, &params, &err));
	params.buf = buf;
	assert(recover(cb, &params, &err));

	// checking for memcpy_c checking the ranges
	checkranges(buf);

	// these are all slightly outdated because checkranges should check all of them, but still around because it's more testing.
	memcpy_c(buf, 10, buf, 0); // should be fine because nothing's actually copied
	params.buf = buf;
	params.buflen = 10;
	params.from = buf;
	params.fromlen = 1;
	assert(recover(cb, &params, &err));
	params.from = buf + 3;
	assert(recover(cb, &params, &err));
	params.from = buf + 9;
	assert(recover(cb, &params, &err)); // still overlapping
	memcpy_c(buf, 10, buf + 10, 1); // not quite overlapping

	params.fromlen = 10;
	assert(recover(cb, &params, &err)); // overlapping more widely

	memcpy_c(buf + 10, 10, buf, 1); // not overlapping at all
	memcpy_c(buf + 10, 10, buf, 10); // not quite overlapping
	params.buf = buf + 10;
	params.buflen = 10;
	params.from = buf;
	params.fromlen = 11; // slightly overlapping
	assert(recover(cb, &params, &err));
	params.fromlen = 30; // very much overlapping
	assert(recover(cb, &params, &err));

	return 0;
}
