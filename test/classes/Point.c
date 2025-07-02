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
	LOG("%s got finalized\n", CTX->object->class->name.str);
	return NULL;
}

void* Point_MethodImpl_Constructable_construct(MethodContext* CTX) {
	TR_METHOD_UNWRAP_START();
	TR_CHECK_TRAIT_STR("Constructable");
	TR_CHECK_METHOD_STR("construct");
	TR_METHOD_UNWRAP_END();
	LOG("%s got constructed\n", CTX->object->class->name.str);
	return NULL;
}

// ===========================================================
// METHODS
// ===========================================================

void Point_initClass(void) {
	clsPoint = TR_CLASS("Point", DataPoint);
	Class_implementMethod(clsPoint, trDescribe.methods.toString, Point_MethodImpl_Describe_toString);
	Class_validateTrait(clsPoint, trDescribe.trait);

	Class_implementMethod(clsPoint, Trait_getMethod(trMove2i.trait, HASH_STR("move")), Point_MethodImpl_Move2i_move);
	Class_implementMethod(clsPoint, Trait_getMethod(trMove2i.trait, HASH_STR("moveX")), Point_MethodImpl_Move2i_moveX);
	Class_implementMethod(clsPoint, Trait_getMethod(trMove2i.trait, HASH_STR("moveY")), Point_MethodImpl_Move2i_moveY);
	Class_validateTrait(clsPoint, trMove2i.trait);

	Class_implementMethod(clsPoint, BuiltIn.traits.Finalizable.methods.finalize, Point_MethodImpl_Finalizable_finalize);
	Class_validateTrait(clsPoint, BuiltIn.traits.Finalizable.trait);

	Class_implementMethod(clsPoint, BuiltIn.traits.Constructable.methods.construct, Point_MethodImpl_Constructable_construct);
	Class_validateTrait(clsPoint, BuiltIn.traits.Constructable.trait);
}

// ===========================================================
// Impl Describe
// ===========================================================

void* Point_MethodImpl_Describe_toString(MethodContext* CTX) {
	const DataPoint* this =	TR_METHOD_UNWRAP_START();
	TR_CHECK_ALL(clsPoint, trDescribe.trait, trDescribe.methods.toString);
	TR_METHOD_UNWRAP_END();

	const int len = snprintf(NULL, 0, "%s(x=%d, y=%d)", CTX->object->class->name.str, this->x, this->y);
	char* buf = malloc(len + 1);
	snprintf(buf, len + 1, "%s(x=%d, y=%d)", CTX->object->class->name.str, this->x, this->y);
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