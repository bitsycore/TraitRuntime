#include "TraitRuntime.h"

#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include "TraitRuntime/Commons/ErrorHandling.h"
#include "TraitRuntime/Memory/Arena.h"


// =====================================
// MARK: BUILT IN TYPE
// =====================================

Container_BuiltIn BuiltIn;

// =====================================
// MARK: GENERAL
// =====================================

Arena* ARENA_GLOBAL;

static bool IS_INIT = false;

static Class* POOL_CLASSES[MAX_CLASSES];
static size_t POOL_CLASSES_COUNT = 0;

static Trait* POOL_TRAITS[MAX_TRAITS];
static size_t POOL_TRAITS_COUNT = 0;

static TraitImpl* POOL_TRAIT_IMPL[MAX_TRAIT_IMPLS];
static size_t POOL_TRAIT_IMPL_COUNT = 0;

// =====================================
// MARK: GENERAL
// =====================================

static void INTERNAL_initialize_built_in_traits_and_types(void) {
	const size_t arena_size = Arena_requiredSize(64 * 1024 * 1024);
	Arena* arena = Arena_init(malloc(arena_size), arena_size);
	uint8_t* buffer = Arena_alloc(arena, 64 * 1024 * 1024);
	buffer = memset(buffer, 0x88, 64 * 1024 * 1024);

	BuiltIn.traits.Finalizable.trait = TR_TRAIT("Finalizable");
	BuiltIn.traits.Finalizable.methods.finalize = Trait_addMethod(BuiltIn.traits.Finalizable.trait, HASH_STR("finalize"), TR_PARAMS());

	BuiltIn.traits.Constructable.trait = TR_TRAIT("Constructable");
	BuiltIn.traits.Constructable.methods.construct = Trait_addMethod(BuiltIn.traits.Constructable.trait, HASH_STR("construct"), TR_PARAMS());

	BuiltIn.types.UInt8		= TR_CLASS("UInt8", uint8_t);
	BuiltIn.types.UInt16	= TR_CLASS("UInt16", uint16_t);
	BuiltIn.types.UInt32	= TR_CLASS("UInt32", uint32_t);
	BuiltIn.types.UInt64	= TR_CLASS("UInt64", uint64_t);
	BuiltIn.types.Int8		= TR_CLASS("Int8", int8_t);
	BuiltIn.types.Int16		= TR_CLASS("Int16", int16_t);
	BuiltIn.types.Int32		= TR_CLASS("Int32", int32_t);
	BuiltIn.types.Int64		= TR_CLASS("Int64", int64_t);
	BuiltIn.types.Float32	= TR_CLASS("Float32", float);
	BuiltIn.types.Float64	= TR_CLASS("Float64", double);
}

void TraitRuntime_init(const bool enable_builtin) {
	memset(POOL_CLASSES, 0, sizeof(POOL_CLASSES));
	memset(POOL_TRAITS, 0, sizeof(POOL_TRAITS));
	memset(POOL_TRAIT_IMPL, 0, sizeof(POOL_TRAIT_IMPL));

	IS_INIT = true;

	if (enable_builtin == false) return;

	// ==============================================
	// LOAD ALL BUILT IN TYPES AND TRAIT

	INTERNAL_initialize_built_in_traits_and_types();
}

void TraitRuntime_clean() {
	if (IS_INIT == false) {
		WARN("TraitRuntime_clean() called but Runtime not init or already cleaned");
		return;
	}
	for (int i = 0; i < POOL_TRAITS_COUNT; i++) {
		free(POOL_TRAITS[i]);
	}
	for (int i = 0; i < POOL_TRAIT_IMPL_COUNT; i++) {
		free(POOL_TRAIT_IMPL[i]);
	}
	for (int i = 0; i < POOL_CLASSES_COUNT; i++) {
		free(POOL_CLASSES[i]);
	}
	IS_INIT = false;
}

// =====================================
// MARK: TYPE
// =====================================

Class* Class_create(const HashStr name, const size_t size) {
	EXIT_IF_NOT(POOL_CLASSES_COUNT < MAX_CLASSES, "Can't create Class \"%s\", Max number of Class created in TraitRuntime", name.str);

	Class* t = malloc(sizeof(Class));
	t->name = name;
	t->size = size;
	t->id = POOL_CLASSES_COUNT;
	POOL_CLASSES[POOL_CLASSES_COUNT++] = t;

	return t;
}

