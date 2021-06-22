/**
**
  内阻测试页面
  
*/

#include "MainTask.h"
#include  "gui.h"
#include "DIALOG.h"
#include "my_register.h" 
#include "sysflag.h"
#include "key.h"

WM_HWIN hWinR;

float DISS_Voltage;   //负载电压
float DISS_POW_Voltage;//稳压电源电压
float DISS_Current;//负载电流
float DISS_POW_Current;//稳压电源电流
float DISS_R;//内阻

extern GUI_CONST_STORAGE GUI_FONT GUI_FontSONG16;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontSONG12;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontSONGB12;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontSONGB16;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontSONG32;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontSONG26;

#define ID_WINDOW_R   (GUI_ID_USER + 0x00)

#define ID_BUTTON_1   (GUI_ID_USER + 0x02)
#define ID_BUTTON_2   (GUI_ID_USER + 0x03)
#define ID_BUTTON_3   (GUI_ID_USER + 0x04)
#define ID_BUTTON_4   (GUI_ID_USER + 0x05)
#define ID_BUTTON_5   (GUI_ID_USER + 0x06)

#define ID_TimerTimeR    1


static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
	
  { WINDOW_CreateIndirect, "R",  ID_WINDOW_R,  		0, 	 0, 480, 272, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Button", ID_BUTTON_1,   85,  232, 70, 45, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Button", ID_BUTTON_2,   85+79, 232, 70, 45, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Button", ID_BUTTON_3,   85+79*2, 232, 70, 45, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Button", ID_BUTTON_4,   85+79*3, 232, 70, 45, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Button", ID_BUTTON_5,   85+79*4, 232, 70, 45, 0, 0x0, 0 },
  
};

static struct SGraphData SGhListR[] = {
			
		//标记ID  是否点击  类型   中文显示   英文显示   字体颜色   背景颜色   字体  x坐标   y坐标  对角x坐标  对角y坐标
		//d_ID   focusType  t_Type  f_SName   f_EName    d_FColor   d_BKColor   0    d_x     d_y     d_x1        d_y1
		
		{1,  0, T_Text, "V:", 		"V:",   0,0, 		  &GUI_FontSONG26, 	30,  45, 0, 0 },   
		{2,  0, T_Text, "0.00",		"0.00",	0x0000FF7F,0, &GUI_FontSONG26,  88,  45, 0, 0 }, 
		{3, 0, T_Text, "V", 		"V",	0,0, 		  &GUI_FontSONG26, 	178, 45, 0, 0 },
		 
		{4, 0, T_Text, "R:", 		"R:" ,	0,0, 		  &GUI_FontSONG26, 	30,  45+35*1, 0, 0 },	
		{5,  0, T_Text, "0000", 	"0000",	0x0000FF7F,0, &GUI_FontSONG26, 	88,  45+35*1, 0, 0 },
		{6, 0, T_Text, "m", 		"m",	0,0, 	      &GUI_FontSONG26, 	178, 45+35*1, 0, 0 },
		{7, 0, T_Text, "Ω", 		"Ω",	0,0, 		  &GUI_FontSONG26, 	187, 45+35*1+2, 0, 0 },
		
		{8,  0, T_Text, "I:", 		"I:",   0,0, 		  &GUI_FontSONG26, 	30,  45+35*2, 0, 0 },   
		{9,  0, T_Text, "0.000",	"0.000",	0x0000FF7F,0, &GUI_FontSONG26,  88,  45+35*2, 0, 0 }, 
		{10, 0, T_Text, "A", 		"A",	0,0, 		  &GUI_FontSONG26, 	178, 45+35*2, 0, 0 },
		
		{11, 0, T_Text, "R1:", 		"R1:" ,	0,0, 		  &GUI_FontSONG26, 	30,  45+35*3, 0, 0 },	
		{12,  0, T_Text, "0000", 	"0000",	0x0000FF7F,0, &GUI_FontSONG26, 	88,  45+35*3, 0, 0 },
		{13, 0, T_Text, "", 		"",	0,0, 	      &GUI_FontSONG26, 	178, 45+35*3, 0, 0 },
		{14, 0, T_Text, "Ω", 		"Ω",	0,0, 		  &GUI_FontSONG26, 	187, 45+35*3+2, 0, 0 },
		
		{15, 0, T_Text,  "R2:", 		"R2",	0,0, 		  &GUI_FontSONG26, 	30, 45+35*4, 0, 0 },
		{16,  0, T_Text,  "0000",	"0000" , 0x0000FF7F,0,&GUI_FontSONG26,  88, 45+35*4, 0, 0 },	
		{17, 0, T_Text,  "", 		"",	0,0, 	      &GUI_FontSONG26, 	178, 45+35*4, 0, 0 },
		{18, 0, T_Text,  "Ω", 		"Ω",	0,0, 		  &GUI_FontSONG26, 	187, 45+35*4+2, 0, 0 },
		
