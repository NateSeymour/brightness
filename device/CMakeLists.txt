cmake_minimum_required(VERSION 4.0)

set(PICO_SDK_FETCH_FROM_GIT ON)
set(PICO_BOARD pico_w)
include(cmake/pico.cmake)

project(doorctl C CXX ASM)

pico_sdk_init()

set(CMAKE_C_STANDARD 23)

configure_file(src/pka.in.h generated/pka.h)

add_executable(doorctl
        src/main.c
        src/driver.c
        src/driver.h
        src/device.c
        src/device.h
        src/error.h
        src/error.c
        src/crypto.c
        src/crypto.h
        src/pka.in.h
        src/btstack_config.h
        src/mbedtls_config.h
        src/ble.c
        src/ble.h
        src/lwipopts.h
        src/mathutil.h
)
target_link_libraries(doorctl
        pico_stdlib
        pico_double_pico
        pico_float_pico
        pico_async_context_poll
        pico_btstack_ble
        pico_btstack_cyw43
        pico_cyw43_arch_none
        pico_mbedtls
)
target_include_directories(doorctl PUBLIC src ${CMAKE_BINARY_DIR})
pico_btstack_make_gatt_header(doorctl PUBLIC ${CMAKE_SOURCE_DIR}/src/doorctl.gatt)

pico_add_extra_outputs(doorctl)