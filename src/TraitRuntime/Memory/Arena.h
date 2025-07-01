#ifndef Arena_H
#define Arena_H

#include "../Commons/ErrorHandling.h"
#include "MaxAlign.h"

typedef struct {
	size_t capacity;
	uintptr_t current;
	uintptr_t buf;
} Arena;

#define Arena_requiredSize(size) (sizeof(Arena) + (size))
Arena* Arena_init(void* buffer, size_t size);
void* Arena_alloc(Arena* arena, size_t size);
#define Arena_reset(_arena) EXIT_IF(_arena == NULL, "_arena is NULL");(_arena->current = _arena->buf)
#define Arena_buffer(_arena) (void*)(_arena->buf - sizeof(Arena))
#endif