#include "tp.h"
#include "tpl_os.h"

int count = 0;

FUNC(int, OS_APPL_CODE) main(void)
{
  initBoard();
  RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
  TIM2->CR1 |= TIM_CR1_CEN;
  StartOS(OSDEFAULTAPPMODE);
  return 0;
}


TASK(task_manage)
{
  switch (count)
  {
  case 0:
    ActivateTask (task_green);
    break;

  case 4:
    ActivateTask (task_orange);
    break;

  case 5:
    ActivateTask (task_red);
    break;
  
  default:
    break;
  }
  count = (count + 1) % 10;

  ledOff(BLUE);
  delay(500);
  ledOn(BLUE);

  TerminateTask();
}

TASK(task_green)
{
  ledOff(ORANGE);
  ledOff(RED);

  ledOn(GREEN);

  TerminateTask();
}

TASK(task_orange)
{
  ledOff(RED);
  ledOff(GREEN);

  ledOn(ORANGE);

  TerminateTask();
}

TASK(task_red)
{
  ledOff(GREEN);
  ledOff(ORANGE);

  ledOn(RED);

  TerminateTask();
}

/*
 * This is necessary for ST libraries
 */
FUNC(void, OS_CODE) assert_failed(uint8_t* file, uint32_t line)
{
}

