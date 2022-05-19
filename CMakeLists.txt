cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(picocam C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
pico_sdk_init()

add_executable(picocam
    picocam.c
    cam.c
    sccb_if.c
)

pico_generate_pio_header(picocam ${CMAKE_CURRENT_LIST_DIR}/picocam.pio)
pico_generate_pio_header(picocam ${CMAKE_CURRENT_LIST_DIR}/iot_sram.pio)

pico_enable_stdio_usb(picocam 1)
#pico_enable_stdio_uart(picocam 1)

pico_add_extra_outputs(picocam)

target_link_libraries(picocam PRIVATE
        pico_malloc 
        pico_stdlib
        hardware_pio
        hardware_i2c
        hardware_pwm
        hardware_dma
        hardware_irq
        )

