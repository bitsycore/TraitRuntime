#include "HashStr.h"

#include <stdbool.h>
#include <string.h>

bool HashStr_equal(const HashStr *a, const HashStr *b) {
  if (a == b)
    return true;
  if (a->hash != b->hash)
    return false;
  return strcmp(a->str, b->str) == 0;
}
