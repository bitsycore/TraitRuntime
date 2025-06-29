#include "Format.h"

Trait* trait_Format;
Method* method_Format_toString;

void Format_loadTrait() {
	trait_Format = TRAIT("Format");
	method_Format_toString = Trait_addMethod(trait_Format, HASH_STR("toString"), DEF_PARAM());
}