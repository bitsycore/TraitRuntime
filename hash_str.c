#include "hash_str.h"

#include <stdbool.h>
#include <string.h>

bool hashed_str_eq(const HashStr *a, const HashStr *b) {
  if (a == b)
    return true;
  if (a->hash != b->hash)
    return false;
  return strcmp(a->str, b->str) == 0;
}
