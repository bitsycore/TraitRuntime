//
// Created by DWA on 01/07/2025.
//

#ifndef SEGREGATEDFREELISTARENA_H
#define SEGREGATEDFREELISTARENA_H

#include <stdint.h>
#include <stdbool.h>

#include "MaxAlign.h"

#define MAX_CLASSES 12
#define HEADER_SIZE ((sizeof(MemBlock) + (TRM_MAX_ALIGN - 1)) & ~(TRM_MAX_ALIGN - 1))

typedef struct MemBlock {
    size_t size;
    bool free;
    struct MemBlock* next;
    struct MemBlock* prev;
} MemBlock;

typedef struct {
    uint8_t* memory;
    size_t size;
    MemBlock* free_lists[MAX_CLASSES];
} MemBucket;

typedef struct MemPoolNode {
    MemBucket arena;
    struct MemPoolNode* next;
} MemPoolNode;

typedef struct {
    MemPoolNode* head;
    size_t arena_size;
} MemPool;

#endif //SEGREGATEDFREELISTARENA_H
