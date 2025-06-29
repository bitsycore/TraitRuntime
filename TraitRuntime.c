#include "TraitRuntime.h"

#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include "common/ErrorHandling.h"

// =====================================
// BUILT IN TYPE
// =====================================

BuiltInStore BuiltIn;

// =====================================
// GENERAL
// =====================================

static bool IS_INIT = false;

static Type* POOL_TYPE[MAX_TYPE];
static size_t POOL_TYPE_COUNT = 0;

static Trait* POOL_TRAIT[MAX_TRAITS];
static size_t POOL_TRAIT_COUNT = 0;

static TraitImpl* POOL_TRAIT_IMPL[MAX_TRAIT_IMPLS];
static size_t POOL_TRAIT_IMPL_COUNT = 0;

// =====================================
// GENERAL
// =====================================

void TraitRuntime_init(const bool enable_builtin) {
	memset(POOL_TYPE, 0, sizeof(POOL_TRAIT_IMPL));
	memset(POOL_TRAIT, 0, sizeof(POOL_TRAIT_IMPL));
	memset(POOL_TRAIT_IMPL, 0, sizeof(POOL_TRAIT_IMPL));

	IS_INIT = true;

	if (enable_builtin == false) return;

	// ==============================================
	// LOAD ALL BUILT IN TYPES AND TRAIT

	BuiltIn.traits.Finalizable.trait = TRAIT("Finalizable");
	BuiltIn.traits.Finalizable.methods.finalize = Trait_addMethod(BuiltIn.traits.Finalizable.trait, HASH_STR("finalize"), DEF_PARAM());

	BuiltIn.traits.Constructable.trait = TRAIT("Constructable");
	BuiltIn.traits.Constructable.methods.construct = Trait_addMethod(BuiltIn.traits.Constructable.trait, HASH_STR("construct"), DEF_PARAM());

	BuiltIn.types.UInt8 = TYPE("UInt8", uint8_t);
	BuiltIn.types.UInt16 = TYPE("UInt16", uint16_t);
	BuiltIn.types.UInt32 = TYPE("UInt32", uint32_t);
	BuiltIn.types.UInt64 = TYPE("UInt64", uint64_t);
	BuiltIn.types.Int8 = TYPE("Int8", int8_t);
	BuiltIn.types.Int16 = TYPE("Int16", int16_t);
	BuiltIn.types.Int32 = TYPE("Int32", int32_t);
	BuiltIn.types.Int64 = TYPE("Int64", int64_t);
	BuiltIn.types.Float32 = TYPE("Float32", float);
	BuiltIn.types.Float64 = TYPE("Float64", double);
}

void TraitRuntime_clean() {
	if (IS_INIT == false) {
		WARN("TraitRuntime_clean() called but Runtime not init or already cleaned");
		return;
	}
	for (int i = 0; i < POOL_TRAIT_COUNT; i++) {
		free(POOL_TRAIT[i]);
	}
	for (int i = 0; i < POOL_TRAIT_IMPL_COUNT; i++) {
		free(POOL_TRAIT_IMPL[i]);
	}
	for (int i = 0; i < POOL_TYPE_COUNT; i++) {
		free(POOL_TYPE[i]);
	}
	IS_INIT = false;
}

// =====================================
// TYPE
// =====================================

Type* Type_create(const HashStr name, const size_t size) {
	EXIT_IF_NOT(POOL_TYPE_COUNT < MAX_TYPE, "Can't create Type \"%s\", Max number of Type created in TraitRuntime", name.str);

	Type* t = malloc(sizeof(Type));
	t->name = name;
	t->size = size;
	t->id = POOL_TYPE_COUNT;
	POOL_TYPE[POOL_TYPE_COUNT++] = t;

	return t;
}

Type* Type_get(const HashStr name) {
	for (size_t i = 0; i < POOL_TYPE_COUNT; ++i) {
		if (HashStr_equal(&POOL_TYPE[i]->name, &name) == true) {
			return POOL_TYPE[i];
		}
	}
	EXIT("Type: %s, not found", name.str);
	return NULL;
}

Type* Type_getById(const size_t id) {
	EXIT_IF(id >= POOL_TYPE_COUNT, "Can't find type with id %zu", id);
	return POOL_TYPE[id];
}

bool Type_equal(const Type* this, const Type* other) {
	if (this == other) return true;
	if (HashStr_equal(&this->name, &other->name)) return true;
	return false;
}

