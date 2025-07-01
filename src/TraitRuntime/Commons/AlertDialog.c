#include "AlertDialog.h"

#ifdef _WIN32
#include <windows.h>
#endif

void AlertDialog_Error(const char* message) {
#if defined(_WIN32)
	MessageBoxA(NULL, message, "Error", MB_OK | MB_ICONERROR);
#elif defined(__APPLE__)
	// TODO obj-c
#elif defined(__linux__)

#endif
}
