#include "Vector3f.h"

#include <stdarg.h>
#include <stdio.h>

#include "../common/ErrorHandling.h"
#include "../traits/Describe.h"
#include "../traits/Move3f.h"

Type* type_Vector3f;

// ===========================================================
// METHODS
// ===========================================================

void Vector3f_initType() {
	type_Vector3f = TR_TYPE("Vector3f", Data_Vector3f);
	TraitImpl* traitImpl_Describe_Vector3f = TraitImpl_create(Describe.trait, type_Vector3f);
	TraitImpl_addMethod(traitImpl_Describe_Vector3f, Describe.methods.toString, MethodImpl_Vector3f_Describe_toString);

	TR_USE(TraitImpl, TraitImpl_create(Move3f.trait, type_Vector3f)) {
		TraitImpl_addMethod(it, Move3f.methods.move, MethodImpl_Move3f_Vector3f_move);
		TraitImpl_addMethod(it, Move3f.methods.moveX, MethodImpl_Move3f_Vector3f_moveX);
		TraitImpl_addMethod(it, Move3f.methods.moveY, MethodImpl_Move3f_Vector3f_moveY);
		TraitImpl_addMethod(it, Move3f.methods.moveZ, MethodImpl_Move3f_Vector3f_moveZ);
	}

}

void* MethodImpl_Vector3f_Describe_toString(MethodContext* CTX) {
	const Data_Vector3f* this = METHOD_UNWRAP_START();
	CHECK_ALL_STR("Vector3f", "Describe", "toString");
	METHOD_UNWRAP_END();
	const char* template = "%s(x=%f, y=%f, z=%f)";
	const Type* type = Object_getType(CTX->object);
	const int len = snprintf(NULL, 0, template, type->name.str, this->x, this->y, this->z);
	char* buf = malloc(len + 1);
	snprintf(buf, len + 1, template, type->name.str, this->x, this->y, this->z);
	return buf;
}

void* MethodImpl_Move3f_Vector3f_move(MethodContext* CTX) {
	Data_Vector3f* this = METHOD_UNWRAP_START();
	CHECK_ALL_STR("Vector3f", "Move3f", "move");
	const float deltaX = ARG_UNWRAP(double);
	const float deltaY = ARG_UNWRAP(double);
	const float deltaZ = ARG_UNWRAP(double);
	METHOD_UNWRAP_END();

	this->x += deltaX;
	this->y += deltaY;
	this->z += deltaZ;

	return NULL;
}

void* MethodImpl_Move3f_Vector3f_moveX(MethodContext* CTX) {
	Data_Vector3f* this = METHOD_UNWRAP_START();
	CHECK_ALL_STR("Vector3f", "Move3f", "moveX");
	const float deltaX = ARG_UNWRAP(double);
	METHOD_UNWRAP_END();

	this->x += deltaX;

	return NULL;
}

void* MethodImpl_Move3f_Vector3f_moveY(MethodContext* CTX) {
	Data_Vector3f* this = METHOD_UNWRAP_START();
	CHECK_ALL_STR("Vector3f", "Move3f", "moveY");
	const float deltaY = ARG_UNWRAP(double);
	METHOD_UNWRAP_END();

	this->y += deltaY;

	return NULL;
}

void* MethodImpl_Move3f_Vector3f_moveZ(MethodContext* CTX) {
	Data_Vector3f* this = METHOD_UNWRAP_START();
	CHECK_ALL_STR("Vector3f", "Move3f", "moveZ");
	const float deltaZ = ARG_UNWRAP(double);
	METHOD_UNWRAP_END();

	this->z += deltaZ;

	return NULL;
}