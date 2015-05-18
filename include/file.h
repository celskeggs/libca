/*
 * Copyright (c) 2015 Colby Skeggs
 * This file is part of libca, and is licensed under the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _FILE_H
#define _FILE_H

#include <catype.h>
#include <stream.h>

#define FILE_READ FALSE
#define FILE_WRITE TRUE
#define FILE_TEXTUAL FALSE
#define FILE_BINARY TRUE

ioerr openf(stream *c, string filename, bool out, bool binary);
stream openfq(string filename, bool out, bool binary); // panics on error

ioerr openf_unbuf(stream *c, string filename, bool out, bool binary);
stream openfq_unbuf(string filename, bool out, bool binary); // panics on error

ioerr removef(string filename);
void removefq(string filename); // panics on error
ioerr renamef(string from, string to);
void renamefq(string from, string to); // panics on error

// private: stream openfd(u64 fd, bool out, bool binary);
// referenced from stream.c

#endif
