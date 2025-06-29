#include "Move3f.h"

Trait* trait_Move3f;
Method* method_Move3f_move;
Method* method_Move3f_moveX;
Method* method_Move3f_moveY;
Method* method_Move3f_moveZ;

void Move3f_loadTrait() {
	trait_Move3f = TRAIT("Move3f");
	const HashStr params[] = {PARAM("x"), PARAM("y"), PARAM("z")};
	method_Move3f_move = Trait_addMethod(trait_Move3f, HASH_STR("move"), params, sizeof(params) / sizeof(HashStr));
	const HashStr params2[] = {PARAM("x")};
	method_Move3f_moveX = Trait_addMethod(trait_Move3f, HASH_STR("moveX"), params2, sizeof(params2) / sizeof(HashStr));
	const HashStr params3[] = {PARAM("y")};
	method_Move3f_moveY = Trait_addMethod(trait_Move3f, HASH_STR("moveY"), DEF_PARAM(PARAM("y")));
	method_Move3f_moveZ = Trait_addMethod(trait_Move3f, HASH_STR("moveZ"), (HashStr[]){ PARAM("z") }, 1);
}