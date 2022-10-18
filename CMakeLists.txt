cmake_minimum_required(VERSION 3.13)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

set(PRJ_NAME wearable_hid)


set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
project(PRJ_NAME C CXX ASM)


set(PICO_SDK_PATH "${CMAKE_SOURCE_DIR}/external/pico-sdk")
set(FREERTOS_KERNEL_PATH "${CMAKE_SOURCE_DIR}/external/FreeRTOS-Kernel")

include(${PICO_SDK_PATH}/external/pico_sdk_import.cmake)



# initialize the Raspberry Pi Pico SDK
pico_sdk_init()


add_compile_options(-Wall
        -Wno-format          # int != int32_t as far as the compiler is concerned because gcc has int32_t as long int
        -Wno-unused-function # we have some for the docs that aren't called
        -Wno-maybe-uninitialized
        -g3
        -O0
        )


add_subdirectory(src)
add_subdirectory(external)