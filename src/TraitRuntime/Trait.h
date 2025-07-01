#ifndef TRAIT_H
#define TRAIT_H
#include "TraitRuntime.h"

// ===================================
// Trait
Trait* Trait_create(HashStr name, size_t data_size);
Trait* Trait_get(HashStr name);
Method* Trait_addMethod(Trait* trait, HashStr method_name, const HashStr* param_types, size_t param_count);
Method* Trait_getMethod(const Trait* trait, HashStr method_name);
bool Trait_equal(const Trait* this, const Trait* other);

// ===================================
// TraitImpl
TraitImpl* TraitImpl_create(Trait* trait, Class* clazz);
void TraitImpl_addMethod(TraitImpl* trait_impl, const Method* method, MethodImpl method_impl);
TraitImpl* TraitImpl_get(size_t type_id, const Trait* trait);
MethodImpl TraitImpl_getMethodImplStr(const TraitImpl* trait_impl, HashStr method_name);
TraitImpl* TraitImpl_getForMethodStr(Class* clazz, HashStr methodName);

#endif //TRAIT_H
