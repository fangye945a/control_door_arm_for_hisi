#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include <time.h>
#include "MQTTAsync.h"
#include "cJSON.h"
#include "sqlite3_database.h"



#define CONTROL_DEBUG 1
extern CONFIG_INFO device_info;  //设备配置信息
extern char buffer[128];//消息数组
extern MQTTAsync client;
extern MQTTAsync_responseOptions ropts;
extern struct
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
} opts;

void timeSynchronization(time_t timep)  //与服务器时间同步
{
	char cmd[64]={0};
	struct tm *p;  
	timep += 8*60*60;  //由于linux 3516cv300是UTC时间  需要转换为东八区CST时间
	p =localtime(&timep);
	sprintf(cmd,"date -s \"%d-%02d-%02d %d:%02d:%02d\"",1900+p->tm_year,1+p->tm_mon,p->tm_mday,
															p->tm_hour, p->tm_min, p->tm_sec);
	printf("cmd = %s\n",cmd);
	system(cmd); //需要root权限
}

void whiteBlackListOpt(cJSON *pMsgJson)  //单条黑白名单操作
{
	int type = -1;
	int add = -1;
	int lenght = 0;
	char card_ID[32]={0};
	const cJSON *pOptcodeJson = NULL;
	pOptcodeJson = cJSON_GetObjectItemCaseSensitive(pMsgJson, "type"); //解析type字段
	if(pOptcodeJson == NULL)
	{
		printf("Fuction:%s LINE:%d  error:pMsgJson analyze fail!!\n",__FUNCTION__,__LINE__);
		return;
	}else{
		type = pOptcodeJson->valueint;
		printf("type = %d\n",pOptcodeJson->valueint);
	}
	pOptcodeJson = cJSON_GetObjectItemCaseSensitive(pMsgJson, "add");  //解析add 字段
	if(pOptcodeJson == NULL)
	{
		printf("Fuction:%s LINE:%d  error:pMsgJson analyze fail!!\n",__FUNCTION__,__LINE__);
		return;
	}else{
		add = pOptcodeJson->valueint;
		printf("add = %d\n",pOptcodeJson->valueint);
	}
	pOptcodeJson = cJSON_GetObjectItemCaseSensitive(pMsgJson, "card"); //解析card 字段
	if(pOptcodeJson == NULL)
	{
		printf("Fuction:%s LINE:%d  error:pMsgJson analyze fail!!\n",__FUNCTION__,__LINE__);
		return;
	}else{
		strcpy(card_ID,pOptcodeJson->valuestring);
		printf("card_ID = %s\n",pOptcodeJson->valuestring);
	}

	if(type != 0 && type != 1 || add != 0 && add != 1 )
	{
		printf("pMsgJson type or add error!!\n");
		return;
	}
	
	MSG_CARD pMsg;
	pMsg.msgtype = add; //操作类型  添加或删除
	pMsg.cardId = card_ID; //卡片ID
	pMsg.type = type;   //白名单或黑名单
	
	int ret = sqlite_add_or_delete_card(&pMsg);
	if(ret != -1)
	{
		printf("sqlite_add_or_delete_card success!!\n");
	}else
		printf("sqlite_add_or_delete_card fail!!\n");
}

