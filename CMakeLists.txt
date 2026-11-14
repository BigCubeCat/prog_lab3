cmake_minimum_required(VERSION 3.13)


set(EXE_NAME perlin)

include(pico_sdk_import.cmake)

project(${EXE_NAME} C CXX ASM)

set(CMAKE_C_STANDARD 11)

pico_sdk_init()

add_executable(${EXE_NAME}
    src/main.c

    src/led_matrix.c
    src/color.c
    src/perlin.c

    include/led_matrix.h
    include/color.h
    include/perlin.h
)

target_include_directories(${EXE_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/include
)

pico_generate_pio_header(${EXE_NAME}
    ${CMAKE_CURRENT_LIST_DIR}/pioasm/ws2812.pio
)

target_link_libraries(${EXE_NAME}
    pico_stdlib
    hardware_pwm
    hardware_i2c
    hardware_dma
    hardware_pio
)

pico_add_extra_outputs(${EXE_NAME})

pico_enable_stdio_usb(${EXE_NAME} 1)
pico_enable_stdio_uart(${EXE_NAME} 0)
