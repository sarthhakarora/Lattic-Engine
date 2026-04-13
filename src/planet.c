#include "planet.h"
#include "assetmanager.h"
#include "raylib.h"
#include "raymath.h"
#include "stdlib.h"
#include "string.h"
#include <stdio.h>

Planet planet_create(char *name, char *texture_path, float radius,
                     Vector3 position, int rings, int slices, int mass,
                     bool has_gravity) {
  ManagedTexture *tex = asset_get_texture(texture_path);

  Planet planet = {
      .name = strdup(name),
      .texture_path = strdup(texture_path),
      .texture = tex,
      .radius = radius,
      .position = position,
      .rings = rings,
      .slices = slices,
      .mass = mass,
      .has_gravity = has_gravity,
      .perpendicular_direction = (Vector3){0},
      .acceleration = (Vector3){0},
      .velocity = (Vector3){0},
  };

  if (planet.rings < 3)
    planet.rings = 3;
  if (planet.slices < 3)
    planet.slices = 3;
  if (planet.radius <= 0.01f)
    planet.radius = 0.01f;

  Mesh mesh = GenMeshSphere(planet.radius, planet.rings, planet.slices);
  planet.model = LoadModelFromMesh(mesh);

  if (planet.model.materialCount > 0) {
    planet.model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture =
        planet.texture->texture;
  }
  planet.model.transform = MatrixRotateY(PI);

  return planet;
}

void remake_model(Planet *planet) {
  if (planet->model.meshCount > 0) {
    UnloadModel(planet->model);
  }

  Mesh mesh = GenMeshSphere(planet->radius, planet->rings, planet->slices);
  planet->model = LoadModelFromMesh(mesh);

  if (planet->rings < 3)
    planet->rings = 3;
  if (planet->slices < 3)
    planet->slices = 3;
  if (planet->radius <= 0.01f)
    planet->radius = 0.01f;

  if (planet->model.materialCount > 0) {
    planet->model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture =
        planet->texture->texture;
  }
  planet->model.transform = MatrixRotateY(PI);
}
void planet_draw(Planet *planet) {
  if (planet->model.meshCount == 0) {
    return;
  }

  DrawModel(planet->model, planet->position, 1.0f, WHITE);
}
void planet_update(Planet *planet) {
  planet->model.transform = MatrixMultiply(MatrixRotateY(GetTime() * 0.1f),
                                           MatrixRotateX(GetTime() * 0.1f));
}
void planet_destroy(Planet *planet) {
  if (!planet) {
    return;
  }
  UnloadModel(planet->model);
  asset_release_texture(planet->texture_path);

  free(planet->name);

  planet->name = NULL;
  planet->texture_path = NULL;
  planet->texture = NULL;
}
