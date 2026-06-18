# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(nucleo_g0b1re)

target_sources(app PRIVATE
    src/app/main.c
    src/drivers/gpio/GPIO_driver.c
    src/drivers/uart/UART_driver.c
    src/middleware/thread_mng/ThreadMng.c
    src/services/uart/UART_service.c
    src/services/button/button_service.c
)

target_include_directories(app PRIVATE
    src/common
    src/drivers/gpio
    src/drivers/uart
    src/middleware/thread_mng
    src/services/uart
    src/services/button
)