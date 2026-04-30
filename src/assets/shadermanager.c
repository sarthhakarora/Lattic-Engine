#include "shadermanager.h"
#include "../log.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "raylib.h"

// find or cache uniform location
static int shader_get_uniform(ShaderData *shader, const char *name) {
  for (int i = 0; i < shader->uniform_count; i++) {
      if (strcmp(shader->uniforms[i].name, name) == 0) {
          return shader->uniforms[i].location;
      }
  }

  int loc = GetShaderLocation(shader->shader, name);

  if (shader->uniform_count >= shader->uniform_capacity) {
      shader->uniform_capacity *= 2;
      shader->uniforms = realloc(shader->uniforms,
                            sizeof(UniformEntry) * shader->uniform_capacity);
  }

  shader->uniforms[shader->uniform_count].name = name;
  shader->uniforms[shader->uniform_count].location = loc;
  shader->uniform_count++;

  if (loc == -1) {
    log_msg(LOG_ENGINE, LOG_LEVEL_ERROR, "Uniform %s not found", name);
    return -1;
  }
  return loc;
}

ShaderData shader_create(const char* path_vs, const char* path_fs) {
  // init paths + allocate uniform array
  ShaderData shader = {
    .path_vs = path_vs,
    .path_fs = path_fs,
    .loaded = false,
    .uniform_count = 0,
    .uniform_capacity = 8,
  };

  shader.uniforms = malloc(sizeof(UniformEntry) * shader.uniform_capacity);

  return shader;
}

void shader_load(ShaderData *shader) {
  // LoadShader into shader->shader
  if(!shader->loaded) {
    shader->shader = LoadShader(shader->path_vs, shader->path_fs);
    shader->loaded = true;
  }
}

void shader_reload(ShaderData *shader) {
  if(shader->loaded) {
      UnloadShader(shader->shader);
  } else {
    log_msg(LOG_ENGINE, LOG_LEVEL_ERROR, "Called Reload to a shader that has not been loaded in the first place");
    return;
  }
  shader->shader = LoadShader(shader->path_vs, shader->path_fs);
  shader->loaded = true;

  shader->uniform_count = 0;
}

void shader_set_float(ShaderData *shader, const char* name, float num) {
  int loc = shader_get_uniform(shader, name);
  if (loc != -1) {
    SetShaderValue(shader->shader, loc, &num, SHADER_UNIFORM_FLOAT);
  }
}

void shader_set_vec2(ShaderData *shader, const char* name, float x, float y) {
  int loc = shader_get_uniform(shader, name);
  Vector2 vec = (Vector2){x, y};
  if (loc != -1) {
    SetShaderValue(shader->shader, loc, &vec, SHADER_UNIFORM_VEC2);
  }
}

void shader_set_vec3(ShaderData *shader, const char* name, float x, float y, float z) {
  int loc = shader_get_uniform(shader, name);
  Vector3 vec = (Vector3){x, y, z};
  if (loc != -1) {
    SetShaderValue(shader->shader, loc, &vec, SHADER_UNIFORM_VEC3);
  }
}

void shader_set_texture(ShaderData *shader, const char* name, Texture2D texture) {
  int loc = shader_get_uniform(shader, name);
  if (loc != -1) {
    SetShaderValueTexture(shader->shader, loc, texture);
  }
}

void shader_unload(ShaderData *shader) {
  if (shader->loaded) {
    UnloadShader(shader->shader);
  }

  free(shader->uniforms);

  shader->uniforms = NULL;
  shader->uniform_count = 0;
  shader->uniform_capacity = 0;
  shader->loaded = false;
}
