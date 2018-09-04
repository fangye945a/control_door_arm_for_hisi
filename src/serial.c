//串口相关的头文件
#include<stdio.h>      /*标准输入输出定义*/
#include<stdlib.h>     /*标准函数库定义*/
#include<unistd.h>     /*Unix 标准函数定义*/
#include<sys/ioctl.h>
#include<sys/types.h>
#include<sys/time.h>

#include<sys/stat.h>
#include<fcntl.h>      /*文件控制定义*/
#include<termios.h>    /*PPSIX 终端控制定义*/
#include<errno.h>      /*错误号定义*/
#include<string.h>
#include "MQTTAsync.h"
#include "sqlite3_database.h"
#include "serial.h"

char RFID_buff[MAX_LEN_RFID]={0};
unsigned char serial_rx_buff[SERIAL_BUFF_MAX_LEN]={0};  //接受缓冲区
const int baudrate = 115200; //波特率
int serial_fd = -1;
static int rx_start = 0;
static int rx_end = 0;
static int start_second = 0;


//const unsigned char rfid_init[] = {0x20,0x20,0x20,0x20,0x20,0xC1,0x41,0x00,0x79,0x03};//rfid波特率检测初始化命令
const unsigned char rfid_init[] = {0x20};
const unsigned char mifare_init[] =  {0x07,0x31,0x44,0x01,0x00,0x8C,0x03};            //mifare初始化命令
const unsigned char mifare_detect[] ={0x08,0x42,0x4D,0x02,0x00,0x26,0xDC,0x03};  //发送mifare检测命令
const unsigned char typeb_init[] =   {0x07,0x01,0x44,0x01,0x04,0xB8,0x03};            //typeb初始化命令
const unsigned char typeb_detect[] = {0x07,0x06,0x54,0x01,0x08,0xA3,0x03};            //typeb检测命令

unsigned char last_detect_type = 0;     //上次读卡检测类型  //0 S50卡   //1 身份证
unsigned long last_detect_time = 0;     //上次读卡检测时间
unsigned char detect_card_id[32] = {0}; //上次检测到卡片的ID
unsigned long last_card_time = 0;       //上次检测到卡片时间

extern MQTTAsync client;
extern MQTTAsync_responseOptions ropts;
extern ClietParam opts;
extern CONFIG_INFO device_info;
extern char buffer[128];//消息数组
extern volatile int open_door_flag;  //开门标志

/*******************************************************************
* 名称：                  UART0_Open
* 功能：                打开串口并返回串口设备文件描述
* 入口参数：        fd    :文件描述符     port :串口号(ttyS0,ttyS1,ttyS2)
* 出口参数：        正确返回为1，错误返回为0
*******************************************************************/
int UART0_Open(const char* port)
{

    int fd = open(port, O_RDWR|O_NOCTTY|O_NDELAY);
    if( FALSE == fd )
    {
        perror("Can't Open Serial Port");
        return FALSE;
    }
	
    //恢复串口为阻塞状态
    if(fcntl(fd, F_SETFL, 0) < 0)
    {
        printf("fcntl failed!\n");
        return(FALSE);
    }
    else
    {
        printf("fcntl=%d\n",fcntl(fd, F_SETFL,0));
    }
	
    //测试是否为终端设备
    /*if(0 == isatty(STDIN_FILENO))
    {
        printf("standard input is not a terminal device\n");
        return(FALSE);
    }
    else
    {
        printf("Serial init success! fd = %d\n",fd);
    }*/
    printf("Serial init success! fd = %d\n",fd);
    return fd;
}
/*******************************************************************
* 名称：                UART0_Close
* 功能：                关闭串口并返回串口设备文件描述
* 入口参数：        fd    :文件描述符     port :串口号(ttyS0,ttyS1,ttyS2)
* 出口参数：        void
*******************************************************************/

void UART0_Close(int fd)
{
    close(fd);
}

