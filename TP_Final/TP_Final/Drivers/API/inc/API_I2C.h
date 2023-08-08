/*
 * API_I2C.h
 *
 *  Created on: Aug 4, 2023
 *      Author: estanislaocrivosgandini
 */

#ifndef API_INC_API_I2C_H_
#define API_INC_API_I2C_H_

#include <stdint.h>

void I2C_Init(void);

void I2C_Send_Cmd(uint8_t device_address, uint8_t * buffer, uint8_t bytes);

#endif /* API_INC_API_I2C_H_ */
