#ifndef DESCRIBE_H
#define DESCRIBE_H

#include <TraitRuntime.h>

typedef struct {
    Trait* trait;
    struct {
        Method* toString;
    } methods;
} Container_Describe;

extern Container_Describe trDescribe;

void Describe_loadTrait();

#endif //DESCRIBE_H
