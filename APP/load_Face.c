/******************************************************************/
/* 名称：                                                  */
/* 效果：                                                        */
/* 内容：负载界面                                                     */
/* 作者：                                                 */
/* 联系方式QQ:                                       */
/******************************************************************/
#include "MainTask.h"
#include  "gui.h"
#include "my_register.h" 
#include "sysflag.h"
#include "key.h"
#include <string.h>

WM_HWIN load_wind;  //负载界面句柄

extern GUI_CONST_STORAGE GUI_FONT GUI_FontHZ16;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontSONG16;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontSONG12;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontSONG32;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontSONGB16;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontHZ12;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontSONG26;

#define ID_WINDOW_LOAD      (GUI_ID_USER + 0x25)

#define ID_BUTTON_6     	(GUI_ID_USER + 0x26)
#define ID_BUTTON_7     	(GUI_ID_USER + 0x27)
#define ID_BUTTON_8     	(GUI_ID_USER + 0x28)
#define ID_BUTTON_9     	(GUI_ID_USER + 0x29)
#define ID_BUTTON_10     	(GUI_ID_USER + 0x30)
#define ID_BUTTON_11     	(GUI_ID_USER + 0x31)

#define ID_TimerTimeLoad  2

static const GUI_WIDGET_CREATE_INFO _aDialogCreateload[] = {
	
  { WINDOW_CreateIndirect, "Window",  ID_WINDOW_LOAD, 	0,   0,   480, 272, 0, 0x0, 0 },
  
  { BUTTON_CreateIndirect, "Button",  ID_BUTTON_7,   85,   232, 70, 45, 0, 0x0, 0 },
  
  { BUTTON_CreateIndirect, "Button",  ID_BUTTON_8,   85+79*1,  232, 70, 45, 0, 0x0, 0 },
  
  { BUTTON_CreateIndirect, "Button",  ID_BUTTON_9,   85+79*2,  232, 70, 45, 0, 0x0, 0 },
  
  { BUTTON_CreateIndirect, "Button",  ID_BUTTON_10,  85+79*3,  232, 70, 45, 0, 0x0, 0 },
  
  { BUTTON_CreateIndirect, "Button",  ID_BUTTON_11,  85+79*4,  232, 70, 45, 0, 0x0, 0 },
  
};

static struct SGraphData  SGhListL[] = {
		
		{1, 0,T_Text ,"V:", 	  	"V:",	0 , 0 ,  			&GUI_FontSONG32, 30,  55, 0, 0 },
		{2, 0,T_Text ,"0.000",		"0.000", 0x0000FF7F,  0, 	&GUI_FontSONG26, 88,  55+3, 0, 0 }, 		
		{3 ,0,T_Text ,"V", 	  		"V",	0 , 0 , 			&GUI_FontSONG32, 178, 55, 0, 0 },
		
		{4,0,T_Text ,"I:", 		"I:",	0 , 0 , 			&GUI_FontSONG32, 30,  116, 0, 0},
		{5,0,T_Text ,"0.000", 		"0.000",  0x0000FF7F , 0 , 	&GUI_FontSONG26, 88,  116+3, 0, 0},
		{6,0,T_Text ,"A",   		"A",	0, 0 , 				&GUI_FontSONG32, 178,  116, 0, 0},
		
		{7,0,T_Text ,"C:", 		"C:",	0 , 0 , 		&GUI_FontSONG32, 30,  177, 0, 0},
		{8,0,T_Text ,"00000",		"00000", 0x0000FF7F, 0, &GUI_FontSONG26,  88,  177+3, 0, 0 },
		{9,0,T_Text ,"mAH", 		"mAH", 0 , 0 , 			&GUI_FontSONG32,  178,  177, 0, 0},
		
		{10,0,T_Text ,"负载电流",	"Load C",0 , 0 , 0,  260, 90, 0, 0},
		{11,1,T_THref ,"1.80",		"1.80",	0 , 0 , 0, 	350, 90, 400, 107},		
		{12,0,T_Text ,"A", 	  		"A",	0 , 0 , 0,  430, 90, 0, 0},	

		{13,0,T_Text ,"负载电压",	"Load V",0 , 0 , 0,  260, 130, 0, 0},
		{14, 0, T_THref,"1.80",		"1.80",	0 , 0 , 0, 	350, 130, 400, 147},		
		{15,0,T_Text ,"V", 	  	"V",	0 , 0 , 0,  430, 130, 0, 0},			
 };

