#ifndef ERROR_HANDLING_H
#define ERROR_HANDLING_H

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef NDEBUG

// ================================
// MARK: Internal Helper Function
// ================================

void INTERNAL_ErrorHandling_printError(
    const char *type_str,
    const char *file,
    int line,
    const char *func_name,
    const char *header_fmt,
    ...
);

// ================================
// MARK: Warnings
// ================================

#define WARN(msg, ...) \
    INTERNAL_ErrorHandling_printError("WARNING", __FILE__, __LINE__, __func__, "Info: "/*glue*/msg, ##__VA_ARGS__)

#define WARN_IF(expr, msg, ...) do { \
    if ((expr)) { \
        INTERNAL_ErrorHandling_printError("WARNING", __FILE__, __LINE__, __func__, "Expression: %s\nInfo: "/*glue*/msg, #expr, ##__VA_ARGS__); \
    } \
} while (0)

#define WARN_IF_NOT(expr, msg, ...) do { \
    if (!(expr)) { \
        INTERNAL_ErrorHandling_printError("WARNING", __FILE__, __LINE__, __func__, "Expression: !(%s)\nInfo: "/*glue*/msg, #expr, ##__VA_ARGS__); \
    } \
} while (0)

#define WARN_IF_EQUAL(expected, actual, msg, ...) do { \
    if ((expected) == (actual)) { \
        INTERNAL_ErrorHandling_printError("WARNING", __FILE__, __LINE__, __func__, "%s\nEQUAL\n%s\nInfo: "/*glue*/msg, #expected, #actual, ##__VA_ARGS__); \
    } \
} while (0)

#define WARN_IF_NOT_EQUAL(expected, actual, msg, ...) do { \
    if ((expected) != (actual)) { \
        INTERNAL_ErrorHandling_printError("WARNING", __FILE__, __LINE__, __func__, "%s\nNOT EQUAL\n%s\nInfo: "/*glue*/msg, #expected, #actual, ##__VA_ARGS__); \
    } \
} while (0)


// =================================
// MARK: Critical Errors
// =================================

#define EXIT(msg, ...) \
    INTERNAL_ErrorHandling_printError("ERROR", __FILE__, __LINE__, __func__, "Info: " msg, ##__VA_ARGS__);exit(EXIT_FAILURE)\

#define EXIT_IF(expr, msg, ...) do { \
    if ((expr)) { \
        INTERNAL_ErrorHandling_printError("ERROR", __FILE__, __LINE__, __func__, "Expression: %s\nInfo: "/*glue*/msg, #expr, ##__VA_ARGS__); \
    exit(EXIT_FAILURE);\
    } \
} while (0)

#define EXIT_IF_NOT(expr, msg, ...) do { \
    if (!(expr)) { \
        INTERNAL_ErrorHandling_printError("ERROR", __FILE__, __LINE__, __func__, "Expression: !(%s)\nInfo: "/*glue*/msg, #expr, ##__VA_ARGS__); \
        exit(EXIT_FAILURE);\
    } \
} while (0)

#define EXIT_IF_EQUAL(expected, actual, msg, ...) do { \
    if ((expected) == (actual)) { \
        INTERNAL_ErrorHandling_printError("ERROR", __FILE__, __LINE__, __func__, "%s\nEQUAL\n%s\nInfo: "/*glue*/msg, #expected, #actual, ##__VA_ARGS__); \
        exit(EXIT_FAILURE);\
    } \
} while (0)

#define EXIT_IF_NOT_EQUAL(expected, actual, msg, ...) do { \
    if ((expected) != (actual)) { \
        INTERNAL_ErrorHandling_printError("ERROR", __FILE__, __LINE__, __func__, "%s\nNOT EQUAL\n%s\nInfo: "/*glue*/msg, #expected, #actual, ##__VA_ARGS__); \
        exit(EXIT_FAILURE);\
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