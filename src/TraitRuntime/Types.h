#ifndef TYPES_H
#define TYPES_H

#include "Config.h"
#include "String/HashStr.h"

struct Trait;
struct MethodContext;

typedef size_t ClassId;
typedef size_t TraitId;

typedef struct Method {
    HashStr name;
    HashStr params[MAX_PARAMS_PER_METHODS];
    size_t params_count;
    size_t param_vararg_at;
    struct Trait *trait;
} Method;

typedef struct Trait {
    TraitId id;
    HashStr name;
    Method methods[MAX_METHODS_PER_TRAITS];
    size_t method_count;
    size_t data_size;
} Trait;

typedef void * (*MethodImpl)(struct MethodContext *CTX);

typedef struct TraitImpl {
    Trait *trait;
    MethodImpl methods[MAX_METHODS_PER_TRAITS];
} TraitImpl;

typedef struct Class {
    HashStr name;
    ClassId id;
    size_t size;
    size_t trait_impl_count;
    TraitImpl traits_impl[MAX_TRAIT_IMPLS];
} Class;

typedef struct Object {
    Class *class;
    void *data;
} Object;

typedef struct MethodContext {
    const Object *object;
    const Trait *trait;
    const Method *method;
    va_list *args;
} MethodContext;


#endif //TYPES_H
