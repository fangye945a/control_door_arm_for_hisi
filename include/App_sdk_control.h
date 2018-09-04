#ifndef _APP_SDK_CONTROL_
#define _APP_SDK_CONTROL_

#define PIC_SAVE_PATH "shoot_pic.jpg"

enum CONTROL_NUM
{
	BEEP_CONTROL = 0,
	DOOR_CONTROL,
	RESET_CONTROL
};

int IO_Init(); //IO口初始化
void Open_Door(); //开门
int IO_Control(int Chn,int state); //IO口控制
int SDK_Task_Init(); //SDK任务初始化
#endif
