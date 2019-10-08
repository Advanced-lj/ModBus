#include "led.h"
#include "string.h"

unxx_led_t green_led;
unxx_led_t red_led;

static unxx_led_t green_led_save;
static unxx_led_t red_led_save;

static void led_state_store(unxx_led_t *led)
{
	if(led->GPIO_Pin == red_led.GPIO_Pin) {
		memcpy(&red_led_save  , led, sizeof(unxx_led_t));
	} else {
		memcpy(&green_led_save, led, sizeof(unxx_led_t));
	}	
}

static void led_state_restore(unxx_led_t *led)
{
	if(led->GPIO_Pin == red_led.GPIO_Pin) {
		memcpy(led, &red_led_save  , sizeof(unxx_led_t));
	} else {
		memcpy(led, &green_led_save, sizeof(unxx_led_t));
	}	
}

void unxx_led_init(void)
{
	green_led.GPIOx = LED1_GPIO_Port;
	green_led.GPIO_Pin = LED1_Pin;
	
	red_led.GPIOx = LED0_GPIO_Port;
	red_led.GPIO_Pin = LED0_Pin;
	
	HAL_GPIO_WritePin(green_led.GPIOx, green_led.GPIO_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(red_led.GPIOx, red_led.GPIO_Pin, GPIO_PIN_SET);
}

void unxx_set_led_on(unxx_led_t *led)
{
	led->state = LED_ON;
	
	HAL_GPIO_WritePin(led->GPIOx, led->GPIO_Pin, GPIO_PIN_RESET);
}

void unxx_set_led_off(unxx_led_t *led)
{
	led->state = LED_OFF;
	
	HAL_GPIO_WritePin(led->GPIOx, led->GPIO_Pin, GPIO_PIN_SET);
}

void unxx_set_led_toggle(unxx_led_t *led, uint32_t on_time, uint32_t off_time, uint32_t times)
{
	if(times) {
		if(led->times == 0) {
			led_state_store(led);
		} else {
			return;
		}
	}
		
	led->state    = LED_TOGGLE;
	led->times    = times;
	led->on_time	= on_time;
	led->off_time = off_time;
	led->count    = 0;
	led->number   = 0;
}

void unxx_led_timer_callback(void)
{
	if(green_led.state == LED_TOGGLE) {		
		if(green_led.count == 0) {
			HAL_GPIO_WritePin(green_led.GPIOx, green_led.GPIO_Pin, GPIO_PIN_RESET);
		} else if(green_led.count == green_led.on_time) {
			HAL_GPIO_WritePin(green_led.GPIOx, green_led.GPIO_Pin, GPIO_PIN_SET);
		} 
		
		if(green_led.count >= green_led.on_time + green_led.off_time) {
			green_led.count = 0;
			green_led.number++;
			if(green_led.times && green_led.number > green_led.times) {
				green_led.number = 0;
				led_state_restore(&green_led);
			}
		} else {
			green_led.count++;
		}
	}
	
	if(red_led.state == LED_TOGGLE) {
		if(red_led.count == 0) {
			HAL_GPIO_WritePin(red_led.GPIOx, red_led.GPIO_Pin, GPIO_PIN_RESET);
		} else if(red_led.count == red_led.on_time) {
			HAL_GPIO_WritePin(red_led.GPIOx, red_led.GPIO_Pin, GPIO_PIN_SET);
		} 
		
		if(red_led.count >= red_led.on_time + red_led.off_time) {
			red_led.count = 0;
			red_led.number++;
			if(red_led.times && red_led.number > red_led.times) {
				red_led.number = 0;
				led_state_restore(&red_led);
			}
		} else {
			red_led.count++;
		}
	}
}
