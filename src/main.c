#include "MQTTAsync.h"

#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>

#if defined(WIN32)
#include <windows.h>
#define sleep Sleep
#else
#include <unistd.h>
#include <sys/time.h>
#include <unistd.h>
#endif

#include <OsWrapper.h>

#include <time.h>
#include "App_sdk_control.h"
#include "cJSON.h"
#include "sqlite3_database.h"
#include "common.h"
#include "serial.h"


volatile int finished = 0;
volatile int open_door_flag = 0;
volatile int shooting_flag = 0;
volatile int upload_pic_flag = 0;

volatile int toStop = 0;
char topic[64] = {0};
char buffer[128]={0};//消息数组
MQTTAsync client;
int subscribed = 0;
MQTTAsync_responseOptions ropts = MQTTAsync_responseOptions_initializer;


CONFIG_INFO device_info;  //设备配置信息

ClietParam opts =
{
	DEVICE_ID, "\n", 100, QoS, 0, "zx_admin", "ZxNumber1", "106.14.26.130", "1995", 0, 10
};

void cfinish(int sig)
{
	signal(SIGINT, NULL);
	toStop = 1;
}

static int published = 0;
static int disconnected = 0;
static int connected = 0;

void onPublishFailure(void* context, MQTTAsync_failureData* response)
{
	printf("Publish failed, rc %d\n", response ? -1 : response->code);
	published = -1;
}


void onPublish(void* context, MQTTAsync_successData* response)
{
	published = 1;
}

void onDisconnect(void* context, MQTTAsync_successData* response)
{
	disconnected = 1;
}
void myconnect(MQTTAsync* client);


int messageArrived(void* context, char* topicName, int topicLen, MQTTAsync_message* message)
{
	/* not expecting any messages */
	//MQTTAsync client = (MQTTAsync)context;
	printf("%s\t", topicName);
	printf("%.*s\n", message->payloadlen, (char*)message->payload);
	
	const cJSON *pOptcodeJson = NULL;

	cJSON *pMsgJson = cJSON_Parse(message->payload);

	pOptcodeJson = cJSON_GetObjectItemCaseSensitive(pMsgJson, "optcode");

	if(cJSON_IsNumber(pOptcodeJson))
	{
		printf("optcode is %d\n",	pOptcodeJson->valueint);
		switch(pOptcodeJson->valueint)
		{
			case 1:
			{
				open_door_flag = 1;
				break;
			}
			case 2:		//单条黑白名单操作
			{
				whiteBlackListOpt(pMsgJson);  
				break;
			}
			case 3:		//临时卡功能使能设置
			{
				pOptcodeJson = cJSON_GetObjectItemCaseSensitive(pMsgJson, "type"); //解析type 字段
				if(pOptcodeJson == NULL)
				{
					printf("Fuction:%s LINE:%d  error:pMsgJson analyze fail!!\n",__FUNCTION__,__LINE__);
					break;
				}else{
					int type = pOptcodeJson->valueint;
					if(type != 0 && type != 1)	//不符合规定则不处理
						break;
					device_info.tempCardFlag = type;
					setDeviceParam(&device_info);
				}
				break;
			}
			case 4:   //设置开门时长
			{
				pOptcodeJson = cJSON_GetObjectItemCaseSensitive(pMsgJson, "time"); //解析time 字段
				if(pOptcodeJson == NULL)
				{
					printf("Fuction:%s LINE:%d  error:pMsgJson analyze fail!!\n",__FUNCTION__,__LINE__);
					break;
				}else{
					int opentime = pOptcodeJson->valueint;
					device_info.openingTime = opentime;
					setDeviceParam(&device_info);
				}
				break;
			}
			case 7:	//同步服务器时间
			{
				pOptcodeJson = cJSON_GetObjectItemCaseSensitive(pMsgJson,"time"); //解析time 字段
				if(pOptcodeJson == NULL)
				{
					printf("Fuction:%s LINE:%d  error:pMsgJson analyze fail!!\n",__FUNCTION__,__LINE__);
					break;
				}else{
					time_t server_time = pOptcodeJson->valueint;
					timeSynchronization(server_time);//时间同步
				}
				break;
			}
			case 8: //设置持续开门报警时长
			{
				pOptcodeJson = cJSON_GetObjectItemCaseSensitive(pMsgJson, "optim"); //解析time 字段
				if(pOptcodeJson == NULL)
				{
					printf("Fuction:%s LINE:%d  error:pMsgJson analyze fail!!\n",__FUNCTION__,__LINE__);
					break;
				}else{
					int alarm_time = pOptcodeJson->valueint;
					device_info.alarmTime = alarm_time;
					setDeviceParam(&device_info);
				}
				break;
			}
			case 9: //批量黑白名单操作
			{
				batch_WhiteBlackListOpt(pMsgJson);  
				break;
			}
			case 110: //上报上线消息
			{
				sprintf(buffer,"{\"devId\":\"%s\",\"state\":1,\"signal\":%d}",device_info.devId,30); 
				printf("push online message  -- %s\n",buffer);
				MQTTAsync_send(client, "zx/door/firstword", strlen(buffer), buffer, opts.qos, opts.retained, &ropts);
				break;
			}
			case 120: //上报上线消息
			{
				toStop = 2;
				break;
			}
			default:
				break;
		}
	}
	else
		printf("Fuction:%s LINE:%d  error:pMsgJson analyze fail!!\n",__FUNCTION__,__LINE__);
	

	fflush(stdout);
	cJSON_Delete(pMsgJson);
	MQTTAsync_freeMessage(&message);
	MQTTAsync_free(topicName);
	return 1;
}


