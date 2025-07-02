#ifndef BUILTIN_H
#define BUILTIN_H

#include "Types.h"

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
    } classes;
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

#endif //BUILTIN_H
