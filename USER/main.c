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


//�������ȼ�
#define START_TASK_PRIO				3
//�����ջ��С	
#define START_STK_SIZE 				1024
//������ƿ�
OS_TCB StartTaskTCB;
//�����ջ	
CPU_STK START_TASK_STK[START_STK_SIZE];
//������
void start_task(void *p_arg);

//TOUCH����
//�����������ȼ�
#define TOUCH_TASK_PRIO				4
//�����ջ��С
#define TOUCH_STK_SIZE				128
//������ƿ�
OS_TCB TouchTaskTCB;
//�����ջ
CPU_STK TOUCH_TASK_STK[TOUCH_STK_SIZE];
//touch����
void touch_task(void *p_arg);

//LED0����
//�����������ȼ�
#define LED0_TASK_PRIO 				5
//�����ջ��С
#define LED0_STK_SIZE				128
//������ƿ�
OS_TCB Led0TaskTCB;
//�����ջ
CPU_STK LED0_TASK_STK[LED0_STK_SIZE];
//led0����
void led0_task(void *p_arg);

//EMWINDEMO����
//�����������ȼ�
#define EMWINDEMO_TASK_PRIO			6
//�����ջ��С
#define EMWINDEMO_STK_SIZE			2048
//������ƿ�
OS_TCB EmwindemoTaskTCB;
//�����ջ
CPU_STK EMWINDEMO_TASK_STK[EMWINDEMO_STK_SIZE];
//emwindemo_task����
void emwindemo_task(void *p_arg);

//�����������ȼ�
#define Showtime_TASK_PRIO			7
//�����ջ��С
#define Showtime_STK_SIZE			1024
//������ƿ�
OS_TCB Showtime_TaskTCB;
//�����ջ
CPU_STK Showtime_TASK_STK[Showtime_STK_SIZE];
//emwindemo_task����
void Showtime_task(void *p_arg);

//�����������ȼ�
#define Getdata_TASK_PRIO			7
//�����ջ��С
#define Getdata_STK_SIZE			1024
//������ƿ�
OS_TCB Getdata_TaskTCB;
//�����ջ
CPU_STK Getdata_TASK_STK[Getdata_STK_SIZE];
//emwindemo_task����
void Getdata_task(void *p_arg);

int main(void)
{	
	OS_ERR err;
	CPU_SR_ALLOC();
	
	delay_init();	    	//��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(4800);	 	//���ڳ�ʼ��Ϊ115200
 	LED_Init();			    //LED�˿ڳ�ʼ��
	TFTLCD_Init();			//LCD��ʼ��	
	KEY_Init();	 			//������ʼ��
	BEEP_Init();			//��ʼ��������
	FSMC_SRAM_Init();		//��ʼ��SRAM
	my_mem_init(SRAMIN); 	//��ʼ���ڲ��ڴ��
	my_mem_init(SRAMEX);  	//��ʼ���ⲿ�ڴ��
	
	exfuns_init();			//Ϊfatfs�ļ�ϵͳ�����ڴ�
	f_mount(fs[0],"0:",1);	//����SD��
	f_mount(fs[1],"1:",1);	//����FLASH
	TP_Init();				//��������ʼ��
	
	OSInit(&err);			//��ʼ��UCOSIII
	OS_CRITICAL_ENTER();	//�����ٽ���
	//������ʼ����
	OSTaskCreate((OS_TCB 	* )&StartTaskTCB,		//������ƿ�
				 (CPU_CHAR	* )"start task", 		//��������
                 (OS_TASK_PTR )start_task, 			//������
                 (void		* )0,					//���ݸ��������Ĳ���
                 (OS_PRIO	  )START_TASK_PRIO,     //�������ȼ�
                 (CPU_STK   * )&START_TASK_STK[0],	//�����ջ����ַ
                 (CPU_STK_SIZE)START_STK_SIZE/10,	//�����ջ�����λ
                 (CPU_STK_SIZE)START_STK_SIZE,		//�����ջ��С
                 (OS_MSG_QTY  )0,					//�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
                 (OS_TICK	  )0,					//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
                 (void   	* )0,					//�û�����Ĵ洢��
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //����ѡ��
                 (OS_ERR 	* )&err);				//��Ÿú�������ʱ�ķ���ֵ
	OS_CRITICAL_EXIT();	//�˳��ٽ���	 
	OSStart(&err);  //����UCOSIII
	while(1);
}