void onConnectFailure(void* context, MQTTAsync_failureData* response)
{
	printf("Connect failed, rc %d\n", response ? response->code : -1);
	connected = -1;

	MQTTAsync client = (MQTTAsync)context;
	myconnect(client);
}

void onSubscribe(void* context, MQTTAsync_successData* response)
{
	subscribed = 1;
}


void onSubscribeFailure(void* context, MQTTAsync_failureData* response)
{
	printf("Subscribe failed, rc %d\n", response->code);
	finished = 1;
}

void onConnect(void* context, MQTTAsync_successData* response)
{
	printf("Connected\n");
	MQTTAsync client = (MQTTAsync)context;
	
	int rc;

	//topic = "abc11";
	
	ropts.onSuccess = onSubscribe;
	ropts.onFailure = onSubscribeFailure;
	ropts.context = client;

	sprintf(topic,"zx/door/opt/%s",device_info.devId);
	printf("Subscribing to topic %s with client %s at QoS %d\n",topic, opts.clientid, opts.qos);

	rc = MQTTAsync_subscribe(client, topic, opts.qos, &ropts);
	if(rc != MQTTASYNC_SUCCESS)
	{
		printf("Failed to start subscribe, return code %d\n", rc);
		finished = 1;
	}

//	char *const topics[] = {"zx/door/firstword","zx/door/opt/v000001","zx/door/lastword","zx/door/openlog"};
//	int qoss[4]={0,0,0,0};	
//	rc = MQTTAsync_subscribeMany(client, 1,(char *const *)topic, qoss, &ropts);
//	if (rc != MQTTASYNC_SUCCESS)
//	{
//		printf("Failed to start subscribe, return code %d\n", rc);
//		finished = 1;
//	}
	
	//发送上线消息
	int data_len = 0;
    char buffer[128]={0};

	ropts.onSuccess = onPublish;
	ropts.onFailure = onPublishFailure;
 
	sprintf(buffer,"{\"devId\":\"%s\",\"state\":1,\"signal\":%d}",device_info.devId,30);
	printf("%s\n",buffer);
	data_len = strlen(buffer);
	MQTTAsync_send(client, "zx/door/firstword", data_len, buffer, opts.qos, opts.retained, &ropts);
	
}

void myconnect(MQTTAsync* client)
{
	MQTTAsync_connectOptions conn_opts = MQTTAsync_connectOptions_initializer;
	MQTTAsync_SSLOptions ssl_opts = MQTTAsync_SSLOptions_initializer;
	MQTTAsync_willOptions will_opts = MQTTAsync_willOptions_initializer;
	char will_message[64] = {0};
	will_opts.qos = 1;
	will_opts.topicName = "zx/door/lastword";
	sprintf(will_message,"{\"devId\":\"%s\",\"state\":0}",device_info.devId);
	printf("will message: %s\n",will_message);
	will_opts.message = will_message;
	
	int rc = 0;


	printf("Connecting\n");
	conn_opts.keepAliveInterval = opts.keepalive;
	conn_opts.cleansession = 1;
	conn_opts.username = opts.username;
	conn_opts.password = opts.password;
	conn_opts.onSuccess = onConnect;
	conn_opts.onFailure = onConnectFailure;
	conn_opts.context = *client;
	conn_opts.will = &will_opts;
	conn_opts.cleansession = 0;
	//ssl_opts.enableServerCertAuth = 0;
	//conn_opts.ssl = &ssl_opts;
	conn_opts.automaticReconnect = 1;
	connected = 0;
	if ((rc = MQTTAsync_connect(*client, &conn_opts)) != MQTTASYNC_SUCCESS)
	{
		printf("Failed to start connect, return code %d\n", rc);
		exit(EXIT_FAILURE);
	}
}




