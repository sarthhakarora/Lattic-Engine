#include "assets/assetmanager.h"
#include "assets/skybox.h"
#include "log.h"
#include "raylib.h"
#include "assets/texture_streamer.h"

#include "app.h"
#include "camera.h"
#include "core.h"
#include "luaapi/luaapi.h"
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

Sky sky;

Core core_create(CoreArgs args) {
  ChangeDirectory(GetApplicationDirectory());

  Core core = {
      .screenWidth = 1080,
      .screenHeight = 720,
      .program_active = true,
      .core_active = false,
      .blendMode = 0,

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

  sky = sky_create(1000.0f, "assets/shaders/sky.vs", "assets/shaders/sky.fs");
  sky_load(&sky);

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
  ClearBackground(BLACK);
  BeginMode3D(core->camera.camera);

  float t = GetTime();
  shader_set_float(&sky.shader, "time", t);

  sky_draw(&sky, core->camera.camera);

  if (core->active_world.valid) {
    draw_lua(core->L);
    world_draw(&core->active_world);
  }

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
      core->blendMode = 0;
      core->active_world = (World){0};
      core->active_world.valid = false;
    }
    return;
  }

  asset_update();

  handle_cursor_input(core);
  enable_cursor(core);

  fullscreen();

  if (core->active_world.valid) {
    pause_time(&core->active_world);
    update_lua(core->L);
    update_camera(&core->camera);
    world_update(&core->active_world);
  }

  if (core->cursor_mode == CURSOR_CAMERA) {
    update_camera(&core->camera);
  }
}
void core_run(Core *core) {
  while (core->program_active == true && !WindowShouldClose()) {
    if (core->core_active) {
      core_draw(core);
      core_update(core);
    }
    if (!core->core_active) {
      app_run(core, &core->core_active);
      if (core->isFirstFrame) {
        double now = GetTime();
        log_msg(LOG_ENGINE, LOG_LEVEL_INFO, "boot time: %.3f ms",
          (now - core->bootTime) * 1000.0);
        core->isFirstFrame = false;
      }
    }
  }
  if (core->active_world.valid) {
    world_destroy(&core->active_world);
  }
}
void core_destroy(Core *core) {
  sky_unload(&sky);
  if (core->active_world.valid) {
    world_destroy(&core->active_world);
  }
  CloseWindow();
}
