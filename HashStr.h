#ifndef HASHED_STR_H
#define HASHED_STR_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

typedef struct {
  uint32_t hash;
  const char *str;
} HashStr;

#define FNV1A_32_PRIME 16777619u
#define FNV1A_32_BASIS 2166136261u

static inline uint32_t fnv1a_32(const char *str, const size_t len) {
  uint32_t hash = FNV1A_32_BASIS;
  for (size_t i = 0; i < len; ++i)
    hash = (hash ^ (uint8_t)str[i]) * FNV1A_32_PRIME;
  return hash;
}

bool HashStr_equal(const HashStr* a, const HashStr* b);

#define HASH_STR(s)                                                            \
  ((HashStr){.hash = fnv1a_32(s, sizeof(s) / sizeof(s[0])), .str = s})

#endif // HASHED_STR_H