void connectionLost(void* context, char* cause)
{
	MQTTAsync client = (MQTTAsync)context;
	MQTTAsync_connectOptions conn_opts = MQTTAsync_connectOptions_initializer;
	MQTTAsync_SSLOptions ssl_opts = MQTTAsync_SSLOptions_initializer;
	int rc = 0;

	printf("Connecting\n");
	conn_opts.keepAliveInterval = 10;
	conn_opts.cleansession = 1;
	conn_opts.username = opts.username;
	conn_opts.password = opts.password;
	conn_opts.onSuccess = onConnect;
	conn_opts.onFailure = onConnectFailure;
	conn_opts.context = client;
	//ssl_opts.enableServerCertAuth = 0;
	//conn_opts.ssl = &ssl_opts;
	connected = 0;
	if ((rc = MQTTAsync_connect(client, &conn_opts)) != MQTTASYNC_SUCCESS)
	{
		printf("Failed to start connect, return code %d\n", rc);
		exit(EXIT_FAILURE);
	}
}


int main(int argc, char** argv)
{
	millis_Init();
	App_Init();
	IO_Init();				    //IO口初始化 外围模块复位
	SDK_Task_Init();		    //开门、抓拍任务初始化
	Upload_Pic_Task_Init();     //图片上传任务初始化
	Serial_Task_Init();			//串口接收任务初始化
	RFID_Init();          		//刷卡模块初始化

    cJSON * root = NULL;
    cJSON * item = NULL;//cjson对象
   
	MQTTAsync_disconnectOptions disc_opts = MQTTAsync_disconnectOptions_initializer;
	MQTTAsync_responseOptions pub_opts = MQTTAsync_responseOptions_initializer;
	MQTTAsync_createOptions create_opts = MQTTAsync_createOptions_initializer;
	
//	const char* topic  = NULL;
	char* buffer = NULL;
	char url[100]={0};
	int rc = 0;
	rc = deviceParamInit(&device_info);//参数初始化
	if(rc < 0)
		return 0;
	
	sprintf(url, "%s:%d", device_info.serverIP,  device_info.port);
	printf("URL is %s\n", url);
	
	create_opts.sendWhileDisconnected = 1;
	rc = MQTTAsync_createWithOptions(&client, url, opts.clientid, MQTTCLIENT_PERSISTENCE_NONE, NULL, &create_opts);

	signal(SIGINT, cfinish);
	signal(SIGTERM, cfinish);

	rc = MQTTAsync_setCallbacks(client, client, connectionLost, messageArrived, NULL);

	myconnect(&client);
	//buffer = malloc(opts.maxdatalen);
	
	int index =  0;
	while (!toStop)
	{
		card_detect(); //卡片检测
	    if(Serial_Avaliable() > 0)
	    {
	        index = 0;
	        memset(RFID_buff,0,sizeof(RFID_buff));
	        while(Serial_Avaliable() > 0)
	        {
	             RFID_buff[index] = Serial_Read();
	             if(RFID_buff[index] == 0x03 && index == 0x0D)
	             {          
	                  Card_task(index+1);
					  int ret = 0;
					  printf("clean Serial!!\n");
	                  Serial_Clean();   //清空串口缓冲区
					  printf("clean Serial finish!!\n");
	                  break;
	             }
	             if(RFID_buff[0] == 0x0E )
	             {
	                index++;
	                delay(1);
	             }   
	             delay(1);
	        }
	    }
		//getchar();
	}	
	printf("Stopping\n");
	//free(buffer);
	disc_opts.onSuccess = onDisconnect;
	if ((rc = MQTTAsync_disconnect(client, &disc_opts)) != MQTTASYNC_SUCCESS)
	{
		printf("Failed to start disconnect, return code %d\n", rc);
		exit(EXIT_FAILURE);
	}

	while	(!disconnected)
		#if defined(WIN32)
			Sleep(100);
		#else
			usleep(10000L);
		#endif

	MQTTAsync_destroy(&client);
	App_stop();//关闭sdk
	printf("-------------- Exit mqtt_door\n");
	if(toStop == 2)
	{
		printf("-------------- reboot\n");
		system("reboot");
	}
	return EXIT_SUCCESS;
}
