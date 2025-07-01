#include "Describe.h"

Container_Describe Describe;

void Describe_loadTrait() {
	Describe.trait = TR_TRAIT("Describe");
	Describe.methods.toString = Trait_addMethod(Describe.trait, HASH_STR("toString"), TR_PARAMS());
}