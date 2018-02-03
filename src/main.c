/**
 ******************************************************************************
 * @file    main.c
 * @author  Ac6
 * @version V1.0
 * @date    01-December-2013
 * @brief   Default main function.
 ******************************************************************************
 */

#include <dma2_driver.h>
#include "main.h"
#include <string.h>
#include <usart6_dma_comm.h>
#include "inp_capture.h"
#include "timer2_output.h"
#include "comm_service.h"
#include "external_int_button.h"
#include "i2c_lcd.h"
#include "gpio_led.h"

//#define LED_DEBUG
#define BUF_SIZE       384
char gps_string[BUF_SIZE] = { 0 };

uint8_t str_to_disp[DMA_RX_BUFFER_SIZE] = { 0 };

volatile static uint16_t debounce_count = 0;

volatile unsigned long msTicks = 0;
volatile uint8_t Rx_Interrupt_flag = FALSE;
volatile char Transfer_cplt = FALSE;
volatile uint8_t Ext_Int_Occured = FALSE;
volatile uint8_t DMA2_TxCplt_Occured = FALSE;
volatile uint8_t Recvd_word = 0;
void Delay_ms(unsigned long del_count)
{
	del_count *= 1000;
	msTicks = del_count;
	while (msTicks != 0)
		;
}

void SysTick_Init(void)
{
	/****************************************
	 *SystemFrequency/1000      1ms         *
	 *SystemFrequency/100000    10us        *
	 *SystemFrequency/1000000   1us         *
	 *****************************************/

	//SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
	while (SysTick_Config(SystemCoreClock / 1000000) != 0)
	{
	}

}
static uint16_t buf_count = 0;

int main(void)
{

	SystemInit();
	SysTick_Init();
	RCC_HSICmd(ENABLE);

	/* Timer2 config */
	initialiseTimer();
	init_led();
	enable_Timer2_Interrupts();

	/* InputCapture config */
//	init_capture_module();
//	init_IC_gpio();
//	enable_capture_mod_irq();
	/* USART config */
	init_usart2_comm_module();
	init_usart2_gpio();
	enable_usart2_rx_irq();

	/* I2C config init*/
	init_i2c_bus_config();
	init_i2c_gpio();
	/* I2C config init*/

	/* LCD Display Init*/
	LCD_Init();
	LCD_Clear();
	/* LCD Display Init*/

	LCD_Goto(1, 1);
	LCD_Write_String("Time: ");

	init_external_int_gpio();
	init_external_interrupt();
	enable_external_button_interrupt();

	/*Crazy USART DMA Part*/
	init_dma2();
	enable_dma2_irq();

	/*USART6 init*/
	init_usart6_gpio();
	init_usart6_comm_module();
	enable_usart6_rx_irq();

#ifdef LED_DEBUG

	init_led();

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_InitDef.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitDef.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitDef.GPIO_OType = GPIO_OType_PP;
	GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitDef.GPIO_Speed = GPIO_Speed_100MHz;
	//Initialize pins
	GPIO_Init(GPIOA, &GPIO_InitDef);

#endif

	while (1)
	{
		GPIO_SetBits(GPIOD, GPIO_Pin_13);

		if (DMA2_TxCplt_Occured == TRUE)
		{

			/* Copy the DMA Buffer into local buffer*/
			memcpy(gps_string, DMA_RX_Buffer, DMA_RX_BUFFER_SIZE);

			//USART6_TX_string(gps_string);

			init_dma2();

			/*Display the parsed string over LCD*/
			Display_Time_LCD(gps_string, "$GPRMC", 7);

			DMA2_TxCplt_Occured = FALSE;
		}

		if (Ext_Int_Occured == TRUE)
		{
			LCD_Clear();
			LCD_Goto(1, 2);
			LCD_Write_String("Location: ");
			//Delay_ms(100);
			LCD_Clear();
			LCD_Goto(1, 1);
			LCD_Write_String("Time: ");
			Ext_Int_Occured = FALSE;
		}
		if (Rx_Interrupt_flag == TRUE)
		{
			buf_count++;

			if (buf_count > BUF_SIZE - 1)
			{
				buf_count = 0;
			}
			Rx_Interrupt_flag = FALSE;
		}
	}
	return 0;
}

void TIM2_IRQHandler(void)
{
	//GPIO_SetBits(GPIOD, GPIO_Pin_12);
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{
		GPIO_ToggleBits(GPIOD, GPIO_Pin_12);
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}

void TIM1_CC_IRQHandler(void)
{

	if (TIM_GetITStatus(TIM1, TIM_IT_CC1) != RESET)
	{
		//GPIO_ToggleBits(GPIOD, GPIO_Pin_14);
		TIM_ClearITPendingBit(TIM1, TIM_IT_CC1);
	}
}

void SysTick_Handler(void)
{ /* SysTick interrupt Handler. */

	msTicks--;
}

void USART2_IRQHandler(void)
{

	if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{

		//GPIO_ToggleBits(GPIOD, GPIO_Pin_14);
		Rx_Interrupt_flag = TRUE;
		Recvd_word = USART_ReceiveData(USART2);
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);

	}
}

void EXTI0_IRQHandler(void)
{

	if ((EXTI_GetITStatus(EXTI_Line0) != RESET))
	{
		Ext_Int_Occured = TRUE;
		//GPIO_ToggleBits(GPIOD, GPIO_Pin_14);
		EXTI_ClearITPendingBit(EXTI_Line0);

	}

}

void DMA2_Stream2_IRQHandler(void)
{

	if (DMA_GetITStatus(DMA2_USART6_STREAM, DMA_IT_TCIF2) != RESET)
	{
		DMA2_TxCplt_Occured = TRUE;

		GPIO_ToggleBits(GPIOD, GPIO_Pin_15);

		DMA_ClearITPendingBit(DMA2_USART6_STREAM, DMA_IT_TCIF2);
		DMA_ClearITPendingBit(DMA2_USART6_STREAM, DMA_IT_HTIF2);
		DMA_ClearITPendingBit(DMA2_USART6_STREAM, DMA_IT_FEIF2);
		DMA_ClearITPendingBit(DMA2_USART6_STREAM, DMA_IT_DMEIF2);
		DMA_ClearITPendingBit(DMA2_USART6_STREAM, DMA_IT_TEIF2);

	}
}

