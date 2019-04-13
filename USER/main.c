#include "led.h"
#include "beep.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "ILI93xx.h"
#include "usart.h"	 
#include "24cxx.h"
#include "flash.h"
#include "touch.h"
#include "sram.h"
#include "timer.h"
#include "sdio_sdcard.h"
#include "malloc.h"
#include "GUI.h"
#include "ff.h"
#include "exfuns.h"
#include "w25qxx.h"
#include "includes.h"
#include "myframewin.h"
#include "WM.h"
#include "DIALOG.h"
#include "databox.h"

extern int interface;
extern int saloon[4],kitchen[4],shower[4],bed[4];
extern u8 my_result1[100];


//任务优先级
#define START_TASK_PRIO				3
//任务堆栈大小	
#define START_STK_SIZE 				1024
//任务控制块
OS_TCB StartTaskTCB;
//任务堆栈	
CPU_STK START_TASK_STK[START_STK_SIZE];
//任务函数
void start_task(void *p_arg);

//TOUCH任务
//设置任务优先级
#define TOUCH_TASK_PRIO				4
//任务堆栈大小
#define TOUCH_STK_SIZE				128
//任务控制块
OS_TCB TouchTaskTCB;
//任务堆栈
CPU_STK TOUCH_TASK_STK[TOUCH_STK_SIZE];
//touch任务
void touch_task(void *p_arg);

//LED0任务
//设置任务优先级
#define LED0_TASK_PRIO 				5
//任务堆栈大小
#define LED0_STK_SIZE				128
//任务控制块
OS_TCB Led0TaskTCB;
//任务堆栈
CPU_STK LED0_TASK_STK[LED0_STK_SIZE];
//led0任务
void led0_task(void *p_arg);

//EMWINDEMO任务
//设置任务优先级
#define EMWINDEMO_TASK_PRIO			6
//任务堆栈大小
#define EMWINDEMO_STK_SIZE			2048
//任务控制块
OS_TCB EmwindemoTaskTCB;
//任务堆栈
CPU_STK EMWINDEMO_TASK_STK[EMWINDEMO_STK_SIZE];
//emwindemo_task任务
void emwindemo_task(void *p_arg);

//设置任务优先级
#define Showtime_TASK_PRIO			7
//任务堆栈大小
#define Showtime_STK_SIZE			1024
//任务控制块
OS_TCB Showtime_TaskTCB;
//任务堆栈
CPU_STK Showtime_TASK_STK[Showtime_STK_SIZE];
//emwindemo_task任务
void Showtime_task(void *p_arg);

//设置任务优先级
#define Getdata_TASK_PRIO			7
//任务堆栈大小
#define Getdata_STK_SIZE			1024
//任务控制块
OS_TCB Getdata_TaskTCB;
//任务堆栈
CPU_STK Getdata_TASK_STK[Getdata_STK_SIZE];
//emwindemo_task任务
void Getdata_task(void *p_arg);

int main(void)
{	
	OS_ERR err;
	CPU_SR_ALLOC();
	
	delay_init();	    	//延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(4800);	 	//串口初始化为115200
 	LED_Init();			    //LED端口初始化
	TFTLCD_Init();			//LCD初始化	
	KEY_Init();	 			//按键初始化
	BEEP_Init();			//初始化蜂鸣器
	FSMC_SRAM_Init();		//初始化SRAM
	my_mem_init(SRAMIN); 	//初始化内部内存池
	my_mem_init(SRAMEX);  	//初始化外部内存池
	
	exfuns_init();			//为fatfs文件系统分配内存
	f_mount(fs[0],"0:",1);	//挂载SD卡
	f_mount(fs[1],"1:",1);	//挂载FLASH
	TP_Init();				//触摸屏初始化
	
	OSInit(&err);			//初始化UCOSIII
	OS_CRITICAL_ENTER();	//进入临界区
	//创建开始任务
	OSTaskCreate((OS_TCB 	* )&StartTaskTCB,		//任务控制块
				 (CPU_CHAR	* )"start task", 		//任务名字
                 (OS_TASK_PTR )start_task, 			//任务函数
                 (void		* )0,					//传递给任务函数的参数
                 (OS_PRIO	  )START_TASK_PRIO,     //任务优先级
                 (CPU_STK   * )&START_TASK_STK[0],	//任务堆栈基地址
                 (CPU_STK_SIZE)START_STK_SIZE/10,	//任务堆栈深度限位
                 (CPU_STK_SIZE)START_STK_SIZE,		//任务堆栈大小
                 (OS_MSG_QTY  )0,					//任务内部消息队列能够接收的最大消息数目,为0时禁止接收消息
                 (OS_TICK	  )0,					//当使能时间片轮转时的时间片长度，为0时为默认长度，
                 (void   	* )0,					//用户补充的存储区
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //任务选项
                 (OS_ERR 	* )&err);				//存放该函数错误时的返回值
	OS_CRITICAL_EXIT();	//退出临界区	 
	OSStart(&err);  //开启UCOSIII
	while(1);
}

