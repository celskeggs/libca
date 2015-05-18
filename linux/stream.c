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

#include <stream.h>
#include <assert.h>
#include <alloc.h>
#include <string.h>
#include <memory.h>
#include <file.h>

#define MIN(a,b) (a<=b)?a:b

stream defstream(stream_def *def, ulen datasize) {
	struct _stream *out = (struct _stream *) malloc(sizeof(struct _stream) + datasize);
	out->def = def;
	out->err = IE_NONE;
	return out;
}

struct string_stream_s {
	string str;
	string cur;
	string end;
};

static ioerr strin_read(stream c, u8* data, ulen* length) {
	struct string_stream_s *s = STREAM_DATA(c, struct string_stream_s);
	string cur = s->cur;
	*length = MIN(s->end - s->str, *length);
	memcpy(data, cur, *length);
	s->cur = cur + *length;
	return IE_NONE;
}
static bool strin_ateof(stream c) {
	return STREAM_DATA(c, struct string_stream_s)->cur[0] == '\0';
}
static ioerr strin_tell(stream c, u64* out) {
	struct string_stream_s *s = STREAM_DATA(c, struct string_stream_s);
	*out = s->cur - s->str;
	return IE_NONE;
}
static ioerr strin_seek_set(stream c, u64 loc) {
	struct string_stream_s *s = STREAM_DATA(c, struct string_stream_s);
	string nstr = s->str + loc;
	if (nstr < s->str || nstr > s->end) {
		return IE_OUTOFRANGE;
	}
	s->cur = nstr;
	return IE_NONE;
}
static ioerr strin_seek_rel(stream c, i64 loc) {
	struct string_stream_s *s = STREAM_DATA(c, struct string_stream_s);
	string nstr = s->cur + loc;
	if (nstr < s->str || nstr > s->end) {
		return IE_OUTOFRANGE;
	}
	s->cur = nstr;
	return IE_NONE;
}
static ioerr strin_seek_end(stream c, u64 loc) {
	struct string_stream_s *s = STREAM_DATA(c, struct string_stream_s);
	string nstr = s->end - loc;
	if (nstr < s->str || nstr > s->end) {
		return IE_OUTOFRANGE;
	}
	s->cur = nstr;
	return IE_OUTOFRANGE;
}

static stream_def string_stream;

static void _init_string_streams() {
	string_stream = (stream_def) { .write=NULL, .read=&strin_read, .at_eof=&strin_ateof, .p_tell=&strin_tell, .p_seek_set=&strin_seek_set, .p_seek_rel=&strin_seek_rel, .p_seek_end=&strin_seek_end, .check=NULL, .flush=NULL, .close=NULL };
}

stream openstrin(string str) {
	stream out = defstream(&string_stream, sizeof(struct string_stream_s));
	STREAM_DATA(out, struct string_stream_s)->str = str;
	STREAM_DATA(out, struct string_stream_s)->cur = str;
	STREAM_DATA(out, struct string_stream_s)->end = str + strlen(str);
	return out;
}

/* TODO
ioerr openbufin(stream *c, u8 *bytes, ulen count);
ioerr openbufout(stream *c, u8 *bytes, ulen max, ulen *count);
ioerr bufferstream(stream base, stream *nstr, bool flush_on_newline, ulen bufsize);
*/

#define CHECKERR(x,r) { if (c->err != IE_NONE) { return r; } if (c->def->x == NULL) { c->err = IE_UNSUPPORTED; return r; } }

// core file interface
void writeb(stream c, const u8 *bytes, ulen count) {
	CHECKERR(write,)
	c->err = c->def->write(c, bytes, count);
}
bool ateof(stream c) {
	CHECKERR(at_eof,true)
	return c->def->at_eof(c);
}
ulen readb(stream c, u8 *bytes, ulen count) {
	CHECKERR(read,0)
	if (ateof(c)) {
		return 0;
	}
	c->err = c->def->read(c, bytes, &count);
	assert(count != 0 || c->err != IE_NONE || ateof(c)); // must read something, have an error, or be at EOF.
	return count;
}
ioerr closes(stream c) {
	if (c->err != IE_NONE) {
		return c->err;
	}
	if (c->def->close != NULL) {
		return c->def->close(c);
	} else if (c->def->flush != NULL) {
		return c->def->flush(c);
	} else if (c->def->check != NULL) {
		return c->def->check(c);
	} else {
		return IE_NONE;
	}
}
ioerr flushs(stream c) {
	if (c->err != IE_NONE) {
		return c->err;
	}
	if (c->def->flush != NULL) {
		return c->def->flush(c);
	} else if (c->def->check != NULL) {
		return c->def->check(c);
	} else {
		return IE_NONE;
	}
}
ioerr checks(stream c) {
	if (c->err != IE_NONE) {
		return c->err;
	}
	if (c->def->check != NULL) {
		return c->def->check(c);
	} else {
		return IE_NONE;
	}
}

