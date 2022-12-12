#ifndef __SR04_H__
#define __SR04_H__
#include "stm32f4xx.h"


//TRIG PE2
#define TRIG_PERIPH RCC_AHB1Periph_GPIOE
#define TRIG_PORT   GPIOE
#define TRIG_PIN    GPIO_Pin_2

//ECHO PE3
#define ECHO_PERIPH RCC_AHB1Periph_GPIOE
#define ECHO_PORT   GPIOE
#define ECHO_PIN    GPIO_Pin_3

//should be called before anything else
void USInit();


//send a trigger pulse (polling)
void USTrigger();

//start the free running timer (TIM2) to measure the echo duration
void USStartTimer();

//get the current value of the timer.
uint32_t USReadTimer();

#endif
