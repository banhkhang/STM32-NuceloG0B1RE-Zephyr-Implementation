/* UART_driver.c */
#include <zephyr/drivers/uart.h>
#include <zephyr/device.h>
#include <zephyr/logging/log.h>

#include "UART_driver.h"

LOG_MODULE_REGISTER(uart_driver);

static const struct device *uart_dev;
static uart_rx_byte_cb_t rx_callback;

static void uart_isr(const struct device *dev, void *user_data)
{
    uint8_t byte;

    while (uart_irq_update(dev) && uart_irq_is_pending(dev)) {
        if (uart_irq_rx_ready(dev)) {
            while (uart_fifo_read(dev, &byte, 1) == 1) {
                if (rx_callback) {
                    rx_callback(byte);
                }
            }
        }
    }
}

bool UART_DriverInit(void)
{
    uart_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_console));

    if (!device_is_ready(uart_dev)) {
        LOG_ERR("UART device not ready");
        return false;
    }

    uart_irq_callback_user_data_set(uart_dev, uart_isr, NULL);
    uart_irq_rx_enable(uart_dev);

    return true;
}

void UART_DriverSend(const uint8_t *data, size_t len)
{
    for (size_t i = 0; i < len; i++) {
        uart_poll_out(uart_dev, data[i]);
    }
}

void UART_DriverRegisterRxCallback(uart_rx_byte_cb_t cb)
{
    rx_callback = cb;
}