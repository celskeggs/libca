/*
 * Copyright (c) 2015 Colby Skeggs
 * This file is part of libca, and is licensed under the MIT license.
 * See LICENSE.txt for details.
 */

#ifndef _ALLOC_H
#define _ALLOC_H

#include <catype.h>

typedef struct {
	// first arguments are to arena_custom_data
	void *(*alloc)(void*, ulen); // will never be passed zero as an argument. can return NULL if it cannot allocate memory, or panic itself.
	void *(*alloc_zeroed)(void*, ulen); // can be a NULL pointer to mean that faster functionality for zeroing is unavailable.
	void *(*realloc)(void*, void*, ulen); // can be a NULL pointer to mean that faster functionality is unavailable. ALSO will never be passed a NULL pointer, and length will never be zero.
	void (*free)(void*, void*); // can be a NULL pointer if freeing is unsupported.
	void (*deallocate)(void*); // can be a NULL pointer if complete deallocation is unsupported.
	void *_arena_custom_data;
} memory_arena;

// one crucial difference: returns NULL iff size == 0. On ENOMEM, panics.
void *malloc(ulen size);
void *malloc_zeroed(ulen size);
void *realloc(void *ptr, ulen size);
void free(void *target);

void *malloc_a(memory_arena *arena, ulen size);
void *malloc_zeroed_a(memory_arena *arena, ulen size);
void *realloc_a(memory_arena *arena, void *memory, ulen size);
void free_a(memory_arena *arena, void *target);

memory_arena *get_global_arena(void);

void new_default_arena(memory_arena *arena);
void new_default_arena_with_capacity(memory_arena *arena, ulen starting_capacity);
void new_slab_arena(memory_arena *arena, ulen element_size, ulen starting_count); // only allows elements of the specified size
void new_nofree_arena(memory_arena *arena, ulen starting_capacity); // cannot be freed. but faster! can be deallocated.
void new_custom_arena(memory_arena *arena, void *custom_data); // just NULLs out the fields and sets the custom data
void deallocate_arena(memory_arena *arena);

#endif
