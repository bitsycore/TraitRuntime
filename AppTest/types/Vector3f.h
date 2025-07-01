#ifndef VECTOR3F_H
#define VECTOR3F_H

#include "../../TraitRuntime/TraitRuntime.h"

extern Type* type_Vector3f;

typedef struct {
    Type* type;
} Vector3f;

typedef struct {
    float x, y, z;
} Data_Vector3f;

void Vector3f_initType();
void* MethodImpl_Vector3f_Describe_toString(MethodContext* CTX);
void* MethodImpl_Move3f_Vector3f_move(MethodContext* CTX);
void* MethodImpl_Move3f_Vector3f_moveX(MethodContext* CTX);
void* MethodImpl_Move3f_Vector3f_moveY(MethodContext* CTX);
void* MethodImpl_Move3f_Vector3f_moveZ(MethodContext* CTX);

#endif //VECTOR3F_H
