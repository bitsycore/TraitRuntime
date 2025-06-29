#ifndef MOVE3F_H
#define MOVE3F_H

#include "../TraitRuntime.h"

typedef struct {
	float x, y, z;
} Move3fData;

extern Trait* trait_Move3f;
extern Method* method_Move3f_move;
extern Method* method_Move3f_moveX;
extern Method* method_Move3f_moveZ;
extern Method* method_Move3f_moveY;

void Move3f_loadTrait();

#endif //MOVE3F_H
