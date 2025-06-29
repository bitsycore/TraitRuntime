#ifndef TRAIT_RUNTIME_H
#define TRAIT_RUNTIME_H

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "HashStr.h"

#define MAX_TRAITS 16
#define MAX_TRAIT_IMPLS 16
#define MAX_TYPE 16

#define MAX_METHODS_PER_TRAITS 8
#define MAX_PARAMS_PER_METHODS 8

typedef struct {
	HashStr name;
	size_t size;
} Type;

typedef struct {
	Type* type;
	void* data;
} Object;

struct Trait;

typedef struct {
	HashStr name;
	HashStr param_types[MAX_PARAMS_PER_METHODS];
	size_t param_count;
	struct Trait* trait;
} Method;

typedef struct {
	HashStr name;
	Method methods[MAX_METHODS_PER_TRAITS];
	size_t method_count;
} Trait;

typedef struct {
	const Object* object;
	const Trait* trait;
	const Method* method;
} MethodContext;

typedef void* (*MethodImpl)(MethodContext CTX, va_list argv);

typedef struct {
	Type* type;
	Trait* trait;
	MethodImpl methods[MAX_METHODS_PER_TRAITS];
} TraitImpl;

// ===================================
// General
void TraitRuntime_init();
void TraitRuntime_clean();

// ===================================
// Type
Type* Type_create(HashStr name, size_t size);
Type* Type_get(HashStr name);
bool Type_implement(const Type* type, const Trait* trait);

// ===================================
// Trait
Trait* Trait_create(HashStr name);
Trait* Trait_get(HashStr name);
Method* Trait_addMethod(Trait* trait, HashStr method_name, const HashStr* param_types, size_t param_count);
Method* Trait_getMethod(const Trait* trait, HashStr method_name);
bool Trait_equal(const Trait* this, const Trait* other);

// ===================================
// TraitImpl
TraitImpl* TraitImpl_create(Trait* trait, Type* type);
void TraitImpl_addMethod(TraitImpl* trait_impl, const Method* method, MethodImpl method_impl);
TraitImpl* TraitImpl_get(const Type* type, const Trait* trait);
MethodImpl TraitImpl_getMethodImpl(const TraitImpl* impl, HashStr method_name);

// ===================================
// Object
Object* Object_new(Type* type);
Object* Object_newFrom(Type* type, void* data);
MethodImpl Object_getMethod(const Object* obj, HashStr trait_name, HashStr method_name);
bool Object_is(const Object* obj, const Type* type);
void Object_finalize(Object* obj);
void* Object_call(const Object* obj, const Method* method, ...);
void* Object_callStr(const Object* obj, HashStr trait_name, HashStr method_name, ... );

// ======================================================================================
// BUILT IN TYPE
// ======================================================================================

extern Type* UInt8;
extern Type* UInt16;
extern Type* UInt32;
extern Type* UInt64;

extern Type* Int8;
extern Type* Int16;
extern Type* Int32;
extern Type* Int64;

extern Type* Float32;
extern Type* Float64;

extern Trait* trait_Finalizable;
extern Method* method_Finalizable_finalize;

// ======================================================================================
// MACRO PRIVATE UTILS
// ======================================================================================

