#pragma once

#include "raylib.h"

#include "camera.h"
#include "string.h"
#include "world.h"
#include "planet.h"

typedef struct {
    const int screenWidth;
    const int screenHeight;   
    bool core_active;

    Cam camera;
    World active_world;
    
} Core;

typedef struct {
    bool show_logs;
} CoreArgs;

Core core_create(CoreArgs args);
CoreArgs core_parce_args(int argc, char **argv);
void core_run(Core *core);
void core_destroy(Core *core);