cmake_minimum_required(VERSION 4.0)

set(PICO_SDK_FETCH_FROM_GIT ON)
include(cmake/pico.cmake)

project(doorctl C CXX ASM)

pico_sdk_init()

set(CMAKE_C_STANDARD 23)

add_executable(doorctl
        src/main.c
        src/driver.c
        src/driver.h
        src/device.c
        src/device.h
        src/error.h
        src/error.c
)
target_link_libraries(doorctl pico_stdlib)

pico_add_extra_outputs(doorctl)