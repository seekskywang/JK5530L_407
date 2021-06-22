/**
**
  电源页面
  
*/

#include "MainTask.h"
#include  "gui.h"
#include "my_register.h" 
#include "sysflag.h"
#include "key.h"

WM_HWIN hWinWind;

extern GUI_CONST_STORAGE GUI_FONT GUI_FontSONG16;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontSONG12;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontSONG32;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontSONGB32;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontSONGB16;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontSONG26;

#define ID_WINDOW_MENU    (GUI_ID_USER + 0x3E)

#define ID_BUTTON_13     	(GUI_ID_USER + 0x40)
#define ID_BUTTON_14     	(GUI_ID_USER + 0x41)
#define ID_BUTTON_15     	(GUI_ID_USER + 0x42)
#define ID_BUTTON_16     	(GUI_ID_USER + 0x43)
#define ID_BUTTON_17     	(GUI_ID_USER + 0x44)

#define ID_TimerTimeMenu    3

static const GUI_WIDGET_CREATE_INFO _aDialogCreateMenu[] = {
	
  { WINDOW_CreateIndirect, "Window", ID_WINDOW_MENU, 0, 0, 480, 272, 0, 0x0, 0 },

  { BUTTON_CreateIndirect, "Button", ID_BUTTON_13, 85,   232, 70, 45, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Button", ID_BUTTON_14, 85+79*1,  232, 70, 45, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Button", ID_BUTTON_15, 85+79*2, 232, 70, 45, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Button", ID_BUTTON_16, 85+79*3,  232, 70, 45, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Button", ID_BUTTON_17, 85+79*4,  232, 70, 45, 0, 0x0, 0 },
  
};
	
static struct  SGraphData SGhListM[] = {
		
	{1,0,T_Text ,"V:", 	"V:",	0 , 0 ,				&GUI_FontSONG32, 30,  55, 0, 0},	
	{2,0,T_Text ,"0.00", "0.00",  0x0000FF7F , 0 ,	&GUI_FontSONG26, 88,  55+3, 0, 0 }, 
	{3,0,T_Text ,"V", 	"V",	0 , 0 ,				&GUI_FontSONG32,  178, 55, 0, 0},
		
	{4,0,T_Text ,"I:", 	"I:",	0 , 0 ,				&GUI_FontSONG32,  30,  116, 0, 0},
	{5,0,T_Text ,"0.00", "0.00", 0x0000FF7F , 0 ,	&GUI_FontSONG26, 88,  116+3, 0, 0 },
	{6,0,T_Text ,"A",   "A",0, 0 , 				&GUI_FontSONG32, 178, 116, 0, 0},
		
	{7,0,T_Text ,"C:", 	"C:",	0 , 0 , 			&GUI_FontSONG32, 30,  177, 0, 0},
	{8,0,T_Text ,"00000"  , "00000", 0x0000FF7F , 0 ,&GUI_FontSONG26, 88,  177+3, 0, 0 },	
	{9,0,T_Text ,"mAH", "mAH",	0 , 0 , 			&GUI_FontSONG32, 178, 177, 0, 0},

	{10,0,T_Text ,"输出电压","Output V",	0 , 0, 	0, 		260, 90, 0, 0},
	{11,1,T_THref ,"4.20", "4.20",  0 , 0,	0, 	350, 90, 400, 107 },		
	{12,0,T_Text ,"V", 	 "V", 	0 , 0,	0,     	430, 90, 0, 0},	
		
	{13,0,T_Text ,"限制电流","Limiting C",0 , 0,	0,	260, 130, 0, 0},
	{14,0,T_THref ,"1.00", "1.00",  0 , 0 ,	0,	350, 130, 400, 147  },		
	{15,0,T_Text ,"A", 	"A",  	0 , 0,			0 ,430, 130, 0, 0},	
		
};  // 数据列表

char MStr[10];

//焦点id布局
static const int WINM_id_DataLists[] = { 11, 14 };

