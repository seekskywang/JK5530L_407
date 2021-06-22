/**
**
  充放电页面
*/

#include "DIALOG.h"
#include  "gui.h"
#include "my_register.h" 
#include "sysflag.h"
#include "key.h"

WM_HWIN cdchargeWin;

extern GUI_CONST_STORAGE GUI_FONT GUI_FontHZ16;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontSONG16;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontSONG12;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontSONG32;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontSONGB32;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontSONGB16;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontHZ12;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontSONG26;

#define ID_WINDOW_CDCHARGE  (GUI_ID_USER + 0x54)

#define ID_BUTTON_19     		(GUI_ID_USER + 0x56)
#define ID_BUTTON_20    		(GUI_ID_USER + 0x57)
#define ID_BUTTON_21     		(GUI_ID_USER + 0x58)
#define ID_BUTTON_22     		(GUI_ID_USER + 0x59)
#define ID_BUTTON_23     		(GUI_ID_USER + 0x5A)

#define ID_TimerTimeCDC  4

static const GUI_WIDGET_CREATE_INFO _aDialogCreatecdc[] = {
	
  { WINDOW_CreateIndirect, "Window", ID_WINDOW_CDCHARGE, 	0, 	 0,   480, 272, 0, 0x0, 0 },
  
  { BUTTON_CreateIndirect, "Button", ID_BUTTON_19, 			85,  232, 70, 45, 0, 0x0, 0 },
  
  { BUTTON_CreateIndirect, "Button", ID_BUTTON_20, 			85+79*1,  232, 70, 45, 0, 0x0, 0 },
  
  { BUTTON_CreateIndirect, "Button", ID_BUTTON_21, 			85+79*2,  232, 70, 45, 0, 0x0, 0 },
  
  { BUTTON_CreateIndirect, "Button", ID_BUTTON_22, 			85+79*3,  232, 70, 45, 0, 0x0, 0 },
  
  { BUTTON_CreateIndirect, "Button", ID_BUTTON_23, 			85+79*4,  232, 70, 45, 0, 0x0, 0},
	
};

static struct  SGraphData SGhListCDC[] = {
			
	{1,0,T_Text ,"V:", 	 "V:",	 0 , 0 ,  		 	&GUI_FontSONG32,30,  55+5, 0, 0 },
	{2,0,T_Text ,"0.00","0.00",  0x0000FF7F , 0 , 	&GUI_FontSONG26,88,  55+3+5, 0, 0  },
	{3,0,T_Text ,"V", 	 "V",	 0 , 0 ,  			&GUI_FontSONG32,	178, 55+5, 0, 0  },
		
	{4,0,T_Text ,"I:", 	 "I:",	 0 , 0 , 			&GUI_FontSONG32,30,  116+5, 0, 0 },
	{5,0,T_Text ,"0.00","0.00",   0x0000FF7F , 0 ,	&GUI_FontSONG26,88,  116+3+5, 0, 0 },
	{6,0,T_Text ,"A",    "A",	 0 , 0 , 			&GUI_FontSONG32,178,  116+5, 0, 0 },
	
	{7,0,T_Text ,"C:", 	 "C:",	 0 , 0 ,  			&GUI_FontSONG32, 30,  177+5, 0, 0 },
	{8,0,T_Text ,"00000","00000",  0x0000FF7F , 0 , &GUI_FontSONG26, 88,  177+3+5, 0, 0 },		
	{9,0,T_Text ,"mAH",  "mAH",	 0 , 0 ,   			&GUI_FontSONG32, 178,  177+5, 0, 0 },
		
	{10, 0, T_Text ,"充电", 		"", 0 , 0 , &GUI_FontSONGB16,       270,	40+5,	0,	0 },
	{11,0,T_Text ,"输出电压",	"Output V",0 , 0 ,  0,       320,	40+5,	395,	57+5 },
	{12,1,T_THref ,"4.20",		"4.20",  0 , 0 ,0,   415,	40+5,	465,	57+5 },
		
	{13,0,T_Text ,"限制电流",	"Limiting C",0 , 0 ,  0,       320,	40+25*1+5,	395,   40+25*1+17+5 },
	{14,0,T_THref ,"1.00",		"1.00",  0 , 0 , 0,  415,	40+25*1+5,	465,   40+25*1+17+5 },
		
	{15,0,T_Text ,"截止电压",	"Cutoff V",0 , 0 ,   0,      320,	40+25*2+5,	395,    40+25*2+17+5 },
	{16,0,T_THref ,"4.20", 		"4.2",   0 ,0,0 ,  415,	40+25*2+5,	465,    40+25*2+17+5 },
		
	{17,0,T_Text ,"截止电流", "Cutoff C",0 , 0 ,  0,       320,	40+25*3+5,	395,	40+25*3+17+5 },
	{18,0,T_THref ,"0.06",      "0.06",  0 , 0 , 0,  415,	40+25*3+5,	465,	40+25*3+17+5 },
		
	{19,0,T_Text ,"放电",       "",	0 , 0,  &GUI_FontSONGB16,       270,	150,	0,	0 },
		
	{20,0,T_Text ,"负载电流",   "Load C",0 , 0 ,  0,       320,	150,	0,	0 },
	{21,0,T_THref ,"10.00",      "10.00",  0 , 0 ,0, 415,   150,   465,	 167 },
		
