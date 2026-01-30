#ifndef DOORCTL_DRIVER_H
#define DOORCTL_DRIVER_H

#include <stdint.h>
#include "error.h"

typedef struct
{
    uint8_t step_index;
    uint8_t hold_time_ms;

    uint16_t angle;
    uint16_t target_angle;
} driver_state_t;

extern driver_state_t g_driver_state;

typedef enum : int8_t
{
    LEFT = -1,
    RIGHT = 1,
} driver_direction_t;

typedef enum : uint8_t
{
    OFF = 0,
    ON = 1,
} driver_power_state_t;

constexpr uint8_t k_driver_pins[] = { 2, 3, 4, 5 };
constexpr uint8_t k_driver_pin_count = 4;

constexpr uint8_t k_driver_stop_pin = 6;

/**
 * Driver follows an 8 step motor driving pattern.
 * The pattern controls four outputs (BLUE - 1, PINK - 2, YELLOW - 3, ORANGE - 4).
 * Outputs are encoded into a bitfield on the lower nibble of a byte.
 */
constexpr uint8_t k_driver_steps[] = {
    0b0001,
    0b0011,
    0b0010,
    0b0110,
    0b0100,
    0b1100,
    0b1000,
    0b1001,
};
constexpr uint8_t k_driver_step_count = 8;

constexpr uint32_t k_driver_steps_per_rotation = 4096;

/**
 * Set current power state either by turning on all nodes in current step or turning all off.
 * @param state ON or OFF.
 * @return Error.
 */
error_t driver_set_power_state(driver_power_state_t state);

/**
 * Step the motor in the desired direction. Updates g_driver_state global struct.
 * @param direction LEFT or RIGHT.
 * @return Error.
 */
error_t driver_step(driver_direction_t direction);

/**
 * Turn the step motor to home position (locked).
 * @return Error.
 */
error_t driver_reset_position();

/**
 * Sets the new target angle in degrees of the driver and triggers IRQ.
 * @param angle New angle to point towards.
 * @return Error.
 */
error_t driver_set_angle_deg(float angle);

/**
 * Interrupt handler for driver movement.
 */
void driver_rotate_handler();

/**
 * Initializes the motor driver by completing one forward-reverse step run to ensure motor is aligned with step.
 * @return Error.
 */
error_t driver_init();

#endif //DOORCTL_DRIVER_H