static void _cbDialogMenu(WM_MESSAGE * pMsg) {
	
	WM_HWIN hItem;
	
	int     NCode, Id, MAX_LEN = 0;
	
	vu8 i;
	
	char *winName, *chargeName;
	
	float ftemp;
	
	static struct SListData slistM[] = {
		
		{T_Button ,"内阻测试", "I.R. TEST", 	0x00B22222 , 0x001e9ffa,	ID_BUTTON_13 },
		
		{T_Button ,"程控电源", "POWER",  0x00B22222 , 0x001e9ffa, ID_BUTTON_14 },
		
		{T_Button ,"程控负载",   "LOAD",	0x00B22222 , 0x001e9ffa, ID_BUTTON_15 },
		
		{T_Button , "充放电", "C&DC",  0x00B22222 , 0x001e9ffa, ID_BUTTON_16 },
		
		{T_Button ,"系统设置", "SYSTEM", 	0x00B22222 , 0x001e9ffa, ID_BUTTON_17 },
	
	};
	vu8 _dloc;
	
	char mChar[5];

	switch (pMsg->MsgId) {

	case WM_PAINT:
		
		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_14);	
		
		BUTTON_SetPressed( hItem , 1 );		
		
		GUI_SetColor(0x0000FF7F);
		
		GUI_FillRoundedRect(0,0,480,33,5);
	
		GUI_SetColor(GUI_BLACK);
		
		GUI_SetFont(&GUI_FontSONG16);
		
		GUI_UC_SetEncodeUTF8();
		
		GUI_SetTextMode(GUI_TM_TRANS);//设置文本模式为底色透明
	
		winName = ( FLAG_LANG == 1) ? "POWER" : "程控电源";
		
		GUI_DispStringAt( winName, (480 - GUI_GetStringDistX( winName))/2, 10);
		
		for( i = 0; i < sizeof( SGhListM )/sizeof( SGhListM[0]) ; i++  ){
			
			bush_Graph_String( FLAG_LANG, SGhListM[i] );
		} 
		
		GUI_SetFont(&GUI_FontSONG12);
		
		GUI_UC_SetEncodeUTF8();
		
		GUI_SetTextMode(GUI_TM_TRANS);//设置文本模式为底色透明
		
		GUI_SetColor(0x00456654);
		
		chargeName = ( POWER_RECHARGE == 1 ) ? ( ( FLAG_LANG == 1) ? "Charging" : "充电中" ) : (( FLAG_LANG == 1) ? "Not Charging" : "未充电" );
			
		GUI_DispStringAt( chargeName, 460 - GUI_GetStringDistX(chargeName), 13);	
		
	break;
	
	case WM_TIMER:
			
		if(WM_GetTimerId(pMsg->Data.v) == ID_TimerTimeMenu){
			
			sprintf( MStr, "%f", DISS_POW_Voltage );
			
			_dloc = dot_Loc_isNull(MStr,strlen(MStr)) > 1 ? 6 : 5;

		    memset(SGhListM[1].f_SName, '\0', sizeof(SGhListM[1].f_SName));

			memset(SGhListM[1].f_EName, '\0', sizeof(SGhListM[1].f_EName));
				
			strncpy( SGhListM[1].f_SName, MStr, _dloc);
			
			strncpy( SGhListM[1].f_EName, MStr, _dloc);
			
			memset( MStr, '\0', sizeof(MStr));		
			
			sprintf( MStr, "%f", DISS_POW_Current );
			
			_dloc = dot_Loc_isNull(MStr,strlen(MStr)) > 1 ? 6 : 5;

		    memset(SGhListM[4].f_SName, '\0', sizeof(SGhListM[4].f_SName));

			memset(SGhListM[4].f_EName, '\0', sizeof(SGhListM[4].f_EName));
							
			strncpy( SGhListM[4].f_SName, MStr, _dloc );
			
			strncpy( SGhListM[4].f_EName, MStr, _dloc );
			
	
			memset( MStr, '\0', sizeof(MStr));	

		    memset( mChar, '\0', sizeof(mChar));				
			
			WM_InvalidateWindow(hWinWind);		
			
			WM_RestartTimer(pMsg->Data.v, 500);
		}
	case WM_KEY://接受按键消息来处理按键功能
		

	break;
	
	case WM_INIT_DIALOG://重绘外观
  
		hItem = pMsg->hWin;
		
		WINDOW_SetBkColor(hItem, 0x004A4655);
		
		WM_CreateTimer(hItem,ID_TimerTimeMenu,20,0);//创建本窗口定时器

		MAX_LEN = sizeof(slistM)/sizeof(slistM[0]);
		
		bush_WinObj( FLAG_LANG, pMsg ,slistM, MAX_LEN);
 
    break;
	
	case WM_NOTIFY_PARENT:
		
    break;
	
	default:
	
		WM_DefaultProc(pMsg);
	
    break;
  }
}