	{22,0,T_Text ,"截止电压",   "Cutoff V", 0 , 0 ,  0,      320,	175,	0,	0 },
	{23,0,T_THref ,"4.2",        "4.2",  0 , 0 ,  0, 415,	175,   465,	  192 },
		
	{24,0,T_Text ,"循环次数",   "Cycle Times",0 , 0 ,  0,       320,	205,    0,	0 },
	{25,0,T_THref ,"0", 	     "0",   0 , 0,0,     415,	205,	465,	222 },
	
};

vu32 CDC_EndTime[2][255] = {{0},{0}};     //运行时间记录  0-充电    1-放电     最大循环次数 255

char RunTimesStr[2][30];      //运行时间显示  0-充电    1-放电   

vu32 CDC_AvgCmah = 0;         //容量平均值   (放电电池容量+) / 循环次数



vu8  run_num = 1;             //运行次数

char CStr[10];

static const int WINC_id_DataLists[] =  { 12, 14, 16, 18, 21, 23, 25 };



static void _cbDialogcdc(WM_MESSAGE * pMsg) {
	
	WM_HWIN hItem;
	
	int NCode, Id, MAX_LEN = 0, itemp = 0;
	
	vu8 i;
	
	char *winName, *show_Name, *ctime_name, *dtime_name;
	
	static struct SListData slistCDC[] = {
		
		{T_Button , "内阻测试","I.R. TEST",  0x00B22222 , 0, ID_BUTTON_19 },
		{T_Button , "程控电源", "POWER",  0x00B22222 , 0, ID_BUTTON_20 },
		{T_Button , "程控负载","LOAD",  0x00B22222 , 0, ID_BUTTON_21 },
		{T_Button , "充放电", 	"C&DC",  0x00B22222 , 0, ID_BUTTON_22 },
		{T_Button , "系统设置","SYSTEM",  0x00B22222 , 0, ID_BUTTON_23 },
		
	};
	
	vu8 _dloc;
	
	char cChar[20];
	
	float ftemp;
	
	vu32 CDC_Cmah = 0;
	
	switch (pMsg->MsgId) {
	  	
		case WM_PAINT: {
			
			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_22);	
			
			BUTTON_SetPressed( hItem , 1 );		
			
			GUI_SetColor(0x00D3D3D3);
			
			GUI_FillRoundedRect(0,0,480,33,5);
			
			GUI_SetColor(GUI_BLACK);
			
			GUI_SetFont(&GUI_FontSONGB16);
			
			GUI_UC_SetEncodeUTF8();
			
			GUI_SetTextMode(GUI_TM_TRANS);//设置文本模式为底色透明

			winName = ( FLAG_LANG == 1) ? "C&DC" : "充放电";
		
			GUI_DispStringAt( winName, (480 - GUI_GetStringDistX( winName))/2, 10);
		
			for( i = 0; i < sizeof( SGhListCDC )/sizeof( SGhListCDC[0]) ; i++  ){
				
				bush_Graph_String( FLAG_LANG, SGhListCDC[i] );
			}
			
			GUI_SetFont(&GUI_FontSONG12);	
			GUI_UC_SetEncodeUTF8();	
			GUI_SetTextMode(GUI_TM_TRANS);//设置文本模式为底色透明
			GUI_SetColor(0x00456654);
			
			if( POWER_RECHARGE == 0 &&  C_DISCHARGE == 0 )
			{				
				show_Name = ( FLAG_LANG == 1) ? "Waiting Open" : "等待开启";
			}
			else if( POWER_RECHARGE == 1 &&  C_DISCHARGE == 0 )
			{	
				show_Name = ( FLAG_LANG == 1) ? "Charging" : "充电中";			
			}
			else if( POWER_RECHARGE == 0 &&  C_DISCHARGE == 1 )
			{
				show_Name = ( FLAG_LANG == 1) ? "Discharging" : "放电中";
			}
			GUI_DispStringAt( show_Name, 460 - GUI_GetStringDistX(show_Name), 13);
				
			ctime_name =  ( FLAG_LANG == 1) ? "Charg Times：" : "充电耗时";
				
			GUI_DispStringAt( ctime_name, 18, 6);
				
			GUI_DispStringAt( RunTimesStr[0], 18, 20);
				
			dtime_name =  ( FLAG_LANG == 1) ? "Discharg Times：" : "放电耗时" ;	
				
			GUI_DispStringAt( dtime_name, 35 + GUI_GetStringDistX(ctime_name), 6);
				
			GUI_DispStringAt( RunTimesStr[1], 35 + GUI_GetStringDistX(ctime_name), 20);	
					
		}
		
		break;

		case WM_TIMER:{
			
			if(WM_GetTimerId(pMsg->Data.v) == ID_TimerTimeCDC){	
										
				if( RunTimesStr[0][0] == '\0' || strlen( RunTimesStr[0]) == 0 ){
							
					strcpy( RunTimesStr[0], "00:00:00");	
								
				}else if( strlen( RunTimesStr[0]) > 25 ){
							
					strcpy( RunTimesStr[0], "error");
				}
						
				if( RunTimesStr[1][0] == '\0' || strlen( RunTimesStr[1]) == 0 ){
								
					strcpy( RunTimesStr[1], "00:00:00");
								
				}else if( strlen( RunTimesStr[1]) > 25 ){
							
					strcpy( RunTimesStr[1], "error");
				}		
			
				/*************************************************
				*                   判断电压电流
				**************************************************/
				if( POWER_RECHARGE == 1 ){	  				
		
					strcpy( RunTimesStr[0], show_times_char( abc_time  ) );
					
					if( DISS_POW_Voltage >= (double)CLists.CDC_Ccutoff_V/1000 && DISS_POW_Current < (double)CLists.CDC_Ccutoff_C/1000 ){
					
						CDC_EndTime[0][run_num] = abc_time;
						
						Off_GPOI_ResetSet();
						
						C_DISCHARGE = 1;                //放电状态
						
						OnOff_GPOI_ResetSet( 2, 1 );    //开启放电
						
						abc_time = 0;
						
						CDC_CsumMah = 0;
						
						strcpy( RunTimesStr[0], show_times_char( CDC_EndTime[0][run_num] ) );
					}	
				}
				
				if( C_DISCHARGE == 1 ){
					
					strcpy( RunTimesStr[1], show_times_char( abc_time ) );
					
					if( DISS_Voltage < (double)CLists.CDC_Dcutoff_V/1000 ){
						
					    CDC_EndTime[1][run_num] = abc_time;
						
						Off_GPOI_ResetSet();   
						
						trig_flag_state = 0;								
						
						//itemp = atoi( SGhListCDC[7].f_EName );
							
						CDC_Cmah += CDC_CsumMah ;	
						
						CDC_CsumMah = 0;
						
						if( CLists.CDC_Cycle_Time  > run_num )	{
							
							POWER_RECHARGE = 1;
							
							trig_flag_state = 1;
								
							strcpy( RunTimesStr[1], show_times_char( CDC_EndTime[1][run_num] ) );
							
							run_num++;
							
							OnOff_GPOI_ResetSet( 0, 1 );   //开启充电
							
							abc_time = 0;
							
		
						}else{
							
							/*   根据循环次数 --- 获取平均值   */
							CDC_AvgCmah = CDC_Cmah / run_num;
							
							if( CDC_AvgCmah > 99999 ){
								
								CDC_AvgCmah = 99999;
							}
							
							CDC_Cmah = 0;
						}
						
						if( trig_flag_state == 0 ){
							
						}
					}	
				}		
			
				/*************************************************
				*                    页面显示值
				**************************************************/
				
				/* 负载电压  */
				sprintf( CStr, "%f", DISS_Voltage );
				
				_dloc = dot_Loc_isNull(CStr,strlen(CStr)) > 1 ? 6 : 5;

				memset(SGhListCDC[1].f_SName, '\0', sizeof(SGhListCDC[1].f_SName));

				memset(SGhListCDC[1].f_EName, '\0', sizeof(SGhListCDC[1].f_EName));

				strncpy( SGhListCDC[1].f_SName, CStr, _dloc );	
				
				strncpy( SGhListCDC[1].f_EName, CStr, _dloc );
				
				/*  电流  */
				memset( CStr, '\0', sizeof(CStr));
				
				if( C_DISCHARGE == 1 ){  //开启放电
					
					sprintf( CStr, "%f", DISS_Current );       //负载电流
					
				}else{
					
					sprintf( CStr, "%f", DISS_POW_Current );   //电源电流
				}

				_dloc = dot_Loc_isNull(CStr,strlen(CStr)) > 1 ? 6 : 5;

				memset(SGhListCDC[4].f_SName, '\0', sizeof(SGhListCDC[4].f_SName));

				memset(SGhListCDC[4].f_EName, '\0', sizeof(SGhListCDC[4].f_EName));
				
				strncpy( SGhListCDC[4].f_SName, CStr, _dloc );	
				
				strncpy( SGhListCDC[4].f_EName, CStr, _dloc );
				
				/* 容量  */
				memset( CStr, '\0', sizeof(CStr));
				memset( cChar, '\0', sizeof(cChar));		
					
				if( trig_flag_state == 0 ){
						
					CDC_CsumMah = CDC_AvgCmah;
				}
				
				sprintf( cChar, "%d", (int)CDC_CsumMah );
				
				memset(SGhListCDC[7].f_SName, '\0', sizeof(SGhListCDC[7].f_SName));

				memset(SGhListCDC[7].f_EName, '\0', sizeof(SGhListCDC[7].f_EName));
							
				if( strlen(cChar) < 5){
					
					for( i = 0; i < 5 - strlen(cChar); i++) {
					
						strcat( CStr, "0");
					}
					strcat( CStr, cChar);
					
				}else{
					
					strncpy( CStr, cChar, 5 );
				}
				
				strcpy( SGhListCDC[7].f_SName, CStr );	
				
				strcpy( SGhListCDC[7].f_EName, CStr );
				
				memset( CStr, '\0', sizeof(CStr));
				memset( cChar, '\0', sizeof(cChar));
				
				/*    无效化重绘    */
				WM_InvalidateWindow(cdchargeWin);	
				
				WM_RestartTimer(pMsg->Data.v, 200);//复位定时器数值越大刷新时间越短		
			}	
			
			break;
		}
		
		break;	
		
		case WM_KEY:

		break;
			
		case WM_INIT_DIALOG:{
				
			hItem = pMsg->hWin;
			
			WINDOW_SetBkColor(hItem, 0x004A4655);
			
			WM_CreateTimer( hItem ,ID_TimerTimeCDC , 20 , 0);//创建本窗口定时器
			
			MAX_LEN = sizeof( slistCDC )/sizeof( slistCDC[0] );
			
			bush_WinObj( FLAG_LANG, pMsg ,slistCDC, MAX_LEN );
		}
		
		break;
		
		default:
		
			WM_DefaultProc(pMsg);
			
		break;
	 }

}

