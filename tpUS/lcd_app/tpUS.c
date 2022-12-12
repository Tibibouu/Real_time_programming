#include "tp.h"
#include "tpl_os.h"
#include "lcd/lcd.h"
#include "sr04/sr04.h"

typedef enum {
    ECHO_START,
    ECHO_STOP
} echo_t;

echo_t echo = ECHO_START;

int myTime = 0;
int range;
int lcdVar;
int rangeToDisplay = 0;

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

    GetResource(resLcd);
    lcdSetPos(0, 14);
    lcdPrintString(myTimeStr);
    ReleaseResource(resLcd);

	TerminateTask();
}

TASK(displayTask)
{
    GetResource(resRange);
    rangeToDisplay = range;
    ReleaseResource(resRange);

    //lcdClear();

    GetResource(resLcd);
    lcdSetLine(4);
    if (range == -1)
    {
        lcdPrintString("Error");
    }
    else {
        lcdPrintInt(rangeToDisplay, 5);
    }
    ReleaseResource(resLcd);
    
	TerminateTask();
}

TASK(measureTask)
{
    USTrigger();


    SetRelAlarm(activateTimeout, 35, 0);

	TerminateTask();
}

TASK(timeoutTask)
{
    GetResource(resRange);
    range = -1;
    ReleaseResource(resRange);

    echo = ECHO_START;

	TerminateTask();
}

ISR(echoISR)
{
    CancelAlarm(activateTimeout);
    switch (echo)
    {
    case ECHO_START:
        USStartTimer();
        echo = ECHO_STOP;
        break;

    case ECHO_STOP:

        GetResource(resRange);
        range = USReadTimer();
        ReleaseResource(resRange);

        echo = ECHO_START;
        break;
    
    default:
        break;
    }
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
    if (task_id == displayTask) {
        ledOn(ORANGE);
    }
}

FUNC(void, OS_CODE) PostTaskHook()
{
    TaskType task_id = 0;
    GetTaskID(&task_id);
    if (task_id == displayTask) {
        ledOff(ORANGE);
    }
}

