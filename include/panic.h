/*
    Copyright 2015 Colby Skeggs

    This file is part of libca. See LICENSE.txt for details.

    libca is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    libca is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with libca.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _PANIC_H
#define _PANIC_H

#include <catype.h>

enum error_type_e {
	ERR_NONE=0,
	ERR_MALLOCED,
	ERR_STATIC,
	ERR_SYSEXIT
};

typedef struct {
	enum error_type_e type;
	u8 exit_code;
	string str;
} error;

typedef void (*recover_callback)(void *);

// return true if an error occurred
bool recover(recover_callback cb, void *param, error *err_ret);
void panic(string str) __attribute__((noreturn));
void panic_static(string str) __attribute__((noreturn));

#endif
