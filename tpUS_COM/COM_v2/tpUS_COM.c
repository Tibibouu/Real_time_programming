#include "tp.h"
#include "tpl_os.h"
#include "lcd/lcd.h"
#include "sr04/sr04.h"

typedef enum {
    ECHO_START,
    ECHO_STOP
} echo_t;

typedef struct s_myTime {
    int min;
    int sec;
} myTime_t;

echo_t echo = ECHO_START;


FUNC(int, OS_APPL_CODE) main(void)
{
	initBoard();
	lcdInit();
	USInit();
	StartOS(OSDEFAULTAPPMODE);
	return 0;
}

FUNC(void, OS_CODE) displayRange()
{
    static int range;

    ReceiveMessage(msg_rangeChange, &range);

    lcdSetLine(4);
    if (range == -1)
    {
        lcdPrintString("Error");
    }
    else {
        lcdPrintInt(range, 5);
    }
}

FUNC(void, OS_CODE) displayTime(int timeDisplay)
{
    static char myTimeStr[6] = "00:00";
    static myTime_t myTime;

    ReceiveMessage(msg_timeChange, &myTime);

    if (timeDisplay) {
        lcdSetPos(0, 14);

        myTimeStr[0] = myTime.min/10 + '0';
        myTimeStr[1] = myTime.min%10 + '0';
        myTimeStr[3] = myTime.sec/10 + '0';
        myTimeStr[4] = myTime.sec%10 + '0';

        lcdPrintString(myTimeStr);
    }
}

TASK(taskTime)
{
    static myTime_t myTime;

    myTime.sec++;

    if (myTime.sec == 60) {
        myTime.sec = 0;
        myTime.min++;
    }
    
    SendMessage(msg_timeChange, &myTime);

	TerminateTask();
}

TASK(displayTask)
{
    static int timeDisplay = 1;

    EventMaskType event_got ;

    displayTime(timeDisplay);

    while(1) {
        WaitEvent(ev_rangeChange | ev_timeChange | ev_stopTime);

        GetEvent(displayTask, &event_got);

        if (event_got & ev_rangeChange) {
            ClearEvent(ev_rangeChange);

            displayRange();
        }
        if (event_got & ev_timeChange) {
            ClearEvent(ev_timeChange);

            displayTime(timeDisplay);
        }
        if (event_got & ev_stopTime) {
            ClearEvent(ev_stopTime);

            timeDisplay = (timeDisplay + 1) % 2;

            lcdSetPos(0, 14);
            lcdPrintString("     ");

            displayTime(timeDisplay);
        }
    }
    
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
    static int range;

    range = -1;

    echo = ECHO_START;

    SendMessage(msg_rangeChange, &range);

	TerminateTask();
}

ISR(echoISR)
{
    static int range;

    CancelAlarm(activateTimeout);
    switch (echo)
    {
    case ECHO_START:
        USStartTimer();
        echo = ECHO_STOP;
        break;

    case ECHO_STOP:

        range = USReadTimer();

        SendMessage(msg_rangeChange, &range);

        echo = ECHO_START;
        break;
    
    default:
        break;
    }
}

ISR(buttonISR)
{
    SetEvent(displayTask, ev_stopTime);
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

