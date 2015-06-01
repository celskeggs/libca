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
#include <alloc.h>

/* Tests for strchr, strrchr, strstr */

bool has(string s, u8 c) {
	while (*s) {
		if (*s++ == c) {
			return true;
		}
	}
	return false;
}

void checkstring(string s) {
	assert(strstr(s, "") == s);
	for (u8 c = 32; c <= 126; c++) {
		if (has(s, c)) {
			string s2 = strchr(s, c);
			string s3 = strrchr(s, c);
			assert(s2 != NULL && *s2 == c);
			assert(s3 != NULL && *s3 == c);
			assert(s3 >= s2);
			string si;
			for (si = s; si < s2; si++) {
				assert(*si != c && *si != 0);
			}
			for (; si <= s3; si++) {
				assert(*si != 0);
			}
			for (; *si; si++) {
				assert(*si != c);
			}
		} else {
			assert(strchr(s, c) == NULL);
			assert(strrchr(s, c) == NULL);
		}
	}
	if (*s && *(s+1) && *(s+2)) {
		for (u8 c1 = 32; c1 <= 126; c1++) {
			for (u8 c2 = 32; c2 <= 126; c2++) {
				for (u8 c3 = 32; c3 <= 126; c3++) {
					char ss[] = {c1, c2, c3, 0};
					string found = strstr(s, ss);
					if (found == NULL) {
						for (string si = s + 2; *si; si++) {
							assert(si[-2] != c1 || si[-1] != c2 || si[0] != c3);
						}
					} else {
						for (string si = s; si < found; si++) {
							assert(*si != 0);
						}
						for (string si = s + 2; si < found; si++) {
							assert(si[-2] != c1 || si[-1] != c2 || si[0] != c3);
						}
						assert(found[0] == c1 && found[1] == c2 && found[2] == c3);
					}
				}
			}
		}
	}
}

CAMAIN0() {
	checkstring("");
	checkstring("mixed");
	checkstring("awaaawawawaoaethuaeou");
	checkstring("gibberish IS fun, it is.");
	checkstring("CHICKEN CHICKEN CHICKEN");
	checkstring("any sentence here is eternal");
	checkstring("libca is still in development as of today");
	return 0;
}
