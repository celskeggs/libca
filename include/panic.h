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
	const char *str;
} error;

typedef void (*recover_callback)(void *);

error recover(recover_callback cb, void *param);
void panic(const char *str);
void panic_static(const char *str);
void exit(u8 code); // also defined in lifecycle.h

#endif