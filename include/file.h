#ifndef _FILE_H
#define _FILE_H

#include <catype.h>
#include <stream.h>

#define FILE_READ FALSE
#define FILE_WRITE TRUE
#define FILE_TEXTUAL FALSE
#define FILE_BINARY TRUE

ioerr openf(stream *c, string filename, bool out, bool binary);

ioerr removef(string filename);
ioerr renamef(string from, string to);

#endif
