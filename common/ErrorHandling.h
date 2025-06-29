#ifndef ERROR_HANDLING_H
#define ERROR_HANDLING_H

#include <stdio.h>
#include <stdlib.h>

#include "Debug.h"

#ifndef NDEBUG

#define ERR_HANDLING___STRINGIFY(x) #x

// ================================
// MARK: Warnings
// ================================

#define WARN(msg) do { \
    char* callstack__ = print_call_stack();\
    fprintf(stderr, "\n---- [WARNING] ----\nInfo: %s\nFile: %s:%d\nFunction: %s\nCallstack:\n%s-------------------\n", msg, __FILE__, __LINE__, __func__, callstack__); \
    free(callstack__); \
} while (0)

#define WARN_FORMAT(msg, ...) do { \
    char* callstack__ = print_call_stack();\
    fprintf(stderr, "\n---- [WARNING] ----\nInfo: " msg "\nFile: %s:%d\nFunction: %s\nCallstack:\n%s-------------------\n", __VA_ARGS__, __FILE__, __LINE__, __func__, callstack__); \
    free(callstack__); \
} while (0)

#define WARN_IF(expr, msg) do { \
    if ((expr)) { \
        char* callstack__ = print_call_stack();\
        fprintf(stderr, "\n---- [WARNING] ----\nExpression: %s\nInfo: %s\nFile: %s:%d\nFunction: %s\nCallstack:\n%s-------------------\n", ERR_HANDLING___STRINGIFY(expr), msg, __FILE__, __LINE__, __func__, callstack__); \
        free(callstack__); \
    } \
} while (0)

#define WARN_IF_NOT(expr, msg) do { \
    if (!(expr)) { \
        char* callstack__ = print_call_stack();\
        fprintf(stderr, "\n---- [WARNING] ----\nExpression: %s\nInfo: %s\nFile: %s:%d\nFunction: %s\nCallstack:\n%s-------------------\n", ERR_HANDLING___STRINGIFY(expr), msg, __FILE__, __LINE__, __func__, callstack__); \
        free(callstack__); \
    } \
} while (0)

#define WARN_IF_EQUAL(expected, actual, msg) do { \
    if ((expected) == (actual)) { \
        char* callstack__ = print_call_stack();\
        fprintf(stderr, "\n---- [WARNING] ----\n%s\nEQUAL\n%s\nInfo: %s\nFile: %s:%d\nFunction: %s\nCallstack:\n%s-------------------\n", \
        ERR_HANDLING___STRINGIFY(expected), ERR_HANDLING___STRINGIFY(actual), msg, __FILE__, __LINE__, __func__, callstack__); \
        free(callstack__); \
    } \
} while (0)

#define WARN_IF_NOT_EQUAL(expected, actual, msg) do { \
    if ((expected) != (actual)) { \
        char* callstack__ = print_call_stack();\
        fprintf(stderr, "\n---- [WARNING] ----\n%s\nNOT_EQUAL\n%s\nInfo: %s\nFile: %s:%d\nFunction: %s\nCallstack:\n%s-------------------\n", \
        ERR_HANDLING___STRINGIFY(expected), ERR_HANDLING___STRINGIFY(actual), msg, __FILE__, __LINE__, __func__, callstack__); \
        free(callstack__); \
    } \
} while (0)

// =================================
// MARK: Critical Errors
// =================================

#define EXIT(msg, ...) do { \
    char* callstack__ = print_call_stack();\
    if (callstack__ == NULL || callstack__[0] == '\n')\
        fprintf(stderr, "\n----- [ERROR] -----\nInfo: " msg "\nFile: %s:%d\nFunction: %s\nCallstack\n%s-------------------\n", __VA_ARGS__ __VA_OPT__(,) __FILE__, __LINE__, __func__, callstack__); \
    else\
        fprintf(stderr, "\n----- [ERROR] -----\nInfo: " msg "\nFile: %s:%d\nFunction: %s\n-------------------\n", __VA_ARGS__ __VA_OPT__(,) __FILE__, __LINE__, __func__); \
    free(callstack__); \
    exit(EXIT_FAILURE); \
} while (0)

