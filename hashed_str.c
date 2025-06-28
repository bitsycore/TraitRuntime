#include "hashed_str.h"

#include <stdbool.h>
#include <string.h>

bool hashed_str_eq(const HashedStr *a, const HashedStr *b) {
  if (a == b)
    return true;
  if (a->hash != b->hash)
    return false;
  return strcmp(a->str, b->str) == 0;
}
