#ifndef __RFID_SERIAL_H__
#define __RFID_SERIAL_H__

//宏定义
#define FALSE  -1
#define TRUE   0
#define SERIAL_BUFF_MAX_LEN 1024  /*串口缓冲区大小  1K */
#define SERIAL_DEVICE_NAME "/dev/ttyS000"
#define RFID_DETECT_CYCLE  300//读卡模块检测卡片周期 300ms
#define SAME_DETECT_CYCLE  2000//相同卡片读卡周期   2s
#define MAX_LEN_RFID  256


extern char RFID_buff[MAX_LEN_RFID];
typedef struct
{
	char* clientid;
	char* delimiter;
	int maxdatalen;
	int qos;
	int retained;
	char* username;
	char* password;
	char* host;
	char* port;
	int verbose;
	int keepalive;
}ClietParam;


//函数声明
int Serial_Task_Init();

void millis_Init();

int RFID_Init();

void card_detect();  //卡片检测

int Serial_Avaliable();

char Serial_Read();

void Card_task(int len);

void Serial_Clean();   //清空串口缓冲区

void delay(int time);

#endif
