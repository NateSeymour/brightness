#ifndef DOORCTL_ERROR_H
#define DOORCTL_ERROR_H

typedef enum
{
    ERROR_OK = 0,

    ERROR_AUTH_TOKEN_INVALID_COMMAND,
    ERROR_AUTH_TOKEN_EXPIRED,
    ERROR_AUTH_TOKEN_INVALID_SIGNATURE,

    ERROR_CRYPTO_INIT,
    ERROR_CRYPTO_HASH,

    ERROR_DRIVER_CGIF_INIT,
    ERROR_DRIVER_CGIF_RENDER,

    ERROR_CHIPSET_CONTROL,

    ERROR_ROUTINE_NOT_IMPLEMENTED,
} error_code_t;

typedef struct
{
    error_code_t code;
    char const *message;
    void *data;
} error_t;

extern error_t const error_ok;
extern error_t error_last_fatal;

/**
 * Utility function to check if an error has been thrown.
 * @param error Error to inspect.
 * @return Pointer to error thrown or nullptr.
 */
error_t const* error_thrown(error_t const *error);

/**
 * Triggers the fatal error handler (ERROR_IRQ) via interrupt after optionally setting the global error_last_fatal.
 */
void error_fatal(error_t const *error);

#endif //DOORCTL_ERROR_H
