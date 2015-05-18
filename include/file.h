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

ioerr openf(stream *c, string filename, bool out);
stream openfq(string filename, bool out); // panics on error

ioerr openf_unbuf(stream *c, string filename, bool out);
stream openfq_unbuf(string filename, bool out); // panics on error

ioerr removef(string filename);
void removefq(string filename); // panics on error
ioerr renamef(string from, string to);
void renamefq(string from, string to); // panics on error

// referenced from stream.c
stream _libca_openfd(u32 fd, bool out);

#endif
