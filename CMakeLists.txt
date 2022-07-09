cmake_minimum_required(VERSION 3.13)

set(PRJ_NAME wearable_hid)

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()

if (DEFINED ENV{PICO_SDK_PATH} AND (NOT PICO_SDK_PATH))
    set(PICO_SDK_PATH $ENV{PICO_SDK_PATH})
    message("Using PICO_SDK_PATH from environment ('${PICO_SDK_PATH}')")
endif ()
if (NOT PICO_SDK_PATH)
    message(FATAL_ERROR "PICO_SDK_PATH is not defined")
endif()
get_filename_component(PICO_SDK_PATH "${PICO_SDK_PATH}" REALPATH BASE_DIR "${CMAKE_BINARY_DIR}")
if (NOT EXISTS ${PICO_SDK_PATH})
    message(FATAL_ERROR "Directory '${PICO_SDK_PATH}' not found")
endif ()




include(${PICO_SDK_PATH}/external/pico_sdk_import.cmake)

set(FREERTOS_KERNEL_PATH "../src/bsp/freertos/FreeRTOS-Kernel")
# Pull in FreeRTOS
include(src/bsp/freertos/FreeRTOS_Kernel_import.cmake)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
project(PRJ_NAME C CXX ASM)


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
