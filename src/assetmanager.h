#pragma once

#include "raylib.h"
#include "uthash.h"
#include <stdbool.h>

typedef struct {
    Image img;
    Texture2D texture;
    int refCount;
    bool loaded;
} ManagedTexture;

typedef struct {
    const char* key;
    ManagedTexture managed;
    UT_hash_handle hh;
} TextureEntry;

typedef struct {
    Texture2D texture;
    int refCount;
} TextureAsset;

extern TextureEntry *texture_map;

ManagedTexture *asset_get_texture(const char *path);
void asset_release_texture(const char *path);
void asset_manager_shutdown(void);
