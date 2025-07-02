#ifndef MOVE3F_H
#define MOVE3F_H

#include <TraitRuntime.h>

typedef struct {
	Trait* trait;
	struct {
		MethodDef* move;
		MethodDef* moveX;
		MethodDef* moveY;
		MethodDef* moveZ;
	} methods;
} Container_Move3f;

extern Container_Move3f trMove3f;

void Move3f_loadTrait();

#endif //MOVE3F_H