bool Type_implement(const Type* type, const Trait* trait) {
	if (type == NULL || trait == NULL) return false;
	return Type_implementById(type->id, trait);
}

bool Type_implementById(const size_t id, const Trait* trait) {
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

TraitImpl* Type_getTraitImplMethod(const size_t type_id, const HashStr methodName) {

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

	EXIT("No Implementation of Method \"%s\" found for Type \"%s\"", methodName.str, Type_getById(type_id)->name.str);
	return NULL;
}

// =====================================
// TRAIT
// =====================================

Trait* Trait_create(const HashStr name, const size_t data_size) {
	EXIT_IF_NOT(POOL_TRAIT_COUNT < MAX_TRAITS, "Can't add Trait \"%s\", Max number of Trait created in TraitRuntime", name.str);

	Trait* t = malloc(sizeof(Trait));
	t->name = name;
	t->method_count = 0;
	t->data_size = data_size;
	POOL_TRAIT[POOL_TRAIT_COUNT++] = t;

	return t;
}

Trait* Trait_get(const HashStr name) {
	for (size_t i = 0; i < POOL_TRAIT_COUNT; ++i) {
		if (HashStr_equal(&POOL_TRAIT[i]->name, &name) == true) {
			return POOL_TRAIT[i];
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
	EXIT_IF_NOT(trait->method_count < MAX_METHODS_PER_TRAITS, "Can't add Method \"%s\" to Trait \"%s\", max number of method per Trait reached.", method_name.str, trait->name.str);

	Method* m = &trait->methods[trait->method_count++];
	m->name = method_name;
	m->param_count = param_count;
	m->param_vararg_at = -1;
	m->trait = (struct Trait*)trait;
	if (param_types != NULL) {
		for (size_t i = 0; i < param_count; ++i) {
			m->param_types[i] = param_types[i];
		}
	}

	return m;
}

// =====================================
// TRAIT METHOD
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
// TRAIT IMPL
// =====================================

TraitImpl* TraitImpl_create(Trait* trait, Type* type) {
	EXIT_IF(trait == NULL, "param trait cannot be NULL");
	EXIT_IF(type == NULL, "param type cannot be NULL");
	EXIT_IF_NOT(POOL_TRAIT_IMPL_COUNT < MAX_TRAIT_IMPLS, "Cannot create TraitImpl \"%s\" for Type \"%s\"", trait->name.str, type->name.str);

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

	if (Trait_equal(trait_impl->trait, (Trait*)method->trait)) {
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

	EXIT("No implementation of Trait \"%s\" found for Type \"%s\"", trait->name.str, Type_getById(type_id)->name.str);
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

	EXIT("No implementation of Method \"%s\" of Trait \"%s\" found for Type \"%s\"", method_name.str, trait_impl->trait->name.str, Type_getById(trait_impl->type_id)->name.str);
	return NULL;
}

// =====================================
// OBJECT
// =====================================

Object* Object_newFrom(const Type* type, void* data) {
	EXIT_IF(type == NULL, "param type cannot be NULL");
	Object* obj = malloc(sizeof(Object));
	obj->type_id = type->id;
	obj->data = data;
	if (Object_implement(obj, BuiltIn.traits.Constructable.trait)) {
		Object_call(obj, BuiltIn.traits.Constructable.methods.construct);
	}
	return obj;
}

Object* Object_new(const Type* type) {
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

	const TraitImpl* trait_impl = Type_getTraitImplMethod(obj->type_id, method_name);
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

Type* Object_getType(const Object* obj) {
	EXIT_IF(obj == NULL, "param obj cannot be NULL");
	return Type_getById(obj->type_id);
}

bool Object_is(const Object* obj, const Type* type) {
	if (obj == NULL || type == NULL) return false;
	return obj->type_id == type->id;
}

bool Object_implement(const Object* obj, const Trait* trait) {
	if (obj == NULL || trait == NULL) return false;
	return Type_implementById(obj->type_id, trait);
}

void Object_destroy(Object* obj) {
	if (obj == NULL) return;

	if (Type_implementById(obj->type_id, BuiltIn.traits.Finalizable.trait)) {
		Object_call(obj, BuiltIn.traits.Finalizable.methods.finalize);
	}

	free(obj->data);
	free(obj);
}