//焦点id布局
static const int WINL_id_DataLists[] = { 11, 14, };

char LStr[10];

static void _cbDialogload(WM_MESSAGE * pMsg) {
	
	WM_HWIN hItem;
	
	int NCode, Id, MAX_LEN = 0;
	
 	vu8 i;
	
	char* winName, *chargeName;
	
	static struct SListData slistL[] = {	
	
		{T_Button ,"内阻测试", 	"I.R. TEST",  0x00B22222 , 0,	    ID_BUTTON_7 },
		
		{T_Button ,"程控电源", 	"POWER",  0x00B22222 , 0, 	ID_BUTTON_8 },
		
		{T_Button ,"程控负载", 	"LOAD",  0x00B22222, 0, 	ID_BUTTON_9 },
		
		{T_Button , "充放电", "C&DC",  0x00B22222, 0, ID_BUTTON_10 },
		
		{T_Button ,"系统设置", 	"SYSTEM",  0x00B22222 , 0, 	ID_BUTTON_11},
		
	};
	
	vu8 _dloc;
	
	float ftemp;
	
	char lChar[4];
	
	switch (pMsg->MsgId) {
 
	case WM_PAINT://无效化重绘
	
		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_9);	
		
		BUTTON_SetPressed( hItem , 1 );		
	
		GUI_SetColor(0x0003FCF5);
		
		GUI_FillRoundedRect(0,0,480,33,5);
		
		GUI_SetColor(GUI_BLACK);
		
		GUI_SetFont(&GUI_FontSONG16);
		
		GUI_UC_SetEncodeUTF8();
		
		GUI_SetTextMode(GUI_TM_TRANS);//设置文本模式为底色透明
		
		winName = ( FLAG_LANG == 1) ? "LOAD" : "程控负载";
		
		GUI_DispStringAt( winName, (480 - GUI_GetStringDistX( winName))/2, 10);

		for( i = 0; i < sizeof( SGhListL )/sizeof( SGhListL[0]) ; i++  ){
			
			bush_Graph_String( FLAG_LANG, SGhListL[i] );
		}
		
		GUI_SetFont(&GUI_FontSONG12);
		
		GUI_UC_SetEncodeUTF8();
		
		GUI_SetColor(0x00456654);
		
		GUI_SetTextMode(GUI_TM_TRANS);//设置文本模式为底色透明
		
		chargeName = ( C_DISCHARGE == 1 ) ? ( ( FLAG_LANG == 1) ? "Discharging" : "放电中" ) : (( FLAG_LANG == 1) ?  "Not Charging" : "未放电" );
			
		GUI_DispStringAt( chargeName, 460 - GUI_GetStringDistX(chargeName), 13);		
	
	break;
	
	case WM_KEY://接受按键消息来处理按键功能

	break;
		
	case WM_TIMER://定时模块消息
	
		if(WM_GetTimerId(pMsg->Data.v) == ID_TimerTimeLoad){
		
		   	sprintf( LStr, "%f", DISS_Voltage );
			
			_dloc = dot_Loc_isNull(LStr,strlen(LStr)) > 1 ? 6 : 5;

			memset(SGhListL[1].f_SName, '\0', sizeof(SGhListL[1].f_SName));

			memset(SGhListL[1].f_EName, '\0', sizeof(SGhListL[1].f_EName));
			
			strncpy( SGhListL[1].f_SName, LStr, _dloc );
			
			strncpy( SGhListL[1].f_EName, LStr, _dloc );
			
			memset( LStr, '\0', sizeof(LStr));		
			
			sprintf( LStr, "%f", DISS_Current );
			
			_dloc = dot_Loc_isNull(LStr,strlen(LStr)) > 1 ? 6 : 5;

			memset(SGhListL[4].f_SName, '\0', sizeof(SGhListL[4].f_SName));

			memset(SGhListL[4].f_EName, '\0', sizeof(SGhListL[4].f_EName));

			strncpy( SGhListL[4].f_SName, LStr, _dloc );
			
			strncpy( SGhListL[4].f_EName, LStr, _dloc );
			
 			/* 容量  */
        /*	memset( LStr, '\0', sizeof(LStr));
			
			memset( lChar, '\0', sizeof(lChar));
				
			ftemp = atof(SGhListL[4].f_SName);
				
			ftemp = ftemp * 1000 * 1/3600 * abc_time;
				
			sprintf( lChar, "%d", ftemp );
				
			if( strlen(lChar) < 5){
					
				for( i = 0; i < 5 - strlen(lChar); i++) {
					
					strcat( LStr, "0");
				}
				strcat( LStr, lChar);
					
			}else{
					
				strncpy( LStr, lChar, 5 );
			}
				
			memset(SGhListL[7].f_SName, '\0', sizeof(SGhListL[7].f_SName));

			memset(SGhListL[7].f_EName, '\0', sizeof(SGhListL[7].f_EName));
				
			strcpy( SGhListL[7].f_SName, LStr );	
				
			strcpy( SGhListL[7].f_EName, LStr );
			
			memset( lChar, '\0', sizeof(lChar)); */
			
			memset( LStr, '\0', sizeof(LStr));	
			
			WM_InvalidateWindow(load_wind);
		
			WM_RestartTimer(pMsg->Data.v, 200);
			
		}
	break;
	
	case WM_INIT_DIALOG://重绘外观

		hItem = pMsg->hWin;
		
		WINDOW_SetBkColor(hItem, 0x004A4655);
		
		WM_CreateTimer(hItem,ID_TimerTimeLoad,20,0);//创建本窗口定时器	
		
	  MAX_LEN = sizeof(slistL)/sizeof(slistL[0]);
		
		bush_WinObj( FLAG_LANG, pMsg ,slistL, MAX_LEN);
		
    break;
	
	case WM_NOTIFY_PARENT:
	
		Id    = WM_GetId(pMsg->hWinSrc);
		
		NCode = pMsg->Data.v;
		
    break;
	
	default:
	
    WM_DefaultProc(pMsg);
	
    break;
  }

}

