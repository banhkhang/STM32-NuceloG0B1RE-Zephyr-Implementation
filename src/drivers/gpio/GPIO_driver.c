/* GPIO_driver.c */
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include "config.h"
#include "GPIO_driver.h"

LOG_MODULE_REGISTER(GPIO_driver);

static const struct gpio_dt_spec led0 = GPIO_DT_SPEC_GET(LED_NODE, gpios);
static const struct gpio_dt_spec sw0  = GPIO_DT_SPEC_GET(BTN_NODE, gpios);
static struct gpio_callback button_cb_data;		//create callback object as a registration record (pins, fnc when interrupted).

/* interrupt object */


void pin_config(void){

	int ret;

	/* Configure LED pin */
	if(!gpio_is_ready_dt(&led0)){
		LOG_ERR("Built-inLED not ready");
		return;
	}
	ret = gpio_pin_configure_dt(&led0, GPIO_OUTPUT_ACTIVE);
	LOG_INF("led config ret=%d", ret);

	/* Configure Switch pin */
	if(!gpio_is_ready_dt(&sw0)){
		LOG_ERR("User Switch not ready");
		return;
	}
	ret = gpio_pin_configure_dt(&sw0, GPIO_INPUT);
	LOG_INF("sw config ret=%d", ret);

	/* Interrupt configuration */
	ret = gpio_pin_interrupt_configure_dt(&sw0, GPIO_INT_EDGE_TO_ACTIVE);
	LOG_INF("sw interrupt config ret=%d", ret);
}

/* The driver only wires the ISR into the GPIO controller.
   It has no idea what the callback DOES — that's the service's job. */
void GPIO_RegisterButtonCallback(button_callback_t cb)
{
    gpio_init_callback(&button_cb_data, cb, BIT(sw0.pin));
    gpio_add_callback(sw0.port, &button_cb_data);
}

void GPIO_ToggleLED(void)
{
    gpio_pin_toggle_dt(&led0);
}
	