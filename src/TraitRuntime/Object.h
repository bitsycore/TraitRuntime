#ifndef OBJECT_H
#define OBJECT_H

#include "Types.h"
#include "String/HashStr.h"

Object* Object_new(Class* clazz);
Object* Object_newFrom(Class* clazz, void* data_buffer);
void Object_destroy(Object* obj);

void* Object_call(const Object* obj, const MethodDef* method, ...);
void* Object_callTrait(const Object* obj, const Trait* trait, const MethodDef* method, ...);
void* Object_callStr(const Object* obj, HashStr method_name, ... );
void* Object_callTraitStr(const Object* obj, HashStr trait_name, HashStr method_name, ... );

Class* Object_getClass(const Object* obj);
bool Object_is(const Object* obj, const Class* clazz);
bool Object_implement(const Object* obj, const Trait* trait);

#endif //OBJECT_H