void batch_WhiteBlackListOpt(cJSON *pMsgJson)  //批量黑白名单操作
{
	int type = -1;
	int add = -1;
	int lenght = -1;
	char card_ID[32]={0};
	const cJSON *pOptcodeJson = NULL;
	pOptcodeJson = cJSON_GetObjectItemCaseSensitive(pMsgJson, "type"); //解析type字段
	if(pOptcodeJson == NULL)
	{
		printf("Fuction:%s LINE:%d  error:pMsgJson analyze fail!!\n",__FUNCTION__,__LINE__);
		return;
	}else{
		type = pOptcodeJson->valueint;
		printf("type = %d\n",pOptcodeJson->valueint);
	}
	pOptcodeJson = cJSON_GetObjectItemCaseSensitive(pMsgJson, "add");  //解析add 字段
	if(pOptcodeJson == NULL)
	{
		printf("Fuction:%s LINE:%d  error:pMsgJson analyze fail!!\n",__FUNCTION__,__LINE__);
		return;
	}else{
		add = pOptcodeJson->valueint;
		printf("add = %d\n",pOptcodeJson->valueint);
	}	

	if(type != 0 && type != 1 || add != 0 && add != 1 )
	{
		printf("pMsgJson type or add error!!\n");
		return;
	}
	
	pOptcodeJson = cJSON_GetObjectItemCaseSensitive(pMsgJson, "length"); //解析length字段
	if(pOptcodeJson == NULL)
	{
		printf("Fuction:%s LINE:%d  error:pMsgJson analyze fail!!\n",__FUNCTION__,__LINE__);
		return;
	}else{
		lenght = pOptcodeJson->valueint;
		printf("length = %d\n",pOptcodeJson->valueint);
		if(lenght <= 0)
			printf("Fuction:%s LINE:%d  error:The length  < or = 0!!\n",__FUNCTION__,__LINE__);
	}

	MSG_CARD pMsg;
	pMsg.msgtype = add; //操作类型  添加或删除
	pMsg.type = type;   //白名单或黑名单
	pMsg.cardId = card_ID; //卡片ID
	int ret = -1;
	cJSON *cardslist=cJSON_GetObjectItem(pMsgJson,"cards")->child;//子对象
	if(cardslist == NULL)
	{
		printf("Fuction:%s LINE:%d  error:pMsgJson analyze fail!!\n",__FUNCTION__,__LINE__);
		return;
	}else{
		while(cardslist != NULL)
		{	
			printf("%s\n",cardslist->valuestring);
			strcpy(card_ID,cardslist->valuestring);
			ret = sqlite_add_or_delete_card(&pMsg);
			if(ret != -1)
				lenght--;
			cardslist = cardslist->next;
		}
	}
	if(lenght == 0)
	{
		sprintf(buffer,"{\"devId\":\"%s\",\"optcode\":9,\"type\":%d,\"add\":%d,\"state\":1}",device_info.devId,type,add); 
		
	}else{
		sprintf(buffer,"{\"devId\":\"%s\",\"optcode\":9,\"type\":%d,\"add\":%d,\"state\":0}",device_info.devId,type,add); 

	}
	printf("push online message  -- %s\n",buffer);
	MQTTAsync_send(client, "zx/door/optack", strlen(buffer), buffer, opts.qos, opts.retained, &ropts);
	
}

void setDeviceParam(CONFIG_INFO* pConfigInfo)//设置设备参数
{
	MSG_CONFIG pMsg;
	pMsg.devId = pConfigInfo->devId;
	pMsg.serverIP = pConfigInfo->serverIP;
	pMsg.URL = pConfigInfo->URL;
	pMsg.openingTime = pConfigInfo->openingTime;
	pMsg.port = pConfigInfo->port;
	pMsg.tempCardFlag = pConfigInfo->tempCardFlag;
	pMsg.alarmTime = pConfigInfo->alarmTime;

	int rc = sqlite_add_or_update_config(&pMsg);  //增加数据到数据库
	if(rc != -1)
	{
		printf("setDeviceParam success!\n");
	}
	else
	{
		printf("setDeviceParam fail\n");
	}
}
int deviceParamInit(CONFIG_INFO *pConfigInfo) //设备配置信息初始化
{
	int rc = sqlite_find_config(pConfigInfo);
	if(rc == 1)
	{
		printf("deviceParamInit success!\n");
		printf("alarmTime = %d\n",device_info.alarmTime);
		printf("devId = %s\n",device_info.devId);
		printf("openingTime = %d\n",device_info.openingTime);
		printf("port = %d\n",device_info.port);
		printf("serverIP = %s\n",device_info.serverIP);
		printf("tempCardFlag = %d\n",device_info.tempCardFlag);
		printf("URL = %s\n",device_info.URL);
		
		opts.clientid = device_info.devId;
		//opts.host = device_info.serverIP;
		//opts.port = device_info.port;
		
		return 0;
	}
	else if(rc == 0)
	{	
		printf("error deviceParamInit fail,the datebase may be NULL!\n");
	}
	return -1;
}
void saveCardRecord(char *card, int state)//保存离线刷卡信息
{
	MSG_CARD_LOG *pMsg;
	pMsg->cardId = card;
	pMsg->state = state;
	sqlite_add_or_update_log(pMsg);
	return;
}

