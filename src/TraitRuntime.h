#ifndef TRAIT_RUNTIME_H
#define TRAIT_RUNTIME_H

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "TraitRuntime/String/HashStr.h"
#include "TraitRuntime/Commons/MacroUtils.h"
#include "TraitRuntime/Commons/ErrorHandling.h"

#define ENABLE_BUILTIN true

#define MAX_TRAITS 32
#define MAX_TYPE 32
#define MAX_TRAIT_IMPLS ((MAX_TYPE * MAX_TRAITS) / 2)

#define MAX_METHODS_PER_TRAITS 8
#define MAX_PARAMS_PER_METHODS 8

struct Class;
struct Object;
struct Trait;
struct TraitImpl;
struct Method;
struct MethodContext;

typedef struct Class {
	HashStr name;
	size_t size;
	size_t id;
} Class;

typedef struct Object {
	size_t type_id;
	void* data;
} Object;

typedef struct Method {
	HashStr name;
	HashStr params[MAX_PARAMS_PER_METHODS];
	size_t params_count;
	size_t param_vararg_at;
	struct Trait* trait;
} Method;

typedef struct Trait {
	HashStr name;
	Method methods[MAX_METHODS_PER_TRAITS];
	size_t method_count;
	size_t data_size;
} Trait;

typedef struct MethodContext {
	const Object* object;
	const Trait* trait;
	const Method* method;
	va_list args;
} MethodContext;

typedef void* (*MethodImpl)(MethodContext* CTX);

typedef struct TraitImpl {
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
Class* Type_create(HashStr name, size_t size);
Class* Type_get(HashStr name);
Class* Type_getById(size_t id);
bool Type_implement(const Class* type, const Trait* trait);
bool Type_implementById(size_t id, const Trait* trait);
bool Type_equal(const Class* this, const Class* other);

// ===================================
// Trait
Trait* Trait_create(HashStr name, size_t data_size);
Trait* Trait_get(HashStr name);
Method* Trait_addMethod(Trait* trait, HashStr method_name, const HashStr* param_types, size_t param_count);
Method* Trait_getMethod(const Trait* trait, HashStr method_name);
bool Trait_equal(const Trait* this, const Trait* other);

// ===================================
// TraitImpl
TraitImpl* TraitImpl_create(Trait* trait, Class* type);
void TraitImpl_addMethod(TraitImpl* trait_impl, const Method* method, MethodImpl method_impl);
TraitImpl* TraitImpl_get(size_t type_id, const Trait* trait);
MethodImpl TraitImpl_getMethodImpl(const TraitImpl* trait_impl, HashStr method_name);

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
Class* Object_getType(const Object* obj);

// ======================================================================================
// BUILT IN TYPE
// ======================================================================================

typedef struct {
	struct {
		Class* UInt8;
		Class* UInt16;
		Class* UInt32;
		Class* UInt64;
		Class* Int8;
		Class* Int16;
		Class* Int32;
		Class* Int64;
		Class* Float32;
		Class* Float64;
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
} Container_BuiltIn;

extern Container_BuiltIn BuiltIn;

// =========================================
// SUGAR
// =========================================

#define TR_INIT() TraitRuntime_init(ENABLE_BUILTIN)
#define TR_TYPE(name, data) Type_create(HASH_STR(name), sizeof(data))

#define _______TR_TRAIT_1(name, ...) Trait_create(HASH_STR(name), 0)
#define _______TR_TRAIT_2(name, type) Trait_create(HASH_STR(name), sizeof(type))
#define TR_TRAIT(name, ...) MU_GET_MACRO_2(_, ##__VA_ARGS__, _______TR_TRAIT_2, _______TR_TRAIT_1)(name, __VA_ARGS__)

#define TR_TRAIT_ADD_METHOD(trait, method_name, ...) Trait_addMethod(trait, HASH_STR(method_name), TR_PARAMS(__VA_ARGS__))
#define TR_PARAMS(...) MU_COUNT_ARGS(__VA_ARGS__) == 0 ? NULL : (HashStr[]){ __VA_OPT__(MU_MAP(HASH_STR, __VA_ARGS__)) } , MU_COUNT_ARGS(__VA_ARGS__)
#define TR_PARAM_VA "__*_VAMARK_*__"


#define TR_TRAIT_IMPL(traitName, typeName) TraitImpl_create(Trait_get(HASH_STR(traitName)), Type_get(HASH_STR(typeName)))
#define TR_TRAIT_IMPL_METHOD(traitImpl, methodName, fn) TraitImpl_addMethod(traitImpl, Trait_getMethod(traitImpl->trait, HASH_STR(methodName)), fn)

#define TR_OBJ_CALL(obj, methodName, ...) Object_callStr(obj, HASH_STR(methodName) __VA_OPT__(,) __VA_ARGS__)
#define TR_OBJ_CALL_EX(obj, traitName, methodName, ...) Object_callStrEx(obj, HASH_STR(traitName), HASH_STR(methodName) __VA_OPT__(,) __VA_ARGS__)

// =========================================
// METHOD DEFINITION
// =========================================

#define TR_METHOD_UNWRAP_START() \
	CTX->object->data; \
	unsigned int HIDDEN___count = 0\

#define TR_CHECK_TYPE(_type) \
	EXIT_IF_NOT(Type_equal(Object_getType(CTX->object), _type), "Type \"%s\" called this method expecting type \"%s\"", Object_getType(CTX->object)->name.str, _type->name.str)
#define TR_CHECK_TRAIT(_trait) \
	EXIT_IF_NOT(Trait_equal(CTX->trait, _trait), "Type \"%s\" called this method expecting type \"%s\"", CTX->trait->name.str, _trait->name.str)
#define TR_CHECK_METHOD(_method) \
	EXIT_IF_NOT(HashStr_equal(&CTX->method->name, &_method->name), "Method \"%s\" called but this method expect \"%s\"", CTX->method->name.str, _method->name.str)
#define TR_CHECK_METHOD_STR(_methodName) \
	EXIT_IF_NOT(HashStr_equal(&CTX->method->name, &HASH_STR(_methodName)), "Method \"%s\" called but this method expect \"%s\"", CTX->method->name.str, _methodName)

#define TR_CHECK_ALL(_type, _trait, _method) \
	TR_CHECK_TYPE(_type); TR_CHECK_TRAIT(_trait); TR_CHECK_METHOD(_method)

#define TR_CHECK_TYPE_STR(_typeName) TR_CHECK_TYPE(Type_get(HASH_STR(_typeName)))
#define TR_CHECK_TRAIT_STR(_traitName) TR_CHECK_TRAIT(Trait_get(HASH_STR(_traitName)))
#define TR_CHECK_ALL_STR(_typeName, _traitName, _methodName) TR_CHECK_TYPE_STR(_typeName); TR_CHECK_TRAIT_STR(_traitName); TR_CHECK_METHOD_STR(_methodName)

#define ARG_UNWRAP(type) \
	va_arg(CTX->args, type); HIDDEN___count++;\
	CTX->args = CTX->args/*Disable clang warn on clang mingw cause msvc require it to be not const*/ \

#define TR_METHOD_UNWRAP_END() \
	EXIT_IF(HIDDEN___count != CTX->method->params_count, \
		"Arg count declarated for Method \"%s\" differt from param unwrapping count for impl of Type \"%s\"", \
		CTX->method->name.str, Object_getType(CTX->object)->name.str)

// =========================================
// MACRO UTILITIES
// =========================================

#define TR_USE(T, traitName) for (T * it = traitName; it != NULL; it = NULL)
#define TR_REPEAT(count) for (unsigned int i = 0; i < count; i++)

#endif