#include <stdio.h>
#include "raylib.h"
#include "luaapi/luaapi.h"
#include "core.h"
#include "app.h"

Core *global_core = NULL;

int main(int argc, char **argv) {
    CoreArgs args = core_parce_args(argc, argv);

    Core core = core_create(args);

    global_core = &core;

    core_run(&core);
    core_destroy(&core);

    return 0;
}