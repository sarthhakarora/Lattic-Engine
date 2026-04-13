#include "luaapi.h"
#include "../planet.h"
#include "../platform/platform_win32.h"
#include "../world.h"
#include "raylib.h"

#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>

#include "../core.h"

#define X(k)                                                                   \
  lua_pushinteger(L, (int)(k));                                                \
  lua_setfield(L, -2, #k + 4);

extern Core *global_core;

static void push_keyboard_keys(lua_State *L) {
  lua_newtable(L);

#define X(k)                                                                   \
  lua_pushinteger(L, (int)(k));                                                \
  lua_setfield(L, -2, #k + 4);

#include "../generated/keyboard_keys.def"

#undef X

  lua_setglobal(L, "KEY");
}

void init_lua(lua_State *L) {
  lua_getglobal(L, "init");
  if (!lua_isfunction(L, -1)) {
    lua_pop(L, 1);
    platform_throw_error("Lua script must define init", "init not found",
                         PLATFORM_SYSTEM_MODAL || PLATFORM_MSG_OK);
    return;
  }
  if (lua_pcall(L, 0, 0, 0) != LUA_OK) {
    printf("LUA INIT ERROR: %s\n", lua_tostring(L, -1));
    lua_pop(L, 1);
  }
}
void update_lua(lua_State *L) {
  lua_getglobal(L, "update");
  if (!lua_isfunction(L, -1)) {
    lua_pop(L, 1);
    platform_throw_error("Lua script must define update", "update not found",
                         PLATFORM_SYSTEM_MODAL || PLATFORM_MSG_OK);
    return;
  }
  if (lua_pcall(L, 0, 0, 0) != LUA_OK) {
    printf("LUA INIT ERROR: %s\n", lua_tostring(L, -1));
    lua_pop(L, 1);
  }
}

void init_luaapi(const char *scriptPath, lua_State *L) {
  lua_register(L, "create_world", l_create_world);
  lua_register(L, "add_planet", l_world_add_planet);
  lua_register(L, "find_planet", l_find_planet);

  lua_register(L, "unload_planet", l_planet_unload);
  lua_register(L, "unload_world", l_world_unload);

  lua_register(L, "set_world_gravity_constant", l_set_world_gravity_constant);

  lua_register(L, "get_world_gravity_constant", l_get_world_gravity_constant);
  lua_register(L, "get_time_state", l_get_time_state);

  lua_register(L, "pause", l_pause);
  lua_register(L, "resume", l_resume);

  lua_register(L, "is_key_down", l_IsKeyDown);
  lua_register(L, "is_key_up", l_IsKeyUp);
  lua_register(L, "is_key_pressed", l_IsKeyPressed);

  push_keyboard_keys(L);

  if (luaL_dofile(L, scriptPath)) {
    platform_throw_error("Script is not loaded check file paths",
                         "Script not loaded",
                         PLATFORM_SYSTEM_MODAL || PLATFORM_MSG_OK);
    const char *err = lua_tostring(L, -1);
    printf("LUA ERROR: %s\n", err);
    lua_pop(L, 1);
  }
}

int l_create_world(lua_State *L) {
  if (!global_core) {
    platform_throw_error("Engine not initialized", "CRITICAL ENGINE ERROR",
                         PLATFORM_SYSTEM_MODAL || PLATFORM_MSG_OK);
    return luaL_error(L, "Engine not initialized");
  }

  global_core->active_world = world_create();

  return 0;
}

int l_world_add_planet(lua_State *L) {
  if (!global_core || !global_core->active_world.valid) {
    platform_throw_error("World not created", "World Creation Error",
                         PLATFORM_SYSTEM_MODAL || PLATFORM_MSG_OK);
    return luaL_error(L, "World not created");
  }
  if (!lua_istable(L, 1)) {
    platform_throw_error("Planet Expects Table", "Syntax Error",
                         PLATFORM_SYSTEM_MODAL || PLATFORM_MSG_OK);
    return luaL_error(L, "add_planet expects a table");
  }

  lua_getfield(L, 1, "name");
  const char *name_lua = luaL_checkstring(L, -1);
  const char *name = strdup(name_lua);
  lua_pop(L, 1);

  lua_getfield(L, 1, "texturepath");
  const char *texturepath_lua = luaL_checkstring(L, -1);
  const char *texturepath = strdup(texturepath_lua);
  lua_pop(L, 1);

  lua_getfield(L, 1, "radius");
  float radius = luaL_checknumber(L, -1);
  lua_pop(L, 1);

  lua_getfield(L, 1, "position");
  if (!lua_istable(L, -1)) {
    platform_throw_error("Position Expects Table", "Syntax Error",
                         PLATFORM_SYSTEM_MODAL || PLATFORM_MSG_OK);
    return luaL_error(L, "postion must be a table");
  }

  lua_rawgeti(L, -1, 1);
  lua_rawgeti(L, -2, 2);
  lua_rawgeti(L, -3, 3);

  Vector3 pos = {luaL_checknumber(L, -3), luaL_checknumber(L, -2),
                 luaL_checknumber(L, -1)};
  lua_pop(L, 4);

  lua_getfield(L, 1, "rings");
  int rings = luaL_checkinteger(L, -1);
  lua_pop(L, 1);

  lua_getfield(L, 1, "slices");
  int slices = luaL_checkinteger(L, -1);
  lua_pop(L, 1);

  lua_getfield(L, 1, "mass");
  int mass = luaL_checknumber(L, -1);
  lua_pop(L, 1);

  lua_getfield(L, 1, "has_gravity");
  bool has_gravity = lua_toboolean(L, -1);
  lua_pop(L, 1);

  Planet *p = malloc(sizeof(Planet));
  *p = planet_create((char *)name, (char *)texturepath, radius, pos, rings,
                     slices, mass, has_gravity);

  world_add_planet(p, &global_core->active_world);

  lua_pushinteger(L, p->id);

  return 1;
}

int l_set_world_gravity_constant(lua_State *L) {
  float gravity = (float)luaL_checknumber(L, 1);

  if (global_core->active_world.valid) {
    global_core->active_world.gravity_strength = gravity;
  } else {
    platform_throw_error_without_exit(
        "Trying to set gravity before creation of world", "User error",
        PLATFORM_ICON_WARNING | PLATFORM_MSG_OK);
  }
}

int l_get_world_gravity_constant(lua_State *L) {
  lua_pushnumber(L, global_core->active_world.gravity_strength);

  return 1;
}

int l_get_time_state(lua_State *L) {
  lua_pushboolean(L, global_core->is_paused);

  return 1;
}

int l_get_delta_time(lua_State *L) {
  lua_pushnumber(L, global_core->active_world.deltaTime);

  return 1;
}

int l_pause(lua_State *L) {
  global_core->active_world.savedTimeScale =
      global_core->active_world.timeScale;
  global_core->active_world.timeScale = 0.0f;
  global_core->is_paused = true;

  return 0;
}

int l_resume(lua_State *L) {
  global_core->active_world.timeScale =
      global_core->active_world.savedTimeScale;
  global_core->is_paused = false;

  return 0;
}

int l_find_planet(lua_State *L) {
  int64_t id = luaL_checknumber(L, 1);

  Planet *planet = find_planet(&global_core->active_world, id);
  if (planet == NULL) {
    lua_pushnil(L);
  } else {
    lua_pushinteger(L, id);
  }

  return 1;
}

int l_IsKeyDown(lua_State *L) {
  KeyboardKey key = (KeyboardKey)luaL_checkinteger(L, 1);
  bool is_pressed = IsKeyDown(key);

  lua_pushboolean(L, is_pressed);

  return 1;
}

int l_IsKeyUp(lua_State *L) {
  KeyboardKey key = (KeyboardKey)luaL_checkinteger(L, 1);
  bool is_pressed = IsKeyUp(key);

  lua_pushboolean(L, is_pressed);

  return 1;
}

int l_IsKeyPressed(lua_State *L) {
  KeyboardKey key = (KeyboardKey)luaL_checkinteger(L, 1);
  bool is_pressed = IsKeyPressed(key);

  lua_pushboolean(L, is_pressed);

  return 1;
}

int l_world_unload(lua_State *L) {
  world_destroy(&global_core->active_world);

  return 0;
}

int l_planet_unload(lua_State *L) {
  int32_t id = luaL_checkinteger(L, 1);

  Planet *planet = find_planet(&global_core->active_world, id);
  if (planet == NULL) {
    printf("ERROR: PLANET NOT FOUND");
    return 0;
  }

  world_unload_planet(&global_core->active_world, id);

  return 0;
}
