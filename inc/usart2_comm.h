/*
 * usart2_comm.h
 *
 *  Created on: Jan 27, 2018
 *      Author: abel
 */

#ifndef USART2_COMM_H_
#define USART2_COMM_H_


#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_it.h"
#include "stm32f4xx_usart.h"
#include <string.h>

volatile uint16_t Recvd_word;
void init_usart2_comm_module(void);
void init_usart2_gpio(void);
void enable_usart2_rx_irq(void);
void USART_TX_byte(uint8_t);
void USART_TX_string(char *);

#endif /* USART2_COMM_H_ */