Class* Class_get(const HashStr name) {
	for (size_t i = 0; i < POOL_CLASSES_COUNT; ++i) {
		if (HashStr_equal(&POOL_CLASSES[i]->name, &name) == true) {
			return POOL_CLASSES[i];
		}
	}
	EXIT("Class: %s, not found", name.str);
	return NULL;
}

Class* Class_getById(const size_t id) {
	EXIT_IF(id >= POOL_CLASSES_COUNT, "Can't find type with id %zu", id);
	return POOL_CLASSES[id];
}

bool Class_equal(const Class* this, const Class* other) {
	if (this == other) return true;
	if (HashStr_equal(&this->name, &other->name)) return true;
	return false;
}

bool Class_implement(const Class* type, const Trait* trait) {
	if (type == NULL || trait == NULL) return false;
	return Class_implementById(type->id, trait);
}

bool Class_implementById(const size_t id, const Trait* trait) {
	if (trait == NULL) return false;

	for (size_t i = 0; i < POOL_TRAIT_IMPL_COUNT; ++i) {
		const TraitImpl* impl = POOL_TRAIT_IMPL[i];
		if (
			Trait_equal(impl->trait, trait) &&
			impl->type_id == id
		) {
			return true;
		}
	}
	return false;
}

TraitImpl* Class_getTraitImplMethod(const size_t type_id, const HashStr methodName) {

	for (size_t i = 0; i < POOL_TRAIT_IMPL_COUNT; ++i) {
		TraitImpl* impl = POOL_TRAIT_IMPL[i];
		if (impl->type_id == type_id) {
			for (size_t j = 0; j < impl->trait->method_count; ++j) {
				const Method method = impl->trait->methods[j];
				if (HashStr_equal(&method.name, &methodName)) {
					return impl;
				}
			}
		}
	}

	EXIT("No Implementation of Method \"%s\" found for Class \"%s\"", methodName.str, Class_getById(type_id)->name.str);
	return NULL;
}

// =====================================
// MARK: TRAIT
// =====================================

Trait* Trait_create(const HashStr name, const size_t data_size) {
	EXIT_IF_NOT(POOL_TRAITS_COUNT < MAX_TRAITS, "Can't add Trait \"%s\", Max number of Trait created in TraitRuntime", name.str);

	Trait* t = malloc(sizeof(Trait));
	t->name = name;
	t->method_count = 0;
	t->data_size = data_size;
	POOL_TRAITS[POOL_TRAITS_COUNT++] = t;

	return t;
}

Trait* Trait_get(const HashStr name) {
	for (size_t i = 0; i < POOL_TRAITS_COUNT; ++i) {
		if (HashStr_equal(&POOL_TRAITS[i]->name, &name) == true) {
			return POOL_TRAITS[i];
		}
	}

	EXIT("Trait %s not found.", name.str);
	return NULL;
}

bool Trait_equal(const Trait* this, const Trait* other) {
	if (this == NULL || other == NULL) return false;
	if (this == other) return true;
	if (HashStr_equal(&this->name, &other->name)) return true;
	return false;
}

Method* Trait_addMethod(Trait* trait, const HashStr method_name, const HashStr* param_types, const size_t param_count) {
	EXIT_IF(trait == NULL, "param trait cannot be NULL");
	EXIT_IF_NOT(trait->method_count < MAX_METHODS_PER_TRAITS,
		"Can't add Method \"%s\" to Trait \"%s\", max number of method per Trait reached.", method_name.str, trait->name.str);

	Method* m = &trait->methods[trait->method_count++];
	m->name = method_name;
	m->params_count = param_count;
	m->param_vararg_at = -1;
	m->trait = trait;
	if (param_types != NULL) {
		for (size_t i = 0; i < param_count; ++i) {
			m->params[i] = param_types[i];
		}
	}

	return m;
}

// =====================================
// MARK: TRAIT
// =====================================

