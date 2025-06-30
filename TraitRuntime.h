#ifndef TRAIT_RUNTIME_H
#define TRAIT_RUNTIME_H

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "HashStr.h"
#include "common/MacroUtils.h"
#include "common/ErrorHandling.h"

#define ENABLE_BUILTIN true

#define MAX_TRAITS 32
#define MAX_TRAIT_IMPLS 32
#define MAX_TYPE 32

#define MAX_METHODS_PER_TRAITS 8
#define MAX_PARAMS_PER_METHODS 8

struct Trait;

typedef struct {
	HashStr name;
	size_t size;
	size_t id;
} Type;

typedef struct {
	size_t type_id;
	void* data;
} Object;

typedef struct {
	HashStr name;
	HashStr param_types[MAX_PARAMS_PER_METHODS];
	size_t param_count;
	size_t param_vararg_at;
	struct Trait* trait;
} Method;

typedef struct {
	HashStr name;
	Method methods[MAX_METHODS_PER_TRAITS];
	size_t method_count;
	size_t data_size;
} Trait;

typedef struct {
	const Object* object;
	const Trait* trait;
	const Method* method;
	va_list args;
} MethodContext;

typedef void* (*MethodImpl)(MethodContext* CTX);

typedef struct {
	size_t type_id;
	Trait* trait;
	MethodImpl methods[MAX_METHODS_PER_TRAITS];
} TraitImpl;

// ===================================
// General
void TraitRuntime_init(bool enable_builtin);
void TraitRuntime_clean();

// ===================================
// Type
Type* Type_create(HashStr name, size_t size);
Type* Type_get(HashStr name);
Type* Type_getById(size_t id);
bool Type_implement(const Type* type, const Trait* trait);
bool Type_implementById(size_t id, const Trait* trait);
bool Type_equal(const Type* this, const Type* other);

// ===================================
// Trait
Trait* Trait_create(HashStr name, size_t data_size);
Trait* Trait_get(HashStr name);
Method* Trait_addMethod(Trait* trait, HashStr method_name, const HashStr* param_types, size_t param_count);
Method* Trait_getMethod(const Trait* trait, HashStr method_name);
bool Trait_equal(const Trait* this, const Trait* other);

// ===================================
// TraitImpl
TraitImpl* TraitImpl_create(Trait* trait, Type* type);
void TraitImpl_addMethod(TraitImpl* trait_impl, const Method* method, MethodImpl method_impl);
TraitImpl* TraitImpl_get(size_t type_id, const Trait* trait);
MethodImpl TraitImpl_getMethodImpl(const TraitImpl* trait_impl, HashStr method_name);

// ===================================
// Object
Object* Object_new(const Type* type);
Object* Object_newFrom(const Type* type, void* data);
MethodImpl Object_getMethod(const Object* obj, HashStr trait_name, HashStr method_name);
bool Object_is(const Object* obj, const Type* type);
bool Object_implement(const Object* obj, const Trait* trait);
void Object_destroy(Object* obj);
void* Object_call(const Object* obj, const Method* method, ...);
void* Object_callStrEx(const Object* obj, HashStr trait_name, HashStr method_name, ... );
void* Object_callStr(const Object* obj, HashStr method_name, ... );
Type* Object_getType(const Object* obj);

// ======================================================================================
// BUILT IN TYPE
// ======================================================================================

typedef struct {
	struct {
		Type* UInt8;
		Type* UInt16;
		Type* UInt32;
		Type* UInt64;
		Type* Int8;
		Type* Int16;
		Type* Int32;
		Type* Int64;
		Type* Float32;
		Type* Float64;
	} types;
	struct {
		struct {
			Trait* trait;
			struct {
				Method* finalize;
			} methods;
		} Finalizable;
		struct {
			Trait* trait;
			struct {
				Method* construct;
			} methods;
		} Constructable;
	} traits;
} BuiltInStore;

extern BuiltInStore BuiltIn;

// =========================================
// SUGAR
// =========================================

#define TRAIT_RUNTIME_INIT() TraitRuntime_init(ENABLE_BUILTIN)

#define TYPE(name, data) \
	Type_create(HASH_STR(name), sizeof(data))

