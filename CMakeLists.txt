cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(picampinos C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
pico_sdk_init()

add_executable( picampinos
    main.c
    cam.c
    sccb_if.c
    iot_sram.c
    ezspi_slave.c
    udp.c
    tbl_8b10b.c
)

pico_generate_pio_header(picampinos ${CMAKE_CURRENT_LIST_DIR}/picampinos.pio)
pico_generate_pio_header(picampinos ${CMAKE_CURRENT_LIST_DIR}/iot_sram.pio)
pico_generate_pio_header(picampinos ${CMAKE_CURRENT_LIST_DIR}/ezspi_slave.pio)
pico_generate_pio_header(picampinos ${CMAKE_CURRENT_LIST_DIR}/ser_100base_fx.pio)

pico_enable_stdio_usb(picampinos 1)
#pico_enable_stdio_uart(picampinos 1)

pico_add_extra_outputs(picampinos)

target_link_libraries(picampinos PRIVATE
        pico_malloc 
        pico_stdlib
        hardware_vreg
        hardware_pio
        hardware_i2c
        hardware_pwm
        hardware_dma
        hardware_irq
        )

