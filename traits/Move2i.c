#include "Move2i.h"

Trait* trait_Move2i;
Method* method_Move2i_move;
Method* method_Move2i_moveX;
Method* method_Move2i_moveY;

void Move2i_loadTrait() {
	trait_Move2i = TRAIT("Move2i");
	method_Move2i_move = Trait_addMethod(trait_Move2i, HASH_STR("move"), DEF_PARAM("x", "y"));
	method_Move2i_moveX = Trait_addMethod(trait_Move2i, HASH_STR("moveX"), DEF_PARAM("x"));
	method_Move2i_moveY = Trait_addMethod(trait_Move2i, HASH_STR("moveY"), DEF_PARAM("y"));
}
