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

#include <alloc.h>
#include <panic.h>
#include <memory.h>
#include "dlmalloc-2.8.6/interface.h"

static memory_arena global;

static void _init_alloc() __attribute__((constructor));

static void _init_alloc() {
	new_default_arena(&global);
}

// one crucial difference: returns NULL iff size == 0. On ENOMEM, panics.
void *malloc(ulen size) {
	return malloc_a(&global, size);
}
void *malloc_zeroed(ulen size) {
	return malloc_zeroed_a(&global, size);
}
void *realloc(void *ptr, ulen size) {
	return realloc_a(&global, ptr, size);
}
void free(void *target) {
	free_a(&global, target);
}
memory_arena *get_global_arena(void) {
	return &global;
}

void *malloc_a(memory_arena *arena, ulen size) {
	if (size == 0) {
		return NULL;
	}
	void *out = arena->alloc(arena->_arena_custom_data, size);
	if (out == NULL) {
		panic_static("OOM");
	}
	return out;
}

void *malloc_zeroed_a(memory_arena *arena, ulen size) {
	if (size == 0) {
		return NULL;
	}
	if (arena->alloc_zeroed == NULL) {
		void *out = malloc_a(arena, size);
		memset(out, 0, size);
		return out;
	}
	void *out = arena->alloc_zeroed(arena->_arena_custom_data, size);
	if (out == NULL) {
		panic_static("OOM");
	}
	return out;
}

void *realloc_a(memory_arena *arena, void *memory, ulen size) {
	if (size == 0) {
		if (memory != NULL) {
			free_a(arena, memory);
		}
		return NULL;
	} else if (memory == NULL) {
		return malloc_a(arena, size);
	}
	if (arena->realloc == NULL) {
		panic_static("attempt to realloc in unsupporting arena");
	}
	void *out = arena->realloc(arena->_arena_custom_data, memory, size);
	if (out == NULL) {
		panic_static("OOM");
	}
	return out;
}

void free_a(memory_arena *arena, void *memory) {
	if (memory == NULL) {
		panic_static("attempt to free NULL");
	}
	if (arena->free == NULL) {
		panic_static("attempt to free in unsupporting arena");
	}
	arena->free(arena->_arena_custom_data, memory);
}

/*
typedef struct {
	// first arguments are to arena_custom_data
	void *(*alloc)(void*, ulen); // will never be passed zero as an argument. can return NULL if it cannot allocate memory, or panic itself.
	void *(*alloc_zeroed)(void*, ulen); // can be a NULL pointer to mean that faster functionality for zeroing is unavailable.
	void *(*realloc)(void*, void*, ulen); // will never be passed a NULL pointer, and length will never be zero. can return NULL if it cannot allocate memory, or panic itself.
	void (*free)(void*, void*); // can be a NULL pointer if freeing is unsupported.
	void (*deallocate)(void*); // can be a NULL pointer if complete deallocation is unsupported.
	void *_arena_custom_data;
} memory_arena;
*/

void new_custom_arena(memory_arena *arena, void *custom_data) { // just NULLs out the fields and sets the custom data
	if (custom_data == NULL) {
		panic_static("memory arena custom data cannot be null!");
	}
	arena->alloc = NULL;
	arena->alloc_zeroed = NULL;
	arena->realloc = NULL;
	arena->free = NULL;
	arena->deallocate = NULL;
	arena->_arena_custom_data = custom_data;
}

void new_default_arena(memory_arena *arena) {
	new_default_arena_with_capacity(arena, 0);
}

void *dlmalloc_mspace_alloc_zeroed(void *space, ulen size) { // substitute
	return mspace_calloc(space, size, 1);
}

void dlmalloc_mspace_deallocate(void *target) { // substitute
	destroy_mspace(target);
}

void new_default_arena_with_capacity(memory_arena *arena, ulen starting_capacity) {
	mspace space = create_mspace(starting_capacity, 0); // TODO: Should locked be 1 instead? That would help with threadsafety.
	if (space == NULL) {
		panic("OOM while building new dlmalloc mspace");
	}
	mspace_track_large_chunks(space, 1); // make sure that destroying an arena will always free all its memory
	new_custom_arena(arena, space);
	arena->alloc = mspace_malloc;
	arena->alloc_zeroed = dlmalloc_mspace_alloc_zeroed;
	arena->free = mspace_free;
	arena->realloc = mspace_realloc;
	arena->deallocate = dlmalloc_mspace_deallocate;
}

void deallocate_arena(memory_arena *arena) {
	if (arena->deallocate == NULL) {
		panic_static("arena does not support deallocation!");
	}
	arena->deallocate(arena->_arena_custom_data);
	arena->_arena_custom_data = NULL; // cause segfaults later rather than stale accesses.
}

/* TODO
void new_slab_arena(memory_arena *arena, ulen element_size, ulen starting_count); // only allows elements of the specified size
void new_nofree_arena(memory_arena *arena, ulen starting_pages); // cannot be freed. but faster! can be deallocated.
*/
