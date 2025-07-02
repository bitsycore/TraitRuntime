#ifndef CLASS_H
#define CLASS_H

#include "Types.h"
#include "String/HashStr.h"

// ===================================
// Class
Class* Class_create(HashStr name, size_t size);
Class* Class_get(HashStr name);
Class* Class_getById(ClassId id);
bool Class_implement(const Class* clazz, const Trait* trait);
bool ClassId_implement(ClassId, const Trait* trait);
bool Class_equal(const Class* this, const Class* other);

#endif //CLASS_H
