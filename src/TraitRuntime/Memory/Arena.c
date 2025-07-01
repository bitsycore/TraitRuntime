#include "Arena.h"

Arena* Arena_init(void *buffer, size_t size) {
    Arena* arena = (buffer);
    EXIT_IF(
        size <= sizeof(Arena),
        "Capacity must be greater than the size of Arena, "
        "use Arena_requiredSize to calculate the required size"
    );
    arena->capacity = (size) - sizeof(Arena); \
    arena->current = (uintptr_t)arena + sizeof(Arena); \
    arena->buf = arena->current;
    return arena;
}

void* Arena_alloc(Arena *arena, const size_t size) {
    EXIT_IF(arena == NULL, "arena is NULL");
    EXIT_IF(size == 0, "Size must be greater than 0");
    const uintptr_t aligned_current = arena->current + (TRM_MAX_ALIGN - 1) & ~(TRM_MAX_ALIGN - 1);\
    if (arena->buf + arena->capacity - aligned_current >= size) {
        arena->current = aligned_current + size;
    }
    return (void*)aligned_current;
}