#ifndef POINT_H
#define POINT_H

#include "../TraitRuntime.h"

extern Type* type_Point;

typedef struct {
	int x, y;
} DataPoint;

void Point_initType(void);

void* Point_MethodImpl_Format_toString(const MethodContext* CTX, va_list args);

void Point_MethodImpl_Finalizable_finalize(const MethodContext* CTX, va_list args);

void Point_MethodImpl_Move2i_move(const MethodContext* CTX, va_list args);
void Point_MethodImpl_Move2i_moveX(const MethodContext* CTX, va_list args);
void Point_MethodImpl_Move2i_moveY(const MethodContext* CTX, va_list args);

#endif //POINT_H
