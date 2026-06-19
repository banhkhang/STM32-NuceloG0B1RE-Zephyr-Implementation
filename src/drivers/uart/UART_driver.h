/* UART_DRIVER_H */
#ifndef UART_DRIVER_H
#define UART_DRIVER_H

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

// A function pointer template for UART
typedef void (*uart_rx_byte_cb_t)(uint8_t byte);

bool UART_DriverInit(void);
void UART_DriverSend(const uint8_t *data, size_t len);
void UART_DriverRegisterRxCallback(uart_rx_byte_cb_t cb);

/* implement TX*/

#endif 
