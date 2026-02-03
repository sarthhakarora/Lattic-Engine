#include "luaapi.h"
#include "raylib.h"
#include "../planet.h"
#include "../world.h"
#include "../platform/platform_win32.h"
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include "../core.h"

extern Core *global_core;

void init_luaapi(const char *scriptPath)
{
    static lua_State *L;

    L = luaL_newstate();
    luaL_openlibs(L);

    lua_register(L, "create_world", l_create_world);
    lua_register(L, "add_planet", l_world_add_planet);

    if(luaL_dofile(L, scriptPath)) {
        platform_throw_error("Script is not loaded check file paths", "Script not loaded", PLATFORM_SYSTEM_MODAL || PLATFORM_MSG_OK);
        const char *err = lua_tostring(L, -1);
        printf("LUA ERROR: %s\n", err);
        lua_pop(L, 1);
    }
}

int l_create_world(lua_State *L)
{
    if(!global_core) {
        platform_throw_error("Engine not initialized", "CRITICAL ENGINE ERROR", PLATFORM_SYSTEM_MODAL || PLATFORM_MSG_OK);
        return luaL_error(L, "Engine not initialized");
    }

    global_core->active_world = world_create();

    return 0;
}

int l_world_add_planet(lua_State *L)
{
    if(!global_core || !global_core->active_world.valid) {
        platform_throw_error("World not created", "World Creation Error", PLATFORM_SYSTEM_MODAL || PLATFORM_MSG_OK);
        return luaL_error(L, "World not created");
    }
    if(!lua_istable(L, 1)) {
        platform_throw_error("Planet Expects Table", "Syntax Error", PLATFORM_SYSTEM_MODAL || PLATFORM_MSG_OK);
        return luaL_error(L, "add_planet expects a table");
    }

    lua_getfield(L, 1, "name");
    const char *name = luaL_checkstring(L, -1);
    lua_pop(L, 1);

    lua_getfield(L, 1, "texturepath");
    const char *texturepath = luaL_checkstring(L, -1);
    lua_pop(L, 1);

    lua_getfield(L, 1, "radius");
    float radius = luaL_checknumber(L, -1);
    lua_pop(L, 1);

    lua_getfield(L, 1, "position");
    if(!lua_istable(L, -1)) {
        platform_throw_error("Position Expects Table", "Syntax Error", PLATFORM_SYSTEM_MODAL || PLATFORM_MSG_OK);
        return luaL_error(L, "postion must be a table");
    }

    lua_rawgeti(L, -1, 1);
    lua_rawgeti(L, -2, 2);
    lua_rawgeti(L, -3, 3);

    Vector3 pos = {
        luaL_checknumber(L, -3),
        luaL_checknumber(L, -2),
        luaL_checknumber(L, -1)
    };
    lua_pop(L, 4); 

        lua_getfield(L, 1, "rings");
    int rings = luaL_checkinteger(L, -1);
    lua_pop(L, 1);

    lua_getfield(L, 1, "slices");
    int slices = luaL_checkinteger(L, -1);
    lua_pop(L, 1);

    lua_getfield(L, 1, "mass");
    int mass = luaL_checkinteger(L, -1);
    lua_pop(L, 1);

    lua_getfield(L, 1, "has_gravity");
    bool has_gravity = lua_toboolean(L, -1);
    lua_pop(L, 1);

    Planet *p = malloc(sizeof(Planet));
    *p = planet_create(
        (char*)name,
        (char*)texturepath,
        radius,
        pos,
        rings,
        slices,
        mass,
        has_gravity
    );

    world_add_planet(p, &global_core->active_world);

    return 0;
}