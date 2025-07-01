#include "Move3f.h"

#include "TraitRuntime/Trait.h"

Container_Move3f trMove3f;

void Move3f_loadTrait() {
	trMove3f.trait = TR_TRAIT("Move3f");
	TR_USE(Trait, trMove3f.trait) {
		trMove3f.methods.move  = TR_TRAIT_ADD_METHOD(it, "move", "x", "y", "z");
		trMove3f.methods.moveX = TR_TRAIT_ADD_METHOD(it, "moveX", "y");
		trMove3f.methods.moveY = TR_TRAIT_ADD_METHOD(it, "moveY", "y");
		trMove3f.methods.moveZ = TR_TRAIT_ADD_METHOD(it, "moveZ", "z");
	}
}