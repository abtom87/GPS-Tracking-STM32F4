/**
 ******************************************************************************
 * @file    main.c
 * @author  Ac6
 * @version V1.0
 * @date    01-December-2013
 * @brief   Default main function.
 ******************************************************************************
 */

#include "main.h"
#include <string.h>
#include "inp_capture.h"
#include "timer2_output.h"
#include "comm_service.h"
#include "external_int_button.h"
#include "i2c_lcd.h"

//#define LED_DEBUG
#define BUF_SIZE       384
char gps_string[BUF_SIZE] = { 0 };
char Time_buffer[10]={0};
volatile char *substr;

volatile static uint16_t debounce_count=0;

volatile unsigned long msTicks = 0;
volatile uint8_t Rx_Interrupt_flag = FALSE;
volatile char Transfer_cplt = FALSE;
volatile uint8_t Ext_Int_Occured= FALSE;


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

	int k = 0;
	SystemInit();
	SysTick_Init();
	RCC_HSICmd(ENABLE);

	/* Timer2 config */
	initialiseTimer();
	init_led();
	enable_Timer2_Interrupts();

	/* InputCapture config */
	init_capture_module();
	init_IC_gpio();
	enable_capture_mod_irq();

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


		if(Ext_Int_Occured)
		{
//			LCD_Clear();
//			LCD_Goto(1,2);
//			LCD_Write_String("Location: ");
//		//	Delay_ms(3000);
//			Ext_Int_Occured=FALSE;


		}


		if(Rx_Interrupt_flag==TRUE)
		{   __disable_irq();
			USART_TX_byte(Recvd_word);

			gps_string[buf_count]=Recvd_word;
			buf_count++;

			if(buf_count>BUF_SIZE-1)
			{
				substr = strstr(gps_string,"$GPRMC");
				k=7;
				while(k<13)
				{
			    	Time_buffer[k-7]=(substr[k]);
			    	k++;
				}

				Time_buffer[1] += 1;
				//Time_buffer[1] %=0x0a;
				LCD_Goto(1,1);
				LCD_Write_String("Time: ");
				LCD_Write_String(Time_buffer);
				LCD_Goto(7,1);

				buf_count=0;

			}

			Rx_Interrupt_flag=FALSE;
			__enable_irq();


		}






#ifdef LED_DEBUG
		if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0))
		{
			GPIO_SetBits(GPIOD, GPIO_Pin_13 | GPIO_Pin_14);
			Delay_ms(1000);
			GPIO_ResetBits(GPIOD, GPIO_Pin_13 | GPIO_Pin_14);
			Delay_ms(1000);

		}
		else
		{
			GPIO_ResetBits(GPIOD, GPIO_Pin_13 | GPIO_Pin_14);
		}

#endif

	}
	return 0;
}

void TIM2_IRQHandler()
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

void USART2_IRQHandler()
{

	if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{

		//GPIO_ToggleBits(GPIOD, GPIO_Pin_14);
		Rx_Interrupt_flag = TRUE;
		Recvd_word = USART_ReceiveData(USART2);
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);

	}
}

void EXTI0_IRQHandler()
{


	if( (EXTI_GetITStatus(EXTI_Line0)!= RESET ) )
	{
		Ext_Int_Occured = TRUE;
		//GPIO_ToggleBits(GPIOD, GPIO_Pin_15);
		EXTI_ClearITPendingBit(EXTI_Line0);

	}

}

