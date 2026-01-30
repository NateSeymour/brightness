//
// Created by Nathan Seymour on 1/2/2026.
//

#include "ble.h"
#include <stdint.h>
#include "btstack.h"
#include "crypto.h"
#include "pico/btstack_cyw43.h"
#include "doorctl.h"
#include "pico/cyw43_arch.h"
#include "device.h"
#include "driver.h"
#include "mathutil.h"

#define ATT_CHARACTERISTIC_AUTHORIZATION_TOKEN ATT_CHARACTERISTIC_5a605272_fee8_43f6_bde6_8588bd0b857f_01_VALUE_HANDLE

static btstack_packet_callback_registration_t hci_event_handler;
static hci_con_handle_t hci_con_handle;

constexpr uint8_t advertisement_data[] = {
    0x02, BLUETOOTH_DATA_TYPE_FLAGS, 0x06,
    0x0b, BLUETOOTH_DATA_TYPE_COMPLETE_LOCAL_NAME, 'd', 'o', 'o', 'r', 'c', 't', 'l',
};
constexpr uint8_t adv_data_len = sizeof(advertisement_data);

static void packet_handler(uint8_t packet_type, uint16_t channel, uint8_t *packet, uint16_t size)
{
    if (packet_type != HCI_EVENT_PACKET) return;

    switch (hci_event_packet_get_type(packet))
    {
    case HCI_EVENT_DISCONNECTION_COMPLETE:
    case ATT_EVENT_CAN_SEND_NOW:
    case ATT_EVENT_DISCONNECTED:
        default:
        {
            break;
        }
    }
}

static uint16_t att_read_callback(hci_con_handle_t con_handle, uint16_t att_handle, uint16_t offset, uint8_t * buffer, uint16_t buffer_size) {}

static int att_write_callback(hci_con_handle_t con_handle, uint16_t att_handle, uint16_t transaction_mode, uint16_t offset, uint8_t *buffer, uint16_t buffer_size)
{
    error_t error = error_ok;

    switch (att_handle)
    {
        case ATT_CHARACTERISTIC_AUTHORIZATION_TOKEN:
        {
            char authorization_token[DOORCTL_AUTHORIZATION_TOKEN_LENGTH] = {0};
            memcpy(authorization_token, buffer, min(sizeof(authorization_token), buffer_size));

            error = error_ok; // crypto_verify_authorization_token(authorization_token, sizeof(authorization_token));
            if (!error_thrown(&error))
            {
                irq_set_pending(IRQ_DRIVER_ROTATE);
            }
            break;
        }

        default:
        {
            break;
        }
    }
}

error_t ble_init()
{
    int res_cyw43_arch_init = cyw43_arch_init_with_country(CYW43_COUNTRY_USA);
    if(res_cyw43_arch_init != 0)
    {
        error_t error = {
            .code = ERROR_CHIPSET_CONTROL,
            .message = "Failed to init the cyw43 chipset.",
        };
        return error;
    }

    btstack_memory_init();
    l2cap_init();
    sm_init();
    att_server_init(profile_data, att_read_callback, att_write_callback);

    // Setup advertisements
    uint16_t advertising_interval = 0x0030;

    gap_advertisements_set_params(advertising_interval, advertising_interval, 0, 0, nullptr, 0x07, 0x00);
    gap_advertisements_set_data(adv_data_len, (uint8_t*) advertisement_data);
    gap_advertisements_enable(1);

    // Callbacks
    hci_event_handler.callback = packet_handler;
    hci_add_event_handler(&hci_event_handler);
    att_server_register_packet_handler(packet_handler);

    // Enable chipset power
    if(hci_power_control(HCI_POWER_ON) != 0)
    {
        error_t error = {
            .code = ERROR_CHIPSET_CONTROL,
            .message = "Failed to switch power on to cyw43 chipset.",
        };
        return error;
    }

    g_device_state.ble_initialized = true;

    return error_ok;
}