/*******************************************************************
* 名称：                UART0_Set
* 功能：                设置串口数据位，停止位和效验位
* 入口参数：        fd        串口文件描述符
*                              speed     串口速度
*                              flow_ctrl   数据流控制
*                           databits   数据位   取值为 7 或者8
*                           stopbits   停止位   取值为 1 或者2
*                           parity     效验类型 取值为N,E,O,,S
*出口参数：          正确返回为1，错误返回为0
*******************************************************************/
int UART0_Set(int fd,int speed,int flow_ctrl,int databits,int stopbits,int parity)
{

    int   i;
    int   status;
    int   speed_arr[] = { B115200, B19200, B9600, B4800, B2400, B1200, B300};
    int   name_arr[] = {115200,  19200,  9600,  4800,  2400,  1200,  300};

    struct termios options;

    /*tcgetattr(fd,&options)得到与fd指向对象的相关参数，并将它们保存于options,该函数还可以测试配置是否正确，该串口是否可用等。若调用成功，函数返回值为0，若调用失败，函数返回值为1.
    */
    if( tcgetattr(fd,&options)  !=  0)
    {
        perror("SetupSerial 1");
        return(FALSE);
    }

    //设置串口输入波特率和输出波特率
    for ( i= 0;  i < sizeof(speed_arr) / sizeof(int);  i++)
    {
        if  (speed == name_arr[i])
        {
            cfsetispeed(&options, speed_arr[i]);
            cfsetospeed(&options, speed_arr[i]);
        }
    }

    //修改控制模式，保证程序不会占用串口
    options.c_cflag |= CLOCAL;
    //修改控制模式，使得能够从串口中读取输入数据
    options.c_cflag |= CREAD;

    //设置数据流控制
    switch(flow_ctrl)
    {

    case 0 ://不使用流控制
        options.c_cflag &= ~CRTSCTS;
        break;

    case 1 ://使用硬件流控制
        options.c_cflag |= CRTSCTS;
        break;
    case 2 ://使用软件流控制
        options.c_cflag |= IXON | IXOFF | IXANY;
        break;
    }
    //设置数据位

    //屏蔽其他标志位
    options.c_cflag &= ~CSIZE;
    switch (databits)
    {
    case 5    :
        options.c_cflag |= CS5;
        break;
    case 6    :
        options.c_cflag |= CS6;
        break;
    case 7    :
        options.c_cflag |= CS7;
        break;
    case 8:
        options.c_cflag |= CS8;
        break;
    default:     fprintf(stderr,"Unsupported data size\n");
        return (FALSE);
    }
    //设置校验位
    switch (parity)
    {
    case 'n':
    case 'N': //无奇偶校验位。
        options.c_cflag &= ~PARENB;
        options.c_iflag &= ~INPCK;
        break;
    case 'o':
    case 'O'://设置为奇校验
        options.c_cflag |= (PARODD | PARENB);
        options.c_iflag |= INPCK;
        break;
    case 'e':
    case 'E'://设置为偶校验
        options.c_cflag |= PARENB;
        options.c_cflag &= ~PARODD;
        options.c_iflag |= INPCK;
        break;
    case 's':
    case 'S': //设置为空格
        options.c_cflag &= ~PARENB;
        options.c_cflag &= ~CSTOPB;
        break;
    default:
        fprintf(stderr,"Unsupported parity\n");
        return (FALSE);
    }
    // 设置停止位
    switch (stopbits)
    {
    case 1:
        options.c_cflag &= ~CSTOPB; break;
    case 2:
        options.c_cflag |= CSTOPB; break;
    default:
        fprintf(stderr,"Unsupported stop bits\n");
        return (FALSE);
    }

    //修改输出模式，原始数据输出
    options.c_oflag &= ~OPOST;

    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

	options.c_lflag &= ~(ISIG | ICANON);
	options.c_lflag &= ~(ONLCR | OCRNL);  //防止输出映射处理
    //设置等待时间和最小接收字符
    options.c_cc[VTIME] = 1; /* 读取一个字符等待1*(1/10)s */
    options.c_cc[VMIN] = 1; /* 读取字符的最少个数为1 */

    //如果发生数据溢出，接收数据，但是不再读取 刷新收到的数据但是不读
    tcflush(fd,TCIFLUSH);

    //激活配置 (将修改后的termios数据设置到串口中）
    if (tcsetattr(fd,TCSANOW,&options) != 0)
    {
        perror("com set error!\n");
        return (FALSE);
    }
    return (TRUE);
}
/*******************************************************************
* 名称：                UART0_Init()
* 功能：                串口初始化
* 入口参数：        fd       :  文件描述符
*               speed  :  串口速度
*                              flow_ctrl  数据流控制
*               databits   数据位   取值为 7 或者8
*                           stopbits   停止位   取值为 1 或者2
*                           parity     效验类型 取值为N,E,O,,S
*
* 出口参数：        正确返回为1，错误返回为0
*******************************************************************/
int UART0_Init(int fd, int speed,int flow_ctrl,int databits,int stopbits,int parity)
{
    int err;
    //设置串口数据帧格式
    if (UART0_Set(fd,speed,flow_ctrl,databits,stopbits,parity) == FALSE)
    {
        return FALSE;
    }
    else
    {
        return  TRUE;
    }
}

