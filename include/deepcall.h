#ifndef _DEEPCALL_H
#define _DEEPCALL_H

#include <catype.h>

typedef void *deepcall_structure;

typedef void (*deepcall_callback)(deepcall_structure, void *);

bool deepcall(deepcall_callback cb, void *argument);
void deepreturn(deepcall_struct str);

#endif
