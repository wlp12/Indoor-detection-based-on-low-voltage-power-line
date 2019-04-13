#include "myframewin.h"
#include "GUI.h"
#include "FRAMEWIN.h"
#include "string.h"
#include "DIALOG.h"
#include "BUTTON.h"
#include "beep.h"
#include "usart.h"
#include "CHECKBOX.h"
#include "delay.h"
#include "includes.h"
#include "ILI93xx.h"
#include "databox.h"

int interface=1,flag,value_1,value_2[5],value_3[5],value_4[5],value_5[5];

WM_HWIN CreateFramewin_1(void);
WM_HWIN CreateFramewin_2(void);
WM_HWIN CreateFramewin_3(void);
WM_HWIN CreateFramewin_4(void);
WM_HWIN CreateFramewin_5(void);

/***************界面一*******************/
#define ID_BUTTON_1      (GUI_ID_USER + 0x02)
#define ID_BUTTON_2      (GUI_ID_USER + 0x03)
#define ID_BUTTON_3      (GUI_ID_USER + 0x04)
#define ID_BUTTON_4      (GUI_ID_USER + 0x05)

/***************界面二********************/
#define ID_DROPDOWN_1    (GUI_ID_USER + 0x0C)
#define ID_DROPDOWN_2    (GUI_ID_USER + 0x0D)
#define ID_DROPDOWN_3    (GUI_ID_USER + 0x0E)


/***************界面三*******************/
#define ID_DROPDOWN_6    (GUI_ID_USER + 0x20)
#define ID_DROPDOWN_7    (GUI_ID_USER + 0x21)

/***************界面四*******************/
#define ID_BUTTON_11     (GUI_ID_USER +0x3B)
#define ID_DROPDOWN_11   (GUI_ID_USER + 0x30)
#define ID_DROPDOWN_12   (GUI_ID_USER + 0x31)


/***************界面五*******************/
#define ID_DROPDOWN_16   (GUI_ID_USER + 0x40)
#define ID_DROPDOWN_17   (GUI_ID_USER + 0x41)
#define ID_DROPDOWN_18   (GUI_ID_USER + 0x42)

