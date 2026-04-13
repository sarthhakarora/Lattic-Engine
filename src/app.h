#pragma once

#include "core.h"
#include "raylib.h"
#include "scriptdb.h"
#include <stdbool.h>

typedef enum { APP_STATE_MENU, APP_STATE_SETTINGS } Appstate;

typedef struct {
  int total;
  int done;
} LoadTracker;

void app_run(Core *core, bool *program_active);