void uploadCardRecord() //上传离线刷卡信息
{
	LOG_INFO pLog;
	while(sqlite_find_log(&pLog) > 0)
	{
		sprintf(buffer,"{\"devId\":\"%s\",\"time\":%s,\"card\":%s,\"state\":%d}",device_info.devId,pLog.time,pLog.cardId,pLog.state); 
		MQTTAsync_send(client, "zx/door/openlog", strlen(buffer), buffer, opts.qos, opts.retained, &ropts);
	}
	return;
}

/******************************************************************************
* funciton : 查询数据库表中是否存卡片的通行权限
* note : exist: return 1
         no exist mcard: return 0
         error: return -1
******************************************************************************/
int sqlite_find_card(const char * table, char *cardId, int type)
{


    int nrow = 0;       //查询到数据的行数
    int ncolumn = 0;   //查询到数据的列数
    char **azResult;   //二维数组存放结果
    char *zErrMsg = NULL;
    char sql[128]={0};   //查询sql语句

	sqlite3 *pdb;
	int rc;
	sqlite3_stmt *pstmt;							//定义一个sql语句对象		
	rc = sqlite3_open(SETTING_DATABASE_FILE,&pdb);   //打开数据库
	if(rc != SQLITE_OK)
	{
		perror("sqlite3_open error");
		return -1;
	}

    sprintf(sql,"SELECT * FROM %s WHERE cardId = '%s' AND type = %d;", table, cardId, type );


#ifdef CONTROL_DEBUG
    printf("The SQL CMD:%s\n",sql);
#endif


    rc = sqlite3_get_table( pdb, sql, &azResult, &nrow, &ncolumn, &zErrMsg ); //查询数据库
    if( rc != SQLITE_OK )
    {
        fprintf(stderr,"Can't get table: %s",sqlite3_errmsg(pdb));
        return -1;
    }


#ifdef CONTROL_DEBUG
    printf( "nrow = %d ,ncolumn = %d\n", nrow, ncolumn );
#endif


    if( nrow !=0 && ncolumn != 0 )                                              //在数据库表中查到该车牌
    {
        //printf( "Find %d records from the table named %s.\n", nrow, table );
        printf("-----%s-%s-%s\n", azResult[4],azResult[5],azResult[6]);
        sqlite3_free_table( azResult );                                     //释放掉 azResult 的内存空间
        sqlite3_close(pdb);         //关闭数据库
        return 1;
    }
    else                                                                        //在数据库表中未查到该车牌
    {
        sqlite3_free_table( azResult );                                     //释放掉 azResult 的内存空间
        sqlite3_close(pdb);         //关闭数据库
#ifdef CONTROL_DEBUG
        printf("Can not find the records from database\n");
#endif
        return 0;
    }
}

/******************************************************************************
* funciton : 添加或删除卡片信息
* note : exist: return 1
         no exist mcard: return 0
         error: return -1
******************************************************************************/
int sqlite_add_or_delete_card(MSG_CARD *pMsg)
{
	if(pMsg->msgtype == 1)
	{
		return sqlite_add_or_update_card(pMsg); //更新数据库中的数据
	}
	else if(pMsg->msgtype == 0)
	{
		return sqlite_delete_card(pMsg);//删除数据库中的数据
	}
	printf("error:Function:%s Line:%d  The add flag is invaild!!\n",__FUNCTION__,__LINE__);
	return -1;
}

