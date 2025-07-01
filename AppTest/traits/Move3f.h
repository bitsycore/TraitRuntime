#ifndef MOVE3F_H
#define MOVE3F_H

#include "../../TraitRuntime/TraitRuntime.h"

typedef struct {
	Trait* trait;
	struct {
		Method* move;
		Method* moveX;
		Method* moveY;
		Method* moveZ;
	} methods;
} Container_Move3f;

extern Container_Move3f Move3f;

void Move3f_loadTrait();

#endif //MOVE3F_H