/***************共用********************/
#define ID_FRAMEWIN_0    (GUI_ID_USER + 0x10)
#define ID_TEXT_0        (GUI_ID_USER + 0x11)
#define ID_DROPDOWN_0    (GUI_ID_USER + 0x12)
#define ID_TEXT_1        (GUI_ID_USER + 0x13)
#define ID_TEXT_2        (GUI_ID_USER + 0x14)
#define ID_TEXT_3 			 (GUI_ID_USER + 0x15)
#define ID_TEXT_4        (GUI_ID_USER + 0x16)
/***********对话框资源表**************/
//主机界面
static const GUI_WIDGET_CREATE_INFO _aDialogCreate_1[] = {
  { FRAMEWIN_CreateIndirect, "Framewin", ID_FRAMEWIN_0,   0,   0,  800, 480, 0, 0x64, 0 }, //方框
	{ BUTTON_CreateIndirect,   "Button",   ID_BUTTON_1,   550,  75,  180,  70, 0,  0x0, 0 },//按键
  { BUTTON_CreateIndirect,   "Button",   ID_BUTTON_2,   550, 175,  180,  70, 0,  0x0, 0 },//按键
	{ BUTTON_CreateIndirect,   "Button",   ID_BUTTON_3,   550, 275,  180,  70, 0,  0x0, 0 },//按键
	{ BUTTON_CreateIndirect,   "Button",   ID_BUTTON_4,   350, 330,  150,  70, 0,  0x0, 0 },//按键
	{ DROPDOWN_CreateIndirect, "Dropdown", ID_DROPDOWN_0,  20,   5,  240, 160, 0,  0x0, 0 },
};
//客厅界面
static const GUI_WIDGET_CREATE_INFO _aDialogCreate_2[] = {
  { FRAMEWIN_CreateIndirect, "Framewin", ID_FRAMEWIN_0,   0,   0,  800, 480, 0, 0x64, 0 },//方框
 	{ DROPDOWN_CreateIndirect, "Dropdown", ID_DROPDOWN_1, 350,  20,  180,  80, 0,  0x0, 0 },
	{ DROPDOWN_CreateIndirect, "Dropdown", ID_DROPDOWN_2, 550,  20,  180,  80, 0,  0x0, 0 },
	{ DROPDOWN_CreateIndirect, "Dropdown", ID_DROPDOWN_3, 520, 170,  220,  80, 0,  0x0, 0 },
	{ TEXT_CreateIndirect,     "Text",     ID_TEXT_1,      20, 220,  100,  30, 0, 0x64, 0 },	
	{ TEXT_CreateIndirect,     "Text",     ID_TEXT_2,      20, 270,  100,  30, 0, 0x64, 0 },
  { TEXT_CreateIndirect,     "Text",     ID_TEXT_3,      20, 320,  100,  30, 0, 0x64, 0 },
	{ TEXT_CreateIndirect,     "Text",     ID_TEXT_4,      20, 370,  100,  30, 0, 0x64, 0 },
  { DROPDOWN_CreateIndirect, "Dropdown", ID_DROPDOWN_0,  20,   5,  240, 160, 0,  0x0, 0 },
};
//厨房界面
static const GUI_WIDGET_CREATE_INFO _aDialogCreate_3[] = {
  { FRAMEWIN_CreateIndirect, "Framewin", ID_FRAMEWIN_0,    0,   0, 800, 480,  0, 0x64, 0 }, //方框
	{ DROPDOWN_CreateIndirect, "Dropdown", ID_DROPDOWN_6,  350,  20, 180,  80,  0,  0x0, 0 },
	{ DROPDOWN_CreateIndirect, "Dropdown", ID_DROPDOWN_7,  550,  20, 180,  80,  0,  0x0, 0 },
	{ TEXT_CreateIndirect,     "Text",     ID_TEXT_1,      20, 220,  100,  30, 0, 0x64, 0 },	
	{ TEXT_CreateIndirect,     "Text",     ID_TEXT_2,      20, 270,  100,  30, 0, 0x64, 0 },
  { TEXT_CreateIndirect,     "Text",     ID_TEXT_3,      20, 320,  100,  30, 0, 0x64, 0 },
	{ TEXT_CreateIndirect,     "Text",     ID_TEXT_4,      20, 370,  100,  30, 0, 0x64, 0 },
  { DROPDOWN_CreateIndirect, "Dropdown", ID_DROPDOWN_0,   20,   5, 240, 160,  0,  0x0, 0 },	
}; 
//浴室界面
static const GUI_WIDGET_CREATE_INFO _aDialogCreate_4[] = {
  { FRAMEWIN_CreateIndirect, "Framewin", ID_FRAMEWIN_0,    0,   0,  800, 480, 0, 0x64, 0 }, //MY_HOME方框
	{ TEXT_CreateIndirect,     "Text",     ID_TEXT_1,      20, 220,  100,  30, 0, 0x64, 0 },	
	{ TEXT_CreateIndirect,     "Text",     ID_TEXT_2,      20, 270,  100,  30, 0, 0x64, 0 },
  { TEXT_CreateIndirect,     "Text",     ID_TEXT_3,      20, 320,  100,  30, 0, 0x64, 0 },
	{ TEXT_CreateIndirect,     "Text",     ID_TEXT_4,      20, 370,  100,  30, 0, 0x64, 0 },
	{ DROPDOWN_CreateIndirect, "Dropdown", ID_DROPDOWN_11,  350,  20, 180,  80, 0,  0x0, 0 },
	{ DROPDOWN_CreateIndirect, "Dropdown", ID_DROPDOWN_12,  550,  20, 180,  80, 0,  0x0, 0 },
  { DROPDOWN_CreateIndirect, "Dropdown", ID_DROPDOWN_0,    20,   5, 240, 160, 0,  0x0, 0 },
}; 
//卧室界面
static const GUI_WIDGET_CREATE_INFO _aDialogCreate_5[] = {
  { FRAMEWIN_CreateIndirect, "Framewin", ID_FRAMEWIN_0,    0,   0, 800, 480, 0, 0x64, 0 }, //MY_HOME方框
	{ TEXT_CreateIndirect,     "Text",     ID_TEXT_1,      20, 220,  100,  30, 0, 0x64, 0 },	
	{ TEXT_CreateIndirect,     "Text",     ID_TEXT_2,      20, 270,  100,  30, 0, 0x64, 0 },
  { TEXT_CreateIndirect,     "Text",     ID_TEXT_3,      20, 320,  100,  30, 0, 0x64, 0 },
	{ TEXT_CreateIndirect,     "Text",     ID_TEXT_4,      20, 370,  100,  30, 0, 0x64, 0 },
 	{ DROPDOWN_CreateIndirect, "Dropdown", ID_DROPDOWN_16, 350,  20, 180,  80, 0,  0x0, 0 },
	{ DROPDOWN_CreateIndirect, "Dropdown", ID_DROPDOWN_17, 550,  20, 180,  80, 0,  0x0, 0 },
	{ DROPDOWN_CreateIndirect, "Dropdown", ID_DROPDOWN_18, 550, 170, 180,  80, 0,  0x0, 0 },
  { DROPDOWN_CreateIndirect, "Dropdown", ID_DROPDOWN_0,   20,   5, 240, 160, 0,  0x0, 0 },
}; 
//对话框回调函数
static void _cbDialog_1(WM_MESSAGE * pMsg);//界面1
static void _cbDialog_2(WM_MESSAGE * pMsg);//界面2
static void _cbDialog_3(WM_MESSAGE * pMsg);//界面3
static void _cbDialog_4(WM_MESSAGE * pMsg);//界面4
static void _cbDialog_5(WM_MESSAGE * pMsg);//界面5

