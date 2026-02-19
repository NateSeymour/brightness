#ifndef DOORCTL_DRIVER_H
#define DOORCTL_DRIVER_H

#include "error.h"

/**
 * Initializes the motor driver by completing one forward-reverse step run to ensure motor is aligned with step.
 * @return Error.
 */
error_t driver_init();

#endif //DOORCTL_DRIVER_H