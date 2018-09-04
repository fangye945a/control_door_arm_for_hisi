#include <stdio.h>
#include "App.h"
#include "App_sdk_control.h"
#include "sqlite3_database.h"
#include "serial.h"

extern volatile int open_door_flag;  //开门标志
extern volatile int upload_pic_flag;  //图片上传标志
extern volatile int shooting_flag;


extern CONFIG_INFO device_info;//设备配置信息


int TalksStream(void *pTalkData,unsigned int nTalkDataLen,void *pContext)
{
	AV_FRAME_HEAD	*pFrameHead = (AV_FRAME_HEAD *)pTalkData;
	EXT_FRAME_HEAD  *pExtFrame  = (EXT_FRAME_HEAD *)(pTalkData + sizeof(AV_FRAME_HEAD));

    int ret = 0;
	ret = App_Send_TalkData(pTalkData, pFrameHead->nByteNum);
	printf("get talk stream pFrameHead->nByteNum = %d\n", pFrameHead->nByteNum);
	printf("App_Send_TalkData == %d\n",ret);
}


int App_DoAlarmCreat(APP_ALARM_STATUS_OUTPUT *pAlarmMsg, int alarmState)
{
	printf("func = %s\n", __func__);
	if(alarmState)
	{
		printf("alarm creat..\n");
		printf("OutputStatus = %d MotionAlarm = %d, MotionAlarmRec = %d \n", pAlarmMsg->OutputStatus, pAlarmMsg->MotionAlarm
, pAlarmMsg->MotionAlarmRec[0]);
		printf("time = %d:%d:%d:%d\n", pAlarmMsg->day, pAlarmMsg->hour, pAlarmMsg->minute, pAlarmMsg->millsecond);
		printf("OutputStatus = %d SensorAlarm = %d, SensorAlarmRec = %d \n", pAlarmMsg->OutputStatus, pAlarmMsg->
SensorAlarm , pAlarmMsg->SensorAlarmRec[0]);
		printf("Motion flag = %d\n", pAlarmMsg->MotionAlarmRec[0]);
	}
	else
	{
		printf("alarm canner..\n");
		printf("OutputStatus = %d MotionAlarm = %d, MotionAlarmRec = %d \n", pAlarmMsg->OutputStatus, pAlarmMsg->MotionAlarm
, pAlarmMsg->MotionAlarmRec[0]);
		printf("time = %d:%d:%d:%d\n", pAlarmMsg->day, pAlarmMsg->hour, pAlarmMsg->minute, pAlarmMsg->millsecond);
		printf("OutputStatus = %d SensorAlarm = %d, SensorAlarmRec = %d \n", pAlarmMsg->OutputStatus, pAlarmMsg->
SensorAlarm , pAlarmMsg->SensorAlarmRec[0]);
		printf("Motion flag = %d\n", pAlarmMsg->MotionAlarmRec[0]);
	}
	return 0;
}

/*----------------------------------------------
* func name  : int App_MainStreamCallBack()
*introduce:	deal video or audio stream
* param:	
			pStreamData 	--> stream  callback func
			frameno 		-->  frame number
			sub               --->  0 main stream   1 sub stream
//建议: 码流不要在此回调函数里面处理， 可用一个缓冲区来存。
----------------------------------------------*/
int App_StreamCallBack(void *pStreamData, unsigned long frameno, int sub)
{
	AV_FRAME_HEAD	*pFrameHead = (AV_FRAME_HEAD *)pStreamData;
	EXT_FRAME_HEAD  *pExtFrame  = (EXT_FRAME_HEAD *)(pStreamData + sizeof(AV_FRAME_HEAD));
	char *pFrameData  = (char *)(pStreamData + sizeof(AV_FRAME_HEAD) + sizeof(EXT_FRAME_HEAD));
	//static int flag = 1;
	static int mlastTimestamp = 0, slastTimestamp = 0;
	//printf("head = %d, exthead = %d\n", sizeof(AV_FRAME_HEAD), sizeof(EXT_FRAME_HEAD));
	if (FRAME_DATA_A == pFrameHead->streamFlag)
	{
        if(sub == 0)
        {
        	printf("nByteNum = %d\n", pFrameHead->nByteNum);
			//App_Send_TalkData(pStreamData, pFrameHead->nByteNum);
        }
		return -1;
	}
	if(sub)
	{
		if((pExtFrame->nTimestamp - slastTimestamp) > 40)
		//	printf("sub slastTimestamp = %d, nTimestamp = %d\n", slastTimestamp, pExtFrame->nTimestamp);
		slastTimestamp = pExtFrame->nTimestamp;
	}
	else
	{
		if((pExtFrame->nTimestamp - mlastTimestamp) > 40)
		//	printf("main slastTimestamp = %d, nTimestamp = %d\n", mlastTimestamp, pExtFrame->nTimestamp);
		mlastTimestamp = pExtFrame->nTimestamp;
	}
	return 0;
	
}

