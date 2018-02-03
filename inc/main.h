/*
 * main.h
 *
 *  Created on: Jan 27, 2018
 *      Author: abel
 */

#ifndef MAIN_H_
#define MAIN_H_
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_it.h"

#define TRUE  1
#define FALSE 0












void initialiseTimer(void);
void SysTick_Init(void);
void Delay_ms(unsigned long);
void enable_Tim_Interrupts(void);





//GPIO_InitTypeDef GPIO_InitDef;

#endif /* MAIN_H_ */
