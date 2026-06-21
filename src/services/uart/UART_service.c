/* UART_service.c - UART service implementation */
#include <zephyr/kernel.h>
#include <zephyr/sys/ring_buffer.h>
#include <zephyr/logging/log.h>
#include <string.h>

#include "UART_service.h"
#include "UART_driver.h"
#include "config.h"

LOG_MODULE_REGISTER(uart_service);

/* Ring buffer: ISR writes in, service thread drains.
   RING_BUF is Zephyr's built-in lock-free-ish ring buffer primitive,
   safe for single-producer (ISR) / single-consumer (thread) use. */
RING_BUF_DECLARE(rx_ringbuf, UART_RX_RINGBUF_SIZE);

K_THREAD_STACK_DEFINE(uart_rx_thread_stack, UART_RX_THREAD_STACK_SIZE);
static struct k_thread uart_rx_thread_data;

K_SEM_DEFINE(uart_rx_sem, 0, 1);

static void uart_rx_byte_handler(uint8_t byte)
{
    /* Called from ISR context - keep this trivial */
    ring_buf_put(&rx_ringbuf, &byte, 1);
    k_sem_give(&uart_rx_sem);
}

static void uart_rx_thread(void *a1, void *a2, void *a3)
{
    uint8_t byte;

    while (1) {
        k_sem_take(&uart_rx_sem, K_FOREVER);
        while (ring_buf_get(&rx_ringbuf, &byte, 1) == 1) {
            /* Echo behavior for now - this is the seam where
               command parsing / framing logic will go later */
            UART_DriverSend(&byte, 1);
            if(byte == '\r'){
                static const uint8_t newline = '\n';
                UART_DriverSend(&newline, 1);
            }
        }
    }
}

void UART_ServiceInit(void)
{
    if (!UART_DriverInit()) {
        LOG_ERR("UART service init failed - driver not ready");
        return;
    }

    //  uart_rx_byte_handler (service) now equals rx_callback (driver)
    UART_DriverRegisterRxCallback(uart_rx_byte_handler);

    k_thread_create(&uart_rx_thread_data, uart_rx_thread_stack,
                     K_THREAD_STACK_SIZEOF(uart_rx_thread_stack),
                     uart_rx_thread, NULL, NULL, NULL,
                     UART_RX_THREAD_PRIORITY, 0, K_NO_WAIT);
}

// Public APIs
void UART_ServiceSend(char *msg)
{
    UART_DriverSend((const uint8_t *)msg, strlen(msg));
}