// extended interface
void writes(stream c, string str) {
	writeb(c, str, strlen(str));
}
void writesn(stream c, string str, ulen max) {
	writeb(c, str, strnlen(str, max));
}
void writech(stream c, char ch) {
	u8 uch = ch;
	writeb(c, &uch, 1);
}
void writeu8(stream c, u8 v) {
	writeb(c, &v, 1);
}
void writeu16be(stream c, u16 v) {
	u8 vs[] = {(u8) (v >> 8), (u8) v};
	writeb(c, vs, 2);
}
void writeu16le(stream c, u16 v) {
	u8 vs[] = {(u8) v, (u8) (v >> 8)};
	writeb(c, vs, 2);
}
void writeu32be(stream c, u32 v) {
	u8 vs[] = {(u8) (v >> 24), (u8) (v >> 16), (u8) (v >> 8), (u8) v};
	writeb(c, vs, 4);
}
void writeu32le(stream c, u32 v) {
	u8 vs[] = {(u8) v, (u8) (v >> 8), (u8) (v >> 16), (u8) (v >> 24)};
	writeb(c, vs, 4);
}
void writeu64be(stream c, u64 v) {
	u8 vs[] = {(u8) (v >> 56), (u8) (v >> 48), (u8) (v >> 40), (u8) (v >> 32), (u8) (v >> 24), (u8) (v >> 16), (u8) (v >> 8), (u8) v};
	writeb(c, vs, 8);
}
void writeu64le(stream c, u64 v) {
	u8 vs[] = {(u8) v, (u8) (v >> 8), (u8) (v >> 16), (u8) (v >> 24), (u8) (v >> 32), (u8) (v >> 40), (u8) (v >> 48), (u8) (v >> 56)};
	writeb(c, vs, 8);
}
void writeln(stream c, string str) {
	writes(c, str);
	writech(c, '\n');
}
ulen reads(stream c, mutable_string str, ulen count) {
	count = readb(c, str, count - 1);
	str[count] = '\0';
	return count;
}
char readch(stream c) {
	u8 target;
	return readb(c, &target, 1) == 0 ? 0 : (char) target;
}
u8 readu8(stream c) {
	u8 target;
	return readb(c, &target, 1) == 0 ? 0 : target;
}
u16 readu16be(stream c) {
	u8 vs[2];
	readb(c, vs, 2);
	return ((u16) vs[1]) | (((u16) vs[0]) << 8);
}
u16 readu16le(stream c) {
	u8 vs[2];
	readb(c, vs, 2);
	return ((u16) vs[0]) | (((u16) vs[1]) << 8);
}
u32 readu32be(stream c) {
	u8 vs[4];
	readb(c, vs, 4);
	return ((u32) vs[3]) | (((u32) vs[2]) << 8) | (((u32) vs[1]) << 16) | (((u32) vs[0]) << 24);
}
u32 readu32le(stream c) {
	u8 vs[4];
	readb(c, vs, 4);
	return ((u32) vs[0]) | (((u32) vs[1]) << 8) | (((u32) vs[2]) << 16) | (((u32) vs[3]) << 24);
}
u64 readu64be(stream c) {
	u8 vs[8];
	readb(c, vs, 8);
	return ((u64) vs[7]) | (((u64) vs[6]) << 8) | (((u64) vs[5]) << 16) | (((u64) vs[4]) << 24) | (((u64) vs[3]) << 32) | (((u64) vs[2]) << 40) | (((u64) vs[1]) << 48) | (((u64) vs[0]) << 56);
}
u64 readu64le(stream c) {
	u8 vs[8];
	readb(c, vs, 8);
	return ((u64) vs[0]) | (((u64) vs[1]) << 8) | (((u64) vs[2]) << 16) | (((u64) vs[3]) << 24) | (((u64) vs[4]) << 32) | (((u64) vs[5]) << 40) | (((u64) vs[6]) << 48) | (((u64) vs[7]) << 56);
}
void flushsq(stream c) {
	ioerr e = flushs(c);
	if (e != IE_NONE) {
		panic_static(ie_name(e));
	}
}
void checksq(stream c) {
	ioerr e = checks(c);
	if (e != IE_NONE) {
		panic_static(ie_name(e));
	}
}
void closesq(stream c) {
	ioerr e = closes(c);
	if (e != IE_NONE) {
		panic_static(ie_name(e));
	}
}

