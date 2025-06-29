#include <assert.h>
#include <stdarg.h>
#include <stdio.h>

#include "trait_runtime.h"

// ===========================================================
// POINT
// ===========================================================

typedef struct {
	int x, y;
} DataPoint;

void* MethodImpl_Point_Display_toString(const MethodContext* CTX, va_list args) {
	const DataPoint* this = METHOD_UNWRAP_START("Point", "Display", "toString");
	METHOD_UNWRAP_END();
	const int len = snprintf(NULL, 0, "%s(x=%d, y=%d)", CTX->object->type->name.str, this->x, this->y);
	char* buf = malloc(len + 1);
	snprintf(buf, len + 1, "%s(x=%d, y=%d)", CTX->object->type->name.str, this->x, this->y);
	return buf;
}

void MethodImpl_Point_Move2i_move(const MethodContext* CTX, const va_list args) {
	DataPoint* this = METHOD_UNWRAP_START("Point", "Move2i", "move");

	const int deltaX = ARG_UNWRAP(int);
	const int deltaY = ARG_UNWRAP(int);

	METHOD_UNWRAP_END();

	this->x += deltaX;
	this->y += deltaY;
}

void MethodImpl_Point_Move2i_moveX(const MethodContext* CTX, const va_list args) {
	DataPoint* this = METHOD_UNWRAP_START("Point", "Move2i", "moveX");

	const int deltaX = ARG_UNWRAP(int);

	METHOD_UNWRAP_END();

	this->x += deltaX;
}

void MethodImpl_Point_Move2i_moveY(const MethodContext* CTX, const va_list args) {
	DataPoint* this = METHOD_UNWRAP_START("Point", "Move2i", "moveY");

	const int deltaY = ARG_UNWRAP(int);

	METHOD_UNWRAP_END();

	this->y += deltaY;
}

// ===========================================================
// VECTOR3F
// ===========================================================

typedef struct {
	float x, y, z;
} DataVector3f;

void* MethodImpl_Vector3f_Display_toString(const MethodContext* CTX, va_list args) {
	const DataVector3f* this = METHOD_UNWRAP_START("Vector3f", "Display", "toString");
	METHOD_UNWRAP_END();
	const char* template = "%s(x=%f, y=%f, z=%f)";
	const int len = snprintf(NULL, 0, template, CTX->object->type->name.str, this->x, this->y, this->z);
	char* buf = malloc(len + 1);
	snprintf(buf, len + 1, template, CTX->object->type->name.str, this->x, this->y, this->z);
	return buf;
}

void MethodImpl_Vector3f_Move3f_move(const MethodContext* CTX, const va_list args) {
	DataVector3f* this = METHOD_UNWRAP_START("Vector3f", "Move3f", "move");

	const float deltaX = ARG_UNWRAP(double);
	const float deltaY = ARG_UNWRAP(double);
	const float deltaZ = ARG_UNWRAP(double);

	METHOD_UNWRAP_END();

	this->x += deltaX;
	this->y += deltaY;
	this->z += deltaZ;
}

void MethodImpl_Vector3f_Move3f_moveX(const MethodContext* CTX, const va_list args) {
	DataVector3f* this = METHOD_UNWRAP_START("Vector3f", "Move3f", "moveX");

	const float deltaX = ARG_UNWRAP(double);

	METHOD_UNWRAP_END();

	this->x += deltaX;
}

void MethodImpl_Vector3f_Move3f_moveY(const MethodContext* CTX, const va_list args) {
	DataVector3f* this = METHOD_UNWRAP_START("Vector3f", "Move3f", "moveY");

	const float deltaY = ARG_UNWRAP(double);

	METHOD_UNWRAP_END();

	this->y += deltaY;
}

void MethodImpl_Vector3f_Move3f_moveZ(const MethodContext* CTX, const va_list args) {
	DataVector3f* this = METHOD_UNWRAP_START("Vector3f", "Move3f", "moveZ");

	const float deltaZ = ARG_UNWRAP(double);

	METHOD_UNWRAP_END();

	this->z += deltaZ;
}

void Display_print(const Object* this) {
	const char* objToString = CALL(this, "Display", "toString");
	printf("toString: %s\n", objToString);
	free((void*)objToString);
	objToString = NULL;
}

// ===========================================================
// UINT64
// ===========================================================

void* MethodImpl_UInt64_Display_toString(const MethodContext* CTX, va_list args) {
	const uint64_t this = *(uint64_t*) METHOD_UNWRAP_START("UInt64", "Display", "toString");
	METHOD_UNWRAP_END();
	const char* template = "%s(%llu)";
	const int len = snprintf(NULL, 0, template, CTX->object->type->name.str, this);
	char* buf = malloc(len + 1);
	snprintf(buf, len + 1, template, CTX->object->type->name.str, this);
	return buf;
}

