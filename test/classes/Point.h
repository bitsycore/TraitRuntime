#ifndef POINT_H
#define POINT_H

#include <TraitRuntime.h>

extern Class* clsPoint;

typedef struct {
	int x, y;
} DataPoint;

void Point_initClass(void);

void* Point_MethodImpl_Describe_toString(MethodContext* CTX);

void* Point_MethodImpl_Finalizable_finalize(MethodContext* CTX);

void* Point_MethodImpl_Move2i_move(MethodContext* CTX);
void* Point_MethodImpl_Move2i_moveX(MethodContext* CTX);
void* Point_MethodImpl_Move2i_moveY(MethodContext* CTX);

#endif //POINT_H
