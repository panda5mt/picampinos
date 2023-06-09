cmake_minimum_required(VERSION 3.13)
include(pico_sdk_import.cmake)
set(target_name picampinos)

project(${target_name} C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
pico_sdk_init()


add_executable(${target_name})

# overclocking technique
pico_define_boot_stage2(pico-coremark-div8 ${PICO_DEFAULT_BOOT_STAGE2_FILE})
target_compile_definitions(pico-coremark-div8 PRIVATE PICO_FLASH_SPI_CLKDIV=8)
pico_set_boot_stage2(${target_name} pico-coremark-div8)

pico_generate_pio_header(${target_name} ${CMAKE_CURRENT_LIST_DIR}/picampinos.pio)
pico_generate_pio_header(${target_name} ${CMAKE_CURRENT_LIST_DIR}/iot_sram.pio)
# pico_generate_pio_header(${target_name} ${CMAKE_CURRENT_LIST_DIR}/ezspi_slave.pio)
pico_generate_pio_header(${target_name} ${CMAKE_CURRENT_LIST_DIR}/ser_100base_fx.pio)

target_sources(${target_name} PRIVATE
        tbl_8b10b.c
        udp.c
        sfp_hw.c
        main.c
        cam.c
        sccb_if.c
        iot_sram.c
        # arithmetic/pico_dct8.c
        # arithmetic/pico_fft.c
        # arithmetic/pico_leq.c
        # arithmetic/test_code.c       
        # ezspi_slave.c
        )

target_link_libraries(${target_name} PRIVATE
        pico_malloc
        pico_divider 
        pico_stdlib
        pico_multicore
        hardware_vreg
        hardware_pio
        hardware_i2c
        hardware_pwm
        hardware_dma
        hardware_irq

        )


pico_enable_stdio_usb(${target_name} 1)
#pico_set_float_implementation(${target_name} compiler)
#pico_enable_stdio_uart(${target_name} 1)

pico_add_extra_outputs(${target_name})

