#ifndef DISPLAY_H
#define DISPLAY_H

#include "../TraitRuntime.h"

typedef struct {
    Trait* trait;
    struct {
        Method* toString;
    } methods;
} Container_Describe;

extern Container_Describe Describe;

void Describe_loadTrait();

#endif //DISPLAY_H
