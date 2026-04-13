#include "assetmanager.h"
#include "platform/platform_win32.h"
#include "raylib.h"
#include "raymath.h"
#include "stdlib.h"
#include "texture_streamer.h"

#include "app.h"
#include "camera.h"
#include "core.h"
#include "luaapi/luaapi.h"
#include "planet.h"
#include "ui/ui.h"
#include "world.h"

#define ORBITAL_RELEASE

CoreArgs core_parce_args(int argc, char **argv) {
  CoreArgs args = {0};

  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "--showlogs") == 0) {
      args.show_logs = true;
    }
  }

  return args;
}

Core core_create(CoreArgs args) {
  ChangeDirectory(GetApplicationDirectory());

  Core core = {
      .screenWidth = 1080,
      .screenHeight = 720,
      .program_active = true,
      .core_active = false,

      .bootTime = 0.0f,
      .bootStart = 0.0f,
      .isFirstFrame = true,

      .lua_initalzied = false,
      .next_world_id = 1,

      .is_paused = false,

      .camera = create_camera(),
  };

  SetConfigFlags(FLAG_MSAA_4X_HINT);
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);

#ifdef ORBITAL_RELEASE
  if (!args.show_logs) {
    SetTraceLogLevel(LOG_NONE);
  }
#endif

  InitWindow(core.screenWidth, core.screenHeight,
             "Lattic Engine v1 - Sarthhak Arora");

  core.bootStart = GetTime();
  core.camera = create_camera();

  SetExitKey(KEY_NULL);

  core.active_world = (World){0};
  core.active_world.valid = false;

  loader_init();

  return core;
}

void enable_cursor(Core *core) {
  static CursorMode last = -1;

  if (core->cursor_mode == last)
    return;

  if (core->cursor_mode == CURSOR_UI) {
    EnableCursor();
    core->camera.rotate_active = false;
  } else {
    DisableCursor();
    core->camera.rotate_active = true;
  }

  last = core->cursor_mode;
}

void handle_cursor_input(Core *core) {
  if (IsKeyPressed(KEY_ENTER)) {
    if (core->cursor_mode == CURSOR_UI) {
      core->cursor_mode = CURSOR_CAMERA;
    } else {
      core->cursor_mode = CURSOR_UI;
    }
  }
}

void fullscreen() {
  if (IsKeyPressed(KEY_F11) && IsWindowFullscreen() == false) {
    SetWindowMinSize(GetMonitorWidth(0), GetMonitorHeight(0));
    SetWindowState(FLAG_FULLSCREEN_MODE);
  } else if (IsKeyPressed(KEY_F11) && IsWindowFullscreen() == true) {
    ClearWindowState(FLAG_FULLSCREEN_MODE);
  }
}

void pause_time(World *world) {
  static float old_deltaTime = 0.0f;
  if (IsKeyPressed(KEY_P) && world->deltaTime != 0) {
    old_deltaTime = world->deltaTime;
    world->deltaTime = 0;
  } else if (IsKeyPressed(KEY_P) && world->deltaTime == 0) {
    world->deltaTime = old_deltaTime;
  }
}

// Ik ui function in core i am lazy!
static void draw_status_panel(Core *core) {
  int x = 10;
  int y = 10;
  int w = 180;
  int h = 90;

  // background
  DrawRectangle(x, y, w, h, (Color){0x13, 0x16, 0x1B, 200});

  // border
  DrawRectangleLines(x, y, w, h, (Color){0x2C, 0x2F, 0x38, 255});

  // accent
  Color accent = core->cursor_mode == CURSOR_UI
                     ? (Color){0xA7, 0x8B, 0xFA, 255}
                     : (Color){0x4F, 0xC3, 0xF7, 255};

  DrawRectangle(x, y, 4, h, accent);

  int ty = y + 6;

  // mode
  DrawText(core->cursor_mode == CURSOR_UI ? "UI MODE" : "CAMERA MODE", x + 10,
           ty, 12, RAYWHITE);

  ty += 16;

  // fps
  DrawText(TextFormat("FPS: %d", GetFPS()), x + 10, ty, 12,
           (Color){0xC4, 0xC4, 0xC4, 255});

  ty += 14;

  // planet count
  if (core->active_world.valid) {
    DrawText(TextFormat("Planets: %d", core->active_world.planet_count), x + 10,
             ty, 12, (Color){0xC4, 0xC4, 0xC4, 255});
    ty += 14;

    DrawText(TextFormat("dt: %.4f", core->active_world.deltaTime), x + 10, ty,
             12, (Color){0x55, 0x5A, 0x66, 255});
    ty += 14;

    DrawText(TextFormat("Time: %.2fx", core->active_world.timeScale), x + 10,
             ty, 12, (Color){0x55, 0x5A, 0x66, 255});
  }
}

void core_draw(Core *core) {
  BeginDrawing();
  BeginMode3D(core->camera.camera);
  ClearBackground(BLACK);

  if (core->active_world.valid) {
    world_draw(&core->active_world);
  }

  DrawGrid(300, 10.4);

  EndMode3D();

  draw_status_panel(core);

  if (core->active_world.valid) {
    ui(&core->active_world);
  }

  EndDrawing();
}

static void core_update(Core *core) {
  if (IsKeyPressed(KEY_ESCAPE)) {
    core->core_active = false;
    if (core->active_world.valid) {
      world_destroy(&core->active_world);
      core->active_world = (World){0};
      core->active_world.valid = false;
    }
    return;
  }

  Image img;
  const char *path;
  while (loader_poll(&img, &path)) {
    TextureEntry *entry = NULL;
    HASH_FIND_STR(texture_map, path, entry);
    if (entry) {
      entry->managed.texture = LoadTextureFromImage(img);
      entry->managed.loaded = true;
    }
    UnloadImage(img);
  }

  update_lua(core->L);

  handle_cursor_input(core);
  enable_cursor(core);

  fullscreen();

  if (core->active_world.valid) {
    pause_time(&core->active_world);
    world_update(&core->active_world);
  }

  if (core->cursor_mode == CURSOR_CAMERA) {
    update_camera(&core->camera);
  }
}
void core_run(Core *core) {
  while (core->program_active == true && !WindowShouldClose()) {
    if (core->core_active) {
      core_update(core);
      core_draw(core);
    }
    if (!core->core_active) {
      app_run(core, &core->core_active);
      if (core->isFirstFrame) {
        double now = GetTime();
        printf("--------------------------------\nBOOT: %.3fms taken to "
               "boot\n--------------------------------\n",
               (now - core->bootTime) * 1000);
        core->isFirstFrame = false;
      }
    }
  }
  if (core->active_world.valid) {
    world_destroy(&core->active_world);
  }
}
void core_destroy(Core *core) {
  if (core->active_world.valid) {
    world_destroy(&core->active_world);
  }
  CloseWindow();
}
