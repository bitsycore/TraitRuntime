//
// Created by DWA on 02/07/2025.
//

#include "Runtime.h"

#include <assert.h>
#include <stdbool.h>

#include "TraitRuntime/BuiltIn.h"
#include "TraitRuntime/Class.h"
#include "TraitRuntime/Trait.h"
#include "TraitRuntime/Commons/ErrorHandling.h"
#include "TraitRuntime/Memory/Arena.h"

// =====================================
// MARK: GENERAL
// =====================================

static bool IS_INIT = false;
Arena* ARENA_GLOBAL;

// =====================================
// MARK: GENERAL
// =====================================

static void INTERNAL_initialize_built_in_traits_and_types(void) {
	BuiltIn.traits.Finalizable.trait = Trait_create(HASH_STR("Finalizable"));
	BuiltIn.traits.Finalizable.methods.finalize =
		Trait_addMethod(BuiltIn.traits.Finalizable.trait, HASH_STR("finalize"), NULL, 0);

	BuiltIn.traits.Constructable.trait = Trait_create(HASH_STR("Constructable"));
	BuiltIn.traits.Constructable.methods.construct =
		Trait_addMethod(BuiltIn.traits.Constructable.trait, HASH_STR("construct"), NULL, 0);

	BuiltIn.classes.UInt8	= Class_create(HASH_STR("UInt8"), sizeof(uint8_t));
	BuiltIn.classes.UInt16	= Class_create(HASH_STR("UInt16"), sizeof(uint16_t));
	BuiltIn.classes.UInt32	= Class_create(HASH_STR("UInt32"), sizeof(uint32_t));
	BuiltIn.classes.UInt64	= Class_create(HASH_STR("UInt64"), sizeof(uint64_t));
	BuiltIn.classes.Int8	= Class_create(HASH_STR("Int8"), sizeof(int8_t));
	BuiltIn.classes.Int16	= Class_create(HASH_STR("Int16"), sizeof(int16_t));
	BuiltIn.classes.Int32	= Class_create(HASH_STR("Int32"), sizeof(int32_t));
	BuiltIn.classes.Int64	= Class_create(HASH_STR("Int64"), sizeof(int64_t));
	BuiltIn.classes.Float32	= Class_create(HASH_STR("Float32"), sizeof(float));
	BuiltIn.classes.Float64	= Class_create(HASH_STR("Float64"), sizeof(double));

	FLAG_SET(BuiltIn.classes.UInt8->flags, CLASS_FLAG_IS_BUILTIN );
	FLAG_SET(BuiltIn.classes.UInt16->flags, CLASS_FLAG_IS_BUILTIN );
	FLAG_SET(BuiltIn.classes.UInt32->flags, CLASS_FLAG_IS_BUILTIN );
	FLAG_SET(BuiltIn.classes.UInt64->flags, CLASS_FLAG_IS_BUILTIN );
	FLAG_SET(BuiltIn.classes.Int8->flags, CLASS_FLAG_IS_BUILTIN );
	FLAG_SET(BuiltIn.classes.Int16->flags, CLASS_FLAG_IS_BUILTIN );
	FLAG_SET(BuiltIn.classes.Int32->flags, CLASS_FLAG_IS_BUILTIN );
	FLAG_SET(BuiltIn.classes.Int64->flags, CLASS_FLAG_IS_BUILTIN );
	FLAG_SET(BuiltIn.classes.Float32->flags, CLASS_FLAG_IS_BUILTIN );
	FLAG_SET(BuiltIn.classes.Float64->flags, CLASS_FLAG_IS_BUILTIN );
}

void Runtime_init(const bool enable_builtin) {
	const size_t arena_size = Arena_requiredSize(64*1024*1024);
	ARENA_GLOBAL = Arena_init(malloc(arena_size), arena_size);

	IS_INIT = true;
	if (enable_builtin == false) return;

	// ==============================================
	// LOAD ALL BUILT IN TYPES AND TRAIT
	INTERNAL_initialize_built_in_traits_and_types();
}

Arena* Internal_Runtime_getArena() {
	return ARENA_GLOBAL;
}

void Runtime_clean() {
	if (IS_INIT == false) {
		WARN("TraitRuntime_clean() called but Runtime not init or already cleaned");
		return;
	}
	IS_INIT = false;
}