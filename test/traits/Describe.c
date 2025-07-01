#include "Describe.h"

#include "TraitRuntime/Trait.h"

Container_Describe trDescribe;

void Describe_loadTrait() {
	trDescribe.trait = TR_TRAIT("Describe");
	trDescribe.methods.toString = Trait_addMethod(trDescribe.trait, HASH_STR("toString"), TR_PARAMS());
}