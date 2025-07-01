#ifndef OBJECT_H
#define OBJECT_H

#include "TraitRuntime.h"

// ===================================
// Object
Object* Object_new(const Class* type);
Object* Object_newFrom(const Class* type, void* data);
MethodImpl Object_getMethod(const Object* obj, HashStr trait_name, HashStr method_name);
bool Object_is(const Object* obj, const Class* type);
bool Object_implement(const Object* obj, const Trait* trait);
void Object_destroy(Object* obj);
void* Object_call(const Object* obj, const Method* method, ...);
void* Object_callStrEx(const Object* obj, HashStr trait_name, HashStr method_name, ... );
void* Object_callStr(const Object* obj, HashStr method_name, ... );
Class* Object_getClass(const Object* obj);

#endif //OBJECT_H