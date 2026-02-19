#ifndef DOORCTL_DRIVER_H
#define DOORCTL_DRIVER_H

#include <stdint.h>
#include <stddef.h>
#include "error.h"

/**
 * Bank of GPIOs used for interfacing with LED module.
 */
#define DRIVER_GPIO_R1 28
#define DRIVER_GPIO_R2 29
#define DRIVER_GPIO_B1 24
#define DRIVER_GPIO_B2 25
#define DRIVER_GPIO_G1 26
#define DRIVER_GPIO_G2 27

constexpr uint8_t k_driver_color_pins[] = {
    DRIVER_GPIO_R1,
    DRIVER_GPIO_R2,
    DRIVER_GPIO_B1,
    DRIVER_GPIO_B2,
    DRIVER_GPIO_G1,
    DRIVER_GPIO_G2,
};

#define DRIVER_GPIO_A 19
#define DRIVER_GPIO_B 18
#define DRIVER_GPIO_C 20

constexpr uint8_t k_driver_address_pins[] = {
    DRIVER_GPIO_A,
    DRIVER_GPIO_B,
    DRIVER_GPIO_C,
};

#define DRIVER_GPIO_CLK 11
#define DRIVER_GPIO_OE 9
#define DRIVER_GPIO_LAT 10

constexpr uint8_t k_driver_control_pins[] = {
    DRIVER_GPIO_CLK,
    DRIVER_GPIO_OE,
    DRIVER_GPIO_LAT,
};

#define DRIVER_SCREEN_WIDTH 32
#define DRIVER_SCREEN_HEIGHT 16
#define DRIVER_SCREEN_BUFFER_SIZE (DRIVER_SCREEN_WIDTH * DRIVER_SCREEN_HEIGHT)
#define DRIVER_SCREEN_BUFFER_COUNT 2

extern uint8_t driver_screen_main_buffer[DRIVER_SCREEN_BUFFER_SIZE * DRIVER_SCREEN_BUFFER_COUNT];
extern uint8_t *driver_screen_buffer[DRIVER_SCREEN_BUFFER_COUNT];
extern uint8_t driver_buffer_index;

/**
 * GIF image containing the smiley face splash image included from splash.s.
 */
extern uint8_t *driver_splash_image;

/**
 * Swap front and back buffers.
 */
void driver_swap_buffers();

/**
 * Returns the buffer currently being used for rendering.
 * @return Front buffer.
 */
uint8_t const *driver_get_front_buffer();

/**
 * Returns the buffer currently being written to.
 * @return Back buffer.
 */
uint8_t *driver_get_back_buffer();

/**
 * Set current driver address line.
 * @param address Address line.
 */
void driver_set_address(uint8_t address);

/**
 * Write GIF to back buffer and swap to front.
 * @param buffer Buffer containing GIF data.
 * @return Error.
 */
error_t driver_display_gif(uint8_t const *buffer);

/**
 * Rendering loop for the GPU routine to be run on core 1.
 */
[[noreturn]] void driver_gpu_entry();

/**
 * Initialize bank of GPIOs.
 * @param bank Array of GPIOs to initialize.
 * @param count Number of GPIOs in bank.
 * @param direction Direction.
 * @return Error.
 */
void driver_init_io_bank(uint8_t const *bank, size_t count, bool direction);

/**
 * Initializes the GPU driver and starts rendering on core 1.
 * @return Error.
 */
error_t driver_init();

#endif //DOORCTL_DRIVER_H