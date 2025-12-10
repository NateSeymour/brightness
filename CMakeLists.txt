cmake_minimum_required(VERSION 4.0)

set(PICO_SDK_FETCH_FROM_GIT ON)
include(cmake/pico.cmake)

set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")
project(door_control C)

pico_sdk_init()

set(CMAKE_C_STANDARD 23)

add_executable(door_control main.c)
