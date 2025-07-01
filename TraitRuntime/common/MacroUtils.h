#ifndef MACROUTILS_H
#define MACROUTILS_H

#define _______MU_COUNT_ARGS_IMPL( _0, \
_1, _2, _3, _4, _5, _6, _7, _8, _9,_10,	\
_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,\
_21,_22,_23,_24,_25,_26,_27,_28,_29,_30,\
_31,_32,_33,_34,_35,_36,_37,_38,_39,_40,N,\
...) N

#define MU_COUNT_ARGS(...) _______MU_COUNT_ARGS_IMPL( \
    _, ##__VA_ARGS__, \
    40, \
    39, 38, 37, 36, 35, 34, 33, 32, 31, 30, \
    29, 28, 27, 26, 25, 24, 23, 22, 21, 20, \
    19, 18, 17, 16, 15, 14, 13, 12, 11, 10, \
    9,  8,  7,  6,  5,  4,  3,  2,  1,  0	\
)

// ----------------------------------------------------
// Optional param with at least ONE argument
// ----------------------------------------------------
// #define FUN_1(x,...) "fun_1"
// #define FUN_2(x, y, ...) "fun_2"
// #define FUN(...) MU_GET_MACRO_2(__VA_ARGS__, FUN_2, FUN_1)(__VA_ARGS__)

// ----------------------------------------------------
// Optional param with ZERO or more arguments
// ----------------------------------------------------
// #define FUN_0(...) "fun_0"
// #define FUN_1(x,...) "fun_1"
// #define FUN_2(x, y, ...) "fun_2"
// #define FUN(...) MU_GET_MACRO_3(_, ##__VA_ARGS__, FUN_2, FUN_1, FUN_0)(__VA_ARGS__)
                                /* ^-- Leading placeholder allows correct dispatch when __VA_ARGS__ is empty */

// ----------------------------------------------------
// First param required, rest optional.
// ----------------------------------------------------
// Can be repeated for more required params.
// variadic (...) required for fun before the last one (one more for each time repeated).
// #define FUN_1(x, ...) fun(x, 0)
// #define FUN_2(x, y)   fun(x, y)
// #define TRAIT(x, ...) MU_GET_MACRO_2(_, ##__VA_ARGS__, FUN_1, FUN_2)(x, __VA_ARGS__)

#define MU_GET_MACRO_1(_1, NAME, ...) NAME
#define MU_GET_MACRO_2(_1, _2, NAME, ...) NAME
#define MU_GET_MACRO_3(_1, _2, _3, NAME, ...) NAME
#define MU_GET_MACRO_4(_1, _2, _3, _4, NAME, ...) NAME
#define MU_GET_MACRO_5(_1, _2, _3, _4, _5, NAME, ...) NAME
#define MU_GET_MACRO_6(_1, _2, _3, _4, _5, _6, NAME, ...) NAME
#define MU_GET_MACRO_7(_1, _2, _3, _4, _5, _6, _7, NAME, ...) NAME
#define MU_GET_MACRO_8(_1, _2, _3, _4, _5, _6, _7, _8, NAME, ...) NAME
#define MU_GET_MACRO_9(_1, _2, _3, _4, _5, _6, _7, _8, _9, NAME, ...) NAME
#define MU_GET_MACRO_10(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, NAME, ...) NAME
#define MU_GET_MACRO_11(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, NAME, ...) NAME
#define MU_GET_MACRO_12(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, NAME, ...) NAME
#define MU_GET_MACRO_13(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, NAME, ...) NAME
#define MU_GET_MACRO_14(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, NAME, ...) NAME
#define MU_GET_MACRO_15(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, NAME, ...) NAME
#define MU_GET_MACRO_16(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, NAME, ...) NAME
#define MU_GET_MACRO_17(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, NAME, ...) NAME
#define MU_GET_MACRO_18(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, NAME, ...) NAME
#define MU_GET_MACRO_19(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, NAME, ...) NAME
#define MU_GET_MACRO_20(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, NAME, ...) NAME
#define MU_GET_MACRO_21(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, NAME, ...) NAME
#define MU_GET_MACRO_22(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, NAME, ...) NAME
#define MU_GET_MACRO_23(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, NAME, ...) NAME
#define MU_GET_MACRO_24(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, NAME, ...) NAME
#define MU_GET_MACRO_25(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, NAME, ...) NAME
#define MU_GET_MACRO_26(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, NAME, ...) NAME
#define MU_GET_MACRO_27(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, NAME, ...) NAME
#define MU_GET_MACRO_28(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, NAME, ...) NAME
#define MU_GET_MACRO_29(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, NAME, ...) NAME
#define MU_GET_MACRO_30(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, NAME, ...) NAME
#define MU_GET_MACRO_31(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, NAME, ...) NAME
#define MU_GET_MACRO_32(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, NAME, ...) NAME
#define MU_GET_MACRO_33(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, NAME, ...) NAME
#define MU_GET_MACRO_34(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, NAME, ...) NAME
#define MU_GET_MACRO_35(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, NAME, ...) NAME
#define MU_GET_MACRO_36(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, NAME, ...) NAME
#define MU_GET_MACRO_37(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, NAME, ...) NAME
#define MU_GET_MACRO_38(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, NAME, ...) NAME
#define MU_GET_MACRO_39(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, NAME, ...) NAME
#define MU_GET_MACRO_40(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, NAME, ...) NAME
#define MU_GET_MACRO_41(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, _41, NAME, ...) NAME

