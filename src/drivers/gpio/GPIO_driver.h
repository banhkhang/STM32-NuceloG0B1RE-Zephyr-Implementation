/* GPIO_driver.h */
#ifndef GPIO_DRIVER_H
#define GPIO_DRIVER_H

#include <zephyr/drivers/gpio.h>

// Function pointer - template for interrupt function
typedef void (*button_callback_t)(const struct device *dev,
    struct gpio_callback *cb, uint32_t pins);
//"button_callback_t is a name for a pointer to the isr function
//that returns void and takes those three arguments:

void pin_config(void);
void GPIO_RegisterButtonCallback(button_callback_t cb);
void GPIO_ToggleLED(void);

#endif