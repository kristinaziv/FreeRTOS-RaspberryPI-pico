add_executable(${ProjectName}
    main.c
)

target_include_directories(${ProjectName} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(
    ${ProjectName} 
    pico_stdlib
    pico_stdlib_headers
    hardware_adc
    FreeRTOS-Kernel-Heap4 
    )
pico_enable_stdio_usb(${ProjectName} 1)
pico_enable_stdio_uart(${ProjectName} 0)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    LCD_RS=8
    LCD_EN=9
    LCD_D4=10
    LCD_D5=11
    LCD_D6=12
    LCD_D7=13
)

pico_add_extra_outputs(${ProjectName})