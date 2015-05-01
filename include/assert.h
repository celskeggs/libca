#ifndef _ASSERT_H
#define _ASSERT_H

#include <catype.h>
#include <panic.h>

#ifdef NDEBUG
#define assert(ignore) ((void) 0)
#else
#define assert(expr) (expr ? ((void) 0) : panic_static(__FILE__ ":" __LINE__ ": " __func__ ": assertion '" #expr "' failed."))
#endif

#endif
