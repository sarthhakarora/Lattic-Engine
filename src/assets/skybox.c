#include "raylib.h"
#include "rlgl.h"
#include "raymath.h"
#include "shadermanager.h"
#include "stddef.h"
#include <stdbool.h>
#include "skybox.h"

Sky sky_create(float radius, const char* vertex_shader_path, const char *fragment_shader_path) {
  Sky sky = {
    .radius = radius,
    .loaded = false,
  };

  sky.shader = shader_create(vertex_shader_path, fragment_shader_path);

  return sky;
}

void sky_load(Sky *sky) {
  if (sky->loaded) return;

  Mesh mesh = GenMeshSphere(sky->radius, 32, 32);
  sky->model = LoadModelFromMesh(mesh);

  shader_load(&sky->shader);

  sky->model.materials[0].shader = sky->shader.shader;

  sky->loaded = true;
}

void sky_unload(Sky *sky) {
  if (!sky->loaded) return;

  UnloadModel(sky->model);
  shader_unload(&sky->shader);

  sky->loaded = false;
}

void sky_draw(Sky *sky, Camera3D camera) {
      if (!sky->loaded) return;

    sky->model.transform = MatrixTranslate(
        camera.position.x,
        camera.position.y,
        camera.position.z
    );

    rlDisableBackfaceCulling();
    rlDisableDepthMask();

    DrawModel(sky->model, (Vector3){0,0,0}, 1.0f, WHITE);

    rlEnableDepthMask();
    rlEnableBackfaceCulling();
}
