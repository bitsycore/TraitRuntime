#ifndef TRAIT_RUNTIME_H
#define TRAIT_RUNTIME_H

#include <stdarg.h>
#include <stdlib.h>

#include "Config.h"
#include "TraitRuntime/BuiltIn.h"
#include "TraitRuntime/Class.h"
#include "TraitRuntime/Object.h"
#include "TraitRuntime/Runtime.h"
#include "TraitRuntime/Trait.h"
#include "TraitRuntime/Types.h"
#include "TraitRuntime/Commons/ErrorHandling.h"
#include "TraitRuntime/Commons/MacroUtils.h"
#include "TraitRuntime/String/HashStr.h"

// =========================================
// MARK: MACRO SUGAR
// =========================================

#define TR_INIT() Runtime_init(ENABLE_BUILTIN)

#define TR_CLASS(name, data) Class_create(HASH_STR(name), sizeof(data))
#define TR_TRAIT(name) Trait_create(HASH_STR(name))

#define TR_TRAIT_ADD_METHOD(trait, method_name, ...) Trait_addMethod(trait, HASH_STR(method_name), TR_PARAMS(__VA_ARGS__))
#define TR_PARAMS(...) MU_COUNT_ARGS(__VA_ARGS__) == 0 ? NULL : (HashStr[]){ __VA_OPT__(MU_MAP(HASH_STR, __VA_ARGS__)) } , MU_COUNT_ARGS(__VA_ARGS__)
#define TR_PARAM_VA "__*_VAMARK_*__"

#define TR_TRAIT_IMPL(traitName, typeName) TraitImpl_create(Trait_get(HASH_STR(traitName)), Class_get(HASH_STR(typeName)))
#define TR_TRAIT_IMPL_METHOD(traitImpl, methodName, fn) TraitImpl_addMethod(traitImpl, Trait_getMethod(traitImpl->trait, HASH_STR(methodName)), fn)

#define TR_OBJ_CALL(obj, methodName, ...) Object_callStr(obj, HASH_STR(methodName) __VA_OPT__(,) __VA_ARGS__)
#define TR_OBJ_TRAIT_CALL(obj, traitName, methodName, ...) Object_callTraitStr(obj, HASH_STR(traitName), HASH_STR(methodName) __VA_OPT__(,) __VA_ARGS__)

// =========================================
// MARK: METHODS SUGAR
// =========================================

#define TR_METHOD_UNWRAP_START() \
	CTX->object->data; \
	CTX = CTX;\
	unsigned int HIDDEN___count = 0\

#define TR_CHECK_TYPE(_type) \
	EXIT_IF_NOT(Class_equal(Object_getClass(CTX->object), _type), "Class \"%s\" called this method expecting type \"%s\"", Object_getClass(CTX->object)->name.str, _type->name.str)
#define TR_CHECK_TRAIT(_trait) \
	EXIT_IF_NOT(Trait_equal(CTX->method_def->trait, _trait), "Class \"%s\" called this method expecting type \"%s\"", CTX->method_def->trait->name.str, _trait->name.str)
#define TR_CHECK_METHOD(_method) \
	EXIT_IF_NOT(HashStr_equal(&CTX->method_def->name, &_method->name), "Method \"%s\" called but this method expect \"%s\"", CTX->method_def->name.str, _method->name.str)
#define TR_CHECK_METHOD_STR(_methodName) \
	EXIT_IF_NOT(HashStr_equal(&CTX->method_def->name, &HASH_STR(_methodName)), "Method \"%s\" called but this method expect \"%s\"", CTX->method_def->name.str, _methodName)

#define TR_CHECK_ALL(_type, _trait, _method) \
	TR_CHECK_TYPE(_type); TR_CHECK_TRAIT(_trait); TR_CHECK_METHOD(_method)

#define TR_CHECK_TYPE_STR(_typeName) TR_CHECK_TYPE(Class_get(HASH_STR(_typeName)))
#define TR_CHECK_TRAIT_STR(_traitName) TR_CHECK_TRAIT(Trait_get(HASH_STR(_traitName)))
#define TR_CHECK_ALL_STR(_typeName, _traitName, _methodName) TR_CHECK_TYPE_STR(_typeName); TR_CHECK_TRAIT_STR(_traitName); TR_CHECK_METHOD_STR(_methodName)

#define ARG_UNWRAP(type) \
	va_arg(*CTX->args, type); HIDDEN___count++\

#define TR_METHOD_UNWRAP_END() \
	EXIT_IF(HIDDEN___count != CTX->method_def->params_count, \
		"Arg count declarated for Method \"%s\" differt from param unwrapping count for impl of Class \"%s\"", \
		CTX->method_def->name.str, Object_getClass(CTX->object)->name.str)

// =========================================
// MARK: MACRO UTILITIES
// =========================================

#define TR_USE(T, traitName) for (T * it = traitName; it != NULL; it = NULL)
#define TR_REPEAT(count) for (unsigned int it = 0; it < count; it++)

#endif