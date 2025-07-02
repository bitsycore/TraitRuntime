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
	trait->id = POOL_TRAITS_COUNT;
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

TraitImpl* TraitImpl_create(Trait* trait, Class* clazz) {
	EXIT_IF(trait == NULL, "param trait cannot be NULL");
	EXIT_IF(clazz == NULL, "param type cannot be NULL");
	EXIT_IF_NOT(clazz->trait_impl_count < MAX_TRAIT_IMPLS, "Cannot create TraitImpl \"%s\" for Class \"%s\"", trait->name.str, clazz->name.str);

	TraitImpl* trait_impl = &clazz->traits_impl[clazz->trait_impl_count];

	trait_impl->trait = trait;

	clazz->trait_impl_count++;

	return trait_impl;
}

void TraitImpl_addMethod(TraitImpl* trait_impl, const Method* method, const MethodFunc method_impl) {
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

TraitImpl* TraitImpl_get(const Class* clazz, const Trait* trait) {
	EXIT_IF(trait == NULL, "param trait cannot be NULL");

	for (size_t i = 0; i < clazz->trait_impl_count; ++i) {
		TraitImpl* impl = (TraitImpl*) &clazz->traits_impl[i];
		if (Trait_equal(impl->trait, trait)) {
			return impl;
		}
	}

	EXIT("No implementation of Trait \"%s\" found for Class \"%s\"", trait->name.str, clazz->name.str);
	return NULL;
}

MethodFunc TraitImpl_getMethodImplStr(const TraitImpl* trait_impl, const HashStr method_name) {
	EXIT_IF(trait_impl == NULL, "param trait_impl cannot be NULL");

	for (size_t j = 0; j < trait_impl->trait->method_count; ++j) {
		if (HashStr_equal(&trait_impl->trait->methods[j].name, &method_name) == true) {
			const MethodFunc method_impl = trait_impl->methods[j];
			if (method_impl == NULL) break;
			return method_impl;
		}
	}

	EXIT("No implementation of Method \"%s\" of Trait \"%s\" found for Class \"%s\"", method_name.str, trait_impl->trait->name.str);
	return NULL;
}

MethodFunc TraitImpl_getMethodImpl(const TraitImpl* trait_impl, const Method* method) {
	EXIT_IF(trait_impl == NULL, "param trait_impl cannot be NULL");

	for (size_t j = 0; j < trait_impl->trait->method_count; ++j) {
		if (HashStr_equal(&trait_impl->trait->methods[j].name, &method->name) == true) {
			const MethodFunc method_impl = trait_impl->methods[j];
			if (method_impl == NULL) break;
			return method_impl;
		}
	}

	EXIT("No implementation of Method \"%s\" of Trait \"%s\" found for Class \"%s\"", method->name.str, trait_impl->trait->name.str);
	return NULL;
}

TraitImpl* TraitImpl_getForMethodStr(Class* clazz, const HashStr methodName) {
	for (size_t i = 0; i < clazz->trait_impl_count; ++i) {
		TraitImpl* impl = &clazz->traits_impl[i];
		for (size_t j = 0; j < impl->trait->method_count; ++j) {
			const Method method = impl->trait->methods[j];
			if (HashStr_equal(&method.name, &methodName)) {
				return impl;
			}
		}
	}

	EXIT("No Implementation of Method \"%s\" found for Class \"%s\"", methodName.str, clazz->name.str);
	return NULL;
}