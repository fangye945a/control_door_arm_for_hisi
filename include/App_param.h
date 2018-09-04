/******************************************************************************
* 系统名称：
* 文件名称：App_param.h
* 版本    ：V 2.0.0.1
* 说明	  ：开发包接口

			  一、TYPEDEF    定义
			  
			  二、宏定义
			  		I、 系统常量
			  		II、数据帧标识
			  		III、扩展帧标识
			  		
			  三、枚举类型定义
			  		I、球机控制命令
			  		II、参数操作命令
			  		III、 视频输入的色彩 、场景 类型定义
			  		IV、3G、WIFI的相关定义
			  		
			   四、接口结构类型定义
			   		I、音视频帧头结构信息
			   		II、 报警信息状态

			   五、编码器参数设置信息结构
			   		I、   公用结构
			   		II、  系统参数
			   			1、日期时间
			   			2、配置信息
			   			3、NTP参数
			   			4、用户设置

			   		III、  视频参数
			   			1、视频设置(osd字符叠加、 视频编码、 亮色调节,   图片参数)
			   			2、设备支持的编码格式和宽高
			   			3、图像屏蔽设置
			   			4、视频输入的色彩调节和相关设置
			   			5、视频输入场景相关设置 
			   			6、视频输入的所有色彩调节和场景设置
			   			7、视频信号参数
			   		IV、  音频属性
			   			1、音频设置
			   			
			   		V、   网络设置
			   			1、网络设置结构(基本参数、有线网络、pppoe参数、 DDNS参数、中心连接)
			   			2、无线 WIFI 参数
			   			3、UPNP  参数
			   			4、EMAIL参数
			   			5、 FTP 服务器参数
			   			6、DDNS参数
			   			7、3G参数
			   		VI、  存储设置
			   			1、磁盘信息
			   			2、录像计划
			   			3、抓拍计划
			   			
			   		VII、 报警设置
			   			1、移动报警
			   			2、探头报警
			   			3、网络报警
			   			4、视频丢失报警
			   			5、报警输出控制
			   			
			   		VIII、串口设置

					IX、云台
			   			1、透明通讯
			   			2、云台控制
			   			3、云台信息
			   			
			   		X、  其它参数结构
			   			1、TW2824参数
			   			2、定时重启
			   			3、扩展信息结构(ntp, upnp, email, alarm in/out, wifi, send dest, auto reset, ftp, rstp, gps)
			   			4、用户数据
			   			5、设备所有参数结构
******************************************************************************/
#ifndef  __PARAM_STRUCT_H__
#define  __PARAM_STRUCT_H__


/*************************************************************************
  						一、	Typedef
 *************************************************************************/

typedef	char				APP_S8;
typedef	short				APP_S16;
typedef int					APP_S32;
typedef	unsigned char		APP_U8;
typedef	unsigned short		APP_U16;
typedef unsigned int		APP_U32;
typedef	unsigned long		APP_UL32;

typedef unsigned long*		PHANDLE;
typedef unsigned long  		*PTALKHANDLE;
typedef void*				PVOID;

/*************************************************************************/


/**************************** ********************************************
						 二、宏定义
**************************************************************************/

//-------------------------I、系统常量---------------------------
#ifndef NULL
#define NULL  ((void *)0)
#endif

#define	FILE_PATH_MAX				256

#define MAX_STRBUF					64
#define MAX_NAME_LEN				16

#define VIDEO_MAX_CHN				4	//数字视频服务器最大通道数
#define AUDIO_MAX_CHN				4	//数字视频服务器音频最大通道数
#define SENSOR_MAX_CHN				4	//探头输入路数
#define RF_SENSOR_MAX_CHN			8	//无线探头最大路数
#define OUTPUT_MAX_CHN				4	//探头输出路数
#define COM_MAX						2	//串口最大个数	

#define USER_MAX_NUM				5	//用户个数
#define LONGON_MAX_NUM				10	//最大同时登录用户数
#define LINKVIEW_MAX_NUM			16	//最大同时TCP、UDP预览用户数
#define MULTIVIEW_MAX_NUM			60	//最大多播用户数
#define DEV_NAME_LEN           	32	//数字视频服务器名称长度


#define IPC_TITLE_LEN				64	//31个汉字，63个字符
#define USER_NAME_MAX_LEN 			16	//用户名的最大长度
#define USER_PASS_MAX_LEN   		16	//用户密码最大长度
	
#define DDNS_NAME_MAX_LEN			32	//注册主机名称长度
#define DDNS_PASS_MAX_LEN			16	//注册主机密码长度	
#define	DDNS_SER_NAME_LEN			32
#define PPPOE_NAME_LEN				32	//拨号用户名长度
#define PPPOE_PASSWD_LEN			16	//拨号用户名长度
#define YUNTAI_NAME_MAX_LEN		32	//云台协议名称长度
#define YUNTAI_PROTOCOL_MAX_NUM	36	//内置最大云台协议数
#define PTZ_PROTOCOL_MAX_NUM  		4	//最大云台协议数
#define	IP_NAME_MAX_LEN			128	//输入IP或名字的最大长度
#define	IP_STRING_LEN				16	//IP长

#define	DOMAIN_NAME_MAX_LEN		32  //域名的长度
#define EMAIL_MAX_LEN         		32	//EMAIL头和内容长

#define	MAX_PROTOCOL_NUM		16		//云台协议个数
#define MAX_PRESET_NAME			16		//预置点名称
#define	MAX_PRESET_NUM			256		//预置点个数
#define	MAX_TRACK_NUM			32		//轨迹个数
#define	MAX_CRUISE_NUM			32		//巡航个数


//----------------------II、数据帧标志----------------------------------
// frame  flags
#define FRAME_DATA_VP		0x0b	//Video P frame
#define FRAME_DATA_VI		0x0e	//Video I frame
#define FRAME_DATA_A		0x0d	//Audio frame

//-----------------------III、扩展帧头标志------------------------------
#define	EXT_HEAD_FLAG		0x06070809
#define	EXT_TAIL_FLAG		0x0a0b0c0d

/*******************************************************************************/


/********************************************************************************
								三、枚举类型定义
*********************************************************************************/

//--------------------------I、球机控制命令-------------------------------

typedef enum _APP_PTZ_COMMAND
{
	CMD_PTZ_UP		        	= 1,	//云台上
	CMD_PTZ_DOWN	      		= 2,	//云台下	
	CMD_PTZ_LEFT		    	= 3,	//云台左
	CMD_PTZ_RIGHT	        	= 4,	//云台右
	CMD_PTZ_FOCUSADD	    	= 5,	//聚焦+
	CMD_PTZ_FOCUSSUB	    	= 6,	//聚焦-
	CMD_PTZ_IRISADD	    		= 7,	//光圈+
	CMD_PTZ_IRISSUB	    		= 8,	//光圈-
	CMD_PTZ_ZOOMADD	    		= 9,	//变倍+
	CMD_PTZ_ZOOMSUB	    		= 10,	//变倍-
	CMD_PTZ_AUTOOPEN	    	= 11,	//自动开
	CMD_PTZ_AUTOCLOSE	    	= 12,	//自动关
	CMD_PTZ_LAMPOPEN	    	= 13,	//灯光开
	CMD_PTZ_LAMPCLOSE	    	= 14,	//灯光关
	CMD_PTZ_BRUSHOPEN	    	= 15,	//雨刮开
	CMD_PTZ_BRUSHCLOSE	    	= 16,	//雨刮关
	CMD_PTZ_WATEROPEN	    	= 17,	//放水开
	CMD_PTZ_WATERCLOSE	    	= 18,	//放水关

	//19、20内部保留	
 	CMD_PTZ_PRESET	        	= 19,	//预置 + 号
 	CMD_PTZ_CALL		    	= 20,	//调用 + 号
	CMD_PTZ_STOP		    	= 21,	//停止
	CMD_PTZ_UP_STOP	    		= 30,	//云台上-停	
	CMD_PTZ_DOWN_STOP	    	= 31,	//云台下-停	
	CMD_PTZ_LEFT_STOP	    	= 32,	//云台左-停
	CMD_PTZ_RIGHT_STOP	    	= 33,	//云台右-停
	CMD_PTZ_FOCUSADD_STOP 		= 34,	//聚焦+ -停
	CMD_PTZ_FOCUSSUB_STOP 		= 35,	//聚焦- -停
	CMD_PTZ_IRISADD_STOP		= 36,	//光圈+ -停
	CMD_PTZ_IRISSUB_STOP		= 37,	//光圈- -停
	CMD_PTZ_ZOOMADD_STOP		= 38,	//变倍+ -停
	CMD_PTZ_ZOOMSUB_STOP		= 39,	//变倍- -停	
	CMD_PTZ_UP_LEFT	    		= 40,	//云台左上	
	CMD_PTZ_UP_RIGHT	    	= 41,	//云台右上	
	CMD_PTZ_DOWN_LEFT	    	= 42,	//云台左下
	CMD_PTZ_DOWN_RIGHT	    	= 43,	//云台右下

	CMD_PTZ_UP_LIMIT	    	= 50,	//上限位
	CMD_PTZ_DOWN_LIMIT	    	= 51,	//下限位
	CMD_PTZ_LEFT_LIMIT	    	= 52,	//左限位
	CMD_PTZ_RIGHT_LIMIT			= 53,	//右限位
        
	CMD_PTZ_PREST_SCAN1			= 54,	//预置位巡视1
	CMD_PTZ_PREST_SCAN2			= 55,	//预置位巡视2
	CMD_PTZ_PREST_SCAN_STOP		= 56,	//预置位巡视停止
	CMD_PTZ_APPLE_SCAN 	    	= 57,	//削苹果扫描
	CMD_PTZ_APPLE_SCAN_STOP 	= 58,	//削苹果扫描停止

	CMD_PTZ_X_SCAN_SET	    	= 60,	//花样扫描设置
	CMD_PTZ_X_SCAN_SAVE	    	= 61,	//花样扫描设置结束
	CMD_PTZ_X_SCAN_RUN	    	= 62,	//花样扫描运行
	CMD_PTZ_X_SCAN_STOP	   		= 63,	//花样扫描停止
        
	CMD_PTZ_X_FLIP	        	= 64,	//水平翻转
	CMD_PTZ_VZERO_DETECT		= 65,	//虚拟零位检测

	CMD_PTZ_VZERO_SET	    	= 66,	//虚拟零位  设置
	CMD_PTZ_VZERO_OK	    	= 67,	//虚拟零位	确认
	CMD_PTZ_VZERO_CANCEL		= 68,	//虚拟零位  取消

	CMD_PTZ_RESET				= 69,	//球机云台复位
	CMD_PTZ_CLEAR_PRESET		= 70,  	// 删除预置?

	CMD_PTZ_LEFTANDUP			= 71,	//云台左上
	CMD_PTZ_LEFTANDDOWN			= 72,	//云台左下
	CMD_PTZ_RIGHTANDUP			= 73,	//云台右上
	CMD_PTZ_RIGHTANDDOWN		= 74,	//云台右下		
	CMD_PTZ_LEFTANDUP_STOP		= 75,	//云台左上停
	CMD_PTZ_LEFTANDDOWN_STOP	= 76,	//云台左下停
	CMD_PTZ_RIGHTANDUP_STOP		= 77,	//云台右上停
	CMD_PTZ_RIGHTANDDOWN_STOP 	= 78,	//云台右下停
	CMD_PTZ_PREST_SCAN3 		= 79,	//预置位巡视3
	CMD_PTZ_PREST_SCAN4			= 80,	//预置位巡视4
}PTZ_COMMAND;


//---------------------II、参数操作命令-------------------------