#define EXIT_IF(expr, msg) do { \
    if ((expr)) { \
        char* callstack__ = print_call_stack();\
        if(callstack__ == NULL || callstack__[0] == '\n')\
            fprintf(stderr, "\n----- [ERROR] -----\nExpression: %s\nInfo: %s\nFile: %s:%d\nFunction: %s\n-------------------\n", ERR_HANDLING___STRINGIFY(expr), msg, __FILE__, __LINE__, __func__); \
        else \
            fprintf(stderr, "\n----- [ERROR] -----\nExpression: %s\nInfo: %s\nFile: %s:%d\nFunction: %s\nCallstack\n%s-------------------\n", ERR_HANDLING___STRINGIFY(expr), msg, __FILE__, __LINE__, __func__, callstack__); \
        free(callstack__); \
        exit(EXIT_FAILURE); \
    } \
} while (0)

#define EXIT_IF_NOT(expr, msg, ...) do { \
    if (!(expr)) { \
        char* callstack__ = print_call_stack();\
        if(callstack__ == NULL || callstack__[0] == '\n')\
            fprintf(stderr, "\n----- [ERROR] -----\nExpression: %s\nInfo: " msg "\nFile: %s:%d\nFunction: %s\n-------------------\n", ERR_HANDLING___STRINGIFY(expr), __VA_ARGS__ __VA_OPT__(,) __FILE__, __LINE__, __func__); \
        else \
            fprintf(stderr, "\n----- [ERROR] -----\nExpression: %s\nInfo: " msg "\nFile: %s:%d\nFunction: %s\nCallstack\n%s-------------------\n", ERR_HANDLING___STRINGIFY(expr), __VA_ARGS__ __VA_OPT__(,) __FILE__, __LINE__, __func__, callstack__); \
        free(callstack__); \
    exit(EXIT_FAILURE); \
    } \
} while (0)

#define EXIT_IF_NOT_EQUAL(expected, actual, msg) do { \
    if ((expected) != (actual)) { \
        char* callstack__ = print_call_stack(); \
        if(callstack__ == NULL || callstack__[0] == '\n')\
            fprintf(stderr, "\n----- [ERROR] -----\n%s\nEQUAL\n%s\nInfo: %s\nFile: %s:%d\nFunction: %s\n-------------------\n", \
            ERR_HANDLING___STRINGIFY(expected), ERR_HANDLING___STRINGIFY(actual), msg, __FILE__, __LINE__, __func__); \
        else \
            fprintf(stderr, "\n----- [ERROR] -----\n%s\nEQUAL\n%s\nInfo: %s\nFile: %s:%d\nFunction: %s\nCallstack\n%s-------------------\n", \
            ERR_HANDLING___STRINGIFY(expected), ERR_HANDLING___STRINGIFY(actual), msg, __FILE__, __LINE__, __func__, callstack__); \
        free(callstack__); \
        exit(EXIT_FAILURE); \
    } \
} while (0)

#define EXIT_IF_EQUAL(expected, actual, msg) do { \
if ((expected) == (actual)) { \
        char* callstack__ = print_call_stack(); \
        if(callstack__ == NULL || callstack__[0] == '\n')\
            fprintf(stderr, "\n----- [ERROR] -----\n%s\nEQUAL\n%s\nInfo: %s\nFile: %s:%d\nFunction: %s\n-------------------\n", \
            ERR_HANDLING___STRINGIFY(expected), ERR_HANDLING___STRINGIFY(actual), msg, __FILE__, __LINE__, __func__); \
        else \
            fprintf(stderr, "\n----- [ERROR] -----\n%s\nEQUAL\n%s\nInfo: %s\nFile: %s:%d\nFunction: %s\nCallstack\n%s-------------------\n", \
            ERR_HANDLING___STRINGIFY(expected), ERR_HANDLING___STRINGIFY(actual), msg, __FILE__, __LINE__, __func__, callstack__); \
        free(callstack__); \
        exit(EXIT_FAILURE); \
    } \
} while (0)

#else

#include <assert.h>

#define WARN(msg)
#define WARN_FORMAT(msg, ...)
#define WARN_IF(expr, msg)
#define WARN_IF_NOT(expr, msg)
#define WARN_IF_EQUAL(expected, actual, msg)
#define WARN_IF_NOT_EQUAL(expected, actual, msg)

#define EXIT(msg)
#define EXIT_IF(expr, msg)
#define EXIT_IF_NOT(expr, msg)
#define EXIT_IF_NOT_EQUAL(expected, actual, msg)
#define EXIT_IF_EQUAL(expected, actual, msg)

#endif // NDEBUG

#endif //ERROR_HANDLING_H