void App_Init()
{
	int ret = -1;
	APP_NET_CONFIG stNetTest;
	APP_VIDEO_CONFIG stVideoConfig;
	APP_U32 len;
	APP_UL32 nAppend = 0;
	ret = App_start();
	ret = App_set_alarm_callback(App_DoAlarmCreat);
	printf("App_set_alarm_callback == %d\n",ret);
	
	//-----------------音视频流------------------//
	ret = App_set_stream_callback( App_StreamCallBack); //  stream
	printf("App_set_stream_callback == %d\n",ret);

	ret = App_StopMainStream();
	printf("App_StopMainStream == %d\n",ret);
	
	ret = App_StopSubStream();
	printf("App_StopSubStream == %d\n",ret);

	ret = App_maxstream_bufsize();
	printf("App_maxstream_bufsize = %d\n",ret);

	//------------------对讲----------------------//

	ret = App_Open_TalkRequsest(TalksStream, NULL);  //启动对讲
	printf("App_Open_TalkRequsest == %d\n",ret);

	ret = App_Stop_TalkRequsest();	//关闭对讲
	printf("App_Stop_TalkRequsest == %d\n",ret);

	ret = App_PTZControl(1, 0, 0);
	printf("\nApp_PTZControl == %d\n",ret);
}

int IO_Init() //IO口初始化
{
	IO_Control(DOOR_CONTROL,1);
	IO_Control(BEEP_CONTROL,0);
	IO_Control(RESET_CONTROL,0);
	usleep(1000*500);
	IO_Control(BEEP_CONTROL,1);  //开机蜂鸣器提示
	IO_Control(RESET_CONTROL,1);   //外围模块复位
	printf("IO_Init finish!!\n");
}

/***************************************************************
功能: 开门控制
****************************************************************/
void Open_Door() //开门
{
	IO_Control(DOOR_CONTROL,0);
	IO_Control(BEEP_CONTROL,0);  //开机蜂鸣器提示
	printf("-----Open door!!\n");
}

void Close_Door() //关门
{
	IO_Control(DOOR_CONTROL,1);
	IO_Control(BEEP_CONTROL,1);  //开机蜂鸣器提示
	printf("-----Close door!!\n");
}

/***************************************************************
功能:IO口控制  
Chn:通道号      0 报警输入         1 报警输出       2 复位引脚 
state:输出电平状态  0或1
return: 0 成功  other 失败 
****************************************************************/
int IO_Control(int Chn,int state) 
{
	int ret = -1;
	if(Chn != 0 && Chn != 1 && Chn != 2) //通道错误直接返回
	{
		printf("IO_Control: Chn Number Error!!\n");
		return ret;
	}
	if(state != 0 && state != 1)
	{
		printf("IO_Control: State Error!!\n");
		return ret;
	}
	
	switch(Chn)
	{
		case BEEP_CONTROL:
			ret = App_SetDeviceConfig(SET_IO3_OUT,NULL,0,state);//蜂鸣器
			break;
		case DOOR_CONTROL:
			ret = App_SetDeviceConfig(SET_OUTPUT_CTRL,NULL,0,state);//开门
			break;
		case RESET_CONTROL:
			ret = App_SetDeviceConfig(SET_IO2_OUT,NULL,0,state);      //刷卡模块复位
			break;
		default:break;
	}
	if(ret)
		printf("IO_Control: ret = %d  Sdk control fail!!\n",ret);
	return ret;	
}

int Shoot_Pictrue() //抓图
{
	char buf[1024*500] = {0}; //图像缓冲区大小
	APP_U32 len = 1024*500;	   //图片最大大小500K
	APP_UL32 nAppend = 0;
	int ret = App_GetDeviceConfig(GET_CAPTURE_PIC,buf,&len,& nAppend);
	if(ret != 0)
	{
		printf("SDK shooting fail!\n");
	}
	
	FILE *fd = NULL;
	fd = fopen(PIC_SAVE_PATH, "wb+");
	if(fd == NULL)
	{
		printf("Save pic fail! Please check the path:%s\n",PIC_SAVE_PATH);
		return -1;
	}
	fwrite(buf, 1, len, fd);
	fclose(fd);
	printf("Shooting picture finish...\n");
	upload_pic_flag = 1;
	return 0;
}

void *SDK_Task_1(void *argv)	//开门控制线程
{
	while(1) 
	{
		if(open_door_flag) //等待标志置位
		{
			unsigned long PrevMillis = millis(); //记录开门时间
			Open_Door(); //开门
			unsigned long currentMillis = millis();
			while((currentMillis - PrevMillis) < device_info.openingTime*100)//达到开门时间（单位100ms）
			{
				currentMillis = millis();
				usleep(1000*20);
			}
			Close_Door(); //关门
			open_door_flag = 0;
			shooting_flag = 1;	//抓图使能
		}
		usleep(1000*20);
	}
}

void *SDK_Task_2(void *argv)	//抓图控制线程
{
	while(1) 
	{
		if(shooting_flag) //等待抓图标志
		{
			Shoot_Pictrue();//抓图
			shooting_flag = 0;
		}
		usleep(1000*20);
	}
}

int SDK_Task_Init()
{
	pthread_t p_thread_1,p_thread_2;
	int ret = 0;
	ret = pthread_create(&p_thread_1, NULL, SDK_Task_1,NULL);   //开门控制线程
    if(ret != 0)
    {
    	printf("Error: Create SDK_Task_1 thread failed!\n");
		return -1;
    }else
		printf("Create SDK_Task_1 thread success!\n");

	ret = pthread_create(&p_thread_2, NULL, SDK_Task_2,NULL);	//抓图控制线程
    if(ret != 0)
    {
    	printf("Error: Create SDK_Task_2 thread failed!\n");
		return -1;
    }else
		printf("Create SDK_Task_2 thread success!\n");
	return 0;
}

