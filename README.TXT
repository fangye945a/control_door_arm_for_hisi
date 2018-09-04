/*********************************************************
 > File name : README.TXT
 > Create time : 2018-05-21 星期1 15:12
 > Author : 方烨
 > Email : fangye945@qq.com
 ********************************************************/

1、编译程序
	① 安装编译环境
	② cd ./src
	③ make
	④ 生成 mqtt_door 可执行文件
	
2、配置程序运行环境
   ① 复制工程lib目录下的lib16cv300app.so文件 至板子 /lib 目录
   
   ② 复制工程src 目录下的 mqtt_door 文件至板子 /home 目录
   
   ③ 在开发板/home 目录下创建文件名为db的文件夹，并将工程lib目录下的videoDoorDevice.db文件复制至该目录。
   
 
3、配置工作串口及启动文件
   ① vi /etc/inittab   使用'#'注释掉 ::respawn:/sbin/getty -L ttyS000 115200 vt100 -n root -I "Auto login as root ..."
   
   ② vi /etc/init.d/rcS  
   在末尾添加如下两行:    
    cd /home
	#./mqtt_door &
   ③执行 reboot 命令    重启设备
   
   ④cd /home   执行 ./mqtt_door     测试程序是否能够正常运行，查看刷卡模块是否在闪烁。若在闪烁则执行第五步，否则联系开发人员解决。
   
   ⑤ 测试门禁功能是否完成，有无bug。测试通过则执行第六步，否则联系开发人员解决。
   
   ⑥ 将第②步中#./mqtt_door &的 注释去掉，重启设备开机自动运行程序。
