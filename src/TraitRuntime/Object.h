#ifndef OBJECT_H
#define OBJECT_H

#include "Types.h"
#include "String/HashStr.h"

Object* Object_new(Class* clazz);
Object* Object_newFrom(Class* clazz, void* data_buffer);

MethodFunc Object_getMethodStr(const Object* obj, HashStr trait_name, HashStr method_name);
MethodFunc Object_getMethod(const Object* obj, const Method* method);
Class* Object_getClass(const Object* obj);

void* Object_call(const Object* obj, const Method* method, ...);
void* Object_callStr(const Object* obj, HashStr method_name, ... );
void* Object_callTraitStr(const Object* obj, HashStr trait_name, HashStr method_name, ... );

bool Object_is(const Object* obj, const Class* clazz);
bool Object_implement(const Object* obj, const Trait* trait);

void Object_destroy(Object* obj);


#endif //OBJECT_H