WM_HWIN CreateCdCharge(void);
WM_HWIN CreateCdCharge(void) {
	
	int dlen = 0;
	
	int *key_loc;
	
	if( FLAG_FIRSTWIN[3] ){
		
		if( CLists.CDC_OutPut_V 	  !=	0xffffff ){	
		
			strcpy( SGhListCDC[11].f_SName , Read_Data_Parse(1) ); 		strcpy( SGhListCDC[11].f_EName , Read_Data_Parse(1) ); 			
		}
		
		if( CLists.CDC_Limit_C 	  	  !=	0xffffff ){	
		
			strcpy( SGhListCDC[13].f_SName , Read_Data_Parse(2) ); 		strcpy( SGhListCDC[13].f_EName , Read_Data_Parse(2) ); 			
		}
		
		if( CLists.CDC_Ccutoff_V 	  !=	0xffffff ){	
		
			strcpy( SGhListCDC[15].f_SName , Read_Data_Parse(3) ); 		strcpy( SGhListCDC[15].f_EName , Read_Data_Parse(3) ); 			
		}
		
		if( CLists.CDC_Ccutoff_C 	  !=	0xffffff ){	
		
			strcpy( SGhListCDC[17].f_SName , Read_Data_Parse(4) ); 		strcpy( SGhListCDC[17].f_EName , Read_Data_Parse(4) ); 			
		}
		
		if( CLists.CDC_Load_C 	  	  !=	0xffffff ){	
		
			strcpy( SGhListCDC[20].f_SName , Read_Data_Parse(5) ); 		strcpy( SGhListCDC[20].f_EName , Read_Data_Parse(5) ); 			
		}
		
		if( CLists.CDC_Dcutoff_V 	  !=	0xffffff ){	
		
			strcpy( SGhListCDC[22].f_SName , Read_Data_Parse(6) ); 		strcpy( SGhListCDC[22].f_EName , Read_Data_Parse(6) ); 			
		}
		
		if( CLists.CDC_Cycle_Time 	  !=	0xffffff ){	
		
			strcpy( SGhListCDC[24].f_SName , Read_Data_Parse(7) ); 		strcpy( SGhListCDC[24].f_EName , Read_Data_Parse(7) ); 			
		}
		
		FLAG_FIRSTWIN[3] = 0;	
	} 	
	
	CLists.CSET_Current_Laod =  CLists.CDC_Load_C;
	
	CLists.CSET_Voltage = CLists.CDC_OutPut_V;
	 
	CLists.CSET_Current = CLists.CDC_Limit_C;
	
	dlen = sizeof( SGhListCDC ) / sizeof( SGhListCDC[0] );
	
	key_loc = get_id_index( SGhListCDC, dlen);
	
	memset( displayStr, '\0', sizeof(displayStr));
	
	if( *key_loc != 12 ){

		SGhListCDC[*(key_loc+1)].focusType = 0;	
		
		SGhListCDC[11].focusType = 1;
	}
	
	cdchargeWin = GUI_CreateDialogBox(_aDialogCreatecdc, GUI_COUNTOF(_aDialogCreatecdc), _cbDialogcdc, WM_HBKWIN, 0, 0);
	
	return cdchargeWin;	
}

