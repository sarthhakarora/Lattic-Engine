#include "luaapi.h"
#include "raylib.h"
#include "../planet.h"
#include "../world.h"
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include "../core.h"

extern Core *global_core;

void init_luaapi(void)
{
    static lua_State *L;
    L = luaL_newstate();

    luaL_openlibs(L);
    lua_register(L, "create_world", l_create_world);
    lua_register(L, "world_add_planet", l_world_add_planet);

    luaL_dofile(L, "scripts/main.lua");
}

int l_create_world(lua_State *L)
{
    World world = world_create();
    global_core->active_world = world;
    world.valid = true;

    return 0;
}

int l_world_add_planet(lua_State *L)
{
    World *world;

    Planet planet;

    world_add_planet(planet, world);
    return 0;
}