#pragma once

#include <zephyr/kernel.h>

extern struct k_sem button_sem;

void Button_ServiceInit(void);
