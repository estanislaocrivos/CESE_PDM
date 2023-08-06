/*
 * API_LCD.h
 *
 *  Created on: Aug 4, 2023
 *  Author: estanislaocrivosgandini
 */

#ifndef API_INC_API_LCD_H_
#define API_INC_API_LCD_H_

void LCD_Send_String(uint8_t *string);

void LCD_Send_Cmd(uint8_t command);

void LCD_Send_Char(uint8_t character);

void LCD_Place_Cursor(uint8_t x, uint8_t y);

void LCD_Init(void);

#endif /* API_INC_API_I2C_H_ */



