#include "Point.h"

#include <stdarg.h>
#include <stdio.h>

#include <TraitRuntime.h>
#include <TraitRuntime/Commons/Log.h>

#include "../traits/Describe.h"
#include "../traits/Move2i.h"

Class* clsPoint;

// ===========================================================
// Impl Finalizable
// ===========================================================

void* Point_MethodImpl_Finalizable_finalize(MethodContext* CTX) {
	TR_METHOD_UNWRAP_START();
	TR_CHECK_TRAIT_STR("Finalizable");
	TR_CHECK_METHOD_STR("finalize");
	TR_METHOD_UNWRAP_END();
	LOG("%s got finalized\n", Class_getById(CTX->object->type_id)->name.str);
	return NULL;
}

void* Point_MethodImpl_Constructable_construct(MethodContext* CTX) {
	TR_METHOD_UNWRAP_START();
	TR_CHECK_TRAIT_STR("Constructable");
	TR_CHECK_METHOD_STR("construct");
	TR_METHOD_UNWRAP_END();
	LOG("%s got constructed\n", Class_getById(CTX->object->type_id)->name.str);
	return NULL;
}

// ===========================================================
// METHODS
// ===========================================================

void Point_initClass(void) {
	clsPoint = TR_CLASS("Point", DataPoint);
	TraitImpl* traitImpl_Describe_Point = TraitImpl_create(trDescribe.trait, clsPoint);
	TraitImpl_addMethod(traitImpl_Describe_Point, trDescribe.methods.toString, Point_MethodImpl_Describe_toString);

	TR_USE(TraitImpl, TraitImpl_create(trMove2i.trait, clsPoint)) {
		TR_TRAIT_IMPL_METHOD(it, "move", Point_MethodImpl_Move2i_move);
		TR_TRAIT_IMPL_METHOD(it, "moveX", Point_MethodImpl_Move2i_moveX);
		TR_TRAIT_IMPL_METHOD(it, "moveY", Point_MethodImpl_Move2i_moveY);
	}

	TraitImpl* traitImpl_Finalizable_Point = TraitImpl_create(BuiltIn.traits.Finalizable.trait, clsPoint);
	TraitImpl_addMethod(traitImpl_Finalizable_Point, BuiltIn.traits.Finalizable.methods.finalize, Point_MethodImpl_Finalizable_finalize);

	TraitImpl* traitImpl_Constructable_Point = TraitImpl_create(BuiltIn.traits.Constructable.trait, clsPoint);
	TraitImpl_addMethod(traitImpl_Constructable_Point, BuiltIn.traits.Constructable.methods.construct, Point_MethodImpl_Constructable_construct);
}

// ===========================================================
// Impl Describe
// ===========================================================

void* Point_MethodImpl_Describe_toString(MethodContext* CTX) {
	const DataPoint* this =	TR_METHOD_UNWRAP_START();
	TR_CHECK_ALL(clsPoint, trDescribe.trait, trDescribe.methods.toString);
	TR_METHOD_UNWRAP_END();

	const int len = snprintf(NULL, 0, "%s(x=%d, y=%d)", Class_getById(CTX->object->type_id)->name.str, this->x, this->y);
	char* buf = malloc(len + 1);
	snprintf(buf, len + 1, "%s(x=%d, y=%d)", Class_getById(CTX->object->type_id)->name.str, this->x, this->y);
	return buf;
}

// ===========================================================
// Impl Move2i
// ===========================================================

void* Point_MethodImpl_Move2i_move(MethodContext* CTX) {
	DataPoint* this = TR_METHOD_UNWRAP_START();
	TR_CHECK_ALL(clsPoint, trMove2i.trait, trMove2i.methods.move);
	const int deltaX = ARG_UNWRAP(int);
	const int deltaY = ARG_UNWRAP(int);
	TR_METHOD_UNWRAP_END();

	this->x += deltaX;
	this->y += deltaY;

	return NULL;
}

void* Point_MethodImpl_Move2i_moveX(MethodContext* CTX) {
	DataPoint* this = TR_METHOD_UNWRAP_START();
	TR_CHECK_ALL_STR("Point", "Move2i", "moveX");
	const int deltaX = ARG_UNWRAP(int);
	TR_METHOD_UNWRAP_END();

	this->x += deltaX;

	return NULL;
}

void* Point_MethodImpl_Move2i_moveY(MethodContext* CTX) {
	DataPoint* this = TR_METHOD_UNWRAP_START();
	TR_CHECK_ALL_STR("Point", "Move2i", "moveY");
	const int deltaY = ARG_UNWRAP(int);
	TR_METHOD_UNWRAP_END();

	this->y += deltaY;

	return NULL;
}