int main() {
	TraitRuntime_init();

	// =====================================================================
	// Create Type
	Type* type_Vector3f = TYPE("Vector3f", DataVector3f);
	Type* type_Point = TYPE("Point", DataPoint);

	// =====================================================================
	// Create Traits
	Trait* trait_Display = TRAIT("Display");
	const Method* method_Display_toString = TRAIT_ADD_METHOD(trait_Display, "toString");

	Trait* trait_Move2i = TRAIT("Move2i");
	USE(Trait, trait_Move2i) {
		TRAIT_ADD_METHOD(it, "move", PARAM("x"), PARAM("y"));
		TRAIT_ADD_METHOD(it, "moveY", PARAM("y"));
		TRAIT_ADD_METHOD(it, "moveX", PARAM("x"));
	}

	Trait* trait_Move3f = TRAIT("Move3f");
	USE(Trait, trait_Move3f) {
		TRAIT_ADD_METHOD(it, "move", PARAM("x"), PARAM("y"), PARAM("z"));
		TRAIT_ADD_METHOD(it, "moveY", PARAM("y"));
		TRAIT_ADD_METHOD(it, "moveX", PARAM("x"));
		TRAIT_ADD_METHOD(it, "moveZ", PARAM("z"));
	}

	// =====================================================================
	// Impl Traits for Point
	TraitImpl* traitImpl_Display_Point = TRAIT_IMPL("Display", "Point");
	USE(TraitImpl, traitImpl_Display_Point) {
		TraitImpl_addMethod(it, method_Display_toString, MethodImpl_Point_Display_toString);
	}

	TraitImpl* traitImpl_Point_Move2i = TRAIT_IMPL("Move2i", "Point");
	TRAIT_IMPL_METHOD(traitImpl_Point_Move2i, "move", MethodImpl_Point_Move2i_move);
	TRAIT_IMPL_METHOD(traitImpl_Point_Move2i, "moveX", MethodImpl_Point_Move2i_moveX);
	TRAIT_IMPL_METHOD(traitImpl_Point_Move2i, "moveY", MethodImpl_Point_Move2i_moveY);

	// =====================================================================
	// Impl Traits for Vector3F
	TraitImpl* traitImpl_Display_Vector3f = TraitImpl_create(trait_Display, type_Vector3f);
	TRAIT_IMPL_METHOD(traitImpl_Display_Vector3f, "toString", MethodImpl_Vector3f_Display_toString);

	TraitImpl* traitImpl_Move3f_Vector3f = TraitImpl_create(trait_Move3f, type_Vector3f);
	TRAIT_IMPL_METHOD(traitImpl_Move3f_Vector3f, "move", MethodImpl_Vector3f_Move3f_move);
	TRAIT_IMPL_METHOD(traitImpl_Move3f_Vector3f, "moveX", MethodImpl_Vector3f_Move3f_moveX);
	TRAIT_IMPL_METHOD(traitImpl_Move3f_Vector3f, "moveY", MethodImpl_Vector3f_Move3f_moveY);
	TRAIT_IMPL_METHOD(traitImpl_Move3f_Vector3f, "moveZ", MethodImpl_Vector3f_Move3f_moveZ);

	// =====================================================================
	// Impl Traits for Uint64
	TraitImpl* traitImpl_Display_UInt64 = TraitImpl_create(trait_Display, UInt64);
	TRAIT_IMPL_METHOD(traitImpl_Display_UInt64, "toString", MethodImpl_UInt64_Display_toString);

	// =====================================================================
	// Create Object
	Object* point = Object_new(type_Point);
	printf("Object is of type: %s\n", point->type->name.str);
	printf("Object data size: %zu\n", point->type->size);
	printf("Point is Display = %d\n", Type_implement(point->type, trait_Display));
	printf("Point is Move2i = %d\n", Type_implement(point->type, trait_Move2i));
	printf("Point is Move3f = %d\n", Type_implement(point->type, trait_Move3f));

	Object* vector3f = Object_new(type_Vector3f);
	printf("Object is of type: %s\n", vector3f->type->name.str);
	printf("Object data size: %zu\n", vector3f->type->size);
	printf("Point is Display = %d\n", Type_implement(vector3f->type, trait_Display));
	printf("Point is Move2i = %d\n", Type_implement(vector3f->type, trait_Move2i));
	printf("Point is Move3f = %d\n", Type_implement(vector3f->type, trait_Move3f));

	Object* uint64 = Object_new(UInt64);
	*(uint64_t*)uint64->data = 512;
	printf("Object is of type: %s\n", uint64->type->name.str);
	printf("Object data size: %zu\n", uint64->type->size);
	printf("Point is Display = %d\n", Type_implement(uint64->type, trait_Display));
	printf("Point is Move2i = %d\n", Type_implement(uint64->type, trait_Move2i));
	printf("Point is Move3f = %d\n", Type_implement(uint64->type, trait_Move3f));

	// =====================================================================
	// Test Call
	Display_print(point);
	Display_print(vector3f);
	Display_print(uint64);

	CALL(point, "Move2i", "move", 3, 2);
	CALL(point, "Move2i", "moveX", -12);
	CALL(point, "Move2i", "moveY", 33);
	CALL(point, "Move2i", "moveY", 123);

	Display_print(point);
	Display_print(vector3f);

	const Method* method_vector3f_move = Trait_getMethod(trait_Move3f, HASH_STR("move"));
	Object_call(vector3f, method_vector3f_move, 1.0f, 2.0f, 3.0f);
	CALL(vector3f, "Move3f", "moveX", -12.0f);
	CALL(vector3f, "Move3f", "moveY", 33.0f);
	CALL(vector3f, "Move3f", "moveZ", -66.0f);

	Display_print(point);
	Display_print(vector3f);

	Object_finalize(point);
	Object_finalize(vector3f);

	TraitRuntime_clean();
	return 0;
}
