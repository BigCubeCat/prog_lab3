#include "perlin.h"

#include <math.h>
#include <stdint.h>
#include <stdlib.h>

float dot(point3d_t a, point3d_t b) {
    return a.x * b.x +
           a.y * b.y +
           a.z * b.z;
}

float lerp(float a, float b, float t) {
    return a + t * (b - a);
}

// Quintic fade function
float fade(float t) {
    return t * t * t *
           (t * (t * 6.0f - 15.0f) + 10.0f);
}

// Hash functions
uint32_t mix(uint32_t x) {
    x ^= x >> 16;
    x *= 0x7feb352dU;

    x ^= x >> 15;
    x *= 0x846ca68bU;

    x ^= x >> 16;

    return x;
}

uint32_t hash3(
    int x, int y, int z,
    uint32_t seed
) {
    uint32_t h = seed;

    h = mix(h ^ mix(
        (uint32_t)(x) + 0x9e3779b9U
    ));

    h = mix(h ^ mix(
        (uint32_t)(y) + 0x85ebca6bU
    ));

    h = mix(h ^ mix(
        (uint32_t)(z) + 0xc2b2ae35U
    ));

    return h;
}

// 12 gradient directions
point3d_t gradient(int x, int y, int z, uint32_t seed) {
    static const float s = 0.70710678118f;

    static const point3d_t gradients[12] = {
         s,  s, 0.0f,
        -s,  s, 0.0f,
         s, -s, 0.0f,
        -s, -s, 0.0f,

         s, 0.0f,  s,
        -s, 0.0f,  s,
         s, 0.0f, -s,
        -s, 0.0f, -s,

        0.0f,  s,  s,
        0.0f, -s,  s,
        0.0f,  s, -s,
        0.0f, -s, -s
    };

    uint32_t h = hash3(x, y, z, seed);

    return gradients[h % 12];
}

// Dot product between gradient and displacement
float corner(
    int ix, int iy, int iz,
    float x, float y, float z,
    uint32_t seed
) {
    point3d_t g = gradient(ix, iy, iz, seed);

    point3d_t d = {
        x - (float)(ix),
        y - (float)(iy),
        z - (float)(iz)
    };

    return dot(g, d);
}

float noise3d(
    float x, float y, float z,
    uint32_t seed
) {
    // Integer cube coordinates
    int x0 = (int)(floor(x));
    int y0 = (int)(floor(y));
    int z0 = (int)(floor(z));

    int x1 = x0 + 1;
    int y1 = y0 + 1;
    int z1 = z0 + 1;

    // Local coordinates
    float u = x - x0;
    float v = y - y0;
    float w = z - z0;

    // Fade
    float U = fade(u);
    float V = fade(v);
    float W = fade(w);

    // Eight corner contributions
    float n000 = corner(x0, y0, z0, x, y, z, seed);
    float n100 = corner(x1, y0, z0, x, y, z, seed);
    float n010 = corner(x0, y1, z0, x, y, z, seed);
    float n110 = corner(x1, y1, z0, x, y, z, seed);

    float n001 = corner(x0, y0, z1, x, y, z, seed);
    float n101 = corner(x1, y0, z1, x, y, z, seed);
    float n011 = corner(x0, y1, z1, x, y, z, seed);
    float n111 = corner(x1, y1, z1, x, y, z, seed);

    // Interpolation along X
    float a0 = lerp(n000, n100, U);
    float a1 = lerp(n010, n110, U);
    float a2 = lerp(n001, n101, U);
    float a3 = lerp(n011, n111, U);

    // Interpolation along Y
    float b0 = lerp(a0, a1, V);
    float b1 = lerp(a2, a3, V);

    // Interpolation along Z
    return lerp(b0, b1, W);
}

void scene_init(int width, int height, int depth, uint32_t seed) {
    scene_t *scene = get_scene();
    scene->width = width;
    scene->height = height;
    scene->depth = depth;
    scene->seed = seed;
    scene->data = (float *)malloc(
        width * height * depth * sizeof(float)
    );
}

scene_t *get_scene() {
    static scene_t scene;
    return &scene;
}

void scene_generate() {
    const float SCALE_X = 0.15f;
    const float SCALE_Y = 0.15f;
    const float SCALE_Z = 0.12f;

    scene_t *scene = get_scene();

    for (int x = 0; x < scene->width; ++x) {
        for (int y = 0; y < scene->height; ++y) {
            for (int z = 0; z < scene->depth; ++z) {
                scene->data[x * scene->height * scene->depth + y * scene->depth + z] = noise3d(
                    x * SCALE_X,
                    y * SCALE_Y,
                    z * SCALE_Z,
                    scene->seed
                );
            }
        }
    }
}

float fbm(float x, float y, float z) {
    float result = 0.0f;
    float amplitude = 1.0f;
    float frequency = 1.0f;
    float total_amplitude = 0.0f;

    for (int i = 0; i < 4; ++i) {
        result += amplitude * noise3d(
            x * frequency,
            y * frequency,
            z * frequency,
            42
        );

        total_amplitude += amplitude;

        amplitude *= 0.5f;
        frequency *= 2.0f;
    }

    return result / total_amplitude;
}