/******************************************************************************
* funciton : 添加或更新卡片信息
* note : exist: return 1
         no exist mcard: return 0
         error: return -1
******************************************************************************/
int sqlite_add_or_update_card(MSG_CARD *pMsg)  
{
	sqlite3 *pdb;
	int rc;
	sqlite3_stmt *pstmt;							//定义一个sql语句对象		
	
	//系统时间
	time_t timep;
    char timeStr[128] = {0};
    time(&timep);   
    ctime_r(&timep, timeStr);
	pMsg->createTime = timeStr;
	pMsg->updateTime = timeStr;
	
	rc = sqlite3_open(SETTING_DATABASE_FILE,&pdb);   //打开数据库
	if(rc != SQLITE_OK)
	{
		perror("sqlite3_open error");
		return -1;
	}
	char *sql_add = "insert or replace into card values(@cardId,@type,@createTime,@updateTime);";
	rc = sqlite3_prepare_v2(pdb,sql_add,strlen(sql_add),&pstmt,NULL);
	if(rc != SQLITE_OK)
	{
		perror("sqlite3_prepare_v2 error:");
		sqlite3_close(pdb);
		return -1;
	}
	char *vars[]={"@cardId","@type","@createTime","@updateTime",NULL};
	int vars_index[5];
	int i;
	for(i=0;vars[i] != NULL;i++)
	{
		vars_index[i]= sqlite3_bind_parameter_index(pstmt,vars[i]);
	}
	sqlite3_bind_text(pstmt,vars_index[0],pMsg->cardId,strlen(pMsg->cardId),NULL);
	sqlite3_bind_int(pstmt,vars_index[1],pMsg->type);
	sqlite3_bind_text(pstmt,vars_index[2],pMsg->createTime,strlen(pMsg->createTime),NULL);
	sqlite3_bind_text(pstmt,vars_index[3],pMsg->updateTime,strlen(pMsg->updateTime),NULL);
	rc = sqlite3_step(pstmt);
	if(rc == SQLITE_DONE)
	{
		printf("add success.\n");
	}
	else
	{
		perror("sqlite3_step error:333\n");
		sqlite3_finalize(pstmt);    //销毁一个sql语句对象
		sqlite3_close(pdb);         //关闭数据库
		return -1;
	}
	//  sqlite3_reset(pstmt);//复位sql语句，以便重新赋值参数
	sqlite3_finalize(pstmt);    //销毁一个sql语句对象
	sqlite3_close(pdb);         //关闭数据库
	return 0;
}


