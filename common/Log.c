#include "Log.h"

#include <stdarg.h>
#include <stdio.h>

const char* levelToString(const LogLevel level) {
	switch (level) {
		case DEBUG: return "DEBUG";
		case INFO: return "INFO";
		case WARN: return "WARN";
		case ERROR: return "ERROR";
		default: return NULL;
	}
}

void send_log(const LogLevel level, const char* format, ...) {
	const char* log_string = levelToString(level);
	if (log_string != NULL) {
		printf("[%s] ", log_string);
	}
	va_list args;
	va_start(args, format);
	vprintf(format, args);
	va_end(args);
}
