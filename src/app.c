#include "app.h"
#include "core.h"
#include "luaapi/luaapi.h"
#include "platform/platform_win32.h"
#include "raygui.h"
#include "raylib.h"
#include "stdlib.h"
#include "string.h"
#include "ui/ui_widgets.h"
#include <stdbool.h>
#include <stdio.h>

Camera2D camera = {
    .offset = (Vector2){0, 0},
    .target = (Vector2){0, 0},
    .rotation = 0.0f,
    .zoom = 1.0f,
};

static void main_menu(Core *core, int *choice, char *choiceText,
                      ScriptList *scripts, bool *dropDownOpen, int *appstate) {

  Rectangle titleBox = {GetScreenWidth() / 2 - 300, GetScreenHeight() * 0.18f,
                        600, 140};

  DrawRectangleLinesEx(titleBox, 1, (Color){200, 200, 200, 80});
  const char *title = "Lattic engine";
  const char *subtitle = "v1 Orbital Physics Simulator - By Sarthhak";

  Font font = GetFontDefault();

  Vector2 tilesize = MeasureTextEx(font, title, 40, 1.0f);
  Vector2 subtitleSize = MeasureTextEx(font, subtitle, 15, 1.0f);

  DrawTextPro(font, title,
              (Vector2){GetScreenWidth() / 2 - tilesize.x / 2, titleBox.y + 30},
              (Vector2){0, 0}, 0.0f, 40, 1.0f, (Color){200, 200, 200, 255});

  DrawTextPro(
      font, subtitle,
      (Vector2){GetScreenWidth() / 2 - subtitleSize.x / 2, titleBox.y + 80},
      (Vector2){0, 0}, 0.0f, 15, 1.0f, (Color){150, 150, 150, 255});

  Rectangle dropDownRec = {GetScreenWidth() / 2 - 200, GetScreenHeight() * 0.7f,
                           400, tilesize.y};

  Vector2 mousepos = GetMousePosition();

  static bool settingsButtonPressed;
  static bool startButtonPressed;

  if (core->isStarting) {

    GuiProgressBar((Rectangle){GetScreenWidth() / 2 - 200,
                               GetScreenHeight() * 0.6f, 400, tilesize.y},
                   NULL, NULL, &core->progress, 0.0f, 1.0f);

    core->progress += GetFrameTime() * 0.8f;

    if (core->progress >= 1.0f) {
      core->progress = 1.0f;

      core->L = luaL_newstate();
      luaL_openlibs(core->L);

      init_luaapi(scripts->paths[*choice], core->L);
      init_lua(core->L);

      core->cursor_mode = CURSOR_CAMERA;
      core->core_active = true;
      core->isStarting = false;
    }

    return;
  }

  startButtonPressed =
      GuiButton((Rectangle){GetScreenWidth() / 2 - 200,
                            GetScreenHeight() * 0.75f, 400, tilesize.y},
                "Start Engine");

  settingsButtonPressed =
      GuiButton((Rectangle){GetScreenWidth() / 2 - 200,
                            GetScreenHeight() * 0.85f, 400, tilesize.y},
                "Settings");

  if (GuiDropdownBox(dropDownRec, choiceText, choice, *dropDownOpen)) {
    *dropDownOpen = !(*dropDownOpen);
  }
  if (startButtonPressed && !core->isStarting) {
    if (*choice < 0 || *choice >= scripts->count) {
      platform_throw_error_without_exit("Invalid selection", "User Error",
                                        PLATFORM_ICON_WARNING |
                                            PLATFORM_MSG_OK);
      return;
    }

    core->isStarting = true;
    core->progress = 0.0f;
  }

  if (settingsButtonPressed) {
    *appstate = APP_STATE_SETTINGS;
  }
}

static void settings_menu(Core *core, int *choice, char *choiceText,
                          ScriptList *scripts, bool *editMode, int *appstate) {

  const char *title = "Settings";
  Font font = GetFontDefault();

  Vector2 tilesize = MeasureTextEx(font, title, 40, 1.0f);

  DrawTextPro(font, title,
              (Vector2){GetScreenWidth() / 2 - tilesize.x / 2,
                        GetScreenHeight() * 0.2f},
              (Vector2){0, 0}, 0.0f, 40, 1.0f, (Color){200, 200, 200, 255});

  if (GuiButton((Rectangle){GetScreenWidth() / 2 - 200,
                            GetScreenHeight() * 0.8f, 400, tilesize.y},
                "Back to Menu")) {
    *appstate = APP_STATE_MENU;
  }
}

static void draw_app(Core *core, int *choice, char *choiceText,
                     ScriptList *scripts, bool *editMode, int *appstate) {

  BeginDrawing();
  ClearBackground(BLACK); // ← ONLY here

  float time = GetTime();
  Vector2 mouse = GetMousePosition();
  Vector2 res = {GetScreenWidth(), GetScreenHeight()};

  static Shader bgShader;
  static bool shaderLoaded = false;

  if (!shaderLoaded) {
    bgShader = LoadShader(0, "../assets/shaders/bg.fs");
    shaderLoaded = true;
  }

  SetShaderValue(bgShader, GetShaderLocation(bgShader, "time"), &time,
                 SHADER_UNIFORM_FLOAT);
  SetShaderValue(bgShader, GetShaderLocation(bgShader, "mouse"), &mouse,
                 SHADER_UNIFORM_VEC2);
  SetShaderValue(bgShader, GetShaderLocation(bgShader, "resolution"), &res,
                 SHADER_UNIFORM_VEC2);

  BeginShaderMode(bgShader);
  DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), WHITE);
  EndShaderMode();

  /// BeginMode2D(camera);

  if (*appstate == APP_STATE_MENU) {
    main_menu(core, choice, choiceText, scripts, editMode, appstate);
  } else if (*appstate == APP_STATE_SETTINGS) {
    settings_menu(core, choice, choiceText, scripts, editMode, appstate);
  }

  // EndMode2D();

  EndDrawing();
}

void app_run(Core *core, bool *program_active) {

  if (core->core_active)
    return;

  core->cursor_mode = CURSOR_UI;
  handle_cursor_input(core);
  enable_cursor(core);

  styling();

  static int appstate = APP_STATE_MENU;

  static ScriptList scripts = {0};
  static bool scripts_loaded = false;

  if (!scripts_loaded) {
    scripts = scan_scripts("../scripts");
    scripts_loaded = true;
  }

  if (scripts.count == 0) {
    printf("No Lua scripts found\n");
    return;
  }

  static int choice = 0;
  static char choiceText[4096];
  static bool built = false;
  static bool dropDownOpen = false;

  if (!built) {
    choiceText[0] = '\0';
    for (int i = 0; i < scripts.count; i++) {
      strncat(choiceText, scripts.paths[i],
              sizeof(choiceText) - strlen(choiceText) - 1);
      if (i < scripts.count - 1) {
        strncat(choiceText, ";", sizeof(choiceText) - strlen(choiceText) - 1);
      }
    }
    built = true;
  }

  draw_app(core, &choice, choiceText, &scripts, &dropDownOpen, &appstate);
}
