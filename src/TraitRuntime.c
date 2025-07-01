#include "TraitRuntime.h"

#include <assert.h>
#include <stdbool.h>
#include <string.h>

#include "TraitRuntime/Class.h"
#include "TraitRuntime/Trait.h"
#include "TraitRuntime/Commons/ErrorHandling.h"
#include "TraitRuntime/Memory/Arena.h"


// =====================================
// MARK: BUILT IN TYPE
// =====================================

Container_BuiltIn BuiltIn;

// =====================================
// MARK: GENERAL
// =====================================

static bool IS_INIT = false;
Arena* ARENA_GLOBAL;

// =====================================
// MARK: GENERAL
// =====================================

static void INTERNAL_initialize_built_in_traits_and_types(void) {
	BuiltIn.traits.Finalizable.trait = TR_TRAIT("Finalizable");
	BuiltIn.traits.Finalizable.methods.finalize = Trait_addMethod(BuiltIn.traits.Finalizable.trait, HASH_STR("finalize"), TR_PARAMS());

	BuiltIn.traits.Constructable.trait = TR_TRAIT("Constructable");
	BuiltIn.traits.Constructable.methods.construct = Trait_addMethod(BuiltIn.traits.Constructable.trait, HASH_STR("construct"), TR_PARAMS());

	BuiltIn.classes.UInt8	= TR_CLASS("UInt8", uint8_t);
	BuiltIn.classes.UInt16	= TR_CLASS("UInt16", uint16_t);
	BuiltIn.classes.UInt32	= TR_CLASS("UInt32", uint32_t);
	BuiltIn.classes.UInt64	= TR_CLASS("UInt64", uint64_t);
	BuiltIn.classes.Int8	= TR_CLASS("Int8", int8_t);
	BuiltIn.classes.Int16	= TR_CLASS("Int16", int16_t);
	BuiltIn.classes.Int32	= TR_CLASS("Int32", int32_t);
	BuiltIn.classes.Int64	= TR_CLASS("Int64", int64_t);
	BuiltIn.classes.Float32	= TR_CLASS("Float32", float);
	BuiltIn.classes.Float64	= TR_CLASS("Float64", double);
}

void TraitRuntime_init(const bool enable_builtin) {
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
	IS_INIT = false;
}