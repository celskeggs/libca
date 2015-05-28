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

#ifndef _RANDOM_H
#define _RANDOM_H

#include <catype.h>

// should be high-quality but not necessarily cryptographically secure

typedef void *random_state;

u32 random();
void random_seed(u32 seed);

random_state random_new(u32 seed);
u32 random_next(random_state rand);
void random_destroy(random_state rand);

#endif