		{19, 0, T_Text ,"过流测试",	 "Overflow Test",	0,0, 0, 		260, 40, 0, 0 },
		{20, 0, T_Text ,"起始电流",	"Starting C",		0,0, 0, 		260, 65, 0, 0 },
		{21,  1, T_THref ,"1.00",		"1.00" ,0,0, 0, 	350, 65, 400, 82 },
		{22, 0, T_Text ,"A", 	  	"A", 	  0,0, 0,  		430, 65, 0, 0 },
		
		{23, 0, T_Text ,"步进电流",	"Stepping C",		0,0,  0, 	260, 90, 0, 0 },	
		{24,  0, T_THref ,"5.00", 	"5.00" ,0,0,  0, 	350, 90, 400, 107 },	
		{25, 0, T_Text ,"A", 	 	"A",  	0,0,  0, 	430, 90, 0, 0 },
		
		{26, 0, T_Text ,"门槛电压",	"Threshold V",		0,0,  0, 	260, 115, 0, 0 },
		{27,  0, T_THref ,"0.00", 	"0.00",	0,0,  0, 	350, 115, 400, 132 },	
		{28, 0, T_Text ,"V", 		"V",  	0,0,  0, 	430, 115, 0, 0 },
		
		{29, 0, T_Text ,"步进时间",	"Step Time" ,	0,0,  0,	260, 140,	0, 0 },
		{30,  0, T_THref ,"0002",		"0002" ,	0,0,  0, 350, 140,	400, 157 },				
		{31, 0, T_Text ,"ms", 	 	"ms",	0,0,  0, 	430, 140,	0, 0 },
		
		{32, 0, T_Text ,"电流", 	"Current" ,	0,0,  0, 	260, 165,	0, 0 },	
		{33,  0, T_Text ,"0.000", 	"0.000",	0,0,  0, 	350, 165,	400, 182 },
		{34, 0, T_Text  ,"A", 		"A" ,	0,0,  0, 	430, 165,	0, 0 },
		
		{35, 0, T_Text ,"短路时间",	"Short Time",		0,0,  0,	260, 190,	0, 0 },	
		{36,  0, T_Text ,"0000", 		"0000" ,	0,0,  0, 	350, 190,	400, 207 },
		{37, 0, T_Text ,"ms", 	 "ms" ,		0,0,  0, 	430, 190,	0, 0 }
	};
	

	static struct SGraphData Un_SGhListR[] = {
			
			{1,  0, T_Text ,"V:", 		"V:",   0,0, 		  &GUI_FontSONG26, 	30,  55, 0, 0 },   
			{2,  0, T_Text ,"0.00",		"0.00",	0x0000FF7F,0, &GUI_FontSONG26,  88,  55, 0, 0 }, 
			{3, 0, T_Text ,"V", 		"V",	0,0, 		  &GUI_FontSONG26, 	178, 55, 0, 0 },
			
			{4, 0, T_Text ,"R:", 		"R:" ,	0,0, 		  &GUI_FontSONG26, 	30,  55+35*1, 0, 0 },	
			{5,  0, T_Text ,"0000", 	"0000",	0x0000FF7F,0, &GUI_FontSONG26, 	88,  55+35*1, 0, 0 },
			{6, 0, T_Text ,"m", 		"m",	0,0, 	      &GUI_FontSONG26, 	178, 55+35*1, 0, 0 },
			{7, 0, T_Text ,"Ω", 		"Ω",	0,0, 		  &GUI_FontSONG26, 	187, 55+35*1+2, 0, 0 },
			
			{8, 0, T_Text ,"R1:", 		"R1:" ,	0,0, 		  &GUI_FontSONG26, 	30,  55+35*2, 0, 0 },	
			{9,  0, T_Text ,"0000", 	"0000",	0x0000FF7F,0, &GUI_FontSONG26, 	88,  55+35*2, 0, 0 },
			{10, 0, T_Text ,"m", 		"m",	0,0, 	      &GUI_FontSONG26, 	178, 55+35*2, 0, 0 },
			{11, 0, T_Text ,"Ω", 		"Ω",	0,0, 		  &GUI_FontSONG26, 	187, 55+35*2+2, 0, 0 },
		
			{12, 0, T_Text ,"R2:", 		"R2",	0,0, 		  &GUI_FontSONG26, 	30,  55+35*3, 0, 0 },
			{13, 0, T_Text ,"0000",	"0000" , 0x0000FF7F,0, &GUI_FontSONG26, 	88,  55+35*3, 0, 0 },	
			{14, 0, T_Text ,"m", 		"m",	0,0, 	      &GUI_FontSONG26, 	178, 55+35*3, 0, 0 },
			{15, 0, T_Text ,"Ω", 		"Ω",	0,0, 		  &GUI_FontSONG26, 	187, 55+35*3+2, 0, 0 },
	};

//焦点id布局
static const int WINR_id_DataLists[] =  { 21, 24, 27, 30 };

char RStr[10];

float old_I = 0;

float max_discahrgeNum = 0;  

float v_memory = 0, r_memory = 0;

vu8 keydown_trig = 0;

