/*
    This file is part of libca/linux. See LICENSE.txt for details.

    libca/linux is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    libca/linux is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with libca/linux.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "settings.h"
#include <catype.h>
#include <panic.h>
#include <memory.h>
#include <libsyscall.h>

#define size_t ulen
#define ptrdiff_t iptrd

#define PROT_READ	0x1
#define PROT_WRITE	0x2
#define PROT_EXEC	0x4
#define PROT_SEM	0x8
#define PROT_NONE	0x0
#define PROT_GROWSDOWN	0x01000000
#define PROT_GROWSUP	0x02000000

#define MAP_SHARED	0x01
#define MAP_PRIVATE	0x02
#define MAP_TYPE	0x0F
#define MAP_FIXED	0x10
#define MAP_ANONYMOUS	0x20

static void *mmap(void *addr, size_t len, int prot, int flags, int fd, iptr off) {
	void *out;
#ifdef LSC_LINUX
# ifdef LSC_32BIT
	if ((off & 4095) != 0) {
		return (void*) -1;
	}
	out = lsc_mmap2(addr, len, prot, flags, fd, off >> 12);
	if (((u32) out) >= 0xFFFFF000) { // an error - the last page in memory is never mapped by the kernel.
		return (void*) -1;
	}
	return out;
# else
#  ifdef LSC_64BIT
	out =  lsc_mmap(addr, len, prot, flags, fd, off);
	if (((u64) out) >= 0xFFFFFFFFFFFFF000) { // an error - the last page in memory is never mapped by the kernel.
		return (void*) -1;
	}
	return out;
#  else
#   error Cannot tell if 32-bit or 64-bit Linux!
#  endif
# endif
#else
# error mmap interface.c bridge only implemented for Linux.
#endif
}

static int munmap(void *addr, ulen length) {
	int out = lsc_munmap(addr, length);
	return out == 0 ? 0 : -1;
}

#include "malloc-2.8.6.c"
