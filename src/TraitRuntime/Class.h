#ifndef CLASS_H
#define CLASS_H

#include "Types.h"
#include "String/HashStr.h"

#define CLASS_FLAG_IS_INIT    (1 << 0)
#define CLASS_FLAG_IS_BUILTIN (1 << 1)

// ===================================
// Class
Class* Class_create(HashStr name, size_t size);
Class* Class_get(HashStr name);

bool Class_implement(const Class* clazz, const Trait* trait);
bool Class_equal(const Class* this, const Class* other);

void Class_validateTrait(Class* clazz, const Trait* trait);
MethodImpl* Class_implementMethod(Class* clazz, MethodDef* method_def, MethodFunc method_func);
MethodImpl* Class_getMethodImplStr(const Class* class, HashStr method_name);

#endif //CLASS_H
