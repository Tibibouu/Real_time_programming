#include "tp.h"
#include "tpl_os.h"
#include "tpl_com_definitions.h"
#include "acc/accelero.h"

#define SEUIL 100

typedef enum {
    INSTANTANE,
    DELAY,
} displayMode_t;

typedef struct s_position {
    int16_t x;
    int16_t y;
} position;


FUNC(int, OS_APPL_CODE) main(void)
{
	initBoard();
	ACC_init();
	StartOS(OSDEFAULTAPPMODE);
	return 0;
}

TASK(displayTask)
{
    static position pos;
    static EventMaskType event_got ;

    WaitEvent(ev_posChange);

    GetEvent(displayTask, &event_got);

    if (event_got & ev_posChange) {
        ClearEvent(ev_posChange);

        ReceiveMessage(msg_posChange, &pos);

        if(pos.x < -SEUIL) {
            ledOn(BLUE);
            ledOff(ORANGE);
        }
        else if(pos.x > SEUIL) {
            ledOn(ORANGE);
            ledOff(BLUE);
        }
        else {
            ledOff(BLUE);
            ledOff(ORANGE);
        }
        
        if(pos.y < -SEUIL) {
            ledOn(GREEN);
            ledOff(RED);
        }
        else if(pos.y > SEUIL) {
            ledOn(RED);
            ledOff(GREEN);
        }
        else {
            ledOff(GREEN);
            ledOff(RED);
        }
    }
	TerminateTask();
}

TASK(measureTask)
{
    static position pos;

    ACC_read();

    pos.x = ACC_getX();
    pos.y = ACC_getY();

    SendMessage(msg_posChange, &pos);

	TerminateTask();
}

TASK(stopDisplay)
{
    SetRelAlarm(al_display, 100, 100);

	TerminateTask();
}

ISR(isr_stopDisplay)
{
    static displayMode_t mode = INSTANTANE;

    if (mode == INSTANTANE) {
        CancelAlarm(al_display);

        ledOff(BLUE);
        ledOff(ORANGE);
        ledOff(GREEN);
        ledOff(RED);

        SetRelAlarm(al_stopDisplay, 5000, 0);

        mode = DELAY;
    }
    else {
        ActivateTask(stopDisplay);
        ActivateTask(clearMessages);

        mode = INSTANTANE;
    }
}

TASK(clearMessages)
{
    static position pos;
    StatusType conitnue;

    conitnue = ReceiveMessage(msg_posChange, &pos);

    while (conitnue != E_COM_NOMSG) {
        conitnue = ReceiveMessage(msg_posChange, &pos);
    }
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
    if (task_id == displayTask) {
    }
}

FUNC(void, OS_CODE) PostTaskHook()
{
    TaskType task_id = 0;
    GetTaskID(&task_id);
    if (task_id == displayTask) {
    }
}

