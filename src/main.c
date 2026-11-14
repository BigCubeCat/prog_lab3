#include <stdint.h>
#include <stdio.h>

#include "led_matrix.h"

#define ENC_S1          6
#define ENC_S2          7
#define ENC_KEY         8

#define PWM_TOP         100
#define BRIGHTNESS_STEP 10

#define OLED_POWER      12

#define BRIGHTNESS_Y    20
#define TEXT_X          10
#define TEXT_Y          30


int main(void) {
    led_matrix_init();
    led_matrix_fill(0, 128, 255);
    led_matrix_show();


    while (true) {
        led_matrix_fill(0, 128, 255);
        led_matrix_show();

        sleep_ms(100);
    }
}
