#include "Vector3f.h"

#include "../traits/Format.h"
#include "../traits/Move3f.h"

Type* type_Vector3f;

// ===========================================================
// METHODS
// ===========================================================

void Vector3f_initType() {
	type_Vector3f = TYPE("Vector3f", DataVector3f);
	TraitImpl* traitImpl_Format_Vector3f = TraitImpl_create(trait_Format, type_Vector3f);
	TraitImpl_addMethod(traitImpl_Format_Vector3f, method_Format_toString, MethodImpl_Vector3f_Format_toString);

	TraitImpl* traitImpl_Move3f_Vector3f = TraitImpl_create(trait_Move3f, type_Vector3f);
	TraitImpl_addMethod(traitImpl_Move3f_Vector3f, method_Move3f_move, MethodImpl_Move3f_Vector3f_move);
	TraitImpl_addMethod(traitImpl_Move3f_Vector3f, method_Move3f_moveX, MethodImpl_Move3f_Vector3f_moveX);
	TraitImpl_addMethod(traitImpl_Move3f_Vector3f, method_Move3f_moveY, MethodImpl_Move3f_Vector3f_moveY);
	TraitImpl_addMethod(traitImpl_Move3f_Vector3f, method_Move3f_moveZ, MethodImpl_Move3f_Vector3f_moveZ);
}

void* MethodImpl_Vector3f_Format_toString(const MethodContext* CTX, va_list args) {
	const DataVector3f* this = METHOD_UNWRAP_START("Vector3f", "Format", "toString");
	METHOD_UNWRAP_END();
	const char* template = "%s(x=%f, y=%f, z=%f)";
	const int len = snprintf(NULL, 0, template, CTX->object->type->name.str, this->x, this->y, this->z);
	char* buf = malloc(len + 1);
	snprintf(buf, len + 1, template, CTX->object->type->name.str, this->x, this->y, this->z);
	return buf;
}

void MethodImpl_Move3f_Vector3f_move(const MethodContext* CTX, va_list args) {
	DataVector3f* this = METHOD_UNWRAP_START("Vector3f", "Move3f", "move");

	const float deltaX = ARG_UNWRAP(double);
	const float deltaY = ARG_UNWRAP(double);
	const float deltaZ = ARG_UNWRAP(double);

	METHOD_UNWRAP_END();

	this->x += deltaX;
	this->y += deltaY;
	this->z += deltaZ;
}

void MethodImpl_Move3f_Vector3f_moveX(const MethodContext* CTX, va_list args) {
	DataVector3f* this = METHOD_UNWRAP_START("Vector3f", "Move3f", "moveX");

	const float deltaX = ARG_UNWRAP(double);

	METHOD_UNWRAP_END();

	this->x += deltaX;
}

void MethodImpl_Move3f_Vector3f_moveY(const MethodContext* CTX, va_list args) {
	DataVector3f* this = METHOD_UNWRAP_START("Vector3f", "Move3f", "moveY");

	const float deltaY = ARG_UNWRAP(double);

	METHOD_UNWRAP_END();

	this->y += deltaY;
}

void MethodImpl_Move3f_Vector3f_moveZ(const MethodContext* CTX, va_list args) {
	DataVector3f* this = METHOD_UNWRAP_START("Vector3f", "Move3f", "moveZ");

	const float deltaZ = ARG_UNWRAP(double);

	METHOD_UNWRAP_END();

	this->z += deltaZ;
}