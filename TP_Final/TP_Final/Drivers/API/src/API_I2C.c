/*
 * API_I2C.c
 *
 *  Created on: Aug 4, 2023
 *      Author: estanislaocrivosgandini
 */

#include "API_I2C.h"
#include <stdint.h>
#include "stm32f4xx.h"

I2C_HandleTypeDef hi2c1;

void I2C_Init(void)
{
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
  }
}

void I2C_Send_Cmd(uint8_t device_address, uint8_t * buffer, uint8_t bytes)
{
    HAL_I2C_Master_Transmit(&hi2c1, device_address, buffer, bytes, 100);
    return;
}
    	
