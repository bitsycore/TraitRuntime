#include "MemPool.h"

#include <stddef.h>
#include <stdlib.h>

#include "MaxAlign.h"

// ===========================================================
// MARK: INTERNAL UTILITIES
// ===========================================================

static size_t align_up(const size_t x) {
    return x + (TRM_MAX_ALIGN - 1) & ~(TRM_MAX_ALIGN - 1);
}

static int size_class(size_t size) {
    int class = 0;
    size = align_up(size) >> 4;
    while (size >>= 1) ++class;
    if (class >= MAX_CLASSES) class = MAX_CLASSES - 1;
    return class;
}

// ===========================================================
// MARK: BUCKET UTILITIES
// ===========================================================

static void insert_block(MemBucket* a, MemBlock* b) {
    const int class = size_class(b->size);
    b->free = true;
    b->next = a->free_lists[class];
    if (b->next) b->next->prev = b;
    b->prev = NULL;
    a->free_lists[class] = b;
}

static void remove_block(MemBucket* a, MemBlock* b, const int class) {
    if (b->prev) b->prev->next = b->next;
    else a->free_lists[class] = b->next;
    if (b->next) b->next->prev = b->prev;
    b->next = b->prev = NULL;
}

static void split_block(MemBucket* a, MemBlock* b, const size_t size) {
    if (b->size < size + HEADER_SIZE + TRM_MAX_ALIGN) return;
    MemBlock* newb = (MemBlock*)((uint8_t*)b + HEADER_SIZE + size);
    newb->size = b->size - size - HEADER_SIZE;
    newb->free = true;
    b->size = size;
    insert_block(a, newb);
}

static void coalesce_and_insert(MemBucket* a, MemBlock* b) {
    MemBlock* next = (MemBlock*)((uint8_t*)b + HEADER_SIZE + b->size);
    if ((uint8_t*)next < a->memory + a->size && next->free) {
        const int cls = size_class(next->size);
        remove_block(a, next, cls);
        b->size += HEADER_SIZE + next->size;
    }

    uint8_t* scan = a->memory;
    while (scan < (uint8_t*)b) {
        MemBlock* blk = (MemBlock*)scan;
        uint8_t* next_ptr = scan + HEADER_SIZE + blk->size;
        if (next_ptr == (uint8_t*)b && blk->free) {
            const int cls = size_class(blk->size);
            remove_block(a, blk, cls);
            blk->size += HEADER_SIZE + b->size;
            b = blk;
            break;
        }
        scan = next_ptr;
    }

    insert_block(a, b);
}

// ===========================================================
// MARK: BUCKET
// ===========================================================

MemBucket MemBucket_create(const size_t size) {
    MemBucket a = {0};
    a.size = align_up(size);
    a.memory = malloc(a.size);

    MemBlock* b = (MemBlock*)a.memory;
    b->size = a.size - HEADER_SIZE;
    b->free = true;
    b->next = b->prev = NULL;

    a.free_lists[MAX_CLASSES - 1] = b;
    return a;
}

void* MemBucket_alloc(MemBucket* a, size_t size) {
    size = align_up(size);
    const int class = size_class(size);
    for (int i = class; i < MAX_CLASSES; ++i) {
        MemBlock* b = a->free_lists[i];
        while (b) {
            if (b->size >= size) {
                remove_block(a, b, i);
                split_block(a, b, size);
                b->free = false;
                return (uint8_t*)b + HEADER_SIZE;
            }
            b = b->next;
        }
    }
    return NULL;
}

void MemBucket_free(MemBucket* a, void* ptr) {
    if (!ptr) return;
    MemBlock* b = (MemBlock*)((uint8_t*)ptr - HEADER_SIZE);
    b->free = true;
    coalesce_and_insert(a, b);
}

void MemBucket_destroy(MemBucket* a) {
    free(a->memory);
    a->memory = NULL;
}

// ===========================================================
// MARK: POOL
// ===========================================================

MemPool MemPool_create(const size_t arena_size) {
    MemPool pool = {0};
    pool.arena_size = arena_size;
    return pool;
}

void* MemPool_alloc(MemPool* pool, const size_t size) {
    for (MemPoolNode* node = pool->head; node; node = node->next) {
        void* p = MemBucket_alloc(&node->arena, size);
        if (p) return p;
    }

    MemPoolNode* new_node = malloc(sizeof(MemPoolNode));
    new_node->arena = MemBucket_create(pool->arena_size);
    new_node->next = pool->head;
    pool->head = new_node;

    return MemBucket_alloc(&new_node->arena, size);
}

void MemPool_free(const MemPool* pool, void* ptr) {
    for (MemPoolNode* node = pool->head; node; node = node->next) {
        const uint8_t* start = node->arena.memory;
        const uint8_t* end = start + node->arena.size;
        if ((uint8_t*)ptr > start && (uint8_t*)ptr < end) {
            MemBucket_free(&node->arena, ptr);
            return;
        }
    }
}

void MemPool_destroy(MemPool* pool) {
    MemPoolNode* node = pool->head;
    while (node) {
        MemPoolNode* next = node->next;
        MemBucket_destroy(&node->arena);
        free(node);
        node = next;
    }
    pool->head = NULL;
}