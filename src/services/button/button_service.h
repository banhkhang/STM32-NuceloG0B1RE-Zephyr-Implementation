#ifndef BUTTON_SERVICE_H
#define BUTTON_SERVICE_H

#include <zephyr/kernel.h>

extern struct k_sem button_sem;

void Button_ServiceInit(void);

#endif