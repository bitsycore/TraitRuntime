#ifndef TRAIT_H
#define TRAIT_H

#include "Types.h"
#include "String/HashStr.h"

// ===================================
// Trait
Trait* Trait_create(HashStr name);
Trait* Trait_get(HashStr name);
Method* Trait_addMethod(Trait* trait, HashStr method_name, const HashStr* param_types, size_t param_count);
Method* Trait_getMethod(const Trait* trait, HashStr method_name);
bool Trait_equal(const Trait* this, const Trait* other);

// ===================================
// TraitImpl
TraitImpl* TraitImpl_create(Trait* trait, Class* clazz);
void TraitImpl_addMethod(TraitImpl* trait_impl, const Method* method, MethodFunc method_impl);
TraitImpl* TraitImpl_get(const Class* clazz, const Trait* trait);
MethodFunc TraitImpl_getMethodImplStr(const TraitImpl* trait_impl, HashStr method_name);
TraitImpl* TraitImpl_getForMethodStr(Class* clazz, HashStr methodName);

#endif //TRAIT_H