// 显示时间
char *show_times_char( vu32 _r ){
	
	char charTemp[30];
	int time_h, time_m, time_s;
	char time_h_s[20], time_m_s[20], time_s_s[20];

	time_h = _r / 3600;					
	time_m = _r % 3600 / 60;
	time_s = _r % 3600 % 60;

    memset( charTemp, '\0', sizeof(charTemp));				
	memset( time_h_s, '\0', sizeof(time_h_s));
	memset( time_m_s, '\0', sizeof(time_m_s));
	memset( time_s_s, '\0', sizeof(time_s_s));
				
	sprintf( time_h_s, "%d", time_h );  
	sprintf( time_m_s, "%d", time_m ); 
	sprintf( time_s_s, "%d", time_s ); 
						
	if( strlen(time_h_s) < 2){
						
		strcat(charTemp,"0");
	}
	strcat(charTemp,time_h_s);	
	strcat(charTemp,":");
	if( strlen(time_m_s) < 2){
						
		strcat(charTemp,"0");
	}
	strcat(charTemp,time_m_s);	
	strcat(charTemp,":");
	if( strlen(time_s_s) < 2){
						
		strcat(charTemp,"0");
	}
	strcat(charTemp,time_s_s);
	
	if( strlen(charTemp) > 20 ){
		
		strcpy( charTemp, "error");
	}
	
	return charTemp;
	
}

