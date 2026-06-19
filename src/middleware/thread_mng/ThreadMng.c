/* ThreadMng.c*/
#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include "ThreadMng.h"
#include "UART_service.h"
#include "button_service.h"
#include "GPIO_driver.h"
#include "config.h"

LOG_MODULE_REGISTER(thread_mng);

static uint8_t count = 0;

/* Thread def*/
	
K_THREAD_STACK_DEFINE(thread_stack_a, THREAD_A_STACK_SIZE);
struct k_thread thread_data_a;

K_THREAD_STACK_DEFINE(thread_stack_b, THREAD_B_STACK_SIZE);
struct k_thread thread_data_b;

/* Thread A */
void thread_a(void *arg1, void *arg2, void *arg3){
	while (1)
	{
		UART_ServiceSend("Thread A is running");
		UART_ServiceSend("Hello\n");
		k_sleep(K_SECONDS(2));
	}
}

/* Thread B */
void thread_b(void *arg1, void *arg2, void *arg3){
	while (1)
	{
		k_sem_take(&button_sem, K_FOREVER);
		count++;
		LOG_INF("Button detected, count = %d", count);
		GPIO_ToggleLED();
	}
}

void thread_init(void){
    k_thread_create(&thread_data_a, thread_stack_a, K_THREAD_STACK_SIZEOF(thread_stack_a),
		thread_a, NULL, NULL, NULL, K_PRIO_PREEMPT(1), 0, K_NO_WAIT);

	k_thread_create(&thread_data_b, thread_stack_b, K_THREAD_STACK_SIZEOF(thread_stack_b),
		thread_b, NULL, NULL, NULL, K_PRIO_PREEMPT(1), 0, K_NO_WAIT);
}

void sys_init(void){
	LOG_INF("System init, count = %d", count);
	Button_ServiceInit();
	UART_ServiceInit();
}

