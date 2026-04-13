#pragma once

#include "raylib.h"
#include <stdbool.h>
#include "assetmanager.h"

typedef struct {
    char* name;
    int64_t id;
    char* texture_path;
    ManagedTexture *texture;
    Model model;
    float radius;
    Vector3 position;
    int rings;
    int slices;
    float mass;
    Vector3 velocity;
    Vector3 perpendicular_direction;
    Vector3 acceleration;
    bool has_gravity;
} Planet;

Planet planet_create(char* name, char* texture_path, float radius, Vector3 position, int rings, int slices, int mass, bool has_gravity);
void remake_model(Planet *planet);
void planet_draw(Planet *planet);
void planet_update(Planet *planet);
void planet_destroy(Planet *planet);