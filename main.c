#include <assert.h>
#include <stdarg.h>
#include <stdio.h>

#include "trait_runtime.h"

typedef struct {
	int x, y;
} Point;

void impl_Point_Display_toString(const MethodCtx* CTX, ...) {
	const Point* this = METHOD_UNWRAP_START("Point", "Display", "toString");

	METHOD_UNWRAP_END();

	printf("%s(x=%d, y=%d)\n", CTX->object->type->name.str, this->x, this->y);
}

void impl_Point_Move2D_move(const MethodCtx* CTX, ...) {
	Point* this = METHOD_UNWRAP_START("Point", "Move2D", "move");

	const int deltaX = ARG_UNWRAP(int);
	const int deltaY = ARG_UNWRAP(int);

	METHOD_UNWRAP_END();

	this->x += deltaX;
	this->y += deltaY;
}

void impl_Point_Move2D_moveX(const MethodCtx* CTX, ...) {
	Point* this = METHOD_UNWRAP_START("Point", "Move2D", "moveX");

	const int deltaX = ARG_UNWRAP(int);

	METHOD_UNWRAP_END();

	this->x += deltaX;
}

void impl_Point_Move2D_moveY(const MethodCtx* CTX, ...) {
	Point* this = METHOD_UNWRAP_START("Point", "Move2D", "moveY");

	const int deltaY = ARG_UNWRAP(int);

	METHOD_UNWRAP_END();

	this->y += deltaY;
}

int main() {
	// Declare Traits
	Trait* trait_Display = TRAIT("Display");
	USE(Trait, trait_Display) {
		TRAIT_ADD_METHOD(this, "toString");
	}

	Trait* trait_Move2D = TRAIT("Move2D");
	USE(Trait, trait_Move2D) {
		TRAIT_ADD_METHOD(this, "move", HASH_STR("x"), HASH_STR("y"));
		TRAIT_ADD_METHOD(this, "moveY", HASH_STR("y"));
		TRAIT_ADD_METHOD(this, "moveX", HASH_STR("x"));
	}

	// Impl Traits
	TraitImpl* traitImpl_Display_Point = TRAIT_IMPL(trait_Display, "Point");
	USE(TraitImpl, traitImpl_Display_Point) {
		TRAIT_IMPL_METHOD(this, "toString", impl_Point_Display_toString);
	}

	TraitImpl* impl_Point_Move2D = TRAIT_IMPL(trait_Move2D, "Point");
	USE(TraitImpl, impl_Point_Move2D) {
		TRAIT_IMPL_METHOD(this, "move", impl_Point_Move2D_move);
		TRAIT_IMPL_METHOD(this, "moveX", impl_Point_Move2D_moveX);
		TRAIT_IMPL_METHOD(this, "moveY", impl_Point_Move2D_moveY);
	}

	// Create instance
	Point* p = malloc(sizeof(Point));
	p->x = 42;
	p->y = 12;
	Object* obj = OBJECT_CREATE("Point", p);

	// Dispatch method
	CALL(obj, "Display", "toString");
	CALL(obj, "Move2D", "move", 10, 12);
	CALL(obj, "Display", "toString");
	CALL(obj, "Move2D", "moveX", -12);
	CALL(obj, "Display", "toString");
	CALL(obj, "Move2D", "moveY", 33);
	CALL(obj, "Display", "toString");
	CALL(obj, "Move2D", "moveY", 33);
	CALL(obj, "Move2D", "move", -1110, 9999);
	CALL(obj, "Display", "toString");

	OBJECT_FINALIZE(obj);
	return 0;
}
