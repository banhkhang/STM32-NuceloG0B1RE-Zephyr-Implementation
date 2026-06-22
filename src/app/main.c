/*main.c*/

#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/gpio.h>

#include "ThreadMng.h"
#include "GPIO_driver.h"

LOG_MODULE_REGISTER(main);

/* LED Configuration */

int main(void)
{
	pin_config();
    sys_init();
	thread_init();

    while (1)
    {
        k_sleep(K_FOREVER);
    }
}
