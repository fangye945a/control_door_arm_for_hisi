#ifndef  SS_APP_D_H_
#define  SS_APP_D_H_

#include "App_param.h"

#define	VID_MAX_LEN 16

//=============================
//  param command contorl   (old   interface )
//=============================
typedef enum _PARAM_COMMAND
{
	CMD_SET_NETPARAM, // Set Network param
	CMD_GET_NETPARAM, // Get Network param
	CMD_SET_VIDEOPARAM, //Set Video Param
	CMD_GET_VIDEOPARAM, // Get Video Param
	CMD_SET_ALARMOUTPUT, // Set alarm out put
}PARAM_COMMAND;


enum _FLAG {
	TRUE,
	FALSE,
};


//network setting struct   //old struct  (CMD_SET_NETPARAM¡¢CMD_GET_NETPARAM)
typedef struct _APP_NET_CONFIG
{	
	unsigned int        IPAddr ;						//IP   address
	unsigned int        SubNetMask ;					//subnet mask
	unsigned int        GateWay ;						//gateway
	unsigned long		DNS1;							//DNS 1
	unsigned long		DNS2;							//DNS 2
   	unsigned short      DataPort;						//data port
   	unsigned short      WebPortNo;						//Webserver  prot
	unsigned int        MultiCastIPAddr ;				//multicast address
   	unsigned short      MultiCastPortNo;				//multicast port
	
	unsigned char		MacAddr[6];      				//MAC addr		0~5
   	unsigned char      	DHCP;							//DHCP    0 : close    1 : open

	
	unsigned int       Reserved[8];
} APP_NET_CONFIG;



//video set  //old struct(CMD_GET_VIDEOPARAM, CMD_SET_VIDEOPARAM)
typedef struct _APP_VIDEO_CONFIG
{
	unsigned int	dwSize;					//struct lengs

	VENC_PARAM_ST 	stMainVEncCtrl;			//main stream control
	VENC_PARAM_ST 	stMinVEncCtrl;			//sun stream  control
	PICTURE_CFG_ST	stPictureCfg;			//picture config
	OSD_CFG_ST		stDateOsd;				
	OSD_CFG_ST		stTimeOsd;
	OSD_CFG_ST		stTitleOsd;
	OSD_CFG_ST		stBitrateOsd;
	char 			Title[MAX_STRBUF];
	
	unsigned char   reseved[40];
}APP_VIDEO_CONFIG;

typedef struct _GB28181_NEW  //sdk sunchao 20131211
{
	char szPuid[128];
	char szPassWord[128];
	char szServerid[32];
	char szChanid[32];
	char szAlarmid[32];

	char szGbRegip[64];	
	int	 GbRegport;
  	int	 Gbdevport;	
	int  longitude;
	int  latitude;
	int  activetime;
	int  action;


	char username[32]; 
	char devicebelong[32];
	char administrativeregion[32];
	char policeregion[32]; 
	char installation[32];  
	char confidential[32];  
	char manufacturer[32];  
	
}GB28181_NEW;



//====================================
//   callback function  interface
//====================================


//===================================
//function:   
// specification : AlarmStatus    0 : alarm disappear  1 : Alarm generated
//===================================
typedef int  ( *MsgAlarmCallback)(APP_ALARM_STATUS_OUTPUT *pAlarmMsg, int AlarmStatus);

//===================================
//function:  
// specification :   pStreamData  : voideo  or audio data  ;
//						OneVoideoFrame = FRAME HEAD  + EXT FRAME HEAD +  voideoData 
//						OneAudioFrame = FRAME HEAD  + EXT FRAME HEAD +  audioData 
//				frameno :   fream number
//				sub	: 	0 main stream    1 sub stream
//===================================
typedef int  ( *StreamCallback)(void *pStreamData, unsigned long frameno, int sub);


//===================================
//function:  deal revc talk stream 
// specification :   
//				pTalkData:  talk stream data
//				nTalkDataLen:
//				pContext :
//===================================
typedef int  ( *TalksStreamCallback)(void *pTalkData,unsigned int nTalkDataLen,void *pContext);


