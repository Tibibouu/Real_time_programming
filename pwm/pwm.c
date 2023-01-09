#include "tp.h"
#include "tpl_os.h"

#define PWM_PERIODE_MAX 20

static int dutyCycle = 0;

FUNC(int, OS_APPL_CODE) main(void)
{
  initBoard();
  RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
  TIM2->CR1 |= TIM_CR1_CEN;
  StartOS(OSDEFAULTAPPMODE);
  return 0;
}

TASK(commandTask)
{
  if (dutyCycle == 100)
  {
    dutyCycle = 0;
  }
  else {
    dutyCycle = dutyCycle + 10;
  }


  TerminateTask();
}

TASK(pwmTask)
{
  static int pwmOn = 0;

  if (dutyCycle == 0)
  {
    pwmOn = 0;

    ledOff(RED);
  }
  else if (dutyCycle == 100)
  {
    pwmOn = 1;

    ledOn(RED);
  }
  else {
    if (!pwmOn)
    {
      int periode = PWM_PERIODE_MAX * dutyCycle / 100;
      SetRelAlarm(al_pwmWakeUp, periode, 0);

      pwmOn = 1;

      ledOn(RED);
    }
    else
    {
      pwmOn = 0;

      ledOff(RED);
    }
  }

  TerminateTask();
}

/*
 * This is necessary for ST libraries
 */
FUNC(void, OS_CODE) assert_failed(uint8_t* file, uint32_t line)
{
}

