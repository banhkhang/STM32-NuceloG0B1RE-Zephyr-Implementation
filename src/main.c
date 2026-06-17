/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/gpio.h>

#include "ThreadMng.h"

LOG_MODULE_REGISTER(main);

/* LED Configuration */

/* miscellaneous */
extern volatile uint8_t count;

int main(void)
{
	pin_config();
    sys_init();
	thread_init();

    while (1)
    {
        LOG_INF("Init: %d", count);
        k_sleep(K_FOREVER);
    }
}
