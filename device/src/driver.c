#include "driver.h"
#include <stdlib.h>
#include "device.h"

driver_state_t g_driver_state = {
    .step_index = 0,
    .hold_time_ms = 1,

    .angle = 0,
    .target_angle = 0,
};

error_t driver_set_power_state(driver_power_state_t state)
{
    for (uint8_t i = 0; i < k_driver_pin_count; i++)
    {
        gpio_put(k_driver_pins[i], state & (k_driver_steps[g_driver_state.step_index] >> i));
    }

    return error_ok;
}

error_t driver_step(driver_direction_t direction)
{
    if (abs(direction) != 1)
    {
        error_t error = {
            .code = ERROR_DRIVER_BAD_STEP,
            .message = "Step direction must be equal to -1 or +1",
        };
        return error;
    }

    driver_set_power_state(ON);

    g_driver_state.step_index = (g_driver_state.step_index + 1 * direction) % k_driver_step_count;

    return error_ok;
}

error_t driver_reset_position()
{
    for (int i = 0; i < k_driver_steps_per_rotation; i++)
    {
        driver_step(LEFT);
        sleep_ms(g_driver_state.hold_time_ms);

        bool motor_at_stop = gpio_get(k_driver_stop_pin);

        if (motor_at_stop)
        {
            driver_set_power_state(OFF);
            g_driver_state.angle = 0;
            return error_ok;
        }
    }

    driver_set_power_state(OFF);
    error_t error = {
        .code = ERROR_DRIVER_BAD_ANGLE_SENSOR,
        .message = "Motor made full rotation without sensing stop.",
    };
    return error;
}

error_t driver_set_angle_deg(float angle)
{
    uint16_t angle_normalized = (float)k_driver_steps_per_rotation / 360.f * angle;

    if (angle_normalized <= k_driver_steps_per_rotation)
    {
        g_driver_state.target_angle = angle_normalized;
    } else
    {
        error_t error = {
            .code = ERROR_DRIVER_INVALID_ANGLE,
            .message = "Driver angle must be between 0deg and 360deg.",
        };
        return error;
    }

    if (g_driver_state.angle != g_driver_state.target_angle)
    {
        irq_set_pending(IRQ_DRIVER_ROTATE);
    }

    return error_ok;
}

float driver_get_angle_deg()
{
    return (float)g_driver_state.angle / (float)k_driver_steps_per_rotation * 360.f;
}

void driver_rotate_handler()
{
    driver_set_power_state(ON);

    int step = g_driver_state.angle - g_driver_state.target_angle;
    int step_count = abs(step);
    int direction = step / step_count;

    for (int i = 0; i < step_count; i++)
    {
        driver_step(direction);
        busy_wait_ms(g_driver_state.hold_time_ms);
    }

    driver_set_power_state(OFF);

    g_driver_state.angle = g_driver_state.target_angle;

    irq_clear(IRQ_DRIVER_ROTATE);
}

error_t driver_init()
{
    irq_set_exclusive_handler(IRQ_DRIVER_ROTATE, driver_rotate_handler);
    irq_set_priority(IRQ_DRIVER_ROTATE, 255);
    irq_set_enabled(IRQ_DRIVER_ROTATE, true);

    for (uint8_t i = 0; i < k_driver_pin_count; i++)
    {
        gpio_init(k_driver_pins[i]);
        gpio_set_dir(k_driver_pins[i], true);
        gpio_set_irq_enabled(k_driver_pins[i], 0b1111, false);
    }

    gpio_init(k_driver_stop_pin);
    gpio_set_dir(k_driver_stop_pin, false);

    /*
    error_t error = driver_reset_position();
    if (error_thrown(&error))
    {
        return error;
    }
    */

    g_device_state.driver_initialized = true;

    return error_ok;
}
