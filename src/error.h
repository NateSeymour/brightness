#ifndef DOORCTL_ERROR_H
#define DOORCTL_ERROR_H

typedef enum
{
    ERR_NONE = 0,

    ERR_ROUTINE_NOT_IMPLEMENTED,
} error_code_t;

typedef struct
{
    error_code_t code;
    char const *message;
    void *data;
} error_t;

constexpr error_t error_ok = {
    .code = ERR_NONE,
};
extern error_t error_last_fatal;

/**
 * Utility function to check if an error has been thrown.
 * @param error Error to inspect.
 * @return Pointer to error thrown or nullptr.
 */
error_t const* error_thrown(error_t *error);

/**
 * Triggers the fatal error handler (ERROR_IRQ) via interrupt after optionally setting the global error_last_fatal.
 */
void error_fatal(error_t *error);

#endif //DOORCTL_ERROR_H
