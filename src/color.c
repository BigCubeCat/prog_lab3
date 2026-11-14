#include "color.h"

#include <stdlib.h>

static uint8_t lerp_u8(uint8_t a, uint8_t b, float t) {
    return (uint8_t)(a + (b - a) * t + 0.5f);
}

rgb_t value_to_rgb(float value) {
    // Ограничиваем значение диапазоном [-1, 1]
    if (value < -1.0f) value = -1.0f;
    if (value >  1.0f) value =  1.0f;

    // Опорные цвета (RGB)
    // 0: тёмно-синий    (-1.0)
    // 1: светло-голубой (-0.5)
    // 2: жёлтый         ( 0.0)
    // 3: оранжевый      ( 0.5)
    // 4: тёмно-красный  ( 1.0)
    static const struct rgb stops[5] = {
        { 49,  54, 149}, // тёмно-синий
        { 171, 217, 233}, // светло-голубой
        { 255, 255, 191}, // жёлтый
        { 253, 174,  97}, // оранжевый
        { 215,  48,  39}  // тёмно-красный
    };

    float pos = (value + 1.0f) * 2.0f;
    int idx = (int)pos;
    if (idx > 3) idx = 3; // защита от выхода за границы при value == 1.0

    float frac = pos - (float)idx;

    rgb_t result;
    result.r = lerp_u8(stops[idx].r, stops[idx + 1].r, frac);
    result.g = lerp_u8(stops[idx].g, stops[idx + 1].g, frac);
    result.b = lerp_u8(stops[idx].b, stops[idx + 1].b, frac);

    return result;
}