#define TR___MAP0(m,x1)
#define TR___MAP1(m,x1,...) m(x1), TR___MAP0(m,__VA_ARGS__)
#define TR___MAP2(m,x1,...) m(x1), TR___MAP1(m,__VA_ARGS__)
#define TR___MAP3(m,x1,...) m(x1), TR___MAP2(m,__VA_ARGS__)
#define TR___MAP4(m,x1,...) m(x1), TR___MAP3(m,__VA_ARGS__)
#define TR___MAP5(m,x1,...) m(x1), TR___MAP4(m,__VA_ARGS__)
#define TR___MAP6(m,x1,...) m(x1), TR___MAP5(m,__VA_ARGS__)
#define TR___MAP7(m,x1,...) m(x1), TR___MAP6(m,__VA_ARGS__)
#define TR___MAP8(m,x1,...) m(x1), TR___MAP7(m,__VA_ARGS__)
#define TR___MAP9(m,x1,...) m(x1), TR___MAP8(m,__VA_ARGS__)
#define TR___MAP10(m,x1,...) m(x1), TR___MAP9(m,__VA_ARGS__)
#define TR___MAP11(m,x1,...) m(x1), TR___MAP10(m,__VA_ARGS__)
#define TR___MAP12(m,x1,...) m(x1), TR___MAP11(m,__VA_ARGS__)
#define TR___MAP13(m,x1,...) m(x1), TR___MAP12(m,__VA_ARGS__)
#define TR___MAP14(m,x1,...) m(x1), TR___MAP13(m,__VA_ARGS__)
#define TR___MAP15(m,x1,...) m(x1), TR___MAP14(m,__VA_ARGS__)
#define TR___MAP16(m,x1,...) m(x1), TR___MAP15(m,__VA_ARGS__)
#define TR___MAP17(m,x1,...) m(x1), TR___MAP16(m,__VA_ARGS__)
#define TR___MAP18(m,x1,...) m(x1), TR___MAP17(m,__VA_ARGS__)
#define TR___MAP19(m,x1,...) m(x1), TR___MAP18(m,__VA_ARGS__)
#define TR___MAP20(m,x1,...) m(x1), TR___MAP19(m,__VA_ARGS__)

#define TR___GET_MACRO(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16, _17,_18,_19,_20,NAME,...) NAME

#define TR___MAP(m, ...) TR___GET_MACRO(__VA_ARGS__, \
	TR___MAP20, TR___MAP19, TR___MAP18, TR___MAP17, TR___MAP16, TR___MAP15, \
	TR___MAP14, TR___MAP13, TR___MAP12, TR___MAP11, TR___MAP10, TR___MAP9, TR___MAP8, \
	TR___MAP7, TR___MAP6, TR___MAP5, TR___MAP4, TR___MAP3, TR___MAP2, TR___MAP1, TR___MAP0 \
)(m, __VA_ARGS__)

#define TR_____COUNT_ARGS(...) TR_____COUNT_ARGS_(,##__VA_ARGS__,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0)
#define TR_____COUNT_ARGS_(z,a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,cnt,...) cnt

#define USE(T, traitName) for (T * it = traitName; it != NULL; it = NULL)

// =========================================
// METHOD DEFINITION
// =========================================

#define TYPE(name, data) \
	Type_create(HASH_STR(name), sizeof(data))

#define TRAIT(name) \
	Trait_create(HASH_STR(name))

#define DEF_PARAM(...) TR_____COUNT_ARGS(__VA_ARGS__) == 0 ? NULL : (HashStr[]){ TR___MAP(HASH_STR, __VA_ARGS__) }, TR_____COUNT_ARGS(__VA_ARGS__)

#define TRAIT_IMPL(traitName, typeName) \
	TraitImpl_create(Trait_get(HASH_STR(traitName)), Type_get(HASH_STR(typeName)))

#define TRAIT_IMPL_METHOD(traitImpl, methodName, fn) \
    TraitImpl_addMethod(traitImpl, Trait_getMethod(traitImpl->trait, HASH_STR(methodName)), fn)

#define CALL(obj, traitName, methodName, ...) \
	Object_callStr(obj, HASH_STR(traitName), HASH_STR(methodName) __VA_OPT__(,) __VA_ARGS__) \

// =========================================
// METHOD DEFINITION
// =========================================

#define METHOD_UNWRAP_START(objTypeName, traitTypeName, methodTypeName) \
	CTX->object->data; \
	assert(HashStr_equal(&CTX->object->type->name, &HASH_STR(objTypeName))); \
	assert(HashStr_equal(&CTX->trait->name, &HASH_STR(traitTypeName))); \
	assert(HashStr_equal(&CTX->method->name, &HASH_STR(methodTypeName))); \
	unsigned int HIDDEN___count = 0\

#define ARG_UNWRAP(type) \
	va_arg(args, type); HIDDEN___count++

#define METHOD_UNWRAP_END() \
	assert(HIDDEN___count == CTX->method->param_count)

#endif