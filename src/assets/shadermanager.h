#pragma once
#include "raylib.h"

typedef struct {
  const char* name;
  int location;
} UniformEntry;

typedef struct {
  Shader shader;
  bool loaded;

  const char* path_vs;
  const char* path_fs;

  UniformEntry* uniforms;
  int uniform_count;
  int uniform_capacity;

} ShaderData;

ShaderData shader_create(const char* path_vs, const char* path_fs);
void shader_load(ShaderData *shader);
void shader_set_float(ShaderData *shader, const char* name, float num);
void shader_set_vec2(ShaderData *shader, const char* name, float x, float y);
void shader_set_vec3(ShaderData *shader, const char* name, float x, float y, float z);
void shader_set_texture(ShaderData *shader, const char* name, Texture2D texture);
void shader_unload(ShaderData *shader);
