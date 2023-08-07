cmake_minimum_required(VERSION 3.12)
cmake_policy(SET CMP0079 NEW)

# cmake -DARM2D=/ArmSoftware/Arm-2D 
# -DCMSISCORE=/ArmSoftware/CMSIS_5/CMSIS/Core 
# -DCMSISDSP=/ArmSoftware/Embedded/CMSIS-DSP 
# -DARM2D_RP2040=/ArmSoftware/Arm2D_RP2040/RP2040..

# Pull in SDK (must be before project)
include(pico_sdk_import.cmake)

include(pico_extras_import_optional.cmake)

project(pico_examples C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

if (PICO_SDK_VERSION_STRING VERSION_LESS "1.3.0")
    message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.3.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

set(PICO_EXAMPLES_PATH ${PROJECT_SOURCE_DIR})

add_compile_options(-Wall)


# Initialize the SDK
pico_sdk_init()


add_executable(arm2d_demo main.c
        arm_2d_scene_0.c 
        arm_2d_scene_3.c 
        arm_2d_scene_watch.c
        arm_2d_scene_1.c 
        arm_2d_scene_4.c 
        arm_2d_scene_2.c 
        arm_2d_scene_meter.c
        MeterPanel.c
        )

option(ARM2D_CONTROLS "" ON)
option(ARM2D_HELPER "" ON)
option(ARM2D_LCD_PRINTF "" ON)
option(ARM2D_BENCHMARK_GENERIC "" OFF)
option(ARM2D_BENCHMARK_WATCHPANEL "" OFF)
option(ARM2D_ALPHA_BLENDING "" ON)
option(ARM2D_TRANSFORM "" ON)
add_subdirectory(${ARM2D_RP2040} arm2d_rp2040_bin)

target_include_directories(arm2d_demo PUBLIC .)

# pull in common dependencies and adc hardware support
target_link_libraries(arm2d_demo 
    pico_stdlib 
    pico_multicore
    hardware_pio
    hardware_interp
    arm2d_rp2040
    )


# enable usb output, disable uart output
    pico_enable_stdio_usb(arm2d_demo 1)
    pico_enable_stdio_uart(arm2d_demo 0)
    
# create map/bin/hex file etc.
pico_add_extra_outputs(arm2d_demo)

# add url via pico_set_program_url

