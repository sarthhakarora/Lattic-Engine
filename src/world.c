#include "world.h"
#include "core.h"
#include "planet.h"
#include "raylib.h"
#include "raymath.h"
#include "stdlib.h"
#include "string.h"
#include <float.h>
#include <stdbool.h>
#include <stdio.h>

extern Core *global_core;

static void initorbit(World *world) {
  if (world->planet_count == 0) {
    return;
  }

  world->count_g = 0;
  world->count_ng = 0;

  for (int i = 0; i < world->planet_count; i++) {
    if (world->planets[i]->has_gravity == true) {
      world->gravity_planet_indexs[world->count_g++] = i;
    }

    if (world->planets[i]->has_gravity == false) {
      world->non_gravity_planet_indexs[world->count_ng++] = i;
    }
  }

  if (world->count_g == 0 || world->count_ng == 0) {
    return;
  }

  int sun = world->gravity_planet_indexs[0];
  Vector3 up = (Vector3){0.0f, 1.0f, 0.0f};

  for (int j = 0; j < world->count_ng; j++) {
    int p = world->non_gravity_planet_indexs[j];
    Vector3 r = Vector3Subtract(world->planets[p]->position,
                                world->planets[sun]->position);

    float distance = Vector3Length(r);
    if (distance < 0.0001f) {
      continue;
    }

    Vector3 perpendicular_direction =
        Vector3Normalize(Vector3CrossProduct(up, r));

    float speed =
        sqrt(world->gravity_strength * world->planets[sun]->mass / distance);

    world->planets[p]->velocity = Vector3Scale(perpendicular_direction, speed);
    world->planets[p]->acceleration = (Vector3){0, 0, 0};
  }
}

World world_create() {
  World world = {
      .valid = true,
      .planets = malloc(sizeof(Planet *) * 8),
      .next_planet_id = 1,
      .planet_count = 0,
      .planet_capacity = 8,
      .gravity_strength = 50.0f,
      .deltaTime = 0.0f,
      .timeScale = 1.0f,
      .savedTimeScale = 1.0f,
      .isPaused = false,
      .gravity_planet_indexs = malloc(sizeof(int) * world.planet_capacity),
      .non_gravity_planet_indexs = malloc(sizeof(int) * world.planet_capacity),
      .linkMode = LINKS_NEAREST,
      .drawGrid = true,
      .linksEnabled = true,
      .ui_typing = false};

  world.id = global_core->next_world_id++;

  return world;
}

bool world_add_planet(Planet *planet, World *world) {
  if (!world)
    return false;

  if (world->planet_count >= world->planet_capacity) {
    int new_cap = world->planet_capacity * 2;

    Planet **new_planets = realloc(world->planets, new_cap * sizeof(Planet *));

    if (!new_planets)
      return false;

    int *new_g = realloc(world->gravity_planet_indexs, sizeof(int) * new_cap);
    if (!new_g)
      return false;

    int *new_ng =
        realloc(world->non_gravity_planet_indexs, sizeof(int) * new_cap);
    if (!new_ng)
      return false;

    world->planets = new_planets;
    world->planet_capacity = new_cap;
    world->gravity_planet_indexs = new_g;
    world->non_gravity_planet_indexs = new_ng;
    world->planet_capacity = new_cap;
  }

  world->planets[world->planet_count++] = planet;
  planet->id = world->next_planet_id++;
  // printf("%d\n", planet->id);

  initorbit(world);

  return true;
}

static void world_ui(World *world) {
  if (!world)
    return;

  switch (world->linkMode) {

  case LINKS_OFF:
    break;

  case LINKS_NEAREST: {
    for (int i = 0; i < world->planet_count; i++) {
      Planet *p = world->planets[i];
      if (!p)
        continue;

      float bestDistSq = FLT_MAX;
      Planet *closest = NULL;

      for (int j = 0; j < world->count_g; j++) {
        int gi = world->gravity_planet_indexs[j];

        if (gi < 0 || gi >= world->planet_count)
          continue;

        Planet *g = world->planets[gi];
        if (!g || g == p)
          continue;

        Vector3 d = Vector3Subtract(g->position, p->position);
        float distSq = Vector3LengthSqr(d);

        if (distSq < bestDistSq) {
          bestDistSq = distSq;
          closest = g;
        }
      }

      if (closest) {
        DrawLine3D(p->position, closest->position, (Color){167, 139, 250, 80});
      }
    }
  } break;

  case LINKS_STRONGEST: {
    for (int i = 0; i < world->planet_count; i++) {
      Planet *p = world->planets[i];
      if (!p)
        continue;

      float bestForce = 0.0f;
      Planet *strongest = NULL;

      for (int j = 0; j < world->count_g; j++) {
        int gi = world->gravity_planet_indexs[j];

        if (gi < 0 || gi >= world->planet_count)
          continue;

        Planet *g = world->planets[gi];
        if (!g || g == p)
          continue;

        Vector3 d = Vector3Subtract(g->position, p->position);
        float distSq = Vector3LengthSqr(d);

        if (distSq < 0.0001f)
          continue;

        float force = g->mass / distSq;

        if (force > bestForce) {
          bestForce = force;
          strongest = g;
        }
      }

      if (strongest) {
        DrawLine3D(p->position, strongest->position,
                   (Color){79, 195, 247, 100});
      }
    }
  } break;

  case LINKS_ALL: {
    for (int i = 0; i < world->planet_count; i++) {
      Planet *p = world->planets[i];
      if (!p)
        continue;

      for (int j = 0; j < world->count_g; j++) {
        int gi = world->gravity_planet_indexs[j];

        if (gi < 0 || gi >= world->planet_count)
          continue;

        Planet *g = world->planets[gi];
        if (!g || g == p)
          continue;

        DrawLine3D(p->position, g->position, (Color){120, 120, 120, 40});
      }
    }
  } break;
  }
}

