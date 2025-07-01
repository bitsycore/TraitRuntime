#ifndef DESCRIBE_H
#define DESCRIBE_H

#include "../../TraitRuntime/TraitRuntime.h"

typedef struct {
    Trait* trait;
    struct {
        Method* toString;
    } methods;
} Container_Describe;

extern Container_Describe Describe;

void Describe_loadTrait();

#endif //DESCRIBE_H