typedef enum _APP_PARAM_CFG_COMMAND_EN
{
	//编码器命令
	//GET_ALL_PARAMETER,			//0得到所有编码器参数//invalid
	SET_DEFAULT_PARAMETER = 1,		//1恢复所有编码器默认参数
	SET_RESTART_DVS,				//2重启编码器
	GET_SYS_CONFIG,				//3获取系统设置
	SET_SYS_CONFIG,				//4设置系统设置
	GET_TIME,						//5获取编码器时间
	SET_TIME,						//6设置编码器时间
	GET_AUDIO_CONFIG,				//7获取音频设置
	SET_AUDIO_CONFIG,				//8设置音频设置
	//GET_VIDEO_CONFIG,			//9获取视频设置//invalid
	//SET_VIDEO_CONFIG,			//10设置视频设置//invalid
	//GET_VMOTION_CONFIG,		//11获取移动侦测设置//invalid
	//SET_VMOTION_CONFIG,		//12设置移动侦测设置//invalid
	GET_VMASK_CONFIG		=13,	//13获取图像屏蔽设置
	SET_VMASK_CONFIG		=14,	//14设置图像屏蔽设置
	//GET_VLOST_CONFIG,			//15获取视频丢失设置//invalid
	//SET_VLOST_CONFIG,			//16设置视频丢失设置//invalid
	//GET_SENSOR_ALARM,			//17获取探头报警侦测设置//invalid
	//SET_SENSOR_ALARM,			//18设置探头报警侦测设置//invalid
	GET_USER_CONFIG		=19,	//19获取用户设置
	SET_USER_CONFIG,				//20设置用户设置
	GET_NET_CONFIG,				//21获取网络设置结构
	SET_NET_CONFIG,				//22设置网络设置结构
	GET_COM_CONFIG,				//23获取串口设置
	SET_COM_CONFIG,				//24设置串口设置
	GET_YUNTAI_CONFIG,				//25获取内置云台信息
	SET_YUNTAI_CONFIG,				//26设置内置云台信息
	GET_VIDEO_SIGNAL_CONFIG,		//27获取视频信号参数（亮度、色度、对比度、饱和度）
	SET_VIDEO_SIGNAL_CONFIG,		//28设置视频信号参数（亮度、色度、对比度、饱和度）
	SET_PAN_CTRL,					//29云台控制
	SET_COMM_SENDDATA,				//30透明数据传输
	SET_COMM_START_GETDATA,		//31开始采集透明数据
	SET_COMM_STOP_GETDATA,			//32停止采集透明数据
	SET_OUTPUT_CTRL,				//33继电器控制
	SET_PRAPP_S32_DEBUG,			//34调试信息开关
	SET_ALARM_CLEAR,				//35清除报警
	GET_ALARM_INFO,				//36获取报警状态和继电器状态
	SET_TW2824,						//37设置多画面芯片参数(保留)
	SET_SAVE_PARAM,				//38设置保存参数
	GET_USERINFO,					//39获取当前登陆的用户信息
	GET_DDNS,						//40获取DDNS
	SET_DDNS,						//41设置DDNS
	GET_CAPTURE_PIC,				//42前端抓拍
	//GET_SENSOR_CAP,				//43获取触发抓拍设置//invalid
	//SET_SENSOR_CAP,				//44设置触发抓拍设置//invalid
	GET_EXTINFO		=45,			//45获取扩展配置
	SET_EXTINFO,					//46设置扩展配置
	GET_USERDATA,					//47获取用户配置
	SET_USERDATA,					//48设置用户配置
	GET_NTP,						//49获取NTP配置
	SET_NTP,						//50设置NTP配置
	GET_UPNP,						//51获取UPNP配置
	SET_UPNP,						//52设置UPNP配置
	GET_MAIL,						//53获取MAIL配置
	SET_MAIL,						//54设置MAIL配置
	GET_ALARMNAME,					//55获取报警名配置
	SET_ALARMNAME,					//56设置报警名配置
	GET_WFNET,						//57获取无线网络配置
	SET_WFNET,						//58设置无线网络配置
	GET_SEND_DEST,					//59设置视频定向发送目标机//invalid
	SET_SEND_DEST,					//60设置视频定向发送目标机//invalid
	GET_AUTO_RESET,				//61取得定时重新注册
	SET_AUTO_RESET,				//62设置定时重新注册
	//GET_REC_SCHEDULE,			//63取得录像策略 //invalid
	//SET_REC_SCHEDULE,			//64设置录像策略 //invalid
	GET_DISK_INFO		=65,		//65取得磁盘信息
	SET_MANAGE,						//66设置命令和操作
	GET_CMOS_REG,					//67取得CMOS参数
	SET_CMOS_REG,					//68设置CMOS参数
	SET_SYSTEM_CMD,				//69设置执行命令
	SET_KEYFRAME_REQ,           	//70.设置关键帧请求
	GET_CONFIGENCPAR,           	//71.取得视频参数
	SET_CONFIGENCPAR,           	//72.设置视频参数
	GET_ALL_PARAMETER_NEW,      	//73.获取所有参数
	FING_LOG,                   	//74. 查找日志(查询方式:0－全部，1－按类型，2－按时间，3－按时间和类型 0xFF-关闭本次搜索) //invalid
	GET_LOG,                   	 	//75.读取查找到的日志 //invalid
	GET_SUPPORT_AV_FMT,         	//76.获取设备支持的编码格式、宽高及音频格式
	GET_VIDEO_CONFIG_NEW,       	//77.获取视频参数－new
	SET_VIDEO_CONFIG_NEW,       	//78.设置视频参数－new
	GET_VMOTION_CONFIG_NEW,     	//79.获取移动报警参数－new
	SET_VMOTION_CONFIG_NEW,     	//80.设置移动报警参数－new
	GET_VLOST_CONFIG_NEW,       	//81.获取视频丢失报警参数－new
	SET_VLOST_CONFIG_NEW,       	//82.设置视频丢失报警参数－new
	GET_SENSOR_ALARM_NEW,       	//83.获取探头报警参数－new
	SET_SENSOR_ALARM_NEW,       	//84.设置探头报警参数－new
	GET_NET_ALARM_CONFIG,       	//85.获取网络故障报警参数
	SET_NET_ALARM_CONFIG,       	//86.设置网络故障报警参数
	GET_RECORD_CONFIG,          	//87.获取定时录像参数
	SET_RECORD_CONFIG,          	//88.定时录像参数
	GET_SHOOT_CONFIG,           	//89.获取定时抓拍参数
	SET_SHOOT_CONFIG,            	//90.设置定时抓拍参数
	GET_FTP_CONFIG,             	//91.获取FTP参数
	SET_FTP_CONFIG,             	//92.设置FTP参数
	GET_RF_ALARM_CONFIG,        	//93.获取无线报警参数
	SET_RF_ALARM_CONFIG,        	//94.设置无线报警参数
	GET_EXT_DATA_CONFIG,        	//95.获取其它扩展参数(如平台设置其它参数) // invalid
	SET_EXT_DATA_CONFIG,        	//96.设置其它扩展参数(如平台设置其它参数)  //invalid
	GET_FORMAT_PROCESS,         	//97.获取硬盘格式化进度
	GET_PING_CONFIG,            	//98.PING 设置获取
	SET_PING_CONFIG,            	//99.PING 设置设置


	GET_VPN_CONFIG = 200,			//200.获取VPN设置参数
	SET_VPN_CONFIG = 201,			//201.设置VPN参数
	GET_3G_CONFIG  = 202,			//获取3G参数
	SET_3G_CONFIG  = 203,        	//设置3G参数
	GET_GPS_CONFIG = 204,
	SET_GPS_CONFIG = 205,
	//GET_3G_DIALCTRL= 206,
	//SET_3G_DIALCTRL= 207,	
//参数扩展===================
	GET_IR_CONFIG = 400,		//获取红外信息配置
	SET_IR_CONFIG,				//设置红外信息配置
	GET_ALL_CONFIGPARAM,
	SET_FORMATTING,			    //格式化

	GET_VID_UUID 		 = 404,			//获取VID众云
	SET_VID_UUID 		 = 405,			//设置VID众云
	
	GET_BG28181_CONFIG  = 406,//获取GB28181
	SET_BG28181_CONFIG  = 407,//设置GB28181
	WIFI_WPS			  = 408, 	//设置wps
	GET_WIFI_CONNECTSTAT = 409, 	//获取wifi链接状态

	GET_ZYUN_VID  = 410,	//获取众云VID 
	SET_ZYUN_VID  = 411,	//设置众云VID
	
	GET_RF_CFG = 412,	//RF参数获取
	SET_RF_CFG_RF = 413,	//RF 参数设置
	SET_RF_CFG_ALARM = 414, //RFALARM 参数
	SET_IO2_OUT = 415,
	SET_IO3_OUT = 416,

	GET_VI_SENSOR=1000,
	SET_VI_SENSOR,
	GET_VI_SCENE,
	SET_VI_SCENE,
	GET_VI_CFG,
	SET_VI_CFG,
	CMD_GET_DOME_PTZ_CFG,		//球机定时任务
	CMD_SET_DOME_PTZ_CFG,	
	CMD_GET_DOME_PRESET_CFG,  //预置位巡航
	CMD_SET_DOME_PRESET_CFG,
}PARAM_CFG_COMMAND_EN;


//----------------------III、 视频输入的色彩 、场景 类型定义-----------------
//视频输入的色彩调节和相关设置，类型定义
typedef enum APP_VIDEO_IN_SENSOR_EN
{
	//色彩
	VI_BRIGHTNESS	=	0X00000001,//亮度
	VI_HUE			=	0X00000002,//色度
	VI_CONTRAST		=	0X00000004,//对比度
	VI_SATURATION	=	0X00000008,//饱和度
	VI_SHARPNESS	=	0X00000010,//锐度

	
	VI_AUTODAGC		=	0X00000020,//最大自动模拟增益
	VI_HANDAAGC		=	0X00000040,//手动模拟增益
	VI_HANDDAGC		=	0X00000080,	//手动数字增益

	VI_GAMMA		=	0X00000100,//Gamma

	//白平衡
	VI_AUTOAWB		=	0X00000200,//自动白平衡
	VI_AWBRED		=	0X00000400,//白平衡 红
	VI_AWBGREEN		=	0X00000800,//白平衡 绿
	VI_AWBBLUE		=	0X00001000,//白平衡 蓝

	//增益
	VI_AUTOAGC		=	0X00002000,//自动增益
	VI_AGCLEVEL		=	0X00004000,//增益值

	//背光补偿
	VI_AUTOBLC		=	0X00008000,//自动补偿
	VI_BLCLEVEL		=	0X00010000,//补偿值
	
	//曝光
	VI_AUTOEXPOSURE=	0X00020000,//自动曝光 
	VI_EXPOSURETIME=	0X00040000,//手动曝光时间 
	
	//快门
	VI_SHUTTERFIRST=	0X00080000,//快门、曝光优先
	VI_AUTOSHUTTER	=	0X00100000,//自动快门
	VI_SHUTTERSPEED=	0X00200000,//快门速度
	VI_SLOWSHUTTER	=	0X00400000,//慢快门
	VI_SLOWSHUTTERLEVEL=0X00800000,//慢快门速度
	
	VI_AUTOAWBMODE =	0X01000000,		//1080p Hispeed自动白平衡模式
	VI_MAXAGC		=   0X02000000,		//最大增益值			 
	VI_EXPTIMEMAX	=   0X04000000,		//自动曝光快门最大值  
	VI_ANTIFOG	    =   0x08000000,
	VI_ANTIFLASECOLOR =    0x10000000,
	VI_ANTIDIS		=   0x20000000,
	VI_ROTATE	    =   0x40000000,
	VI_SENSOR_ALL	=	0xFFFFFFFF,//设置所以参数
}VIDEO_IN_SENSOR_EN;