WM_HWIN CreateWindowMenu(void) {

	int dlen = 0;
	
	int *key_loc;
	
	if( FLAG_FIRSTWIN[0] )
	{
		if( CLists.POWER_OutPut_V 	  !=	0xffffff )
		
			strcpy( SGhListM[10].f_SName , Read_Data_Parse(1) ); 		strcpy( SGhListM[10].f_EName , Read_Data_Parse(1) ); 			
		
		if( CLists.POWER_Limit_C 	  != 	0xffffff )
			
			strcpy( SGhListM[13].f_SName , Read_Data_Parse(2) ); 		strcpy( SGhListM[13].f_EName , Read_Data_Parse(2) ); 		
		
		FLAG_FIRSTWIN[0] = 0;	
	} 	
	CLists.CSET_Voltage = CLists.POWER_OutPut_V;
	 
	CLists.CSET_Current = CLists.POWER_Limit_C;
	
	dlen = sizeof(SGhListM) / sizeof(SGhListM[0]);
	
	key_loc = get_id_index( SGhListM, dlen);
	
	memset( displayStr, '\0', sizeof(displayStr));
	
	if( *key_loc != 11 )
	{
		SGhListM[*(key_loc+1)].focusType = 0;
		
		SGhListM[10].focusType = 1;
	}
	hWinWind = GUI_CreateDialogBox(_aDialogCreateMenu, GUI_COUNTOF(_aDialogCreateMenu), _cbDialogMenu, WM_HBKWIN, 0, 0);
	
	return hWinWind;
}


void KEY_POWER_INPUT(char *strx){
	
	int i, dot_loc = 1024, dlen = 0;	
	int *key_loc;
	
	dlen = sizeof(SGhListM) / sizeof(SGhListM[0]);
		
	key_loc = get_id_index( SGhListM, dlen);
		
	if( *key_loc == 0x194 )
			
		SGhListM[20].focusType = 1;
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
					
					goto mmore_point;
			}
			if( dot_loc == 1024 ){
				
				if( strlen(displayStr) < 4 )
					
					strcat( displayStr, strx);
			}
			else
			{
				if( strlen(displayStr) < 6 )
					
					strcat( displayStr, strx);
			}
		}	
		if(  SGhListM[*(key_loc+1)].focusType == 1 &&( SGhListM[*(key_loc+1)].t_Type == T_THref || SGhListM[*(key_loc+1)].t_Type == T_TLISTBOX ))
		{				
			strcpy( SGhListM[*(key_loc+1)].f_SName, displayStr );	
					 
			strcpy( SGhListM[*(key_loc+1)].f_EName, displayStr );
		}			
	}
	mmore_point:
	
		WM_InvalidateWindow(hWinWind);
	
}


void KEY_DOWN_POWER( int handle )
{
	int i;
	int *key_loc;
	int get_nextid = 0, nowid = 0, nowid_index = 0, dlen = 0;  
	
	if( FLAG_INPUTAUTH == 0)
	{
		dlen = sizeof(SGhListM) / sizeof(SGhListM[0]);
		key_loc = get_id_index( SGhListM, dlen);
				
		nowid = *key_loc;
		nowid_index = *(key_loc + 1);
		
		if( nowid == 0x194 )
			
			SGhListM[10].focusType = 1;
		
		get_nextid = get_nextindex( nowid, WINM_id_DataLists, handle);
		
		if(  get_nextid != 0xFF )
		{
			SGhListM[nowid_index].focusType = 0;	
				
			for	( i = 0; i < dlen; i++)
			{
				if( SGhListM[i].d_ID == get_nextid && (SGhListM[i].t_Type == T_THref || SGhListM[i].t_Type == T_TLISTBOX) )
				{				
					  SGhListM[i].focusType = 1;	
					  break;
				}
			}	
		}
	}
	else	
	{
	
	}

}

