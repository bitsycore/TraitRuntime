#include "Move3f.h"

Trait* trait_Move3f;
Method* method_Move3f_move;
Method* method_Move3f_moveX;
Method* method_Move3f_moveY;
Method* method_Move3f_moveZ;

void Move3f_loadTrait() {
	trait_Move3f = TRAIT("Move3f", Move3fData);
	method_Move3f_move = TRAIT_ADD_METHOD(trait_Move3f, "move", "x", "y", "z");
	method_Move3f_moveX = TRAIT_ADD_METHOD(trait_Move3f, "moveX", "y");
	method_Move3f_moveY = TRAIT_ADD_METHOD(trait_Move3f, "moveY", "y");
	method_Move3f_moveZ = TRAIT_ADD_METHOD(trait_Move3f, "moveZ", "z");
}