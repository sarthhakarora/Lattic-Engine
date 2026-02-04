#include "raylib.h"
#include "raymath.h"
#include "stdlib.h"
#include "platform/platform_win32.h"

#include "core.h"
#include "app.h"
#include "planet.h"
#include "world.h"
#include "camera.h"
#include "ui/ui.h"
#include "luaapi/luaapi.h"

#define ORBITAL_RELEASE

CoreArgs core_parce_args(int argc, char **argv)
{
    CoreArgs args = {0};
    
    for(int i = 1; i < argc; i++) {
        if(strcmp(argv[i], "--showlogs") == 0) {
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

        .camera = create_camera(),
    };

    SetConfigFlags(FLAG_MSAA_4X_HINT);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    #ifdef ORBITAL_RELEASE
        if(!args.show_logs) {
            SetTraceLogLevel(LOG_NONE);
        }
    #endif

    InitWindow(core.screenWidth, core.screenHeight, "Orbital Physics Simulator");

    core.bootStart = GetTime();


    core.camera = create_camera();

    /*
    Planet earth = planet_create("earth", "assets/graphics/earth.jpg", 10.0f, (Vector3){200.0f, 0.0f, 0.0f}, 32, 32, 1, false);
    Planet mars = planet_create("mars", "assets/graphics/mars.jpg", 10.0f, (Vector3){-250.0f, 0.0f, 0.0f}, 32, 32, 1, false);
    Planet sun = planet_create("sun", "assets/graphics/sun.jpg", 30.0f, (Vector3){0.0f, 200.0f, 0.0f}, 48, 48, 1000, true);
    Planet jupiter = planet_create("jupiter", "assets/graphics/jupiter.jpg", 30.0f, (Vector3){0.0f, 0.0f, 0.0f}, 48, 48, 1000, true);
    */

    /*
    planets[count++] = earth;
    planets[count++] = sun;
    planets[count++] = jupiter;
    planets[count++] = mars;
    */

    core.active_world = (World){0};
    core.active_world.valid = false;
    
    return core;
}

void enable_cursor(Core *core) {
    static CursorMode last = -1;

    if (core->cursor_mode == last) return;

    if (core->cursor_mode == CURSOR_UI) {
        EnableCursor();
        core->camera.rotate_active = false;
    } else {
        DisableCursor();
        core->camera.rotate_active = true;
    }

    last = core->cursor_mode;
}

static void handle_cursor_input(Core *core)
{
    if (IsKeyPressed(KEY_ENTER)) {
        core->cursor_mode = CURSOR_UI;
    }

    if (IsKeyPressed(KEY_RIGHT_SHIFT)) {
        core->cursor_mode = CURSOR_CAMERA;
    }
}

void fullscreen() {
    if(IsKeyPressed(KEY_F11) && IsWindowFullscreen() == false) {
        SetWindowMinSize(GetMonitorWidth(0), GetMonitorHeight(0));
        SetWindowState(FLAG_FULLSCREEN_MODE);
    }
    else if(IsKeyPressed(KEY_F11) && IsWindowFullscreen() == true) {
        ClearWindowState(FLAG_FULLSCREEN_MODE);
    }
}

void pause_time(World *world) {
    static float old_deltaTime = 0.0f;
    if(IsKeyPressed(KEY_P) && world->deltaTime != 0) {
        old_deltaTime = world->deltaTime;
        world->deltaTime = 0;
    }
    else if(IsKeyPressed(KEY_P) && world->deltaTime == 0) {
        world->deltaTime = old_deltaTime;
    }
}

void core_draw(Core *core) {
    BeginDrawing();
    BeginMode3D(core->camera.camera);
    ClearBackground(BLACK);

    if(core->active_world.valid) {
        world_draw(&core->active_world);
    }

    EndMode3D();

    DrawFPS(10, 10); 
    if(core->active_world.valid) { 
        ui(&core->active_world);
        static char buffer[256];
        snprintf(buffer, 256, "Planet Count: %d", core->active_world.planet_count);
        DrawText(buffer, 10, 30, 20, GREEN);
    }

    EndDrawing();
}

static void core_update(Core *core)
{
    if (IsKeyPressed(KEY_ESCAPE)) {
        core->core_active = false;
        if(core->active_world.valid) {
            world_destroy(&core->active_world);
            core->active_world = (World){0};
            core->active_world.valid = false;
        }
        return;
    }

    handle_cursor_input(core);
    enable_cursor(core);

    fullscreen();

    if(core->active_world.valid) {
        pause_time(&core->active_world);
        world_update(&core->active_world);
    }
    update_camera(&core->camera);
}
void core_run(Core *core) {
    while (core->program_active == true && !WindowShouldClose()) {
        if(core->core_active){
            core_update(core);
            core_draw(core);

        } if(!core->core_active) {
            app_run(core, &core->core_active);
            if(core->isFirstFrame) {
                double now = GetTime();
                printf("--------------------------------\nBOOT: %.3fms taken to boot\n--------------------------------\n", (now - core->bootTime) * 1000);
                core->isFirstFrame = false;
            }
        }
    }
    if(core->active_world.valid) {
        world_destroy(&core->active_world);
    }
}
void core_destroy(Core *core) {
    if(core->active_world.valid) {
        world_destroy(&core->active_world);
    }
    CloseWindow();
}