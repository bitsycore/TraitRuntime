#include "Arena.h"

Arena* Arena_init(void *buffer, const size_t size) {
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

void* Arena_alloc(Arena *arena, const size_t size) {
    EXIT_IF(arena == NULL, "arena is NULL");
    EXIT_IF(size == 0, "Size must be greater than 0");
    EXIT_IF(arena->current + size > arena->buf + arena->capacity, "Arena is full");
    void* ptr = (void*)arena->current;
    arena->current += size;
    return ptr;
}