//��ʼ������
void start_task(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;

	CPU_Init();
#if OS_CFG_STAT_TASK_EN > 0u
   OSStatTaskCPUUsageInit(&err);  	//ͳ������                
#endif
	
#ifdef CPU_CFG_INT_DIS_MEAS_EN		//���ʹ���˲����жϹر�ʱ��
    CPU_IntDisMeasMaxCurReset();	
#endif

#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //��ʹ��ʱ��Ƭ��ת��ʱ��
	 //ʹ��ʱ��Ƭ��ת���ȹ���,ʱ��Ƭ����Ϊ1��ϵͳʱ�ӽ��ģ���1*5=5ms
	OSSchedRoundRobinCfg(DEF_ENABLED,1,&err); 
     OSTaskTimeQuantaSet(&Getdata_TaskTCB,1,&err);
      OSTaskTimeQuantaSet(&Showtime_TaskTCB,1,&err);	
#endif		
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC,ENABLE);//����CRCʱ��
	WM_SetCreateFlags(WM_CF_MEMDEV); 	//�������д��ڵĴ洢�豸
	GUI_Init();  			//STemWin��ʼ��

	OS_CRITICAL_ENTER();	//�����ٽ���
	//STemWin Demo����	
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
	//����������
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
	//LED0����
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
	OS_TaskSuspend((OS_TCB*)&StartTaskTCB,&err);		//����ʼ����			 
	OS_CRITICAL_EXIT();	//�˳��ٽ���
}

//EMWINDEMO����
void emwindemo_task(void *p_arg)
{
		GUI_CURSOR_Show(); 	
	//����Ƥ��
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

//TOUCH����
void touch_task(void *p_arg)
{
	OS_ERR err;
	while(1)
	{
		GUI_TOUCH_Exec();	
		OSTimeDlyHMSM(0,0,0,5,OS_OPT_TIME_PERIODIC,&err);//��ʱ5ms
	}
}

void Showtime_task(void *p_arg)
{	
	OS_ERR err;	
	while(1)
	{
	  if(interface==2)//����4
		 { 	
			 LCD_ShowxNum(130,270,saloon[0],5,16,0);
			 LCD_ShowxNum(130,320,saloon[1],5,16,0);
			 LCD_ShowxNum(130,370,saloon[2],5,16,0);
			 LCD_ShowxNum(130,420,saloon[3],5,16,0);
	   }	 
		else if(interface==3)//����4
		 { 		 
			 LCD_ShowxNum(130,270,kitchen[0],5,16,0);
			 LCD_ShowxNum(130,320,kitchen[1],5,16,0);
			 LCD_ShowxNum(130,370,kitchen[2],5,16,0);
			 LCD_ShowxNum(130,420,kitchen[3],5,16,0);
	   }	 
	  else if(interface==4)//����4
		 { 		
       LCD_ShowxNum(130,270,shower[0],5,16,0);
			 LCD_ShowxNum(130,320,shower[1],5,16,0);
			 LCD_ShowxNum(130,370,shower[2],5,16,0);
			 LCD_ShowxNum(130,420,shower[3],5,16,0);			 
	   }	 
		 else if(interface==5)//����4
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
    OSTimeDlyHMSM(0,0,0,5,OS_OPT_TIME_PERIODIC,&err);//��ʱ5ms	
   }
}
//LED0����
void led0_task(void *p_arg)
{
	OS_ERR err;
	while(1)
	{
		OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_PERIODIC,&err);//��ʱ500ms
	}
}
