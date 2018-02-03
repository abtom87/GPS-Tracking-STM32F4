/*
 * external_int_button.c
 *
 *  Created on: Jan 29, 2018
 *      Author: abel
 */

#include "external_int_button.h"


void enable_external_button_interrupt(void)
{
	NVIC_InitTypeDef NVIC_EXTINT_Button;
	NVIC_EXTINT_Button.NVIC_IRQChannel                   = EXTI0_IRQn;
	NVIC_EXTINT_Button.NVIC_IRQChannelCmd                = ENABLE;
	NVIC_EXTINT_Button.NVIC_IRQChannelPreemptionPriority = PRIO_EXT_INT0;
	NVIC_EXTINT_Button.NVIC_IRQChannelSubPriority        = 0;

    NVIC_Init(&NVIC_EXTINT_Button);


}




void init_external_int_gpio(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);

	GPIO_InitTypeDef GPIO_EXT_InitStruct;
	GPIO_EXT_InitStruct.GPIO_Mode   = GPIO_Mode_IN;
	GPIO_EXT_InitStruct.GPIO_OType  = GPIO_OType_OD;
	GPIO_EXT_InitStruct.GPIO_Pin    = GPIO_Pin_0;
	GPIO_EXT_InitStruct.GPIO_PuPd   = GPIO_PuPd_DOWN;
	GPIO_EXT_InitStruct.GPIO_Speed  = GPIO_High_Speed;

	GPIO_Init(EXTERNAL_BUTTON_PORT,&GPIO_EXT_InitStruct);



}


void init_external_interrupt(void)
{
	EXTI_InitTypeDef EXT_GPIO_InitStruct;
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
//
//
//	SYSCFG_EXTILineConfig(EXTERNAL_BUTTON_PORT,GPIO_Pin_0);
	EXT_GPIO_InitStruct.EXTI_Line    = EXTI_Line0;
	EXT_GPIO_InitStruct.EXTI_LineCmd = ENABLE;
	EXT_GPIO_InitStruct.EXTI_Mode    = EXTI_Mode_Interrupt;
	EXT_GPIO_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;

	EXTI_Init(&EXT_GPIO_InitStruct);



}
