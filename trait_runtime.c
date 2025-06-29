#include "trait_runtime.h"

#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>

// =====================================
// BUILT IN TYPE
// =====================================

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

Trait* Finalizable;
Method* Finalizable_finalize;

// =====================================
// GENERAL
// =====================================

static Type* POOL_TYPE[MAX_TYPE];
static size_t POOL_TYPE_COUNT = 0;

static Trait* POOL_TRAIT[MAX_TRAITS];
static size_t POOL_TRAIT_COUNT = 0;

static TraitImpl* POOL_TRAIT_IMPL[MAX_TRAIT_IMPLS];
static size_t POOL_TRAIT_IMPL_COUNT = 0;

// =====================================
// GENERAL
// =====================================

void TraitRuntime_init() {
    memset(POOL_TYPE, 0, sizeof(POOL_TRAIT_IMPL));
    memset(POOL_TRAIT, 0, sizeof(POOL_TRAIT_IMPL));
    memset(POOL_TRAIT_IMPL, 0, sizeof(POOL_TRAIT_IMPL));

    Finalizable = TRAIT("Finalizable");
    Finalizable_finalize = TRAIT_ADD_METHOD(Finalizable, "finalize");

    UInt8  = TYPE("UInt8", uint8_t);
    UInt16 = TYPE("UInt16", uint16_t);
    UInt32 = TYPE("UInt32", uint32_t);
    UInt64 = TYPE("UInt64", uint64_t);
    Int8  = TYPE("Int8", int8_t);
    Int16 = TYPE("Int16", int16_t);
    Int32 = TYPE("Int32", int32_t);
    Int64 = TYPE("Int64", int64_t);
    Float32 = TYPE("Float32", float);
    Float64 = TYPE("Float64", double);
}

void TraitRuntime_clean() {
    for (int i = 0; i < POOL_TRAIT_COUNT; i++) {
        free(POOL_TRAIT[i]);
    }
    for (int i = 0; i < POOL_TRAIT_IMPL_COUNT; i++) {
        free(POOL_TRAIT_IMPL[i]);
    }
    for (int i = 0; i < POOL_TYPE_COUNT; i++) {
        free(POOL_TYPE[i]);
    }
}

// =====================================
// TYPE
// =====================================

Type* Type_create(const HashStr name, const size_t size) {
    Type* t = malloc(sizeof(Type));
    t->name = name;
    t->size = size;
    assert(POOL_TYPE_COUNT < MAX_TYPE);
    POOL_TYPE[POOL_TYPE_COUNT++] = t;
    return t;
}

Type* Type_get(const HashStr name) {
    for (size_t i = 0; i < POOL_TYPE_COUNT; ++i) {
        if (hashed_str_eq(&POOL_TYPE[i]->name, &name) == true) {
            return POOL_TYPE[i];
        }
    }
    assert(!"<Type> not found");
    return NULL;
}

bool type_eq(const Type* this, const Type* other) {
    if (this == other) return true;
    if (hashed_str_eq(&this->name, &other->name)) return true;
    return false;
}

bool Type_implement(const Type* type, const Trait* trait) {
    for (size_t i = 0; i < POOL_TRAIT_IMPL_COUNT; ++i) {
        const TraitImpl* impl = POOL_TRAIT_IMPL[i];
        if (
            Trait_equal(impl->trait, trait) &&
            type_eq(impl->type, type)
        ) {
            return true;
        }
    }
    return false;
}

// =====================================
// TRAIT
// =====================================

Trait* Trait_create(const HashStr name) {
    Trait* t = malloc(sizeof(Trait));
    t->name = name;
    t->method_count = 0;
    assert(POOL_TRAIT_COUNT < MAX_TRAITS);
    POOL_TRAIT[POOL_TRAIT_COUNT++] = t;
    return t;
}

Trait* Trait_get(const HashStr name) {
    for (size_t i = 0; i < POOL_TRAIT_COUNT; ++i)
        if (hashed_str_eq(&POOL_TRAIT[i]->name, &name) == true)
            return POOL_TRAIT[i];
    return NULL;
}

bool Trait_equal(const Trait* this, const Trait* other) {
    if (this == other) return true;
    if (hashed_str_eq(&this->name, &other->name)) return true;
    return false;
}

Method* Trait_addMethod(Trait* trait, const HashStr method_name, const HashStr* param_types, const size_t param_count) {
    assert(trait->method_count < MAX_METHODS);
    Method* m = &trait->methods[trait->method_count++];
    m->name = method_name;
    m->param_count = param_count;
    m->trait = (struct Trait*) trait;
    for (size_t i = 0; i < param_count; ++i)
        m->param_types[i] = param_types[i];
    return m;
}

// =====================================
// TRAIT METHOD
// =====================================

