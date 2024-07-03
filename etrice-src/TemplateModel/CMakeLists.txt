file(GLOB_RECURSE RUNTIME_SOURCE "${PROJECT_SOURCE_DIR}/etrice/org.eclipse.etrice.runtime.c/src/platforms/MT_32Bit_FreeRTOS_Generic/*.c")
file(GLOB_RECURSE C_RUNTIME_SOURCE "${PROJECT_SOURCE_DIR}/etrice/org.eclipse.etrice.runtime.c/src/common/**/*.c") 
file(GLOB_RECURSE MODELLIB_SOURCE "${PROJECT_SOURCE_DIR}/etrice/org.eclipse.etrice.modellib.c/**/*.c")
file(GLOB_RECURSE SOURCE "${PROJECT_SOURCE_DIR}/etrice-src/**/*.c" )



add_executable(${ProjectName}
   ${RUNTIME_SOURCE}
   ${C_RUNTIME_SOURCE}
   ${MODELLIB_SOURCE}
   ${SOURCE}
)
target_compile_options(${ProjectName} PRIVATE
    -I${PROJECT_SOURCE_DIR}/etrice/org.eclipse.etrice.runtime.c/src/common
    -I${PROJECT_SOURCE_DIR}/etrice/org.eclipse.etrice.runtime.c/src/config
    -I${PROJECT_SOURCE_DIR}/etrice/org.eclipse.etrice.runtime.c/src/util
    -I${PROJECT_SOURCE_DIR}/etrice/org.eclipse.etrice.runtime.c/src/platforms/MT_32Bit_FreeRTOS_Generic
    -I${PROJECT_SOURCE_DIR}/etrice/org.eclipse.etrice.modellib.c/src-gen
    -I${PROJECT_SOURCE_DIR}/etrice/org.eclipse.etrice.modellib.c
    -I${PROJECT_SOURCE_DIR}/etrice-src
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
    hardware_i2c
    )
pico_enable_stdio_usb(${ProjectName} 1)
pico_enable_stdio_uart(${ProjectName} 0)



pico_add_extra_outputs(${ProjectName})