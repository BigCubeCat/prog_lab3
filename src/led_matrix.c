#include "led_matrix.h"
#include <stdlib.h>

#include "ws2812.pio.h"

#include "hardware/pio.h"
#include "pico/stdlib.h"

#define LED_FREQUENCY  800000.0f

static PIO pio     = pio0;
static uint sm     = 0;
static uint offset = 0;


static uint32_t make_pixel(uint8_t r, uint8_t g, uint8_t b) {
    /*
     * WS2812 expects color bytes in GRB order.
     * The PIO sends each 24-bit word MSB first.
     */
    return ((uint32_t)g << 16u) | ((uint32_t)r << 8u) | (uint32_t)b;
}

led_matrix_t *get_led_matrix() {
    static led_matrix_t m;
    return &m;
}


void led_matrix_init(int count_rows, int count_cols, int pin) {
    led_matrix_t *m = get_led_matrix();
    m->count_rows = count_rows;
    m->count_cols = count_cols;
    m->total_count = count_rows * count_cols;
    m->pin = pin;

    offset = pio_add_program(pio, &ws2812_program);

    /*
     * Everything timing-related is described by ws2812.pio.
     * This generated helper configures the GPIO, sideset pin,
     * shift direction, autopull, FIFO and clock divider.
     */
    ws2812_program_init(pio, sm, offset, m->pin, LED_FREQUENCY, false);

    led_matrix_fill(0, 0, 0);
    led_matrix_show();
}

void led_matrix_fill(uint8_t r, uint8_t g, uint8_t b) {
    led_matrix_t *m = get_led_matrix();
    const uint32_t pixel = make_pixel(r, g, b);

    for (uint i = 0; i < m->total_count; ++i) {
        m->pixels[i] = pixel;
    }
}

void led_matrix_set_pixel(
    uint8_t x,
    uint8_t y,
    uint8_t r,
    uint8_t g,
    uint8_t b
) {
    led_matrix_t *m = get_led_matrix();
    if (x >= m->count_cols || y >= m->count_rows) {
        return;
    }
    m->pixels[y * m->count_rows + x] = make_pixel(r, g, b);
}

void led_matrix_show(void) {
    led_matrix_t *m = get_led_matrix();
    for (uint i = 0; i < m->total_count; ++i) {
        pio_sm_put_blocking(pio, sm, m->pixels[i] << 8u);
    }

    /*
     * WS2812 latches the received frame after the data line
     * stays low long enough.
     */
    sleep_us(80);
}
