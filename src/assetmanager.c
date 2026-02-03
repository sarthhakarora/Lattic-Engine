#include "assetmanager.h"
#include "uthash.h"
#include <string.h>
#include <stdlib.h>
#include "raylib.h"

TextureEntry *texture_map = NULL;

ManagedTexture *asset_get_texture(const char *path)
{
    TextureEntry *entry = NULL;

    HASH_FIND_STR(texture_map, path, entry);

    if(!entry) {
        entry = malloc(sizeof(TextureEntry));
        entry->key = path;
        entry->managed.texture = LoadTexture(path); 
        entry->managed.refCount = 1;
        entry->managed.loaded = true;

        HASH_ADD_KEYPTR(hh, texture_map, entry->key, strlen(entry->key), entry);
    } else {
        entry->managed.refCount++;
    }

    return &entry->managed;
}

void asset_release_texture(const char *path)
{
    TextureEntry *entry = NULL;
    HASH_FIND_STR(texture_map, path, entry);

    if (!entry) {
        return;
    }

    entry->managed.refCount--;

    if(entry->managed.refCount <= 0) {
        UnloadTexture(entry->managed.texture);
        HASH_DEL(texture_map, entry);

        free((void *) entry->key);
        free(entry);
    }
}

void asset_manager_shutdown(void)
{
    TextureEntry *entry, *tmp;

    HASH_ITER(hh, texture_map, entry, tmp) {
        UnloadTexture(entry->managed.texture);
        HASH_DEL(texture_map, entry);
        free((void *) entry->key);
        free(entry);
    }
}
