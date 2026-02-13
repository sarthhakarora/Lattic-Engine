// ============================================================
// LUA API TODO — PRIORITIZED
// Personal C → Lua Orbital / N-Body Engine
// ============================================================
// PART 1 — CORE ESSENTIALS (BARE MINIMUM TO FUNCTION)
// ============================================================
//
// Engine
// engine.pause()
// engine.resume()
// engine.is_paused() -> bool
// engine.get_time() -> number
// engine.get_dt() -> number
//
// World
// create_world() -> world
// world:step()
// world:set_gravity(G)
// world:get_gravity() -> number
// world:clear()
//
// Planet (creation + core state)
// create_planet(mass, vec3_pos) -> planet
// planet:destroy()
// planet:is_alive() -> bool
//
// Planet (motion)
// planet:get_position() -> vec3
// planet:set_position(vec3)
// planet:get_velocity() -> vec3
// planet:set_velocity(vec3)
//
// ============================================================
// PART 2 — PHYSICS CONTROL & FORCE APPLICATION
// ============================================================
//
// Planet (mass & forces)
// planet:get_mass() -> number
// planet:set_mass(mass)
// planet:apply_force(vec3)
// planet:apply_impulse(vec3)
// planet:clear_forces()
//
// World helpers
// world:get_planet_count() -> number
// world:has_planet(planet) -> bool
//
// Physics helpers
// compute_gravitational_force(planet_a, planet_b) -> vec3
// distance_between(planet_a, planet_b) -> number
//
// Simulation control
// world:set_time_scale(scale)
// world:get_time_scale() -> number
//
// ============================================================
// PART 3 — VECTOR MATH & NUMERICAL UTILITIES
// ============================================================
//
// Vector creation
// vec3(x, y, z) -> vec3
//
// Vector inspection
// vec3:length() -> number
// vec3:length_squared() -> number
//
// Vector ops
// vec3:normalize() -> vec3
// vec3:dot(other) -> number
// vec3:cross(other) -> vec3
//
// Vector arithmetic
// vec3:add(other) -> vec3
// vec3:sub(other) -> vec3
// vec3:mul(scalar) -> vec3
// vec3:div(scalar) -> vec3
//
// Scalar helpers
// clamp(value, min, max) -> number
// lerp(a, b, t) -> number
//
// ============================================================
// PART 4 — CALLBACKS, DEBUGGING, INSPECTION
// ============================================================
//
// Callbacks
// on_update(function(dt))
// on_fixed_update(function(dt))
// on_planet_destroyed(function(planet))
//
// Debug rendering
// debug.draw_vector(position, vector, color)
// debug.draw_point(position, color)
// debug.draw_text(position, string)
//
// Debug logging
// debug.log(value)
// debug.warn(value)
// debug.error(value)
//
// Inspection
// planet:get_id() -> number        // debug only
// planet:get_force_accumulator() -> vec3
//
// ============================================================
// PART 5 — QUALITY OF LIFE / ADVANCED (ONLY IF NEEDED)
// ============================================================
//
// World queries
// world:get_planets() -> table<planet>
// world:find_nearest_planet(position) -> planet
//
// Planet state flags
// planet:enable_gravity(bool)
// planet:is_gravity_enabled() -> bool
//
// Simulation control (advanced)
// engine.step_once()
// engine.set_fixed_dt(dt)
// engine.get_fixed_dt() -> number
//
// Persistence (optional)
// save_world(path)
// load_world(path)
//
// Performance / metrics
// engine.get_fps() -> number
// engine.get_step_time() -> number
//
// register_texture(name, path)
// engine.preload_assets()
// engine.are_assets_loaded() -> bool (optional)
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

// <-------- Setters -------->
int l_set_world_gravity_constant(lua_State *L);

// <-------- Getters -------->
int l_get_world_gravity_constant(lua_State *L);
int l_get_time_state(lua_State *L);
int l_get_delta_time(lua_State *L);

// <-------- Engine Control -------->
int l_pause(lua_State *L);
int l_resume(lua_State *L);

// <-------- Input -------->
int l_IsKeyPressed(lua_State *L);
int l_IsKeyDown(lua_State *L);
int l_IsKeyUp(lua_State *L);