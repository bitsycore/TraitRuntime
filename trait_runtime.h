#ifndef TRAIT_RUNTIME_H
#define TRAIT_RUNTIME_H

#include <stdlib.h>

#include "hashed_str.h"

#define MAX_METHODS 16
#define MAX_PARAMS 16
#define MAX_TRAIT_IMPLS 16
#define MAX_TRAITS 16

typedef struct {
    HashedStr name;
} Type;

typedef struct {
    Type* type;
    void* instance;
} Object;

typedef struct {
    HashedStr name;
    HashedStr param_types[MAX_PARAMS];
    size_t param_count;
} TraitMethod;

typedef struct {
    HashedStr name;
    TraitMethod methods[MAX_METHODS];
    size_t method_count;
} Trait;

typedef struct {
    Object* object;
    Trait* trait;
    TraitMethod* method;
} MethodCtx;

typedef void* (*TraitFn)(MethodCtx CTX, ...);

typedef struct {
    HashedStr name;
    Trait* trait;
    TraitFn methods[MAX_METHODS];
} TraitImpl;



// Trait
Trait* trait_create(HashedStr name);
Trait* trait_get(HashedStr name);

// Methods
void trait_add_method(Trait* trait, HashedStr method_name, const HashedStr* param_types, size_t param_count);
TraitMethod* trait_get_method(HashedStr trait_name, HashedStr method_name);

// TraitImpl
TraitImpl* trait_impl_create(Trait* trait, HashedStr type_name);
void trait_impl_add_method(TraitImpl* impl, HashedStr method_name, TraitFn fn);

// Object
Object* object_create(HashedStr type_name, void* instance);
void object_destroy(Object* obj);

// Call
TraitFn resolve_method(const Object* obj, HashedStr trait_name, HashedStr method_name);

// ======================================================================================
// MARK: SUGAR MACRO
// ======================================================================================

#define TRAIT(name) \
    trait_create(HASH_STR(name))

#define TRAIT_ADD_METHOD(trait, method, ...) \
    do { \
        HashedStr _params[] = { __VA_ARGS__ }; \
        size_t _count = sizeof(_params) / sizeof(HashedStr); \
        trait_add_method(trait, HASH_STR(method), _params, _count); \
    } while(0)

#define TRAIT_IMPL(type_name, trait) \
    trait_impl_create(type_name, HASH_STR(trait))\

#define TRAIT_IMPL_METHOD(trait, method, fn) \
    trait_impl_add_method(trait, HASH_STR(method), fn)

#define TRAIT_IMPL_REGISTER(type_name, trait) \
    register_trait_impl(impl_##type_name##_##trait)

#define OBJECT_CREATE(type_name, data) \
    object_create(HASH_STR(type_name), data)

#define OBJECT_FINALIZE(obj) \
    object_destroy(obj)

#define CALL(obj, trait_name, method_name, ...) \
    resolve_method( obj, HASH_STR(trait_name), HASH_STR(method_name)) ( (MethodCtx) { obj, trait_get(HASH_STR(trait_name)), trait_get_method(HASH_STR(trait_name), HASH_STR(method_name)) } __VA_OPT__(, ) __VA_ARGS__)

#define TRAIT_GET(name) \
    trait_get(#name)

#define USE(T, trait_name) \
    for (T * this = trait_name; this != NULL; this = NULL)

// METHOD DEFINITION

#define METHOD_UNWRAP_START(obj_type_name, trait_type_name, method_type_name) \
CTX->object->instance; \
assert(hashed_str_eq(&CTX->object->type->name, &HASH_STR(obj_type_name))); \
assert(hashed_str_eq(&CTX->trait->name, &HASH_STR(trait_type_name))); \
assert(hashed_str_eq(&CTX->method->name, &HASH_STR(method_type_name))); \
unsigned int HIDDEN___count = 0;\
va_list HIDDEN___args; \
va_start(HIDDEN___args, CTX->method->param_count)

#define ARG_UNWRAP(type) \
va_arg(HIDDEN___args, type); HIDDEN___count++

#define METHOD_UNWRAP_END() \
va_end(HIDDEN___args); \
assert(HIDDEN___count == CTX->method->param_count)

#endif
