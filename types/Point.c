#include "Point.h"

#include <stdarg.h>
#include <stdio.h>

#include "../traits/Format.h"
#include "../traits/Move2i.h"

Type* type_Point;

// ===========================================================
// METHODS
// ===========================================================

void Point_initType(void) {
	type_Point = TYPE("Point", DataPoint);
	TraitImpl* traitImpl_Format_Point = TraitImpl_create(trait_Format, type_Point);
	TraitImpl_addMethod(traitImpl_Format_Point, method_Format_toString, Point_MethodImpl_Format_toString);

	TraitImpl* traitImpl_Move2i_Point = TraitImpl_create(trait_Move2i, type_Point);
	TRAIT_IMPL_METHOD(traitImpl_Move2i_Point, "move", Point_MethodImpl_Move2i_move);
	TRAIT_IMPL_METHOD(traitImpl_Move2i_Point, "moveX", Point_MethodImpl_Move2i_moveX);
	TRAIT_IMPL_METHOD(traitImpl_Move2i_Point, "moveY", Point_MethodImpl_Move2i_moveY);

	TraitImpl* traitImpl_Finalizable_Point = TraitImpl_create(BuiltIn.traits.Finalizable.trait, type_Point);
	TraitImpl_addMethod(traitImpl_Finalizable_Point, BuiltIn.traits.Finalizable.methods.finalize, Point_MethodImpl_Finalizable_finalize);
}

// ===========================================================
// Impl Format
// ===========================================================

void* Point_MethodImpl_Format_toString(const MethodContext* CTX, va_list args) {
	const DataPoint* this = METHOD_UNWRAP_START("Point", "Format", "toString");
	METHOD_UNWRAP_END();
	const int len = snprintf(NULL, 0, "%s(x=%d, y=%d)", CTX->object->type->name.str, this->x, this->y);
	char* buf = malloc(len + 1);
	snprintf(buf, len + 1, "%s(x=%d, y=%d)", CTX->object->type->name.str, this->x, this->y);
	return buf;
}

// ===========================================================
// Impl Finalizable
// ===========================================================

void Point_MethodImpl_Finalizable_finalize(const MethodContext* CTX, va_list args) {
	METHOD_UNWRAP_START("Point", "Finalizable", "finalize");
	METHOD_UNWRAP_END();
	printf("%s got freed", CTX->object->type->name.str);
}

// ===========================================================
// Impl Move2i
// ===========================================================

void Point_MethodImpl_Move2i_move(const MethodContext* CTX, va_list args) {
	DataPoint* this = METHOD_UNWRAP_START("Point", "Move2i", "move");

	const int deltaX = ARG_UNWRAP(int);
	const int deltaY = ARG_UNWRAP(int);

	METHOD_UNWRAP_END();

	this->x += deltaX;
	this->y += deltaY;
}

void Point_MethodImpl_Move2i_moveX(const MethodContext* CTX, va_list args) {
	DataPoint* this = METHOD_UNWRAP_START("Point", "Move2i", "moveX");

	const int deltaX = ARG_UNWRAP(int);

	METHOD_UNWRAP_END();

	this->x += deltaX;
}

void Point_MethodImpl_Move2i_moveY(const MethodContext* CTX, va_list args) {
	DataPoint* this = METHOD_UNWRAP_START("Point", "Move2i", "moveY");

	const int deltaY = ARG_UNWRAP(int);

	METHOD_UNWRAP_END();

	this->y += deltaY;
}