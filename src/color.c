#include "color.h"

#include <stdlib.h>

#define N_STOPS 11

static uint8_t lerp_u8(uint8_t a, uint8_t b, float t) {
    return (uint8_t)(a + (b - a) * t + 0.5f);
}

rgb_t value_to_rgb(float value) {
    if (value < -1.0f) value = -1.0f;
    if (value >  1.0f) value =  1.0f;

    static const rgb_t stops[N_STOPS] = {
        { 165,   0,  38 },  // #A50026  тёмно-красный   (-1.0)
        { 215,  48,  39 },  // #D73027
        { 244, 109,  67 },  // #F46D43
        { 253, 174,  97 },  // #FDAE61
        { 254, 224, 139 },  // #FEE08B
        { 255, 255, 191 },  // #FFFFBF  бледно-жёлтый    (0.0)
        { 217, 239, 139 },  // #D9EF8B
        { 166, 217, 106 },  // #A6D96A
        { 102, 189,  99 },  // #66BD63
        {  26, 152,  80 },  // #1A9850
        {   0, 104,  55 },  // #006837  тёмно-зелёный    (+1.0)
    };

    float pos  = (value + 1.0f) * 0.5f * (float)(N_STOPS - 1);
    int   idx  = (int)pos;
    if (idx > N_STOPS - 2) idx = N_STOPS - 2;
    float frac = pos - (float)idx;

    rgb_t c;
    c.r = lerp_u8(stops[idx].r, stops[idx + 1].r, frac);
    c.g = lerp_u8(stops[idx].g, stops[idx + 1].g, frac);
    c.b = lerp_u8(stops[idx].b, stops[idx + 1].b, frac);
    return c;
}