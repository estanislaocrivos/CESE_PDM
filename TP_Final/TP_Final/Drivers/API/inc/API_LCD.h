/*
 * API_LCD.h
 *
 *  Created on: Aug 4, 2023
 *  Author: estanislaocrivosgandini
 */

#ifndef API_INC_API_LCD_H_
#define API_INC_API_LCD_H_

#include <stdint.h>

// void LCD_Send_String(char *string);
void LCD_Send_String(char *str)


void LCD_Send_Cmd(char command);

void LCD_Send_Char(char character);

void LCD_Place_Cursor(uint8_t x, uint8_t y);

void LCD_Init(void);

#endif /* API_INC_API_I2C_H_ */