//视频输入场景设置，类型定义
typedef enum _APP_VIDEO_IN_SCENE_EN
{
	//光圈
	VI_AUTOIRIS		=	0X00000001,//自动光圈
	VI_IRISLEVEL	=	0X00000002,//光圈电平

	//聚焦
	VI_AUTOFOCUS	=	0X00000004,//自动聚焦
	VI_FOCUSLEVEL	=	0X00000008,//焦距

	//变倍
	VI_ZOOMSPEED	=	0X00000010,//变倍速度
	VI_AUTOPTZSPEED=	0X00000020,//景深比例变速

	//电平控制
	VI_AUTOALC		=	0X00000040,//自动电平控制
	VI_ALCLEVEL		=	0X00000080, //电平控制值
	
	//彩转黑
	VI_CTB			=	0X00000100,//彩转黑

	//场景
	VI_SCENE		=	0X00000200,//场景
	VI_MIRROR		=	0X00000400,//镜向
	VI_FLIP			=	0X00000800,//翻转
	VI_AUTOFLIP		=	0X00001000,//自动翻转
	VI_PWDFREQ1		=	0X00002000,//照明频率1
	VI_PWDFREQ2		=	0X00004000,//照明频率2

	//红外
	VI_IREXIST		=	0X00008000,//是否有红外
	VI_IRCFMODE		=	0X00010000,//IRCF模式
	VI_IRLIGHTTYPE	=	0X00020000,//红外灯类型
	//
	VI_WDR 			=	0X00040000,//是否有宽动态
	VI_WDRLEVEL		=	0X00080000,//宽动态的值
	VI_LOWILLUM		=	0X00100000,//低照度
	VI_IMAGEMODE	=	0X00200000,//图像模式
	//
	VI_SIZE     	=   0X00400000,//视频输入尺寸
	VI_CTBLEVEL		=	0X00800000,//彩转黑阀值，当彩转黑为自动时有效
	VI_MINFOCUSLEN =   0X01000000,//设置最小聚焦距离
	VI_IRLEVEL		=   0X02000000,//夜间启动点		
	VI_LENSCORRECTION =0X04000000,//镜头校正		
	VI_SMARTNR     = 	0x08000000,//智能降噪 0 ~ 255  0为关
	VI_3DNR   	    =	0X10000000,//3D降噪		
	VI_3DNRLEVEL	= 	0x20000000,//3D降噪值

	VI_SCENE_ALL	=	0xFFFFFFFF,//设置所以参数
}VIDEO_IN_SCENE_EN;

//------------------------------------------------------------------------------

//---------------------------IV、3G、WIFI的相关定义-----------------------

// 3G拨号限制模式   0-不限制 1-每天限制时间  2-每月限制时间  3-既限制每天的时间，也限制每月的时间
typedef enum APP_E3G_TIME_CTRL_EN
{
	E3G_NO_LIMITED    = 0x00,
	E3G_DAY_LIMITED   = 0x01,
	E3G_MONTH_LIMITED = 0x02,
	E3G_ALL_LIMITED   = 0x03,
}E3G_TIME_CTRL_EN;// 限制模式

//主认证加密类型
typedef enum _APP_WF_AUTH_TYPE_EN		
{
	WIFI_AUTH_NONE		= 0x00,
	WIFI_AUTH_WEP		= 0x01,
	WIFI_AUTH_WPA		= 0x02,
	WIFI_AUTH_WPA2		= 0x03,
	WIFI_AUTH_WPAWPA2	= 0x04,
}WF_AUTH_TYPE_EN;

//辅助认证加密
typedef enum _APP_WF_ENC_TYPE_EN	
{
	WIFI_ENC_NONE		= 0x00,		//WEP  开放系统
	WIFI_ENC_WEP		= 0x01,		//WEP  共享密匙
	WIFI_ENC_TKIP		= 0x02,		//WEP  自动选择
	WIFI_ENC_AES		= 0x03,
}WF_ENC_TYPE_EN;

//---------------------------------------------------------------------------------



/************************************************************************************
							四、接口结构类型定义  
*************************************************************************************/

//--------------------------------I、音视频帧头信息--------------------------------

//音视频数据帧头
typedef struct _AV_FRAME_HEAD
{
	APP_U16		zeroFlag;	  //0
	APP_U8   	oneFlag;	  //0x1
	APP_U8		streamFlag;  //frame type        I frame  ,  p  frame  , video frame 
	
	APP_UL32	nByteNum;	 // vodio or audio data leng
	APP_UL32	nTimestamp; //
}AV_FRAME_HEAD;


//    视频数据信息
typedef struct _APP_EXTEND_FRAME_VIDEO
{
	unsigned short	nVideoEncodeType;	//视频编码算法
	unsigned short	nVideoWidth;		//视频图像宽
	unsigned short	nVideoHeight;		//视频图像高
	unsigned char   nPal;               //制式
	unsigned char   bTwoFeild;			//是否是两场编码(如果是两场编码，PC端需要做deinterlace)
	unsigned char   nFrameRate;			//帧率
	unsigned char   szReserve[7];		//
}EXT_FRAME_VIDEO;

//音频数据信息
typedef struct _APP_EXT_FRAME_AUDIO
{
	unsigned short	nAudioEncodeType;	//音频编码算法
	unsigned short	nAudioChannels;		//通道数
	unsigned short	nAudioBits;			//位数
	unsigned char   szReserve[2];
	unsigned long	nAudioSamples;		//采样率 	
	unsigned long	nAudioBitrate;		//音频编码码率
} EXT_FRAME_AUDIO;


typedef union _APP_EXT_FRAME_TYPE
{
	EXT_FRAME_VIDEO	szFrameVideo;
	EXT_FRAME_AUDIO	szFrameAudio;
} EXT_FRAME_TYPE;

//音视频 扩展帧头
typedef struct _APP_EXT_FRAME_HEAD
{
    unsigned long	nStartFlag;			//扩展帧头起始标识
    unsigned short	nVer;				//版本
    unsigned short	nLength;			//扩展帧头长度
	EXT_FRAME_TYPE	szFrameInfo;		
	unsigned long   nTimestamp;			//以毫秒为单位的时间戳
	unsigned long	nEndFlag;			//扩展帧头起始标识
}EXT_FRAME_HEAD;


//--------------------------------II、 报警信息状态----------------------------
typedef struct _APP_ALARM_STATUS_OUTPUT
{
	unsigned char year;
	unsigned char month;
	unsigned char day;
	unsigned char week;
	unsigned char hour;
	unsigned char minute;
	unsigned char second;
	unsigned char millsecond; 

	unsigned int SensorAlarm;
	unsigned int MotionAlarm;
	unsigned int ViLoseAlarm;
	unsigned int RFSensorAlarm;
	unsigned int NetAlarm;

	unsigned int SensorAlarmRec[SENSOR_MAX_CHN];
	unsigned int MotionAlarmRec[VIDEO_MAX_CHN];
	unsigned int ViLoseAlarmRec[VIDEO_MAX_CHN];
	unsigned int RFSensorAlarmRec[RF_SENSOR_MAX_CHN];
	unsigned int NetAlarmRec;

	unsigned int OutputStatus;

	unsigned int reserved[19];
}APP_ALARM_STATUS_OUTPUT;

 

/******************************************************************************************
							 五、编码器参数设置信息结构
*******************************************************************************************/


//-----------------------------I、公用结构-----------------------------------------
//时间段结构：定义一天的起始时间和结束时间
typedef struct _APP_TIME_SEGMENT_ST
{
	APP_U32 	  bDetectOpen;				//检测开关
	APP_U8        nBgnHour ;				//开始时
	APP_U8        nBgnMinute ;				//开始分
	APP_U8        nEndHour ;				//结束时
	APP_U8        nEndMinute ;				//结束分
}TIME_SEGMENT_ST;

//

typedef struct _DATE_TIME_ST
{
	APP_U32			second : 6;							//秒:  0 ~ 59
	APP_U32			minute : 6;							//分:  0 ~ 59
	APP_U32			hour : 5;							//时:  0 ~ 23
	APP_U32			day : 5;							//日:  1 ~ 31
	APP_U32			month : 4;							//月:  1 ~ 12
	APP_U32			year : 6;							//年:  2000 ~ 2063
}DATE_TIME_ST;

//---------------------------------------------------------------------------------------


//-----------------------------II、系统参数结构-----------------------------------

//1、日期时间结构 
//PARAM_CFG_COMMAND_EN:  GET_TIME  SET_TIME

typedef  struct  _APP_DVS_DATE_ST
{
	APP_U8		year;									//年
	APP_U8		month;									//月
	APP_U8		day;									//日
	APP_U8		week;									//周
	APP_U8		hour;									//小时
	APP_U8		minute;									//分钟
	APP_U8		second;									//秒
} DVS_DATE_ST;


//2、配置信息
//PARAM_CFG_COMMAND_EN : GET_SYS_CONFIG		 SET_SYS_CONFIG      
typedef struct _APP_DVS_CONFIG_ST
{
   	APP_S8              sysName[DEV_NAME_LEN + 1];		//名字
	APP_U8				sysInputNum;					//路数
	APP_U8				sysType;						//机器型号	0:CIF  1:HD1  2:D1  
	APP_U8				sysVideoFormat;			    	//编码格式	
    APP_U8				sysLanguage;					//语言
    APP_U8				sysStandard;					//制式		0:PAL  1:NTSC
    APP_U8				AD[2];
	APP_U32      		sysID;  						//每台机器有个ID号  	
	APP_U32      		sysVer;  						//软件版本  		
} DVS_CONFIG_ST;

//3、NTP  参数
//PARAM_CFG_COMMAND_EN: 	GET_NTP		SET_NTP        
typedef struct _APP_NTP_CONFIG_ST
{
	APP_U32			ntpOpen;						//NTP开关
	APP_U32			ntpTimeZone;					//时区
	APP_S8			ntpHost[DOMAIN_NAME_MAX_LEN];	//NTP Server
}NTP_CONFIG_ST;


//4、用户设置
//PARAM_CFG_COMMAND_EN:	GET_USER_CONFIG	  SET_USER_CONFIG    		  

typedef struct _APP_USER_CONFIG_ST
{
	APP_U32 		Level;									//权限
	APP_S8 		Name[USER_NAME_MAX_LEN + 1];				//用户名
	APP_S8 		Password[USER_PASS_MAX_LEN + 1];			//用户密码
	APP_S8 		AD[2];
}  USER_CONFIG_ST;

//-------------------------------------------------------------------


//-------------------------------III、视频参数-----------------------------

// osd  参数坐标、开关
typedef struct _APP_OSD_CFG_ST
{
	APP_U32		Show;						//0: 关  1: 开
	APP_U16		X;							//x，y：显示的位置，该位置的范围为0-704，0-576，与图像的分辨率无关，x必须为4的倍数；
	APP_U16		Y;
} OSD_CFG_ST ;

//视频编码参数
typedef struct _APP_VENC_PARAM_ST
{
	APP_U32       Open;						//是否开启编码（从码流）
	APP_U32		EncType;					//编码算法
	APP_U32		EncFormat;					//编码格式(D1, HD1, CIF)
	APP_U32		Width;						//宽度
	APP_U32		Height;						//高度
	
	APP_U32		KeyInterval;			    //I 帧间隔
	APP_U32		Bitrate ;					//码率
	float		FrameRate ;					//帧率float
	APP_U32		BitflowType; 			    //码流类型(0:VBR, 1:CBR)
	APP_U32		Qulity;						//质量0--5

	APP_U32		Profile;				 	//0: baseline 1:main profile
	APP_U32     Reserved[7];
} VENC_PARAM_ST ;

