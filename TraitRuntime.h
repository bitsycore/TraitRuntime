#ifndef TRAIT_RUNTIME_H
#define TRAIT_RUNTIME_H

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "HashStr.h"

#define MAX_TRAITS 16
#define MAX_TRAIT_IMPLS 16
#define MAX_TYPE 16

#define MAX_METHODS_PER_TRAITS 8
#define MAX_PARAMS_PER_METHODS 8

typedef struct {
	HashStr name;
	size_t size;
} Type;

typedef struct {
	Type* type;
	void* data;
} Object;

struct Trait;

typedef struct {
	HashStr name;
	HashStr param_types[MAX_PARAMS_PER_METHODS];
	size_t param_count;
	struct Trait* trait;
} Method;

typedef struct {
	HashStr name;
	Method methods[MAX_METHODS_PER_TRAITS];
	size_t method_count;
} Trait;

typedef struct {
	const Object* object;
	const Trait* trait;
	const Method* method;
} MethodContext;

typedef void* (*MethodImpl)(MethodContext CTX, va_list argv);

typedef struct {
	Type* type;
	Trait* trait;
	MethodImpl methods[MAX_METHODS_PER_TRAITS];
} TraitImpl;

// ===================================
// General
void TraitRuntime_init();
void TraitRuntime_clean();

// ===================================
// Type
Type* Type_create(HashStr name, size_t size);
Type* Type_get(HashStr name);
bool Type_implement(const Type* type, const Trait* trait);

// ===================================
// Trait
Trait* Trait_create(HashStr name);
Trait* Trait_get(HashStr name);
Method* Trait_addMethod(Trait* trait, HashStr method_name, const HashStr* param_types, size_t param_count);
Method* Trait_getMethod(const Trait* trait, HashStr method_name);
bool Trait_equal(const Trait* this, const Trait* other);

// ===================================
// TraitImpl
TraitImpl* TraitImpl_create(Trait* trait, Type* type);
void TraitImpl_addMethod(TraitImpl* trait_impl, const Method* method, MethodImpl method_impl);
TraitImpl* TraitImpl_get(const Type* type, const Trait* trait);
MethodImpl TraitImpl_getMethodImpl(const TraitImpl* impl, HashStr method_name);

// ===================================
// Object
Object* Object_new(Type* type);
Object* Object_newFrom(Type* type, void* data);
MethodImpl Object_getMethod(const Object* obj, HashStr trait_name, HashStr method_name);
bool Object_is(const Object* obj, const Type* type);
void Object_finalize(Object* obj);
void* Object_call(const Object* obj, const Method* method, ...);
void* Object_callStr(const Object* obj, HashStr trait_name, HashStr method_name, ... );

// ======================================================================================
// BUILT IN TYPE
// ======================================================================================

extern Type* UInt8;
extern Type* UInt16;
extern Type* UInt32;
extern Type* UInt64;

extern Type* Int8;
extern Type* Int16;
extern Type* Int32;
extern Type* Int64;

extern Type* Float32;
extern Type* Float64;

extern Trait* trait_Finalizable;
extern Method* method_Finalizable_finalize;

// ======================================================================================
// MACRO UTILS
// ======================================================================================

#define TR_____COUNT_ARGS(...) TR_____COUNT_ARGS_(,##__VA_ARGS__,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0)
#define TR_____COUNT_ARGS_(z,a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,cnt,...) cnt

#define USE(T, trait_name) \
	for (T * it = trait_name; it != NULL; it = NULL)

// =========================================
// METHOD DEFINITION
// =========================================

#define DEF_PARAM(...) \
    (HashStr[]){ __VA_ARGS__ }, TR_____COUNT_ARGS(__VA_ARGS__)

#define TYPE(name, data) \
	Type_create(HASH_STR(name), sizeof(data))

#define TRAIT(name) \
	Trait_create(HASH_STR(name))

#define TRAIT_IMPL(trait, type_name) \
	TraitImpl_create(Trait_get(HASH_STR(trait)), Type_get(HASH_STR(type_name)))

#define PARAM(name) \
    HASH_STR(name)

#define TRAIT_ADD_METHOD(trait, method_name, ...) ({ \
    HashStr _params[] = { __VA_ARGS__ }; \
    size_t _count = sizeof(_params) / sizeof(HashStr); \
    Trait_addMethod(trait, HASH_STR(method_name), _params, _count); \
})

#define TRAIT_IMPL_METHOD(trait_impl, method, fn) \
    TraitImpl_addMethod(trait_impl, Trait_getMethod(trait_impl->trait, HASH_STR(method)), fn)

#define CALL(obj, trait_name, method_name, ...) \
	Object_callStr(obj, HASH_STR(trait_name), HASH_STR(method_name) __VA_OPT__(, )__VA_ARGS__) \

// =========================================
// METHOD DEFINITION
// =========================================

#define METHOD_UNWRAP_START(obj_type_name, trait_type_name, method_type_name) \
	CTX->object->data; \
	assert(HashStr_equal(&CTX->object->type->name, &HASH_STR(obj_type_name))); \
	assert(HashStr_equal(&CTX->trait->name, &HASH_STR(trait_type_name))); \
	assert(HashStr_equal(&CTX->method->name, &HASH_STR(method_type_name))); \
	unsigned int HIDDEN___count = 0\

#define ARG_UNWRAP(type) \
	va_arg(args, type); HIDDEN___count++

#define METHOD_UNWRAP_END() \
	assert(HIDDEN___count == CTX->method->param_count)

#endif