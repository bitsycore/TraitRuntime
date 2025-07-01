#ifndef CLASS_H
#define CLASS_H

#include "TraitRuntime.h"

// ===================================
// Class
Class* Class_create(HashStr name, size_t size);
Class* Class_get(HashStr name);
Class* Class_getById(size_t id);
bool Class_implement(const Class* clazz, const Trait* trait);
bool Class_implementById(size_t id, const Trait* trait);
bool Class_equal(const Class* this, const Class* other);

#endif //CLASS_H
