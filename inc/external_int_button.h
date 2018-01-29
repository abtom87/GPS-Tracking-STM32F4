/*
 * external_int_button.h
 *
 *  Created on: Jan 29, 2018
 *      Author: abel
 */

#ifndef EXTERNAL_INT_BUTTON_H_
#define EXTERNAL_INT_BUTTON_H_

#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_it.h"
#include "stm32f4xx_syscfg.h"



#define EXTERNAL_BUTTON_PORT GPIOA



void init_external_interrupt(void);
void init_external_int_gpio(void);
void enable_external_button_interrupt(void);

#endif /* EXTERNAL_INT_BUTTON_H_ */
