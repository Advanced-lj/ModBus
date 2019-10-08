#ifndef __LED_H__
#define __LED_H__

#include "stm32l0xx_hal.h"

typedef enum {
	LED_ON,
	LED_OFF,
	LED_TOGGLE
} led_state_t;

typedef struct unxx_led_e {
	GPIO_TypeDef  *GPIOx;
	led_state_t state;
	uint16_t GPIO_Pin;	
	uint32_t times;
	uint32_t on_time;
	uint32_t off_time;
	uint32_t number;
	uint32_t count;
} unxx_led_t;

extern unxx_led_t green_led;
extern unxx_led_t red_led;

extern void unxx_led_init(void);
extern void unxx_set_led_on(unxx_led_t *led);
extern void unxx_set_led_off(unxx_led_t *led);
extern void unxx_set_led_toggle(unxx_led_t *led, uint32_t on_time, uint32_t off_time, uint32_t times);
extern void unxx_led_timer_callback(void);
#endif

