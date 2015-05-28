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

#ifndef _SYSTEM_H
#define _SYSTEM_H

#include <catype.h>

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

// actually uses panic - see panic.h
void exit(u8 code);

u32 system(string command); // with sh -c
u32 system_nosh(string command, string *args, ulen count); // raw
// return true iff SYS_EXIT_NORMAL would be true
bool system_check(string command);
bool system_check_nosh(string command, string *args, ulen count);
// panic if SYS_EXIT_NORMAL would be false
void systemq(string command);
void systemq_nosh(string command, string *args, ulen count);

// system exited normally, as opposed to sent a signal
#define SYS_EXITED(x) (((x) & 0x100) != 0)
// exit code - or, alternatively, the signal that killed the process (based on SYS_EXITED)
#define SYS_CODE(x) ((x) & 0xFF)
// if either exited normally AND exit code was zero
#define SYS_EXIT_NORMAL(x) (((x) & 0x1FF) == 0x100)

u32 get_argument_count(void);
string get_argument(u32 id);
string get_argument_opt(u32 id, string defarg);
string *get_arguments(void);
string get_program_reference(void);

extern u8 camain(u32 count, string *args);

#define CAMAIN(count, args) u8 camain(u32 count, string *args)
#define CAMAIN0() u8 camain(u32 __c, string *__a)

#ifndef _CA_ALLOW_MAIN // define this if you want to be able to define main anyway even though it won't be called by default.
extern struct _force_undef_type {} main(void); // main should not be defined in a libca program.
#endif

#endif
