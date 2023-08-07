cmake_minimum_required(VERSION 3.12)
cmake_policy(SET CMP0079 NEW)


project(arm2d_rp2040 C CXX ASM)

set(PICO_EXAMPLES_PATH ${PROJECT_SOURCE_DIR})
 

add_library(arm2d_rp2040 
        st7789_lcd.c
        arm_2d_disp_adapter_0.c
        rp2040_hw_acceleration.c
        )

pico_generate_pio_header(arm2d_rp2040 ${CMAKE_CURRENT_LIST_DIR}/st7789_lcd.pio)

target_include_directories(arm2d_rp2040 PUBLIC ${CMAKE_CURRENT_LIST_DIR})

add_subdirectory(${ARM2D} arm2d_bin)

target_include_directories(ARM2D PUBLIC ${CMAKE_CURRENT_LIST_DIR})

add_subdirectory(${CMSISDSP}/Source bin_dsp)
target_compile_options(CMSISDSP PRIVATE -Ofast -ffunction-sections -fdata-sections)
target_link_libraries(ARM2D PUBLIC CMSISDSP)
target_link_libraries(CMSISDSP PUBLIC cmsis_core)

target_link_libraries(arm2d_rp2040 
	ARM2D
    pico_stdlib 
    hardware_pio
    hardware_interp
    )