Method* Trait_getMethod(const Trait* trait, const HashStr method_name) {
	EXIT_IF(trait == NULL, "param trait cannot be NULL");

	const Trait* impl = trait;
	for (size_t j = 0; j < MAX_METHODS_PER_TRAITS; ++j) {
		const Method* m = &impl->methods[j];
		if (HashStr_equal(&m->name, &method_name) == true)
			return (Method*)m;
	}

	EXIT("Method \"%s\" not found in Trait \"%s\"", method_name.str, trait->name.str);
	return NULL;
}

// =====================================
// MARK: TRAIT IMPL
// =====================================

TraitImpl* TraitImpl_create(Trait* trait, Class* type) {
	EXIT_IF(trait == NULL, "param trait cannot be NULL");
	EXIT_IF(type == NULL, "param type cannot be NULL");
	EXIT_IF_NOT(POOL_TRAIT_IMPL_COUNT < MAX_TRAIT_IMPLS, "Cannot create TraitImpl \"%s\" for Class \"%s\"", trait->name.str, type->name.str);

	TraitImpl* impl = malloc(sizeof(TraitImpl));
	impl->type_id = type->id;
	impl->trait = trait;
	memset(impl->methods, 0, sizeof(impl->methods));
	POOL_TRAIT_IMPL[POOL_TRAIT_IMPL_COUNT++] = impl;

	return impl;
}

void TraitImpl_addMethod(TraitImpl* trait_impl, const Method* method, const MethodImpl method_impl) {
	EXIT_IF(method == NULL, "param method cannot be NULL");
	EXIT_IF(trait_impl == NULL, "param trait_impl cannot be NULL");
	EXIT_IF(method_impl == NULL, "param method_impl cannot be NULL");

	if (Trait_equal(trait_impl->trait, method->trait)) {
		for (size_t i = 0; i < trait_impl->trait->method_count; ++i) {
			if (HashStr_equal(&trait_impl->trait->methods[i].name, &method->name) == true) {
				trait_impl->methods[i] = method_impl;
				return;
			}
		}
	}

	EXIT("Method \"%s\" not found in Trait \"%s\"", method->name.str, trait_impl->trait->name.str);
}

TraitImpl* TraitImpl_get(const size_t type_id, const Trait* trait) {
	EXIT_IF(trait == NULL, "param trait cannot be NULL");

	for (size_t i = 0; i < POOL_TRAIT_IMPL_COUNT; ++i) {
		TraitImpl* impl = POOL_TRAIT_IMPL[i];
		if (impl->type_id == type_id && Trait_equal(impl->trait, trait)) {
			return impl;
		}
	}

	EXIT("No implementation of Trait \"%s\" found for Class \"%s\"", trait->name.str, Class_getById(type_id)->name.str);
	return NULL;
}

MethodImpl TraitImpl_getMethodImpl(const TraitImpl* trait_impl, const HashStr method_name) {
	EXIT_IF(trait_impl == NULL, "param trait_impl cannot be NULL");

	for (size_t j = 0; j < trait_impl->trait->method_count; ++j) {
		if (HashStr_equal(&trait_impl->trait->methods[j].name, &method_name) == true) {
			const MethodImpl method_impl = trait_impl->methods[j];
			if (method_impl == NULL) break;
			return method_impl;
		}
	}

	EXIT("No implementation of Method \"%s\" of Trait \"%s\" found for Class \"%s\"", method_name.str, trait_impl->trait->name.str, Class_getById(trait_impl->type_id)->name.str);
	return NULL;
}

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

static void* INTERNAL_Object_call(const Object* obj, const Method* trait_method, const Trait* trait, const TraitImpl* trait_impl, const va_list args) {
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

	const TraitImpl* trait_impl = Class_getTraitImplMethod(obj->type_id, method_name);
	const Method* method = Trait_getMethod(trait_impl->trait, method_name);

	va_list args;
	va_start(args, method_name);
	void* result = INTERNAL_Object_call(obj, method, trait_impl->trait, trait_impl, args);
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
	void* result = INTERNAL_Object_call(obj, trait_method, trait, trait_impl, args);
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
	void* result = INTERNAL_Object_call(obj, method, trait, trait_impl, args);
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

	if (Class_implementById(obj->type_id, BuiltIn.traits.Finalizable.trait)) {
		Object_call(obj, BuiltIn.traits.Finalizable.methods.finalize);
	}

	free(obj->data);
	free(obj);
}
