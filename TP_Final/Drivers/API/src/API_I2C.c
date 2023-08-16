/*
 * API_I2C.c
 *
 * This library contains the necessary functions for initializing and transmitting
 * data through I2C communication protocol using the available hardware.
 *
 * This library is used by API_LCD library for communicating with the
 * peripheral through I2C.
 *
 * Author: Estanislao Crivos
 * Date: August 2023
 *
 */

#include "API_I2C.h"
#include "main.h"

/* I2C handler */
static I2C_HandleTypeDef hi2c1;

/**
  * @brief This function initializes I2C communication module.
  *
  * Calls Error_Handler in case of error.
  *
  * @param void
  * @retval uint8_t
  */
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
    Error_Handler();
  }
}

/**
  * @brief This function transmits data through I2C.
  *
  * This function takes the data and its size as parameters and sends it through I2C.
  * Calls Error_Handler in case of error.
  *
  * @param uint8_t *data, uint16_t size
  * @retval void
  */
void I2C_Transmit(uint8_t *data, uint16_t size)
{
    if (HAL_I2C_Master_Transmit(&hi2c1, LCD_Module_Address, data, size, 100) != HAL_OK)
    {
        Error_Handler();
    }
}
