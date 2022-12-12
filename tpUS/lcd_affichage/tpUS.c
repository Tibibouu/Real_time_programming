#include "tp.h"
#include "tpl_os.h"
#include "lcd/lcd.h"
#include "sr04/sr04.h"

volatile int myTime = 0;

FUNC(int, OS_APPL_CODE) main(void)
{
	initBoard();
	lcdInit();
	USInit();
	StartOS(OSDEFAULTAPPMODE);
	return 0;
}

TASK(taskTime)
{
    myTime++;

    char myTimeStr[6] = "00:00";
    myTimeStr[0] = myTime/60/10 + '0';
    myTimeStr[1] = myTime/60%10 + '0';
    myTimeStr[3] = myTime%60/10 + '0';
    myTimeStr[4] = myTime%60%10 + '0';

    //lcdClear();
    lcdSetPos(0, 14);
    
    lcdPrintString(myTimeStr);

	TerminateTask();
}
/*
 *  * This is necessary for ST libraries
 *   */
FUNC(void, OS_CODE) assert_failed(uint8_t* file, uint32_t line)
{
}

FUNC(void, OS_CODE) PreTaskHook()
{
    TaskType task_id = 0;
    GetTaskID(&task_id);
    if (task_id == taskTime) {
        ledOn(ORANGE);
    }
}

FUNC(void, OS_CODE) PostTaskHook()
{
    TaskType task_id = 0;
    GetTaskID(&task_id);
    if (task_id == taskTime) {
        ledOff(ORANGE);
    }
}

