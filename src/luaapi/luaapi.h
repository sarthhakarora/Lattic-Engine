// ============================================================
// LUA API TODO — CLEANED & FLAT (HANDLE-BASED)
// Personal C → Lua Orbital / N-Body Engine
// ============================================================
//
// ------------------------------------------------------------
// PART 1 — CORE (CURRENT / MINIMAL STABLE API)
// ------------------------------------------------------------
//
// -- Engine Control
// pause()
// resume()
// is_paused() -> bool
// get_dt() -> number
//
// -- World
// create_world()
// set_gravity(G)
// get_gravity() -> number
// clear_world()
//
// -- Planet (Handle-Based)
// create_planet{ ... } -> id
// destroy_planet(id)
// planet_exists(id) -> bool
//
//
// ------------------------------------------------------------
// PART 2 — PLANET STATE & PHYSICS CONTROL
// ------------------------------------------------------------
//
// -- Planet State
// planet_get_mass(id) -> number
// planet_set_mass(id, mass)
//
// planet_get_position(id) -> vec3
// planet_set_position(id, vec3)
//
// planet_get_velocity(id) -> vec3
// planet_set_velocity(id, vec3)
//
// -- Forces
// planet_apply_force(id, vec3)
// planet_apply_impulse(id, vec3)
// planet_clear_forces(id)
//
// -- World Helpers
// get_planet_count() -> number
//
//
// ------------------------------------------------------------
// PART 3 — VECTOR MATH
// ------------------------------------------------------------
//
// vec3(x, y, z) -> vec3
//
// vec3_length(v) -> number
// vec3_length_squared(v) -> number
// vec3_normalize(v) -> vec3
//
// vec3_dot(a, b) -> number
// vec3_cross(a, b) -> vec3
//
// vec3_add(a, b) -> vec3
// vec3_sub(a, b) -> vec3
// vec3_mul(v, scalar) -> vec3
// vec3_div(v, scalar) -> vec3
//
// clamp(value, min, max) -> number
// lerp(a, b, t) -> number
//
//
// ------------------------------------------------------------
// PART 4 — DEBUG / INSPECTION (OPTIONAL)
// ------------------------------------------------------------
//
// debug_log(value)
// debug_warn(value)
// debug_error(value)
//
// planet_get_id(id) -> number        -- debug only
// planet_get_force_accumulator(id) -> vec3
//
//
// ------------------------------------------------------------
// PART 5 — ADVANCED / OPTIONAL
// ------------------------------------------------------------
//
// get_planets() -> table<id>
// find_nearest_planet(vec3) -> id
//
// set_time_scale(scale)
// get_time_scale() -> number
//
// engine_step_once()
// set_fixed_dt(dt)
// get_fixed_dt() -> number
//
// save_world(path)
// load_world(path)
//
// get_fps() -> number
// get_step_time() -> number
//
// ============================================================
// END OF LUA API TODO
// ============================================================
#pragma once

#include "raylib.h"
#include "../planet.h"
#include "../world.h"
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

// <-------- Core -------->
void init_luaapi(const char *scriptPath, lua_State *L);
void init_lua(lua_State *L);
void update_lua(lua_State *L);

// <-------- World Creation -------->
int l_create_world(lua_State *L);
int l_world_add_planet(lua_State *L);

// <-------- Unload functions -------->
int l_world_unload(lua_State *L);
int l_planet_unload(lua_State *L);

// <-------- Setters -------->
int l_set_world_gravity_constant(lua_State *L);

// <-------- Getters -------->
int l_get_world_gravity_constant(lua_State *L);
int l_get_time_state(lua_State *L);
int l_get_delta_time(lua_State *L);

// <-------- Engine Control -------->
int l_pause(lua_State *L);
int l_resume(lua_State *L);
int l_find_planet(lua_State *L);

// <-------- Input -------->
int l_IsKeyPressed(lua_State *L);
int l_IsKeyDown(lua_State *L);
int l_IsKeyUp(lua_State *L);