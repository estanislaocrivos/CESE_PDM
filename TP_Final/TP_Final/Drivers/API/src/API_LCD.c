#include "API_LCD.h"
#include <stdint.h>
#include "API_I2C.h"
#include "stm32f4xx.h"

#define LCD_Module_Address 0x4E // Dir. brindada por el fabricante

extern I2C_HandleTypeDef hi2c1;

/*

Funcion para inicializar el display
Rutina de inicializacion brindada por el fabricante

*/
void LCD_Init(void)
{
	HAL_Delay(50);
	LCD_Send_Cmd(0x30);
	HAL_Delay(5);
	LCD_Send_Cmd(0x30);
	HAL_Delay(1);
	LCD_Send_Cmd(0x30);
	HAL_Delay(10);
	LCD_Send_Cmd(0x20);
	HAL_Delay(10);
	
	LCD_Send_Cmd(0x28);
	HAL_Delay(1);
	LCD_Send_Cmd(0x08);
	HAL_Delay(1);
	LCD_Send_Cmd(0x01);
	HAL_Delay(1);
	HAL_Delay(1);
	LCD_Send_Cmd(0x06);
	HAL_Delay(1);
	LCD_Send_Cmd(0x0C);
	
	return;
}

void LCD_Send_Cmd(char command)
{
	// Funcion para enviarle un comando al display (ver comandos en hoja de datos)
	
	// Separo el byte en parte alta y parte baja:
	uint8_t DATO_HIGH =  (command & 0x0F);
	uint8_t DATO_LOW = ((command << 4) & 0x0F);
	
	// Array para almacenar el byte en 2 envios de 4 bits (y 2 envios para senales RS EN)
	uint8_t Data[4]; 

	Data[0] = DATO_HIGH | 0X0C; // EN = 1 RS = 0
	Data[1] = DATO_HIGH | 0X08;	// EN = 0 RS = 0
	Data[2] = DATO_LOW  | 0X0C; // EN = 1 RS = 0
	Data[3] = DATO_LOW  | 0X08; // EN = 0 RS = 0

//	I2C_Send_Cmd(LCD_Module_Address,(uint8_t *) Data, 4);
	HAL_I2C_Master_Transmit (&hi2c1, LCD_Module_Address,(uint8_t *) Data, 4, 100);
	
	return;
}

void LCD_Send_Char(char character)
{
	// Funcion para enviarle un CHAR al display (ver comandos en hoja de datos)
	
	// Separo el byte en parte alta y parte baja:
	uint8_t DATO_HIGH =  (character & 0x0F);
	uint8_t DATO_LOW = ((character << 4) & 0x0F);
	
	// Array para almacenar el byte en 2 envios de 4 bits (y 2 envios para senales RS EN)
	uint8_t Data[4]; 

	Data[0] = DATO_HIGH | 0X0D; // EN = 1 RS = 0
	Data[1] = DATO_HIGH | 0X09;	// EN = 0 RS = 0
	Data[2] = DATO_LOW  | 0X0D; // EN = 1 RS = 0
	Data[3] = DATO_LOW  | 0X09; // EN = 0 RS = 0

	// I2C_Send_Cmd(LCD_Module_Address,(uint8_t *) Data, 4);
	HAL_I2C_Master_Transmit (&hi2c1, LCD_Module_Address,(uint8_t *) Data, 4, 100);

	
	return;
}

// void LCD_Send_String(char *string)
// {
// 	// Funcion para escribir una cadena de caracteres en el display
	
// 	uint8_t i = 0;
// 	while (string[i] != '\0')
// 	{
// 		LCD_Send_Char(*string);
// 		string += 1;
// 	}
// 	return;
// }

void LCD_Send_String(char *str)
{
	while (*str) LCD_Send_Char (*str++);
}

void LCD_Place_Cursor(uint8_t x, uint8_t y)
{
	// Funcion para ubicar el cursor en el display

	uint8_t inicio[] = {0x80|0x00, 0x80|0xC0};
	I2C_Send_Cmd(LCD_Module_Address, inicio[y-1]+x-1, 1);
}
