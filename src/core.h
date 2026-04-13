#pragma once

#include "raylib.h"

#include "camera.h"
#include "lua.h"
#include "planet.h"
#include "string.h"
#include "world.h"

typedef struct {
  bool show_logs;
} CoreArgs;

typedef enum { CURSOR_UI, CURSOR_CAMERA } CursorMode;

typedef struct {
  const int screenWidth;
  const int screenHeight;
  bool core_active;
  bool program_active;

  float bootTime;
  float bootStart;
  bool isFirstFrame;

  lua_State *L;
  bool lua_initalzied;

  CursorMode cursor_mode;

  bool is_paused;
  bool isStarting;
  float progress;

  Cam camera;
  World active_world;
  int64_t next_world_id;

} Core;

void enable_cursor(Core *core);
void fullscreen();
void pause_time(World *world);
void core_draw(Core *core);
void handle_cursor_input(Core *core);

Core core_create(CoreArgs args);
CoreArgs core_parce_args(int argc, char **argv);
void core_run(Core *core);
void core_destroy(Core *core);