static string ie_names[] = {"no error", "not found", "out of range", "unsupported", "denied", "fault", "invalid", "busy", "full", "unknown"};
static string local_names[] = {"No error", "Operation not permitted", "No such file or directory", "No such process", "Interrupted system call", "I/O error", "No such device or address", "Argument list too long", "Exec format error", "Bad file number", "No child processes", "Try again", "Out of memory", "Permission denied", "Bad address", "Block device required", "Device or resource busy", "File exists", "Cross-device link", "No such device", "Not a directory", "Is a directory", "Invalid argument", "File table overflow", "Too many open files", "Not a typewriter", "Text file busy", "File too large", "No space left on device", "Illegal seek", "Read-only file system", "Too many links", "Broken pipe", "Math argument out of domain of func", "Math result not representable", "Resource deadlock would occur", "File name too long", "No record locks available", "Function not implemented", "Directory not empty", "Too many symbolic links encountered", "Operation would block", "No message of desired type", "Identifier removed", "Channel number out of range", "Level 2 not synchronized", "Level 3 halted", "Level 3 reset", "Link number out of range", "Protocol driver not attached", "No CSI structure available", "Level 2 halted", "Invalid exchange", "Invalid request descriptor", "Exchange full", "No anode", "Invalid request code", "Invalid slot", "Resource deadlock would occur", "Bad font file format", "Device not a stream", "No data available", "Timer expired", "Out of streams resources", "Machine is not on the network", "Package not installed", "Object is remote", "Link has been severed", "Advertise error", "Srmount error", "Communication error on send", "Protocol error", "Multihop attempted", "RFS specific error", "Not a data message", "Value too large for defined data type", "Name not unique on network", "File descriptor in bad state", "Remote address changed", "Can not access a needed shared library", "Accessing a corrupted shared library", ".lib section in a.out corrupted", "Attempting to link in too many shared libraries", "Cannot exec a shared library directly", "Illegal byte sequence", "Interrupted system call should be restarted", "Streams pipe error", "Too many users", "Socket operation on non-socket", "Destination address required", "Message too long", "Protocol wrong type for socket", "Protocol not available", "Protocol not supported", "Socket type not supported", "Operation not supported on transport endpoint", "Protocol family not supported", "Address family not supported by protocol", "Address already in use", "Cannot assign requested address", "Network is down", "Network is unreachable", "Network dropped connection because of reset", "Software caused connection abort", "Connection reset by peer", "No buffer space available", "Transport endpoint is already connected", "Transport endpoint is not connected", "Cannot send after transport endpoint shutdown", "Too many references: cannot splice", "Connection timed out", "Connection refused", "Host is down", "No route to host", "Operation already in progress", "Operation now in progress", "Stale file handle", "Structure needs cleaning", "Not a XENIX named type file", "No XENIX semaphores available", "Is a named type file", "Remote I/O error", "Quota exceeded", "No medium found", "Wrong medium type", "Operation Canceled", "Required key not available", "Key has expired", "Key has been revoked", "Key was rejected by service", "Owner died", "State not recoverable", "Operation not possible due to RF-kill", "Memory page has hardware error"};

string ie_name(ioerr err) {
	u8 ie = IE_GET(err);
	u32 sys = IE_SYSGET(err);
	if (sys >= 1 && sys < (sizeof(local_names) / sizeof(string))) {
		return local_names[sys];
	}
	if (err < 0 || err >= (sizeof(ie_names) / sizeof(string))) {
		return "invalid IO error";
	}
	return ie_names[err];
}

/* TODO
void writesf(stream c, string fmt, ...);
void writesfv(stream c, string fmt, va_list args);

// returns true if it read an entire line, false otherwise. strips newline. empty line and return false on EOF.
bool readln(stream c, mutable_string str, ulen count);
// returns false on EOF, panics if line is too long.
bool readlnq(stream c, mutable_string str, ulen count);

ioerr tells(stream c, u64 *out);
u64 tellsq(stream c);
ioerr seeksets(stream c, u64 rel);
void seeksetsq(stream c, u64 rel);
ioerr seekrels(stream c, i64 rel);
void seekrelsq(stream c, i64 rel);
ioerr seekrevs(stream c, u64 rel);
void seekrevsq(stream c, u64 rel);

*/

stream stdin, stdout, stderr;

extern stream openfd(u64 fd, bool out, bool binary);

static void _init_streams(void) __attribute__((constructor));

static void _init_streams(void) {
	_init_string_streams();
	// TODO: buffering?
	stdin = _libca_openfd(0, false);
	stdout = _libca_openfd(1, true);
	stderr = _libca_openfd(2, true);
}
