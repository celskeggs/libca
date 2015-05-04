#ifdef NDEBUG
# error Asserts are required!
#endif
#include <assert.h>
#include <system.h>

CAMAIN0() {
	{ // u8
	u8 a = 0;
	assert(a == 0);
	a += 127;
	assert(a == 127);
	a++;
	assert(a == 128);
	a += 128;
	assert(a == 255);
	a++;
	assert(a == 0);
	}

	{ // i8
	i8 a = 0;
	assert(a == 0);
	a += 127;
	assert(a == 127);
	a++;
	assert(a == -128);
	a += 127;
	assert(a == -1);
	a++;
	assert(a == 0);
	}

	{ // u16
	u16 a = 0;
	assert(a == 0);
	a += 32767;
	assert(a == 32767);
	a++;
	assert(a == 32768);
	a += 32767;
	assert(a == 65535);
	a++;
	assert(a == 0);
	}

	{ // i16
	i16 a = 0;
	assert(a == 0);
	a += 32767;
	assert(a == 32767);
	a++;
	assert(a == -32768);
	a += 32767;
	assert(a == -1);
	a++;
	assert(a == 0);
	}

	{ // u32
	u32 a = 0;
	assert(a == 0);
	a += 2147483647;
	assert(a == 2147483647);
	a++;
	assert(a == 2147483648);
	a += 2147483647;
	assert(a == 4294967295);
	a++;
	assert(a == 0);
	}

	{ // i32
	i32 a = 0;
	assert(a == 0);
	a += 2147483647;
	assert(a == 2147483647);
	a++;
	assert(a == -2147483648);
	a += 2147483647;
	assert(a == -1);
	a++;
	assert(a == 0);
	}

	// TODO: this for u64, i64, uptr, iptr, uidx, iidx, ulen, ilen.

	return 0;
}