//视频宽高
typedef struct _APP_VIDEO_SIZE_ST
{
	APP_U16		Width;						//宽
	APP_U16		Height;						//高
}VIDEO_SIZE_ST;


//抓拍图片参数

typedef struct _APP_PICTURE_CFG_ST				//sizeof() = 148
{
	APP_U32		dwSize;						//本结构长度

	APP_U32     EncType;					//抓图格式(.jpg, bmp) 
	APP_U32     EncFormat;					//图片其它参数(24位， 32位) 
	APP_U32     Width;
	APP_U32     Height;
	APP_U8		TwoFeild;					//
	APP_U8 		qulity;						//(1~5)
	APP_U8     	reseved[42];
}PICTURE_CFG_ST;


//1、视频设置(osd字符叠加、 视频编码、 亮色调节,   图片参数)
//PARAM_CFG_COMMAND_EN:  GET_VIDEO_CONFIG_NEW	 	SET_VIDEO_CONFIG_NEW		
typedef struct _APP_VIDEO_CONFIG_ST
{
	APP_U32			dwSize;					//本结构长度

	VENC_PARAM_ST 	stMainVEncCtrl;			//主码流控制
	VENC_PARAM_ST 	stMinVEncCtrl;			//从码流控制
	PICTURE_CFG_ST	stPictureCfg;			//抓拍图片参数

	OSD_CFG_ST		stDateOSD; 				// 0:不叠加日期,   1:叠加日期
	OSD_CFG_ST		stTimeOSD; 				// 0:不叠加时间,   1:叠加时间
	OSD_CFG_ST		stWeekOSD; 				// 0:不叠加星期,   1:叠加星期 
	OSD_CFG_ST		stTitleOSD;				// 0:不叠加标题,   1:叠加标题 
	OSD_CFG_ST		stBitrateOSD;			// 0:不叠加码率,   1:叠加码率 	
	
	char    		Title[IPC_TITLE_LEN];	//标题字符(64个字符  31个汉字)

    APP_U8 			Brightness;				    //亮度
	APP_U8 			Hue;						//色调
	APP_U8 			Saturation;				    //饱和度
	APP_U8 			Contrast;					//对比度

	APP_U16 		Protocol;					//协议编号(0 ~ MAX_PAN_PROTOCOL_NUM - 1)
	APP_U16 		YTAddr;						//解码器地址

	APP_U8			mtClear;
	APP_U8			mtRed;
	APP_U8			mtBlue;
	APP_U8			mtGamma;

	unsigned char   nOsdPrefer;   		//0: XXXX-XX-XX 年月日 
								 		//1: XX-XX-XXXX 月日年 
										//2: XXXX年XX月XX日 
										//3: XX月XX日XXXX年
							  
								
	unsigned char       nOsdColor;	 // 		osd颜色0白色 1 黑 2黄 3红 4 蓝
	unsigned char       nScene;      
	unsigned char       nOSDClientNum;
	APP_U8        	reseved[24];
}VIDEO_CONFIG_ST;



//2、设备支持的编码格式和宽高
//PARAM_CFG_COMMAND_EN : GET_SUPPORT_AV_FMT

typedef struct _APP_SUPPORT_AV_FMT_ST
{
	APP_U32		dwSize;						//本结构长度
	
	APP_U32		MainVideoFmt[8];			//主码流编码格式
	APP_U32		MinVideoFmt[8];				//从码流编码格式
	APP_U32		PictureFmt[8];				//图片编码格式

	VIDEO_SIZE_ST  MainVideoSize[8];			//主码流 宽、高	
	VIDEO_SIZE_ST  MinVideoSize[8];			//从码流 宽、高	
	VIDEO_SIZE_ST  PictureSize[8];				//图片 宽、高	

	APP_U32		AudioFmt[8];				//音频编码格式
	APP_U32		AudioSampleRate[8];			//音频采样率
	APP_U32		AudioBitrate[8];			//音频码率
	APP_U32		AuidoAEC;					//是否支持回声抵消

	APP_U8     	reserve[32];	
}SUPPORT_AV_FMT_ST;


//区域位置
typedef struct _APP_AREA_RECT_ST
{
	APP_U16		X;							//X坐标	
	APP_U16		Y;							//Y坐标
	APP_U16		W;							//宽度
	APP_U16		H;							//高度
} AREA_RECT_ST;


//3、图像屏蔽设置
//PARAM_CFG_COMMAND_EN:	GET_VMASK_CONFIG		SET_VMASK_CONFIG 		

typedef struct _APP_VIDEOMASK_CONFIG_ST
{
	APP_U8 			MaskOpen;					//视频屏蔽开关		0:关         1：开
	APP_U8			AA[3];
	AREA_RECT_ST	MaskArea[9];				//视频屏蔽区域		
}VIDEOMASK_CONFIG_ST;



//4、视频输入的色彩调节和相关设置 size=64+32=96
//PARAM_CFG_COMMAND_EN: 	GET_VI_SENSOR		SET_VI_SENSOR	 		

typedef struct _APP_VIDEO_IN_SENSOR_ST
{
	VIDEO_IN_SENSOR_EN	eValidSupport;	/*有效支持的参数，该参数设置时候无效
											在获取的时候有效，用以判断设备是否支持该参数
										*/

	VIDEO_IN_SENSOR_EN	eValidSetting;	/*设置有效的参数，该参数获取时候无效
											在设置的时候有效，用以指定具体设置的参数
										*/

	//色彩
	APP_U8		byBrightness;			/*亮度;			0 ~ 255*/
	APP_U8		byBrightnessDefault;	/*亮度缺省值;	*/
	APP_U8		byBrightnessStep;		/*亮度调节步长;	*/

	APP_U8		byHue;					/*色度;			0 ~ 255*/
	APP_U8		byHueDefault;			/*色度缺省值;	*/
	APP_U8		byHueStep;				/*色度调节步长;	*/
	
	APP_U8		byContrast;				/*对比度;	0 ~ 255*/
	APP_U8		byContrastDefault;		/*对比度缺省值;	0 ~ 255*/
	APP_U8		byContrastStep;		/*对比度调节步长;	0 ~ 255*/

	APP_U8		bySaturation;			/*饱和度;	0 ~ 255*/
	APP_U8		bySaturationDefault;	/*饱和度缺省值;*/
	APP_U8		bySaturationStep;		/*饱和度调节步长;*/

	APP_U8		bySharpness;			/*锐度;		0 ~ 255*/
	APP_U8		bySharpnessDefault;	/*锐度缺省值;*/
	APP_U8		bySharpnessStep;		/*锐度调节步长;*/

	APP_U8		byRed;					/*红色;		0 ~ 255*/
	APP_U8		byRedDefault;			/*红色缺省值;*/
	APP_U8		byRedStep;				/*红色调节步长;*/

	APP_U8		byGreen;				/*绿色;		0 ~ 255*/
	APP_U8		byGreenDefault;		/*绿色缺省值;*/
	APP_U8		byGreenStep;			/*绿色调节步长;*/

	APP_U8		byBlue;					/*蓝色;		0 ~ 255*/
	APP_U8		byBlueDefault;			/*蓝色缺省值;*/
	APP_U8		byBlueStep;				/*蓝色调节步长;*/

	APP_U8		byGamma;				/*gamma;	0 ~ 255*/
	APP_U8		byGammaDefault;		/*gamma缺省值;*/
	APP_U8		byGammaStep;			/*gamma调节步长;*/


	//白平衡
	APP_U8		byAutoAwb;				/*自动白平衡;	0自动, 1 手动*/
	
	APP_U8		byAwbRed;				/*白平衡 红;	0 ~ 255*/
	APP_U8		byAwbRedDefault;		/*白平衡 红缺省值;*/
	APP_U8		byAwbRedStep;			/*白平衡 红调节步长;*/

	APP_U8		byAwbGreen;				/*白平衡 绿;	0 ~ 255*/
	APP_U8		byAwbGreenDefault;		/*白平衡 绿缺省值;*/
	APP_U8		byAwbGreenStep;		/*白平衡 绿调节步长;*/

	APP_U8		byAwBblue;				/*白平衡 蓝;	0 ~ 255*/
	APP_U8		byAwBblueDefault;		/*白平衡 蓝缺省值;*/
	APP_U8		byAwBblueStep;			/*白平衡 蓝调节步长;*/

	// 高清高速球   1080P   byAgcLevel --> 最大增益值
	//						  720P   byAgcLevel --> AGC
	//							     byAutoAgc  --> 最大增益值
 	//增益
	APP_U8		byAutoAgc;				/*自动增益;		0自动, 1 手动*/
	APP_U8		byAgcLevel;				/*增益值;		0 ~ 255*/
	APP_U8		byAgcLevelDefault;		/*增益值缺省值;*/
	APP_U8		byAgcLevelStep;		/*增益值调节步长;*/


	//背光补偿
	APP_U8		byAutoBlc;			/*自动补偿;		0自动, 1 手动*/
	APP_U8		byBlcLevel;			/*补偿值;		0 ~ 255*/
	APP_U8		byBlcLevelDefault;	/*补偿值缺省值;*/
	APP_U8		byBlcLevelStep;		/*补偿值调节步长;*/
	

	//曝光invalid
	APP_U8		byAutoExposure;		/*0自动 1 手动*/
	APP_U16		wExpoSuretime;		/*手动曝光时间	F1.6=16
													F2.2=22
													F3.2=32
													F4.4=44
													F6.4=64
													F8.8=88
													F12.0=120
													F17.0=170
													F24.0=240
													F34.0=340
									*/


	//快门invalid
	APP_U8		byShutterFirst;		/*快门、曝光优先;	0快门优先, 1 曝光优先*/
	APP_U8		byAutoShutter;		/*自动快门;		0自动, 1 固定*/
	APP_U16		wShutterSpeed;		/*快门速度;		1		= 1
													1/2		= 2
													1/4		= 4
													1/8		= 8
													1/16	= 16
													1/25	= 25
													1/50	= 50
													1/100	= 100
													1/150	= 150
													1/200	= 200
													1/250	= 250
													1/300	= 300
													1/400	= 400
													1/1000	= 1000
													1/2000	= 2000
													1/4000	= 4000
													1/10000	= 10000
										*/
	APP_U8		bySlowShutter;				/*慢快门;		0关,   1 开*/// invalid

	APP_U8		bySlowShutterLevel;			/*慢快门速度;	0 ~ 255*///invalid
	APP_U8		bySlowShutterLevelDefault;	/*慢快门速度缺省值;*///invalid
	APP_U8		bySlowShutterLevelStep;		/*慢快门速度调节步长;*///invalid

	APP_U8		byAwbAutoMode;	//自动白平衡模式,仅当白平衡为自动时有效
	APP_U8		byMaxAgc;						//最大增益值   (自动模拟增益)
	APP_U16		wExpTimeMax;					//自动曝光快门最大值  
	APP_U8		byAntiFog;						//去雾
	APP_U8      byAntiFalseColor;               //去伪彩
	APP_U8      byAntiDIS;                      //去抖
	APP_U8      byRotate;                       //旋转
	
	APP_U8 		byAutoDGainMax;				//最大自动数字增益(0 ~ 0xff)  VI_AUTODAGC
	APP_U8 		byManualAGain;					//手动模拟增益(0  ~ 0xff) VI_HANDAAGC	
	APP_U8		byManualDGain;					//手动数字增益(0  ~ 0xff) VI_HANDDAGC	
	APP_U8		byManualAGainEnable;			//手动模拟增益使能  VI_HANDAAGC	
	APP_U8		byManualDGainEnable;			// 手动数字增益使能VI_HANDDAGC	
	APP_U8		byRes[19];
}VIDEO_IN_SENSOR_ST,*PVIDEO_IN_SENSOR_ST;


