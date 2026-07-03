cmake_minimum_required(VERSION 3.15)
project(BlackPill)

set(CMAKE_VERBOSE_MAKEFILE ON)

message(STATUS "CMAKE_TOOLCHAIN_FILE: ${CMAKE_TOOLCHAIN_FILE}")
message(STATUS "Sysroot:" ${CMAKE_SYSROOT})
message(STATUS "C compiler: ${CMAKE_C_COMPILER}")
message(STATUS "CXX compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "ASM compiler: ${CMAKE_ASM_COMPILER}")
message(STATUS "Linker: ${CMAKE_LINKER}")

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

enable_language(ASM)

set(CMSIS_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/drivers/CMSIS/Include")
set(CMSIS_DEVICE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/drivers/CMSIS/Device/ST/STM32F4xx/Include")

set(HAL_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/drivers/STM32F4xx_HAL_Driver/Inc")
set(HAL_SOURCE_DIR "${CMAKE_SOURCE_DIR}/drivers/STM32F4xx_HAL_Driver/Src")

set(APP_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")

set(HAL_SOURCES
    "${HAL_SOURCE_DIR}/stm32f4xx_hal.c"
    "${HAL_SOURCE_DIR}/stm32f4xx_hal_cortex.c"
    "${HAL_SOURCE_DIR}/stm32f4xx_hal_rcc.c"
    "${HAL_SOURCE_DIR}/stm32f4xx_hal_gpio.c"
    "${HAL_SOURCE_DIR}/stm32f4xx_hal_usart.c"
)

set(SOURCES
    src/startup_stm32f411xe.s
    ${CMAKE_CURRENT_SOURCE_DIR}/drivers/CMSIS/Device/ST/STM32F4xx/Source/Templates/system_stm32f4xx.c
    src/syscalls.c
    src/handlers.c
    src/board_init.c
    src/gpio.c
    src/uart.c
    src/main.cpp    
)

add_executable(BlackPill ${SOURCES} ${HAL_SOURCES})

target_compile_definitions(BlackPill PRIVATE
    STM32F411xE
    USE_HAL_DRIVER
)

target_include_directories(BlackPill PRIVATE
    ${CMSIS_INCLUDE_DIR}
    ${CMSIS_DEVICE_DIR}
    ${HAL_INCLUDE_DIR}
    ${APP_INCLUDE_DIR}
)

set_target_properties(BlackPill PROPERTIES
    SUFFIX ".elf"
)

target_compile_options(BlackPill PRIVATE
    -mcpu=cortex-m4
    -mthumb
    -mfloat-abi=hard
    -mfpu=fpv4-sp-d16       
    -g
    -O0
    -ffunction-sections
    -fdata-sections
)

set(LINKER_SCRIPT "${CMAKE_SOURCE_DIR}/linker/STM32F411CEUX_FLASH.ld")
if(NOT EXISTS "${LINKER_SCRIPT}")
    message(FATAL_ERROR "Linker script not found: ${LINKER_SCRIPT}")
endif()

message(STATUS "Linker script: ${LINKER_SCRIPT}")

target_link_options(BlackPill PRIVATE
    -T${LINKER_SCRIPT}
    -mcpu=cortex-m4
    -mthumb
    -mfloat-abi=hard
    -static
    -lg_nano
    -lnosys
    -Wl,--gc-sections    
    -v
)

add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${PROJECT_NAME}> ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.bin
    BYPRODUCTS ${CMAKE_BINARY_DIR}/BlackPill.bin
    COMMENT "Generating ${PROJECT_NAME}.bin from ELF"
)
