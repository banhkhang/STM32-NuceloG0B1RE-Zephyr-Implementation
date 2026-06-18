/* GPIO_driver.h */
#ifndef GPIO_DRIVER_H
#define GPIO_DRIVER_H

#include <zephyr/drivers/gpio.h>

typedef void (*button_callback_t)(const struct device *dev,
    struct gpio_callback *cb, uint32_t pins);

void pin_config(void);
void GPIO_RegisterButtonCallback(button_callback_t cb);
void GPIO_ToggleLED(void);

#endif