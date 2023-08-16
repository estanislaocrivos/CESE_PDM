/*
 * API_LCD.h
 *
 * This library contains functions for using and handling an 16X2 LCD display
 * using I2C communication.
 *
 * Author: Estanislao Crivos
 * Date: August 2023
 *
 */

#ifndef API_INC_API_LCD_H_
#define API_INC_API_LCD_H_

#include <stdint.h>

void LCD_Init(void);
void LCD_Clear(void);
void LCD_Send_Cmd(uint8_t command);
void LCD_Send_Char(uint8_t character);
void LCD_Place_Cursor(uint8_t x, uint8_t y);
void LCD_Send_String(uint8_t *string);
void LCD_Print(uint8_t *String_A, uint8_t *String_B);

#endif /* API_INC_API_I2C_H_ */