//=================================================================
//								interface function
//=================================================================

//===================================
//function:    started by application
// specification :  
//===================================
int App_start();

//===================================
//function:    stop by application
// specification : 
//===================================
int App_stop();

//===================================
//function:   get max stream buf
// specification : 
//===================================
int App_maxstream_bufsize();

//===================================
//function:   Audio and video processing
// specification : 
//===================================
int App_set_stream_callback(StreamCallback funcChannelCallback);

//===================================
//function:  alarm call back
// specification : 
//===================================
int App_set_alarm_callback(MsgAlarmCallback funcAlarmCallback);


//===================================
//function:   start talk 
// specification :  
//			funcTalkCallback : deal device talk data
//			pContext:
//===================================
int App_Open_TalkRequsest(TalksStreamCallback funcTalkCallback, void *pContext);

//===================================
//function:   send talk data
// specification : 
//===================================
int App_Send_TalkData(char *pTalkData, unsigned int nDataLen);

//===================================
//function:   stop talk
// specification : 
//===================================
int App_Stop_TalkRequsest();

//===================================
//function:   
// specification : 
//===================================
int App_PTZControl(PTZ_COMMAND nCtrlCmd, unsigned char speed1, unsigned char speed2);


//===================================
//function:   get device config param (old   interface function)
// specification :   CMDType  :  Parameters of the command
//===================================
int App_get_parameter(PARAM_COMMAND CMDType, char *ParamConfig);

//===================================
//function:   set device config param (old   interface function)
// specification :    CMDType  :  Parameters of the command
//===================================
int App_set_parameter(PARAM_COMMAND CMDType, void *ParamConfig);

//===================================
//function:   get device config param(NEW interface function)
// specification : 
//			enConfigCmd:  Parameters of the command (in)
//			pConfigBuf:    return the structu information (out)
//			nConfigBufSize: return the struct information length(out) 
//			nAppend:  (out) 
//reutrn: 0 succeed    
//===================================
int App_GetDeviceConfig(PARAM_CFG_COMMAND_EN enConfigCmd, char *pConfigBuf, APP_U32 *nConfigBufSize, APP_UL32 *nAppend);


//===================================
//function:    set device config param(NEW interface function)
// specification : 
//			enConfigCmd:  Parameters of the command (in)
//			pConfigBuf:     the structu information (in)
//			nConfigBufSize:  the struct information length(in) 
//			nAppend:  (in) 
//reutrn: 0 succeed   
//===================================
int App_SetDeviceConfig(PARAM_CFG_COMMAND_EN enConfigCmd, char *pConfigBuf, APP_U32 nConfigBufSize, APP_UL32 nAppend);

//===============================
//function:	stop Main Stream
//
//===============================
int App_StopMainStream();

//===============================
//function:	stop Sub Stream
//
//===============================
int App_StopSubStream();

//================================
//function: 
//	Dog_thread(void *SleepTime);App_StartWatchDog(int SleepTime);App_RefreshWatchDog();App_StopWatchDog();
//
//  This is the watchdog interface, you can use it to check whether your program running
//sunchao
//================================
//void Dog_thread(void *SleepTime);
//int App_StartWatchDog(int SleepTime);
//int App_RefreshWatchDog();
//int App_StopWatchDog();

//==============================
//function:
//			Get wifi connection status
//specification:	int	Connections: Connections   Default = 1
//					int Interval:    Interval	   Default = 0
//return:  -2 parameter err;   -1 get  err;  0 unconnect;  1  ok 
//sunchao
//==============================
int App_Wifi_Connectstat(int Connections,int Interval);	

//===============================
//function:	start wps
//return: APP_ERR
//sunchao
//===============================
int App_WPS_CheckButton();	

//===============================
//function: get and set VID
//
//specification: char *pVID  len:16 
//sunchao
//===============================
#define	VID_MAX_LEN 16

int App_GET_VID(char *pVID);	
int App_SET_VID(char *pVID);

#endif
