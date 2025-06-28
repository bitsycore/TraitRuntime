#include "trait_runtime.h"

#include <assert.h>
#include <stdbool.h>
#include <string.h>

TraitImpl* pool_trait_impl[MAX_TRAIT_IMPLS];
size_t pool_trait_impl_count = 0;
Trait* pool_trait[MAX_TRAITS];
size_t pool_trait_count = 0;

Trait* trait_create(const HashedStr name) {
    Trait* t = malloc(sizeof(Trait));
    t->name = name;
    t->method_count = 0;
    assert(pool_trait_count < MAX_TRAITS);
    pool_trait[pool_trait_count++] = t;
    return t;
}

void trait_add_method(Trait* trait, const HashedStr method_name, const HashedStr* param_types, const size_t param_count) {
    assert(trait->method_count < MAX_METHODS);
    TraitMethod* m = &trait->methods[trait->method_count++];
    m->name = method_name;
    m->param_count = param_count;
    for (size_t i = 0; i < param_count; ++i)
        m->param_types[i] = param_types[i];
}

Trait* trait_get(const HashedStr name) {
    for (size_t i = 0; i < pool_trait_count; ++i)
        if (hashed_str_eq(&pool_trait[i]->name, &name) == true)
            return pool_trait[i];
    return NULL;
}

static void register_trait_impl(TraitImpl* impl) {
    assert(pool_trait_impl_count < MAX_TRAIT_IMPLS);
    pool_trait_impl[pool_trait_impl_count++] = impl;
}

TraitImpl* trait_impl_create(Trait* trait, const HashedStr type_name) {
    TraitImpl* impl = malloc(sizeof(TraitImpl));
    impl->name = type_name;
    impl->trait = trait;
    memset(impl->methods, 0, sizeof(impl->methods));
    register_trait_impl(impl);
    return impl;
}

void trait_impl_add_method(TraitImpl* impl, const HashedStr method_name, const TraitFn fn) {
    for (size_t i = 0; i < impl->trait->method_count; ++i) {
        if (hashed_str_eq(&impl->trait->methods[i].name, &method_name) == true) {
            impl->methods[i] = fn;
            return;
        }
    }
    assert(0 && "Method not found in trait definition");
}



TraitFn resolve_method(const Object* obj, const HashedStr trait_name, const HashedStr method_name) {
    for (size_t i = 0; i < pool_trait_impl_count; ++i) {
        const TraitImpl* impl = pool_trait_impl[i];
        if (hashed_str_eq(&impl->name, &obj->type->name) == true &&
            hashed_str_eq(&impl->trait->name, &trait_name) == true) {
            for (size_t j = 0; j < impl->trait->method_count; ++j)
                if (hashed_str_eq(&impl->trait->methods[j].name, &method_name) == true)
                    return impl->methods[j];
        }
    }
    assert(0 && "Trait or method not found for object type");
}

TraitMethod* trait_get_method(const HashedStr trait_name, const HashedStr method_name)
{
    for (size_t i = 0; i < pool_trait_count; ++i)
    {
        const Trait* impl = pool_trait[i];
        if (hashed_str_eq(&trait_name, &impl->name) == true)
        {
            for (size_t j = 0; j < MAX_METHODS; ++j)
            {
                const TraitMethod* m = &impl->methods[j];
                if (hashed_str_eq(&m->name, &method_name) == true)
                    return (TraitMethod*)m;
            }
        }
    }
    assert(0 && "Trait or method not found for object type");
}

Object* object_create(const HashedStr type_name, void* instance) {
    Object* obj = malloc(sizeof(Object));
    Type* type = malloc(sizeof(Type));
    obj->type = type;
    obj->type->name = type_name;
    obj->instance = instance;
    return obj;
}

void object_destroy(Object* obj) {
    free(obj->instance);
    free(obj);
}