int sqlite_delete_card(MSG_CARD *pMsg)	//删除数据库中的数据
{
	sqlite3 *pdb;
	int rc; 
	sqlite3_stmt *pstmt;
	rc = sqlite3_open(SETTING_DATABASE_FILE,&pdb);   //打开数据库
	if(rc != SQLITE_OK)
	{
		perror("sqlite3_open error");
		return -1;
	}
	char *sql_delete = "delete from card where cardId=@cardId;";
	rc = sqlite3_prepare_v2(pdb,sql_delete,strlen(sql_delete),&pstmt,NULL);
	if(rc != SQLITE_OK)
	{
		perror("sqlite3_prepare_v2 error:");
		sqlite3_close(pdb);
		return -1;
	}
	char *vars[]={"@cardId",NULL};
	int vars_index[1];
	int i;
	for(i=0;vars[i] != NULL;i++)
	{
		vars_index[i]= sqlite3_bind_parameter_index(pstmt,vars[i]);
	}
	sqlite3_bind_text(pstmt,vars_index[0],pMsg->cardId,strlen(pMsg->cardId),NULL);

	rc = sqlite3_step(pstmt);
	if(rc == SQLITE_DONE)
	{
		printf("delete success.\n");
	}
	else
	{
		perror("sqlite3_step error:");
		sqlite3_finalize(pstmt);	//销毁一个sql语句对象
		sqlite3_close(pdb);			//关闭数据库
		return -1;
	}
	//	sqlite3_reset(pstmt);//复位sql语句，以便重新赋值参数
	sqlite3_finalize(pstmt);	//销毁一个sql语句对象
	sqlite3_close(pdb);			//关闭数据库
	return 0;
}
/******************************************************************************
* funciton : 查询配置信息
* note : exist: return 1
         no exist mcard: return 0
         error: return -1
******************************************************************************/
int sqlite_find_config(CONFIG_INFO* pConfigInfo)
{


    int nrow = 0;       //查询到数据的行数
    int ncolumn = 0;   //查询到数据的列数
    char **azResult;   //二维数组存放结果
    char *zErrMsg = NULL;
    char sql[128]={0};   //查询sql语句

	sqlite3 *pdb;
	int rc;
	sqlite3_stmt *pstmt;							//定义一个sql语句对象		
	rc = sqlite3_open(SETTING_DATABASE_FILE,&pdb);   //打开数据库
	if(rc != SQLITE_OK)
	{
		perror("sqlite3_open error");
		return -1;
	}

    sprintf(sql,"SELECT * FROM config;" );


#ifdef CONTROL_DEBUG
    printf("The SQL CMD:%s\n",sql);
#endif


    rc = sqlite3_get_table( pdb, sql, &azResult, &nrow, &ncolumn, &zErrMsg ); //查询数据库
    if( rc != SQLITE_OK )
    {
        fprintf(stderr,"Can't get table: %s",sqlite3_errmsg(pdb));
        return -1;
    }


#ifdef CONTROL_DEBUG
    printf( "nrow = %d ,ncolumn = %d\n", nrow, ncolumn );
#endif


    if( nrow !=0 && ncolumn != 0 )                                              //在数据库表中查到该车牌
    {
        //printf( "Find %d records from the table named %s.\n", nrow, table );
        printf("-----%s-%s-%s\n", azResult[ncolumn],azResult[ncolumn+1],azResult[ncolumn+2]);
		strcpy(pConfigInfo->devId, azResult[ncolumn]);
		strcpy(pConfigInfo->serverIP, azResult[ncolumn+1]);
		pConfigInfo->port = atoi(azResult[ncolumn+2]);
		pConfigInfo->openingTime = atoi(azResult[ncolumn+3]);
		pConfigInfo->alarmTime = atoi(azResult[ncolumn+4]);
		pConfigInfo->tempCardFlag = atoi(azResult[ncolumn+5]);
		strcpy(pConfigInfo->URL, azResult[ncolumn+6]);
        sqlite3_free_table( azResult );                                     //释放掉 azResult 的内存空间
        sqlite3_close(pdb);         //关闭数据库
        return 1;
    }
    else                                                                        //在数据库表中未查到该车牌
    {
        sqlite3_free_table( azResult );                                     //释放掉 azResult 的内存空间
        sqlite3_close(pdb);         //关闭数据库
#ifdef CONTROL_DEBUG
        printf("Can not find the records from database\n");
#endif
        return 0;
    }
}


