#pragma once

#include "raylib.h"

typedef struct {
    char* name;
    char* texture_path;
    Model model;
    Texture2D texture;
    float radius;
    Vector3 position;
    int rings;
    int slices;
    float mass;
    float speed;
    Vector3 velocity;
    Vector3 perpendicular_direction;
    Vector3 acceleration;
    bool has_gravity;
} Planet;

Planet planet_create(char* name, char* texture_path, float radius, Vector3 position, int rings, int slices, float mass, float speed, bool has_gravity);
void remake_model(Planet *planet);
void planet_draw(Planet *planet);
void planet_update(Planet *planet);
void planet_destroy(Planet *planet);