//开始任务函数
void start_task(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;

	CPU_Init();
#if OS_CFG_STAT_TASK_EN > 0u
   OSStatTaskCPUUsageInit(&err);  	//统计任务                
#endif
	
#ifdef CPU_CFG_INT_DIS_MEAS_EN		//如果使能了测量中断关闭时间
    CPU_IntDisMeasMaxCurReset();	
#endif

#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //当使用时间片轮转的时候
	 //使能时间片轮转调度功能,时间片长度为1个系统时钟节拍，既1*5=5ms
	OSSchedRoundRobinCfg(DEF_ENABLED,1,&err); 
     OSTaskTimeQuantaSet(&Getdata_TaskTCB,1,&err);
      OSTaskTimeQuantaSet(&Showtime_TaskTCB,1,&err);	
#endif		
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC,ENABLE);//开启CRC时钟
	WM_SetCreateFlags(WM_CF_MEMDEV); 	//启动所有窗口的存储设备
	GUI_Init();  			//STemWin初始化

	OS_CRITICAL_ENTER();	//进入临界区
	//STemWin Demo任务	
	OSTaskCreate((OS_TCB*     )&EmwindemoTaskTCB,		
				 (CPU_CHAR*   )"Emwindemo task", 		
                 (OS_TASK_PTR )emwindemo_task, 			
                 (void*       )0,					
                 (OS_PRIO	  )EMWINDEMO_TASK_PRIO,     
                 (CPU_STK*    )&EMWINDEMO_TASK_STK[0],	
                 (CPU_STK_SIZE)EMWINDEMO_STK_SIZE/10,	
                 (CPU_STK_SIZE)EMWINDEMO_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,  					
                 (void*       )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR*     )&err);
								 	OSTaskCreate((OS_TCB*     )&Showtime_TaskTCB,		
		(CPU_CHAR*   )"Showtime task", 		
                 (OS_TASK_PTR )Showtime_task, 			
                 (void*       )0,					
                 (OS_PRIO	  )Showtime_TASK_PRIO,     
                 (CPU_STK*    )&Showtime_TASK_STK[0],	
                 (CPU_STK_SIZE)Showtime_STK_SIZE/10,	
                 (CPU_STK_SIZE)Showtime_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,  					
                 (void*       )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR*     )&err);	
  OSTaskCreate((OS_TCB*     )&Getdata_TaskTCB,		
				 (CPU_CHAR*   )"Getdata task", 		
                 (OS_TASK_PTR )Getdata_task, 			
                 (void*       )0,					
                 (OS_PRIO	  )Getdata_TASK_PRIO,     
                 (CPU_STK*    )&Getdata_TASK_STK[0],	
                 (CPU_STK_SIZE)Getdata_STK_SIZE/10,	
                 (CPU_STK_SIZE)Getdata_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,  					
                 (void*       )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR*     )&err);
	//触摸屏任务
	OSTaskCreate((OS_TCB*     )&TouchTaskTCB,		
				 (CPU_CHAR*   )"Touch task", 		
                 (OS_TASK_PTR )touch_task, 			
                 (void*       )0,					
                 (OS_PRIO	  )TOUCH_TASK_PRIO,     
                 (CPU_STK*    )&TOUCH_TASK_STK[0],	
                 (CPU_STK_SIZE)TOUCH_STK_SIZE/10,	
                 (CPU_STK_SIZE)TOUCH_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,  					
                 (void*       )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR*     )&err);			 
	//LED0任务
	OSTaskCreate((OS_TCB*     )&Led0TaskTCB,		
				 (CPU_CHAR*   )"Led0 task", 		
                 (OS_TASK_PTR )led0_task, 			
                 (void*       )0,					
                 (OS_PRIO	  )LED0_TASK_PRIO,     
                 (CPU_STK*    )&LED0_TASK_STK[0],	
                 (CPU_STK_SIZE)LED0_STK_SIZE/10,	
                 (CPU_STK_SIZE)LED0_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,  					
                 (void*       )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR*     )&err);
	OS_TaskSuspend((OS_TCB*)&StartTaskTCB,&err);		//挂起开始任务			 
	OS_CRITICAL_EXIT();	//退出临界区
}

