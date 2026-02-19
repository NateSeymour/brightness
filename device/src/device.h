#ifndef DOORCTL_DEVICE_H
#define DOORCTL_DEVICE_H

#include "pico/stdlib.h"

#define DOORCTL_MAX_CONCURRENT_CONNECTIONS 3
#define DOORCTL_AUTHORIZATION_TOKEN_LENGTH 256

typedef struct
{
    bool ble_initialized;
    bool driver_initialized;
} device_state_t;

typedef enum
{
    IRQ_ERROR_FATAL = SPARE_IRQ_0,
    IRQ_DRIVER_ROTATE = SPARE_IRQ_1,
} device_user_irq_t;

extern device_state_t g_device_state;

#endif //DOORCTL_DEVICE_H