void Power_ent(){
	
	int dlen, flen;
	
	int *key_loc;
	
	if( FLAG_INPUTAUTH ){
		
		FLAG_INPUTAUTH = 0;
		
		Power_value_correct();
		
	}else{
		
		FLAG_INPUTAUTH = 1;
		
		dlen = sizeof(SGhListM) / sizeof( SGhListM[0] );
		
		key_loc = get_id_index( SGhListM, dlen);
		
		if( *key_loc == 0x194 )
			
		   SGhListM[10].focusType = 1;
			
		else
		{
			memset( displayStr, '\0', sizeof( displayStr));
			
			SGhListM[*(key_loc+1)].d_BKColor = 0x00F0FFFF;
			
			strcpy( SGhListM[*(key_loc+1)].f_SName, "");
			
			strcpy( SGhListM[*(key_loc+1)].f_EName, "");
			
			WM_InvalidateWindow(hWinWind);
		}
	}
	
}

void Power_delete_input(){
	
	int dlen = 0;
	
	int *key_loc;
	
	if( FLAG_INPUTAUTH  ){	
	
		dlen = sizeof( SGhListM ) / sizeof( SGhListM[0] );
		
		key_loc = get_id_index( SGhListM, dlen);
		
		if( *key_loc == 0x194 )
			
		   SGhListM[10].focusType = 1;
			
		else
		{	
			strcpy( displayStr , SGhListM[*(key_loc+1)].f_EName );
			
			if( SGhListM[*(key_loc+1)].t_Type == T_THref && ( strlen( displayStr) != 0 || displayStr[0] != '\0' ) ){
				
				displayStr[ strlen( displayStr) - 1 ] = '\0'; 
					
				strcpy( SGhListM[*(key_loc+1)].f_SName, displayStr );	
					
				strcpy( SGhListM[*(key_loc+1)].f_EName, displayStr );	
					
				WM_InvalidateWindow(hWinWind);
			}
		}
	}
	
	
}

