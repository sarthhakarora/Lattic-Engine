#include "camera.h"
#include "raylib.h"
#include "rlgl.h"
#include "stdio.h"
#include "raymath.h"

Cam create_camera(void)
{
    Cam cam = {
        .camera = {
            .position = (Vector3){ 0.0f, 100.0f, 0.0f },
            .target = (Vector3){ 0.0f, 0.0f, 0.0f },
            .up = (Vector3){ 0.0f, 1.0f, 0.0f },
            .fovy = 90.0f,
            .projection = CAMERA_PERSPECTIVE,
        },
        .pitch = 0.0f,
        .yaw = 0.0f,
        .sensitivity = 0.1f,
        .velocity = 100.0f,
        .deltaTime = 0.0f,
    };

    cam.forward = Vector3Normalize(Vector3Subtract(cam.camera.target, cam.camera.position));
    rlSetClipPlanes(0.0001f, 10000000.0f);

    return cam;
}

static void move_camera(Cam *camera)
{
    Vector3 right = Vector3Normalize(Vector3CrossProduct(camera->forward, camera->camera.up));
    Vector3 up = {0.0f, 1.0f, 0.0f};
    float accel = 20.0f;

    if(IsKeyDown(KEY_Q)) {
        camera->velocity += accel * camera->deltaTime;
    }
    if(IsKeyDown(KEY_E)) {
        camera->velocity -= accel * camera->deltaTime;
    }

    camera->velocity = Clamp(camera->velocity, 0.1f, 500.0f);

    if(IsKeyDown(KEY_W)) {
        camera->camera.position = Vector3Add(camera->camera.position, Vector3Scale(camera->forward, camera->velocity * camera->deltaTime));
    }
    if(IsKeyDown(KEY_S)) {
        camera->camera.position = Vector3Subtract(camera->camera.position, Vector3Scale(camera->forward, camera->velocity * camera->deltaTime));
    }
    if(IsKeyDown(KEY_A)) {
        camera->camera.position = Vector3Subtract(camera->camera.position, Vector3Scale(right, camera->velocity * camera->deltaTime));
    }
    if(IsKeyDown(KEY_D)) {
        camera->camera.position = Vector3Add(camera->camera.position, Vector3Scale(right, camera->velocity * camera->deltaTime));
    }
    if(IsKeyDown(KEY_LEFT_SHIFT) && IsKeyDown(KEY_SPACE)) {
        camera->camera.position = Vector3Subtract(camera->camera.position, Vector3Scale(up, camera->velocity * camera->deltaTime));
    }
    else if(IsKeyDown(KEY_SPACE)) {
        camera->camera.position = Vector3Add(camera->camera.position, Vector3Scale(up, camera->velocity * camera->deltaTime));
    }
}

void update_camera(Cam *camera)
{   
    camera->deltaTime = GetFrameTime();

    move_camera(camera);

    camera->forward = Vector3Normalize((Vector3){
        cos(camera->pitch) * cos(camera->yaw),
        sin(camera->pitch),
        cos(camera->pitch) * sin(camera->yaw)
    });


    camera->forward = Vector3Normalize(camera->forward);
    camera->camera.target = Vector3Add(
            camera->camera.position,
            camera->forward
        );
    
    if(camera->rotate_active) {
        Vector2 mouseDelta = GetMouseDelta();

        const float sen = 0.001f;
        camera->pitch -= mouseDelta.y * sen;
        camera->yaw   += mouseDelta.x * sen;

        camera->pitch = Clamp(camera->pitch, -1.5f, 1.5f);

    }
}