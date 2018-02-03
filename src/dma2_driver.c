/*
 * dma1_driver.c
 *
 *  Created on: Feb 2, 2018
 *      Author: abel
 */

#include <dma2_driver.h>


/*CHANNEL 5 STREAM2 is USART6-RX*/


void init_dma2(void)
{
	DMA_InitTypeDef DMA2_InitStruct;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);


    DMA_StructInit(&DMA2_InitStruct);
	DMA2_InitStruct.DMA_Channel            = DMA_Channel_5;
	DMA2_InitStruct.DMA_DIR                = DMA_DIR_PeripheralToMemory;
	DMA2_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA2_InitStruct.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;
	DMA2_InitStruct.DMA_MemoryInc          = DMA_MemoryInc_Enable;
	DMA2_InitStruct.DMA_BufferSize         = DMA_RX_BUFFER_SIZE;
	DMA2_InitStruct.DMA_Priority           = DMA_Priority_Low;
	DMA2_InitStruct.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
	//DMA1_InitStruct.DMA_FIFOMode           = DMA_FIFOMode_Disable;
	DMA2_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&USART6->DR;
	DMA2_InitStruct.DMA_Memory0BaseAddr  =(uint32_t)DMA_RX_Buffer;

	DMA_Init(DMA2_USART6_STREAM,&DMA2_InitStruct);
	//DMA_ClearITPendingBit(DMA_USART1_STREAM,DMA_IT_TCIF2);
	DMA_ITConfig(DMA2_USART6_STREAM, DMA_IT_TC, ENABLE);

	DMA_Cmd(DMA2_USART6_STREAM, ENABLE);
}

void enable_dma2_irq(void)
{

	NVIC_InitTypeDef DMA2_NVICStruct;
	DMA2_NVICStruct.NVIC_IRQChannel                   = DMA2_Stream2_IRQn;
	DMA2_NVICStruct.NVIC_IRQChannelCmd                = ENABLE;
	DMA2_NVICStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
	DMA2_NVICStruct.NVIC_IRQChannelSubPriority        = 0x00;

	NVIC_Init(&DMA2_NVICStruct);
}
