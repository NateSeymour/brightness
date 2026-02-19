#include "crypto.h"

#include <stdlib.h>
#include <string.h>
#include "psa/crypto.h"
#include "mbedtls/pk.h"
#include "device.h"
#include "generated/pka.h"

static mbedtls_pk_context ctx;

error_t crypto_verify_authorization_token(char const* buffer, size_t size)
{
    // Add one extra character to the auth token to ensure a null terminator.
    char authorization_token[DOORCTL_AUTHORIZATION_TOKEN_LENGTH + 1] = {0};
    memcpy(authorization_token, buffer, size);

    char const *command = strtok(authorization_token, "@");
    char const *timestamp = strtok(nullptr, "@");
    char const *signature = strtok(nullptr, "@");

    if (strcmp(command, "authorized") != 0)
    {
        error_t error = {
            .code = ERROR_AUTH_TOKEN_INVALID_COMMAND,
            .message = "Invalid command in authorization token. Expected 'authorized'.",
        };
        return error;
    }

    /*
    size_t timestamp_ms = strtol(timestamp, nullptr, 10);
    if (timestamp_ms <= g_device_state.last_command_authorization)
    {
        error_t error = {
            .code = ERROR_AUTH_TOKEN_EXPIRED,
            .message = "Supplied authorization token is expired.",
        };
        return error;
    }

    g_device_state.last_command_authorization = timestamp_ms;
    */

    return error_ok;
}

error_t crypto_init()
{
    if (psa_crypto_init() != PSA_SUCCESS)
    {
        error_t error = {
            .code = ERROR_CRYPTO_INIT,
            .message = "Failed to initialize PSA crypto.",
        };
        return error;
    }

    mbedtls_pk_init(&ctx);

    if (mbedtls_pk_parse_public_key(&ctx, DOORCTL_PKA_SIGNING_PUBLIC, strlen(DOORCTL_PKA_SIGNING_PUBLIC) + 1) != 0)
    {
        error_t error = {
            .code = ERROR_CRYPTO_INIT,
            .message = "Failed to parse public key.",
        };
        return error;
    }

    return error_ok;
}