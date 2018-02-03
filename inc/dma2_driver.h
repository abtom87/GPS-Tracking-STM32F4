/*
 * dma1_driver.h
 *
 *  Created on: Feb 2, 2018
 *      Author: abel
 */

#ifndef DMA2_DRIVER_H_
#define DMA2_DRIVER_H_


#include "stm32f4xx_dma.h"

#define DMA2_USART6_STREAM DMA2_Stream2
#define DMA_RX_BUFFER_SIZE          384
uint8_t DMA_RX_Buffer[DMA_RX_BUFFER_SIZE];

void init_dma2(void);

void enable_dma2_irq(void);


#endif /* DMA2_DRIVER_H_ */
