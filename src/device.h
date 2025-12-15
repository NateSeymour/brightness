#ifndef DOORCTL_DEVICE_H
#define DOORCTL_DEVICE_H

#include <stdint.h>
#include "pico/stdlib.h"

typedef struct
{
} device_state_t;

typedef enum
{
    ERROR_IRQ = SPARE_IRQ_0,
} device_user_irq_t;

extern device_state_t g_device_state;

#endif //DOORCTL_DEVICE_H