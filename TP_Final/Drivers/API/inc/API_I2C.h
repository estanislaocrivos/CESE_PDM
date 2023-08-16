/*
 * API_I2C.h
 *
 * This library contains the necessary functions for initializing and transmitting
 * data through I2C communication protocol using the available hardware.
 *
 * This library is used by API_LCD library for communicating by I2C with the
 * peripheral through I2C.
 *
 * Author: Estanislao Crivos
 * Date: August 2023
 *
 */

#ifndef INC_API_I2C_H_
#define INC_API_I2C_H_

#include "stm32f4xx.h"
#define LCD_Module_Address 0x4E

void I2C_Init(void);
void I2C_Transmit(uint8_t *data, uint16_t size);

#endif /* INC_API_I2C_H_ */