/******************************************************************************
* funciton : 添加或更新配置信息
* note : exist: return 1
         no exist mcard: return 0
         error: return -1
******************************************************************************/
int sqlite_add_or_update_config(MSG_CONFIG *pMsg)  //增加数据到数据库
{
	sqlite3 *pdb;
	int rc;
	sqlite3_stmt *pstmt;							//定义一个sql语句对象		
	
	//系统时间
	time_t timep;
    char timeStr[128] = {0};
    time(&timep);   
    ctime_r(&timep, timeStr);
	pMsg->createTime = timeStr;
	pMsg->updateTime = timeStr;
	
	rc = sqlite3_open(SETTING_DATABASE_FILE,&pdb);   //打开数据库
	if(rc != SQLITE_OK)
	{
		perror("sqlite3_open error");
		return -1;
	}
	char *sql_add = "insert or replace into config values(@devId,@serverIP,@port,@openingTime,@alarmTime,@tempCardFlag,@URL,@createTime,@updateTime);";
	rc = sqlite3_prepare_v2(pdb,sql_add,strlen(sql_add),&pstmt,NULL);
	if(rc != SQLITE_OK)
	{
		perror("sqlite3_prepare_v2 error:33");
		sqlite3_close(pdb);
		return -1;
	}
	char *vars[]={"@devId","@serverIP","@port","@openingTime","@alarmTime","@tempCardFlag","@URL","@createTime","@updateTime",NULL};
	int vars_index[10];
	int i;
	for(i=0;vars[i] != NULL;i++)
	{
		vars_index[i]= sqlite3_bind_parameter_index(pstmt,vars[i]);
	}
	sqlite3_bind_text(pstmt,vars_index[0],pMsg->devId,strlen(pMsg->devId),NULL);
	sqlite3_bind_text(pstmt,vars_index[1],pMsg->serverIP,strlen(pMsg->serverIP),NULL);
	sqlite3_bind_int(pstmt,vars_index[2],pMsg->port);
	sqlite3_bind_int(pstmt,vars_index[3],pMsg->openingTime);
	sqlite3_bind_int(pstmt,vars_index[4],pMsg->alarmTime);
	sqlite3_bind_int(pstmt,vars_index[5],pMsg->tempCardFlag);
	sqlite3_bind_text(pstmt,vars_index[6],pMsg->URL,strlen(pMsg->URL),NULL);
	sqlite3_bind_text(pstmt,vars_index[7],pMsg->createTime,strlen(pMsg->createTime),NULL);
	sqlite3_bind_text(pstmt,vars_index[8],pMsg->updateTime,strlen(pMsg->updateTime),NULL);
	rc = sqlite3_step(pstmt);
	if(rc == SQLITE_DONE)
	{
		printf("add success.\n");
	}
	else
	{
		sqlite3_finalize(pstmt);    //销毁一个sql语句对象
		sqlite3_close(pdb);         //关闭数据库
		perror("sqlite3_step error:333\n");
		return -1;
	}
	//  sqlite3_reset(pstmt);//复位sql语句，以便重新赋值参数
	sqlite3_finalize(pstmt);    //销毁一个sql语句对象
	sqlite3_close(pdb);         //关闭数据库
	return 0;
}

int sqlite_update_card(MSG_CARD *pMsg)	
{
	sqlite3 *pdb;
	int rc; 
	sqlite3_stmt *pstmt;
	rc = sqlite3_open(SETTING_DATABASE_FILE,&pdb);   //打开数据库
	if(rc != SQLITE_OK)
	{
		perror("sqlite3_open error");
		return -1;
	}
	char *sql_update = "update card set cardId=@cardId,type=@type,createTime=@createTime,updateTime=@updateTime where cardId=@cardId";
	rc = sqlite3_prepare_v2(pdb,sql_update,strlen(sql_update),&pstmt,NULL);
	if(rc != SQLITE_OK)
	{
		perror("sqlite3_prepare_v2 error:");
		sqlite3_close(pdb);
		return -1;
	}

	char *vars[]={"@cardId","@type","@createTime","@updateTime",NULL};
	int vars_index[9];
	int i;
	for(i=0;vars[i] != NULL;i++)
	{
		vars_index[i]= sqlite3_bind_parameter_index(pstmt,vars[i]);
	}
	
	sqlite3_bind_text(pstmt,vars_index[0],pMsg->cardId,strlen(pMsg->cardId),NULL);
	sqlite3_bind_int(pstmt,vars_index[1],pMsg->type);
	sqlite3_bind_text(pstmt,vars_index[2],pMsg->createTime,strlen(pMsg->createTime),NULL);
	sqlite3_bind_text(pstmt,vars_index[3],pMsg->updateTime,strlen(pMsg->updateTime),NULL);

	rc = sqlite3_step(pstmt);
	if(rc == SQLITE_DONE)
	{
		printf("update success.\n");
	}
	else
	{
		perror("sqlite3_step error:");
		sqlite3_finalize(pstmt);	//销毁一个sql语句对象
		sqlite3_close(pdb);			//关闭数据库
		return -1;
	}
	//	sqlite3_reset(pstmt);//复位sql语句，以便重新赋值参数
	sqlite3_finalize(pstmt);	//销毁一个sql语句对象
	sqlite3_close(pdb);			//关闭数据库
	return 0;
}