//界面1
static void _cbDialog_1(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int     NCode;
  int     Id;
  int     abs;
  switch (pMsg->MsgId) {                   //区分各种消息类型
  case WM_INIT_DIALOG:                     //外观初始化
   
    hItem = pMsg->hWin;
    FRAMEWIN_SetTitleHeight(hItem, 70);
    FRAMEWIN_SetText(hItem, "Welcome to the Home of chenguoqi ");
    FRAMEWIN_SetTextAlign(hItem, GUI_TA_VCENTER | GUI_TA_CENTER	);
    FRAMEWIN_SetFont(hItem, GUI_FONT_32_1);
    FRAMEWIN_SetTextColor(hItem, 0x00000008);
	
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
    BUTTON_SetText(hItem, "Open  all EA");
    BUTTON_SetFont(hItem, GUI_FONT_24_1);
   
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_2);
    BUTTON_SetText(hItem, "Close all EA");
    BUTTON_SetFont(hItem, GUI_FONT_24_1);

	  hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_3);
    BUTTON_SetText(hItem, "Sure change");
    BUTTON_SetFont(hItem, GUI_FONT_24_1);
	
	  hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_4);
    BUTTON_SetText(hItem, "Close beep");
    BUTTON_SetFont(hItem, GUI_FONT_24_1);
	
	  hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0);
		DROPDOWN_SetFont(hItem, &GUI_Font24B_ASCII);
		DROPDOWN_SetAutoScroll(hItem,1);	//启用自动使用滚动条
		DROPDOWN_AddString(hItem, "Pasition");
		DROPDOWN_AddString(hItem, "Saloon");
		DROPDOWN_AddString(hItem, "Kitchen");		
		DROPDOWN_AddString(hItem, "Shower room");
		DROPDOWN_AddString(hItem, "Bedroom");
		DROPDOWN_AddString(hItem, "Main part");
		
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_BUTTON_1: // Notifications sent by 'Button'
      switch(NCode) {
		  case WM_NOTIFICATION_CLICKED:
				   flag=1;				  
			break;
		  case WM_NOTIFICATION_RELEASED:
			break;
		  }
      break;
    case ID_BUTTON_2: // Notifications sent by 'Button'
      switch(NCode) {
		  case WM_NOTIFICATION_CLICKED:
					 flag=2;  
			break;
		  case WM_NOTIFICATION_RELEASED:
			break;
      }
      break; 
		case ID_BUTTON_3: // Notifications sent by 'Button'
      switch(NCode) {
		  case WM_NOTIFICATION_CLICKED:
					 if(flag==1) { 
           send_databox(Rdata_tip,F_Address_0,command_open,1,"p",1); 
					 delay_ms(1000); 
			     send_databox(Rdata_tip,F_Address_1,command_open,1,"p",1);
           delay_ms(1000);						 
			     send_databox(Rdata_tip,F_Address_2,command_open,1,"p",1);
           delay_ms(1000);						 
					 send_databox(Rdata_tip,F_Address_3,command_open,1,"p",1);
           delay_ms(1000);				 	 
					 send_databox(Rdata_tip,F_Address_0,command_open,1,"p",1); 
			     send_databox(Rdata_tip,F_Address_1,command_open,1,"p",1);					 
			     send_databox(Rdata_tip,F_Address_2,command_open,1,"p",1);						 
					 send_databox(Rdata_tip,F_Address_3,command_open,1,"p",1);				 	 	 
					 }
           else if(flag==2) { 
           send_databox(Rdata_tip,F_Address_0,command_close,1,"q",1);
           delay_ms(1000);							 
			     send_databox(Rdata_tip,F_Address_1,command_close,1,"q",1);
           delay_ms(1000);							 
			     send_databox(Rdata_tip,F_Address_2,command_close,1,"q",1);
           delay_ms(1000);							 
			     send_databox(Rdata_tip,F_Address_3,command_close,1,"q",1);
					 delay_ms(1000);	 
           send_databox(Rdata_tip,F_Address_0,command_close,1,"q",1);						 
			     send_databox(Rdata_tip,F_Address_1,command_close,1,"q",1);							 
			     send_databox(Rdata_tip,F_Address_2,command_close,1,"q",1);							 
			     send_databox(Rdata_tip,F_Address_3,command_close,1,"q",1);			 					 
					 }
					 else
					 break;
		  case WM_NOTIFICATION_RELEASED:
			break;
      }
      break; 
	  case ID_BUTTON_4: // Notifications sent by 'Button'
      switch(NCode) {
		  case WM_NOTIFICATION_CLICKED:
				   send_databox(Rdata_tip,F_Address_0,command_warns,1,"u",1);  
			     send_databox(Rdata_tip,F_Address_1,command_warns,1,"u",1);
			     send_databox(Rdata_tip,F_Address_2,command_warns,1,"u",1);
			     send_databox(Rdata_tip,F_Address_3,command_warns,1,"u",1);
			break;
		  case WM_NOTIFICATION_RELEASED:
			break;
		  }
      break;
		case ID_DROPDOWN_0: // Notifications sent by 'Button'
      switch(NCode) 
					{
						case WM_NOTIFICATION_CLICKED:
							break;
						case WM_NOTIFICATION_RELEASED:
							break;
						case WM_NOTIFICATION_SEL_CHANGED:
                 hItem=WM_GetDialogItem(pMsg->hWin,ID_DROPDOWN_0);
                 value_1=DROPDOWN_GetSel(hItem);
						     if(value_1==1)
								 {
									 GUI_EndDialog(pMsg->hWin, 0);	
                   interface=2;
			             delay_ms(50);
                   CreateFramewin_2();		 
								 }
								 else  if(value_1==2)
								 {
									 GUI_EndDialog(pMsg->hWin, 0);	
                   interface=3;
			             delay_ms(50);
                   CreateFramewin_3();		 
								 }
								 else  if(value_1==3)
								 {
									 GUI_EndDialog(pMsg->hWin, 0);	
                   interface=4;
			             delay_ms(50);
                   CreateFramewin_4();		 
								 }
								  else  if(value_1==4)
								 {
									 GUI_EndDialog(pMsg->hWin, 0);	
                   interface=5;
			             delay_ms(50);
                   CreateFramewin_5();		 
								 }
								 else  if(value_1==5)
								 {
									 GUI_EndDialog(pMsg->hWin, 0);	
                   interface=1;
			             delay_ms(50);
                   CreateFramewin_1();		 
								 }
								 else
							   break;
					}
		 break;	  
    default:
    WM_DefaultProc(pMsg);
    break;
  }
}}
//界面2
static void _cbDialog_2(WM_MESSAGE * pMsg) 
{
  WM_HWIN hItem;
  int     NCode;
  int     Id;
	
  switch (pMsg->MsgId) {                   //区分各种消息类型
  case WM_INIT_DIALOG:                     //外观初始化
   
    hItem = pMsg->hWin;
    FRAMEWIN_SetTitleHeight(hItem, 50);
    FRAMEWIN_SetText(hItem, "Welcome to the Saloon ");
    FRAMEWIN_SetTextAlign(hItem, GUI_TA_VCENTER | GUI_TA_CENTER	);
    FRAMEWIN_SetFont(hItem, GUI_FONT_32_1);
    FRAMEWIN_SetTextColor(hItem, 0x00000008);
	
	  hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
    TEXT_SetFont(hItem, GUI_FONT_20_1);
	  TEXT_SetText(hItem, "Temperature:");  	
	 
	  hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_2);
    TEXT_SetFont(hItem, GUI_FONT_20_1);
	  TEXT_SetText(hItem, "   Humidity:");  	
	
	  hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_3);
    TEXT_SetFont(hItem, GUI_FONT_20_1);
	  TEXT_SetText(hItem, "       Adcx:");  	
	
	  hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_4);
    TEXT_SetFont(hItem, GUI_FONT_20_1);
	  TEXT_SetText(hItem, "      Smoke:"); 
	
    hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_1);
		DROPDOWN_SetFont(hItem, &GUI_Font24B_ASCII);
		DROPDOWN_SetAutoScroll(hItem,1);	//启用自动使用滚动条
		DROPDOWN_AddString(hItem, "LED");
		DROPDOWN_AddString(hItem, "Open  LED");
	  DROPDOWN_AddString(hItem, "Close LED");		
		
    hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_2);
		DROPDOWN_SetFont(hItem, &GUI_Font24B_ASCII);
		DROPDOWN_SetAutoScroll(hItem,1);	//启用自动使用滚动条
		DROPDOWN_AddString(hItem, "Fan");
		DROPDOWN_AddString(hItem, "Open  Fan");
	  DROPDOWN_AddString(hItem, "Close Fan");		
	
    hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_3);
		DROPDOWN_SetFont(hItem, &GUI_Font24B_ASCII);
		DROPDOWN_SetAutoScroll(hItem,1);	//启用自动使用滚动条
		DROPDOWN_AddString(hItem, "Air   conditioner");
		DROPDOWN_AddString(hItem, "Open  airconditioner");
	  DROPDOWN_AddString(hItem, "Close airconditioner");			 
	 
	  hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0);
		DROPDOWN_SetFont(hItem, &GUI_Font24B_ASCII);
		DROPDOWN_SetAutoScroll(hItem,1);	//启用自动使用滚动条
		DROPDOWN_AddString(hItem, "Pasition");
		DROPDOWN_AddString(hItem, "Saloon");
		DROPDOWN_AddString(hItem, "Kitchen");		
		DROPDOWN_AddString(hItem, "Shower room");
		DROPDOWN_AddString(hItem, "bedroom");
		DROPDOWN_AddString(hItem, "Main part");
		
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {   
		case ID_DROPDOWN_0: // Notifications sent by 'Button'
      switch(NCode) 
					{
						case WM_NOTIFICATION_CLICKED:
							break;
						case WM_NOTIFICATION_RELEASED:
							break;
						case WM_NOTIFICATION_SEL_CHANGED:
                 hItem=WM_GetDialogItem(pMsg->hWin,ID_DROPDOWN_0);
                 value_1=DROPDOWN_GetSel(hItem);
						      if(value_1==1)
								 {
									 GUI_EndDialog(pMsg->hWin, 0);	
                   interface=2;
			             delay_ms(50);
                   CreateFramewin_2();		 
								 }
								 else  if(value_1==2)
								 {
									 GUI_EndDialog(pMsg->hWin, 0);	
                   interface=3;
			             delay_ms(50);
                   CreateFramewin_3();		 
								 }
								 else  if(value_1==3)
								 {
									 GUI_EndDialog(pMsg->hWin, 0);	
                   interface=4;
			             delay_ms(50);
                   CreateFramewin_4();		 
								 }
								 else  if(value_1==4)
								 {
									 GUI_EndDialog(pMsg->hWin, 0);	
                   interface=5;
			             delay_ms(50);
                   CreateFramewin_5();		 
								 }
								 else  if(value_1==5)
								 {
									 GUI_EndDialog(pMsg->hWin, 0);	
                   interface=1;
			             delay_ms(50);
                   CreateFramewin_1();		 
								 }
								 else
							   break;
					}
		 break;
  case ID_DROPDOWN_1: // Notifications sent by 'Button'
      switch(NCode) 
					{
						case WM_NOTIFICATION_CLICKED:
							break;
						case WM_NOTIFICATION_RELEASED:
							break;
						case WM_NOTIFICATION_SEL_CHANGED:
                 hItem=WM_GetDialogItem(pMsg->hWin,ID_DROPDOWN_1);
                 value_2[0]=DROPDOWN_GetSel(hItem);
						     if(value_2[0]==1)
								 {
									send_databox(Rdata_tip,F_Address_0,command_1,1,"h",1); 
								 }
								 else  if(value_2[0]==2)
								 {
									send_databox(Rdata_tip,F_Address_0,command_1s,1,"i",1); 		 
								 }
								 else
							   break;
					}
		  break;	  					
	case ID_DROPDOWN_2: // Notifications sent by 'Button'
      switch(NCode) 
					{
						case WM_NOTIFICATION_CLICKED:
							break;
						case WM_NOTIFICATION_RELEASED:
							break;
						case WM_NOTIFICATION_SEL_CHANGED:
                 hItem=WM_GetDialogItem(pMsg->hWin,ID_DROPDOWN_2);
                 value_2[1]=DROPDOWN_GetSel(hItem);
						     if(value_2[1]==1)
								 {
									send_databox(Rdata_tip,F_Address_0,command_3,1,"l",1);  
								 }
								 else  if(value_2[1]==2)
								 {
									send_databox(Rdata_tip,F_Address_0,command_3s,1,"m",1);  		 
								 }
								 else
							   break;
					}
		 break;	
  case ID_DROPDOWN_3: // Notifications sent by 'Button'
      switch(NCode) 
					{
						case WM_NOTIFICATION_CLICKED:
							break;
						case WM_NOTIFICATION_RELEASED:
							break;
						case WM_NOTIFICATION_SEL_CHANGED:
                 hItem=WM_GetDialogItem(pMsg->hWin,ID_DROPDOWN_3);
                 value_2[2]=DROPDOWN_GetSel(hItem);
						     if(value_2[2]==1)
								 {
									send_databox(Rdata_tip,F_Address_0,command_2,1,"j",1); 
								 }
								 else  if(value_2[2]==2)
								 {
									send_databox(Rdata_tip,F_Address_0,command_2s,1,"k",1);  		 
								 }
								 else
							   break;
					}
		 break;	  										
    default:
    WM_DefaultProc(pMsg);
    break;
  }
  }
}
//界面3
static void _cbDialog_3(WM_MESSAGE * pMsg)
{
  WM_HWIN hItem;
  int     NCode;
  int     Id;
  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:    //初始化消息,创建窗口/控件时有效,比如在这里设置一些控件的初始参数
    
    hItem = pMsg->hWin;
    FRAMEWIN_SetTitleHeight(hItem, 50);
    FRAMEWIN_SetText(hItem, "Welcome to the Kitchen ");
    FRAMEWIN_SetTextAlign(hItem, GUI_TA_VCENTER | GUI_TA_CENTER	);
    FRAMEWIN_SetFont(hItem, GUI_FONT_32_1);
    FRAMEWIN_SetTextColor(hItem, 0x00000008);
  
   hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
    TEXT_SetFont(hItem, GUI_FONT_20_1);
	  TEXT_SetText(hItem, "Temperature:");  	
	 
	  hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_2);
    TEXT_SetFont(hItem, GUI_FONT_20_1);
	  TEXT_SetText(hItem, "   Humidity:");  	
	
	  hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_3);
    TEXT_SetFont(hItem, GUI_FONT_20_1);
	  TEXT_SetText(hItem, "       Adcx:");  	
	
	  hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_4);
    TEXT_SetFont(hItem, GUI_FONT_20_1);
	  TEXT_SetText(hItem, "      Smoke:"); 	 
	
    hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_6);
		DROPDOWN_SetFont(hItem, &GUI_Font24B_ASCII);
		DROPDOWN_SetAutoScroll(hItem,1);	//启用自动使用滚动条
		DROPDOWN_AddString(hItem, "Cooker hood");
		DROPDOWN_AddString(hItem, "Open  cooker");
	  DROPDOWN_AddString(hItem, "Close cooker");		
		
    hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_7);
		DROPDOWN_SetFont(hItem, &GUI_Font24B_ASCII);
		DROPDOWN_SetAutoScroll(hItem,1);	//启用自动使用滚动条
		DROPDOWN_AddString(hItem, "Fan");
		DROPDOWN_AddString(hItem, "Open  Fan");
	  DROPDOWN_AddString(hItem, "Close Fan");		
	 
	  hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0);
		DROPDOWN_SetFont(hItem, &GUI_Font24B_ASCII);
		DROPDOWN_SetAutoScroll(hItem,1);	//启用自动使用滚动条
		DROPDOWN_AddString(hItem, "Pasition");
		DROPDOWN_AddString(hItem, "Saloon");
		DROPDOWN_AddString(hItem, "Kitchen");		
		DROPDOWN_AddString(hItem, "Shower room");
		DROPDOWN_AddString(hItem, "bedroom");
		DROPDOWN_AddString(hItem, "Main part");	
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {   
		case ID_DROPDOWN_0: // Notifications sent by 'Button'
      switch(NCode) 
					{
						case WM_NOTIFICATION_CLICKED:
							break;
						case WM_NOTIFICATION_RELEASED:
							break;
						case WM_NOTIFICATION_SEL_CHANGED:
                 hItem=WM_GetDialogItem(pMsg->hWin,ID_DROPDOWN_0);
                 value_1=DROPDOWN_GetSel(hItem);
						    if(value_1==1)
								 {
									 GUI_EndDialog(pMsg->hWin, 0);	
                   interface=2;
			             delay_ms(50);
                   CreateFramewin_2();		 
								 }
								 else  if(value_1==2)
								 {
									 GUI_EndDialog(pMsg->hWin, 0);	
                   interface=3;
			             delay_ms(50);
                   CreateFramewin_3();		 
								 }
								 else  if(value_1==3)
								 {
									 GUI_EndDialog(pMsg->hWin, 0);	
                   interface=4;
			             delay_ms(50);
                   CreateFramewin_4();		 
								 }
								  else  if(value_1==4)
								 {
									 GUI_EndDialog(pMsg->hWin, 0);	
                   interface=5;
			             delay_ms(50);
                   CreateFramewin_5();		 
								 }
								 else  if(value_1==5)
								 {
									 GUI_EndDialog(pMsg->hWin, 0);	
                   interface=1;
			             delay_ms(50);
                   CreateFramewin_1();		 
								 }
								 else
							   break;
					}
		 break;
  case ID_DROPDOWN_6: // Notifications sent by 'Button'
      switch(NCode) 
					{
						case WM_NOTIFICATION_CLICKED:
							break;
						case WM_NOTIFICATION_RELEASED:
							break;
						case WM_NOTIFICATION_SEL_CHANGED:
                 hItem=WM_GetDialogItem(pMsg->hWin,ID_DROPDOWN_6);
                 value_3[0]=DROPDOWN_GetSel(hItem);
						     if(value_3[0]==1)
								 {
								  send_databox(Rdata_tip,F_Address_1,command_4,1,"n",1); 
								 }
								 else  if(value_3[0]==2)
								 {
									send_databox(Rdata_tip,F_Address_1,command_4s,1,"o",1);  		 
								 }
								 else
							   break;
					}
		  break;	  					
	case ID_DROPDOWN_7: // Notifications sent by 'Button'
      switch(NCode) 
					{
						case WM_NOTIFICATION_CLICKED:
							break;
						case WM_NOTIFICATION_RELEASED:
							break;
						case WM_NOTIFICATION_SEL_CHANGED:
                 hItem=WM_GetDialogItem(pMsg->hWin,ID_DROPDOWN_7);
                 value_3[1]=DROPDOWN_GetSel(hItem);
						     if(value_3[1]==1)
								 {
									send_databox(Rdata_tip,F_Address_1,command_3,1,"l",1);  
								 }
								 else  if(value_3[1]==2)
								 {
									send_databox(Rdata_tip,F_Address_1,command_3s,1,"m",1); 		 
								 }
								 else
							   break;
					}
		 break;	
    default:
    WM_DefaultProc(pMsg);
    break;
  }
}
}

