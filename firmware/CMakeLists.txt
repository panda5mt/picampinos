cmake_minimum_required(VERSION 3.13)
set(target_name picampinos)
set(PICO_BOARD pico2)

# using PSRAM
# use our own allocator
# the following enables the system malloc/free to be wrapped
set(SKIP_PICO_MALLOC 1)


# the following enables wrapping in sparkfun_pico builds
set(SFE_PICO_ALLOC_WRAP 1)

# the following enables the system malloc/free to be wrapped during compilation
add_definitions(-DSFE_PICO_ALLOC_WRAP)

set(PICO_SDK_FETCH_FROM_GIT on)
include(pico_sdk_import.cmake)

project(${target_name} C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
pico_sdk_init()

add_executable(${target_name})

add_subdirectory(sparkfun_pico)

target_include_directories(${target_name} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/arithmetic
    ${CMAKE_CURRENT_LIST_DIR}/sparkfun_pico
    #${CMAKE_CURRENT_LIST_DIR}/sfp
    ${CMAKE_CURRENT_LIST_DIR}/rj45
    ${CMAKE_CURRENT_LIST_DIR}/
)

# overclocking technique
pico_define_boot_stage2(slower_boot2 ${PICO_DEFAULT_BOOT_STAGE2_FILE})
target_compile_definitions(
        slower_boot2 
        PRIVATE PICO_FLASH_SPI_CLKDIV=4
        )
pico_set_boot_stage2(${target_name} slower_boot2)

pico_generate_pio_header(${target_name} ${CMAKE_CURRENT_LIST_DIR}/picampinos.pio)
#pico_generate_pio_header(${target_name} ${CMAKE_CURRENT_LIST_DIR}/sfp/ser_100base_fx.pio)
pico_generate_pio_header(${target_name} ${CMAKE_CURRENT_LIST_DIR}/rj45/ser_10base_t.pio)
pico_generate_pio_header(${target_name} ${CMAKE_CURRENT_LIST_DIR}/rj45/des_10base_t.pio)


# image_process.cが存在するか確認
if(EXISTS "${CMAKE_SOURCE_DIR}/arithmetic/image_process.c")
    # ライブラリ用のimage_process.cをコンパイルして、libimage_process.aを作成
    add_library(image_process STATIC arithmetic/image_process.c)

    # libimage_process.aをbuild/以外に配置
    set_target_properties(image_process PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/arithmetic)
else()
    message(STATUS "image_process.c was not found. We'll use static library")
endif()

set(STATIC_LIBRARIES
        ${CMAKE_CURRENT_LIST_DIR}/arithmetic/libimage_process.a
        )

target_sources(${target_name} PRIVATE
        # sfp/tbl_8b10b.c
        # sfp/udp.c
        # sfp/sfp_hw.c

        rj45/arp.c
        rj45/eth.c
        rj45/hwinit.c
        rj45/icmp.c
        rj45/udp.c
        main.c
        cam.c
        sccb_if.c
        #pico_psram.c

        arithmetic/pico_fft.c
        #arithmetic/test_code.c
        )



target_link_libraries(${target_name} PRIVATE
        #pico_malloc
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
        sparkfun_pico
        )


pico_enable_stdio_usb(${target_name} 1)
#pico_set_float_implementation(${target_name} compiler)
#pico_enable_stdio_uart(${target_name} 1)

pico_add_extra_outputs(${target_name})

