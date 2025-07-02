#include "Class.h"

#include <string.h>

#include "Trait.h"
#include "TraitRuntime.h"

#define CLASS_HASHTABLE_SIZE 64
#define BUILT_IN_CLASSES_SIZE 10

typedef struct ClassNode {
	Class* cur;
	struct ClassNode* next;
} ClassNode;

ClassNode POOL_CLASSES[CLASS_HASHTABLE_SIZE] = {0};

#define MSB_SIZE_T_MASK (1ULL << ((sizeof(size_t) * 8) - 1))
#define SET_BUILTIN_FLAG(value) ((value) | MSB_SIZE_T_MASK)
#define IS_BUILTIN_FLAG_SET(value) (((value) & MSB_SIZE_T_MASK) != 0)
#define GET_ACTUAL_INDEX(value_with_flag) ((value_with_flag) & ~MSB_SIZE_T_MASK)

// =====================================
// MARK: CLASS
// =====================================

Class* Class_create(const HashStr name, const size_t size) {
	Arena* arena = Internal_Runtime_getArena();

	Class* it = Arena_alloc(arena, sizeof(Class));
	it->name = name;
	it->data_size = size;
	it->flags = 0;
	it->trait_impl_count = 0;
	memset(it->traits_impl, 0, sizeof(TraitImpl) * MAX_TRAIT_IMPLS);

	const size_t hash_index = name.hash % CLASS_HASHTABLE_SIZE;

	ClassNode* node = &POOL_CLASSES[hash_index];

	while (node->cur != NULL) {
		if (node->next != NULL) {
			node = node->next;
		} else {
			node->next = Arena_alloc(arena, sizeof(ClassNode));
			node->next->cur = NULL;
			node->next->next = NULL;
			break;
		}
	}

	node->cur = it;

	FLAG_SET(it->flags, CLASS_FLAG_IS_INIT);

	return it;
}

Class* Class_get(const HashStr name) {
	const size_t hash_index = name.hash % CLASS_HASHTABLE_SIZE;
	const ClassNode *cur = &POOL_CLASSES[hash_index];
	while (cur->cur != NULL) {
		if (HashStr_equal(&cur->cur->name, &name)) {
			return cur->cur;
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
	for (size_t i = 0; i < clazz->trait_impl_count; ++i) {
		const TraitImpl* impl = &clazz->traits_impl[i];
		if (
			Trait_equal(impl->trait, trait)
		) {
			return true;
		}
	}
	return false;
}