//5、视频输入场景相关设置 size=32+64=96
//PARAM_CFG_COMMAND_EN:  GET_VI_SCENE	SET_VI_SCENE       

typedef struct _APP_VIDEO_IN_SCENE_ST
{
	VIDEO_IN_SCENE_EN	eValidSupport;	  /*有效支持的参数，该参数设置时候无效
											在获取的时候有效，用以判断设备是否支持该参数
											*/

	VIDEO_IN_SCENE_EN	eValidSetting;		/*设置有效的参数，该参数获取时候无效
											在设置的时候有效，用以指定具体设置的参数
											*/


	//光圈
	APP_U8		byAutoIris;					/*自动光圈;		0自动, 1 手动*/

	APP_U8		byIrisLevel;				/*光圈电平;			0 ~ 255*/
	APP_U8		byIrisLevelDefault;		/*光圈电平缺省值;*/
	APP_U8		byIrisLevelStep;			/*光圈电平调节步长;*/


	//聚焦
	APP_U8		byAutoFocus;				/*自动聚焦;		0自动, 1 手动*/

	APP_U8		byFocusLevel;				/*焦距;				0 ～ 255*/
	APP_U8		byFocusLevelDefault;		/*焦距缺省值;*/
	APP_U8		byFocusLevelStep;			/*焦距调节步长;*/


	//变倍
	APP_U8		byZoomSpeed;				/*变倍速度;		0 高速,  1 正常速度*/
	APP_U8		byAutoPtzSpeed;			/*景深比例变速; 0关,   1 开*/


	//电平控制
	APP_U8		byAutoAlc;					/*自动电平控制;	AutoALC	0自动 1 手动*/

	APP_U8		byAlcLevel;					/*电平值;			ALC Level	0 ~ 255*/
	APP_U8		byAlcLevelDefault;			/*电平缺省值;		ALC Level	*/
	APP_U8		byAlcLevelStep;			/*电平值调节步长;	ALC Level	*/


	//彩转黑
	APP_U8		byCtb;						/*彩转黑;		0关,   1 开*/


	//场景
	APP_U8		byScene;					/*场景;			0 ~ 255*/
	APP_U8		byMirror;					/*镜向;			0关,   1 开*/
	APP_U8		byFlip;						/*翻转;			0关,   1 开*/
	APP_U8		byAutoFlip;					/*自动翻转;		0关,  1 开*/
	APP_U8		byPwdFreq1;					/*照明频率1;	0 60HZ,	1 50HZ*/
	APP_U8		byPwdFreq2;					/*照明频率2;	0 60HZ,	1 50HZ,	2 户外*/


	//红外
	APP_U8		byIRExist;					/*是否有红外	0无,   1 有*/
	APP_U8		byIRCFMode;					/*IRcf模式;		0 OUT=>IN, 1 IN=>OUT*/
	APP_U8		byIRLightType;				/*红外灯类型;	0 正常光,  1 850mm,   2 950mm*/
	APP_U8		byWDR;						/*宽动态		0无,   1 有*/
	APP_U8		byWDRLevel;					/*宽动态		1~3低中高等级*/
	APP_U8		byLowIllumination;			/*低照度		1默认AF
													2低照度AF优化开启
													3低照度点光源AF优化开启
											*/
	APP_U8		byImageMode;				/*图像模式		0 模式 1，  1 模式 2 */

	APP_U16      u16ViWidth;        	 	/*视频输入 宽度*/
	APP_U16		u16ViHeight;        		/*视频输入 高度*/
	APP_U8		byCtbLevel;					/*彩转黑阀值，自动彩转黑时有效*/
	APP_U8		byMinFocusLen;				/*设置最小聚焦距离*/			  
	APP_U8		byIRLevel;					/*夜间启动点 */		          
	APP_U8 		byLensCorrection; 			/*镜头校正	0: 关	1：开  */ 
	APP_U8       bySmartNR;					//智能降噪  0 ~ 255 0: 关 
	APP_U8       bySmartNRDefault;			//智能降噪  默认值 
	APP_U8 		by3DNR; 					/*3D 降噪   0: 关	1：开  */  
	APP_U8       by3DNRLevel;				//3D降噪值 
	APP_U8		byret[13];
	APP_U8		byWDR2;						// 提供宽动态 (0 , 关 1  自动DRC， 2 手动DRC)   (此模式有有包含自动DRC)
	APP_U8		byWDRLevel2;				//手动控制 DRC 强度 0 ~0xff    需要对应byWDR2 的手动模式
	APP_U8		byRes[34];					//
}VIDEO_IN_SCENE_ST,*PVIDEO_IN_SCENE_ST;


//6、视频输入的所有色彩调节和场景设置
//大小196(sizeof(APP_VIDEO_IN_CFG))
//PARAM_CFG_COMMAND_EN:	GET_VI_CFG		SET_VI_CFG     

typedef struct _APP_VIDEO_IN_CFG_ST
{
	APP_U32				dwSize;		    /*结构大小*/
	VIDEO_IN_SENSOR_EN	struViSensor;	/*视频输入设置*/
	VIDEO_IN_SCENE_ST	struViScene;	/*视频输入场景相关设置*/
}VIDEO_IN_CFG_ST,*PVIDEO_IN_CFG_ST;


//7、视频信号参数
//PARAM_CFG_COMMAND_EN:	GET_VIDEO_SIGNAL_CONFIG 	 SET_VIDEO_SIGNAL_CONFIG
typedef struct __APP_VIDEO_SIGNAL_CONFIG_ST
{
	APP_U8				ChannelNo;						//通道号	0 ~ PORTNUM - 1
	APP_U8 				ChannelNum;						//通道数<NVS总的通道数，因为单路、四路的参数默认值是不同的>
	APP_U8 				Brightness;						//亮度
	APP_U8 				Hue;							//色调
	APP_U8 				Saturation;						//饱和度
	APP_U8 				Contrast;						//对比度
}VIDEO_SIGNAL_CONFIG_ST;
/**/
//8、红外信息参数
//PARAM_CFG_COMMAND_EN: GET_IR_CONFIG  SET_IR_CONFIG
typedef struct _VIDEO_IR_CFG_ST  //sdk 
{
	APP_U8				byInfraredDetectMode;			//0-- 视频检测   1-- 时间控制        2 --光敏检测
	APP_U8				byTRCutLevel;					//IRCUT切换             0 --低电平有效 1 --高电平有效           为夜晚模式
	APP_U8				byphotoresistorLevel;			//光敏电阻            0 --低电平有效 1 --高电平有效 		为黑夜
	APP_U8				byInfraredLamp ;				//红外灯切换      0 --低电平有效 1-- 高电平有效           打开红外灯
	DATE_TIME_ST		uDayDetecttime;			        //转白天检测时间
	DATE_TIME_ST		uNigntDetecttime;			    //转夜晚检测时间
	APP_U8 				byIrTime; 						//0~60秒可设，光敏模式，彩转黑转换时间
	APP_U8 				byIRLevel;						//视频检测灵敏度
	APP_U8				byRes[34];	
}VIDEO_IR_CFG_ST;


//-------------------------------------------------------------------


//-----------------------------IV、音频属性--------------------------

//1、音频设置
//PARAM_CFG_COMMAND_EN  GET_AUDIO_CONFIG  SET_AUDIO_CONFIG
typedef struct _APP_AUDIO_CONFIG_ST
{
	APP_U8		Open:1;									//音频是否打开
	APP_U8		MicOrLine:1;							//音频输入模式：麦克或线输入
	APP_U8		InVol:6;
	APP_U8		Type;
	APP_U8	  	AEC:1;					  
	APP_U8      OutVol:7;	
	APP_U8		AD;
				//压缩格式
	APP_U32		Bitrate ;								//码率
	APP_U32		SampleBitrate ;

} AUDIO_CONFIG_ST;


//--------------------------------------------------------------------



//-----------------------------V、网络设置---------------------------

//1、网络设置结构(基本参数、有线网络、pppoe参数、 DDNS参数、中心连接)
//PARAM_CFG_COMMAND_EN: GET_NET_CONFIG     SET_NET_CONFIG

typedef struct _APP_NET_CONFIG_ST
{	
	APP_U32             IPAddr ;						//IP地址
	APP_U32             SubNetMask ;					//掩码
	APP_U32             GateWay ;						//网关

   	APP_U16             ComPortNo;						//设置接收客户端命令端口号        UDP	
   	APP_U16             WebPortNo;						//Webserver端口     			  TCP
	APP_U32             MultiCastIPAddr ;				//多播IP地址
   	APP_U16             MultiCastPortNo;				//UDP传输起始端口<多播传输>
	
	APP_U8			   	RT8139MAC[6];      				//人工设置网卡的MAC地址		5---0有效
   	APP_U8             	DHCP;							//DHCP 开关	0	关		1:开
	APP_U8             	PppoeOpen;						//PPPOE 开关     	
	APP_S8				PppoeName[PPPOE_NAME_LEN+1];	//拨号用户名
	APP_S8				PppoePass[PPPOE_PASSWD_LEN+1]; //拨号密码	
	APP_U32             PppoeTimes;   					//在线时间
	APP_U32             PppoeIPAddr ;					//PPPOEIP地址		报警回传IP	
	
	APP_U32				DdnsOpen;						//DDNS开关
	APP_S8				DdnsName[DDNS_NAME_MAX_LEN+1];	//注册主机名称
	APP_S8				DdnsPass[DDNS_PASS_MAX_LEN+1];	//注册主机密码	
	
	APP_S8				DdnsIP[DDNS_SER_NAME_LEN+1];	//DDNS服务器
	APP_S8				AAD[1];
	APP_U16				DdnsPortNo;						//DDNS服务器端口
	APP_U16				DdnsMapWebPort;					//本地WEB映射端口
	APP_U16				DdnsMapDataPort;				//本地数据映射端口
	APP_S8				ABD[2];

	APP_U32				DNSHostIP;						//DNS的IP

	APP_S32				ConnectCenter;					//是否主动连接中心
	APP_S8				ConnectCenterIP[DDNS_SER_NAME_LEN+1];//中心IP
	APP_S8				ACD[1];
	APP_U16				ConnectCenterPort;						//中心端口

	APP_U16				appFunction;					//实现功能定义,以bit位表示:0-PPPOE,1-NTP,2-UPNP,3-WF,4-MAIL,5-定向发送
	APP_U8				tcpSendInterval;
	APP_U8				udpSendInterval;
	APP_U16				PacketMTU;

	APP_U8				CaptureMode;					//抓拍模式
	APP_U8				CapturePort[SENSOR_MAX_CHN];	//抓拍通道(0-3bit: 分别表示1 ~ 4号通道  1:开  0: 关)
	APP_S8				AED[1];
	union
	{
	  APP_U32       	DdnsIP2;						
 	  APP_U32       	DNSHostIP2;						
	};						//DDNS 2的IP,非网络字节次序
	APP_U16				DdnsPortNo2;					//DDNS 2的端口

	APP_S8				sysByname[DEV_NAME_LEN+1];		//域名
	APP_U8				domainSetState;				//0－成功；1－名字存在，修改新域名；2－正在申请；3－失败
} NET_CONFIG_ST;


//2、无线 WIFI 参数
//PARAM_CFG_COMMAND_EN  GET_WFNET    SET_WFNET

