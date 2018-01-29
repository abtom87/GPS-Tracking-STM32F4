/*
 * i2c_lcd.c
 *
 *  Created on: Jan 24, 2018
 *      Author: abel
 */


#include <i2c_lcd.h>
#include "main.h"
static uint8_t  LCD_BL_Status = 1;


unsigned char bit_test(uint8_t nibble, uint8_t bit_num)
{
   if( (nibble&(1<<bit_num)) )
    return TRUE;
   else
	return FALSE;

}




void LCD_BL(uint8_t status)
{
    LCD_BL_Status = status;
    LCD_Write_Byte(0x00, 0x00);
}

void LCD_Init()
{


   //  Following bytes are all Command bytes, i.e. address = 0x00
    LCD_Write_Byte(0x00, 0x03);   // Write Nibble 0x03 three times (per HD44780U initialization spec)
    Delay_ms(10);                // (per HD44780U initialization spec)
    LCD_Write_Byte(0x00, 0x03);   //
    Delay_ms(10);               // (per HD44780U initialization spec)
    LCD_Write_Byte(0x00, 0x03);   //
    Delay_ms(10);               // (per HD44780U initialization spec)
    LCD_Write_Byte(0x00, 0x02);   // Write Nibble 0x02 once (per HD44780U initialization spec)
    Delay_ms(10);
    LCD_Write_Byte(0x00, 0x02);   // Write Nibble 0x02 once (per HD44780U initialization spec)
    Delay_ms(10);               // (per HD44780U initialization spec)
    LCD_Write_Byte(0x00, 0x01);   // Set mode: 4-bit, 2+lines, 5x8 dots
    Delay_ms(10);               // (per HD44780U initialization spec)
    LCD_Write_Byte(0x00, 0x0C);   // Display ON 0x0C
    Delay_ms(10);               // (per HD44780U initialization spec)
    LCD_Write_Byte(0x00, 0x01);   // Clear display
    Delay_ms(10);              // (per HD44780U initialization spec)
    LCD_Write_Byte(0x00, 0x06);   // Set cursor to increment
    Delay_ms(10);               // (per HD44780U initialization spec)


}

void LCD_Goto(uint8_t x, uint8_t y)
{
uint8_t address;

   switch(y)
     {
      case 1:
        address = LCD_PIC_LINE_1_ADDRESS;
        break;

      case 2:
        address = LCD_PIC_LINE_2_ADDRESS;
        break;

      case 3:
        address = LCD_PIC_LINE_3_ADDRESS;
        break;

      case 4:
        address = LCD_PIC_LINE_4_ADDRESS;
        break;

      default:
        address = LCD_PIC_LINE_1_ADDRESS;
        break;
     }

   address += x-1;
   LCD_Write_Byte(0, 0x80 | address);
}

//===================================
void LCD_Write_String(const char *str)
{
   // Writes a string text[] to LCD via I2C
   pin_RS  = 1;
   pin_RW  = 0;
   pin_E   = 0;
   pin_BL  = LCD_BL_Status;

   while (*str)
   {
        // Send upper nibble
        _LCD_Write_Upper_Nibble(*str);

        // Send lower nibble
        _LCD_Write_Lower_Nibble(*str);

        str++;
   }
}

void LCD_Write_Char(char c)
{
    LCD_Write_Byte(0x01,c);
}

void LCD_Write_Int(int32_t num)
{
    if (num < 0) { LCD_Write_String("-"); num *= -1; }

    uint8_t number[10];
    uint8_t num_count = 0;

    do {
        number[num_count] = num % 10;
        num_count++;
        num /= 10;
    } while (num > 0);

    for (int i = num_count-1; i>= 0; i--)
    {
        LCD_Write_Char(number[i] + 0b00110000);
    }
}

//===================================
void LCD_Write_Byte(uint8_t address, uint8_t n)
{
    if (address)
    {
        pin_RS=1;   // Data
    }
    else
    {
        pin_RS=0;   // Command
    }

    pin_RW  = 0;
    pin_E   = 0;
    pin_BL  = LCD_BL_Status;

    // Send upper nibble
   _LCD_Write_Upper_Nibble(n);

    // Send lower nibble
   _LCD_Write_Lower_Nibble(n);
}

