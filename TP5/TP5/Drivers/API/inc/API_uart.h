/*
 * API_uart.h
 *
 *  Created on: Jul 27, 2023
 *      Author: estanislaocrivosgandini
 */

#ifndef API_INC_API_UART_H_
#define API_INC_API_UART_H_

#include <stdbool.h>
#include <stdint.h>
#include "stm32f4xx.h"

bool uartInit();
void uartSendString();
void uartSendStringSize(uint8_t * pstring, uint16_t size);
void uartReceiveStringSize(uint8_t * pstring, uint16_t size);

#endif /* API_INC_API_UART_H_ */