WM_HWIN CreateWindowload(void) {

	int dlen = 0;
		
	int *key_loc;
		
	if( FLAG_FIRSTWIN[2] )
	{
		if( CLists.LOAD_C 	  !=	0xffffff )
				
			strcpy( SGhListL[10].f_SName , Read_Data_Parse(1) ); 		strcpy( SGhListL[10].f_EName , Read_Data_Parse(1) ); 			
				
		if( CLists.LOAD_V 	  !=	0xffffff )
				
			strcpy( SGhListL[13].f_SName , Read_Data_Parse(2) ); 		strcpy( SGhListL[13].f_EName , Read_Data_Parse(2) ); 			
				
		FLAG_FIRSTWIN[2] = 0;	
			
	} 	
		
	CLists.CSET_Current_Laod =  CLists.LOAD_C;

	dlen = sizeof(SGhListL) / sizeof(SGhListL[0]);
		
	key_loc = get_id_index( SGhListL, dlen);
		
	memset( displayStr, '\0', sizeof(displayStr));
		
	if( *key_loc != 11 ){

		SGhListL[*(key_loc+1)].focusType = 0;
			
		SGhListL[10].focusType = 1;
	}

	load_wind = GUI_CreateDialogBox(_aDialogCreateload, GUI_COUNTOF(_aDialogCreateload), _cbDialogload, WM_HBKWIN, 0, 0);
		
	return load_wind;
  
}

void KEY_LOAD_INPUT(char *strx){
	
	int *key_loc;
		
	int i, dlen, flen;
		
	int dot_loc = 1024;
		
	dlen = sizeof(SGhListL) / sizeof(SGhListL[0]);
		
	key_loc = get_id_index( SGhListL, dlen);
		
	if( *key_loc == 0x194 )
			
		SGhListL[10].focusType = 1;
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
									
					goto lmore_point;
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
		if( SGhListL[*(key_loc+1)].focusType == 1 &&( SGhListL[*(key_loc+1)].t_Type == T_THref || SGhListL[*(key_loc+1)].t_Type == T_TLISTBOX) )
		{	
			strcpy( SGhListL[*(key_loc+1)].f_SName, displayStr );	
									 
			strcpy( SGhListL[*(key_loc+1)].f_EName, displayStr );		 
		}
	}
	lmore_point:	
		
		WM_InvalidateWindow(load_wind);
}


