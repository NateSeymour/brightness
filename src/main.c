#include <stdio.h>
#include "pico/stdlib.h"
#include "driver.h"
#include "device.h"

/*
 * Components
 * - Driver
 * - Debug management
 * - Crypto
 * - State management
 */

/**
 * Manages the
 */
[[noreturn]] void error_state_handler()
{
    while (true)
    {}
}

int main(void)
{
    // Initialize error IRQs
    irq_set_exclusive_handler(ERROR_IRQ, error_state_handler);
    irq_set_priority(ERROR_IRQ, 0);
}