typedef struct _APP_WF_NET_CONFIG_ST
{
	APP_U32			dwSize;							//大小
	APP_U16			wfOpen;							//是否打开无线
	APP_U16			dhcpOpen;						//DHCP开关
	//
	APP_U32			ipAddress;						//IP地址
	APP_U32			subMask;						//子网掩码
	APP_U32			gateWay;						//网关
	APP_U32			dnsIP1;							//DNS服务器1 IP
	APP_U32			dnsIP2;							//DNS服务器2 IP
	APP_U8			macAddress[6];
	//
	APP_U8			ssidLen;						//SSID名称长度
	APP_U8			pswLen;							//密码长度

	APP_S8			ssidName[34];					//SSID名称
	APP_S8			pswText[34];					//密码
	APP_U32			channel;						//通道
	APP_U32			bps;
	APP_U32			type;
	APP_U8			wfAuthType;						//主认证加密类型
	APP_U8			wfEncType;						//辅助认证加密
	APP_U8			wfPswType;
	APP_S8			reserve[9];
}WF_NET_CONFIG_ST;

//3、UPNP  参数
//PARAM_CFG_COMMAND_EN:	GET_UPNP   SET_UPNP

typedef struct _APP_UPNP_CONFIG_ST
{
	APP_U16			upnpOpen;					//UPNP开关
	APP_U8			upnpEthNo;					//UPNP网卡 0－有线，1－无线
	APP_U8			upnpMode;					//UPNP模式 0－指定，1－自动
	APP_U16			upnpPortWeb;				//UPNP WEB端口
	APP_U16			upnpPortData;				//UPNP DATA端口
	APP_U32			upnpHost;					//UPNP主机
	APP_U16			upnpStatusWeb;				//UPNP WEB状态
	APP_U16			upnpStatusData;				//UPNP DATA状态
}UPNP_CONFIG_ST;


//4、EMAIL参数
//PARAM_CFG_COMMAND_EN :  GET_MAIL     SET_MAIL

typedef struct _APP_EMAIL_CONFIG_ST
{
	APP_S8			smtpServer[DOMAIN_NAME_MAX_LEN];	//发送服务器
	APP_S8			pop3Server[DOMAIN_NAME_MAX_LEN];	//接收服务器
	APP_S8			fromMailAddr[DOMAIN_NAME_MAX_LEN];	//邮件发送地址
	APP_S8			toMailAddr[DOMAIN_NAME_MAX_LEN];	//邮件接收地址
	APP_S8			ccMailAddr[DOMAIN_NAME_MAX_LEN];	//邮件抄送地址
	APP_S8			smtpUser[DOMAIN_NAME_MAX_LEN];		//SMTP 用户名
	APP_S8			smtpPassword[DOMAIN_NAME_MAX_LEN];  //SMTP 用户密码
	APP_S8			pop3User[DOMAIN_NAME_MAX_LEN];		//POP3 用户名
	APP_S8			pop3Password[DOMAIN_NAME_MAX_LEN];	//POP3 用户密码
		
	APP_U32			motionMailOpen;					//视频移动触发MAIL开关(invalid)
	APP_U32			motionMail[VIDEO_MAX_CHN];		//视频移动触发抓拍发送MAIL开关(invalid)
	APP_U32			sensorMailOpen;					//探头报警触发MAIL开关(invalid)
	APP_U32			sensorMail[SENSOR_MAX_CHN];		//探头报警触发抓拍发送MAIL开?invalid)?
	APP_U32			viloseMailOpen;		//(invalid)

	APP_U32			timeMailOpen;					//定时发送(/分)
	APP_U32			timeVideoMail[VIDEO_MAX_CHN];	//视频定时抓拍发送MAIL开关
	
	APP_S8			subjectMail[EMAIL_MAX_LEN];	//邮件头
	APP_S8			textMail[EMAIL_MAX_LEN];		//邮件内容
		
	APP_U32			sendMailCount;					//状态: 发送的email记数	
	APP_U32			sendNow;						//立即发送
	APP_U16			smtpPort;						//SMTP 端口
	APP_U16			popPort;						//POP3 端口
	APP_U16			protocolType;					//SSL
	APP_S8			res[6];
}EMAIL_CONFIG_ST;


//5、 FTP 服务器参数
//PARAM_CFG_COMMAND_EN: GET_FTP_CONFIG  SET_FTP_CONFIG

typedef struct __APP_FTP_CONFIG
{
    char			ftpURL[64];
    char			ftpPath[64];
    APP_U32			ftpPort;
    char			ftpUser[32];
    char			ftpPsw[32];

    char			ftpURL2[64];
    char			ftpPath2[64];
    APP_U32			ftpPort2;
    char			ftpUser2[32];
    char			ftpPsw2[32];

	APP_U16			StartPort;				
	APP_U16			EndPort;	
	APP_U8          retransimission;		
    char			res[27];
}FTP_CONFIG_ST;



//6、DDNS设置
//PARAM_CFG_COMMAND_EN: GET_DDNS  SET_DDNS

typedef struct _APP_DDNS_SET_ST
{
	APP_S32				bDdnsStart;
	APP_S8				szDdnsIP[DDNS_SER_NAME_LEN+1];
	APP_U32				dwDdnsPort;
	APP_S8				szDdnsName[DEV_NAME_LEN+1];
	APP_U32				dwMapWebPort;
	APP_U32				dwMapDataPort;
} DDNS_SET_ST;



//流媒体服务RTSP
typedef struct _APP_MEDIA_SERVER_ST
{
	APP_U8				bRtspOpen; 		//RTSP  服务开关
	APP_U8				bMmsOpen;		//no
	APP_U16				rtspPort;		//被动RTSP  服务端口
	APP_U8				bRtspAuth;		//是否认证
	APP_U8				bRtspPassive;	//0: 主动连服务器  1: 被动等待客户端连接
	APP_U16				maxPacketSize;	//RTSP MTU
	APP_U32				rtspActiveIp;	//主动连接时RTSP的IP地址
	APP_U16				rtspActiveport;//主动连接时RTSP的端口
	APP_U16				mmsPort;       //no
	
}MEDIA_SERVER_ST;


//GPS 参数
//PARAM_CFG_COMMAND_EN: GET_GPS_CONFIG    SET_GPS_CONFIG

typedef struct _APP_GPS_INFO_ST
{
	APP_U8		bGpsOpen;
	APP_U8		gpsStatus;
	APP_U16		serverPort;
	APP_U32		serverIP;
	APP_U8		bGpsOSD;
	APP_U8		gpsPlatform;
	APP_U8		phoneNum[11];
	APP_U8       	reportTime;	//
	APP_U8		res[2];
	//APP_U8		res[3];
}GPS_INFO_ST;


//3G 参数(高级)

typedef struct _APP_T3G_CONFIG_ST
{
	unsigned char      nSize;
	unsigned char       n3gOpen;					//3g 开关
	unsigned char       n3gType;					//3g 类型 0 evdo 1 wcdma 2 td
	unsigned char       n3gSecType;				    //3g 验证类型  0 自动选择 1 pap 2 chap
	unsigned char       n3gParam;		            //标记要用哪些参数 
	char        		sz3gUsr[32];				//3g 拨号用户名			
	char        		sz3gPsw[32];				//3g 拨号密码
	char        		sz3gApn[32];				//3g apn 
	char        		sz3gTelNum[16];           	//3g 拨号号码
	char        		sz3gLocalIp[16];
	char        		sz3gRemoteIp[16];
	char        		sz3gPara1[16];
	char        		sz3gPara2[16];
	unsigned char       res[16]; 
} T3G_CONFIG_ST;//200字节

//3G 状态参数
typedef struct _APP_T3G_INFO_ST
{
	unsigned char 	  moduleExits;   // 1 module exits else 0
	unsigned char     moduleStatus;  // 1 ok else 0
	unsigned char     simExits;      // 1 umi exits  else 0
	unsigned char     sigal;         // 
	unsigned char     serviceStatus; // srv status 
	// 0 no srv status
	// 1 a restrict srv status
	// 2 a good srv
	// 3 a restrict district srv
	// 4 sleep status
	unsigned char     roamStatus;    // 0 not roam 1 roam
	unsigned char     networkType ;  // 0 not service
	// 1 AMPS
	// 2 CDMA
	// 3 GSM/GPRS
	// 4 HDR
	// 5 WCDMA
	// 6 GPS
	// 7 GSM/WCDMA
	// 8 CDMA/HDR HYBIRD
	unsigned char     refresh      ;
	unsigned char     initModule   ;
	unsigned char     initDail	   ;
	unsigned char	  c3gstatus	   ;// 3G状态
	unsigned char     res          ;// 拨号失败次数,可以不使用
	char      softWareVer[128];   // software version
	char      hardWareVer[128];   // hardware version
	char      moduleType[32] ;   // module type
} T3G_INFO_ST, *PT3G_INFO_ST;

//3G  时间
typedef struct _APP_T3G_TIME_CTRL_ST//  结构调整,实现字节对齐
{
	APP_U8		t3gtimectrlopen;		// 3G拨号限时是否开启 0-关闭  1-开启
	APP_U8		t3gdaylimitedend;		// 标志位-每天限时拨号     0-未到  1-已到
	APP_U8		t3gmonthlimitedend;		// 标志位-每月限时拨号   0-未到  1-已到
	APP_U8		t3gdaylimited;			// 限制每天拨号时长   0-不限制
	E3G_TIME_CTRL_EN	t3gtimectrlmode;// 3G时间限制模式
	APP_UL32		t3gdialusedtime;		// 已经拨号时间
	APP_UL32		t3gmonthlimited;		// 限制每月拨号时长   0-不限制
	APP_U8		t3openlimitedmonth;		// 初次设定限时的月份
	APP_U8		t3openlimitedday;// 初次设定限时的号数
	APP_U8		revers[2];// 字节对齐
	TIME_SEGMENT_ST	t3gtimesegment[4];// 定义每天4个可设定时间段
} T3G_TIME_CTRL_ST;// 50个字节 = 1+4+4+1+1+1+4+1+1+4*(4+4)

//7、3G 总参数
//PARAM_CFG_COMMAND_EN  GET_3G_CONFIG   SET_3G_CONFIG
typedef struct _APP_T3Ginfo_ST
{
	T3G_CONFIG_ST  t_3ginfo;
	T3G_TIME_CTRL_ST   t_3gTime;
	T3G_INFO_ST	  t_3ginfo1;
}T3Ginfo_ST;


//--------------------------------------------------------------------




//----------------------------------VI、存储设置-------------------------------

//磁盘信息
//1、PARAM_CFG_COMMAND_EN:  GET_DISK_INFO   
typedef struct _APP_DISK_INFO_ST
{
	APP_U32	DiskNum;					//磁盘个数
	APP_U8	DiskType[8];				//磁盘类型
	APP_U32	TotalSize[8];				//磁盘总空间大小
	APP_U32	FreeSize[8];				//磁盘剩余空间
	APP_U8	Reserved[8];				//APP_U8	Reserved[4];
}DISK_INFO_ST;


//2、磁盘操作

typedef struct _APP_DISK_MANAGE_ST
{
	APP_U8	Cmd;
	APP_U8	Data;
	APP_U8	Reserved[126];
}DISK_MANAGE_ST;

//3、磁盘格式化进度
//PARAM_CFG_COMMAND_EN:  GET_FORMAT_PROCESS   
typedef struct _APP_DISK_FORMAT_PROCESS_ST
{
	APP_U32	DiskType;					//磁盘类型
	APP_U32	Status;						//格式化状态
	APP_U32	Process;					//进度()
	APP_U32	Reserved[5];
}DISK_FORMAT_PROCESS_ST;


//定时录像
typedef struct _APP_CH_TIMER_RECORD_CFG_ST		//sizeof() = 140
{
	APP_U16					KeepTime;								//保留天数(天)
	APP_U16					RecordSvaeMode;							//bit0 本地存储  bit1 ftp上传  bit2 email
	APP_U32					reserve[2];

	TIME_SEGMENT_ST		tSchTable1[8] ;							//某个通道的定时录像时间段1 (64)
	TIME_SEGMENT_ST		tSchTable2[8] ;							//某个通道的定时录像时间段2 (64)
}CH_TIMER_RECORD_CFG_ST;	

