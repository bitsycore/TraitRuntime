#include "Object.h"

#include "BuiltIn.h"
#include "Class.h"
#include "Trait.h"
#include "TraitRuntime.h"
#include "Commons/ErrorHandling.h"

// =====================================
// MARK: LIFE
// =====================================

Object* Object_newFrom(Class* clazz, void* data_buffer) {
	EXIT_IF(clazz == NULL, "param type cannot be NULL");
	Object* obj = malloc(sizeof(Object));
	obj->class = clazz;
	obj->data = data_buffer;
	if (Object_implement(obj, BuiltIn.traits.Constructable.trait)) {
		Object_call(obj, BuiltIn.traits.Constructable.methods.construct);
	}
	return obj;
}

Object* Object_new(Class* clazz) {
	EXIT_IF(clazz == NULL, "param type cannot be NULL");
	return Object_newFrom(clazz, calloc(1, clazz->data_size));
}

void Object_destroy(Object* obj) {
	if (obj == NULL) return;

	if (BuiltIn.traits.Finalizable.trait != NULL && Class_implement(obj->class, BuiltIn.traits.Finalizable.trait)) {
		Object_call(obj, BuiltIn.traits.Finalizable.methods.finalize);
	}

	free(obj->data);
	free(obj);
}

// =====================================
// MARK: GET INFO
// =====================================

Class* Object_getClass(const Object* obj) {
	EXIT_IF(obj == NULL, "param obj cannot be NULL");
	return obj->class;
}

bool Object_is(const Object* obj, const Class* clazz) {
	if (obj == NULL || clazz == NULL) return false;
	return obj->class == clazz;
}

bool Object_implement(const Object* obj, const Trait* trait) {
	if (obj == NULL || trait == NULL) return false;
	return Class_implement(obj->class, trait);
}

// =====================================
// MARK: CALL
// =====================================

static void* PRIVATE_Object_call(const Object* obj, const MethodImpl* method_impl, va_list* args) {
	EXIT_IF(obj == NULL, "param obj cannot be NULL");
	EXIT_IF(method_impl == NULL, "param trait impl cannot be NULL");
	EXIT_IF(args == NULL, "param args cannot be NULL");

	MethodContext ctx = (MethodContext){obj, method_impl->method_def, args};

	return method_impl->method_func(&ctx);
}

void* Object_callStr(const Object* obj, const HashStr method_name, ...) {
	EXIT_IF(obj == NULL, "param obj cannot be NULL");

	const MethodImpl* method_impl = Class_getMethodImplStr(obj->class, method_name);

	va_list args;
	va_start(args, method_name);
	void* result = PRIVATE_Object_call(obj, method_impl, &args);
	va_end(args);

	return result;
}

void* Object_callTraitStr(const Object* obj, const HashStr trait_name, const HashStr method_name, ...) {
	EXIT_IF(obj == NULL, "param obj cannot be NULL");

	const MethodImpl* trait_impl = Class_getMethodImplStr(obj->class, method_name); // TODO: trait

	va_list args;
	va_start(args, method_name);
	void* result = PRIVATE_Object_call(obj, trait_impl, &args);
	va_end(args);

	return result;
}

void* Object_call(const Object* obj, const MethodDef* method, ...) {
	EXIT_IF(obj == NULL, "param obj cannot be NULL");
	EXIT_IF(method == NULL, "param method cannot be NULL");

	const MethodImpl* method_impl = Class_getMethodImplStr(obj->class, method->name);

	va_list args;
	va_start(args, method);
	void* result = PRIVATE_Object_call(obj, method_impl, &args);
	va_end(args);

	return result;
}