static void _cbDialog(WM_MESSAGE * pMsg) {
	
	WM_HWIN hItem;
	int     NCode, Id, MAX_LEN;
	vu8 i = 0;
	char *winName,  *chargeName;
	char tChar[10];

	static struct SListData slistR[] = {	

		{T_Button , "内阻测试", "I.R. TEST",  0x00B22222 , 0, ID_BUTTON_1 },
		
		{T_Button , "程控电源", "POWER",  0x00B22222 , 0, ID_BUTTON_2 },
		
		{T_Button , "程控负载", "LOAD",  0x00B22222 , 0, ID_BUTTON_3 },
		
		{T_Button , "充放电", "C&DC",  0x00B22222 , 0, ID_BUTTON_4 },
		
		{T_Button , "系统设置", "SYSTEM",  0x00B22222 , 0, ID_BUTTON_5 },
		
	};	
	
	vu8 _dloc;
	
  switch (pMsg->MsgId) {
	
			//重绘
			case WM_PAINT:  {
				
				hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);	
				
				BUTTON_SetPressed( hItem , 1 );	
				
				GUI_SetColor(GUI_ORANGE);  //设置画笔颜色
				
				GUI_FillRoundedRect(0,0,480,33,5);
				
				GUI_SetColor(GUI_BLACK);
				
				GUI_SetFont(&GUI_FontSONG16);
				
				GUI_UC_SetEncodeUTF8();
				
				GUI_SetTextMode(GUI_TM_TRANS);//设置文本模式为底色透明
				
				winName = ( FLAG_LANG == 1) ? "L.R. TEST" : "内阻测试";
				
				GUI_DispStringAt( winName, (480 - GUI_GetStringDistX( winName))/2, 10);
				
				if( FLAG_OVTest )
				{
					for( i = 0; i < sizeof( SGhListR )/sizeof( SGhListR[0]) ; i++  ){
						
						bush_Graph_String( FLAG_LANG, SGhListR[i] );
					}
					
				}else{	
				
					for( i = 0; i < sizeof( Un_SGhListR )/sizeof( Un_SGhListR[0]) ; i++  ){
						
						bush_Graph_String( FLAG_LANG, Un_SGhListR[i] );		
					}
				}
				
				GUI_SetFont(&GUI_FontSONG12);	
				GUI_UC_SetEncodeUTF8();	
				GUI_SetTextMode(GUI_TM_TRANS);//设置文本模式为底色透明
				GUI_SetColor(0x00456654);
				
				if( C_DISCHARGE == 1 )
				{
					chargeName =  ( FLAG_LANG == 1) ? "Testing" : "测试中";
				}
				else if( POWER_RECHARGE == 1 ){
					chargeName =  ( FLAG_LANG == 1) ? "Charging" : "充电中";
				}
				else{
					chargeName =  ( FLAG_LANG == 1) ? "Not Open" : "未开启";
				}
				
				GUI_DispStringAt( chargeName, 460 - GUI_GetStringDistX(chargeName), 13);	

			}
			
			break;
			
			//定时模块消息
			case WM_TIMER:
				
				if(WM_GetTimerId(pMsg->Data.v) == ID_TimerTimeR){	
				
					memset( RStr, '\0', sizeof(RStr));
					
					/*  判断是否开启过流测试  */
					if( FLAG_OVTest ){
						
						/* 未开启 */
						if( trig_flag_state == 0  ){
							
							/* 电源电压 > 设置的门槛电压  */
							if( DISS_Voltage > (float)CLists.IR_Threshold_V / 1000 &&  v_memory == 0 && r_memory == 0 ){
								
								/*  开启放电  */
								trig_flag_state = 1;
								C_DISCHARGE = 1;        	
								OnOff_GPOI_ResetSet( 2, 1 );   
								ms_time = 0;
								abc_time = 0;
								max_discahrgeNum = (float)CLists.IR_Start_C / 1000;
								
							}				
						}						
						
						if( trig_flag_state == 1  && abc_time > 1 &&  v_memory == 0 && r_memory == 0 ){
							
							v_memory = DISS_Voltage;
							r_memory = CLists.CR_VLUE;	
						}
						
						/* 打开充电一秒  */
						if( POWER_RECHARGE == 1 && abc_time > 1 && DISS_Voltage > (float)CLists.IR_Threshold_V/1000 ){
				
							Off_GPOI_ResetSet();
							C_DISCHARGE = 1;		
							old_I = 0;
							max_discahrgeNum = (float)CLists.IR_Start_C / 1000;
							OnOff_GPOI_ResetSet( 2, 1 ); 
							ms_time = 0;
							abc_time = 0;
							CLists.CSET_Current_Laod =  CLists.IR_Start_C;
						}
						
						sprintf( RStr, "%f", old_I );
						_dloc = dot_Loc_isNull(RStr,strlen(RStr)) > 1 ? 6 : 5;
						
						memset(SGhListR[8].f_SName, '\0', sizeof(SGhListR[8].f_SName));
						memset(SGhListR[8].f_EName, '\0', sizeof(SGhListR[8].f_EName));
						
						strncpy( SGhListR[8].f_SName, RStr, _dloc );	
						strncpy( SGhListR[8].f_EName, RStr, _dloc);	
						
						memset( RStr, '\0', sizeof(RStr));		
						memset( tChar, '\0', sizeof(tChar));
						
						sprintf( RStr, "%f", DISS_Current );
						_dloc = dot_Loc_isNull(RStr,strlen(RStr)) > 1 ? 6 : 5;
						
						memset(SGhListR[32].f_SName, '\0', sizeof(SGhListR[32].f_SName));
						memset(SGhListR[32].f_EName, '\0', sizeof(SGhListR[32].f_EName));
						
						strncpy(SGhListR[32].f_SName, RStr, _dloc );	
						strncpy(SGhListR[32].f_EName, RStr, _dloc);	
						
						memset( RStr, '\0', sizeof(RStr));		
						memset( tChar, '\0', sizeof(tChar));

						if( v_memory != 0 ){
							
							sprintf( RStr, "%f", v_memory );
							_dloc = dot_Loc_isNull(RStr,strlen(RStr)) > 1 ? 6 : 5;
							
							memset(SGhListR[1].f_SName, '\0', sizeof(SGhListR[1].f_SName));
							memset(SGhListR[1].f_EName, '\0', sizeof(SGhListR[1].f_EName));
							
							strncpy(SGhListR[1].f_SName, RStr, _dloc );	
							strncpy(SGhListR[1].f_EName, RStr, _dloc);	
						
						}else{
							
							sprintf( RStr, "%f", DISS_Voltage );
							_dloc = dot_Loc_isNull(RStr,strlen(RStr)) > 1 ? 6 : 5;
							
							memset(SGhListR[1].f_SName, '\0', sizeof(SGhListR[1].f_SName));
							memset(SGhListR[1].f_EName, '\0', sizeof(SGhListR[1].f_EName));
							
							strncpy( SGhListR[1].f_SName, RStr, _dloc );	
							strncpy( SGhListR[1].f_EName, RStr, _dloc);	
							
						}
						
						if( r_memory != 0 ){
							
							memset( RStr, '\0', sizeof(RStr));		
							memset( tChar, '\0', sizeof(tChar));					
							sprintf( tChar, "%d", (int)r_memory );
							if( r_memory < 1 ){
								strcpy( RStr, "0000" );
							}else{
								for( i = 0; i < 4 - strlen(tChar); i++ ){			
									strcat(RStr,"0");
								}
								strcat( RStr, tChar);
							}
							
							memset(SGhListR[4].f_SName, '\0', sizeof(SGhListR[4].f_SName));
							memset(SGhListR[4].f_EName, '\0', sizeof(SGhListR[4].f_EName));
							strcpy(SGhListR[4].f_SName, RStr );	
							strcpy(SGhListR[4].f_EName, RStr);
						
						}else{
							
							memset( RStr, '\0', sizeof(RStr));		
							memset( tChar, '\0', sizeof(tChar));					
							sprintf( tChar, "%d", CLists.CR_VLUE );
							if( DISS_Voltage < 0.01 ){
								strcpy( RStr, "0000" );
							}else{
								for( i = 0; i < 4 - strlen(tChar); i++ ){			
									strcat(RStr,"0");
								}
								strcat( RStr, tChar);
							}
							
							memset(SGhListR[4].f_SName, '\0', sizeof(SGhListR[4].f_SName));
							memset(SGhListR[4].f_EName, '\0', sizeof(SGhListR[4].f_EName));
							strcpy(SGhListR[4].f_SName, RStr );	
							strcpy(SGhListR[4].f_EName, RStr);
						}
							
					}
					else{
				
						sprintf( RStr, "%f", DISS_Voltage );
						_dloc = dot_Loc_isNull(RStr,strlen(RStr)) > 1 ? 6 : 5;
						
						memset(Un_SGhListR[1].f_SName, '\0', sizeof(Un_SGhListR[1].f_SName));
						memset(Un_SGhListR[1].f_EName, '\0', sizeof(Un_SGhListR[1].f_EName));
						
						strncpy( Un_SGhListR[1].f_SName, RStr, _dloc );	
						strncpy( Un_SGhListR[1].f_EName, RStr, _dloc);	
						
						memset( RStr, '\0', sizeof(RStr));		
						memset( tChar, '\0', sizeof(tChar));					
						sprintf( tChar, "%d", CLists.CR_VLUE );
						if( DISS_Voltage < 0.01 ){
							strcpy( RStr, "0000" );
						}else{
							for( i = 0; i < 4 - strlen(tChar); i++ ){			
								strcat(RStr,"0");
							}
							strcat( RStr, tChar);
						}
						
						memset(Un_SGhListR[4].f_SName, '\0', sizeof(Un_SGhListR[4].f_SName));
						memset(Un_SGhListR[4].f_EName, '\0', sizeof(Un_SGhListR[4].f_EName));
						strcpy(Un_SGhListR[4].f_SName, RStr );	
						strcpy(Un_SGhListR[4].f_EName, RStr);
							
					}
					memset( RStr, '\0', sizeof(RStr)); 		
					memset( tChar, '\0', sizeof(tChar));
					
					WM_InvalidateWindow(hWinR);	
					WM_RestartTimer(pMsg->Data.v, 500);    //复位定时器数值越大刷新时间越短
				}
				
			break;
				
			case WM_INIT_DIALOG:{
			
					WINDOW_SetBkColor(pMsg->hWin, 0x004A4655);
					
					WM_CreateTimer( pMsg->hWin ,ID_TimerTimeR ,20 , 0);//创建本窗口定时器
						
					MAX_LEN = sizeof(slistR)/sizeof(slistR[0]);
					
					bush_WinObj( FLAG_LANG, pMsg , slistR , MAX_LEN);
			}
			
			break;
			
			case WM_NOTIFY_PARENT:

			break;
			
			default:
			
				WM_DefaultProc(pMsg);
			break;
  
	}

}


