cmake_minimum_required(VERSION 3.13)
set(LIBRARY_NAME image_process)

if(EXISTS "${CMAKE_SOURCE_DIR}/arithmetic/image_process.c")
    

    add_library(image_process STATIC 
        image_process.c 
        fft4f2d.c)

    # インクルードパスを指定
    target_include_directories(
        image_process PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/                        
        ${CMAKE_CURRENT_LIST_DIR}/../
        ${CMAKE_CURRENT_LIST_DIR}/../sparkfun_pico/
    )

    target_link_libraries(image_process PUBLIC
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
        pico_async_context_freertos
        FreeRTOS-Kernel
        FreeRTOS-Kernel-Heap4
        )

    # ライブラリの出力ディレクトリを現在のディレクトリに設定
    set_target_properties(image_process PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    )
endif()
