#pragma once

#include <stdint.h>

typedef struct point3d {
    float x, y, z;
} point3d_t;

typedef struct scene {
    int width;
    int height;
    int depth;
    uint32_t seed;
    float *data; // 3D array flattened into 1D
} scene_t;

float dot(point3d_t a, point3d_t b);

float lerp(float a, float b, float t);

float fade(float t);

float noise3d(float x, float y, float z, uint32_t seed);

void scene_init(int width, int height, int depth, uint32_t seed);

void scene_generate();

scene_t *get_scene();
