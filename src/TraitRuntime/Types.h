#ifndef TYPES_H
#define TYPES_H

#include "Config.h"
#include "Container/HashTable.h"
#include "String/HashStr.h"

struct Trait;
struct MethodContext;

typedef void * (*MethodFunc)(struct MethodContext *CTX);

typedef struct MethodDef {
    HashStr name;
    HashStr params[MAX_PARAMS_PER_METHODS];
    uint32_t params_count;
    uint32_t param_vararg_at;
    struct Trait *trait;
} MethodDef;

typedef struct Trait {
    HashStr name;
    MethodDef methods[MAX_METHODS_PER_TRAITS];
    size_t method_count;
    size_t data_size;
} Trait;

typedef struct MethodImpl {
    MethodDef *method_def;
    MethodFunc method_func;
} MethodImpl;

HT_DEF_TYPED_NODE(Trait)
HT_DEF_TYPED_NODE(MethodImpl)

typedef struct Class {
    uint8_t flags;
    HashStr name;
    size_t data_size;
    HT_NODE(Trait) traits_impl[TRAIT_IMPL_HASHTABLE_SIZE];
    HT_NODE(MethodImpl) methods_impl[METHODS_IMPL_HASHTABLE_SIZE];
} Class;

typedef struct Object {
    Class *class;
    void *data;
} Object;

typedef struct MethodContext {
    const Object *object;
    const MethodDef *method_def;
    va_list *args;
} MethodContext;

#endif //TYPES_H
