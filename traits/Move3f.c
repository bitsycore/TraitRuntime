#include "Move3f.h"

Trait* trait_Move3f;
Method* method_Move3f_move;
Method* method_Move3f_moveX;
Method* method_Move3f_moveY;
Method* method_Move3f_moveZ;

void Move3f_loadTrait() {
	trait_Move3f = TRAIT("Move3f");
	method_Move3f_move = Trait_addMethod(trait_Move3f, HASH_STR("move"), DEF_PARAM("x", "y", "z"));
	method_Move3f_moveX = Trait_addMethod(trait_Move3f, HASH_STR("moveX"), DEF_PARAM("y"));
	method_Move3f_moveY = Trait_addMethod(trait_Move3f, HASH_STR("moveY"), DEF_PARAM("y"));
	method_Move3f_moveZ = Trait_addMethod(trait_Move3f, HASH_STR("moveZ"), DEF_PARAM("z"));
}