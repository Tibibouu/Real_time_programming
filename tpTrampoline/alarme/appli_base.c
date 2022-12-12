#include "tp.h"
#include "tpl_os.h"

typedef enum {
  NORMAL,
  REVERSE
} direction_t;

direction_t direction = NORMAL;

FUNC(int, OS_APPL_CODE) main(void)
{
  initBoard();
  RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
  TIM2->CR1 |= TIM_CR1_CEN;
  StartOS(OSDEFAULTAPPMODE);
  return 0;
}

ISR(isr_change_direction)
{
  direction = (direction == NORMAL) ? REVERSE : NORMAL;

  CancelAlarm(alarm_orange);
  CancelAlarm(alarm_green);
  CancelAlarm(alarm_blue);
  CancelAlarm(alarm_red);

  switch (direction) {
  case NORMAL:
      SetRelAlarm(alarm_orange, 250, 1000);
      SetRelAlarm(alarm_green, 500, 1000);
      SetRelAlarm(alarm_blue, 750, 1000);
      SetRelAlarm(alarm_red, 1000, 1000);
    break;

  case REVERSE:
      SetRelAlarm(alarm_red, 250, 1000);
      SetRelAlarm(alarm_blue, 500, 1000);
      SetRelAlarm(alarm_green, 750, 1000);
      SetRelAlarm(alarm_orange, 1000, 1000);
    break;
  
  default:
    break;
  }
}

TASK(T_chase)
{
  EventMaskType event_got ;

  while(1) {
    WaitEvent(ev_orange | ev_green | ev_blue | ev_red);

    GetEvent(T_chase, &event_got);

    ledOff(ORANGE);
    ledOff(GREEN);
    ledOff(BLUE);
    ledOff(RED);

    if (event_got & ev_orange) {
      ClearEvent(ev_orange);

      ledOn(ORANGE);
    }
    else if (event_got & ev_green) {
      ClearEvent(ev_green);
      
      ledOn(GREEN);
    }
    else if (event_got & ev_blue) {
      ClearEvent(ev_blue);
      
      ledOn(BLUE);
    }
    else if (event_got & ev_red) {
      ClearEvent(ev_red);
      
      ledOn(RED);
    }
  }

  delay(500);
  ledOn(ORANGE);
  TerminateTask();
}

TASK(task_orange)
{
  ledOff(GREEN);
  ledOff(BLUE);
  ledOff(RED);

  ledOn(ORANGE);

  TerminateTask();
}

/*
 * This is necessary for ST libraries
 */
FUNC(void, OS_CODE) assert_failed(uint8_t* file, uint32_t line)
{
}

