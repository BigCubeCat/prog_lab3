#pragma once

#include <stdint.h>
#include <math.h>

typedef struct rgb {
    uint8_t r, g, b;
} rgb_t;

// Вспомогательная функция для линейной интерполяции
static uint8_t lerp_u8(uint8_t a, uint8_t b, float t);

// Палитра RdYlBu (упрощённая, 5 опорных точек)
// Синий (отрицательные) -> Голубой -> Жёлтый (0) -> Оранжевый -> Красный (положительные)
rgb_t value_to_rgb(float value);
