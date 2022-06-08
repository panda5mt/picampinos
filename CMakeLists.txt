cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(picapinos C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
pico_sdk_init()

add_executable(picapinos
    main.c
    cam.c
    sccb_if.c
    iot_sram.c
    ezspi_slave.c
)

pico_generate_pio_header(picapinos ${CMAKE_CURRENT_LIST_DIR}/picapinos.pio)
pico_generate_pio_header(picapinos ${CMAKE_CURRENT_LIST_DIR}/iot_sram.pio)
pico_generate_pio_header(picapinos ${CMAKE_CURRENT_LIST_DIR}/ezspi_slave.pio)

pico_enable_stdio_usb(picapinos 1)
#pico_enable_stdio_uart(picapinos 1)

pico_add_extra_outputs(picapinos)

target_link_libraries(picapinos PRIVATE
        pico_malloc 
        pico_stdlib
        hardware_pio
        hardware_i2c
        hardware_pwm
        hardware_dma
        hardware_irq
        )

