/*
 * inp_capture.c
 *
 *  Created on: Jan 27, 2018
 *      Author: abel
 */

#include "inp_capture.h"



void init_IC_gpio(void)
{

	GPIO_InitTypeDef gpioIC_InitStr;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	gpioIC_InitStr.GPIO_Pin = GPIO_Pin_8;
	gpioIC_InitStr.GPIO_OType=GPIO_OType_PP;
    gpioIC_InitStr.GPIO_Mode = 	GPIO_Mode_AF;
    gpioIC_InitStr.GPIO_PuPd = GPIO_PuPd_NOPULL;
    gpioIC_InitStr.GPIO_Speed = GPIO_Speed_2MHz;


	GPIO_Init(GPIOA, &gpioIC_InitStr);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource8 ,GPIO_AF_TIM1);

	GPIO_WriteBit(GPIOA,  GPIO_Pin_8, Bit_RESET);

}





void init_capture_module(void)
{
	TIM_TimeBaseInitTypeDef Tim1_BaseInitStruct;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

	Tim1_BaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	Tim1_BaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	Tim1_BaseInitStruct.TIM_Period = 0x2; /*Auto reload register value*/
	Tim1_BaseInitStruct.TIM_Prescaler = 0x200;
	Tim1_BaseInitStruct.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1, &Tim1_BaseInitStruct);

	TIM_ClearITPendingBit(TIM1, TIM_IT_CC1);
	TIM_ITConfig(TIM1, TIM_IT_CC1, ENABLE);
	TIM_Cmd(TIM1, ENABLE);

	Tim1_IC_Struct.TIM_Channel      = TIM_Channel_1;
	Tim1_IC_Struct.TIM_ICFilter     = 0x0;
	Tim1_IC_Struct.TIM_ICPrescaler  = TIM_ICPSC_DIV1;
	Tim1_IC_Struct.TIM_ICPolarity   = TIM_ICPolarity_Rising;
	Tim1_IC_Struct.TIM_ICSelection  = TIM_ICSelection_DirectTI;
	TIM_ICInit(TIM1,&Tim1_IC_Struct);

}

void enable_capture_mod_irq(void)
{

	NVIC_InitTypeDef CC_IRQ_InitStruct;
	CC_IRQ_InitStruct.NVIC_IRQChannel     = TIM1_CC_IRQn;
	CC_IRQ_InitStruct.NVIC_IRQChannelCmd  = ENABLE;
	CC_IRQ_InitStruct.NVIC_IRQChannelPreemptionPriority = PRIO_INP_CAPTURE;
	CC_IRQ_InitStruct.NVIC_IRQChannelSubPriority =0x00;

	NVIC_Init(&CC_IRQ_InitStruct);


}










