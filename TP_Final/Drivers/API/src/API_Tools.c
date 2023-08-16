/*
 * API_Tools.c
 *
 * This library contains functions for handling indicating LEDs.
 *
 * Author: Estanislao Crivos
 * Date: August 2023
 *
 */

#include "stdint.h"
#include "stm32f4xx.h"
#include "API_Tools.h"

void Green_LED_OFF(void)
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, 0);
}

void Red_LED_ON(void)
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, 1);
}

void Green_LED_Toggle(void)
{
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_8);
}

void Red_LED_Toggle(void)
{
	HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_10);
}

void Buzzer_Toggle(void)
{
	HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_4);
}

