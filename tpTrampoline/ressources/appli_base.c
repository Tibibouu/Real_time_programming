#include "tp.h"
#include "tpl_os.h"

volatile int val = 0;
int count = 0;

FUNC(int, OS_APPL_CODE) main(void)
{
  initBoard();
  RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
  TIM2->CR1 |= TIM_CR1_CEN;
  StartOS(OSDEFAULTAPPMODE);
  return 0;
}

TASK(T_affichage)
{
  GetResource(resVal);
  const int tmp = val ;
  ReleaseResource(resVal);

  if(tmp >= -1 && tmp <= 2) {
    ledOff(RED);
    ledOn(GREEN);
  }
  else {
    ledOff(GREEN);
    ledOn(RED);
  }

  TerminateTask();
}

TASK(T_fond)
{
  while(1) {
    GetResource(resVal);
    val++;
    ReleaseResource(resVal);


    GetResource(resVal);
    val--;
    ReleaseResource(resVal);
  }

  TerminateTask();
}

TASK(T_periode)
{
  count++;

  if(count % 2) {
    GetResource(resVal);
    val++;
    ReleaseResource(resVal);
  }
  else {
    GetResource(resVal);
    val--;
    ReleaseResource(resVal);
  }

  TerminateTask();
}

/*
 * This is necessary for ST libraries
 */
FUNC(void, OS_CODE) assert_failed(uint8_t* file, uint32_t line)
{
}

