#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include <TraitRuntime.h>
#include <TraitRuntime/Commons/Log.h>

#include "classes/Point.h"
#include "classes/Vector3f.h"
#include "traits/Describe.h"
#include "traits/Move2i.h"
#include "traits/Move3f.h"

// ===========================================================
// Using Any Describe
// ===========================================================

void Describe_print(const Object* this) {
	const char* objToString = Object_call(this, trDescribe.methods.toString);
	LOG("toString: %s\n", objToString);
	free((void*)objToString);
	objToString = NULL;
}

// ===========================================================
// UINT64
// ===========================================================

void* MethodImpl_Describe_UInt64_toString(MethodContext* CTX) {
	const uint64_t this = *(uint64_t*) TR_METHOD_UNWRAP_START();
	TR_CHECK_ALL_STR("UInt64", "Describe", "toString");
	TR_METHOD_UNWRAP_END();
	const char* template = "%s(%llu)";
	const int len = snprintf(NULL, 0, template, CTX->object->class->name.str, this);
	char* buf = malloc(len + 1);
	snprintf(buf, len + 1, template, CTX->object->class->name.str, this);
	return buf;
}

void printObjInfo(const Object* instance_point) {
	const Class* type = Object_getClass(instance_point);
	LOG("Object is of type: %s\n", type->name.str);
	LOG("Object data size: %zu\n", type->size);
	LOG("Point implement Describe = %d\n", Class_implement(type, trDescribe.trait));
	LOG("Point implement Move2i = %d\n", Class_implement(type, trMove2i.trait));
	LOG("Point implement Move3f = %d\n", Class_implement(type, trMove3f.trait));
}

void do_work(void) {
	Object* instance_point = Object_new(clsPoint);
	printObjInfo(instance_point);

	LINE_BREAK();

	Object* instance_vector3f = Object_new(clsVector3f);
	printObjInfo(instance_vector3f);

	LINE_BREAK();

	Object* instance_uint64 = Object_new(BuiltIn.classes.UInt64);
	*(uint64_t*)instance_uint64->data = 512;
	printObjInfo(instance_uint64);

	// =====================================================================
	// Test Call
	LINE_BREAK();

	Describe_print(instance_point);
	TR_OBJ_TRAIT_CALL(instance_point, "Move2i", "move", 3, 2);
	TR_OBJ_TRAIT_CALL(instance_point, "Move2i", "moveX", -12);
	TR_OBJ_TRAIT_CALL(instance_point, "Move2i", "moveY", 33);
	TR_OBJ_TRAIT_CALL(instance_point, "Move2i", "moveY", 123);
	Describe_print(instance_point);
	TR_OBJ_CALL(instance_point, "move", 3, 2);
	TR_OBJ_CALL(instance_point, "moveX", -12);
	TR_OBJ_CALL(instance_point, "moveY", 33);
	TR_OBJ_CALL(instance_point, "moveY", 123);

	LINE_BREAK();

	Describe_print(instance_vector3f);
	Object_call(instance_vector3f, trMove3f.methods.move, 1.0f, 2.0f, 3.0f);
	Object_call(instance_vector3f, trMove3f.methods.moveY, -12.0f);
	Object_call(instance_vector3f, trMove3f.methods.moveZ, 33.0f);
	Object_call(instance_vector3f, trMove3f.methods.moveX, -66.0f);
	Describe_print(instance_vector3f);

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
	TR_INIT();

	// =====================================================================
	// Create Traits
	Describe_loadTrait();
	Move2i_loadTrait();
	Move3f_loadTrait();

	// =====================================================================
	// Load Class & Impl Trait
	Point_initClass();
	Vector3f_initClass();

	// =====================================================================
	// Impl Traits for Uint64
	TraitImpl* traitImpl_Describe_UInt64 = TraitImpl_create(trDescribe.trait, BuiltIn.classes.UInt64);
	TraitImpl_addMethod(traitImpl_Describe_UInt64, trDescribe.methods.toString, MethodImpl_Describe_UInt64_toString);

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
	Runtime_clean();
	return 0;
}