void KEY_DOWN_LOAD( int handle ){

	int i, dlen = 0, flag = 0;
	
	int *key_loc;
	
	int get_nextid = 0, nowid = 0, nowid_index = 0;
	
	if( FLAG_INPUTAUTH == 0 )
	{
		dlen = sizeof(SGhListL) / sizeof(SGhListL[0]);
		
		key_loc = get_id_index( SGhListL, dlen);
			
		nowid = *key_loc;
		
		nowid_index = *(key_loc + 1);
			
		if( nowid == 0x194 ) 
					
			SGhListL[10].focusType = 1;			
		else
				
			flag = 1;
			
		get_nextid = get_nextindex( nowid, WINL_id_DataLists, handle);
			
		if( flag && get_nextid != 0xFF )
		{
			SGhListL[nowid_index].focusType = 0;	
			
			for	( i = 0; i < dlen; i++)
			{
				if( SGhListL[i].d_ID == get_nextid && (SGhListL[i].t_Type == T_THref || SGhListL[i].t_Type == T_TLISTBOX) )
				{				
					SGhListL[i].focusType = 1;	
					break;
				}
			}	
		}
		WM_InvalidateWindow(load_wind);	
	}
	else
	{
	
	}
}


void Load_ent(){
	
	int *key_loc;
	
	int dlen = 0;
	
	if( FLAG_INPUTAUTH ){
		
		FLAG_INPUTAUTH = 0;
		
		Load_value_correct();
		
	}else{
		
		FLAG_INPUTAUTH = 1;
		
		dlen = sizeof( SGhListL ) / sizeof( SGhListL[0] );
		
		key_loc = get_id_index( SGhListL, dlen);
		
		if( *key_loc == 0x194 )
					
			SGhListL[10].focusType = 1;
		else
		{
			memset( displayStr, '\0', sizeof( displayStr));
			
			SGhListL[*(key_loc+1)].d_BKColor = 0x00F0FFFF;
			
			strcpy( SGhListL[*(key_loc+1)].f_SName, "");
			
			strcpy( SGhListL[*(key_loc+1)].f_EName, "");
			
			WM_InvalidateWindow(load_wind);
		}
			
	}
	
}

void Load_delete_input(){
	
	int dlen = 0, flen = 0;
	
	int *key_loc;
	
	if( FLAG_INPUTAUTH  ){	
	
		dlen = sizeof( SGhListL ) / sizeof(SGhListL[0]);
		
		key_loc = get_id_index( SGhListL, dlen);
		
		if( *key_loc == 0x194 )
					
			SGhListL[10].focusType = 1;
		else
		{
			strcpy( displayStr , SGhListL[*key_loc].f_EName );
			
			if( SGhListL[*key_loc].t_Type == T_THref && ( strlen( displayStr) != 0 || displayStr[0] != '\0' ) )
			{		
				displayStr [ strlen( displayStr) - 1 ] = '\0'; 
				
				strcpy( SGhListL[*key_loc].f_SName, displayStr );	
					
				strcpy( SGhListL[*key_loc].f_EName, displayStr );	
					
				WM_InvalidateWindow(load_wind);
			}
		
		}
	}
	
}

