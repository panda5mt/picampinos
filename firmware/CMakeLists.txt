cmake_minimum_required(VERSION 3.13)
include(pico_sdk_import.cmake)
set(target_name picampinos)

project(${target_name} C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
pico_sdk_init()


add_executable(${target_name})

pico_generate_pio_header(${target_name} ${CMAKE_CURRENT_LIST_DIR}/picampinos.pio)
pico_generate_pio_header(${target_name} ${CMAKE_CURRENT_LIST_DIR}/iot_sram.pio)
pico_generate_pio_header(${target_name} ${CMAKE_CURRENT_LIST_DIR}/ezspi_slave.pio)
pico_generate_pio_header(${target_name} ${CMAKE_CURRENT_LIST_DIR}/ser_100base_fx.pio)

target_sources(${target_name} PRIVATE
        arithmetic/pico_fft.c
        main.c
        cam.c
        sccb_if.c
        iot_sram.c
        ezspi_slave.c
        udp.c
        tbl_8b10b.c
        sfp_hw.c
        )

target_link_libraries(${target_name} PRIVATE
        pico_malloc
        pico_divider 
        pico_stdlib
        hardware_vreg
        hardware_pio
        hardware_i2c
        hardware_pwm
        hardware_dma
        hardware_irq

        )


pico_enable_stdio_usb(${target_name} 1)
#pico_enable_stdio_uart(${target_name} 1)

pico_add_extra_outputs(${target_name})