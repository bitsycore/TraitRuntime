#ifndef ERROR_HANDLING_H
#define ERROR_HANDLING_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

// Keep for print_call_stack()
#include "Debug.h"

#ifndef NDEBUG

// ================================
// MARK: Internal Helper Function
// ================================

static inline void _eh___handle_error(
    const int exit_code,  // 0 = no exit
    const char* type_str, // "WARNING" or "ERROR"
    const char* file,
    const int line,
    const char* func_name,
    const char* header_fmt,
    ...
) {
    // Header
    fprintf(stderr, "\n---- [%s] ----\n", type_str);

    // Print Expression and Infos
    va_list args;
    va_start(args, header_fmt);
    vfprintf(stderr, header_fmt, args);
    va_end(args);

    // Print the standard location info
    fprintf(stderr, "\nFile: %s:%d\nFunction: %s\n", file, line, func_name);

    // Get and print the callstack if available
    char* callstack = print_call_stack();
    if (callstack && callstack[0] != '\n' && callstack[0] != '\0') {
        fprintf(stderr, "Callstack:\n%s", callstack);
    }
    free(callstack);

    fprintf(stderr, "-------------------\n");

    if (exit_code != 0) {
        exit(exit_code);
    }
}

// ================================
// MARK: Warnings
// ================================

#define WARN(msg, ...) \
    _eh___handle_error(0, "WARNING", __FILE__, __LINE__, __func__, "Info: "/*glue*/msg, ##__VA_ARGS__)

#define WARN_IF(expr, msg, ...) do { \
    if ((expr)) { \
        _eh___handle_error(0, "WARNING", __FILE__, __LINE__, __func__, "Expression: %s\nInfo: "/*glue*/msg, #expr, ##__VA_ARGS__); \
    } \
} while (0)

#define WARN_IF_NOT(expr, msg, ...) do { \
    if (!(expr)) { \
        _eh___handle_error(0, "WARNING", __FILE__, __LINE__, __func__, "Expression: !(%s)\nInfo: "/*glue*/msg, #expr, ##__VA_ARGS__); \
    } \
} while (0)

#define WARN_IF_EQUAL(expected, actual, msg, ...) do { \
    if ((expected) == (actual)) { \
        _eh___handle_error(0, "WARNING", __FILE__, __LINE__, __func__, "%s\nEQUAL\n%s\nInfo: "/*glue*/msg, #expected, #actual, ##__VA_ARGS__); \
    } \
} while (0)

#define WARN_IF_NOT_EQUAL(expected, actual, msg, ...) do { \
    if ((expected) != (actual)) { \
        _eh___handle_error(0, "WARNING", __FILE__, __LINE__, __func__, "%s\nNOT EQUAL\n%s\nInfo: "/*glue*/msg, #expected, #actual, ##__VA_ARGS__); \
    } \
} while (0)


// =================================
// MARK: Critical Errors
// =================================

#define EXIT(msg, ...) \
    _eh___handle_error(EXIT_FAILURE, "ERROR", __FILE__, __LINE__, __func__, "Info: " msg, ##__VA_ARGS__)

#define EXIT_IF(expr, msg, ...) do { \
    if ((expr)) { \
        _eh___handle_error(EXIT_FAILURE, "ERROR", __FILE__, __LINE__, __func__, "Expression: %s\nInfo: "/*glue*/msg, #expr, ##__VA_ARGS__); \
    } \
} while (0)

#define EXIT_IF_NOT(expr, msg, ...) do { \
    if (!(expr)) { \
        _eh___handle_error(EXIT_FAILURE, "ERROR", __FILE__, __LINE__, __func__, "Expression: !(%s)\nInfo: "/*glue*/msg, #expr, ##__VA_ARGS__); \
    } \
} while (0)

#define EXIT_IF_EQUAL(expected, actual, msg, ...) do { \
    if ((expected) == (actual)) { \
        _eh___handle_error(EXIT_FAILURE, "ERROR", __FILE__, __LINE__, __func__, "%s\nEQUAL\n%s\nInfo: "/*glue*/msg, #expected, #actual, ##__VA_ARGS__); \
    } \
} while (0)

#define EXIT_IF_NOT_EQUAL(expected, actual, msg, ...) do { \
    if ((expected) != (actual)) { \
        _eh___handle_error(EXIT_FAILURE, "ERROR", __FILE__, __LINE__, __func__, "%s\nNOT EQUAL\n%s\nInfo: "/*glue*/msg, #expected, #actual, ##__VA_ARGS__); \
    } \
} while (0)


#else // NDEBUG is defined

#define WARN(...)
#define WARN_IF(...)
#define WARN_IF_NOT(...)
#define WARN_IF_EQUAL(...)
#define WARN_IF_NOT_EQUAL(...)

#define EXIT(...)
#define EXIT_IF(expr, ...)
#define EXIT_IF_NOT(expr, ...)
#define EXIT_IF_EQUAL(expected, actual, ...)
#define EXIT_IF_NOT_EQUAL(expected, actual, ...)

// #define EXIT(...)                                   exit(EXIT_FAILURE)
// #define EXIT_IF(expr, ...)                          do { if(expr) exit(EXIT_FAILURE); } while(0)
// #define EXIT_IF_NOT(expr, ...)                      do { if(!(expr)) exit(EXIT_FAILURE); } while(0)
// #define EXIT_IF_EQUAL(expected, actual, ...)        do { if((expected) == (actual)) exit(EXIT_FAILURE); } while(0)
// #define EXIT_IF_NOT_EQUAL(expected, actual, ...)    do { if((expected) != (actual)) exit(EXIT_FAILURE); } while(0)

#endif // NDEBUG

#endif //ERROR_HANDLING_H