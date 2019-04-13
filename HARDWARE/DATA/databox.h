#ifndef _DATABOX_H
#define _DATABOX_H


#define  Rdata_tip      0x72 //即r字符
#define  Tdata_tip      0x74 //即t字符

#define  Main_Address   0x47 //G
#define  F_Address_0    0x48 //H  客厅
#define  F_Address_1    0x49 //I  厨房
#define  F_Address_2    0x4a //J  浴室
#define  F_Address_3    0x4b //K  卧室

#define  command_0      0x67 //g  类似心跳包的作用，定时给主机发送从机采集的环境数据
#define  command_1      0x68 //h  开灯
#define  command_1s     0x69 //i  关灯
#define  command_2      0x6a //j  开空调
#define  command_2s     0x6b //k  关空调
#define  command_3      0x6c //l  开风扇
#define  command_3s     0x6d //m  关风扇
#define  command_4      0x6e //n  开油烟机
#define  command_4s     0x6f //o  关油烟机
#define  command_open   0x70 //p  电器全开
#define  command_close  0x71 //q  电器全关
#define  command_warn   0x73 //s  烟雾传感器采集值过大，向主机发送警告
#define  command_warns  0x75 //u  关闭警告
#define  command_reset  0x76 //v  主机发送重置命令从机初始化

void cheak_databox(void);
void send_databox(char a,char b,char c,int d,char *e,int  way);
typedef struct 
 {
	unsigned   address;
  unsigned 	char funtion_part;
	int       data_length;		  
 }DATABOX;



#endif
