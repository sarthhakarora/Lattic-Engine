#pragma once

#include "raylib.h"
#include "shadermanager.h"

typedef struct {
    Model model;
    ShaderData shader;
    float radius;
    bool loaded;
} Sky;

Sky sky_create(float radius, const char* vertex_shader_path, const char *fragment_shader_path);
void sky_load(Sky *sky);
void sky_unload(Sky *sky);
void sky_draw(Sky *sky, Camera3D camera);
