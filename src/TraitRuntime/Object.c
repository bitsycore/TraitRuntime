#include "Object.h"

#include "Class.h"
#include "Trait.h"

// =====================================
// MARK: OBJECT
// =====================================

Object* Object_newFrom(const Class* type, void* data) {
	EXIT_IF(type == NULL, "param type cannot be NULL");
	Object* obj = malloc(sizeof(Object));
	obj->type_id = type->id;
	obj->data = data;
	if (Object_implement(obj, BuiltIn.traits.Constructable.trait)) {
		Object_call(obj, BuiltIn.traits.Constructable.methods.construct);
	}
	return obj;
}

Object* Object_new(const Class* type) {
	EXIT_IF(type == NULL, "param type cannot be NULL");
	return Object_newFrom(type, calloc(1, type->size));
}

MethodImpl Object_getMethod(const Object* obj, const HashStr trait_name, const HashStr method_name) {
	EXIT_IF(obj == NULL, "param obj cannot be NULL");
	const Trait* trait = Trait_get(trait_name);
	const TraitImpl* impl = TraitImpl_get(obj->type_id, trait);
	return TraitImpl_getMethodImpl(impl, method_name);
}

static void* INTERNAL_Object_call(const Object* obj, const Method* trait_method, const Trait* trait, const TraitImpl* trait_impl, va_list* args) {
	EXIT_IF(obj == NULL, "param obj cannot be NULL");
	EXIT_IF(trait == NULL, "param trait cannot be NULL");
	EXIT_IF(trait_method == NULL, "param trait method cannot be NULL");
	EXIT_IF(trait_impl == NULL, "param trait impl cannot be NULL");

	const MethodImpl trait_method_impl = TraitImpl_getMethodImpl(trait_impl, trait_method->name);
	MethodContext ctx = (MethodContext){obj, trait, trait_method, args};

	return trait_method_impl(&ctx);
}

void* Object_callStr(const Object* obj, const HashStr method_name, ...) {
	EXIT_IF(obj == NULL, "param obj cannot be NULL");

	const TraitImpl* trait_impl = TraitImpl_getForMethod(obj->type_id, method_name);
	const Method* method = Trait_getMethod(trait_impl->trait, method_name);

	va_list args;
	va_start(args, method_name);
	void* result = INTERNAL_Object_call(obj, method, trait_impl->trait, trait_impl, &args);
	va_end(args);

	return result;
}

void* Object_callStrEx(const Object* obj, const HashStr trait_name, const HashStr method_name, ...) {
	EXIT_IF(obj == NULL, "param obj cannot be NULL");

	const Trait* trait = Trait_get(trait_name);
	const TraitImpl* trait_impl = TraitImpl_get(obj->type_id, trait);
	const Method* trait_method = Trait_getMethod(trait, method_name);

	va_list args;
	va_start(args, method_name);
	void* result = INTERNAL_Object_call(obj, trait_method, trait, trait_impl, &args);
	va_end(args);

	return result;
}

void* Object_call(const Object* obj, const Method* method, ...) {
	EXIT_IF(obj == NULL, "param obj cannot be NULL");
	EXIT_IF(method == NULL, "param method cannot be NULL");

	const Trait* trait = (Trait*)method->trait;
	const TraitImpl* trait_impl = TraitImpl_get(obj->type_id, trait);

	va_list args;
	va_start(args, method);
	void* result = INTERNAL_Object_call(obj, method, trait, trait_impl, &args);
	va_end(args);

	return result;
}

Class* Object_getClass(const Object* obj) {
	EXIT_IF(obj == NULL, "param obj cannot be NULL");
	return Class_getById(obj->type_id);
}

bool Object_is(const Object* obj, const Class* type) {
	if (obj == NULL || type == NULL) return false;
	return obj->type_id == type->id;
}

bool Object_implement(const Object* obj, const Trait* trait) {
	if (obj == NULL || trait == NULL) return false;
	return Class_implementById(obj->type_id, trait);
}

void Object_destroy(Object* obj) {
	if (obj == NULL) return;

	if (BuiltIn.traits.Finalizable.trait != NULL && Class_implementById(obj->type_id, BuiltIn.traits.Finalizable.trait)) {
		Object_call(obj, BuiltIn.traits.Finalizable.methods.finalize);
	}

	free(obj->data);
	free(obj);
}