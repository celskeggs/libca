/*
 * Copyright (c) 2015 Colby Skeggs
 * This file is part of libca, and is licensed under the MIT license.
 * See LICENSE.txt for details.
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
void exit(u8 code) __attribute__((noreturn)); // also defined in lifecycle.h

#endif
