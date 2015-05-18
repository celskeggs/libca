/*
 * Copyright (c) 2015 Colby Skeggs
 * This file is part of libca, and is licensed under the MIT license.
 * See LICENSE.txt for details.
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