#define _______MU_MAP0(m,x1)
#define _______MU_MAP1(m,x1,...)  m(x1)  _______MU_MAP0(m,__VA_ARGS__)
#define _______MU_MAP2(m,x1,...)  m(x1), _______MU_MAP1(m,__VA_ARGS__)
#define _______MU_MAP3(m,x1,...)  m(x1), _______MU_MAP2(m,__VA_ARGS__)
#define _______MU_MAP4(m,x1,...)  m(x1), _______MU_MAP3(m,__VA_ARGS__)
#define _______MU_MAP5(m,x1,...)  m(x1), _______MU_MAP4(m,__VA_ARGS__)
#define _______MU_MAP6(m,x1,...)  m(x1), _______MU_MAP5(m,__VA_ARGS__)
#define _______MU_MAP7(m,x1,...)  m(x1), _______MU_MAP6(m,__VA_ARGS__)
#define _______MU_MAP8(m,x1,...)  m(x1), _______MU_MAP7(m,__VA_ARGS__)
#define _______MU_MAP9(m,x1,...)  m(x1), _______MU_MAP8(m,__VA_ARGS__)
#define _______MU_MAP10(m,x1,...) m(x1), _______MU_MAP9(m,__VA_ARGS__)
#define _______MU_MAP11(m,x1,...) m(x1), _______MU_MAP10(m,__VA_ARGS__)
#define _______MU_MAP12(m,x1,...) m(x1), _______MU_MAP11(m,__VA_ARGS__)
#define _______MU_MAP13(m,x1,...) m(x1), _______MU_MAP12(m,__VA_ARGS__)
#define _______MU_MAP14(m,x1,...) m(x1), _______MU_MAP13(m,__VA_ARGS__)
#define _______MU_MAP15(m,x1,...) m(x1), _______MU_MAP14(m,__VA_ARGS__)
#define _______MU_MAP16(m,x1,...) m(x1), _______MU_MAP15(m,__VA_ARGS__)
#define _______MU_MAP17(m,x1,...) m(x1), _______MU_MAP16(m,__VA_ARGS__)
#define _______MU_MAP18(m,x1,...) m(x1), _______MU_MAP17(m,__VA_ARGS__)
#define _______MU_MAP19(m,x1,...) m(x1), _______MU_MAP18(m,__VA_ARGS__)
#define _______MU_MAP20(m,x1,...) m(x1), _______MU_MAP19(m,__VA_ARGS__)
#define _______MU_MAP21(m,x1,...) m(x1), _______MU_MAP20(m,__VA_ARGS__)
#define _______MU_MAP22(m,x1,...) m(x1), _______MU_MAP21(m,__VA_ARGS__)
#define _______MU_MAP23(m,x1,...) m(x1), _______MU_MAP22(m,__VA_ARGS__)
#define _______MU_MAP24(m,x1,...) m(x1), _______MU_MAP23(m,__VA_ARGS__)
#define _______MU_MAP25(m,x1,...) m(x1), _______MU_MAP24(m,__VA_ARGS__)
#define _______MU_MAP26(m,x1,...) m(x1), _______MU_MAP25(m,__VA_ARGS__)
#define _______MU_MAP27(m,x1,...) m(x1), _______MU_MAP26(m,__VA_ARGS__)
#define _______MU_MAP28(m,x1,...) m(x1), _______MU_MAP27(m,__VA_ARGS__)
#define _______MU_MAP29(m,x1,...) m(x1), _______MU_MAP28(m,__VA_ARGS__)
#define _______MU_MAP30(m,x1,...) m(x1), _______MU_MAP29(m,__VA_ARGS__)
#define _______MU_MAP31(m,x1,...) m(x1), _______MU_MAP30(m,__VA_ARGS__)
#define _______MU_MAP32(m,x1,...) m(x1), _______MU_MAP31(m,__VA_ARGS__)
#define _______MU_MAP33(m,x1,...) m(x1), _______MU_MAP32(m,__VA_ARGS__)
#define _______MU_MAP34(m,x1,...) m(x1), _______MU_MAP33(m,__VA_ARGS__)
#define _______MU_MAP35(m,x1,...) m(x1), _______MU_MAP34(m,__VA_ARGS__)
#define _______MU_MAP36(m,x1,...) m(x1), _______MU_MAP35(m,__VA_ARGS__)
#define _______MU_MAP37(m,x1,...) m(x1), _______MU_MAP36(m,__VA_ARGS__)
#define _______MU_MAP38(m,x1,...) m(x1), _______MU_MAP37(m,__VA_ARGS__)
#define _______MU_MAP39(m,x1,...) m(x1), _______MU_MAP38(m,__VA_ARGS__)
#define _______MU_MAP40(m,x1,...) m(x1), _______MU_MAP39(m,__VA_ARGS__)

