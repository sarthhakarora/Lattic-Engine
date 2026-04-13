#pragma once

#include "planet.h"
#include "raylib.h"

typedef enum { LINKS_OFF, LINKS_NEAREST, LINKS_STRONGEST, LINKS_ALL } LinkMode;

typedef struct {
  bool valid;

  Planet **planets;
  int64_t id;
  int64_t next_planet_id;
  int planet_count;
  unsigned int planet_capacity;
  float gravity_strength;
  int *gravity_planet_indexs;
  int *non_gravity_planet_indexs;
  int count_g;
  int count_ng;
  float deltaTime;
  float timeScale;
  float savedTimeScale;
  bool isPaused;
  LinkMode linkMode;
  bool drawGrid;
  bool linksEnabled;
  bool ui_typing;
} World;

World world_create();
bool world_add_planet(Planet *planet, World *world);
Planet *find_planet(World *world, int64_t id);
void world_unload_planet(World *world, int64_t id);
void world_update(World *world);
void world_draw(World *world);
void world_destroy(World *world);
