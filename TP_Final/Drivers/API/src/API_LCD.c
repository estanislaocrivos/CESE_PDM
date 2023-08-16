/*
 * API_LCD.c
 *
 * This library contains functions for using and handling an 16X2 LCD display
 * using I2C communication.
 *
 * Author: Estanislao Crivos
 * Date: August 2023
 *
 */

#include "API_LCD.h"
#include "API_I2C.h"
#include "stdint.h"
#include "stm32f4xx.h"

/* I2C address defined by the LCD manufacturer */
#define LCD_Module_Address 0x4E

/* Import I2C handler */
//extern I2C_HandleTypeDef hi2c1;

/**
  * @brief Initialization routine for LCD display.
  *
  * This function executes the initialization routine for the LCD display sending the various
  * commands it requires according to its datasheet.
  *
  * @param void
  * @retval void
  */
void LCD_Init(void)
{
	HAL_Delay(50);
	LCD_Send_Cmd(0x30);
	HAL_Delay(10);
	LCD_Send_Cmd(0x30);
	HAL_Delay(1);
	LCD_Send_Cmd(0x30);
	HAL_Delay(1);
	LCD_Send_Cmd(0x20);
	HAL_Delay(50);
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
	HAL_Delay(50);
	
	return;
}

/**
  * @brief This function sends 8-bit commands to the LCD display.
  *
  * This function uses I2C communication to send an 8-bit command to the display,
  * splitting the 8-bit command in two commands, 4-bit each.
  *
  * @param command
  * @retval void
  */
void LCD_Send_Cmd(uint8_t command)
{
	// Funcion para enviarle un comando al display (ver comandos en hoja de datos)
	
	// Separo el byte en parte alta y parte baja:
	uint8_t DATO_HIGH =  (command & 0xF0);
	uint8_t DATO_LOW = (command<<4 & 0xF0);
	
	// Array para almacenar el byte en 2 envios de 4 bits (y 2 envios para senales RS EN)
	uint8_t Data[4]; 

	Data[0] = DATO_HIGH | 0X0C; // EN = 1 RS = 0
	Data[1] = DATO_HIGH | 0X08;	// EN = 0 RS = 0
	Data[2] = DATO_LOW  | 0X0C; // EN = 1 RS = 0
	Data[3] = DATO_LOW  | 0X08; // EN = 0 RS = 0

	I2C_Transmit(Data, 4);
	HAL_Delay(5);
}

/**
  * @brief This function sends a character to the LCD display.
  *
  * This function sends a character to the display using I2C comm. protocol,
  * splitting the 8-bit character in two 4-bit commands.
  *
  * @param uint8_t character
  * @retval void
  */
void LCD_Send_Char(uint8_t character)
{
	uint8_t DATO_HIGH =  (character & 0xF0);
	uint8_t DATO_LOW = (character<<4 & 0xF0);
	
	// Array para almacenar el byte en 2 envios de 4 bits (y 2 envios para senales RS EN)
	uint8_t Data[4]; 

	Data[0] = DATO_HIGH | 0X0D; // EN = 1 RS = 0
	Data[1] = DATO_HIGH | 0X09;	// EN = 0 RS = 0
	Data[2] = DATO_LOW  | 0X0D; // EN = 1 RS = 0
	Data[3] = DATO_LOW  | 0X09; // EN = 0 RS = 0

	I2C_Transmit(Data, 4);
	HAL_Delay(5);
}

/**
  * @brief This function sends a string to the display.
  *
  * This function uses LCD_Send_Char() function to print a string on the display.
  *
  * @param uint8_t *string
  * @retval void
  */
void LCD_Send_String(uint8_t *string)
{
	// Funcion para escribir una cadena de caracteres en el display

	uint8_t i = 0;
	while (string[i] != '\0')
	{
		LCD_Send_Char(*string);
		string += 1;
	}
	return;
}

/**
  * @brief This function places the cursor on the display.
  *
  * This function uses LCD_Send_Command() to place the cursor on the display.
  * As parameters, it takes the coord. (x,y) where the user wants to place the cursor.
  *
  * param uint8_t x, uint8_t y
  * @retval void
  */
void LCD_Place_Cursor(uint8_t x, uint8_t y)
{
	uint8_t inicio[] = {0x80|0x00, 0x80|0xC0};
	LCD_Send_Cmd(inicio[y-1]+x-1);
	HAL_Delay(10);
}

/**
  * @brief This function takes two strings and prints them on the LCD.
  *
  * This function takes two strings A and B and prints them on the display,
  * one in each row.
  *
  * @param uint8_t *String_A, uint8_t *String_B
  * @retval void
  */
void LCD_Print(uint8_t *String_A, uint8_t *String_B)
{
	LCD_Place_Cursor(1,1);
	LCD_Send_String((uint8_t *)String_A);
	LCD_Place_Cursor(1,2);
	LCD_Send_String((uint8_t *)String_B);
}

/**
  * @brief This function clears the display.
  *
  * This function uses LCD_Send_Cmd to clear the display.
  *
  * @param void
  * @retval void
  */
void LCD_Clear(void)
{
	LCD_Send_Cmd(0x01);
	HAL_Delay(10);
}


