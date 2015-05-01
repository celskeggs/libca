#ifndef _SYSTEM_H
#define _SYSTEM_H

#include <catype.h>

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

// actually uses panic - see panic.h
void exit(u8 code);

#endif
