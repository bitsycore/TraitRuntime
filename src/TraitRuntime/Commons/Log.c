
#include "Log.h"

#include <stdarg.h>
#include <stdio.h>

#ifdef _WIN32
#include <windows.h>
#include <io.h>
#endif

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"

#ifdef _WIN32
static void enableWindowsAnsiSupport() {
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hOut == INVALID_HANDLE_VALUE) return;

	DWORD dwMode = 0;
	if (!GetConsoleMode(hOut, &dwMode)) return;

	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	SetConsoleMode(hOut, dwMode);
}
#endif

static const char* levelToString(const LogLevel level) {
	switch (level) {
		case DEBUG: return BLUE "DEBUG" RESET;
		case INFO: return GREEN "INFO" RESET;
		case WARN: return YELLOW "WARN" RESET;
		case _ERROR: return RED "ERROR" RESET;
		default: return NULL;
	}
}

void Log_send(const LogLevel level, const char* format, ...) {
#ifdef _WIN32
	static int initialized = 0;
	if (!initialized) {
		enableWindowsAnsiSupport();
		initialized = 1;
	}
#endif

	const char* log_string = levelToString(level);
	if (log_string != NULL) {
		printf("[%s] ", log_string);
	}
	va_list args;
	va_start(args, format);
	vprintf(format, args);
	va_end(args);
}