WM_HWIN CreateR(void);
WM_HWIN CreateR(void) {
	
		int dlen, flen;
		
		int *key_loc;
		
		char *temp_char;
		
		if( FLAG_FIRSTWIN[1] ){
			
				if( CLists.IR_Start_C 	  !=	0xffffff )
				{	
						strcpy( SGhListR[20].f_SName, Read_Data_Parse(1) ); 		
						strcpy( SGhListR[20].f_EName, Read_Data_Parse(1) ); 			
				}
				
				if( CLists.IR_Step_C 	  != 	0xffffff )
				{	
						strcpy( SGhListR[23].f_SName, Read_Data_Parse(2) ); 	
						strcpy( SGhListR[23].f_EName, Read_Data_Parse(2) ); 		
				}
				
				if( CLists.IR_Threshold_V !=	0xffffff )
				{	
						strcpy( SGhListR[26].f_SName, Read_Data_Parse(3) ); 	
						strcpy( SGhListR[26].f_EName, Read_Data_Parse(3) ); 		
				}
				
				if( CLists.IR_Step_Time   !=	0xffffff )
				{	
						strcpy( SGhListR[29].f_SName, Read_Data_Parse(4) ); 
						strcpy( SGhListR[29].f_EName, Read_Data_Parse(4) ); 		
				}
				
				FLAG_FIRSTWIN[1] = 0;						
		} 	
		CLists.CSET_Current_Laod =  CLists.IR_Start_C;
			
		ms_time = 0;
		abc_time = 0;
		
		dlen = sizeof(SGhListR) / sizeof(SGhListR[0]);
		key_loc = get_id_index( SGhListR, dlen);
		
		memset( displayStr, '\0', sizeof(displayStr));
		
		if( *key_loc != 21 )
		{
				SGhListR[*(key_loc+1)].focusType = 0;	
				SGhListR[20].focusType = 1;
		}
		
		hWinR = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
		
		return hWinR;
}