void Power_value_correct(){
	
	int dlen = 0, flen = 0, i = 0, NUM = 0, flag_null = 0; //是否为最小值
	
	int *key_loc;
	
	int temp; 
	
	int dot_loc = 1024;  //是否具有小数点  1024---无   其余---小数点位置
	
	char temp_char[10];
	
	int temp_int;
	
	float temp_float;
	
	char int_char[10], float_char[10];
	
	vu8 flag_dot = 0;
	
	if( !FLAG_INPUTAUTH ){
		
		dlen = sizeof( SGhListM ) / sizeof( SGhListM[0] );
		
		key_loc = get_id_index( SGhListM, dlen);
		
		if( *key_loc == 0x194 )
			
		   SGhListM[10].focusType = 1;
			
		else
		{
			temp = *(key_loc+1);
			
			strcpy( displayStr , SGhListM[temp].f_EName );
			
			if( (displayStr[0] == '\0' || strlen(displayStr) == 0) && SGhListM[temp].t_Type == T_THref ){
				
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
			
			switch( SGhListM[temp].d_ID ){
				
				case 10:   
				//====================================输出电压======================================
					if( flag_null ){  
					
						strcpy( displayStr, "0.000"  );
						
						CLists.POWER_OutPut_V = 0 * 1000;
						
						CLists.CSET_Voltage = CLists.POWER_OutPut_V;
						
					}else{	
					
						if( !flag_dot ){      	
							
							if( temp_int > 30  ){  
							
								strcpy( displayStr, "30.000"  );
								
								CLists.POWER_OutPut_V = 30 * 1000;
								
								CLists.CSET_Voltage = CLists.POWER_OutPut_V;
								
								break;
							}
							if( temp_int <= 0 ){    
							
								strcpy( displayStr, "0.000"  );
								
								CLists.POWER_OutPut_V = 0 * 1000;
								
								CLists.CSET_Voltage = CLists.POWER_OutPut_V;
								
								break;
							}	
							
							if( strlen(int_char) > 2){
								
								strcat( int_char, ".0");
								
							}else{
								
								strcat( int_char, ".000");
							}	
							
							strcpy( displayStr, int_char );
							
							CLists.POWER_OutPut_V = temp_int * 1000;
							
							CLists.CSET_Voltage = CLists.POWER_OutPut_V;
							
						}else{

							if( temp_float - 30 > 0 ){   //最大值
							
								strcpy( displayStr, "30.000"  );
								
								CLists.POWER_OutPut_V = 30 * 1000;
								
								CLists.CSET_Voltage = CLists.POWER_OutPut_V;
								
								break;
							}
							
							if( temp_float <= 0 ){    //最小值
							
								strcpy( displayStr, "0.000"  );
								
								CLists.POWER_OutPut_V = 0 * 1000;
								
								CLists.CSET_Voltage = CLists.POWER_OutPut_V;
								
								break;
							}	
							NUM = strlen(float_char) - dot_loc - 1;
							
							for( i = 0; i < 3 - NUM; i++ ){		strcat( float_char, "0");	}
							
							NUM = ( dot_loc > 1 ) ? 6 : 5;
							
							strncpy( int_char, float_char, NUM );
							
							strcpy( displayStr, int_char );	
							
							CLists.POWER_OutPut_V = temp_float * 1000;	

							CLists.CSET_Voltage = CLists.POWER_OutPut_V;				
						}
					}
				
				break;
				
				case 13:
				//====================================限制电流======================================
					if( flag_null ){ 
					
						strcpy( displayStr, "0.000"  );
						
						CLists.POWER_Limit_C = 0 * 1000;
		 
						CLists.CSET_Current = CLists.POWER_Limit_C;
						
					}else{	
					
						if( !flag_dot ){      	
							
							if( temp_int > 10  ){   //最大值
							
								strcpy( displayStr, "10.000"  );
								
								CLists.POWER_Limit_C = 10 * 1000;
								
								CLists.CSET_Current = CLists.POWER_Limit_C;
								
								break;
							}
							if( temp_int <= 0 ){    //最小值
							
								strcpy( displayStr, "0.000"  );
								
								CLists.POWER_Limit_C = 0 * 1000;
								
								CLists.CSET_Current = CLists.POWER_Limit_C;
								
								break;
							}								
							if( strlen(int_char) > 2){
								
								strcat( int_char, ".0");
								
							}else{
								
								strcat( int_char, ".000");
							}
							
							strcpy( displayStr, int_char );	
							
							CLists.POWER_Limit_C = temp_int * 1000;
							
							CLists.CSET_Current = CLists.POWER_Limit_C;
							
						}else{

							if( temp_float - 10 > 0 ){   //最大值
							
								strcpy( displayStr, "10.000"  );
								
								CLists.POWER_Limit_C = 10 * 1000;
								
								CLists.CSET_Current = CLists.POWER_Limit_C;
								
								break;
							}
							
							if( temp_float <= 0 ){    //最小值
							
								strcpy( displayStr, "0.000"  );
								
								CLists.POWER_Limit_C = 0 * 1000;
								
								CLists.CSET_Current = CLists.POWER_Limit_C;
									
								break;
							}
							
							NUM = strlen(float_char) - dot_loc - 1;
							
							for( i = 0; i < 3 - NUM; i++ ){		strcat( float_char, "0");		}
							
							NUM = ( dot_loc > 1 ) ? 6 : 5;
							
							strncpy( int_char, float_char, NUM );
							
							strcpy( displayStr, int_char );	
							
							CLists.POWER_Limit_C = temp_float * 1000;
							
							CLists.CSET_Current = CLists.POWER_Limit_C;
						}
					}
					
				break;	
			}
			Write_Flash_Bytes();
			
			SGhListM[temp].d_BKColor = 0;
			
			strcpy( SGhListM[temp].f_SName, displayStr );
			
			strcpy( SGhListM[temp].f_EName, displayStr );
		}
	}
	memset( displayStr, '\0', sizeof(displayStr));
	
	WM_InvalidateWindow(hWinWind);
}

void trig_power_charge(){
	
	vu8 chargeState;
	
	chargeState = ( POWER_RECHARGE == 1 ) ?  1 : 0;
	
	OnOff_GPOI_ResetSet( FLAG_WIN , chargeState );
	
	WM_InvalidateWindow(hWinWind);
	
}




































