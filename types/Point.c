#include "Point.h"

#include <stdarg.h>
#include <stdio.h>

#include "../Log.h"
#include "../traits/Format.h"
#include "../traits/Move2i.h"

Type* type_Point;

// ===========================================================
// Impl Finalizable
// ===========================================================

void Point_MethodImpl_Finalizable_finalize(const MethodContext* CTX) {
	METHOD_UNWRAP_START();
	CHECK_TRAIT_STR("Finalizable");
	CHECK_METHOD_STR("finalize");
	METHOD_UNWRAP_END();
	LOG("%s got finalized\n", Type_getById(CTX->object->type_id)->name.str);
}

void Point_MethodImpl_Constructable_construct(const MethodContext* CTX, va_list args) {
	METHOD_UNWRAP_START();
	CHECK_TRAIT_STR("Constructable");
	CHECK_METHOD_STR("construct");
	METHOD_UNWRAP_END();
	LOG("%s got constructed\n", Type_getById(CTX->object->type_id)->name.str);
}

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

	TraitImpl* traitImpl_Constructable_Point = TraitImpl_create(BuiltIn.traits.Constructable.trait, type_Point);
	TraitImpl_addMethod(traitImpl_Constructable_Point, BuiltIn.traits.Constructable.methods.construct, Point_MethodImpl_Constructable_construct);
}

// ===========================================================
// Impl Format
// ===========================================================

void* Point_MethodImpl_Format_toString(const MethodContext* CTX, va_list args) {
	const DataPoint* this =	METHOD_UNWRAP_START();
	CHECK_ALL(type_Point, trait_Format, method_Format_toString);
	METHOD_UNWRAP_END();

	const int len = snprintf(NULL, 0, "%s(x=%d, y=%d)", Type_getById(CTX->object->type_id)->name.str, this->x, this->y);
	char* buf = malloc(len + 1);
	snprintf(buf, len + 1, "%s(x=%d, y=%d)", Type_getById(CTX->object->type_id)->name.str, this->x, this->y);
	return buf;
}

// ===========================================================
// Impl Move2i
// ===========================================================

void Point_MethodImpl_Move2i_move(const MethodContext* CTX, va_list args) {
	DataPoint* this = METHOD_UNWRAP_START();
	CHECK_ALL(type_Point, trait_Move2i, method_Move2i_move);
	const int deltaX = ARG_UNWRAP(int);
	const int deltaY = ARG_UNWRAP(int);
	METHOD_UNWRAP_END();

	this->x += deltaX;
	this->y += deltaY;
}

void Point_MethodImpl_Move2i_moveX(const MethodContext* CTX, va_list args) {
	DataPoint* this = METHOD_UNWRAP_START();
	CHECK_ALL_STR("Point", "Move2i", "moveX");
	const int deltaX = ARG_UNWRAP(int);
	METHOD_UNWRAP_END();

	this->x += deltaX;
}

void Point_MethodImpl_Move2i_moveY(const MethodContext* CTX, va_list args) {
	DataPoint* this = METHOD_UNWRAP_START();
	CHECK_ALL_STR("Point", "Move2i", "moveY");
	const int deltaY = ARG_UNWRAP(int);
	METHOD_UNWRAP_END();

	this->y += deltaY;
}