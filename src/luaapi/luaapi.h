#pragma once

#include "raylib.h"
#include "../planet.h"
#include "../world.h"
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

void init_luaapi(void);
int l_create_world(lua_State *L);
int l_world_add_planet(lua_State *L);