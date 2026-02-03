#include "app.h"
#include "luaapi/luaapi.h"
#include "platform/platform_win32.h"
#include <stdio.h>
#include "raygui.h"
#include "stdlib.h"
#include "string.h"
#include "raylib.h"
#include "core.h"

static const char *scripts[] = {};

Camera2D camera = {
    .offset = (Vector2){0, 0},
    .target = (Vector2){0, 0},
    .rotation = 0.0f,
    .zoom = 1.0f,
};

void app_run(Core *core, bool *core_active)
{
    if (IsKeyPressed(KEY_ESCAPE)) {
        core->core_active = false;
    }

    fullscreen();

    ScriptList scripts = scan_scripts("../scripts");

    if (scripts.count == 0) {
        printf("No Lua scripts found in /scripts\n");
        return;
    }
    
    //printf("\nAvailable simulations:\n");
    //for (int i = 0; i < scripts.count; i++) {
    //printf(" [%d] %s\n", i, scripts.paths[i]);
    //}

    //printf("\nSelect script: ");

    static int choice = -1;
    static char choiceText[4069]; 
    static bool built = false;
    static bool editMode = false;

    if(!built) {
        choiceText[0] = '\0';
        for(int i = 0; i < scripts.count; i++) {
           strcat(choiceText, scripts.paths[i]);
           if(i < scripts.count - 1) {
                strcat(choiceText, ";");
            }
        }

        built = true;
    }

    BeginDrawing();
    BeginMode2D(camera);
    ClearBackground(RAYWHITE);

    const char* title = "Welcome to Lettic engine";

    Vector2 size = MeasureTextEx(
        GetFontDefault(),
        title,
        40,
        1.0f
    );

    DrawTextPro(GetFontDefault(), title, (Vector2){GetScreenWidth()/2 - size.x/2, GetScreenHeight()/2 * 0.2}, (Vector2){0, 0}, 0.0f, 40, 1.0f, RED);

    if(GuiDropdownBox((Rectangle){GetScreenWidth()/2 - size.x/2, GetScreenHeight()/2, size.x, size.y}, choiceText, &choice, editMode)) {
        editMode = !editMode;
    }

    if(GuiButton((Rectangle){GetScreenWidth()/2 - size.x/2, GetScreenHeight()/2 * 0.7, size.x, size.y}, "Start Engine")) {
        if (choice < 0 || choice >= scripts.count) {
            platform_throw_error("Invalid selection", "User Error", PLATFORM_ICON_WARNING || PLATFORM_MSG_OK);
            return;
        }

        init_luaapi(scripts.paths[choice]);       

        core->cursor_mode = CURSOR_CAMERA;
        *core_active = true;
    }

    EndMode2D();
    EndDrawing();
}