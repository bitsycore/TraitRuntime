#include "Point.h"

#include <stdarg.h>
#include <stdio.h>

#include "../common/Log.h"
#include "../traits/Describe.h"
#include "../traits/Move2i.h"

Type* type_Point;

// ===========================================================
// Impl Finalizable
// ===========================================================

void* Point_MethodImpl_Finalizable_finalize(MethodContext* CTX) {
	METHOD_UNWRAP_START();
	CHECK_TRAIT_STR("Finalizable");
	CHECK_METHOD_STR("finalize");
	METHOD_UNWRAP_END();
	LOG("%s got finalized\n", Type_getById(CTX->object->type_id)->name.str);
	return NULL;
}

void* Point_MethodImpl_Constructable_construct(MethodContext* CTX) {
	METHOD_UNWRAP_START();
	CHECK_TRAIT_STR("Constructable");
	CHECK_METHOD_STR("construct");
	METHOD_UNWRAP_END();
	LOG("%s got constructed\n", Type_getById(CTX->object->type_id)->name.str);
	return NULL;
}

// ===========================================================
// METHODS
// ===========================================================

void Point_initType(void) {
	type_Point = TR_TYPE("Point", DataPoint);
	TraitImpl* traitImpl_Describe_Point = TraitImpl_create(Describe.trait, type_Point);
	TraitImpl_addMethod(traitImpl_Describe_Point, Describe.methods.toString, Point_MethodImpl_Describe_toString);

	TR_USE(TraitImpl, TraitImpl_create(Move2i.trait, type_Point)) {
		TRAIT_IMPL_METHOD(it, "move", Point_MethodImpl_Move2i_move);
		TRAIT_IMPL_METHOD(it, "moveX", Point_MethodImpl_Move2i_moveX);
		TRAIT_IMPL_METHOD(it, "moveY", Point_MethodImpl_Move2i_moveY);
	}

	TraitImpl* traitImpl_Finalizable_Point = TraitImpl_create(BuiltIn.traits.Finalizable.trait, type_Point);
	TraitImpl_addMethod(traitImpl_Finalizable_Point, BuiltIn.traits.Finalizable.methods.finalize, Point_MethodImpl_Finalizable_finalize);

	TraitImpl* traitImpl_Constructable_Point = TraitImpl_create(BuiltIn.traits.Constructable.trait, type_Point);
	TraitImpl_addMethod(traitImpl_Constructable_Point, BuiltIn.traits.Constructable.methods.construct, Point_MethodImpl_Constructable_construct);
}

// ===========================================================
// Impl Describe
// ===========================================================

void* Point_MethodImpl_Describe_toString(MethodContext* CTX) {
	const DataPoint* this =	METHOD_UNWRAP_START();
	CHECK_ALL(type_Point, Describe.trait, Describe.methods.toString);
	METHOD_UNWRAP_END();

	const int len = snprintf(NULL, 0, "%s(x=%d, y=%d)", Type_getById(CTX->object->type_id)->name.str, this->x, this->y);
	char* buf = malloc(len + 1);
	snprintf(buf, len + 1, "%s(x=%d, y=%d)", Type_getById(CTX->object->type_id)->name.str, this->x, this->y);
	return buf;
}

// ===========================================================
// Impl Move2i
// ===========================================================

void* Point_MethodImpl_Move2i_move(MethodContext* CTX) {
	DataPoint* this = METHOD_UNWRAP_START();
	CHECK_ALL(type_Point, Move2i.trait, Move2i.methods.move);
	const int deltaX = ARG_UNWRAP(int);
	const int deltaY = ARG_UNWRAP(int);
	METHOD_UNWRAP_END();

	this->x += deltaX;
	this->y += deltaY;

	return NULL;
}

void* Point_MethodImpl_Move2i_moveX(MethodContext* CTX) {
	DataPoint* this = METHOD_UNWRAP_START();
	CHECK_ALL_STR("Point", "Move2i", "moveX");
	const int deltaX = ARG_UNWRAP(int);
	METHOD_UNWRAP_END();

	this->x += deltaX;

	return NULL;
}

void* Point_MethodImpl_Move2i_moveY(MethodContext* CTX) {
	DataPoint* this = METHOD_UNWRAP_START();
	CHECK_ALL_STR("Point", "Move2i", "moveY");
	const int deltaY = ARG_UNWRAP(int);
	METHOD_UNWRAP_END();

	this->y += deltaY;

	return NULL;
}