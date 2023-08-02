/*
 * API_uart.c
 *
 *  Created on: Jul 27, 2023
 *      Author: estanislaocrivosgandini
 */

#include "API_uart.h"
#include "stm32f4xx_hal.h"

UART_HandleTypeDef UartHandle;

/*
 * Funcion que inicializa la UART
 */

bool uartInit(void)
{
  UartHandle.Instance = USART2;
  UartHandle.Init.BaudRate = 115200;
  UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
  UartHandle.Init.StopBits = UART_STOPBITS_1;
  UartHandle.Init.Parity = UART_PARITY_NONE;
  UartHandle.Init.Mode = UART_MODE_TX_RX;
  UartHandle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  UartHandle.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&UartHandle) != HAL_OK)
  {
    return false;
  }
  return true;
}

/*
 * Funcion para transmitir una cadena de char por la UART. Pasar el nombre de la cadena como parametro
 */

void uartSendString(uint8_t * pstring)
{
    int i = 0;

    while (*(pstring + i) != '\0')
    {
        HAL_UART_Transmit(&UartHandle, &pstring[i], 1, 0xFFFF);
        i++;
    }
}


/*
 * Funcion para transmitir una cadena de char por la UART. Pasar el nombre de la cadena como parametro y el nro. de char como segundo parametro
 */

void uartSendStringSize(uint8_t * pstring, uint16_t size)
{
    int i = 0;

    while (*(pstring + i) != '\0')
    {
        HAL_UART_Transmit(&UartHandle, &pstring[i], size, 0xFFFF);
        i++;
    }}

/*
 * Funcion para recibir una cadena de char por la UART. Pasar el nombre de la cadena vacia como parametro y el tamano en char esperado
 */

void uartReceiveStringSize(uint8_t * pstring, uint16_t size)
{
	HAL_UART_Receive(&UartHandle, pstring, size, 0xFFFF);
}
