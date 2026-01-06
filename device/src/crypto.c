#include "crypto.h"

#include <stdlib.h>
#include <string.h>
#include "mbedtls/ecdsa.h"
#include "mbedtls/base64.h"
#include "mbedtls/platform.h"
#include "mbedtls/sha256.h"
#include "device.h"
#include "generated/pka.h"

static mbedtls_ecdsa_context ctx;
static uint8_t pka_decoded[DOORCTL_PKA_LENGTH_BYTES];
static mbedtls_ecp_point q;
static mbedtls_ecp_keypair pkey;
static constexpr mbedtls_ecp_group_id group_id = MBEDTLS_ECP_DP_SECP256R1;
static mbedtls_ecp_group group;

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

    size_t timestamp_ms = strtol(timestamp, nullptr, 10);
    if (timestamp_ms <= g_device_state.last_command_authorization)
    {
        error_t error = {
            .code = ERROR_AUTH_TOKEN_EXPIRED,
            .message = "Supplied authorization token is expired.",
        };
        return error;
    }

    // We will look back into original buffer for the message hash.
    // All we consider for the hash is the command@timestamp portion.
    // Add one character for the '@' symbol.
    size_t message_length = strlen(command) + strlen(timestamp) + 1;
    char hash[32];
    if (mbedtls_sha256(buffer, message_length, hash, 0) != 0)
    {
        error_t error = {
            .code = ERROR_CRYPTO_HASH,
            .message = "Failed to calculate the SHA2"
        };
        return error;
    }

    if (mbedtls_ecdsa_read_signature(&ctx, hash, sizeof(hash), signature, strlen(signature)) != 0)
    {
        error_t error = {
            .code = ERROR_AUTH_TOKEN_INVALID_SIGNATURE,
            .message = "Supplied signature is invalid.",
        };
        return error;
    }

    g_device_state.last_command_authorization = timestamp_ms;

    return error_ok;
}

error_t crypto_init()
{
    mbedtls_ecdsa_init(&ctx);

    mbedtls_ecp_group_init(&group);
    if (mbedtls_ecp_group_load(&group, group_id) != 0)
    {
        error_t error = {
            .code = ERROR_CRYPTO_INIT,
            .message = "Failed to initialize ECP group.",
        };
        return error;
    }

    size_t bytes_written = 0;
    memset(pka_decoded, 0, sizeof(pka_decoded));
    if (mbedtls_base64_decode(pka_decoded, sizeof(pka_decoded), &bytes_written, k_pka_signing_public, strlen(k_pka_signing_public)) != 0)
    {
        error_t error = {
            .code = ERROR_CRYPTO_INIT,
            .message = "Failed to decode PKA public key.",
        };
        return error;
    }

    mbedtls_ecp_point_init(&q);
    if (mbedtls_ecp_point_read_binary(&group, &q, pka_decoded, sizeof(pka_decoded)) != 0)
    {
        error_t error = {
            .code = ERROR_CRYPTO_INIT,
            .message = "Failed to read PKA public key.",
        };
        return error;
    }

    mbedtls_ecp_keypair_init(&pkey);

    if (mbedtls_ecp_set_public_key(group_id, &pkey, &q) != 0)
    {
        error_t error = {
            .code = ERROR_CRYPTO_INIT,
            .message = "Failed to set PKA public key in context.",
        };
        return error;
    }

    return error_ok;
}