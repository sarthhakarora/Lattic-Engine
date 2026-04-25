#include "assetmanager.h"
#include "planet.h"
#include "raylib.h"
#include "texture_streamer.h"
#include "uthash.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

TextureEntry *texture_map = NULL;

ManagedTexture *asset_get_texture(const char *path) {
  TextureEntry *entry = NULL;

  HASH_FIND_STR(texture_map, path, entry);

  if (!entry) {
    entry = malloc(sizeof(TextureEntry));
    entry->key = strdup(path);
    entry->managed.refCount = 1;
    entry->managed.texture.id = 0;
    entry->managed.loaded = false;
    entry->managed.loading = true;
    HASH_ADD_KEYPTR(hh, texture_map, entry->key, strlen(entry->key), entry);

    const char *paths[] = {entry->key};
    loader_start(paths, 1);

  } else {
    entry->managed.refCount++;
  }

  return &entry->managed;
}

void asset_update(void) {
  Image img;
  char path[256];

  while (loader_poll(&img, path)) {
    TextureEntry *entry = NULL;
    HASH_FIND_STR(texture_map, path, entry);
    
    if (entry && entry->managed.loading) {
      entry->managed.texture = LoadTextureFromImage(img);
      entry->managed.loaded = true;
      entry->managed.loading = false;
    }

    UnloadImage(img);
  }
}

void asset_release_texture(const char *path) {
  TextureEntry *entry = NULL;
  HASH_FIND_STR(texture_map, path, entry);

  if (!entry) {
    return;
  }

  if (entry->managed.refCount <= 0) {
    return;
  }

  entry->managed.refCount--;

  if (entry->managed.refCount <= 0) {
    if (entry->managed.loaded) {
      UnloadTexture(entry->managed.texture);
    }
    HASH_DEL(texture_map, entry);

    free((void *)entry->key);
    free(entry);
  }
}

void asset_manager_shutdown(void) {
  TextureEntry *entry, *tmp;

  HASH_ITER(hh, texture_map, entry, tmp) {
    if (entry->managed.loaded) {
      UnloadTexture(entry->managed.texture);
    }
    HASH_DEL(texture_map, entry);
    free((void *)entry->key);
    free(entry);
  }
}