//4、录像计划
//PARAM_CFG_COMMAND_EN : SET_RECORD_CONFIG				GET_RECORD_CONFIG	(unsupport)
typedef struct _APP_TIMER_RECORD_CFG_ST			//sizeof() = 592
{
	APP_U8					PackageTime;	  						//打包时间(分钟)
	APP_U8 					OverWrite;        						//自动覆盖
	APP_U8     				MainVEnc;								//录像采用 0: 从码流  1: 主码流
	APP_U8     				CheckDisk;		
	APP_U16			        nSaveDays;//系统启动时进行磁盘扫描
	APP_U8     				reserve[24];

	CH_TIMER_RECORD_CFG_ST	ChRecordCfg[VIDEO_MAX_CHN];				//通道的录像参数(140 * 4 = 560)
}TIMER_RECORD_CFG_ST;


//定时抓拍
typedef struct _APP_CH_TIMER_SHOOT_CFG_ST		//sizeof() = 140
{
	APP_U16 					KeepTime;								//保留天数(天)
	APP_U16					ShootSaveMode;							//bit0 本地存储  bit1 ftp上传  bit2 email

	APP_U32					ShootNum;								//连续抓拍张数()
	float					ShootInterval;							//连续抓拍时间间隔(秒)  0: 连续抓拍

	TIME_SEGMENT_ST		tSchTable1[8] ;							//某个通道的定时抓拍时间段1 (40)
	TIME_SEGMENT_ST		tSchTable2[8] ;							//某个通道的定时抓拍时间段2 (40)
}CH_TIMER_SHOOT_CFG_ST;

//5、抓拍计划
//PARAM_CFG_COMMAND_EN: GET_SHOOT_CONFIG   SET_SHOOT_CONFIG
typedef struct _APP_TIMER_SHOOT_CFG_ST			//sizeof() = 592
{
	APP_U8 					OverWrite;								//自动覆盖
	APP_U8     				reserve[31];

	CH_TIMER_SHOOT_CFG_ST	ChShootCfg[VIDEO_MAX_CHN];				//通道的录像参数(92 * 4 = 368)
}TIMER_SHOOT_CFG_ST;

//------------------------------------------------------------------------------


//----------------------------------VII、报警设置-----------------------------

//报警联动操作
typedef struct _APP_ALARM_LINKAGE_ST 
{
	APP_U32 		Output;					//报警联动输出				     bit.0---bit.3	
	APP_U32 		AutoClearTime;		    //报警自动清除时间	

	APP_U32 		Record_PC;				//PC端报警联动录像通道			 bit.0---bit.3
	APP_U32 		Record_Dev;				//设备端报警联动录像通道		 bit.0---bit.3
	APP_U32		RecordSvaeMode;			//   bit1 email   bit2 ftp上传
	APP_U32		RecordTime;					//报警录像时间(秒)

	APP_U32		ShootSaveMode;				//    bit1 email  bit2 ftp上传
	APP_U32		ShootChns;					//抓拍通道(bit.0 ~ bit.3)
	APP_U32		ShootNum;					//连续抓拍张数()
	float		ShootInterval;				//连续抓拍时间间隔(秒)  0: 连续抓拍

	APP_U32		EMail;						//发送email
	APP_U32		Reserved;					//备用
} ALARM_LINKAGE_ST;


//1、移动侦测设置
//PARAM_CFG_COMMAND_EN: GET_VMOTION_CONFIG_NEW  SET_VMOTION_CONFIG_NEW
typedef struct _APP_VIDEO_MOTION_CONFIG_ST
{
	APP_U32			dwSize;						//本结构长度

	APP_U32 			DetectLevel;			    //移动检测灵敏度
	APP_U32 			DetectOpen;				    //移动检测开关

	AREA_RECT_ST		DetectArea[9];				//移动检测区域			

	TIME_SEGMENT_ST  tSchTable1[8] ;		    //移动检测时间表1				 0: 每天
	TIME_SEGMENT_ST  tSchTable2[8] ;		    //移动检测时间表1				 0: 每天

	ALARM_LINKAGE_ST	stAlarmLink;				//报警联动

	APP_U8			reseved[128];
}VIDEO_MOTION_CONFIG_ST ;


//2、 探头报警侦测设置()
//PARAM_CFG_COMMAND_EN: GET_SENSOR_ALARM_NEW 	SET_SENSOR_ALARM_NEW
typedef struct _APP_SENSOR_CONFIG_ST
{
	APP_U32			dwSize;						//本结构长度

	APP_U32			SensorType;					//0:常开探头	1:常闭探头
	APP_U32 			DetectOpen;				    //检测开关

	TIME_SEGMENT_ST  tSchTable1[8] ;		    //探头检测时间1				     0: 每天
	TIME_SEGMENT_ST  tSchTable2[8] ;		    //探头检测时间1				     0: 每天

	ALARM_LINKAGE_ST	stAlarmLink;				//报警联动

	APP_U8			reseved[32];
}SENSOR_CONFIG_ST;


//3、网络断线报警设置
//PARAM_CFG_COMMAND_EN: GET_NET_ALARM_CONFIG  SET_NET_ALARM_CONFIG
typedef struct _APP_NET_ALARM_CONFIG_ST 
{
	APP_U32				dwSize;						//本结构长度

	APP_U32 			DetectOpen;				    //检测开关
	
	ALARM_LINKAGE_ST	stAlarmLink;				//报警联动

	APP_U8				reseved[32];
} NET_ALARM_CONFIG_ST;


//4、视频丢失设置
//PARAM_CFG_COMMAND_EN:	GET_VLOST_CONFIG_NEW  SET_VLOST_CONFIG_NEW
typedef struct _APP_VIDEOLOSE_CONFIG_ST
{
	APP_U32				dwSize;						//本结构长度
	APP_U32				DetectOpen;					//视频丢失检测
	ALARM_LINKAGE_ST	stAlarmLink;				//报警联动
	APP_U8				reseved[32];
} VIDEOLOSE_CONFIG_ST;


//红外探头报警  (不支持)
typedef struct _APP_RF_ALARM_CONFIG_ST
{
	APP_U32			dwSize;						//本结构长度

	APP_U32			SensorType;					//0:常开探头	1:常闭探头
	APP_U32 			DetectOpen;				    //检测开关

	TIME_SEGMENT_ST  tSchTable1[8] ;		    //探头检测时间1				     0: 每天
	TIME_SEGMENT_ST  tSchTable2[8] ;		    //探头检测时间1				     0: 每天

	ALARM_LINKAGE_ST	stAlarmLink;				//报警联动

	APP_U8			reseved[32];
}RF_ALARM_CONFIG_ST;


//5、报警输出继电器控制
//PARAM_CFG_COMMAND_EN:	SET_OUTPUT_CTRL
typedef struct __APP_ALARM_OUTPUT_ST
{
	unsigned int nChn 	 : 16;  //Channel
	unsigned int nStatus : 16;  //alarm out put   0 : close out put    1 : open out put
}APP_ALARM_OUTPUT;

//
typedef struct _APP_ALARM_INOUT_NAME_ST
{
	APP_S8			alarmInName[SENSOR_MAX_CHN][MAX_NAME_LEN+2];	//探头名
	APP_S8			alarmOutName[OUTPUT_MAX_CHN][MAX_NAME_LEN+2];	//报警输出名
}ALARM_INOUT_NAME_ST;


//------------------------------------------------------------------------------

//---------------------------------VIII、串口设置--------------------------------

//1、串口设置
//PARAM_CFG_COMMAND_EN:  GET_COM_CONFIG    SET_COM_CONFIG
typedef struct __APP_COM_CONFIG
{
	APP_U32 			Baudrate;						//串口波特率				300---115200
	APP_U8				Databit;						//串口数据位长度			5=5位 6=6位 7=7位 8=8位	其他=8位
	APP_U8				Stopbit;						//停止位位数				1=1位 2=2位 其他=1位
	APP_U8				CheckType;						//校验						0=无  1=奇  2=偶  3=恒1  4=恒0 
	APP_U8				Flowctrl; 						//软流控/硬流控/无流控		备用
}APP_COM_CONFIG;


//------------------------------------------------------------------------------

//----------------------------------云台-------------------------------------

typedef struct _SERIAL_PROTOCOL_S
{
	APP_UL32 			baudrate : 20;						//波特率(bps) 300 ~ 115200
	APP_UL32			dataBits : 4;						//数据有几位  5: 5位 6: 6位 7: 7位 8: 8位
	APP_UL32			stopBits : 2;						//停止位:     1: 1位 2: 2位
	APP_UL32			parity   : 3;						//校验        0: 无  1: 奇  2:偶   3: 恒1  4:恒0 
	APP_UL32			flowControl : 2;					//流控        0: 无  1: 软流控     2:硬流控
}SERIAL_PROTOCOL_S;


typedef struct _DEV_SERIAL_CHN_S
{
	APP_U32				dwSize;

	APP_S32				bExist;							//是否存在: 只读

	APP_S8				szName[MAX_STRBUF];	
	
	SERIAL_PROTOCOL_S	stSerialProtocol;

	APP_U8				byRes[16];
}DEV_SERIAL_CHN_S;


//1、透明通讯
//PARAM_CFG_COMMAND_EN : SET_COMM_SENDDATA
typedef struct __APP_COMM_CTRL
{
	APP_U8				COMMNo;							//串口号	0 ~ 1
	APP_U8				AD[3];
	
	DEV_SERIAL_CHN_S	COMConfig;						//串口配置
	APP_U16				DataLength;						//数据长度
	APP_U8				Data[256];						//数据
	APP_U8				AE[2];
} APP_COMM_CTRL;


//2、云台控制
//PARAM_CFG_COMMAND_EN: SET_PAN_CTRL
typedef struct __APP_PAN_CTRL_ST
{
	APP_U8				ChannelNo;						//通道号	0 ~ PORTNUM - 1
	APP_U8				COMMNo;							//串口号	0 ~ 1
	APP_U8				PanType;						//云台协议序号 0 ~ YUNTAI_PROTOCOL_NUM - 1
	APP_U8				PanCmd;							//云台控制命令
	APP_U8				Data1;							//数据1		（预置、调用号）
	APP_U8				Data2;							//数据2		（备用）
}PAN_CTRL_ST;


//3、云台信息
//PARAM_CFG_COMMAND_EN:  GET_YUNTAI_CONFIG
typedef struct _APP_YUNTAI_INFO_ST
{
	APP_S8 		Name[YUNTAI_NAME_MAX_LEN + 1];				//云台名称
	APP_S8 		AD[3];
	APP_COM_CONFIG	ComSet;									//通讯协议<波特率...>
} YUNTAI_INFO_ST;

typedef struct _DAY_TIME_S
{
	APP_U32			week : 5;							//星期
	APP_U32			millisec : 10;						//毫秒: 0 ~ 999
	APP_U32			second : 6;							//秒:	0 ~ 59
	APP_U32			minute : 6;							//分:    0 ~ 59
	APP_U32			hour : 5;							//时:	0 ~ 23
}DAY_TIME_S; 


typedef struct _SEG_TIME_S
{
	APP_U8			bEnable;							//是否检测
	APP_U8			byTask;								//任务
	APP_U8			byRes[2];							//备用
	DAY_TIME_S		stBgnTime;							//开始时间
	DAY_TIME_S		stEndTime;							//结束时间
}SEG_TIME_S; 


