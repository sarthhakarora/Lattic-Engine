#pragma once

#include "raylib.h"
#include "planet.h"

typedef struct {
    Planet* planets;
    int planet_count;
    float gravity_strength;
    int gravity_planet_indexs[100];
    int non_gravity_planet_indexs[100];
    int count_g;
    int count_ng;
    float deltaTime;
} World;

World world_create(Planet* planets, int planet_count);
void world_update(World* world);
void world_draw(World* world);
void world_destroy(World* world);