void LCD_Clear()
{
    LCD_Write_Byte(0x00,0x01);
    //HAL_Delay(1);
}

void LCD_Clear_Line(uint8_t line)
{
    LCD_Goto(1,line);
    for (int i = 0; i<20; i++)
    {
        LCD_Write_String(" ");
    }
    LCD_Goto(1,line);
}

void _LCD_Write_Upper_Nibble(uint8_t u)
{
	uint8_t upper_nibble_to_send=0;
    // Send upper nibble
    if(bit_test(u,7))
        pin_D7=1;
    else
        pin_D7=0;

    if(bit_test(u,6))
        pin_D6=1;
    else
        pin_D6=0;

    if(bit_test(u,5))
        pin_D5=1;
    else
        pin_D5=0;

    if(bit_test(u,4))
        pin_D4=1;
    else
        pin_D4=0;

   pin_E = 0;


   upper_nibble_to_send = _LCD_Build_Byte();
   I2C_Transmit(upper_nibble_to_send, SLAVE_ADDR);
   //HAL_I2C_Master_Transmit(&hi2c1,SLAVE_ADDR<<1, &upper_nibble_to_send,1,3 );
   //WriteI2C(_LCD_Build_Byte());
   pin_E = 1;
   upper_nibble_to_send = _LCD_Build_Byte();
   I2C_Transmit(upper_nibble_to_send, SLAVE_ADDR);
   //HAL_I2C_Master_Transmit(&hi2c1,SLAVE_ADDR<<1, &upper_nibble_to_send,1,3 );


   pin_E = 0;
   upper_nibble_to_send = _LCD_Build_Byte();
   I2C_Transmit(upper_nibble_to_send, SLAVE_ADDR);
  //  HAL_I2C_Master_Transmit(&hi2c1,SLAVE_ADDR<<1, &upper_nibble_to_send,1,3 );

}

void _LCD_Write_Lower_Nibble(uint8_t l)
{
	uint8_t lower_nibble_to_send=0;
    // Send lower nibble
    if(bit_test(l,3))
        pin_D7=1;
    else
        pin_D7=0;

    if(bit_test(l,2))
        pin_D6=1;
    else
        pin_D6=0;

    if(bit_test(l,1))
        pin_D5=1;
    else
        pin_D5=0;

    if(bit_test(l,0))
        pin_D4=1;
    else
        pin_D4=0;

    pin_E = 0;

  //    WriteI2C(_LCD_Build_Byte());
    lower_nibble_to_send = _LCD_Build_Byte();
    I2C_Transmit(lower_nibble_to_send, SLAVE_ADDR);
  //  HAL_I2C_Master_Transmit(&hi2c1,SLAVE_ADDR<<1, &lower_nibble_to_send,1,3 );
    pin_E = 1;
    //    WriteI2C(_LCD_Build_Byte());
    lower_nibble_to_send = _LCD_Build_Byte();
    I2C_Transmit(lower_nibble_to_send, SLAVE_ADDR);
    //  HAL_I2C_Master_Transmit(&hi2c1,SLAVE_ADDR<<1, &lower_nibble_to_send,1,3 );
    pin_E = 0;

    lower_nibble_to_send = _LCD_Build_Byte();
    I2C_Transmit(lower_nibble_to_send, SLAVE_ADDR);
    // HAL_I2C_Master_Transmit(&hi2c1,SLAVE_ADDR<<1, &lower_nibble_to_send,1,3 );
}

uint8_t _LCD_Build_Byte()
{
    uint8_t ret = 0x00;

    ret |= pin_E    << 2;
    ret |= pin_RW   << 1;
    ret |= pin_RS   << 0;
    ret |= pin_D4   << 4;
    ret |= pin_D5   << 5;
    ret |= pin_D6   << 6;
    ret |= pin_D7   << 7;
    ret |= pin_BL   << 3;

    return ret;
}