/*  输入  */
void KEY_CDC_INPUT(char *strx){
	
	int *key_loc;
	int i, dlen, flen;
	int dot_loc = 1024;
	
	dlen = sizeof( SGhListCDC ) / sizeof( SGhListCDC[0] );
	
	key_loc = get_id_index( SGhListCDC, dlen);
	
	if( *key_loc == 0x194 )
			
		 SGhListCDC[11].focusType = 1;
	else
	{
	
		if( displayStr[0] == '\0' && *strx == 0x2e )
			
				strcat( displayStr, "0.");	
		else
		{
			for ( i = 0; i < strlen(displayStr); i++){
				
				if( displayStr[i] == 0x2e )
					
					dot_loc = i;

				if( displayStr[i] == 0x2e && *strx == 0x2e )
					
					goto cmore_point;
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
		}
		if(SGhListCDC[*(key_loc+1)].focusType == 1 && (SGhListCDC[*(key_loc+1)].t_Type == T_THref || SGhListCDC[*(key_loc+1)].t_Type == T_TLISTBOX) )
		{	
			
			strcpy( SGhListCDC[*(key_loc+1)].f_SName, displayStr );	
					 
			strcpy( SGhListCDC[*(key_loc+1)].f_EName, displayStr );
		}	
	}
	cmore_point:	
		
		WM_InvalidateWindow(cdchargeWin);
}	


void KEY_DOWN_CDC( int handle){
	
	int i;
	int *key_loc;
	int get_nextid = 0, nowid = 0, nowid_index = 0, dlen = 0, flag = 0;  
	
	if( FLAG_INPUTAUTH == 0)
	{
		dlen = sizeof(SGhListCDC) / sizeof(SGhListCDC[0]);
		key_loc = get_id_index( SGhListCDC, dlen);
				
		nowid = *key_loc;
		nowid_index = *(key_loc + 1);
		
		if( nowid == 0x194 )
			
			SGhListCDC[11].focusType = 1;
		else
			
			flag = 1;
		
		get_nextid = get_nextindex( nowid, WINC_id_DataLists, handle);
		
		if( flag && get_nextid != 0xFF )
		{
			SGhListCDC[nowid_index].focusType = 0;	
				
			for	( i = 0; i < dlen; i++)
			{
				if( SGhListCDC[i].d_ID == get_nextid && (SGhListCDC[i].t_Type == T_THref || SGhListCDC[i].t_Type == T_TLISTBOX) )
				{				
					SGhListCDC[i].focusType = 1;	
					break;
				}
			}	
		}
	}
	else	
	{
	
	}

}



/*  确认  */
void Cdc_ent( ){
	
	int dlen, flen;
	
	int *key_loc;
	
	if( FLAG_INPUTAUTH ){
		
		FLAG_INPUTAUTH = 0;
		
		Cdc_value_correct();
		
	}else{
		
		FLAG_INPUTAUTH = 1;
		
		dlen = sizeof(SGhListCDC) / sizeof( SGhListCDC[0] );
		
		key_loc = get_id_index( SGhListCDC, dlen);
		
		if( *key_loc == 0x194 )
			
		   SGhListCDC[11].focusType = 1;
			
		else
		{
			memset( displayStr, '\0', sizeof( displayStr));
			
			SGhListCDC[*(key_loc + 1)].d_BKColor = 0x00F0FFFF;
			
			strcpy( SGhListCDC[*(key_loc + 1)].f_SName, "");
			
			strcpy( SGhListCDC[*(key_loc + 1)].f_EName, "");
			
			WM_InvalidateWindow(cdchargeWin);
		}
	}

}

/*  删除  */
void Cdc_delete_input( ){
	
	int dlen = 0, flen = 0;

	int *key_loc;
	
	if( FLAG_INPUTAUTH  ){	
	
		dlen = sizeof( SGhListCDC ) / sizeof( SGhListCDC[0] );
		
		key_loc = get_id_index( SGhListCDC, dlen);
		
		if( *key_loc == 0x194 )
			
		   SGhListCDC[11].focusType = 1;	
		
		else
		{			
			strcpy( displayStr , SGhListCDC[*(key_loc + 1)].f_EName );
			
			if( SGhListCDC[*(key_loc + 1)].t_Type == T_THref && ( strlen( displayStr) != 0 || displayStr[0] != '\0' ) )
			{
				displayStr [ strlen( displayStr) - 1 ] = '\0'; 
					
				strcpy( SGhListCDC[*(key_loc + 1)].f_SName, displayStr );	
					
				strcpy( SGhListCDC[*(key_loc + 1)].f_EName, displayStr );	
					
				WM_InvalidateWindow(cdchargeWin);				
			}	
		}
	}

}

void Cdc_value_correct( ){
	
	int dlen = 0, flen = 0, i = 0, NUM = 0, flag_null = 0; //是否为最小值
	int *key_loc;
	int temp_int, temp, dot_loc = 1024;  //是否具有小数点  1024---无   其余---小数点位置
	char temp_char[10];
	float temp_float;
	char int_char[10], float_char[10];
	vu8 flag_dot = 0;
	
	if( !FLAG_INPUTAUTH ){
		
		dlen = sizeof( SGhListCDC ) / sizeof( SGhListCDC[0] );
		
		key_loc = get_id_index( SGhListCDC, dlen);
		
		if( *key_loc == 0x194 )
			
		   SGhListCDC[11].focusType = 1;	
		
		else
		{	
		
			temp = *(key_loc + 1);
			
			strcpy( displayStr , SGhListCDC[temp].f_EName );
			
			if( (displayStr[0] == '\0' || strlen(displayStr) == 0) && SGhListCDC[temp].t_Type == T_THref ){
				
				flag_null = 1;
			}
			for( i = 0; i < strlen(displayStr); i++  ){  
			
				if( displayStr[i] == 0x2e ){		
				
					dot_loc = i;			//记录小数点位置
				}			
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
			
			switch( SGhListCDC[temp].d_ID ){
				
				case 11:   
				//====================================输出电压======================================
					if( flag_null ){  
					
						strcpy( displayStr, "0.000"  );
						
						CLists.CDC_OutPut_V = 0 * 1000;
						
						CLists.CSET_Voltage = CLists.CDC_OutPut_V;
						
					}else{
						
						if( !flag_dot ){      	
							
							if( temp_int > 30  ){   //最大值
							
								strcpy( displayStr, "30.000"  );
								
								CLists.CDC_OutPut_V = 30 * 1000;
								
								CLists.CSET_Voltage = CLists.CDC_OutPut_V;
									
								break;
							}
							if( temp_int <= 0 ){    //最小值
							
								strcpy( displayStr, "0.000"  );
								
								CLists.CDC_OutPut_V = 0 * 1000;
								
								CLists.CSET_Voltage = CLists.CDC_OutPut_V;
								
								break;
							}						
							if( strlen(int_char) > 2){
								
								strcat( int_char, ".0");
								
							}else{
								
								strcat( int_char, ".000");
								
							}
							strcpy( displayStr, int_char );
							
							CLists.CDC_OutPut_V = temp_int * 1000;
							
							CLists.CSET_Voltage = CLists.CDC_OutPut_V;
							
						}else{

							if( temp_float - 30 > 0 ){   //最大值
							
								strcpy( displayStr, "30.000"  );
								
								CLists.CDC_OutPut_V = 30 * 1000;
								
								CLists.CSET_Voltage = CLists.CDC_OutPut_V;
								
								break;
							}
							
							if( temp_float <= 0 ){    //最小值
							
								strcpy( displayStr, "0.000"  );
								
								CLists.CDC_OutPut_V = temp_int * 1000;
								
								CLists.CSET_Voltage = CLists.CDC_OutPut_V;
									
								break;
							}
							NUM = strlen(float_char) - dot_loc - 1;
							
							for( i = 0; i < 3 - NUM; i++ ){
								
								strcat( float_char, "0");
							}	
							
							NUM = ( dot_loc > 1 ) ? 6 : 5;
							
							strncpy( int_char, float_char, NUM );
							
							strcpy( displayStr, int_char );	
							
							CLists.CDC_OutPut_V = temp_float * 1000;	

							CLists.CSET_Voltage = CLists.CDC_OutPut_V;
						
						}
					}
				
				break;
				
				case 13:
				//====================================限制电流======================================
					if( flag_null ){   
					
						strcpy( displayStr, "0.000"  );
						
						CLists.CDC_Limit_C = 0 * 1000;
		 
						CLists.CSET_Current = CLists.CDC_Limit_C;
						
					}else{
						
						if( !flag_dot ){      	
							
							if( temp_int > 10  ){   //最大值
							
								strcpy( displayStr, "10.000"  );
								
								CLists.CDC_Limit_C = 10 * 1000;
								
								CLists.CSET_Current = CLists.CDC_Limit_C;
								
								break;
							}
							if( temp_int <= 0 ){    //最小值
							
								strcpy( displayStr, "0.000"  );
								
								CLists.CDC_Limit_C = 0 * 1000;
								
								CLists.CSET_Current = CLists.CDC_Limit_C;
								
								break;
							}						
							if( strlen(int_char) > 2){
								
								strcat( int_char, ".0");
								
							}else{
								
								strcat( int_char, ".000");
							}
							strcpy( displayStr, int_char );
							
							CLists.CDC_Limit_C = temp_int * 1000;
							
							CLists.CSET_Current = CLists.CDC_Limit_C;
							
						}else{

							if( temp_float - 10 > 0 ){   //最大值
							
								strcpy( displayStr, "10.000"  );
								
								CLists.CDC_Limit_C = 10 * 1000;
								
								CLists.CSET_Current = CLists.CDC_Limit_C;
								
								break;
							}
							
							if( temp_float <= 0 ){    //最小值
							
								strcpy( displayStr, "0.000"  );
								
								CLists.CDC_Limit_C = 0 * 1000;
			
								CLists.CSET_Current = CLists.CDC_Limit_C;
								
								break;
							}
							NUM = strlen(float_char) - dot_loc - 1;
							
							for( i = 0; i < 3 - NUM; i++ ){
								
								strcat( float_char, "0");
							}	
							
							NUM = ( dot_loc > 1 ) ? 6 : 5;
							
							strncpy( int_char, float_char, NUM );
							
							strcpy( displayStr, int_char );	
							
							CLists.CDC_Limit_C = temp_float * 1000;
							
							CLists.CSET_Current = CLists.CDC_Limit_C;
						}
					}
				
				break;	
				
				case 15:
				//====================================截止电压======================================
					if( flag_null ){   
					
						strcpy( displayStr, "0.000"  );
						
						CLists.CDC_Ccutoff_V = 0 * 1000;
						
					}else{
						if( !flag_dot ){  
							
							if( temp_int > 30  ){   //最大值
							
								strcpy( displayStr, "30.000"  );
								
								CLists.CDC_Ccutoff_V = 30 * 1000;
								
								break;
							}
							if( temp_int <= 0 ){    //最小值
							
								strcpy( displayStr, "0.000"  );
								
								CLists.CDC_Ccutoff_V = 0 * 1000;
								
								break;
							}						
							if( strlen(int_char) > 2){
								
								strcat( int_char, ".0");
								
							}else{
								
								strcat( int_char, ".000");
							}
							
							strcpy( displayStr, int_char );
							
							CLists.CDC_Ccutoff_V = temp_int * 1000;
							
						}else{

							if( temp_float - 30 > 0 ){   //最大值
							
								strcpy( displayStr, "30.000"  );
								
								CLists.CDC_Ccutoff_V = 30 * 1000;
								
								break;
							}
							
							if( temp_float <= 0 ){    //最小值
							
								strcpy( displayStr, "0.000"  );
								
								CLists.CDC_Ccutoff_V = 0 * 1000;
								
								break;
							}
							NUM = strlen(float_char) - dot_loc - 1;
							
							for( i = 0; i < 3 - NUM; NUM++ ){
								
								strcat( float_char, "0");
								
							}	
							NUM = ( dot_loc > 1 ) ? 6 : 5;
							
							strncpy( int_char, float_char, NUM );
							
							strcpy( displayStr, int_char );	
							
							CLists.CDC_Ccutoff_V = temp_float * 1000;
						}
					}
				
				break;	
				
				case 17:
				//====================================截止电流======================================
					if( flag_null ){   
					
						strcpy( displayStr, "0.000"  );
						
						CLists.CDC_Ccutoff_C = 0 * 1000;
						
					}else{
						
						if( !flag_dot ){      	
							
							if( temp_int > 10  ){   //最大值
							
								strcpy( displayStr, "10.000"  );
								
								CLists.CDC_Ccutoff_C = 10 * 1000;
								
								break;
							}
							if( temp_int <= 0 ){    //最小值
							
								strcpy( displayStr, "0.000"  );
								
								CLists.CDC_Ccutoff_C = 0 * 1000;
								
								break;
							}						
							if( strlen(int_char) > 2){
								
								strcat( int_char, ".0");
								
							}else{
								
								strcat( int_char, ".000");
								
							}
							strcpy( displayStr, int_char );
							
							CLists.CDC_Ccutoff_C = temp_int * 1000;
							
						}else{

							if( temp_float - 10 > 0 ){   //最大值
							
								strcpy( displayStr, "10.000"  );
								
								CLists.CDC_Ccutoff_C = 10 * 1000;
								
								break;
							}
							
							if( temp_float <= 0 ){    //最小值
							
								strcpy( displayStr, "0.000"  );
								
								CLists.CDC_Ccutoff_C = 0 * 1000;
								
								break;
							}
							NUM = strlen(float_char) - dot_loc - 1;
							
							for( i = 0; i < 3 - NUM; i++ ){
								
								strcat( float_char, "0");
							}	
							NUM = ( dot_loc > 1 ) ? 6 : 5;
							
							strncpy( int_char, float_char, NUM );
							
							strcpy( displayStr, int_char );	
							
							CLists.CDC_Ccutoff_C = temp_float * 1000;
							
						}
					}
				
				break;	
				
				case 20:
				//====================================负载电流======================================
					if( flag_null ){   
					
						strcpy( displayStr, "0.000"  );
						
						CLists.CDC_Load_C = 0 * 1000;
						
						CLists.CSET_Current_Laod =  CLists.CDC_Load_C;
						
					}else{
						
						if( !flag_dot ){      	
							
							if( temp_int > 30  ){   //最大值
							
								strcpy( displayStr, "30.000"  );
								
								CLists.CDC_Load_C = 30 * 1000;
								
								CLists.CSET_Current_Laod =  CLists.CDC_Load_C;
								
								break;
							}
							if( temp_int <= 0 ){    //最小值
							
								strcpy( displayStr, "0.000"  );
								
								CLists.CDC_Load_C = 0 * 1000;
								
								CLists.CSET_Current_Laod =  CLists.CDC_Load_C;
								
								break;
							}						
							if( strlen(int_char) > 2){
								
								strcat( int_char, ".0");
								
							}else{
								
								strcat( int_char, ".000");
							}
							strcpy( displayStr, int_char );
							
							CLists.CDC_Load_C = temp_int * 1000;
							
							CLists.CSET_Current_Laod =  CLists.CDC_Load_C;
							
						}else{

							if( temp_float - 30 > 0 ){   //最大值
							
								strcpy( displayStr, "30.000"  );
								
								CLists.CDC_Load_C = 30 * 1000;
								
								CLists.CSET_Current_Laod =  CLists.CDC_Load_C;
								
								break;
							}
							
							if( temp_float <= 0 ){    //最小值
							
								strcpy( displayStr, "0.000"  );
								
								CLists.CDC_Load_C = 0 * 1000;
								
								CLists.CSET_Current_Laod =  CLists.CDC_Load_C;
								
								break;
							}
							NUM = strlen(float_char) - dot_loc - 1;
							
							for( i = 0; i < 3 - NUM; i++ ){
								
								strcat( float_char, "0");
							}	
							NUM = ( dot_loc > 1 ) ? 6 : 5;
							
							strncpy( int_char, float_char, NUM );
							
							strcpy( displayStr, int_char );	
							
							CLists.CDC_Load_C = temp_float * 1000;
							
							CLists.CSET_Current_Laod =  CLists.CDC_Load_C;
						}
					}
				
				break;	
				
				case 22:
				//====================================截止电压======================================
					if( flag_null ){  
					
						strcpy( displayStr, "0.000"  );
						
						CLists.CDC_Dcutoff_V = 0 * 1000;
			
					}else{
						
						if( !flag_dot ){      	
							
							if( temp_int > 60  ){   //最大值
							
								strcpy( displayStr, "60.000"  );
								
								CLists.CDC_Dcutoff_V = 60 * 1000;
								
								break;
							}
							if( temp_int <= 0 ){    //最小值
							
								strcpy( displayStr, "0.000"  );
								
								CLists.CDC_Dcutoff_V = 0 * 1000;
								
								break;
								
							}						
							if( strlen(int_char) > 2){
								
								strcat( int_char, ".0");
								
							}else{
								
								strcat( int_char, ".000");
								
							}
							strcpy( displayStr, int_char );
							
							CLists.CDC_Dcutoff_V = temp_int * 1000;
							
						}else{

							if( temp_float - 60 > 0 ){   //最大值
							
								strcpy( displayStr, "60.000"  );
								
								CLists.CDC_Dcutoff_V = 60 * 1000;
								
								break;
							}
							
							if( temp_float <= 0 ){    //最小值
							
								strcpy( displayStr, "0.000"  );
								
								CLists.CDC_Dcutoff_V = 0 * 1000;
								
								break;
							}
							NUM = strlen(float_char) - dot_loc - 1;
							
							for( i = 0; i < 3 - NUM; i++ ){
								
								strcat( float_char, "0");
								
							}	
							NUM = ( dot_loc > 1 ) ? 6 : 5;
							
							strncpy( int_char, float_char, NUM );
							
							strcpy( displayStr, int_char );	
							
							CLists.CDC_Dcutoff_V = temp_float * 1000;
							
						}
					}
				
				break;	
				case 24:
				//====================================循环次数======================================
					if( flag_null ){   
					
						strcpy( displayStr, "0000"  );
						
						CLists.CDC_Cycle_Time = 0;
						
					}else{
						
						if( !flag_dot ){      		
		
							if(  temp_int > 200  ){   //最大值
							
								strcpy( displayStr, "200"  );
								
								CLists.CDC_Cycle_Time = 200;
								
								break;
							}
							if( temp_int <= 0 ){    //最小值
							
								strcpy( displayStr, "0"  );
								
								CLists.CDC_Cycle_Time = 0;
								
								break;
							}
							
							strcat( temp_char, int_char); 
							
							strcpy( displayStr, temp_char);
							
						}else{

							if( temp_float - 200 > 0 ){   //最大值
							
								strcpy( displayStr, "200"  );
								
								CLists.CDC_Cycle_Time = 200;
								
								break;
							}	
							if( temp_float <= 0 ){    //最小值
							
								strcpy( displayStr, "0"  );
								
								CLists.CDC_Cycle_Time = 0;
								
								break;
							}
							if( dot_loc + 1 == strlen(displayStr) ){							
										
							}else{
								
								temp_int++;
								
								sprintf( int_char , "%d" , temp_int);		
							}
							strcpy( displayStr, int_char ); 
						}
						
						CLists.CDC_Cycle_Time = temp_int;
					}
				
				break;	
			}
			
			Write_Flash_Bytes();	
			
			SGhListCDC[temp].d_BKColor = 0x0;
			
			strcpy( SGhListCDC[temp].f_SName, displayStr );
			
			strcpy( SGhListCDC[temp].f_EName, displayStr );
			
		}
	}
	memset( displayStr, '\0', sizeof(displayStr));
	
	WM_InvalidateWindow(cdchargeWin);

}

//trig按键
void trig_Cdc_charge(){

	vu8 i = 0;

	if( !trig_flag_state ){
		
		trig_flag_state = 1;
		
		run_num = 1;
		CDC_AvgCmah = 0;
		CDC_CsumMah = 0;
		
		memset( RunTimesStr[0], '\0', sizeof(RunTimesStr[0]));
		memset( RunTimesStr[1], '\0', sizeof(RunTimesStr[1]));
		
		/* 清空存储  */
		for( i = 0; i < 255; i++){
			
			CDC_EndTime[0][i] = 0;
			CDC_EndTime[1][i] = 0;
		}
							
		run_num = 1;
		
	    if( POWER_RECHARGE == 0  ){

			POWER_RECHARGE = 1;
			OnOff_GPOI_ResetSet( 0 , 1 );   // 开启充电
			abc_time = 0;
				
		}
	}else{
		run_num = 1;
		trig_flag_state = 0;	
		abc_time = 0;	
		Off_GPOI_ResetSet();
    }
	
	WM_InvalidateWindow(cdchargeWin);
	
}
















