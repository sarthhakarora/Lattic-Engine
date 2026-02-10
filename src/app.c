#include "app.h"
#include "luaapi/luaapi.h"
#include "platform/platform_win32.h"
#include <stdio.h>
#include "raygui.h"
#include "stdlib.h"
#include "string.h"
#include "raylib.h"
#include "core.h"
#include "ui/ui_widgets.h"

static const char *scripts[] = {};

Camera2D camera = {
    .offset = (Vector2){0, 0},
    .target = (Vector2){0, 0},
    .rotation = 0.0f,
    .zoom = 1.0f,
};

static void main_menu(Core *core, int *choice, char *choiceText, ScriptList *scripts, bool *dropDownOpen, int *appstate) {
    const char* title = "Lettic engine";
    const char* subtitle = "v1 Orbital Physics Simulator - By Sarthhak";

    static LoadTracker loader = {
        .total = 0,
        .done = 0
    };

    Font font = GetFontDefault();

    Vector2 tilesize = MeasureTextEx(
        font,
        title,
        40,
        1.0f
    );
    Vector2 subtitleSize = MeasureTextEx(
        font,
        subtitle,
        15,
        1.0f
    );

    DrawTextPro(font, title, (Vector2){GetScreenWidth()/2 - tilesize.x/2, GetScreenHeight()/2 * 0.2}, (Vector2){0, 0}, 0.0f, 40, 1.0f, (Color){200, 200, 200, 255});
    DrawTextPro(font, subtitle, (Vector2){GetScreenWidth()/2 - subtitleSize.x/2, GetScreenHeight()/2 * 0.3 + 10}, (Vector2){0, 0}, 0.0f, 15, 1.0f, (Color){150, 150, 150, 255});

    DrawBoundingBox(
        (BoundingBox){
            .min = (Vector3){GetScreenWidth()/2 - (subtitleSize.x)/2 - 20 , GetScreenHeight()/2 * 0.15, 0},
            .max = (Vector3){GetScreenWidth()/2 + (subtitleSize.x)/2 + 20, GetScreenHeight()/2 * 0.25 + subtitleSize.y + tilesize.y + 20, 0}
        },
        (Color){100, 100, 100, 255}
    );

    Rectangle dropDownRec = (Rectangle){
        GetScreenWidth()/2 - 200, 
        GetScreenHeight()/2 * 0.8f, 
        400, 
        tilesize.y
    };

    Vector2 mousepos = GetMousePosition();

    bool mouseOverDropdown = CheckCollisionPointRec(mousepos, dropDownRec) || *dropDownOpen;

    static bool dropDownPressed;
    static bool settingsButtonPressed;
    static bool startButtonPressed;

    static float x = 0.0f;
    float y = 0.0f;
    float z = 30.0f;

    x+= 0.0001f;

    GuiProgressBar((Rectangle){GetScreenWidth()/2 - 200, GetScreenHeight()/2 * 1.0f, 400, tilesize.y}, NULL, NULL, &x, y, z);
    startButtonPressed = GuiButton((Rectangle){GetScreenWidth()/2 - 200, GetScreenHeight()/2 * 1.2f, 400, tilesize.y}, "Start Engine");
    settingsButtonPressed = GuiButton((Rectangle){GetScreenWidth()/2 - 200, GetScreenHeight()/2 * 1.4f, 400, tilesize.y}, "Settings");
    dropDownPressed = GuiDropdownBox(dropDownRec, choiceText, choice, *dropDownOpen);

    if(dropDownPressed) {
        *dropDownOpen = !*dropDownOpen;
    }

    if(!mouseOverDropdown && startButtonPressed) {
        if (*choice < 0 || *choice >= scripts->count) {
            platform_throw_error_without_exit("Invalid selection", "User Error", PLATFORM_ICON_WARNING | PLATFORM_MSG_OK);
            return;
        }

        core->L = luaL_newstate();
        luaL_openlibs(core->L);

        init_luaapi(scripts->paths[*choice], core->L);       
        init_lua(core->L);

        core->cursor_mode = CURSOR_CAMERA;
        core->core_active = true;

        if(core->active_world.valid) {
            loader.total += core->active_world.planet_count;
        }
    }

    if(!mouseOverDropdown && settingsButtonPressed) {
        *appstate = APP_STATE_SETTINGS;

    }
    
}

static void settings_menu(Core *core, int *choice, char *choiceText, ScriptList *scripts, bool *editMode, int *appstate) {
    const char* title = "Settings";

    Font font = GetFontDefault();

    Vector2 tilesize = MeasureTextEx(
        font,
        title,
        40,
        1.0f
    );

    DrawTextPro(font, title, (Vector2){GetScreenWidth()/2 - tilesize.x/2, GetScreenHeight()/2 * 0.2}, (Vector2){0, 0}, 0.0f, 40, 1.0f, (Color){200, 200, 200, 255});

    DrawBoundingBox(
        (BoundingBox){
            .min = (Vector3){GetScreenWidth()/2 - (tilesize.x)/2 - 20 , GetScreenHeight()/2 * 0.15, 0},
            .max = (Vector3){GetScreenWidth()/2 + (tilesize.x)/2 + 20, GetScreenHeight()/2 * 0.25 + tilesize.y, 0}
        },
        (Color){100, 100, 100, 255}
    );

    if(GuiButton((Rectangle){GetScreenWidth()/2 - 200, GetScreenHeight()/2 * 1.0f, 400, tilesize.y}, "Back to Menu")) {
        *appstate = APP_STATE_MENU;
    }

}

static void draw_app(Core *core, int *choice, char *choiceText, ScriptList *scripts, bool *editMode, int *appstate) {
    BeginDrawing();
    BeginMode2D(camera);
    ClearBackground(BLACK);

    if(*appstate == APP_STATE_MENU) {
        main_menu(core, choice, choiceText, scripts, editMode, appstate);
    } else if(*appstate == APP_STATE_SETTINGS) {
        settings_menu(core, choice, choiceText, scripts, editMode, appstate);
    }

    EndMode2D();
    EndDrawing();

}

void app_run(Core *core, bool *program_active)
{
    core->cursor_mode = CURSOR_UI;
    handle_cursor_input(core);
    enable_cursor(core);

    styling();

    static int appstate = APP_STATE_MENU;

    static ScriptList scripts = {0};
    static bool scripts_loaded = false;

    if(!scripts_loaded) {
        scripts = scan_scripts("../scripts");
        scripts_loaded = true;
    }

    fullscreen();

    if (scripts.count == 0) {
        printf("No Lua scripts found in /scripts\n");
        return;
    }

    static int choice = -1;
    static char choiceText[4069]; 
    static bool built = false;
    static bool dropDownOpen = false;

    if(!built) {
        choiceText[0] = '\0';
        for(int i = 0; i < scripts.count; i++) {
           strncat(choiceText, scripts.paths[i], sizeof(choiceText) - strlen(choiceText) - 1);
           if(i < scripts.count - 1) {
                strncat(choiceText, ";", sizeof(choiceText) - strlen(choiceText) - 1 );
            }
        }
        built = true;
    }

    draw_app(core, &choice, choiceText, &scripts, &dropDownOpen, &appstate);

}