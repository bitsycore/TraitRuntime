#include "Move3f.h"

Container_Move3f Move3f;

void Move3f_loadTrait() {
	Move3f.trait = TR_TRAIT("Move3f");
	TR_USE(Trait, Move3f.trait) {
		Move3f.methods.move  = TR_TRAIT_ADD_METHOD(it, "move", "x", "y", "z");
		Move3f.methods.moveX = TR_TRAIT_ADD_METHOD(it, "moveX", "y");
		Move3f.methods.moveY = TR_TRAIT_ADD_METHOD(it, "moveY", "y");
		Move3f.methods.moveZ = TR_TRAIT_ADD_METHOD(it, "moveZ", "z");
	}
}