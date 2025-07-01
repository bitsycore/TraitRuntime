#include "ErrorHandling.h"

#include <string.h>

#include "AlertDialog.h"
#include "Callstack.h"

void eh___handle_error(
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

    // Calculate the length of the file/line/function info
    const int location_len = snprintf(NULL, 0, "\nFile: %s:%d\nFunction: %s\n", file, line, func_name);

    // Callstack & its length
    size_t callstack_len = 0;
    char *callstack = Callstack_AllocStr(2);
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

    // Append the callstack if it exists
    if (callstack_len > 0) {
        snprintf(message_buffer + offset, total_len + 1 - offset, "%s%s", callstack_prefix, callstack);
    }

    // Log console
    fprintf(stderr, "\n---- [%s] ----\n%s-------------------\n", type_str, message_buffer);

    // Dialog if available
    AlertDialog_Error(message_buffer);

    free(message_buffer);
    free(callstack);
}
