#ifndef TRAIT_H
#define TRAIT_H

#include "Types.h"
#include "String/HashStr.h"

// ===================================
// Trait
Trait* Trait_create(HashStr name);
Trait* Trait_get(HashStr name);
MethodDef* Trait_addMethod(Trait* trait, HashStr method_name, const HashStr* param_types, size_t param_count);
MethodDef* Trait_getMethod(const Trait* trait, HashStr method_name);
bool Trait_equal(const Trait* this, const Trait* other);

#endif //TRAIT_H