#define _______TR_TRAIT_0() ((void)0, EXIT("Cannot create a trait without a name"))
#define _______TR_TRAIT_1(name) Trait_create(HASH_STR(name), 0)
#define _______TR_TRAIT_2(name, type) Trait_create(HASH_STR(name), sizeof(type))
#define _______TR_TRAIT_N(...) MU_GET_MACRO_2(__VA_ARGS__, _______TR_TRAIT_2, _______TR_TRAIT_1, _______TR_TRAIT_0)
#define TRAIT(...) _______TR_TRAIT_N(__VA_ARGS__)(__VA_ARGS__)

#define DEF_PARAM(...) MU_COUNT_ARGS(__VA_ARGS__) == 0 ? NULL : (HashStr[]){ MU_MAP(HASH_STR, __VA_ARGS__) }, MU_COUNT_ARGS(__VA_ARGS__)
#define PARAM_VA_MARK "__*_VAMARK_*__"
#define TRAIT_ADD_METHOD(trait, method_name, ...) Trait_addMethod(trait, HASH_STR(method_name), DEF_PARAM(__VA_ARGS__))
#define TRAIT_GET(trait_name) Trait_get(HASH_STR(trait_name))

#define TRAIT_IMPL(traitName, typeName) \
	TraitImpl_create(Trait_get(HASH_STR(traitName)), Type_get(HASH_STR(typeName)))

#define TRAIT_IMPL_METHOD(traitImpl, methodName, fn) \
    TraitImpl_addMethod(traitImpl, Trait_getMethod(traitImpl->trait, HASH_STR(methodName)), fn)

#define CALL(obj, methodName, ...) \
	Object_callStr(obj, HASH_STR(methodName) __VA_OPT__(,) __VA_ARGS__) \

#define CALL_EX(obj, traitName, methodName, ...) \
	Object_callStrEx(obj, HASH_STR(traitName), HASH_STR(methodName) __VA_OPT__(,) __VA_ARGS__) \

// =========================================
// METHOD DEFINITION
// =========================================

#define METHOD_UNWRAP_START() \
	CTX->object->data; \
	unsigned int HIDDEN___count = 0\

#define CHECK_TYPE(_type) \
	EXIT_IF_NOT(Type_equal(Object_getType(CTX->object), _type), "Type \"%s\" called this method expecting type \"%s\"", Object_getType(CTX->object)->name.str, _type->name.str)
#define CHECK_TRAIT(_trait) \
	EXIT_IF_NOT(Trait_equal(CTX->trait, _trait), "Type \"%s\" called this method expecting type \"%s\"", CTX->trait->name.str, _trait->name.str)
#define CHECK_METHOD(_method) \
	EXIT_IF_NOT(HashStr_equal(&CTX->method->name, &_method->name), "Method \"%s\" called but this method expect \"%s\"", CTX->method->name.str, _method->name.str)
#define CHECK_METHOD_STR(_methodName) \
	EXIT_IF_NOT(HashStr_equal(&CTX->method->name, &HASH_STR(_methodName)), "Method \"%s\" called but this method expect \"%s\"", CTX->method->name.str, _methodName)

#define CHECK_ALL(_type, _trait, _method) \
	CHECK_TYPE(_type); CHECK_TRAIT(_trait); CHECK_METHOD(_method)

#define CHECK_TYPE_STR(_typeName) CHECK_TYPE(Type_get(HASH_STR(_typeName)))
#define CHECK_TRAIT_STR(_traitName) CHECK_TRAIT(Trait_get(HASH_STR(_traitName)))
#define CHECK_ALL_STR(_typeName, _traitName, _methodName) CHECK_TYPE_STR(_typeName); CHECK_TRAIT_STR(_traitName); CHECK_METHOD_STR(_methodName)

#define ARG_UNWRAP(type) \
	va_arg(CTX->args, type); HIDDEN___count++;\
	CTX->args = CTX->args/*Disable clang warn on clang mingw cause msvc require it to be not const*/ \

#define METHOD_UNWRAP_END() \
	EXIT_IF(HIDDEN___count != CTX->method->param_count, \
		"Arg count declarated for Method \"%s\" differt from param unwrapping count for impl of Type \"%s\"", \
		CTX->method->name.str, Object_getType(CTX->object)->name.str)

// =========================================
// MACRO UTILITIES
// =========================================

#define TR_USE(T, traitName) for (T * it = traitName; it != NULL; it = NULL)
#define TR_REPEAT(count) for (unsigned int i = 0; i < count; i++)

#endif