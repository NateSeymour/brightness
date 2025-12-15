#include "error.h"
#include "pico/stdlib.h"
#include "device.h"

error_t error_last_fatal = error_ok;

error_t const* error_thrown(error_t* error)
{
    if (error->code != ERR_NONE)
    {
        return error;
    }

    return nullptr;
}

void error_fatal(error_t *error)
{
    error_last_fatal = *error;
    irq_set_pending(ERROR_IRQ);
}