//功能: 高清高速球参数 定时设置
//PARAM_CFG_COMMAND_EN 		CMD_GET_DOME_PTZ_CFG, CMD_SET_DOME_PTZ_CFG, 
typedef struct _PTZ_DOME_PARAM_S
{
	APP_U32				dwSize;

	APP_U8				bManualLimit;					//手动限位	 	1 开  	0 关
	APP_U8				bScanLimit;						//扫描限位	 	1 开  	0 关
	APP_U8				bAux1;							//辅助开关一 	1 开  	0 关
	APP_U8				bAux2;							//辅助开关二 	1 开  	0 关

	APP_U8				byHSpeed;						//水平扫描速度	1 - 40度设置
	APP_U8				byVSpeed;						//俯仰扫描速度	1 - 20度设置
	APP_U8				byWatchMode;					//看守位模式	0  无
														//	  			1  预置位1   																																																												
														//	  			2  预置位2   																																																												
														//	  			3  预置位3   																																																												
														//	  			4  预置位4   																																																												
														//	  			5  预置位5   																																																												
														//	  			6  预置位6   																																																												
														//	  			7  预置位7   																																																												
														//	  			8  预置位8   																																																												
														//	  			9  自动扫描  																																																												
														//	  			10 削苹果扫描 																																																												
														//	  			11 预置位巡视1																																																												
														//	 			12 预置位巡视2		
	APP_U8				byWatchTime;					//看守位起动时间2-60分

	SEG_TIME_S			stSegTime[16];					//定时设置 目前支持 (0-4) 

	APP_U8				bLampOn;
	APP_U8				bBrushOn;			

	APP_U8				byRes[34];
}APP_PTZ_DOME_PARAM;



//功能: 每条巡航
//命令: NCC_PTZ_CRUISE_SET
//说明: 设置		 
typedef struct _PTZ_CRUISE_PATH_S
{
	APP_S8				szName[MAX_PRESET_NAME];
	
	APP_U8				dwPresetArray[32];				//预置点   1 ~ 255
	APP_U8				dwPresetSpeed[32];				//速度	   1 ~ 100
	APP_U16				dwPresetDelay[32];				//停留时间 1 ~ 3600(秒)

	APP_U8				byRes[8]; //[0]set时为CruiseRoute，[1]set时为PresetIndex;
}PTZ_CRUISE_PATH_S;


typedef struct _PTZ_CRUISE_S
{
	PTZ_CRUISE_PATH_S   stCruisePath[4];
	unsigned char    	reserved[256];
}APP_PTZ_CRUISE;


//---------------------------------------------------------------



//----------------------------IX、其它参数-----------------------

//1、TW2824参数
//PARAM_CFG_COMMAND_EN: SET_TW2824

typedef struct _APP_TW2824_CTRL_ST
{
	APP_U8				Page;							//页
	APP_U8 				RegNo;							//寄存器
	APP_U8 				Data;							//数据
	APP_U8				AD[1];
	APP_U32				nReserved[8];					//保留
}TW2824_CTRL_ST;


//视频定向发送设置
typedef struct _APP_SEND_DEST_ST
{	
	APP_U32			dwSet[VIDEO_MAX_CHN];			//发送开关
	APP_U32			dwStreamSend[VIDEO_MAX_CHN];	//发送开关
	APP_U32			dwStreamDestIP[VIDEO_MAX_CHN];	//目的IP,网络字节顺序
	APP_U32			dwStreamDestPort[VIDEO_MAX_CHN];//目的端口
	char			res[16];
}SEND_DEST_ST;


//2、定时重启
//PARAM_CFG_COMMAND_EN: GET_AUTO_RESET  SET_AUTO_RESET
typedef struct _APP_AUTO_RESET_TIME_ST
{
	APP_U8			open;				//bit 1:hour,bit 2:day,bit 3:week	
	APP_U8			second;
	APP_U8			minute;
	APP_U8			hour;
	APP_U8			week;				//0 星期日，1 星期一，2 星期二 

	APP_U8			res[3];
}AUTO_RESET_TIME_ST;


//3、扩展信息结构
//PARAM_CFG_COMMAND_EN: GET_EXTINFO   SET_EXTINFO
typedef struct _APP_EXTINFO_CONFIG_ST
{
	APP_U32					nSize;						//结构大小(1328)

	NTP_CONFIG_ST			ntpConfig;					//ntp
	
	UPNP_CONFIG_ST			upnpConfig;					//upnp
		
	EMAIL_CONFIG_ST			mailConfig;					//MAIL
	
	ALARM_INOUT_NAME_ST		alarmInOutName;			//ALARM IN/OUT NAME
	
	WF_NET_CONFIG_ST		wfConfig;					//WF NETWORK
	
	SEND_DEST_ST			sendDest;					//SEND DEST
	
	AUTO_RESET_TIME_ST		autoResetTime;				//AUTO RESET 

	FTP_CONFIG_ST			ftpCfg;						//FTP 服务器配置

	MEDIA_SERVER_ST			mediaServer;				//流媒体服务

	GPS_INFO_ST				gpsInfo;					//GPS 信息,

	APP_U8              	reportWanIp;           	 	// 用于上报公网IP地址
	APP_U8               	reportIpInterval;          // 
	char					reserve[6];

}EXTINFO_CONFIG_ST;


//4、用户数据
//PARAM_CFG_COMMAND_EN:  GET_USERDATA		SET_USERDATA
typedef struct __APP_USERDATA_CONFIG_ST
{
	int		nDataLen;
	char	userData[252];
}USERDATA_CONFIG_ST;


//5、设备所有参数（sizeof = 10248）
//PARAM_CFG_COMMAND_EN: GET_ALL_CONFIGPARAM

typedef struct _APP_DVS_Parameter_ST
{
	APP_U32					dwSize;						//本结构长度

    char                	sysName[DEV_NAME_LEN + 4];

	APP_U8					sysInputNum;		        //路数
	APP_U8					sysType;			        //机器型号	0:CIF  1:HD1  2:D1  
	APP_U8					sysVideoFormat;		        //编码格式	0:D1,1:HD1,2:CIF,3:VGA 4:HVGA 5:CVGA 6:QCIF 7:QVGA
    APP_U8					sysLanguage;		        //语言

    APP_U32					sysStandard;		        //制式		0:PAL  1:NTSC

    APP_U32      				sysID;  	            //每台机器有个ID
	APP_U32      				sysVer;  	            //软件版本  

	VIDEO_CONFIG_ST	    	sysVideo[VIDEO_MAX_CHN];	
	VIDEO_MOTION_CONFIG_ST	sysVideoMotion[VIDEO_MAX_CHN];
	VIDEOMASK_CONFIG_ST    	sysVideoMask[VIDEO_MAX_CHN];	
	VIDEOLOSE_CONFIG_ST    	sysVideoLose[VIDEO_MAX_CHN];	
	AUDIO_CONFIG_ST		    sysAudio[AUDIO_MAX_CHN];
	NET_CONFIG_ST			sysNet;
	APP_COM_CONFIG			sysCom[COM_MAX];
	SENSOR_CONFIG_ST	    sysSensor[SENSOR_MAX_CHN];
	USER_CONFIG_ST			sysUser[USER_MAX_NUM];

    EXTINFO_CONFIG_ST		sysExtConfig;				//扩展参数

	DISK_INFO_ST			DiskInfo;					//存储设备信息

	TIMER_RECORD_CFG_ST		TimerRecordCfg;
	TIMER_SHOOT_CFG_ST		TimerShootCfg;				//定时抓拍参数

	RF_ALARM_CONFIG_ST		rfAlarmCfg[RF_SENSOR_MAX_CHN];	//无线探头报警
	NET_ALARM_CONFIG_ST		netAlarmCfg;					//网络断线报警

	USERDATA_CONFIG_ST		userData;					//用户数据

	APP_U32					YunTaiProtocolNum;
	YUNTAI_INFO_ST 			YunTaiInfo[PTZ_PROTOCOL_MAX_NUM];	

	APP_U8					reserved[256];				//备用

} DVS_Parameter_ST;


//备用1
typedef struct _APP_CONFIG_ENCPAR_ST
{ 
    APP_U32 u32PicWidth;   
    APP_U32 u32PicHeight;     
    APP_U32 u32CapSel; 
    APP_U32 u32StaticParam; 
    APP_U32 u32TargetBitrate; 
    APP_U32 u32TargetFramerate; 
    APP_U32 u32Gop;             
    APP_U32 u32MaxQP;        
    APP_U32 u32PreferFrameRate; 
    APP_U32 u32Strategy; 
    APP_U32 u32VencMode;        
    APP_U32 u32BaseQP;         
    APP_U32 u32MaxBitRate;   
}CONFIG_ENCPAR_ST; 

//备用2
typedef struct _APP_PING_CONFIG_ST
{
	APP_S32	bPingOpen;
	APP_S32	bDisconnectReboot;
	APP_S32	bDisconnectAlarm;
	APP_U32	dwPingNum;
	APP_U32   dwPingPeriod;
	char	szPingServer[64];
}PING_CONFIG_ST;
//---------------------------------------------------------------

 

//计划时间: 周计划时间
typedef struct _SCHWEEK_TIME_S
{
	SEG_TIME_S		stSegTime[7][4];					//0: 星期天，1: 周一，2: 周二
}SCHWEEK_TIME_S;

#define MAX_RF_ALM_NUM   		8
#define MAX_RF_NAME				32

typedef struct _ALM_RF_CFG_S
{
	char szName[MAX_RF_NAME];//名称
	char szAddress[14];//地址
	int blearn;// 0 学习  1 学习中   2 学习成功
	int bEnable;//开关
	int bUniteMDEnable;//同步移动侦测
	SCHWEEK_TIME_S	stSchTime;	//布防时间
}ALM_RF_CFG;

//屏蔽
typedef struct _CHN_MASK_S
{
	unsigned long			dwChns1;							//bit.0 ~ bit.31  表示通道 1  ~ 32
	unsigned long			dwChns2;							//bit.0 ~ bit.31  表示通道 33 ~ 64
}CHN_MASK_S;

//报警联动处理
typedef struct _ALARM_HANDLE_S
{
	CHN_MASK_S				stMaskAlarmOut;						//报警触发设备的输出通道,报警触发的输出,为1表示触发该输出
	unsigned long			dwAlarmOutTime;						//报警输出的时间(秒)

	CHN_MASK_S				stMaskAlarmRec;						//联动录像通道 
	unsigned long			dwRecSaveMode;						//bit0: 本地存储(当本地有存储设备时, 此位固定为1)  
																//bit2: ftp上传   
	unsigned long			dwRecTime;							//录像的时间(秒)

	CHN_MASK_S				stMaskAlarmSnap;					//联动抓拍
	unsigned long			dwSnapSaveMode;						//bit0: 本地存储(当本地有存储设备时, 此位固定为1)  
																//bit1: email 
																//bit2: ftp上传   

	unsigned long			dwSnapNum;							//连续抓拍张数
	unsigned long			dwSnapInterval;						//连续抓拍时间间隔(毫秒)  0: 连续抓拍

	unsigned long			dwUserData1;						//用户数据1(联动PC录像通道)
	unsigned long			dwUserData2;						//用户数据2

	unsigned long			dwSnapDealy;						//延迟抓拍

	unsigned char			bSendEmail;							//联动发送email
	unsigned char			byRes[251];							//备用
}ALARM_HANDLE_S;

//-----------------------RF 报警
typedef struct _ALM_RFIN_NEW_S
{
	unsigned long			dwSize;
	ALM_RF_CFG    			stAlarmRfInfo[MAX_RF_ALM_NUM];
	ALARM_HANDLE_S 			stAlarmHandle;
	unsigned char 			nRemoteLearn;
	unsigned char			byRes[11];
}ALM_RFIN_NEW;

typedef struct _ALM_RFIN_RF_NEW_S
{
	char 					num;
	ALM_RF_CFG    			stAlarmRfInfo;
}ALM_RFIN_RF_NEW;

#endif


