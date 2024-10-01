cmake_minimum_required(VERSION 3.13)
set(target_name picampinos)
set(PICO_SDK_FETCH_FROM_GIT on)
set(PICO_BOARD pico2)

include(pico_sdk_import.cmake)

project(${target_name} C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
pico_sdk_init()


add_executable(${target_name})

# overclocking technique
pico_define_boot_stage2(slower_boot2 ${PICO_DEFAULT_BOOT_STAGE2_FILE})
target_compile_definitions(
        slower_boot2 
        PRIVATE PICO_FLASH_SPI_CLKDIV=4
        )
pico_set_boot_stage2(${target_name} slower_boot2)

pico_generate_pio_header(${target_name} ${CMAKE_CURRENT_LIST_DIR}/picampinos.pio)
pico_generate_pio_header(${target_name} ${CMAKE_CURRENT_LIST_DIR}/iot_sram.pio)
# pico_generate_pio_header(${target_name} ${CMAKE_CURRENT_LIST_DIR}/ezspi_slave.pio)
pico_generate_pio_header(${target_name} ${CMAKE_CURRENT_LIST_DIR}/ser_100base_fx.pio)


# ライブラリ用のimage_process.cをコンパイルして、libimage_process.aを作成
add_library(image_process STATIC arithmetic/image_process.c)
# libimage_process.aをbuild/以外に配置
set_target_properties(image_process PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/)

target_sources(${target_name} PRIVATE
        tbl_8b10b.c
        udp.c
        sfp_hw.c
        main.c
        cam.c
        sccb_if.c
        iot_sram.c
        pico_psram.c
        # ezspi_slave.c
        # arithmetic/pico_dct8.c
        # arithmetic/pico_leq.c

        arithmetic/pico_fft.c
        arithmetic/test_code.c
        #arithmetic/image_process.c       
        )

target_link_libraries(${target_name} PRIVATE
        pico_malloc
        pico_divider 
        pico_stdlib
        pico_multicore
        pico_flash
        hardware_vreg
        hardware_pio
        hardware_spi
        hardware_i2c
        hardware_pwm
        hardware_dma
        hardware_irq
        image_process
        )


pico_enable_stdio_usb(${target_name} 1)
#pico_set_float_implementation(${target_name} compiler)
#pico_enable_stdio_uart(${target_name} 1)

pico_add_extra_outputs(${target_name})

