#include "Move2i.h"

Container_Move2i trMove2i;

void Move2i_loadTrait() {
	trMove2i.trait = TR_TRAIT("Move2i");
	TR_USE(Trait, trMove2i.trait) {
		trMove2i.methods.move = Trait_addMethod(it, HASH_STR("move"), TR_PARAMS("x", "y"));
		trMove2i.methods.moveX = Trait_addMethod(it, HASH_STR("moveX"), TR_PARAMS("x"));
		trMove2i.methods.moveY = Trait_addMethod(it, HASH_STR("moveY"), TR_PARAMS("y"));
	}
}