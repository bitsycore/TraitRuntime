#ifndef TYPES_H
#define TYPES_H

#include "Config.h"
#include "String/HashStr.h"

#define FLAG_GET(flags_var, flag_mask) ((flags_var) & (flag_mask))
#define FLAG_SET(flags_var, flag_to_set) ((flags_var) |= (flag_to_set))
#define FLAG_CLEAR(flags_var, flag_to_clear) ((flags_var) &= ~(flag_to_clear))
#define FLAG_TOGGLE(flags_var, flag_to_toggle) ((flags_var) ^= (flag_to_toggle))

struct Trait;
struct MethodContext;

typedef uint64_t TraitId;

typedef struct Method {
    HashStr name;
    HashStr params[MAX_PARAMS_PER_METHODS];
    uint32_t params_count;
    uint32_t param_vararg_at;
    struct Trait *trait;
} Method;

typedef struct Trait {
    TraitId id;
    HashStr name;
    Method methods[MAX_METHODS_PER_TRAITS];
    size_t method_count;
    size_t data_size;
} Trait;

typedef void * (*MethodFunc)(struct MethodContext *CTX);

// TODO REMOVE TRAIT IMPL TO FLATTEN LIST OF LIST
typedef struct TraitImpl {
    Trait *trait;
    MethodFunc methods[MAX_METHODS_PER_TRAITS];
} TraitImpl;

typedef struct Class {
    uint8_t flags;
    HashStr name;
    size_t data_size;
    size_t trait_impl_count;
    TraitImpl traits_impl[MAX_TRAIT_IMPLS];
} Class;

typedef struct Object {
    Class *class;
    void *data;
} Object;

typedef struct MethodContext {
    const Object *object;
    const Method *method;
    va_list *args;
} MethodContext;


#endif //TYPES_H
