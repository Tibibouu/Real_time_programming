#include "sr04.h"
#include "stm32f4xx.h"
#include "stm32f4xx_tim.h"

TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

void USInit() {
	//configure GPIO for trigger
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(TRIG_PERIPH, ENABLE);
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Pin = TRIG_PIN;
	GPIO_Init(TRIG_PORT, &GPIO_InitStructure);

	//no need to configure GPIO for echo, 
	//this is automatically done by Trampoline

	//configure free running timer TIM2@1us
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	TIM_TimeBaseStructure.TIM_Prescaler = 84 - 1; //config 1 tick/us
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = 0xFFFFFFFF;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	TIM_Cmd(TIM2, ENABLE);  
}

void USTrigger() {
	volatile int i;
	//at least 10 us
	GPIO_SetBits(TRIG_PORT, TRIG_PIN);
	for(i=0;i<10000;i++); //TODO: affiner
	GPIO_ResetBits(TRIG_PORT, TRIG_PIN);
}

void USStartTimer()
{
	TIM_SetCounter(TIM2,0);
}

uint32_t USReadTimer() 
{
	return TIM_GetCounter(TIM2);
}
