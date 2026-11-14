#pragma once

#include <stdint.h>

#define SIZE 128

typedef struct led_matrix {
    int count_rows;
    int count_cols;
    int total_count;
    int pin;

    uint32_t pixels[SIZE];
} led_matrix_t;

void led_matrix_init(int count_rows, int count_cols, int pin);

led_matrix_t *get_led_matrix();

void led_matrix_fill(
    uint8_t r,
    uint8_t g,
    uint8_t b
);

void led_matrix_set_pixel(
    uint8_t x,
    uint8_t y,
    uint8_t r,
    uint8_t g,
    uint8_t b
);

void led_matrix_show(void);

