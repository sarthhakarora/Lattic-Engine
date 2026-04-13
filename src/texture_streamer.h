#pragma once
#include "raylib.h"
#include "stdbool.h"

typedef struct {
    char path[256];

    Image img;
    bool imageLoaded;

    bool gpuUploaded;
    bool failed;
} TextureStreamJob;


void loader_init(void);
void loader_start(const char** paths, int count);
bool loader_poll(Image* outImage, const char** outPath);
bool loader_is_done(void);
void loader_shutdown(void);
