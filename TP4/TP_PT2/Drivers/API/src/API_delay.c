/*
 * API_delay.c
 *
 *  Created on: Jul 6, 2023
 *      Author: estanislaocrivosgandini
 */

#include "stm32f4xx_hal.h"
#include "API_delay.h"
#include <stdbool.h>

// Funcion para inicializar el contador de duracion "duration"
void delayInit( delay_t * delay, tick_t duration )
{

	delay->duration = duration;
	delay->running = false;
}


// Funcion para leer el estado del contador. Si llego al objetivo, finalizarlo y devolver true. Si no esta corriendo, iniciarlo en true.
bool_t delayRead( delay_t * delay )
{

	if(delay->running == false)
	{
		delay->running = true;
		delay->startTime = HAL_GetTick();
	}
	else
	{
		tick_t actual_time = HAL_GetTick();
		if (actual_time - delay->startTime >= delay->duration)
		{
			delay->running = false;
			return true;
		}
	}
	return false;
}


// Funcion para escribir una nueva duracion del contador.
void delayWrite( delay_t * delay, tick_t duration )
{
	delay->duration = duration;
}