/*******************************************************************
* 名称：                  UART0_Recv
* 功能：                接收串口数据
* 入口参数：        fd                  :文件描述符
*                              rcv_buf     :接收串口中数据存入rcv_buf缓冲区中
*                              data_len    :一帧数据的长度
* 出口参数：        正确返回为1，错误返回为0
*******************************************************************/
int UART0_Recv(int fd, char *rcv_buf,int data_len)
{
    int len,fs_sel;
    fd_set fs_read;

    struct timeval time;

    FD_ZERO(&fs_read);
    FD_SET(fd,&fs_read);

    time.tv_sec = 10;
    time.tv_usec = 0;


    //使用select实现串口的多路通信
    fs_sel = select(fd+1,&fs_read,NULL,NULL,&time);
  //  printf("fs_sel = %d\n",fs_sel);
    if(fs_sel)
    {
        len = read(fd,rcv_buf,data_len);
        return len;
    }
    else
    {
        return FALSE;
    }
}
/********************************************************************
* 名称：UART0_Send
* 功能：   发送数据
* 入口参数：     fd:文件描述符
*           send_buf    :存放串口发送数据
*           data_len    :一帧数据的个数
* 出口参数：        正确返回为1，错误返回为0
*******************************************************************/
int UART0_Send(int fd, const unsigned char *send_buf,int data_len)
{
    int len = 0;

    len = write(fd,send_buf,data_len);
    if (len == data_len )
    {
        return len;
    }
    else
    {

        tcflush(fd,TCOFLUSH);
        return FALSE;
    }

}
void close_console()
{
    printf("change console!!\n");
    int fp = open("/dev/ttyAMA1",O_RDONLY); // 改变console
    ioctl(fp,TIOCCONS);
    close(fp);
}


void *Serial_Rx_Task(void *argv)
{
	int len = 0;
	while (1) //循环接收数据
    {
    	unsigned char rx = 0;
		len = UART0_Recv(serial_fd, &rx,1); //接收一个字节数据到缓冲区
		//printf("---Rx_Task\n");
        if(len == 1)
        {
        	if( (rx_end + 1)%SERIAL_BUFF_MAX_LEN == rx_start )   //缓冲区满则丢掉数据
				continue;
        	serial_rx_buff[rx_end++] = rx;
			rx_end %= SERIAL_BUFF_MAX_LEN;
        }
    }
}

int Serial_Avaliable()
{
	int len = 0;
	len = rx_end - rx_start;
	if(len < 0)
		len += SERIAL_BUFF_MAX_LEN;
	return len;
}

char Serial_Read()
{
	char ch = -1;
	if(rx_end == rx_start)   //缓冲区无数据
		return ch;
	ch = serial_rx_buff[rx_start++];
	rx_start %= SERIAL_BUFF_MAX_LEN;
	return ch;
}

void Serial_Clean()   //清空串口缓冲区
{
	rx_start = 0;
	rx_end = 0;
	memset(serial_rx_buff,0,SERIAL_BUFF_MAX_LEN);
}

int Serial_Write(const unsigned char *str,int  len)
{
	if(str == NULL)
		return FALSE;
	return  UART0_Send(serial_fd,str,len);
}
int Serial_Task_Init()
{
	int err = 0;
	close_console(); //将调试串口转换为工作串口
	serial_fd = UART0_Open(SERIAL_DEVICE_NAME); //打开串口，返回文件描述符
	if(serial_fd < 0)
		return -1;
	
	UART0_Init(serial_fd,baudrate,0,8,1,'N');

	pthread_t p_thread;
	int ret = 0;
	ret = pthread_create(&p_thread, NULL, Serial_Rx_Task, NULL);
    if(ret != 0)
    {
    	printf("Error: Create Serial_Rx_Task thread failed!\n");
		return -1;
    }else	
		printf("Create Serial_Rx_Task thread success!\n");
	
	//UART0_Close(serial_fd);
	return 0;
}

void delay(int time)
{
	usleep(1000*time);
}

