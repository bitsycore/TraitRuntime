#include "Trait.h"

#include <string.h>

#include "Class.h"
#include "Commons/ErrorHandling.h"

Trait POOL_TRAITS[MAX_TRAITS] = {0};
size_t POOL_TRAITS_COUNT = 0;

// =====================================
// MARK: TRAIT
// =====================================

Trait* Trait_create(const HashStr name) {
	EXIT_IF_NOT(POOL_TRAITS_COUNT < MAX_TRAITS, "Can't add Trait \"%s\", Max number of Trait created in TraitRuntime", name.str);
	Trait* trait = &POOL_TRAITS[POOL_TRAITS_COUNT];
	trait->name = name;
	POOL_TRAITS_COUNT++;
	return trait;
}

Trait* Trait_get(const HashStr name) {
	for (size_t i = 0; i < POOL_TRAITS_COUNT; ++i) {
		if (HashStr_equal(&POOL_TRAITS[i].name, &name) == true) {
			return &POOL_TRAITS[i];
		}
	}

	EXIT("Trait %s not found.", name.str);
	return NULL;
}

bool Trait_equal(const Trait* this, const Trait* other) {
	if (this == NULL || other == NULL) return false;
	if (this == other) return true;
	// if (HashStr_equal(&this->name, &other->name)) return true;
	return false;
}

MethodDef* Trait_addMethod(Trait* trait, const HashStr method_name, const HashStr* param_types, const size_t param_count) {
	EXIT_IF(trait == NULL, "param trait cannot be NULL");
	EXIT_IF_NOT(trait->method_count < MAX_METHODS_PER_TRAITS,
		"Can't add Method \"%s\" to Trait \"%s\", max number of method per Trait reached.", method_name.str, trait->name.str);

	MethodDef* m = &trait->methods[trait->method_count++];
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

MethodDef* Trait_getMethod(const Trait* trait, const HashStr method_name) {
	EXIT_IF(trait == NULL, "param trait cannot be NULL");

	const Trait* impl = trait;
	for (size_t j = 0; j < MAX_METHODS_PER_TRAITS; ++j) {
		const MethodDef* m = &impl->methods[j];
		if (HashStr_equal(&m->name, &method_name) == true)
			return (MethodDef*)m;
	}

	EXIT("Method \"%s\" not found in Trait \"%s\"", method_name.str, trait->name.str);
	return NULL;
}