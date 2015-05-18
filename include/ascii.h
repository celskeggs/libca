/*
 * Copyright (c) 2015 Colby Skeggs
 * This file is part of libca, and is licensed under the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _ASCII_H
#define _ASCII_H

#include <catype.h>

extern u8 _ASCII_TAB[256];

// 1	control code (0-8, 14-31, 127)
// 2	tab (9)
// 4	whitespace (10-13)
// 8	space (32)
// 16	symbols (33-47, 58-64, 91-96, 123-126)
// 32	hex digits (48-57, 65-70, 97-102)
// 64	all letters (65-90, 97-122)
// 128	uppercase letters (65-90)
// zero for everything above 127, so everything returns false in these cases.
// because EOF is -1, it points to index 255 in _ASCII_TAB, which is zero anyway!

// hex digit but not letter
bool isdigit(int x);
#define isdigit(x) ((_ASCII_TAB[x & 255] & 96) == 32)
bool islower(int x);
#define islower(x) ((_ASCII_TAB[x & 255] & 192) == 64)
bool isupper(int x);
#define isupper(x) ((_ASCII_TAB[x & 255] & 128) != 0)
bool isalpha(int x);
#define isalpha(x) ((_ASCII_TAB[x & 255] & 64) != 0)
bool isalnum(int x);
#define isalnum(x) ((_ASCII_TAB[x & 255] & 96) != 0)
bool isxdigit(int x);
#define isxdigit(x) ((_ASCII_TAB[x & 255] & 32) != 0)
bool iscntrl(int x);
#define iscntrl(x) ((_ASCII_TAB[x & 255] & 1) != 0)
bool isgraph(int x);
#define isgraph(x) ((_ASCII_TAB[x & 255] & 112) != 0)
bool isspace(int x);
#define isspace(x) ((_ASCII_TAB[x & 255] & 14) != 0)
bool isblank(int x);
#define isblank(x) ((_ASCII_TAB[x & 255] & 10) != 0)
bool isprint(int x);
#define isprint(x) ((_ASCII_TAB[x & 255] & 120) != 0)
bool ispunct(int x);
#define ispunct(x) ((_ASCII_TAB[x & 255] & 16) != 0)
bool isascii(int x);
#define isascii(x) (_ASCII_TAB[x & 255] != 0)

int tolower(int x);
int toupper(int x);

#endif
