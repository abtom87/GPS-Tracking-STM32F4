/*
 * usart1_dma_comm.c
 *
 *  Created on: Feb 2, 2018
 *      Author: abel
 */

#include <usart6_dma_comm.h>


void init_usart6_comm_module(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6,ENABLE);

	USART_InitTypeDef USART6_InitStruct;

	USART6_InitStruct.USART_BaudRate            = 9600;
	USART6_InitStruct.USART_HardwareFlowControl = DISABLE;
	USART6_InitStruct.USART_Mode                = (USART_Mode_Tx | USART_Mode_Rx);
	USART6_InitStruct.USART_Parity              = USART_Parity_No;
	USART6_InitStruct.USART_StopBits            = USART_StopBits_1;
	USART6_InitStruct.USART_WordLength          = USART_WordLength_8b;

	USART_Init(USART6,&USART6_InitStruct);

	USART_Cmd(USART6,ENABLE);
	USART_DMACmd(USART6, USART_DMAReq_Rx, ENABLE);


}

void init_usart6_gpio(void)
{
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);

		/*PC6--> Tx; PC7-->Rx*/
		GPIO_InitTypeDef USART6gpio_InitStruct;
		USART6gpio_InitStruct.GPIO_Pin    =  (GPIO_Pin_6 | GPIO_Pin_7) ;
		USART6gpio_InitStruct.GPIO_Mode   =  GPIO_Mode_AF;
		USART6gpio_InitStruct.GPIO_OType  =  GPIO_OType_PP;
		USART6gpio_InitStruct.GPIO_PuPd   =  GPIO_PuPd_NOPULL;
		USART6gpio_InitStruct.GPIO_Speed  =  GPIO_Speed_2MHz;

		GPIO_Init(USART6_GPIO,&USART6gpio_InitStruct);

		GPIO_PinAFConfig(USART6_GPIO,GPIO_PinSource6,GPIO_AF_USART6);
		GPIO_PinAFConfig(USART6_GPIO,GPIO_PinSource7,GPIO_AF_USART6);

}



void enable_usart6_rx_irq(void)
{
   NVIC_InitTypeDef USART6_NVICStruct;
   USART6_NVICStruct.NVIC_IRQChannel = USART6_IRQn;
   USART6_NVICStruct.NVIC_IRQChannelCmd = ENABLE;
   USART6_NVICStruct.NVIC_IRQChannelPreemptionPriority=0;
   USART6_NVICStruct.NVIC_IRQChannelSubPriority =0;

   NVIC_Init(&USART6_NVICStruct);

}






void USART6_TX_byte(uint8_t byte)
{
	while( (SET != USART_GetFlagStatus(USART6,USART_FLAG_TXE) ) )
		;
	USART_SendData(USART6,(uint16_t)byte);

}

void USART6_TX_string(char *str_to_transmit)
{
	int i;

	for(i=0;i<(strlen(str_to_transmit));i++)
		USART6_TX_byte(str_to_transmit[i]);

}














