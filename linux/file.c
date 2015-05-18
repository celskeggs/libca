/*
    This file is part of libca/linux. See LICENSE.txt for details.

    libca/linux is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    libca/linux is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with libca/linux.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <assert.h>
#include <file.h>
#include <stream.h>
#include <libsyscall.h>

static void _init_filedef() __attribute__((constructor));

static stream_def fdfile_write, fdfile_read;

#define FD(x) (*STREAM_DATA(x, u32))

static ioerr locals[] = {
/*   0 */ IE_NONE, IE_DENIED, IE_NOTFOUND, IE_NOTFOUND,
/*   4 */ IE_UNKNOWN, IE_FAULT, IE_NOTFOUND, IE_OUTOFRANGE,
/*   8 */ IE_UNKNOWN, IE_INVALID, IE_UNKNOWN, IE_UNKNOWN,
/*  12 */ IE_FAULT, IE_DENIED, IE_INVALID, IE_INVALID,
/*  16 */ IE_BUSY, IE_INVALID, IE_INVALID, IE_NOTFOUND,
/*  20 */ IE_INVALID, IE_INVALID, IE_INVALID, IE_FAULT,
/*  24 */ IE_FAULT, IE_INVALID, IE_BUSY, IE_OUTOFRANGE,
/*  28 */ IE_FULL, IE_INVALID, IE_DENIED, IE_FAULT,
/*  32 */ IE_INVALID, IE_INVALID, IE_INVALID, IE_FAULT,
/*  36 */ IE_OUTOFRANGE, IE_FAULT, IE_UNSUPPORTED, IE_INVALID,
/*  40 */ IE_FAULT, IE_UNKNOWN, IE_INVALID, IE_FAULT,
/*  44 */ IE_OUTOFRANGE, IE_UNKNOWN, IE_UNKNOWN, IE_UNKNOWN,
/*  48 */ IE_OUTOFRANGE, IE_FAULT, IE_FAULT, IE_UNKNOWN,
/*  52 */ IE_INVALID, IE_INVALID, IE_FULL, IE_FAULT,
/*  56 */ IE_INVALID, IE_INVALID, IE_FAULT, IE_INVALID,
/*  60 */ IE_INVALID, IE_UNKNOWN, IE_UNKNOWN, IE_FULL,
/*  64 */ IE_NOTFOUND, IE_NOTFOUND, IE_INVALID, IE_FAULT,
/*  68 */ IE_FAULT, IE_FAULT, IE_FAULT, IE_FAULT,
/*  72 */ IE_INVALID, IE_UNKNOWN, IE_INVALID, IE_OUTOFRANGE,
/*  76 */ IE_INVALID, IE_FAULT, IE_UNKNOWN, IE_FAULT,
/*  80 */ IE_FAULT, IE_FAULT, IE_INVALID, IE_INVALID,
/*  84 */ IE_INVALID, IE_UNKNOWN, IE_FAULT, IE_BUSY,
/*  88 */ IE_INVALID, IE_INVALID, IE_OUTOFRANGE, IE_INVALID,
/*  92 */ IE_UNSUPPORTED, IE_UNSUPPORTED, IE_UNSUPPORTED, IE_UNSUPPORTED,
/*  96 */ IE_UNSUPPORTED, IE_UNSUPPORTED, IE_INVALID, IE_FAULT,
/* 100 */ IE_FAULT, IE_FAULT, IE_FAULT, IE_FAULT,
/* 104 */ IE_FAULT, IE_UNKNOWN, IE_INVALID, IE_INVALID,
/* 108 */ IE_INVALID, IE_INVALID, IE_NOTFOUND, IE_NOTFOUND,
/* 112 */ IE_NOTFOUND, IE_NOTFOUND, IE_BUSY, IE_UNKNOWN,
/* 116 */ IE_UNKNOWN, IE_UNKNOWN, IE_INVALID, IE_FULL,
/* 120 */ IE_UNKNOWN, IE_FAULT, IE_FULL, IE_NOTFOUND,
/* 124 */ IE_UNKNOWN, IE_UNKNOWN, IE_UNKNOWN, IE_UNKNOWN,
/* 128 */ IE_UNKNOWN, IE_UNKNOWN, IE_FAULT, IE_FAULT
};

static ioerr complete_error(u16 err) {
	return _IE_MAKE_SYS((err < (sizeof(locals) / sizeof(ioerr))) ? locals[err] : IE_UNKNOWN, err);
}

static ioerr fdwrite(stream c, const u8 *data, ulen count) {
	lsc_int_t result;
restart:
	result = lsc_write(FD(c), data, count);
	if (result == -4) { // interrupted
		goto restart;
	}
	if (result < 0) { // error
		return complete_error(-result);
	} else if (result == count) {
		return IE_NONE;
	}
	assert(result < count);
	data += result;
	count -= result;
	goto restart; // try to write the rest of it
}

static ioerr fdtell(stream c, i64 *out) {
	lsc_int_t result = lsc_lseek(FD(c), 0, SEEK_CUR);
	if (result < 0) {
		return complete_error(-result);
	}
	*out = result;
	return IE_NONE;
}

static ioerr fdseekset(stream c, i64 where) {
	lsc_int_t result = lsc_lseek(FD(c), where, SEEK_SET);
	return complete_error(-result);
}

static ioerr fdseekrel(stream c, i64 where) {
	lsc_int_t result = lsc_lseek(FD(c), where, SEEK_CUR);
	return complete_error(-result);
}

static ioerr fdseekend(stream c, i64 where) {
	lsc_int_t result = lsc_lseek(FD(c), where, SEEK_END);
	return complete_error(-result);
}

static ioerr fdflush(stream c, i64 where) {
	lsc_int_t result = lsc_fsync(FD(c));
	return complete_error(-result);
}

static void _init_filedef() {
	fdfile_write.write = fdwrite;
	fdfile_write.read = NULL;
	fdfile_write.at_eof = NULL;
	fdfile_write.p_tell = fdtell;
	fdfile_write.p_seek_set = fdseekset;
	fdfile_write.p_seek_rel = fdseekrel;
	fdfile_write.p_seek_end = fdseekend;
	fdfile_write.check = NULL;
	fdfile_write.flush = fdflush;
	fdfile_write.close = fdclose;

	fdfile_read.write = NULL;
	fdfile_read.read = fdread;
	fdfile_read.at_eof = fdateof;
	fdfile_read.p_tell = fdtell;
	fdfile_read.p_seek_set = fdseekset;
	fdfile_read.p_seek_rel = fdseekrel;
	fdfile_read.p_seek_end = fdseekend;
	fdfile_read.check = NULL;
	fdfile_read.flush = NULL;
	fdfile_read.close = fdclose;
}

stream _libca_openfd(u32 fd, bool out) {
	stream str = defstream(out ? &fdfile_write : &fdfile_read, sizeof(fd_t));
	FD(str) = fd;
	return str;
}

/* TODO
ioerr openf(stream *c, string filename, bool out);
stream openfq(string filename, bool out); // panics on error

ioerr openf_unbuf(stream *c, string filename, bool out);
stream openfq_unbuf(string filename, bool out); // panics on error

ioerr removef(string filename);
void removefq(string filename); // panics on error
ioerr renamef(string from, string to);
void renamefq(string from, string to); // panics on error

*/
