/*
 * gpio_led.c
 *
 *  Created on: Jan 31, 2018
 *      Author: abel
 */



#include "gpio_led.h"

void init_led(void)
{
	GPIO_InitTypeDef gpioInitStr;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	gpioInitStr.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_12 | GPIO_Pin_14 | GPIO_Pin_15;
	gpioInitStr.GPIO_Mode = GPIO_Mode_OUT;
	gpioInitStr.GPIO_OType = GPIO_OType_PP;
	gpioInitStr.GPIO_PuPd = GPIO_PuPd_NOPULL;
	gpioInitStr.GPIO_Speed = GPIO_Speed_100MHz;

	GPIO_Init(GPIOD, &gpioInitStr);
    GPIO_WriteBit(GPIOD,  GPIO_Pin_13, Bit_RESET);

}