/******************************************************************************
* funciton : 查询数据库表中日志信息，读取一条
* note : exist: return 1
         no exist mcard: return 0
         error: return -1
******************************************************************************/
int sqlite_find_log(LOG_INFO* pLog)
{


    int nrow = 0;       //查询到数据的行数
    int ncolumn = 0;   //查询到数据的列数
    char **azResult;   //二维数组存放结果
    char *zErrMsg = NULL;
    char sql[128]={0};   //查询sql语句

	sqlite3 *pdb;
	int rc;
	sqlite3_stmt *pstmt;							//定义一个sql语句对象		
	rc = sqlite3_open(SETTING_DATABASE_FILE,&pdb);   //打开数据库
	if(rc != SQLITE_OK)
	{
		perror("sqlite3_open error");
		return -1;
	}

    sprintf(sql,"SELECT * FROM open_log  limit 0,1;" );


#ifdef CONTROL_DEBUG
    printf("The SQL CMD:%s\n",sql);
#endif


    rc = sqlite3_get_table( pdb, sql, &azResult, &nrow, &ncolumn, &zErrMsg ); //查询数据库
    if( rc != SQLITE_OK )
    {
        fprintf(stderr,"Can't get table: %s",sqlite3_errmsg(pdb));
        return -1;
    }


#ifdef CONTROL_DEBUG
    printf( "nrow = %d ,ncolumn = %d\n", nrow, ncolumn );
#endif


    if( nrow !=0 && ncolumn != 0 )                                              //在数据库表中查到该车牌
    {
        //printf( "Find %d records from the table named %s.\n", nrow, table );
       // printf("-----%s-%s-%s\n", azResult[4],azResult[5],azResult[6],azResult[7]);
		
		
		strcpy(pLog->cardId, azResult[ncolumn]);
		strcpy(pLog->time, azResult[ncolumn+1]);
		pLog->state = atoi(azResult[ncolumn+2]);
		pLog->recodId = atoi(azResult[ncolumn+3]);
		
        sqlite3_free_table( azResult );                                     //释放掉 azResult 的内存空间
        sqlite3_close(pdb);			//关闭数据库
        return 1;
    }
    else                                                                        //在数据库表中未查到该车牌
    {
        sqlite3_free_table( azResult );                                     //释放掉 azResult 的内存空间
        sqlite3_close(pdb);			//关闭数据库
#ifdef CONTROL_DEBUG
        printf("Can not find the records from database\n");
#endif
        return 0;
    }
}



/******************************************************************************
* funciton : 添加或更新刷卡日志信息
* note : exist: return 1
         no exist mcard: return 0
         error: return -1
******************************************************************************/
int sqlite_add_or_update_log(MSG_CARD_LOG *pMsg)  
{
	sqlite3 *pdb;
	int rc;
	sqlite3_stmt *pstmt;							//定义一个sql语句对象		
	
	//系统时间	
	char timeStr[128] = {0};
	time_t timep;
	time(&timep); 
	sprintf(timeStr,"%d",(int)timep);
	pMsg->time = timeStr;
	
	rc = sqlite3_open(SETTING_DATABASE_FILE,&pdb);   //打开数据库
	if(rc != SQLITE_OK)
	{
		perror("sqlite3_open error");
		return -1;
	}
	char *sql_add = "insert or replace into open_log values(@cardId,@time,@state,@recodId);";
	rc = sqlite3_prepare_v2(pdb,sql_add,strlen(sql_add),&pstmt,NULL);
	if(rc != SQLITE_OK)
	{
		perror("sqlite3_prepare_v2 error:");
		sqlite3_close(pdb);
		return -1;
	}
	char *vars[]={"@cardId","@time","@state",NULL};
	int vars_index[5];
	int i;
	for(i=0;vars[i] != NULL;i++)
	{
		vars_index[i]= sqlite3_bind_parameter_index(pstmt,vars[i]);
	}
	sqlite3_bind_text(pstmt,vars_index[0],pMsg->cardId,strlen(pMsg->cardId),NULL);
	//sqlite3_bind_int(pstmt,vars_index[1],pMsg->type);
	sqlite3_bind_text(pstmt,vars_index[1],pMsg->time,strlen(pMsg->time),NULL);
	sqlite3_bind_int(pstmt,vars_index[2],pMsg->state);
	rc = sqlite3_step(pstmt);
	if(rc == SQLITE_DONE)
	{
		printf("add success.\n");
	}
	else
	{
		perror("sqlite3_step error:333\n");
		sqlite3_finalize(pstmt);    //销毁一个sql语句对象
		sqlite3_close(pdb);         //关闭数据库
		return -1;
	}
	//  sqlite3_reset(pstmt);//复位sql语句，以便重新赋值参数
	sqlite3_finalize(pstmt);    //销毁一个sql语句对象
	sqlite3_close(pdb);         //关闭数据库
	return 0;
}


