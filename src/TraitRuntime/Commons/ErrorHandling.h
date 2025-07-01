#ifndef ERROR_HANDLING_H
#define ERROR_HANDLING_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

// Keep for debug_callstack()
#include <string.h>

#include "Debug.h"

#ifndef NDEBUG

// ================================
// MARK: Internal Helper Function
// ================================

static inline void _eh___handle_error(
    const char *type_str,
    const char *file,
    const int line,
    const char *func_name,
    const char *header_fmt,
    ...
) {
    // Calculate the length of the message
    va_list args;
    va_start(args, header_fmt);
    va_list args_copy;
    va_copy(args_copy, args);
    int vararg_length = vsnprintf(NULL, 0, header_fmt, args_copy);
    va_end(args_copy);

    if (vararg_length < 0) {
        vararg_length = 0;
    }

    // Calculate length of the file/line/function info
    const int location_len = snprintf(NULL, 0, "\nFile: %s:%d\nFunction: %s\n", file, line, func_name);

    // Callstack & its length
    int callstack_len = 0;
    char *callstack = debug_callstack(2);
    const char *callstack_prefix = "Callstack:\n";
    if (callstack && callstack[0] != '\n' && callstack[0] != '\0') {
        callstack_len = strlen(callstack_prefix) + strlen(callstack);
    }

    // Allocate memory total memory needed
    const size_t total_len = vararg_length + location_len + callstack_len;
    char *message_buffer  = malloc(total_len + 1);

    if (!message_buffer) {
        // Shouldn't happen bro
        fprintf(stderr, "\n---- [CRITICAL: MALLOC FAILED IN ERROR HANDLER] ----\n");
        fprintf(stderr, "Original Error in %s at %s:%d\n", func_name, file, line);
        va_end(args);
        free(callstack);
        abort();
        return;
    }

    // Formatted Message
    int offset = vsnprintf(message_buffer, vararg_length + 1, header_fmt, args);
    va_end(args);

    // Append loc infos
    offset += snprintf(message_buffer + offset, total_len + 1 - offset, "\nFile: %s:%d\nFunction: %s\n", file, line, func_name);

    // Append callstack if it exists
    if (callstack_len > 0) {
        snprintf(message_buffer + offset, total_len + 1 - offset, "%s%s", callstack_prefix, callstack);
    }

    // Log console
    fprintf(stderr, "\n---- [%s] ----\n%s-------------------\n", type_str, message_buffer);

    // Dialog if available
    debug_alert(message_buffer);

    free(message_buffer);
    free(callstack);
}

// ================================
// MARK: Warnings
// ================================

#define WARN(msg, ...) \
    _eh___handle_error("WARNING", __FILE__, __LINE__, __func__, "Info: "/*glue*/msg, ##__VA_ARGS__)

#define WARN_IF(expr, msg, ...) do { \
    if ((expr)) { \
        _eh___handle_error("WARNING", __FILE__, __LINE__, __func__, "Expression: %s\nInfo: "/*glue*/msg, #expr, ##__VA_ARGS__); \
    } \
} while (0)

#define WARN_IF_NOT(expr, msg, ...) do { \
    if (!(expr)) { \
        _eh___handle_error("WARNING", __FILE__, __LINE__, __func__, "Expression: !(%s)\nInfo: "/*glue*/msg, #expr, ##__VA_ARGS__); \
    } \
} while (0)

#define WARN_IF_EQUAL(expected, actual, msg, ...) do { \
    if ((expected) == (actual)) { \
        _eh___handle_error("WARNING", __FILE__, __LINE__, __func__, "%s\nEQUAL\n%s\nInfo: "/*glue*/msg, #expected, #actual, ##__VA_ARGS__); \
    } \
} while (0)

#define WARN_IF_NOT_EQUAL(expected, actual, msg, ...) do { \
    if ((expected) != (actual)) { \
        _eh___handle_error("WARNING", __FILE__, __LINE__, __func__, "%s\nNOT EQUAL\n%s\nInfo: "/*glue*/msg, #expected, #actual, ##__VA_ARGS__); \
    } \
} while (0)


// =================================
// MARK: Critical Errors
// =================================

#define EXIT(msg, ...) \
    _eh___handle_error("ERROR", __FILE__, __LINE__, __func__, "Info: " msg, ##__VA_ARGS__);exit(EXIT_FAILURE)\

#define EXIT_IF(expr, msg, ...) do { \
    if ((expr)) { \
        _eh___handle_error("ERROR", __FILE__, __LINE__, __func__, "Expression: %s\nInfo: "/*glue*/msg, #expr, ##__VA_ARGS__); \
    exit(EXIT_FAILURE);\
    } \
} while (0)

#define EXIT_IF_NOT(expr, msg, ...) do { \
    if (!(expr)) { \
        _eh___handle_error("ERROR", __FILE__, __LINE__, __func__, "Expression: !(%s)\nInfo: "/*glue*/msg, #expr, ##__VA_ARGS__); \
        exit(EXIT_FAILURE);\
    } \
} while (0)

#define EXIT_IF_EQUAL(expected, actual, msg, ...) do { \
    if ((expected) == (actual)) { \
        _eh___handle_error("ERROR", __FILE__, __LINE__, __func__, "%s\nEQUAL\n%s\nInfo: "/*glue*/msg, #expected, #actual, ##__VA_ARGS__); \
        exit(EXIT_FAILURE);\
    } \
} while (0)

#define EXIT_IF_NOT_EQUAL(expected, actual, msg, ...) do { \
    if ((expected) != (actual)) { \
        _eh___handle_error("ERROR", __FILE__, __LINE__, __func__, "%s\nNOT EQUAL\n%s\nInfo: "/*glue*/msg, #expected, #actual, ##__VA_ARGS__); \
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