//界面4
static void _cbDialog_4(WM_MESSAGE * pMsg) 
{
  WM_HWIN hItem;
  int     NCode;
  int     Id;
	
  switch (pMsg->MsgId) {                   //区分各种消息类型
  case WM_INIT_DIALOG:                     //外观初始化
   
    hItem = pMsg->hWin;
    FRAMEWIN_SetTitleHeight(hItem, 50);
    FRAMEWIN_SetText(hItem, "Welcome to the Shower room");
    FRAMEWIN_SetTextAlign(hItem, GUI_TA_VCENTER | GUI_TA_CENTER	);
    FRAMEWIN_SetFont(hItem, GUI_FONT_32_1);
    FRAMEWIN_SetTextColor(hItem, 0x00000008);
  
   hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
    TEXT_SetFont(hItem, GUI_FONT_20_1);
	  TEXT_SetText(hItem, "Temperature:");  	
	 
	  hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_2);
    TEXT_SetFont(hItem, GUI_FONT_20_1);
	  TEXT_SetText(hItem, "   Humidity:");  	
	
	  hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_3);
    TEXT_SetFont(hItem, GUI_FONT_20_1);
	  TEXT_SetText(hItem, "       Adcx:");  	
	
	  hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_4);
    TEXT_SetFont(hItem, GUI_FONT_20_1);
	  TEXT_SetText(hItem, "      Smoke:"); 
	
    hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_11);
		DROPDOWN_SetFont(hItem, &GUI_Font24B_ASCII);
		DROPDOWN_SetAutoScroll(hItem,1);	//启用自动使用滚动条
		DROPDOWN_AddString(hItem, "LED");
		DROPDOWN_AddString(hItem, "Open  LED");
	  DROPDOWN_AddString(hItem, "Close LED");		
		
    hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_12);
		DROPDOWN_SetFont(hItem, &GUI_Font24B_ASCII);
		DROPDOWN_SetAutoScroll(hItem,1);	//启用自动使用滚动条
		DROPDOWN_AddString(hItem, "Fan");
		DROPDOWN_AddString(hItem, "Open  Fan");
	  DROPDOWN_AddString(hItem, "Close Fan");		
	 
	  hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0);
		DROPDOWN_SetFont(hItem, &GUI_Font24B_ASCII);
		DROPDOWN_SetAutoScroll(hItem,1);	//启用自动使用滚动条
		DROPDOWN_AddString(hItem, "Pasition");
		DROPDOWN_AddString(hItem, "Saloon");
		DROPDOWN_AddString(hItem, "Kitchen");		
		DROPDOWN_AddString(hItem, "Shower room");
		DROPDOWN_AddString(hItem, "bedroom");
		DROPDOWN_AddString(hItem, "Main part");
				
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {   
		case ID_DROPDOWN_0: // Notifications sent by 'Button'
      switch(NCode) 
					{
						case WM_NOTIFICATION_CLICKED:
							break;
						case WM_NOTIFICATION_RELEASED:
							break;
						case WM_NOTIFICATION_SEL_CHANGED:
                 hItem=WM_GetDialogItem(pMsg->hWin,ID_DROPDOWN_0);
                 value_1=DROPDOWN_GetSel(hItem);
						    if(value_1==1)
								 {
									 GUI_EndDialog(pMsg->hWin, 0);	
                   interface=2;
			             delay_ms(50);
                   CreateFramewin_2();		 
								 }
								 else  if(value_1==2)
								 {
									 GUI_EndDialog(pMsg->hWin, 0);	
                   interface=3;
			             delay_ms(50);
                   CreateFramewin_3();		 
								 }
								 else  if(value_1==3)
								 {
									 GUI_EndDialog(pMsg->hWin, 0);	
                   interface=4;
			             delay_ms(50);
                   CreateFramewin_4();		 
								 }
								  else  if(value_1==4)
								 {
									 GUI_EndDialog(pMsg->hWin, 0);	
                   interface=5;
			             delay_ms(50);
                   CreateFramewin_5();		 
								 }
								 else  if(value_1==5)
								 {
									 GUI_EndDialog(pMsg->hWin, 0);	
                   interface=1;
			             delay_ms(50);
                   CreateFramewin_1();		 
								 }
								 else
							   break;
					}
		 break;
  case ID_DROPDOWN_11: // Notifications sent by 'Button'
      switch(NCode) 
					{
						case WM_NOTIFICATION_CLICKED:
							break;
						case WM_NOTIFICATION_RELEASED:
							break;
						case WM_NOTIFICATION_SEL_CHANGED:
                 hItem=WM_GetDialogItem(pMsg->hWin,ID_DROPDOWN_11);
                 value_4[0]=DROPDOWN_GetSel(hItem);
						     if(value_4[0]==1)
								 {
									send_databox(Rdata_tip,F_Address_2,command_1,1,"h",1); 
								 }
								 else  if(value_4[0]==2)
								 {
									send_databox(Rdata_tip,F_Address_2,command_1s,1,"i",1);  		 
								 }
								 else
							   break;
					}
		  break;	  					
	case ID_DROPDOWN_12: // Notifications sent by 'Button'
      switch(NCode) 
					{
						case WM_NOTIFICATION_CLICKED:
							break;
						case WM_NOTIFICATION_RELEASED:
							break;
						case WM_NOTIFICATION_SEL_CHANGED:
                 hItem=WM_GetDialogItem(pMsg->hWin,ID_DROPDOWN_12);
                 value_4[1]=DROPDOWN_GetSel(hItem);
						     if(value_4[1]==1)
								 {
									send_databox(Rdata_tip,F_Address_2,command_3,1,"l",1);  
								 }
								 else  if(value_4[1]==2)
								 {
									send_databox(Rdata_tip,F_Address_2,command_3s,1,"m",1);  		 
								 }
								 else
							   break;
					}
		 break;										
    default:
    WM_DefaultProc(pMsg);
    break;
  }
  }
}

