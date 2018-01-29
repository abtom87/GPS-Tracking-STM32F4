
#include "timer2_output.h"




void initialiseTimer(void)
{
	TIM_TimeBaseInitTypeDef Tim2_InitStruct;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
    Tim2_InitStruct.TIM_ClockDivision= TIM_CKD_DIV1;
	Tim2_InitStruct.TIM_CounterMode=TIM_CounterMode_Up;
	Tim2_InitStruct.TIM_Period = 0x270F;  /*Auto reload register value*/
	Tim2_InitStruct.TIM_Prescaler= 0x270F;
	Tim2_InitStruct.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM2,&Tim2_InitStruct);

    TIM_ClearITPendingBit (TIM2, TIM_IT_Update);
    TIM_ITConfig(TIM2, TIM_IT_Update,ENABLE );

    TIM_Cmd(TIM2,ENABLE);



}


void enable_Timer2_Interrupts(void)
{
	NVIC_InitTypeDef nvicStruct;
	nvicStruct.NVIC_IRQChannel = TIM2_IRQn;
	nvicStruct.NVIC_IRQChannelPreemptionPriority = 0;
	nvicStruct.NVIC_IRQChannelSubPriority = 0x0A;
	nvicStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvicStruct);
}

void init_led(void)
{
	GPIO_InitTypeDef gpioInitStr;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	gpioInitStr.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_12 | GPIO_Pin_14 | GPIO_Pin_15;
	gpioInitStr.GPIO_Mode = GPIO_Mode_OUT;
	gpioInitStr.GPIO_OType = GPIO_OType_PP;
	gpioInitStr.GPIO_PuPd = GPIO_PuPd_NOPULL;
	gpioInitStr.GPIO_Speed = GPIO_Speed_100MHz;

	GPIO_Init(GPIOD, &gpioInitStr);
    GPIO_WriteBit(GPIOD,  GPIO_Pin_13, Bit_RESET);

}





