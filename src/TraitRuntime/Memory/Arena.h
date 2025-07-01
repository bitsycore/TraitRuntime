#ifndef Arena_H
#define Arena_H

#include <stdlib.h>

#include "../Commons/ErrorHandling.h"

typedef struct {
	size_t capacity;
	uintptr_t current;
	uintptr_t buf;
} Arena;

#if !( defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L )
typedef union {
	long long ll;
	long double ld;
	void* p;
} max_align_t;
#else
#include <stddef.h>
#endif

#define Arena_requiredSize(size) (sizeof(Arena) + size)
#define Arena_init(buffer, size) ({ \
	Arena* _arena = (buffer); \
	EXIT_IF(\
		(size) <= sizeof(Arena),\
		"Capacity must be greater than the size of Arena, "\
		"use Arena_requiredSize to calculate the required size"\
	);\
	_arena->capacity = (size) - sizeof(Arena); \
	_arena->current = (uintptr_t)_arena + sizeof(Arena); \
	_arena->buf = _arena->current; \
	(void*)_arena; \
})

#define Arena_alloc(_arena, size) ({ \
	EXIT_IF(Arena == NULL, "_arena is NULL"); \
	EXIT_IF(size == 0, "Size must be greater than 0"); \
	const uintptr_t aligned_current = _arena->current + (sizeof(max_align_t) - 1) & ~(sizeof(max_align_t) - 1);\
	_arena->buf + _arena->capacity - aligned_current < size\
		? NULL\
		: ({\
			_arena->current = aligned_current + size;\
			(void*)aligned_current;\
		});\
})

#define Arena_reset(_arena) EXIT_IF(_arena == NULL, "_arena is NULL");\
	_arena->current = _arena->buf

#endif