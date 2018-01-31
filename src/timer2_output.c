
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
	nvicStruct.NVIC_IRQChannelPreemptionPriority = PRIO_TIMER2;
	nvicStruct.NVIC_IRQChannelSubPriority = 0x00;
	nvicStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvicStruct);
}






