#include "Move2i.h"

Container_Move2i Move2i;

void Move2i_loadTrait() {
	Move2i.trait = TR_TRAIT("Move2i");
	TR_USE(Trait, Move2i.trait) {
		Move2i.methods.move = Trait_addMethod(it, HASH_STR("move"), TR_PARAMS("x", "y"));
		Move2i.methods.moveX = Trait_addMethod(it, HASH_STR("moveX"), TR_PARAMS("x"));
		Move2i.methods.moveY = Trait_addMethod(it, HASH_STR("moveY"), TR_PARAMS("y"));
	}
}