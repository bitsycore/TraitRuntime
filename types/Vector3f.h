#ifndef VECTOR3F_H
#define VECTOR3F_H

#include "../TraitRuntime.h"

extern Type* type_Vector3f;

typedef struct {
	float x, y, z;
} DataVector3f;

void Vector3f_initType();
void* MethodImpl_Vector3f_Format_toString(const MethodContext* CTX, va_list args);
void MethodImpl_Move3f_Vector3f_move(const MethodContext* CTX, va_list args);
void MethodImpl_Move3f_Vector3f_moveX(const MethodContext* CTX, va_list args);
void MethodImpl_Move3f_Vector3f_moveY(const MethodContext* CTX, va_list args);
void MethodImpl_Move3f_Vector3f_moveZ(const MethodContext* CTX, va_list args);

#endif //VECTOR3F_H