/**
 * @brief Maps a function-like macro across all variadic arguments
 *
 * - Supports 1 to 40 arguments
 * - The macro `m` must accept exactly one parameter
 * - Results are comma-separated for use in function calls, initializer lists, etc.
 *
 * @code{.h}
 * #define HASH(x) hash_##x
 * #define HASH_PARAMS(...) MU_MAP(HASH, __VA_ARGS__)
 *
 * HASH_PARAMS("Hello", "World", "Test")
 * // Expands to: HASH("Hello"), HASH("World"), HASH("Test")
 * @endcode
 *
 * @param m The function-like macro to apply to each argument
 * @param ... Variadic arguments (up to 40 arguments supported)
 */

#define MU_MAP(m, ...) MU_GET_MACRO_40(__VA_ARGS__, \
_______MU_MAP40, _______MU_MAP39, _______MU_MAP38, _______MU_MAP37, _______MU_MAP36, \
_______MU_MAP35, _______MU_MAP34, _______MU_MAP33, _______MU_MAP32, _______MU_MAP31, \
_______MU_MAP30, _______MU_MAP29, _______MU_MAP28, _______MU_MAP27, _______MU_MAP26, \
_______MU_MAP25, _______MU_MAP24, _______MU_MAP23, _______MU_MAP22, _______MU_MAP21, \
_______MU_MAP20, _______MU_MAP19, _______MU_MAP18, _______MU_MAP17, _______MU_MAP16, \
_______MU_MAP15, _______MU_MAP14, _______MU_MAP13, _______MU_MAP12, _______MU_MAP11, \
_______MU_MAP10, _______MU_MAP9,  _______MU_MAP8,  _______MU_MAP7,  _______MU_MAP6,  \
_______MU_MAP5,  _______MU_MAP4,  _______MU_MAP3,  _______MU_MAP2,  _______MU_MAP1  \
)(m, __VA_ARGS__)

#endif //MACROUTILS_H
