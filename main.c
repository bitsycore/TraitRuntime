#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "common/Log.h"
#include "TraitRuntime.h"
#include "traits/Format.h"
#include "traits/Move2i.h"
#include "traits/Move3f.h"
#include "types/Point.h"
#include "types/Vector3f.h"

// ===========================================================
// Using Any Format
// ===========================================================

void Format_print(const Object* this) {
	const char* objToString = CALL_EX(this, "Format", "toString");
	LOG("toString: %s\n", objToString);
	free((void*)objToString);
	objToString = NULL;
}

// ===========================================================
// UINT64
// ===========================================================

void* MethodImpl_Format_UInt64_toString(MethodContext* CTX) {
	const uint64_t this = *(uint64_t*) METHOD_UNWRAP_START();
	CHECK_ALL_STR("UInt64", "Format", "toString");
	METHOD_UNWRAP_END();
	const char* template = "%s(%llu)";
	const int len = snprintf(NULL, 0, template, Type_getById(CTX->object->type_id)->name.str, this);
	char* buf = malloc(len + 1);
	snprintf(buf, len + 1, template, Type_getById(CTX->object->type_id)->name.str, this);
	return buf;
}

void printObjInfo(const Object* instance_point) {
	const Type* type = Object_getType(instance_point);
	LOG("Object is of type: %s\n", type->name.str);
	LOG("Object data size: %zu\n", type->size);
	LOG("Point implement Format = %d\n", Type_implement(type, trait_Format));
	LOG("Point implement Move2i = %d\n", Type_implement(type, trait_Move2i));
	LOG("Point implement Move3f = %d\n", Type_implement(type, trait_Move3f));
}

void do_work(void) {
	Object* instance_point = Object_new(type_Point);
	printObjInfo(instance_point);

	LINE_BREAK();

	Object* instance_vector3f = Object_new(type_Vector3f);
	printObjInfo(instance_vector3f);

	LINE_BREAK();

	Object* instance_uint64 = Object_new(BuiltIn.types.UInt64);
	*(uint64_t*)instance_uint64->data = 512;
	printObjInfo(instance_uint64);

	// =====================================================================
	// Test Call
	LINE_BREAK();

	Format_print(instance_point);
	CALL_EX(instance_point, "Move2i", "move", 3, 2);
	CALL_EX(instance_point, "Move2i", "moveX", -12);
	CALL_EX(instance_point, "Move2i", "moveY", 33);
	CALL_EX(instance_point, "Move2i", "moveY", 123);
	Format_print(instance_point);
	CALL(instance_point, "move", 3, 2);
	CALL(instance_point, "moveX", -12);
	CALL(instance_point, "moveY", 33);
	CALL(instance_point, "moveY", 123);

	LINE_BREAK();

	Format_print(instance_vector3f);
	Object_call(instance_vector3f, method_Move3f_move, 1.0f, 2.0f, 3.0f);
	Object_call(instance_vector3f, method_Move3f_moveY, -12.0f);
	Object_call(instance_vector3f, method_Move3f_moveZ, 33.0f);
	Object_call(instance_vector3f, method_Move3f_moveX, -66.0f);
	Format_print(instance_vector3f);

	LINE_BREAK();

	// =====================================================================
	// Cleanup
	Object_destroy(instance_point);
	Object_destroy(instance_vector3f);
	Object_destroy(instance_uint64);
}

int main() {
	const clock_t start = clock();

	// =====================================================================
	// Init Runtime
	TRAIT_RUNTIME_INIT();

	// =====================================================================
	// Create Traits
	Format_loadTrait();
	Move2i_loadTrait();
	Move3f_loadTrait();

	// =====================================================================
	// Load Type & Impl Trait
	Point_initType();
	Vector3f_initType();

	// =====================================================================
	// Impl Traits for Uint64
	TraitImpl* traitImpl_Format_UInt64 = TraitImpl_create(trait_Format, BuiltIn.types.UInt64);
	TraitImpl_addMethod(traitImpl_Format_UInt64, method_Format_toString, MethodImpl_Format_UInt64_toString);

	const clock_t end = clock();
	const double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
	LOG("Init Runtime took %.5f ms\n", time_spent * 1000.0f);

	const clock_t start2 = clock();
	// =====================================================================
	// Create Object
	TR_REPEAT(1) {
		do_work();
	}
	const clock_t end2 = clock();
	const double time_spent2 = (double)(end2 - start2) / CLOCKS_PER_SEC;
	LOG("Repeat Test took %.5f ms\n", time_spent2 * 1000.0f);

	// =====================================================================
	// Clean Runtime
	TraitRuntime_clean();
	return 0;
}
