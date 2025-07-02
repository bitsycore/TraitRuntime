#ifndef RUNTIMEMANAGER_H
#define RUNTIMEMANAGER_H

#include <stdbool.h>

#include "Memory/Arena.h"

// ===================================
// General
void Runtime_init(bool enable_builtin);
Arena* INTERNAL_Runtime_getArena();
void Runtime_clean();

#endif //RUNTIMEMANAGER_H