/*   方向键   */
void KEY_DOWN_IRT( int handle ){
	
	int i, dlen = 0;
	int *key_loc;
	int flag = 0;
	int get_nextid = 0, nowid = 0, nowid_index = 0;
	
	if( FLAG_OVTest  && FLAG_INPUTAUTH == 0  )
	{
		dlen = sizeof(SGhListR) / sizeof(SGhListR[0]);
		key_loc = get_id_index( SGhListR, dlen);
			
		nowid = *key_loc;
		nowid_index = *(key_loc + 1);
			
		if( nowid == 0x194 ) 
					
			SGhListR[20].focusType = 1;
					
		else
				
			flag = 1;
			
		get_nextid = get_nextindex( nowid, WINR_id_DataLists, handle);
			
		if( flag && get_nextid != 0xFF )
		{
			SGhListR[nowid_index].focusType = 0;	
				
			for	( i = 0; i < dlen; i++)
			{
				if( SGhListR[i].d_ID == get_nextid && (SGhListR[i].t_Type == T_THref || SGhListR[i].t_Type == T_TLISTBOX) )
				{				
					SGhListR[i].focusType = 1;	
					break;
				}
			}	
				
		}
		WM_InvalidateWindow(hWinR);	
	}
	else
	{
	
	}
}



/*   数字键输入   */
void KEY_IRT_INPUT(char *strx)
{

	int *key_loc;
		
	int i, dlen = 0;
		
	int dot_loc = 1024;
	
	dlen = sizeof(SGhListR) / sizeof(SGhListR[0]);
		
	key_loc = get_id_index( SGhListR, dlen);
		
	if( *key_loc == 0x194 )
			
		SGhListR[20].focusType = 1;
	else
	{
		if( displayStr[0] == '\0' && *strx == 0x2e ) 
					
			strcat( displayStr, "0.");	
		else
		{
			for ( i = 0; i < strlen(displayStr); i++)
			{
				if( displayStr[i] == 0x2e )
									
					dot_loc = i;	
				
				if( displayStr[i] == 0x2e && *strx == 0x2e )
									
					goto rmore_point;
			}
			if( dot_loc == 1024 )
			{
				if( strlen(displayStr) < 4 )
									
					strcat( displayStr, strx);
			}
			else
			{
				if( strlen(displayStr) < 6 )
											
					strcat( displayStr, strx);
			}
			if( SGhListR[*(key_loc+1)].focusType == 1 && ( SGhListR[*(key_loc+1)].t_Type == T_THref || SGhListR[*(key_loc+1)].t_Type == T_TLISTBOX ) )
			{			
				strcpy( SGhListR[*(key_loc+1)].f_SName, displayStr );	
				
				strcpy( SGhListR[*(key_loc+1)].f_EName, displayStr );
			}					
		}
	}
		rmore_point:	
		
				WM_InvalidateWindow(hWinR);
}



