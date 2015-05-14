#ifndef _STREAM_H
#define _STREAM_H

#include <catype.h>
#include <stdarg.h>

typedef enum {
	// these match up with ie_name's implementation in stream.c
	IE_NONE=0,
	IE_FILENOTFOUND,
	IE_OUTOFRANGE,
	IE_UNSUPPORTED,
	IE_UNKNOWN
} ioerr;

typedef struct _struct_stream_def stream_def;
typedef struct _stream *stream;

struct _struct_stream_def {
	ioerr (*write)(stream, const u8*, ulen);
	ioerr (*read)(stream, u8*, ulen*); // at_eof will be checked before this is called
	bool (*at_eof)(stream);
	ioerr (*p_tell)(stream, u64*);
	ioerr (*p_seek_set)(stream, u64);
	ioerr (*p_seek_rel)(stream, i64);
	ioerr (*p_seek_end)(stream, u64);
	ioerr (*check)(stream); // check defaults to no-op if set to NULL.
	ioerr (*flush)(stream); // flush defaults to check if set to NULL.
	ioerr (*close)(stream); // close defaults to flush if set to NULL.
};
struct _stream {
	stream_def *def;
	ioerr err;
};

// memory layout of a stream: a pointer to a stream_def pointer followed by an active error followed by stream-specific data.
#define STREAM_DATA(c, T) ((T*) (((void*) c) + sizeof(struct _stream)))

stream defstream(stream_def *def, ulen datasize);
stream openstrin(string in);
ioerr openbufin(stream *c, u8 *bytes, ulen count);
ioerr openbufout(stream *c, u8 *bytes, ulen max, ulen *count);

ioerr bufferstream(stream base, stream *nstr, bool flush_on_newline, ulen bufsize);

void writeb(stream c, const u8 *bytes, ulen count);
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

bool ateof(stream c); // return true if EOF or error - does not clear error
ulen readb(stream c, u8 *bytes, ulen count); // return number of bytes actually read - 0 means EOF or error
ulen reads(stream c, mutable_string str, ulen count); // return number of bytes actually read (not including null pointer) - 0 means EOF or error
char readch(stream c); // return 0 on EOF or error, otherwise return character that was read. NOTE THAT THIS IS AMBIGUOUS IF YOU HAVE BINARY INPUT!
// these all return 0 on error or EOF - make sure to check! note that this is AMBIGUOUS!
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

ioerr tells(stream c, u64 *out);
u64 tellsq(stream c);
ioerr seeksets(stream c, u64 rel);
void seeksetsq(stream c, u64 rel);
ioerr seekrels(stream c, i64 rel);
void seekrelsq(stream c, i64 rel);
ioerr seekrevs(stream c, u64 rel);
void seekrevsq(stream c, u64 rel);

ioerr checks(stream c);
void checksq(stream c);
ioerr flushs(stream c);
void flushsq(stream c);
ioerr closes(stream c);
void closesq(stream c);

string ie_name(ioerr error);

extern stream stdin, stdout, stderr;

#endif
