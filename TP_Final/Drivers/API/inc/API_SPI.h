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

#ifndef API_INC_API_SPI_H_
#define API_INC_API_SPI_H_

#include "stm32f4xx.h"

void SPI_Init(void);
uint8_t SPI_Transcieve(uint8_t data);

#endif /* API_INC_API_SPI_H_ */