void Load_value_correct(){
	
	int dlen = 0, i = 0, NUM = 0, flag_null = 0; 
	
	int *key_loc;
	
	int temp, temp_int;
	
	int dot_loc = 1024;   //是否具有小数点  1024---无   其余---小数点位置
	
	char temp_char[10];
	
	float temp_float;
	
	char int_char[10], float_char[10];
	
	vu8 flag_dot = 0;
	
	if( !FLAG_INPUTAUTH ){
		
		dlen = sizeof( SGhListL ) / sizeof( SGhListL[0] );
		
		key_loc = get_id_index( SGhListL, dlen);
		
		if( *key_loc == 0x194 )
		
			SGhListL[10].focusType = 1;
		else
		{
			temp = *(key_loc+1);
			
			strcpy( displayStr , SGhListL[temp].f_EName );
			
			if( (displayStr[0] == '\0' || strlen(displayStr) == 0) && SGhListL[temp].t_Type == T_THref ){
				
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
			
			switch( SGhListL[temp].d_ID ){
				
				case 6:   
				//====================================负载电流======================================
					if( flag_null ){ 
					
						strcpy( displayStr, "0.000"  );
						
						CLists.LOAD_C  = 0 * 1000;
						
						CLists.CSET_Current_Laod =  CLists.LOAD_C;
						
					}else{	
						
						if( !flag_dot ){      	
							
							if( temp_int > 30  ){   //最大值
							
								strcpy( displayStr, "30.000"  );
								
								CLists.LOAD_C  = 30 * 1000;
								
								CLists.CSET_Current_Laod =  CLists.LOAD_C;
								
								break;
							}
							if( temp_int <= 0 ){    //最小值
							
								strcpy( displayStr, "0.000"  );
								
								CLists.LOAD_C  = 0 * 1000;
								
								CLists.CSET_Current_Laod =  CLists.LOAD_C;
								
								break;		
							}						
							if( strlen(int_char) > 2){
								
								strcat( int_char, ".0");
								
							}else{
								
								strcat( int_char, ".000");
								
							}
							
							strcpy( displayStr, int_char );
							
							CLists.LOAD_C  = temp_int * 1000;
							
							CLists.CSET_Current_Laod =  CLists.LOAD_C;
							
						}else{

							if( temp_float - 30 > 0 ){   //最大值
							
								strcpy( displayStr, "30.000"  );
								
								CLists.LOAD_C  = 30 * 1000;
								
								CLists.CSET_Current_Laod =  CLists.LOAD_C;
								
								break;
							}
							
							if( temp_float <= 0 ){    //最小值
							
								strcpy( displayStr, "0.000"  );
								
								CLists.LOAD_C  = 0 * 1000;
								
								CLists.CSET_Current_Laod =  CLists.LOAD_C;
								
								break;
							}
							
							NUM = strlen(float_char) - dot_loc - 1;
							
							for( i = 0; i < 3 - NUM; i++ ){		strcat( float_char, "0");		}	
							
							NUM = ( dot_loc > 1 ) ? 6 : 5;
							
							strncpy( int_char, float_char, NUM );
							
							strcpy( displayStr, int_char );	
							
							CLists.LOAD_C  = temp_float * 1000;
							
							CLists.CSET_Current_Laod =  CLists.LOAD_C;
						}
					}
				
				break;
				
				case 13:   
				//====================================负载电压======================================
					if( flag_null ){ 
					
						strcpy( displayStr, "0.000"  );
						
						CLists.LOAD_V  = 0 * 1000;
						
					}else{	
						
						if( !flag_dot ){      	
							
							if( temp_int > 60  ){   //最大值
							
								strcpy( displayStr, "60.000"  );
								
								CLists.LOAD_V  = 60 * 1000;
								
								break;
							}
							if( temp_int <= 0 ){    //最小值
							
								strcpy( displayStr, "0.000"  );
								
								CLists.LOAD_V  = 0 * 1000;
								
								break;		
							}						
							if( strlen(int_char) > 2){
								
								strcat( int_char, ".0");
								
							}else{
								
								strcat( int_char, ".000");
								
							}
							
							strcpy( displayStr, int_char );
							
							CLists.LOAD_V  = temp_int * 1000;
							
						}else{

							if( temp_float - 60 > 0 ){   //最大值
							
								strcpy( displayStr, "60.000"  );
								
								CLists.LOAD_V  = 60 * 1000;
								
								break;
							}
							
							if( temp_float <= 0 ){    //最小值
							
								strcpy( displayStr, "0.000"  );
								
								CLists.LOAD_V  = 0 * 1000;
								
								break;
							}
							
							NUM = strlen(float_char) - dot_loc - 1;
							
							for( i = 0; i < 3 - NUM; i++ ){		strcat( float_char, "0");		}	
							
							NUM = ( dot_loc > 1 ) ? 6 : 5;
							
							strncpy( int_char, float_char, NUM );
							
							strcpy( displayStr, int_char );	
							
							CLists.LOAD_V  = temp_float * 1000;
						}
					}
				
				break;
			}
			
			Write_Flash_Bytes();
			
			SGhListL[temp].d_BKColor = 0;
			
			strcpy( SGhListL[temp].f_SName, displayStr );
			
			strcpy( SGhListL[temp].f_EName, displayStr );
		}
	}
	memset( displayStr, '\0', sizeof(displayStr));
	
	WM_InvalidateWindow(load_wind);
	
}

void trig_load_discharge(){

	vu8 chargeState;
	
	chargeState = ( C_DISCHARGE == 1 ) ?  1 : 0;
	
	OnOff_GPOI_ResetSet( FLAG_WIN , chargeState );
	
	WM_InvalidateWindow(load_wind);
}






























