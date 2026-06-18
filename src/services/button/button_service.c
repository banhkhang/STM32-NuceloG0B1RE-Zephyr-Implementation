#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include "button_service.h"
#include "GPIO_driver.h"

LOG_MODULE_REGISTER(button_service);

K_SEM_DEFINE(button_sem, 0, 1);

static void button_pressed_isr(const struct device *dev,
                                  struct gpio_callback *cb,
                                  uint32_t pins)
{
    k_sem_give(&button_sem);
}

void Button_ServiceInit(void)
{
    GPIO_RegisterButtonCallback(button_pressed_isr);

    /* Can later add debouncing logic, long press detection ...*/
}