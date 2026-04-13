#pragma once

#include "raylib.h"

typedef struct {
    Camera3D camera;
    float pitch;
    float yaw;
    float sensitivity;
    Vector3 forward;
    float velocity;
    float deltaTime;
    bool rotate_active;

} Cam;

Cam create_camera(void);
void update_camera(Cam *camera);