Method* Trait_getMethod(const Trait* trait, const HashStr method_name) {
    const Trait* impl = trait;
    for (size_t j = 0; j < MAX_METHODS; ++j) {
        const Method* m = &impl->methods[j];
        if (hashed_str_eq(&m->name, &method_name) == true)
            return (Method*)m;
    }
    assert(!"<TraitMethod> not found in <Trait>");
    return NULL;
}

// =====================================
// TRAIT IMPL
// =====================================

TraitImpl* TraitImpl_create(Trait* trait, Type* type) {
    TraitImpl* impl = malloc(sizeof(TraitImpl));
    impl->type = type;
    impl->trait = trait;
    memset(impl->methods, 0, sizeof(impl->methods));
    assert(POOL_TRAIT_IMPL_COUNT < MAX_TRAIT_IMPLS);
    POOL_TRAIT_IMPL[POOL_TRAIT_IMPL_COUNT++] = impl;
    return impl;
}

void TraitImpl_addMethod(TraitImpl* trait_impl, const Method* method, const MethodImpl method_impl) {
    if (Trait_equal(trait_impl->trait, (Trait*) method->trait))
    for (size_t i = 0; i < trait_impl->trait->method_count; ++i) {
        if (hashed_str_eq(&trait_impl->trait->methods[i].name, &method->name) == true) {
            trait_impl->methods[i] = method_impl;
            return;
        }
    }
    assert(!"<TraitMethod> not in <Trait>");
}

TraitImpl* TraitImpl_get(const Type* type, const Trait* trait) {
    for (size_t i = 0; i < POOL_TRAIT_IMPL_COUNT; ++i) {
        TraitImpl* impl = POOL_TRAIT_IMPL[i];
        if (type_eq(impl->type, type) && Trait_equal(impl->trait, trait)) {
            return impl;
        }
    }
    assert(!"No implementation of <Trait> found for this <Type>");
    return NULL;
}

MethodImpl TraitImpl_getMethodImpl(const TraitImpl* impl, const HashStr method_name) {
    for (size_t j = 0; j < impl->trait->method_count; ++j) {
        if (hashed_str_eq(&impl->trait->methods[j].name, &method_name) == true) {
            const MethodImpl method_impl = impl->methods[j];
            if (method_impl == NULL) break;
            return method_impl;
        }
    }
    assert(!"<TraitImplMethod> not found for <TraitImpl>");
    return NULL;
}

// =====================================
// OBJECT
// =====================================

Object* Object_newFrom(Type* type, void* data) {
    Object* obj = malloc(sizeof(Object));
    obj->type = type;
    obj->data = data;
    return obj;
}

Object* Object_new(Type* type) {
    Object* obj = malloc(sizeof(Object));
    obj->type = type;
    obj->data = malloc(type->size);
    memset(obj->data, 0, type->size);
    return obj;
}

MethodImpl Object_getMethod(const Object* obj, const HashStr trait_name, const HashStr method_name) {
    const Trait* trait = Trait_get(trait_name);
    const TraitImpl * impl = TraitImpl_get(obj->type, trait);
    return TraitImpl_getMethodImpl(impl, method_name);
}

void* INTERNAL_Object_call(const Object* obj, const Method* trait_method, const Trait* trait, const TraitImpl* trait_impl, va_list args) {
    const MethodImpl trait_method_impl = TraitImpl_getMethodImpl(trait_impl, trait_method->name);
    const MethodContext ctx = (MethodContext){obj, trait, trait_method};
    return trait_method_impl(ctx, args);
}

void* Object_callStr(const Object* obj, const HashStr trait_name, const HashStr method_name, ... ) {
    const Trait* trait = Trait_get(trait_name);
    const TraitImpl* trait_impl = TraitImpl_get(obj->type, trait);
    const Method* trait_method = Trait_getMethod(trait, method_name);

    va_list args = {0};
    va_start(args, trait_method->param_count);
    void* result = INTERNAL_Object_call(obj, trait_method, trait, trait_impl, args);
    va_end(args);

    return result;
}

void* Object_call(const Object* obj, const Method* method, ...) {
    const Trait* trait = (Trait*) method->trait;
    const TraitImpl* trait_impl = TraitImpl_get(obj->type, trait);

    va_list args = {0};
    va_start(args, method->param_count);
    void* result = INTERNAL_Object_call(obj, method, trait, trait_impl, args);
    va_end(args);

    return result;
}

bool Object_is(const Object* obj, const Type* type) {
    return hashed_str_eq(&obj->type->name,  &type->name);
}

void Object_finalize(Object* obj) {

    if (Type_implement(obj->type, Finalizable)) {
        Object_call(obj, Finalizable_finalize);
    }

    free(obj->data);
    free(obj);
}