void millis_Init()
{
	struct timeval start_time; 
	gettimeofday(&start_time,NULL); 
	start_second = start_time.tv_sec;
	printf("---- start_second = %d\n",start_second);
}
unsigned long millis()   //当前毫秒数
{
	struct timeval now_time; 
	unsigned long cur_time_ms = 0; 
	gettimeofday(&now_time,NULL); 
	cur_time_ms=1000*(now_time.tv_sec - start_second) + (now_time.tv_usec)/1000;
	return cur_time_ms;
}

void card_detect()  //卡片检测
{
    if( (millis() - last_detect_time) > RFID_DETECT_CYCLE)
    {
    	//printf("card_detect!!\n");
        if(last_detect_type)  //上一次读卡的类型
        {
            Serial_Write(mifare_init,sizeof(mifare_init));
            delay(30);
            Serial_Write(mifare_detect,sizeof(mifare_detect));
            last_detect_type = 0;
            last_detect_time = millis();
        }
        else
        {
            Serial_Write(typeb_init,sizeof(typeb_init));
            delay(30);
            Serial_Write(typeb_detect,sizeof(typeb_detect));
            last_detect_type = 1;
            last_detect_time = millis();
        }
    }
}

int get_Card_Id(char *str,int len,char *id)
{
  if(len == 0x0E)
  {
      unsigned char id_table[8]={0};  // 4 8
      unsigned char i=0;
      if(str[1] == 0x06)
      {
        strncpy(id_table,&str[4],8);
        sprintf(id,"%02X%02X%02X%02X%02X%02X%02X%02X",id_table[0],id_table[1],id_table[2],id_table[3],
                                                      id_table[4],id_table[5],id_table[6],id_table[7]);
      }
      else
      {
        strncpy(id_table,&str[8],4);
        sprintf(id,"%02X%02X%02X%02X",id_table[3],id_table[2],id_table[1],id_table[0]);
      }

      if( !strcmp(detect_card_id, id))  //如果与上次检测结果相同 则认为是没有拿开
      {
          if((millis() - last_card_time) > SAME_DETECT_CYCLE)  //如果与上一次检测时间差大于相同卡片读卡周期
          {
                strcpy(detect_card_id,id);  //记录此次读卡的卡ID
                last_card_time = millis();//记录此次读卡的时间
 
                return  1;
          }
          else
          {    
                 return 0; 
          }
      }
      else  //与上次结果不同
      {
          strcpy(detect_card_id,id);  //记录此次读卡的卡ID
          last_card_time = millis();//记录此次读卡的时间
          return 1;
      }
  }
  else
  {
      printf("---- Get card id error!");
      return 0;
  }
}

void Card_task(int len)
{
    char id[20]={0};//卡片ID号字符串十六进制形式
    int ret = get_Card_Id(RFID_buff, len, id);  //获取ID
    if(ret > 0) //获取ID成功
    { 
    	printf("push Card ID: -- %s\n",id);
		int state = sqlite_find_card("card",id,1); //是否有开门权限
		if(state == 1)  //有权限
		{
			sprintf(buffer,"{\"devId\":\"%s\",\"card\":%s,\"state\":%d}",device_info.devId,id,state);
			printf("%s\n",buffer);
			open_door_flag = 1;//开门
		}
		else			//无权限
		{
			state = 0;
			sprintf(buffer,"{\"devId\":\"%s\",\"card\":%s,\"state\":%d}",device_info.devId,id,state);
			printf("%s\n",buffer);
		}
		
		if(1)		//在线
		{
			
			MQTTAsync_send(client, "zx/door/openlog", strlen(buffer), buffer, opts.qos, opts.retained, &ropts);
			
		}else      //离线则保存刷卡信息
		{
			saveCardRecord(id,state);//保存离线刷卡信息
		}
	}
}

int RFID_Init()
{
   last_detect_time == millis(); //初始化检测上一次卡片检测时间
   last_detect_type = 1;  //上次检测卡片的类型
   
   printf("Serial_Write!!\n");
   Serial_Write(rfid_init,sizeof(rfid_init));
   delay(100);
   
   printf("Serial_Write!!\n");
   Serial_Write(rfid_init,sizeof(rfid_init));
   delay(100);
   
   while(Serial_Avaliable() > 0)
   {
      
     if( Serial_Read()  == 0x06 )
     {
		printf("RFID First init success!!");
		delay(100);
		return 0;
     }
  }
  printf("RFID_init success!!");
  return 1;
}