//界面5
static void _cbDialog_5(WM_MESSAGE * pMsg) 
{
  WM_HWIN hItem;
  int     NCode;
  int     Id;
	
  switch (pMsg->MsgId) {                   //区分各种消息类型
  case WM_INIT_DIALOG:                     //外观初始化
   
    hItem = pMsg->hWin;
    FRAMEWIN_SetTitleHeight(hItem, 50);
    FRAMEWIN_SetText(hItem, "Welcome to the bedroom");
    FRAMEWIN_SetTextAlign(hItem, GUI_TA_VCENTER | GUI_TA_CENTER	);
    FRAMEWIN_SetFont(hItem, GUI_FONT_32_1);
    FRAMEWIN_SetTextColor(hItem, 0x00000008);
  
	  hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
    TEXT_SetFont(hItem, GUI_FONT_20_1);
	  TEXT_SetText(hItem, "Temperature:");  	
	 
	  hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_2);
    TEXT_SetFont(hItem, GUI_FONT_20_1);
	  TEXT_SetText(hItem, "   Humidity:");  	
	
	  hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_3);
    TEXT_SetFont(hItem, GUI_FONT_20_1);
	  TEXT_SetText(hItem, "       Adcx:");  	
	
	  hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_4);
    TEXT_SetFont(hItem, GUI_FONT_20_1);
	  TEXT_SetText(hItem, "      Smoke:"); 
	
    hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_16);
	  DROPDOWN_SetFont(hItem, &GUI_Font24B_ASCII);
		DROPDOWN_SetAutoScroll(hItem,1);	//启用自动使用滚动条
		DROPDOWN_AddString(hItem, "LED");
		DROPDOWN_AddString(hItem, "Open  LED");
	  DROPDOWN_AddString(hItem, "Close LED");		
		
    hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_17);
		DROPDOWN_SetFont(hItem, &GUI_Font24B_ASCII);
		DROPDOWN_SetAutoScroll(hItem,1);	//启用自动使用滚动条
		DROPDOWN_AddString(hItem, "Fan");
		DROPDOWN_AddString(hItem, "Open  Fan");
	  DROPDOWN_AddString(hItem, "Close Fan");		
	 
	  hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_18);
		DROPDOWN_SetFont(hItem, &GUI_Font24B_ASCII);
		DROPDOWN_SetAutoScroll(hItem,1);	//启用自动使用滚动条
		DROPDOWN_AddString(hItem, "Air conditioner");
		DROPDOWN_AddString(hItem, "Open  conditioner");
	  DROPDOWN_AddString(hItem, "Close conditioner");	
	 
	  hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0);
		DROPDOWN_SetFont(hItem, &GUI_Font24B_ASCII);
		DROPDOWN_SetAutoScroll(hItem,1);	//启用自动使用滚动条
		DROPDOWN_AddString(hItem, "Pasition");
		DROPDOWN_AddString(hItem, "Saloon");
		DROPDOWN_AddString(hItem, "Kitchen");		
		DROPDOWN_AddString(hItem, "Shower room");
		DROPDOWN_AddString(hItem, "bedroom");
		DROPDOWN_AddString(hItem, "Main part");
				
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {   
		case ID_DROPDOWN_0: // Notifications sent by 'Button'
      switch(NCode) 
					{
						case WM_NOTIFICATION_CLICKED:
							break;
						case WM_NOTIFICATION_RELEASED:
							break;
						case WM_NOTIFICATION_SEL_CHANGED:
                 hItem=WM_GetDialogItem(pMsg->hWin,ID_DROPDOWN_0);
                 value_1=DROPDOWN_GetSel(hItem);
						    if(value_1==1)
								 {
									 GUI_EndDialog(pMsg->hWin, 0);	
                   interface=2;
			             delay_ms(50);
                   CreateFramewin_2();		 
								 }
								 else  if(value_1==2)
								 {
									 GUI_EndDialog(pMsg->hWin, 0);	
                   interface=3;
			             delay_ms(50);
                   CreateFramewin_3();		 
								 }
								 else  if(value_1==3)
								 {
									 GUI_EndDialog(pMsg->hWin, 0);	
                   interface=4;
			             delay_ms(50);
                   CreateFramewin_4();		 
								 }
								  else  if(value_1==4)
								 {
									 GUI_EndDialog(pMsg->hWin, 0);	
                   interface=5;
			             delay_ms(50);
                   CreateFramewin_5();		 
								 }
								  else  if(value_1==5)
								 {
									 GUI_EndDialog(pMsg->hWin, 0);	
                   interface=1;
			             delay_ms(50);
                   CreateFramewin_1();		 
								 }
								 else
							   break;
					}
		 break;
  case ID_DROPDOWN_16: // Notifications sent by 'Button'
      switch(NCode) 
					{
						case WM_NOTIFICATION_CLICKED:
							break;
						case WM_NOTIFICATION_RELEASED:
							break;
						case WM_NOTIFICATION_SEL_CHANGED:
                 hItem=WM_GetDialogItem(pMsg->hWin,ID_DROPDOWN_16);
                 value_5[0]=DROPDOWN_GetSel(hItem);
						     if(value_5[0]==1)
								 {
									send_databox(Rdata_tip,F_Address_3,command_1,1,"h",1);  
								 }
								 else  if(value_4[0]==2)
								 {
									send_databox(Rdata_tip,F_Address_3,command_1s,1,"i",1); 		 
								 }
								 else
							   break;
					}
		  break;	  					
	case ID_DROPDOWN_17: // Notifications sent by 'Button'
      switch(NCode) 
					{
						case WM_NOTIFICATION_CLICKED:
							break;
						case WM_NOTIFICATION_RELEASED:
							break;
						case WM_NOTIFICATION_SEL_CHANGED:
                 hItem=WM_GetDialogItem(pMsg->hWin,ID_DROPDOWN_17);
                 value_5[1]=DROPDOWN_GetSel(hItem);
						     if(value_5[1]==1)
								 {
									 send_databox(Rdata_tip,F_Address_3,command_3,1,"l",1);
								 }
								 else  if(value_5[1]==2)
								 {
									 send_databox(Rdata_tip,F_Address_3,command_3s,1,"m",1); 		 
								 }
								 else
							   break;
					}
		 break;	
  case ID_DROPDOWN_18: // Notifications sent by 'Button'
      switch(NCode) 
					{
						case WM_NOTIFICATION_CLICKED:
							break;
						case WM_NOTIFICATION_RELEASED:
							break;
						case WM_NOTIFICATION_SEL_CHANGED:
                 hItem=WM_GetDialogItem(pMsg->hWin,ID_DROPDOWN_18);
                 value_5[2]=DROPDOWN_GetSel(hItem);
						     if(value_5[2]==1)
								 {
									send_databox(Rdata_tip,F_Address_3,command_2,1,"j",1); 
								 }
								 else  if(value_5[2]==2)
								 {
									send_databox(Rdata_tip,F_Address_3,command_2s,1,"k",1);  		 
								 }
								 else
							   break;
					}
		 break;															
    default:
    WM_DefaultProc(pMsg);
    break;
  }
  }
}
/***********创建对话框***************/

