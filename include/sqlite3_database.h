#ifndef __SQLITE3_DATABASE__
#define __SQLITE3_DATABASE__

#include "common.h"
#include "cJSON.h"

typedef struct _MSG_CARD
{
    char msgtype;
    char *cardId;
    int type;
    char *createTime;
    char *updateTime;
}MSG_CARD;

typedef struct _MSG_CONFIG
{
    char msgtype;
    char *devId;
	char *serverIP;
    int port;
	int openingTime;
	int alarmTime;
	int tempCardFlag;
	char *URL;
    char *createTime;
    char *updateTime;
}MSG_CONFIG;

typedef struct _CONFIG_INFO
{
    char devId[64];
	char serverIP[32];
    int port;
	int openingTime;
	int alarmTime;
	int tempCardFlag;
	char URL[128];
}CONFIG_INFO;

typedef struct _MSG_CARD_LOG
{
    char msgtype;
    char *cardId;
    //int type;
    char *time;
	int state;
}MSG_CARD_LOG;


typedef struct _LOG_INFO
{
    char cardId[32];
	char time[64];
    int state;
	int recodId;
}LOG_INFO;




void timeSynchronization(time_t timep);//同步时间

void whiteBlackListOpt(cJSON *pMsgJson); //单条黑白名单操作

void batch_WhiteBlackListOpt(cJSON *pMsgJson); //批量黑白名单操作

void setDeviceParam(CONFIG_INFO* pConfigInfo);//设置设备参数

int deviceParamInit(CONFIG_INFO *pConfigInfo); //设备配置信息初始化

void saveCardRecord();//保存离线刷卡信息


void uploadCardRecord();//上传刷卡信息

int sqlite_find_card(const char * table, char *cardId, int type);

int sqlite_add_or_delete_card(MSG_CARD *pMsg);

int sqlite_add_or_update_card(MSG_CARD *pMsg);
int sqlite_delete_card(MSG_CARD *pMsg);	//删除数据库中的数据

int sqlite_find_config(CONFIG_INFO* pConfigInfo);
int sqlite_add_or_update_config(MSG_CONFIG *pMsg);  //增加数据到数据库

int sqlite_update_card(MSG_CARD *pMsg);
int sqlite_find_log(LOG_INFO* pLog);
int sqlite_add_or_update_log(MSG_CARD_LOG *pMsg);
int sqlite_delete_log(int recodId);	//删除数据库中的日志数据



#endif

