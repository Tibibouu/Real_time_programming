#include "tp.h"
#include "tpl_os.h"

typedef enum {
  NORMAL,
  MAINTENANCE
} maintenance_mode_t;

typedef enum {
  GREEN_TASK = 0,
  ORANGE_TASK = 4,
  RED_TASK = 5
} color_t;

color_t count = 0;
maintenance_mode_t maintenance_mode = NORMAL;
maintenance_mode_t old_maintenance_mode = NORMAL;
int maintenance_led = 0;

FUNC(int, OS_APPL_CODE) main(void)
{
  initBoard();
  RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
  TIM2->CR1 |= TIM_CR1_CEN;
  StartOS(OSDEFAULTAPPMODE);
  return 0;
}

ISR(isr_maintenance)
{
  maintenance_mode = (maintenance_mode + 1) % 2;
  count = 0;
}

TASK(task_manage)
{
  if(maintenance_mode != old_maintenance_mode)
  {
    resetLeds();
    old_maintenance_mode = maintenance_mode;
  }

  switch(maintenance_mode)
  {
  case NORMAL:
    switch(count)
    {
    case GREEN_TASK:
      ActivateTask (task_green);
      break;

    case ORANGE_TASK:
      ActivateTask (task_orange);
      break;

    case RED_TASK:
      ActivateTask (task_red);
      break;
    
    default:
      break;
    }
    count = (count + 1) % 10;

    ledOff(BLUE);
    delay(500);
    ledOn(BLUE);
    break;

  case MAINTENANCE:
    switch(maintenance_led)
    {
    case 0:
      ledOn(ORANGE);
      break;

    case 1:
      ledOff(ORANGE);
      break;
    
    default:
      break;
    }
    maintenance_led = (maintenance_led + 1) % 2;
    break;
  
  default:
    break;
  }
  

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

void resetLeds(void)
{
  ledOff(GREEN);
  ledOff(ORANGE);
  ledOff(RED);
  ledOff(BLUE);
}

/*
 * This is necessary for ST libraries
 */
FUNC(void, OS_CODE) assert_failed(uint8_t* file, uint32_t line)
{
}

