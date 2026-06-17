/* ThreadMng.c*/

#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/gpio.h>

#include "ThreadMng.h"

LOG_MODULE_REGISTER(thread_mng);

/* pin configuration */
#define LED_NODE DT_ALIAS(led0)
#define BTN_NODE DT_ALIAS(sw0)

static const struct gpio_dt_spec led0 = 
	GPIO_DT_SPEC_GET(LED_NODE, gpios);

static const struct gpio_dt_spec sw0 = 
	GPIO_DT_SPEC_GET(BTN_NODE, gpios);

static int conf_return;


/* prototype functions */
void thread_a(void *, void *, void *);
void thread_b(void *, void *, void *);

volatile uint8_t count = 0;

/* Semaphore for button press event */
K_SEM_DEFINE(button_sem, 0, 1);

/* interrupt isr */
void button_pressed_isr(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
    k_sem_give(&button_sem);
}

/* Thread def*/
	/* Thread A */
K_THREAD_STACK_DEFINE(thread_stack_a, 1024);
struct k_thread thread_data_a;
void thread_a(void *arg1, void *arg2, void *arg3){
	while (1)
	{
		LOG_INF("Thread A is running");
		k_sleep(K_SECONDS(2));
	}
}

	/* Thread B */
K_THREAD_STACK_DEFINE(thread_stack_b, 1024);
struct k_thread thread_data_b;
void thread_b(void *arg1, void *arg2, void *arg3){
	while (1)
	{
		k_sem_take(&button_sem, K_FOREVER);
		count++;
		LOG_INF("Button detected, count = %d", count);
		gpio_pin_toggle_dt(&led0);
	}
}

void thread_init(void){
    k_thread_create(&thread_data_a, 
		thread_stack_a, 
		K_THREAD_STACK_SIZEOF(thread_stack_a),
		thread_a, 
		NULL, NULL, NULL, K_PRIO_PREEMPT(1), 0, K_NO_WAIT);

	k_thread_create(&thread_data_b, 
		thread_stack_b, 
		K_THREAD_STACK_SIZEOF(thread_stack_b),
		thread_b, 
		NULL, NULL, NULL, K_PRIO_PREEMPT(1), 0, K_NO_WAIT);
}

/* interrupt object */
static struct gpio_callback button_cb_data;		//create callback object as a registration record (pins, fnc when interrupted).

void pin_config(void){
	/* Pin configuration code can be added here if needed */
	if(!gpio_is_ready_dt(&led0)){
		LOG_ERR("LED device not ready");
		return;
	}

	conf_return = gpio_pin_configure_dt(&led0, GPIO_OUTPUT_ACTIVE);
	LOG_INF("led config ret=%d", conf_return);

	if(!gpio_is_ready_dt(&sw0)){
		LOG_ERR("Switch device not ready");
		return;
	}
	
	conf_return = gpio_pin_configure_dt(&sw0, GPIO_INPUT);
	LOG_INF("sw config ret=%d", conf_return);

	/* Interrupt configuration code can be added here if needed */
	conf_return = gpio_pin_interrupt_configure_dt(&sw0, GPIO_INT_EDGE_TO_ACTIVE);
	LOG_INF("sw interrupt config ret=%d", conf_return);
}

void sys_init(void){
	/* System initialization code can be added here if needed */
	/* Interrupt handler */
	gpio_init_callback(
    &button_cb_data,		//button_cb_data.callback = button_pressed_isr
    button_pressed_isr,
    BIT(sw0.pin));			//button_cb_data.pin_mask = BIT(sw0.pin)

	gpio_add_callback(		//When GPIOA/GPIOB/GPIOC interrupt happens, look inside button_cb_data.
    sw0.port,
    &button_cb_data);
}



