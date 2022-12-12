#include "tp.h"
#include "tpl_os.h"

int test = 0;

FUNC(int, OS_APPL_CODE) main(void)
{
  initBoard();
  RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
  TIM2->CR1 |= TIM_CR1_CEN;
  StartOS(OSDEFAULTAPPMODE);
  return 0;
}

ISR(isr_a_task)
{
  // TODO: Insert code here
}

TASK(a_task)
{
  ActivateTask (task_1);
  ActivateTask (task_2);

  delay(500);
  ledOn(ORANGE);
  TerminateTask();
}

TASK(task_1)
{
  delay(500);
  ledOn(GREEN);
  TerminateTask();
}

TASK(task_2)
{
  delay(500);
  ledOn(RED);
  TerminateTask();
}

/*
 * This is necessary for ST libraries
 */
FUNC(void, OS_CODE) assert_failed(uint8_t* file, uint32_t line)
{
}