/*  确认键   */
void R_ent(){
	
		int dlen = 0;
		
		int *key_loc;
		
		if( FLAG_INPUTAUTH )
		{	
				FLAG_INPUTAUTH = 0;	
				R_value_correct();		
		}
		else
		{
				FLAG_INPUTAUTH = 1;
				
				dlen = sizeof( SGhListR) / sizeof( SGhListR[0]);
				key_loc = get_id_index( SGhListR, dlen);
				
				if( *key_loc == 0x194 )
						
						SGhListR[20].focusType = 1;
				
				else
				{
						memset( displayStr, '\0', sizeof( displayStr));
						SGhListR[*(key_loc+1)].d_BKColor = 0x00F0FFFF;
						strcpy( SGhListR[*(key_loc+1)].f_SName, "");
						strcpy( SGhListR[*(key_loc+1)].f_EName, "");
						WM_InvalidateWindow(hWinR);
				}
		}	
	
}



/*  删除输入的值  */
void R_delete_input(){
	
		int dlen = 0, flen = 0;
		
		int *key_loc;
		
		if( FLAG_INPUTAUTH )
		{	
				dlen = sizeof(SGhListR) / sizeof(SGhListR[0]);
				
				key_loc = get_id_index( SGhListR, dlen);
				
				if( *key_loc == 0x194 )
				
						SGhListR[20].focusType = 1;
				
				else
				{
						strcpy( displayStr , SGhListR[*(key_loc+1)].f_EName );
					
						if( SGhListR[*(key_loc+1)].t_Type == T_THref && ( strlen( displayStr) != 0 || displayStr[0] != '\0' ) )
						{
								displayStr[strlen( displayStr) - 1] = '\0'; 
								
								strcpy( SGhListR[*(key_loc+1)].f_SName, displayStr );	
								
								strcpy( SGhListR[*(key_loc+1)].f_EName, displayStr );	
								
								WM_InvalidateWindow(hWinR);
						}
				}
		}	

}



