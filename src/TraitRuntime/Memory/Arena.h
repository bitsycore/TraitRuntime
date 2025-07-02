#ifndef Arena_H
#define Arena_H

#include <stdint.h>

#include "../Commons/ErrorHandling.h"
#include "TraitRuntime/Commons/Log.h"

typedef struct {
	size_t capacity;
	uintptr_t current;
	uintptr_t buf;
} Arena;

#define Arena_sizeLeft(arena) (arena->capacity - (arena->current - (uintptr_t)arena->buf))
#define Arena_requiredSize(size) (sizeof(Arena) + (size))
#define Arena_initBuffer(_arena) (void*)(_arena->buf - sizeof(Arena))
#define Arena_reset(_arena) EXIT_IF(_arena == NULL, "_arena is NULL");(_arena->current = _arena->buf)

static inline Arena* Arena_init(void *buffer, const size_t size) {
	Arena* arena = (buffer);
	EXIT_IF(
		size <= sizeof(Arena),
		"Capacity must be greater than the size of Arena, "
		"use Arena_requiredSize to calculate the required size"
	);
	arena->capacity = size - sizeof(Arena); \
	arena->current = (uintptr_t)arena + sizeof(Arena); \
	arena->buf = arena->current;
	return arena;
}

static inline void* Arena_alloc(Arena *arena, const size_t size) {
	EXIT_IF(arena == NULL, "arena is NULL");
	EXIT_IF(size == 0, "Size must be greater than 0");
	EXIT_IF(size > Arena_sizeLeft(arena), "Arena is full");
	void* ptr = (void*)arena->current;
	arena->current += size;
#ifndef NDEBUG
	if (Arena_sizeLeft(arena) < arena->capacity * 0.95)
		LOG_WARN("ARENA SIZE LEFT: %llu\n", Arena_sizeLeft(arena));
#endif
	return ptr;
}

#endif