#include "Class.h"

#include <string.h>

#include "Trait.h"
#include "TraitRuntime.h"
#include "Commons/Flags.h"
#include "Container/HashTable.h"

HT_DEF_TYPED_NODE(Class)

HT_NODE(Class) POOL_CLASSES[CLASS_HASHTABLE_SIZE] = { 0 };

// =====================================
// MARK: CLASS
// =====================================

Class* Class_create(const HashStr name, const size_t size) {
	Arena* arena = INTERNAL_Runtime_getArena();

	EXIT_IF(arena == NULL, "arena has not been initialized, is Runtime_init called ?");

	Class* it = Arena_alloc(arena, sizeof(Class));
	it->name = name;
	it->data_size = size;
	it->flags = 0;
	memset(it->methods_impl, 0, sizeof(HT_NODE(MethodImpl)) * METHODS_IMPL_HASHTABLE_SIZE);
	memset(it->traits_impl, 0, sizeof(HT_NODE(Trait)) * TRAIT_IMPL_HASHTABLE_SIZE);

	HT_INSERT(Class, POOL_CLASSES, name.hash, CLASS_HASHTABLE_SIZE, arena, it);

	FLAG_SET(it->flags, CLASS_FLAG_IS_INIT);

	return it;
}

Class* Class_get(const HashStr name) {
	const size_t hash_index = HT_INDEX(name.hash, CLASS_HASHTABLE_SIZE);
	const HT_NODE(Class) *cur = &POOL_CLASSES[hash_index];
	while (cur->current != NULL) {
		if (HashStr_equal(&cur->current->name, &name)) {
			return cur->current;
		}
		cur = cur->next;
	}
	EXIT("Class: %s, not found", name.str);
	return NULL;
}

bool Class_equal(const Class* this, const Class* other) {
	if (this == other) return true;
	// Not needed but could be used in case of debugging if there is
	// more source of Class creation in the future.
	// If (this->id == other->id) returns true;
	// if (HashStr_equal(&this->name, &other->name)) return true;
	return false;
}

bool Class_implement(const Class* clazz, const Trait* trait) {
	if (clazz == NULL || trait == NULL) return false;

	const size_t hash_index = HT_INDEX(trait->name.hash, TRAIT_IMPL_HASHTABLE_SIZE);
	const HT_NODE(Trait) *cur = &clazz->traits_impl[hash_index];
	while (cur != NULL && cur->current != NULL) {
		if (Trait_equal(cur->current, trait)) {
			return true;
		}
		cur = cur->next;
	}

	return false;
}

// =====================================
// MARK: METHOD IMPL
// =====================================

static void Internal_Class_addTraitImpl(Class* clazz, const Trait* trait) {
	EXIT_IF(clazz == NULL, "param clazz cannot be NULL");
	EXIT_IF(trait == NULL, "param trait cannot be NULL");

	const size_t hash_index = HT_INDEX(trait->name.hash, TRAIT_IMPL_HASHTABLE_SIZE);

	HT_NODE(Trait)* node = &clazz->traits_impl[hash_index];

	while (node->current != NULL) {
		if (node->next != NULL) {
			node = node->next;
		} else {
			Arena* arena = INTERNAL_Runtime_getArena();
			EXIT_IF(arena == NULL, "arena has not been initialized, is Runtime_init called ?");
			node->next = Arena_alloc(arena, sizeof(HT_NODE(Trait)));
			node->next->current = NULL;
			node->next->next = NULL;
			break;
		}
	}

	node->current = (void*)trait;
}

void Class_validateTrait(Class* clazz, const Trait* trait) {
	EXIT_IF(clazz == NULL, "clazz param cannot be null");
	EXIT_IF(trait == NULL, "trait param cannot be null");

	size_t implemented_count = 0;

	for (size_t i = 0; i < trait->method_count; ++i) {
		const MethodDef* method = &trait->methods[i];

		const size_t hash_index = HT_INDEX(method->name.hash, METHODS_IMPL_HASHTABLE_SIZE);
		const HT_NODE(MethodImpl)* node = &clazz->methods_impl[hash_index];

		while (node != NULL && node->current != NULL) {
			const MethodImpl* impl = (MethodImpl*)node->current;
			if (HashStr_equal(&impl->method_def->name, &method->name)) {
				// Verify this implementation is for the correct trait
				if (Trait_equal(impl->method_def->trait, trait)) {
					implemented_count++;
					break;
				}
			}
			node = node->next;
		}

		if (implemented_count == trait->method_count) {
			break;
		}
	}

	EXIT_IF(implemented_count != trait->method_count, "Trait \"%s\" not fully implemented by Class \"%s\"", trait->name.str, clazz->name.str);

	Internal_Class_addTraitImpl(clazz, trait);
}


MethodImpl* Class_implementMethod(Class* clazz, MethodDef* method_def, const MethodFunc method_func) {
	EXIT_IF(clazz == NULL, "param type cannot be NULL");
	EXIT_IF(method_def == NULL, "param method cannot be NULL");
	EXIT_IF(method_func == NULL, "param method_impl cannot be NULL");

	Arena* arena = INTERNAL_Runtime_getArena();

	EXIT_IF(arena == NULL, "arena has not been initialized, is Runtime_init called ?");

	MethodImpl* it = Arena_alloc(arena, sizeof(MethodImpl));
	it->method_def = method_def;
	it->method_func = method_func;

	const size_t hash_index = HT_INDEX(method_def->name.hash, METHODS_IMPL_HASHTABLE_SIZE);

	HT_NODE(MethodImpl)* node = &clazz->methods_impl[hash_index];

	while (node->current != NULL) {
		if (node->next != NULL) {
			node = node->next;
		} else {
			node->next = Arena_alloc(arena, sizeof(HT_NODE(MethodImpl)));
			node->next->current = NULL;
			node->next->next = NULL;
			node = node->next;
			break;
		}
	}

	node->current = it;

	return it;
}

MethodImpl* Class_getMethodImplStr(const Class* class, const HashStr method_name) {
	EXIT_IF(class == NULL, "param class cannot be NULL");

	const size_t hash_index = HT_INDEX(method_name.hash, METHODS_IMPL_HASHTABLE_SIZE)

	const HT_NODE(MethodImpl) *cur = &class->methods_impl[hash_index];
	while (cur->current != NULL) {
		if (HashStr_equal(&cur->current->method_def->name, &method_name)) {
			return cur->current;
		}
		cur = cur->next;
	}

	EXIT("No implementation of Method \"%s\" found for Class \"%s\"", method_name.str, class->name.str);
	return NULL;
}