/*   显示数据校准   */
void R_value_correct(){
	
	int dlen = 0, flen = 0, i = 0, NUM = 0, flag_null = 0; //是否为最小值
	
	int *key_loc;
	
	int temp, temp_int, dot_loc = 1024;  //是否具有小数点  1024---无   其余---小数点位置
	
	char temp_char[10];
	
	float temp_float;
	
	char int_char[10], float_char[10];
	
	vu8 flag_dot = 0;
	
	if( !FLAG_INPUTAUTH ){
		
		dlen = sizeof(SGhListR) / sizeof(SGhListR[0]);
		
		key_loc = get_id_index( SGhListR, dlen);
		
		if( *key_loc == 0x194 )
		
				SGhListR[20].focusType = 1;
		else
		{
				temp = *(key_loc+1);
				
				strcpy( displayStr , SGhListR[temp].f_EName );
				
				if( (displayStr[0] == '\0' || strlen(displayStr) == 0) && SGhListR[temp].t_Type == T_THref ){
					
					flag_null = 1;
					
				}

				temp_int = atoi(displayStr);   //转为整数
				
				temp_float = atof(displayStr);  //转为浮点数
				
				memset( temp_char, '\0', sizeof(temp_char));
				
				memset( int_char, '\0', sizeof(int_char));
				
				memset( float_char, '\0', sizeof(float_char));
				
				sprintf(int_char, "%d", temp_int);
				
				sprintf(float_char, "%f", temp_float);
				
				for( i = 0; i < strlen(displayStr); i++  ){  
				
					if( displayStr[i] == 0x2e ){	
					
						flag_dot = 1;	
						
						break;
					}			
				}
				
				for( i = 0; i < strlen(float_char); i++  ){  
					if( float_char[i] == 0x2e ){
						
						dot_loc = i;
						
						break;
						
					}			
				}
				
				switch( SGhListR[temp].d_ID ){
					
					case 3:   
					
					//====================================起始电流======================================
						if( flag_null ){
							
							strcpy( displayStr, "0.000"  );	
							
							CLists.IR_Start_C = 0 * 1000;	
							
							CLists.CSET_Current_Laod =  CLists.IR_Start_C;
							
						}else{	
						
							if( !flag_dot ){    
							
								if( temp_int > 30  ){
									
									strcpy( displayStr, "30.000"  );	
									
									CLists.IR_Start_C = 30 * 1000;
									
									CLists.CSET_Current_Laod =  CLists.IR_Start_C;
									
									break;		
								}
								
								if( temp_int <= 0  ){  
								
									strcpy( displayStr, "0.000"  );
									
									CLists.IR_Start_C = 0 * 1000;
									
									CLists.CSET_Current_Laod =  CLists.IR_Start_C;
									
									break;		
								}		
								
								if( strlen(int_char) > 2){		strcat( int_char, ".0"	);		}
								
								else{		strcat( int_char, ".000");		}
								
								strcpy( displayStr, int_char );	

								CLists.IR_Start_C = temp_int * 1000;
								
								CLists.CSET_Current_Laod =  CLists.IR_Start_C;
								
							}
							else
							{

								if( temp_float - 30 > 0 ){		
								
									strcpy( displayStr, "30.000"  );		
									
									CLists.IR_Start_C = 30 * 1000;		
									
									CLists.CSET_Current_Laod =  CLists.IR_Start_C;
									
									break;		
								}
								
								if( temp_float <= 0     ){  
								
									strcpy( displayStr, "0.000"  );	
									
									CLists.IR_Start_C = 0 * 1000;	
									
									CLists.CSET_Current_Laod =  CLists.IR_Start_C;
									
									break;		
								}
								
								//字符串    字符串指针首位    几位数
								//strncpy( temp_decimals, displayStr + dot_loc + 1, strlen(displayStr) - dot_loc - 1  );
								
								NUM = strlen(float_char) - dot_loc - 1;
								
								for( i = 0; i < 3 - NUM; i++ ){	strcat( float_char, "0");	}
								
								NUM = ( dot_loc > 1 ) ? 6 : 5;	
								
								strncpy( int_char, float_char, NUM );
								
								strcpy( displayStr, int_char );			
								
								CLists.IR_Start_C = temp_float * 1000;	

								CLists.CSET_Current_Laod =  CLists.IR_Start_C;						
							}			
						}			
						
					break;	
					
					case 4:
					//====================================步进电流======================================
						if( flag_null ){   
						
							strcpy( displayStr, "0.000"  );
							
							CLists.IR_Step_C = 0 * 1000;	
							
						}else{
							
							if( !flag_dot ){      
								
								if( temp_int > 30  ){   //最大值
								
									strcpy( displayStr, "30.000"  );
									
									CLists.IR_Step_C = 30 * 1000;	
									
									break;
								}
								if( temp_int == 0 ){    //最小值
								
									strcpy( displayStr, "0.000"  );
									
									CLists.IR_Step_C = 0 * 1000;	
									
									break;
								}	
								if( strlen(int_char) > 2){
									
									strcat( int_char, ".0");
									
								}else{
									
									strcat( int_char, ".000");
									
								}
								
								strcpy( displayStr, int_char );
								
								temp_int = temp_int * 1000;	
								
								CLists.IR_Step_C = temp_int;
								
							}else{
					
								if( temp_float - 30 > 0 ){   //最大值
								
									strcpy( displayStr, "30.000"  );
									
									CLists.IR_Step_C = 30 * 1000;	
									
									break;
								}
								
								if( temp_float <= 0 ){    //最小值
								
									strcpy( displayStr, "0.000"  );
									
									CLists.IR_Step_C = 0 * 1000;	
									
									break;
								}
								
								NUM = strlen(float_char) - dot_loc - 1;
								
								for( i = 0; i < 3 - NUM; i++ ){
									
									strcat( float_char, "0");
									
								}
								
								NUM = ( dot_loc > 1 ) ? 6 : 5;
								
								strncpy( int_char, float_char, NUM );
								
								strcpy( displayStr, int_char );		
								
								CLists.IR_Step_C = temp_float * 1000;
								
							}
						}				
						
					break;
					
					case 5:
					//====================================门槛电压======================================
						if( flag_null ){  
						
							strcpy( displayStr, "0.000"  );
							
							CLists.IR_Threshold_V = 0 * 1000;
							
						}else{
							
							if( !flag_dot ){      		
								
								if( temp_int > 60  ){   //最大值
								
									strcpy( displayStr, "60.000"  );
									
									CLists.IR_Threshold_V = 60 * 1000;
									
									break;
								}
								if( temp_int <= 0 ){    //最小值
								
									strcpy( displayStr, "0.000"  );
									
									CLists.IR_Threshold_V = 0 * 1000;
									
									break;
								}						
								if( strlen(int_char) > 2){
									
									strcat( int_char, ".0");
									
								}else{
									
									strcat( int_char, ".000");
								}
								
								strcpy( displayStr, int_char );
								
								temp_int = temp_int * 1000;	
								
								CLists.IR_Threshold_V = temp_int;
								
							}else{
								
								if( temp_float - 60 > 0 ){   //最大值
								
									strcpy( displayStr, "60.000"  );
									
									CLists.IR_Threshold_V = 60 * 1000;
									
									break;
								}
								if( temp_float <= 0 ){    //最小值
								
									strcpy( displayStr, "0.000"  );
									
									CLists.IR_Threshold_V = 0 * 1000;
									
									break;
								}
								
								NUM = strlen(float_char) - dot_loc - 1;
								
								for( i = 0; i < 3 - NUM; i++ ){
									
									strcat( float_char, "0");
									
								}	
								
								NUM = ( dot_loc > 1 ) ? 6 : 5;
								
								strncpy( int_char, float_char, NUM );
								
								strcpy( displayStr, int_char );	
								
								CLists.IR_Threshold_V = temp_float * 1000;
							}
						}
					
					break;
					
					case 6:
					//====================================步进时间======================================
						if( flag_null ){ 
						
							strcpy( displayStr,"0000"  );
							
							CLists.IR_Step_Time = 0;
							
						}else{
							
							if( !flag_dot ){  
							
								if(  temp_int > 255  ){   //最大值
								
									strcpy( displayStr, "0255"  );
									
									CLists.IR_Step_Time = 255;
									
									break;
									
								}
								if( temp_int <= 0 ){    //最小值
								
									strcpy( displayStr, "0000"  );
									
									CLists.IR_Step_Time = 0 ;
									
									break;
								}
								for( i = 0; i < 4 - strlen(int_char); i++){
									
									strcat( temp_char, "0");		
									
								}
								
								strcat( temp_char, int_char);
								
								strcpy( displayStr, temp_char);		
							
							}else{

								if( temp_float - 255 > 0 ){   //最大值
								
									strcpy( displayStr, "0255"  );
									
									CLists.IR_Step_Time = 255;
									
									break;
								}
								
								if( temp_float <= 0 ){    //最小值
								
									strcpy( displayStr, "0000"  );
									
									CLists.IR_Step_Time = 0 ;
									
									break;
								}
								
								if( dot_loc + 1 == strlen(displayStr) ){
													
								}else{
									
									temp_int++;
									
									sprintf( int_char , "%d" , temp_int);	
									
								}
								for( i = 0; i < 4 - strlen(int_char); i++){
									
									strcat( temp_char, "0");
									
								}
								
								strcat( temp_char, int_char);
								
								strcpy( displayStr, temp_char);					
							}	
							CLists.IR_Step_Time = temp_int;
						}
					
					break;
					
					case 7:
					//======================================电流========================================
						if( flag_null ){  
						
							strcpy( displayStr, "0.000"  );
							
						}else{	
						
							if( !flag_dot ){      	
								
								if( temp_int > 15  ){   //最大值
								
									strcpy( displayStr, "15.000"  );
									
									break;
								}
								if( temp_int <= 0 ){    //最小值
								
									strcpy( displayStr, "0.000"  );
									
									break;
								}						
								if( strlen(int_char) > 2){
									
									strcat( int_char, ".0");
									
								}else{
									
									strcat( int_char, ".000");
									
								}
								
								strcpy( displayStr, int_char );
								
							}else{

								if( temp_float - 15 > 0 ){   //最大值
								
									strcpy( displayStr, "15.000"  );
									
									break;
								}
								
								if( temp_float <= 0 ){    //最小值
								
									strcpy( displayStr, "0.000"  );
									
									break;
								}
								
								NUM = strlen(float_char) - dot_loc - 1;
								
								for( i = 0; i < 3 - NUM; i++ ){
									
									strcat( float_char, "0");
									
								}
								
								NUM = ( dot_loc > 1 ) ? 6 : 5;
								
								strncpy( displayStr, float_char, NUM );
								
							}
						}
					
					break;
					
					case 8:
					//====================================短路时间======================================
						if( flag_null ){  
						
							strcpy( displayStr, "0000"  );
							
						}else{	
						
							if( !flag_dot ){      							//正整数
			
								if(  temp_int > 255 ){   //最大值
								
									strcpy( displayStr, "255"  );
									
									break;		
								}
								if( temp_int <= 0 ){    //最小值
								
									strcpy( displayStr, "0000"  );
									
									break;
								}
								for( i = 0; i < 4 - strlen(int_char); i++){
									
									strcat( temp_char, "0");
									
								}
								
								strcat( temp_char, int_char);
								
								strcpy( displayStr, temp_char);
								
							}else{
								
								if( temp_float - 255 > 0 ){   //最大值
								
									strcpy( displayStr, "255"  );
									
									break;
								}
								if( temp_float <= 0 ){    //最小值
								
									strcpy( displayStr, "0000"  );
									
									break;
								}
								if( dot_loc + 1 == strlen(displayStr) ){
													
								}else{
									
									temp_int++;
									
									sprintf( int_char , "%d" , temp_int);	
									
								}
								for( i = 0; i < 4 - strlen(int_char); i++){
									
									strcat( temp_char, "0");		
									
								}
								
								strcat( temp_char, int_char);
								
								strcpy( displayStr, temp_char);
							}
						}
					
					break;
				}	
				
				Write_Flash_Bytes();	
				
				SGhListR[temp].d_BKColor = 0;
				
				strcpy( SGhListR[temp].f_SName, displayStr );
				
				strcpy( SGhListR[temp].f_EName, displayStr );
		}
	
	}
	memset( displayStr, '\0', sizeof(displayStr));
	
	WM_InvalidateWindow(hWinR);
}



void trig_R_charge()
{

		if( trig_flag_state == 0 )
		{
				trig_flag_state = 1;	
				POWER_RECHARGE = 1;
				keydown_trig = 1;
				OnOff_GPOI_ResetSet( 0 , 1 );	
				ms_time = 0;
				abc_time = 0;
		
		}
		else if( trig_flag_state == 1 &&( POWER_RECHARGE == 1 || C_DISCHARGE == 1))
		{	
				Off_GPOI_ResetSet();
				trig_flag_state = 0;
				keydown_trig = 0;
				CLists.CSET_Current_Laod =  CLists.IR_Start_C;
		}
		WM_InvalidateWindow(hWinR);
	
}







