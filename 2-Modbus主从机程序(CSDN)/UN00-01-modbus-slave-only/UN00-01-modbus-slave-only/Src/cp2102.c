#include "cp2102.h"
#include "gpio.h"
#include "main.h"

void cp2102_enable(void)
{
	HAL_GPIO_WritePin(CP2102_RST_GPIO_Port, CP2102_RST_Pin, GPIO_PIN_SET);
}

void cp2102_disable(void)
{
	HAL_GPIO_WritePin(CP2102_RST_GPIO_Port, CP2102_RST_Pin, GPIO_PIN_RESET);
}

void cp2102_reset(void)
{
	cp2102_disable();
	HAL_Delay(1000);
	cp2102_enable();
	HAL_Delay(1000);
}
