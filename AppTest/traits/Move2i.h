#ifndef MOVE2I_H
#define MOVE2I_H

#include "../../TraitRuntime/TraitRuntime.h"

typedef struct {
    Trait* trait;
    struct {
        Method* move;
        Method* moveX;
        Method* moveY;
    } methods;
} Container_Move2i;

extern Container_Move2i Move2i;

void Move2i_loadTrait();

#endif //MOVE2I_H
