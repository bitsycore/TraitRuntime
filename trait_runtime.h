#ifndef TRAIT_RUNTIME_H
#define TRAIT_RUNTIME_H

#include <stdlib.h>

#include "hash_str.h"

#define MAX_METHODS 16
#define MAX_PARAMS 16

#define MAX_TRAITS 16
#define MAX_TRAIT_IMPLS 16
#define MAX_TYPE 16

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
	HashStr param_types[MAX_PARAMS];
	size_t param_count;
	struct Trait* trait;
} Method;

typedef struct {
	HashStr name;
	Method methods[MAX_METHODS];
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
	MethodImpl methods[MAX_METHODS];
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

// ======================================================================================
// MARK: SUGAR MACRO
// ======================================================================================

#define TYPE(name, data) \
	Type_create(HASH_STR(name), sizeof(data))

#define TRAIT(name) \
	Trait_create(HASH_STR(name))

#define TRAIT_IMPL(trait, type_name) \
	TraitImpl_create(Trait_get(HASH_STR(trait)), Type_get(HASH_STR(type_name)))

#define PARAM(name) \
    HASH_STR(name)

#define TRAIT_ADD_METHOD(trait, method, ...) ({ \
    HashStr _params[] = { __VA_ARGS__ }; \
    size_t _count = sizeof(_params) / sizeof(HashStr); \
    Trait_addMethod(trait, HASH_STR(method), _params, _count); \
})

#define TRAIT_IMPL_METHOD(trait_impl, method, fn) \
    TraitImpl_addMethod(trait_impl, Trait_getMethod(trait_impl->trait, HASH_STR(method)), fn)

#define CALL(obj, trait_name, method_name, ...) \
	Object_callStr(obj, HASH_STR(trait_name), HASH_STR(method_name) __VA_OPT__(, )__VA_ARGS__); \

// =========================================
// METHOD DEFINITION
// =========================================

#define METHOD_UNWRAP_START(obj_type_name, trait_type_name, method_type_name) \
	CTX->object->data; \
	assert(hashed_str_eq(&CTX->object->type->name, &HASH_STR(obj_type_name))); \
	assert(hashed_str_eq(&CTX->trait->name, &HASH_STR(trait_type_name))); \
	assert(hashed_str_eq(&CTX->method->name, &HASH_STR(method_type_name))); \
	unsigned int HIDDEN___count = 0\

#define ARG_UNWRAP(type) \
	va_arg(args, type); HIDDEN___count++

#define METHOD_UNWRAP_END() \
	assert(HIDDEN___count == CTX->method->param_count)

// =========================================
// UTILITIES
// =========================================

#define USE(T, trait_name) \
	for (T * it = trait_name; it != NULL; it = NULL)

#endif