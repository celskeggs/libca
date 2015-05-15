#include <string.h>
#include <memory.h>
#include <alloc.h>

ulen strlen(string str) {
	ulen out = 0;
	while (str[out]) {
		out++;
	}
	return out;
}
ulen strnlen(string str, ulen max) {
	ulen out = 0;
	while (out < max && str[out]) {
		out++;
	}
	return out;
}
mutable_string strdup(string str) {
	ulen len = strlen(str);
	mutable_string out = (mutable_string) malloc(len + 1);
	memcpy(out, str, len);
	return out;
}

/* TODO
mutable_string strmov(mutable_string restrict dest, ulen max, string restrict src);
mutable_string strapnd(mutable_string restrict dest, ulen max, string restrict src);
i16 strcmp(string lhs, string rhs);
bool streq(string lhs, string rhs);
string strchr(string str, u8 chr);
string strrchr(string str, u8 chr);
string strstr(string str, string substr);

typedef struct {
	ulen count;
	string *strings;
} strsplit_out;
strsplit_out *strsplit(string str, string delimiter, bool compress);
void strsplit_free(strsplit_out *struc);

// if remain is NULL, then will panic if invalid. otherwise, will be set to point to the first unconsumed character.
u8 parseu8(string data, string *remain, u8 base);
u16 parseu16(string data, string *remain, u8 base);
u32 parseu32(string data, string *remain, u8 base);
u64 parseu64(string data, string *remain, u8 base);
i8 parsei8(string data, string *remain, u8 base);
i16 parsei16(string data, string *remain, u8 base);
i32 parsei32(string data, string *remain, u8 base);
i64 parsei64(string data, string *remain, u8 base);
*/
