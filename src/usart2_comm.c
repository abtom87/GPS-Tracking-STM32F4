/*
 * usart2_comm.c
 *
 *  Created on: Jan 27, 2018
 *      Author: abel
 */


#include "usart2_comm.h"

void init_usart2_comm_module(void)
{
	USART_InitTypeDef USART2_InitStruct;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	USART2_InitStruct.USART_BaudRate           = 9600;
	USART2_InitStruct.USART_HardwareFlowControl= DISABLE;
	USART2_InitStruct.USART_Mode               = (USART_Mode_Rx| USART_Mode_Tx);
	USART2_InitStruct.USART_Parity             = USART_Parity_No;
	USART2_InitStruct.USART_StopBits           = USART_StopBits_1;
	USART2_InitStruct.USART_WordLength         = USART_WordLength_8b;

	USART_Init(USART2,&USART2_InitStruct );


	USART_ClearITPendingBit(USART2, USART_IT_RXNE);
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
	USART_Cmd(USART2,ENABLE);

}



void init_usart2_gpio(void)
{
	GPIO_InitTypeDef USART2_GPIOInitStruct;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);

	/*PD5-Tx, PD6-Rx*/
	USART2_GPIOInitStruct.GPIO_Pin   = (GPIO_Pin_5 | GPIO_Pin_6);
	USART2_GPIOInitStruct.GPIO_Mode  = GPIO_Mode_AF;
	USART2_GPIOInitStruct.GPIO_OType = GPIO_OType_PP;
	USART2_GPIOInitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	USART2_GPIOInitStruct.GPIO_Speed = GPIO_Speed_2MHz;

    GPIO_Init(GPIOD,&USART2_GPIOInitStruct);

    GPIO_PinAFConfig(GPIOD,GPIO_PinSource5,GPIO_AF_USART2);
    GPIO_PinAFConfig(GPIOD,GPIO_PinSource6,GPIO_AF_USART2);

}

void enable_usart2_rx_irq(void)
{
	NVIC_InitTypeDef USART2_Rx_IRQ_Struct;
	USART2_Rx_IRQ_Struct.NVIC_IRQChannel                    = USART2_IRQn;
	USART2_Rx_IRQ_Struct.NVIC_IRQChannelCmd                 = ENABLE;
	USART2_Rx_IRQ_Struct.NVIC_IRQChannelPreemptionPriority  = PRIO_USART2;
	USART2_Rx_IRQ_Struct.NVIC_IRQChannelSubPriority         = 0;

	NVIC_Init(&USART2_Rx_IRQ_Struct);


}


void USART_TX_byte(uint8_t byte)
{
	while( (SET != USART_GetFlagStatus(USART2,USART_FLAG_TXE) ) )
		;
	USART_SendData(USART2,(uint16_t)byte);

}

void USART_TX_string(char *str_to_transmit)
{
	int i;

	for(i=0;i<(strlen(str_to_transmit));i++)
		USART_TX_byte(str_to_transmit[i]);

}




