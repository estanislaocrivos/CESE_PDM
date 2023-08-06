#include "API_LCD.h"
#include <stdint.h>
#include "API_I2C.h"
#include "stm32f4xx.h"

#define 0x20<<1 LCD_Address

void LCD_Init(void)
{
	// Funcion para inicializar el display	
	// Rutina de inicializacion brindada por el fabricante
	
	HAL_Delay(20);
	LCD_Send_Cmd(0x30);
	HAL_Delay(5);
	LCD_Send_Cmd(0x30);
	HAL_Delay(10);
	LCD_Send_Cmd(0x32);
	HAL_Delay(5);
	
	LCD_Send_Cmd(0x28);
	HAL_Delay(5);
	
	LCD_Send_Cmd(0x0C);
	HAL_Delay(5);
	LCD_Send_Cmd(0x01);
	HAL_Delay(5);
	LCD_Send_Cmd(0x02);
	HAL_Delay(5);
	LCD_Send_Cmd(0x06);
	HAL_Delay(5);
	LCD_Send_Cmd(0x80);
	
	HAL_Delay(100);
	
	return;
}

void LCD_Send_Cmd(uint8_t command)
{
	// Funcion para enviarle un comando al display (ver comandos en hoja de datos)
	
	// Separo el byte en parte alta y parte baja:
	uint8_t DATO_LOW =  command & 0x0F;
	uint8_t DATO_HIGH = (command>>4) & 0x0F;
	
	// Array para almacenar el byte en 2 envios de 4 bits (y 2 envios para senales RS EN)
	uint8_t Data[4]; 

	Data[0] = DATO_HIGH | 0X0C; // EN = 1 RS = 0
	Data[1] = DATO_HIGH | 0X08;	// EN = 0 RS = 0
	Data[2] = DATO_LOW  | 0X0C; // EN = 1 RS = 0
	Data[3] = DATO_LOW  | 0X08; // EN = 0 RS = 0

	I2C_Send_Cmd(LCD_Address, Data, 4);
	
	return;
}

void LCD_Send_Char(uint8_t character)
{
	// Funcion para enviarle un CHAR al display (ver comandos en hoja de datos)
	
	// Separo el byte en parte alta y parte baja:
	uint8_t DATO_LOW =  character & 0x0F;
	uint8_t DATO_HIGH = (character>>4) & 0x0F;
	
	// Array para almacenar el byte en 2 envios de 4 bits (y 2 envios para senales RS EN)
	uint8_t Data[4]; 

	Data[0] = DATO_HIGH | 0X0D; // EN = 1 RS = 0
	Data[1] = DATO_HIGH | 0X09;	// EN = 0 RS = 0
	Data[2] = DATO_LOW  | 0X0D; // EN = 1 RS = 0
	Data[3] = DATO_LOW  | 0X09; // EN = 0 RS = 0

	I2C_Send_Cmd(LCD_Address, Data, 4);
	
	return;
}

void LCD_Send_String(uint8_t *string)
{
	// Funcion para escribir una cadena de caracteres en el display
	
	uint8_t i = 0;
	while (string[i] != 0x00)
	{
		LCD_Send_Char(string[i++]);
	}
	return;
}

void LCD_Place_Cursor(uint8_t x, uint8_t y)
{
	// Funcion para ubicar el cursor en el display

	uint8_t inicio[] = {0x80|0x00, 0x80|0xC0};
	I2C_Send_Cmd(LCD_Address, inicio[y-1]+x-1, 1);
}
