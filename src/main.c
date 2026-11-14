
#include <stdbool.h>
#include <stdint.h>

#include "pico/stdlib.h"

#include "led_matrix.h"
#include "perlin.h"
#include "color.h"

// Matrix configuration
#define LED_PIN         28
#define LED_ROWS        8
#define LED_COLS        8

// Animation configuration
#define FRAME_DELAY_MS  40
#define NOISE_SEED      42u

// Perlin parameters
#define NOISE_SCALE     0.1f
#define NOISE_GAIN      10.0f
#define TIME_STEP       0.012f

// LED brightness (0.0 - 1.0)
#define BRIGHTNESS      0.10f

// Clamp float to a specified range
static float clampf(float value, float min, float max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

// Reduce RGB brightness
static uint8_t dim_channel(uint8_t value) {
    return (uint8_t)((float)value * BRIGHTNESS + 0.5f);
}

// Fractal Brownian Motion (3 octaves)
static float fbm3d(
    float x,
    float y,
    float z,
    uint32_t seed
) {
    float result = 0.0f;
    float amplitude = 1.0f;
    float frequency = 1.0f;
    float total_amplitude = 0.0f;

    for (uint32_t octave = 0; octave < 3; ++octave) {
        result += amplitude * noise3d(
            x * frequency,
            y * frequency,
            z * frequency,
            seed + octave * 1013u
        );

        total_amplitude += amplitude;

        amplitude *= 0.5f;
        frequency *= 2.0f;
    }

    return result / total_amplitude;
}

// Generate one animation frame
static void render_frame(float time) {
    for (int y = 0; y < LED_ROWS; ++y) {
        for (int x = 0; x < LED_COLS; ++x) {

            // Spatial coordinates
            float nx = (float)x * NOISE_SCALE;
            float ny = (float)y * NOISE_SCALE;

            // Generate noise
            float noise = fbm3d(
                nx,
                ny,
                time,
                NOISE_SEED
            );

            // Increase contrast for the color palette
            noise = clampf(
                noise * NOISE_GAIN,
                -1.0f,
                1.0f
            );

            // Convert noise value into RGB
            rgb_t color = value_to_rgb(noise);

            // Draw pixel
            led_matrix_set_pixel(
                (uint8_t)x,
                (uint8_t)y,
                dim_channel(color.r),
                dim_channel(color.g),
                dim_channel(color.b)
            );
        }
    }

    // Send frame to WS2812 matrix
    led_matrix_show();
}

int main(void) {
    led_matrix_init(
        LED_ROWS,
        LED_COLS,
        LED_PIN
    );

    float animation_time = 0.4f;

    while (true) {
        render_frame(animation_time);

        // Move through the third dimension of noise
        animation_time += TIME_STEP;

        sleep_ms(FRAME_DELAY_MS);
    }

    return 0;
}