void world_draw(World *world) {
  world_ui(world);
  for (int i = 0; i < world->planet_count; i++) {
    planet_draw(world->planets[i]);
  }
}

void world_update(World *world) {
  Vector3 direction;
  float distance;
  static float old_timeScale = 0.0f;
  world->deltaTime = GetFrameTime() * world->timeScale;

  world->count_g = 0;
  world->count_ng = 0;

  for (int i = 0; i < world->planet_count; i++) {
    planet_update(world->planets[i]); // This is visual only
    if (world->planets[i]->has_gravity == true) {
      world->gravity_planet_indexs[world->count_g++] = i;
    }

    if (world->planets[i]->has_gravity == false) {
      world->non_gravity_planet_indexs[world->count_ng++] = i;
    }
  }

  for (int i = 0; i < world->count_ng; i++) {
    Vector3 total_force = (Vector3){0.0f, 0.0f, 0.0f};
    int p = world->non_gravity_planet_indexs[i];
    Vector3 old_acceleration = world->planets[p]->acceleration;
    Vector3 r;

    for (int j = 0; j < world->count_g; j++) {

      int g = world->gravity_planet_indexs[j];
      r = Vector3Subtract(world->planets[g]->position,
                          world->planets[p]->position);
      direction = Vector3Normalize(r);
      distance = Vector3Length(r);

      if (distance < 0.0001f) { // stop divide by zero errors
        continue;
      }

      float strength = world->gravity_strength * world->planets[g]->mass /
                       (distance * distance);

      Vector3 force = Vector3Scale(direction, strength);
      total_force = Vector3Add(total_force, force);
    }
    world->planets[p]->acceleration =
        Vector3Scale(total_force, 1 / world->planets[p]->mass);
    world->planets[p]->position = Vector3Add(
        world->planets[p]->position,
        Vector3Add(Vector3Scale(world->planets[p]->velocity, world->deltaTime),
                   Vector3Scale(old_acceleration,
                                (0.5f * world->deltaTime * world->deltaTime))));
    world->planets[p]->velocity =
        Vector3Add(world->planets[p]->velocity,
                   Vector3Scale(Vector3Add(old_acceleration,
                                           world->planets[p]->acceleration),
                                world->deltaTime * 0.5f));
  }

  old_timeScale = world->deltaTime;
}

Planet *find_planet(World *world, int64_t id) {
  if (!world || !world->valid)
    return NULL;

  for (int i = 0; i < world->planet_count; i++) {
    Planet *p = world->planets[i];

    if (p && p->id == id) {
      return p;
    }
  }
  return NULL;
}

void world_unload_planet(World *world, int64_t id) {
  if (!world || !world->valid) {
    return;
  }

  for (int i = 0; i < world->planet_count; i++) {
    Planet *planet = world->planets[i];

    if (planet && planet->id == id) {
      planet_destroy(planet);
      free(planet);

      for (int j = i; j < world->planet_count - 1; j++) {
        world->planets[j] = world->planets[j + 1];
      }

      world->planets[world->planet_count - 1] = NULL;
      world->planet_count--;

      return;
    }
  }
}

void world_destroy(World *world) {
  if (world->valid) {
    for (int i = 0; i < world->planet_count; i++) {
      planet_destroy(world->planets[i]);
      free(world->planets[i]);
      world->planets[i] = NULL;
    }
  }

  free(world->planets);
  world->planets = NULL;
  world->planet_count = 0;
  world->valid = false;
}