int sqlite_delete_log(int recodId)	//删除数据库中的日志数据
{
	sqlite3 *pdb;
	int rc; 
	sqlite3_stmt *pstmt;
	rc = sqlite3_open(SETTING_DATABASE_FILE,&pdb);   //打开数据库
	if(rc != SQLITE_OK)
	{
		perror("sqlite3_open error");
		return -1;
	}
	char *sql_delete = "delete from open_log where recodId=@recodId;";
	rc = sqlite3_prepare_v2(pdb,sql_delete,strlen(sql_delete),&pstmt,NULL);
	if(rc != SQLITE_OK)
	{
		perror("sqlite3_prepare_v2 error:");
		sqlite3_close(pdb);
		return -1;
	}
	char *vars[]={"@recodId",NULL};
	int vars_index[1];
	int i;
	for(i=0;vars[i] != NULL;i++)
	{
		vars_index[i]= sqlite3_bind_parameter_index(pstmt,vars[i]);
	}
	
	sqlite3_bind_int(pstmt,vars_index[0],recodId);
	rc = sqlite3_step(pstmt);
	if(rc == SQLITE_DONE)
	{
		printf("delete success.\n");
	}
	else
	{
		perror("sqlite3_step error:");
		sqlite3_finalize(pstmt);	//销毁一个sql语句对象
		sqlite3_close(pdb);			//关闭数据库
		return -1;
	}
	//	sqlite3_reset(pstmt);//复位sql语句，以便重新赋值参数
	sqlite3_finalize(pstmt);	//销毁一个sql语句对象
	sqlite3_close(pdb);			//关闭数据库
	return 0;
}

/*
int mc_sqlite_opt(MSG_MC *pMsg)
{
	if(!strcmp(pMsg->opstate,"增加"))
	{
		mc_sqlite_add(pMsg);
	}
	else if(!strcmp(pMsg->opstate,"修改"))
	{
		mc_sqlite_update(pMsg);
	}
	else if(!strcmp(pMsg->opstate,"删除"))
	{
		mc_sqlite_delete(pMsg);
	}
	else 
	{
		printf("Recieve error message from pMsg->opstate.\n"); 
		return -1;
	}
	return 0;			
}
*/
/*
int main()
{
	MSG_CARD_LOG log_info;
	
	log_info.cardId = "aacd";
	log_info.time = "2018";
	log_info.state = 1;
	
	CONFIG_INFO config;
	
	MSG_CONFIG config_info;
	MSG_CARD card_info ;
	card_info.cardId = "aacd";
	
	
	config_info.devId = "z198";
	config_info.serverIP = "192.168.1.1";
	config_info.port = 9000;
	config_info.openingTime = 40;
	config_info.alarmTime = 50;
	config_info.tempCardFlag = 1;
	config_info.URL = "www.baidu.com";
	
	card_info.type = 0;

	LOG_INFO log;
	//sqlite_add_card(&card_info);
	
	//sqlite_add_or_update_card(&card_info);
	//sqlite_delete_card(&card_info);
	
	//sqlite_add_or_update_config(&config_info);
	
	//sqlite_find_config(&config);
	//printf("-------%s-%s-%d-%d-%d-%d-%s\n", config.devId, config.serverIP, config.port, config.openingTime, config.alarmTime, config.tempCardFlag, config.URL);
	
	//sqlite_add_or_update_log(&log_info);
	sqlite_find_log(&log);
	
	sqlite_delete_log(log.recodId);
	printf("%s-%s-%d-%d\n", log.cardId, log.time, log.state, log.recodId);
	//sqlite_find_card("card","aacd",0);
	return 0;
}*/
