/*
 * API_debounce.c
 *
 * API para realizar debounce por software de un pulsador utilizando una maquina de estados finitos
 *
 *  Created on: Jul 24, 2023
 *      Author: estanislaocrivosgandini
 */

#include "API_debounce.h"
#include "API_delay.h"
#include "stm32f4xx_hal.h"

debounceState_t BUTTON_STATE;
delay_t delay;
tick_t Delay_Duration = DELAY_DUR;
static bool_t flanco_descendente;

void debounceFSM_init() // Funcion que carga el estado inicial de la maq.
{
	BUTTON_STATE = BUTTON_UP;
}

void debounceFSM_update() // Funcion que comprueba el estado del boton y actualiza el estado
{
	switch(BUTTON_STATE)
	{
	case BUTTON_UP:
		if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_SET)
		{
			BUTTON_STATE = BUTTON_FALLING;
		}
		break;

	case BUTTON_FALLING:
		delayInit(&delay, Delay_Duration);
		while (delayRead(&delay) != true)
		{
		}
		if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_SET)
		{
			BUTTON_STATE = BUTTON_DOWN;
			flanco_descendente = true;
		}
		else
		{
			BUTTON_STATE = BUTTON_UP;
		}
		break;

	case BUTTON_DOWN:
		if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_RESET)
		{
			BUTTON_STATE = BUTTON_RISING;
		}
		break;

	case BUTTON_RISING:
		delayInit(&delay, Delay_Duration);
		while (delayRead(&delay) != true)
		{
		}
		if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_RESET)
		{
			BUTTON_STATE = BUTTON_UP;
		}
		else
		{
			BUTTON_STATE = BUTTON_DOWN;
		}
		break;
	}
}

bool readKey() // Funcion que lee el estado del boton y devuelve true o false si esta presionado o no
{
	if(BUTTON_STATE == BUTTON_DOWN)
		return true;
	else if(BUTTON_STATE == BUTTON_UP)
		return false;
	return false;
}

