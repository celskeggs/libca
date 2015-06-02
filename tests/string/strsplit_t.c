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
#include <memory.h>

/* Tests for strsplit, strsplit_free */

void check_splits_similar(strsplit_out *unc, strsplit_out *comp) {
	ulen j = 0;
	for (ulen i = 0; i < unc->count; i++) {
		if (*(unc->strings[i]) == 0) {
			continue;
		}
		assert(j < comp->count);
		assert(streq(unc->strings[i], comp->strings[j++]));
	}
	assert(j == comp->count);
}

void check_compressed(strsplit_out *comp) {
	for (ulen i = 0; i < comp->count; i++) {
		assert(*(comp->strings[i]) != 0); // strings must all be nonempty
	}
}

void check2(string str, string delim) {
	strsplit_out *out = strsplit(str, delim, false);

	strsplit_out *out2 = strsplit(str, delim, true);
	check_compressed(out2);
	check_splits_similar(out, out2);
	strsplit_free(out2);

	assert(out != NULL);
	assert(out->count > 0);
	assert(out->strings != NULL);

	char outs[strlen(str) + 2];
	outs[strlen(str) + 1] = '\2';
	strmov(outs, sizeof(outs) / sizeof(char), out->strings[0]);
	for (ulen i = 1; i < out->count; i++) {
		strapnd(outs, sizeof(outs) / sizeof(char), delim);
		strapnd(outs, sizeof(outs) / sizeof(char), out->strings[i]);
	}

	assert(outs[strlen(str)] == '\0');
	assert(outs[strlen(str) + 1] == '\2');
	assert(streq(str, outs));

	strsplit_free(out);
}

void check(string str) {
	for (string end = str + 1; *end; end++) {
		for (string start = str; start < end; start++) {
			char temp[end - start + 1];
			memcpy(temp, start, end - start);
			temp[end - start] = '\0';
			check2(str, temp);
		}
	}
}

CAMAIN0() {
	check("csv,line,test,example");
	check("tsv\tline\ttest\texample");
	check("racecar");
	check("palindromic-cimordnilap");
	check("delimiters delimiters delimiters delimiters");
	check("libca is free software: you can redistribute it and/or modify\n    it under the terms of the GNU Lesser General Public License as published by\n    the Free Software Foundation, either version 3 of the License, or\n    (at your option) any later version.");
	check("AAAAA BBBBB CCCCC DDDDD");
	check("ABCDE ABCDE ABCDE ABCDE");
	return 0;
}
