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

#ifndef _ASSERT_H
#define _ASSERT_H

#include <catype.h>
#include <panic.h>

#define _assert_fail_tostring(x) #x
#define _assert_fail(expr, file, line) panic_static(file ":" _assert_fail_tostring(line) ": assertion '" expr "' failed."))
#define assert(expr) (expr ? ((void) 0) : _assert_fail(#expr, __FILE__, __LINE__)

#endif
