/*
 * API_SPI.c
 *
 * This library contains the necessary functions for initializing and transmitting
 * data through SPI communication protocol using the available hardware.
 *
 * This library is used by RC522 library for communicating with the
 * peripheral through SPI.
 *
 * Author: Estanislao Crivos
 * Date: August 2023
 *
 */

#include "API_SPI.h"
#include "stm32f4xx.h"
#include "main.h"

/* SPI handler */
static SPI_HandleTypeDef hspi1;

/**
  * @brief This function initializes SPI communication module.
  *
  * Calls Error_Handler in case of error.
  *
  * @param void
  * @retval void
  */
void SPI_Init(void)
{
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief This function transmits data through SPI.
  *
  * This function takes the data and its size as parameters and sends it through SPI.
  *
  * Calls Error_Handler in case of error.
  *
  * @param uint8_t data Data to be transmitted.
  * @retval uint8_t rx_data Returns received data.
  */
uint8_t SPI_Transcieve(uint8_t data)
{
	uint8_t rx_data;
	if(HAL_SPI_TransmitReceive(&hspi1, &data, &rx_data, 1, 100) != HAL_OK)
	{
		Error_Handler();
	}
	return rx_data;
}