//EMWINDEMO任务
void emwindemo_task(void *p_arg)
{
		GUI_CURSOR_Show(); 	
	//更换皮肤
	BUTTON_SetDefaultSkin(BUTTON_SKIN_FLEX); 
	CHECKBOX_SetDefaultSkin(CHECKBOX_SKIN_FLEX);
	DROPDOWN_SetDefaultSkin(DROPDOWN_SKIN_FLEX);
	FRAMEWIN_SetDefaultSkin(FRAMEWIN_SKIN_FLEX);
	HEADER_SetDefaultSkin(HEADER_SKIN_FLEX);
	MENU_SetDefaultSkin(MENU_SKIN_FLEX);
	MULTIPAGE_SetDefaultSkin(MULTIPAGE_SKIN_FLEX);
	PROGBAR_SetDefaultSkin(PROGBAR_SKIN_FLEX);
	RADIO_SetDefaultSkin(RADIO_SKIN_FLEX);
	SCROLLBAR_SetDefaultSkin(SCROLLBAR_SKIN_FLEX);
	SLIDER_SetDefaultSkin(SLIDER_SKIN_FLEX);
	SPINBOX_SetDefaultSkin(SPINBOX_SKIN_FLEX); 
	while(1)
	{
		 my_gui_create();
	}
}

//TOUCH任务
void touch_task(void *p_arg)
{
	OS_ERR err;
	while(1)
	{
		GUI_TOUCH_Exec();	
		OSTimeDlyHMSM(0,0,0,5,OS_OPT_TIME_PERIODIC,&err);//延时5ms
	}
}

void Showtime_task(void *p_arg)
{	
	OS_ERR err;	
	while(1)
	{
	  if(interface==2)//界面4
		 { 	
			 LCD_ShowxNum(130,270,saloon[0],5,16,0);
			 LCD_ShowxNum(130,320,saloon[1],5,16,0);
			 LCD_ShowxNum(130,370,saloon[2],5,16,0);
			 LCD_ShowxNum(130,420,saloon[3],5,16,0);
	   }	 
		else if(interface==3)//界面4
		 { 		 
			 LCD_ShowxNum(130,270,kitchen[0],5,16,0);
			 LCD_ShowxNum(130,320,kitchen[1],5,16,0);
			 LCD_ShowxNum(130,370,kitchen[2],5,16,0);
			 LCD_ShowxNum(130,420,kitchen[3],5,16,0);
	   }	 
	  else if(interface==4)//界面4
		 { 		
       LCD_ShowxNum(130,270,shower[0],5,16,0);
			 LCD_ShowxNum(130,320,shower[1],5,16,0);
			 LCD_ShowxNum(130,370,shower[2],5,16,0);
			 LCD_ShowxNum(130,420,shower[3],5,16,0);			 
	   }	 
		 else if(interface==5)//界面4
		 { 		
       LCD_ShowxNum(130,270,bed[0],5,16,0);
			 LCD_ShowxNum(130,320,bed[1],5,16,0);
			 LCD_ShowxNum(130,370,bed[2],5,16,0);
			 LCD_ShowxNum(130,420,bed[3],5,16,0);					 
	   }	 
	}
}

void Getdata_task(void  *p_arg) 
{
	OS_ERR  err;
	while(1)
	 {	
	  cheak_databox();
    OSTimeDlyHMSM(0,0,0,5,OS_OPT_TIME_PERIODIC,&err);//延时5ms	
   }
}
//LED0任务
void led0_task(void *p_arg)
{
	OS_ERR err;
	while(1)
	{
		OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_PERIODIC,&err);//延时500ms
	}
}