//创建界面1:
WM_HWIN CreateFramewin_1(void) {
  WM_HWIN hWin;

  hWin = GUI_CreateDialogBox(_aDialogCreate_1, GUI_COUNTOF(_aDialogCreate_1), _cbDialog_1, WM_HBKWIN, 0, 0);
  return hWin;
}
//创建界面2:
WM_HWIN CreateFramewin_2(void) {
  WM_HWIN hWin;
  hWin = GUI_CreateDialogBox(_aDialogCreate_2, GUI_COUNTOF(_aDialogCreate_2), _cbDialog_2, WM_HBKWIN, 0, 0);
  return hWin;
}
//创建界面3:
WM_HWIN CreateFramewin_3(void) {
  WM_HWIN hWin;
  hWin = GUI_CreateDialogBox(_aDialogCreate_3, GUI_COUNTOF(_aDialogCreate_3), _cbDialog_3, WM_HBKWIN, 0, 0);
  return hWin;
}
//创建界面4:
WM_HWIN CreateFramewin_4(void) {
  WM_HWIN hWin;
  hWin = GUI_CreateDialogBox(_aDialogCreate_4, GUI_COUNTOF(_aDialogCreate_4), _cbDialog_4, WM_HBKWIN, 0, 0);
  return hWin;
}
//创建界面5:
WM_HWIN CreateFramewin_5(void) {
  WM_HWIN hWin;
  hWin = GUI_CreateDialogBox(_aDialogCreate_5, GUI_COUNTOF(_aDialogCreate_5), _cbDialog_5, WM_HBKWIN, 0, 0);
  return hWin;
}

void my_gui_create(void)
{
	WM_HWIN hWin;
	hWin=CreateFramewin_1();
	while(1)
	{
		GUI_Delay(1000);
	}
}


