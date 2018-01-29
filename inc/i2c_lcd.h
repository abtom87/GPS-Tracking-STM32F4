/*
 * i2c_lcd.h
 *
 *  Created on: Jan 24, 2018
 *      Author: abel
 */

#ifndef I2C_LCD_H_
#define I2C_LCD_H_
#include "i2c1_comm.h"


#define SLAVE_ADDR 0x27


// Line addresses for LCDs which use
// the Hitachi HD44780U controller chip
#define LCD_PIC_LINE_1_ADDRESS 0x00
#define LCD_PIC_LINE_2_ADDRESS 0x40
#define LCD_PIC_LINE_3_ADDRESS 0x14
#define LCD_PIC_LINE_4_ADDRESS 0x54


//uint8_t  LCD_BL_Status = 1;     // 1 for POSITIVE control, 0 for NEGATIVE control

uint8_t  pin_E;//   =    I2C_BYTE.2
uint8_t  pin_RW;//  =    I2C_BYTE.1
uint8_t  pin_RS;//  =    I2C_BYTE.0
uint8_t  pin_D4;//  =    I2C_BYTE.4
uint8_t  pin_D5;//  =    I2C_BYTE.5
uint8_t  pin_D6;//  =    I2C_BYTE.6
uint8_t  pin_D7;//  =    I2C_BYTE.7
uint8_t  pin_BL;//  =    I2C_BYTE.3

void LCD_Init();
void LCD_Goto(uint8_t x, uint8_t y);
void LCD_Write_Char(char c);
void LCD_Write_String(const char *str);
void LCD_Write_Int(int32_t num);
void LCD_Write_Byte(uint8_t address, uint8_t n);
void LCD_Clear();
void LCD_Clear_Line(uint8_t line);
unsigned char bit_test(uint8_t nibble, uint8_t bit_num);

void LCD_BL(uint8_t status); // set blacklight

uint8_t _LCD_Build_Byte();
void _LCD_Write_Upper_Nibble(uint8_t u);
void _LCD_Write_Lower_Nibble(uint8_t l);



#endif /* I2C_LCD_H_ */
