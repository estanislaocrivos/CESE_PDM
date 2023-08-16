/*
 * API_delay.c
 *
 * This library contains functions for using non-blocking delay.
 *
 * Author: Estanislao Crivos
 * Date: July 2023
 *
 */

#include "stm32f4xx_hal.h"
#include "API_delay.h"
#include <stdbool.h>

/**
  * @brief Initializes delay structure
  *
  * @param delay_t * delay, tick_t duration
  * @retval void
  */
void delayInit(delay_t * delay, tick_t duration)
{

	delay->duration = duration;
	delay->running = false;
}

/**
  * @brief Read delay status. If the time is up returns true
  *
  * @param delay_t * delay
  * @retval bool_t
  */
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

/**
  * @brief Modifies delay duration
  *
  * @param delay_t * delay, tick_t duration
  * @retval void
  */
void delayWrite(delay_t * delay, tick_t duration)
{
	delay->duration = duration;
}
