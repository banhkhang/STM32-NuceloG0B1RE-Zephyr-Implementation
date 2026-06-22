/* UART_driver.c */
/* 1. Includes and Configuration Definitions */
#include <zephyr/drivers/uart.h>
#include <zephyr/device.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/ring_buffer.h>

#include "UART_driver.h"
#include "config.h"

LOG_MODULE_REGISTER(uart_driver);

/* 2. Private (Static) Driver Variables */
static const struct device *uart_dev;
static uart_rx_byte_cb_t rx_callback;   // pointer to the service layer
//static uart_tx_byte_cb_t tx_callback; 

RING_BUF_DECLARE(tx_ringbuf, UART_TX_RINGBUF_SIZE);

/* 3. The Interrupt Service Routine (ISR) */
static void uart_rx_int_handler(const struct device *dev){
    uint8_t byte;
    while (uart_fifo_read(dev, &byte, 1) == 1) {
        // Read byte out of hardware FIFO and trigger callback
        if (rx_callback) {
            rx_callback(byte);
        }
    }
}

static void uart_tx_int_handler(const struct device *dev){
    uint8_t byte;
    if (ring_buf_get(&tx_ringbuf, &byte, 1) == 1) {
        uart_fifo_fill(dev, &byte, 1);
    }
    else
        uart_irq_tx_disable(dev);
}

//When theres byte on RX pin, byte copied FIFO reg, isr do following
static void uart_isr(const struct device *dev, void *user_data)
{
    // Refresh int stat and Check if theres any unhandled interrupt 
    while (uart_irq_update(dev) && uart_irq_is_pending(dev)) {
        // Check if physical it is RX data and is ready
        if (uart_irq_rx_ready(dev)) {
                 uart_rx_int_handler(dev);
            }
        if (uart_irq_tx_ready(dev)){
            //if there is still data in tx_ringbuf
            uart_tx_int_handler(dev);
        }
    }
}


/* 4. Public API Implementation Functions */
bool UART_DriverInit(void)
{
    uart_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_console));

    if (!device_is_ready(uart_dev)) {
        LOG_ERR("UART device not ready");
        return false;
    }

    //register uart_isr as real ISR
    uart_irq_callback_user_data_set(uart_dev, uart_isr, NULL);
    //enable RX interrupt 
    uart_irq_rx_enable(uart_dev);
    //NOT enable TX interrupt, as it will fires isr cuz FIFO is always ready
    return true;
}

void UART_DriverSend(const uint8_t *data, size_t len)
{
    for (size_t i = 0; i < len; i++) {
        uart_poll_out(uart_dev, data[i]);
    }
}

void UART_DriverSend_IT(const uint8_t *data, size_t len)
{
   
    for (size_t i = 0; i < len; i++) {
            ring_buf_put(&tx_ringbuf, &data[i], 1);
        } 
    uart_irq_tx_enable(uart_dev);
}

void UART_DriverRegisterRxCallback(uart_rx_byte_cb_t cb)
{
    rx_callback = cb;
}
