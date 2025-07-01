#include "Class.h"

#include "Trait.h"
#include "TraitRuntime.h"

Class POOL_CLASSES[MAX_CLASSES] = {0};
size_t POOL_CLASSES_COUNT = 0;

// =====================================
// MARK: CLASS
// =====================================

Class* Class_create(const HashStr name, const size_t size) {
	EXIT_IF_NOT(POOL_CLASSES_COUNT < MAX_CLASSES, "Can't create Class \"%s\", Max number of Class created in TraitRuntime", name.str);

	Class* t = &POOL_CLASSES[POOL_CLASSES_COUNT];

	TR_USE(Class, t) {
		it->name = name;
		it->size = size;
		it->id = POOL_CLASSES_COUNT;
	}

	POOL_CLASSES_COUNT++;

	return t;
}

Class* Class_get(const HashStr name) {
	for (size_t i = 0; i < POOL_CLASSES_COUNT; ++i) {
		if (HashStr_equal(&POOL_CLASSES[i].name, &name) == true) {
			return &POOL_CLASSES[i];
		}
	}
	EXIT("Class: %s, not found", name.str);
	return NULL;
}

Class* Class_getById(const size_t id) {
	EXIT_IF(id >= POOL_CLASSES_COUNT, "Can't find type with id %zu", id);
	return &POOL_CLASSES[id];
}

bool Class_equal(const Class* this, const Class* other) {
	if (this == other) return true;
	if (HashStr_equal(&this->name, &other->name)) return true;
	return false;
}

bool Class_implement(const Class* clazz, const Trait* trait) {
	if (clazz == NULL || trait == NULL) return false;
	for (size_t i = 0; i < clazz->trait_impl_count; ++i) {
		const TraitImpl* impl = &clazz->traits_impl[i];
		if (
			Trait_equal(impl->trait, trait) &&
			impl->class_id == clazz->id
		) {
			return true;
		}
	}
	return false;
}

bool Class_implementById(const size_t id, const Trait* trait) {
	return Class_implement(Class_getById(id), trait);
}