/*
 * inp_capture.h
 *
 *  Created on: Jan 27, 2018
 *      Author: abel
 */

#ifndef INP_CAPTURE_H_
#define INP_CAPTURE_H_

#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_it.h"


TIM_ICInitTypeDef       Tim1_IC_Struct;
void init_capture_module(void);
void enable_capture_mod_irq(void);
void init_IC_gpio(void);






#endif /* INP_CAPTURE_H_ */
