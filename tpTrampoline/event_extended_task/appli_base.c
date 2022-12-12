#include "tp.h"
#include "tpl_os.h"

FUNC(int, OS_APPL_CODE) main(void)
{
  initBoard();
  RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
  TIM2->CR1 |= TIM_CR1_CEN;
  StartOS(OSDEFAULTAPPMODE);
  return 0;
}

TASK(T_master)
{
  EventMaskType event_got ;

  ActivateTask(T_1);
  ActivateTask(T_2);
  ActivateTask(T_3);
  
  while(1) {
    WaitEvent(ev_1 | ev_2 | ev_3);

    GetEvent(T_master, &event_got);

    if (event_got & ev_1) {
      ClearEvent(ev_1);
      ActivateTask(T_1);
    }
    else if (event_got & ev_2) {
      ClearEvent(ev_2);
      ActivateTask(T_2);
    }
    else if (event_got & ev_3) {
      ClearEvent(ev_3);
      ActivateTask(T_3);
    }
  }

  TerminateTask ();
}

TASK(T_1)
{
  ledOn(ORANGE);
  delay(500);
  ledOff(ORANGE);
  delay(500);

  SetEvent(T_master, ev_1);

  TerminateTask();
}

TASK(T_2)
{
  ledOn(GREEN);
  delay(500);
  ledOff(GREEN);
  delay(500);

  SetEvent(T_master, ev_2);

  TerminateTask();
}

TASK(T_3)
{
  ledOn(RED);
  delay(500);
  ledOff(RED);
  delay(500);

  SetEvent(T_master, ev_3);

  TerminateTask();
}

/*
 * This is necessary for ST libraries
 */
FUNC(void, OS_CODE) assert_failed(uint8_t* file, uint32_t line)
{
}

