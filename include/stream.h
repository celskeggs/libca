#ifndef _STREAM_H
#define _STREAM_H

#include <catype.h>
#include <stdarg.h>

typedef enum {
	IE_NONE=0,
	IE_FILENOTFOUND,
	IE_UNKNOWN
} ioerr;

typedef struct _struct_stream_def stream_def;
typedef stream_def **stream;

struct _struct_stream_def {
	ioerr (*w_b)(stream, u8*, ulen);
	ioerr (*w_str)(stream, string, ulen);
	ioerr (*r_b)(stream, u8*, ulen*);
	ioerr (*r_str)(stream, string, ulen*);
	ioerr (*p_tell)(stream, i64*);
	ioerr (*p_seek_set)(stream, i64);
	ioerr (*p_seek_rel)(stream, i64);
	ioerr (*p_seek_end)(stream, i64);
	ioerr (*check)(stream);
	ioerr (*flush)(stream);
	ioerr (*close)(stream);
};

// memory layout of a stream: a pointer to a stream_def pointer followed by stream-specific data.
#define STREAM_DATA(c, T) (T*) (((stream_def**) stream) + 1)

ioerr defstream(stream *c, stream_def *def, ulen datasize);
ioerr openstrin(stream *c, string in);
ioerr openbufin(stream *c, u8 *bytes, ulen count);
ioerr openbufout(stream *c, u8 *bytes, ulen max, ulen *count);

ioerr bufferstream(stream base, stream *nstr, bool flush_on_newline, ulen bufsize);

void writeb(stream c, u8 *bytes, ulen count);
void writes(stream c, string str);
void writesn(stream c, string str, ulen max);
void writesf(stream c, string fmt, ...);
void writesfv(stream c, string fmt, va_list args);
void writech(stream c, char ch);
void writeu8(stream c, u8 v);
void writeu16be(stream c, u16 v);
void writeu16le(stream c, u16 v);
void writeu32be(stream c, u32 v);
void writeu32le(stream c, u32 v);
void writeu64be(stream c, u64 v);
void writeu64le(stream c, u64 v);
void writeln(stream c, string str);

bool ateof(stream c);
ulen readb(stream c, u8 *bytes, ulen count);
ulen reads(stream c, mutable_string str, ulen count);
char readch(stream c);
u8 readu8(stream c);
u16 readu16be(stream c);
u16 readu16le(stream c);
u32 readu32be(stream c);
u32 readu32le(stream c);
u64 readu64be(stream c);
u64 readu64le(stream c);
// returns true if it read an entire line, false otherwise. strips newline. empty line and return false on EOF.
bool readln(stream c, mutable_string str, ulen count);
// returns false on EOF, panics if line is too long.
bool readlnq(stream c, mutable_string str, ulen count);

i64 tells(stream c);
void seeksets(stream c);
void seekrels(stream c);
void seekrevs(stream c);

ioerr flushs(stream c);
ioerr checks(stream c);
ioerr closes(stream c);

extern stream stdin, stdout, stderr;

#endif
