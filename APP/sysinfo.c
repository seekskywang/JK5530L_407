/**
**
  系统信息页面
  
*/

#include "DIALOG.h"
#include  "gui.h"
#include "my_register.h" 
#include "key.h"
#include "sysflag.h"
#include "string.h"

WM_HWIN sysinfoWin;

extern GUI_CONST_STORAGE GUI_FONT GUI_FontSONG16;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontSONG12;

#define ID_WINDOW_SYSINFO (GUI_ID_USER + 0x101)

#define ID_BUTTON_36 (GUI_ID_USER + 0x102)
#define ID_BUTTON_37 (GUI_ID_USER + 0x103)
#define ID_BUTTON_38 (GUI_ID_USER + 0x104)
#define ID_BUTTON_39 (GUI_ID_USER + 0x105)
#define ID_BUTTON_40 (GUI_ID_USER + 0x106)

#define ID_TimerTimeSYSINFO  7

#define ENTERPASSWORD  "123456"
#define MAX_PWD_LEN    15  
#define MAX_INPUT_PWD  3    //最大输入密码的次数

void show_months_ListBox();

static const GUI_WIDGET_CREATE_INFO _aDialogCreatesysinfo[] = {
	
  { WINDOW_CreateIndirect, "Window", ID_WINDOW_SYSINFO, 	0, 0, 	 480, 272, 	0, 0x0, 0 },
  
  { BUTTON_CreateIndirect, "Button", ID_BUTTON_36, 	85,    232, 70, 45, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Button", ID_BUTTON_37, 	85+79*1, 	 232, 70, 45, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Button", ID_BUTTON_38, 	85+79*2, 	 232, 70, 45, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Button", ID_BUTTON_39, 	85+79*3, 	 232, 70, 45, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Button", ID_BUTTON_40, 	85+79*4, 	 232, 70, 45, 0, 0x0, 0 },
  
};

static vu8 FLAG_PWD = 0;    //是否解开密码

static vu8 ent_pwd = 0;      //是否按下确认键

char c_password[MAX_PWD_LEN];
	
static struct SGraphData SGhListSYSinfo[] = {
	 
	{1,0,T_Text ,"仪器型号",	"Instrument Model", 0 , 0 , 0, 30,  45, 0,	0 },
	{2,0,T_Text ,"这是仪器型号","Instrument Model", 0 , 0 , 0, 180, 45, 0,	0 },
	
	{3,0,T_Text ,"软件版本",	"Software Version", 0 , 0 , 0, 30,  75, 0,	0 },
	{4,0,T_Text ,"这是软件版本","Software", 0 , 0 , 0, 		   115, 75, 0,	0 },
	
	{5,0,T_Text ,"硬件版本",	"Hardware Version", 0 , 0 , 0, 30,  75+30*1,  0,	0 },
	{6,0,T_Text ,"这是硬件版本","Hardware", 0 , 0 , 0,		   115, 75+30*1,  0,	0 },
	
	{7,0,T_Text ,"生产日期",	"Production Date", 0 , 0 , 0, 	30,  75+30*2,  0,	0 },
	{8,1,T_THref ,"2012",		"2012", 		   0 , 0 , 0, 	115, 75+30*2,  150,	75+30*2+17 },
	{9,0,T_Text ,"-",			"-",			 0 , 0 , 0,   	155, 75+30*2,  0,	0 },
	{10,0, T_THref ,"02",		"02", 			0 , 0 , 0, 		173, 75+30*2,  193,	75+30*2+17 },
	{11,0,T_Text ,"-",		"-", 		  		0 , 0 , 0, 		198, 75+30*2,  0,	0 },
	{12,0,T_THref ,"11",		"11", 			0 , 0 , 0, 		213, 75+30*2,  233,	75+30*2+17 },
	
	{13,0,T_Text ,"仪器编号",	"Serial NO.", 0 , 	0 , 0, 		30, 75+30*3, 0,	0 },
	{14,0,T_THref ,"jk5530",	"jk5530", 0 , 		0 , 0,	 	115,75+30*3, 205,	75+30*2+17 },
}; 

static struct SGraphData S_PWD[] = {
		 
	{1,0, T_Text ,"密码:",	"PassWord:", 	0 , 0 , &GUI_FontSONG12, 30,  75+30*4, 0,	0 },	
	{2,1, T_THref ,"",        "", 			0 , 0x00F0FFFF , &GUI_FontSONG12, 115,  75+30*4, 205,	75+30*3+17 },
		
}; 

static struct SGraphData m_List[] ={
	
	/*  月  */
	{1, 0, T_THref, "01", "01", 0, 0, 0, 0, 0, 0, 0 },
	{2, 0, T_THref, "02", "02", 0, 0, 0, 0, 0, 0, 0 },	
	{3, 0, T_THref, "03", "03", 0, 0, 0, 0, 0, 0, 0 },
	{4, 0, T_THref, "04", "04", 0, 0, 0, 0, 0, 0, 0 },	
	{5, 0, T_THref, "05", "05", 0, 0, 0, 0, 0, 0, 0 },
	{6, 0, T_THref, "06", "06", 0, 0, 0, 0, 0, 0, 0 },
	{7, 0, T_THref, "07", "07", 0, 0, 0, 0, 0, 0, 0 },
	{8, 0, T_THref, "08", "08", 0, 0, 0, 0, 0, 0, 0 },
	{9, 0, T_THref, "09", "09", 0, 0, 0, 0, 0, 0, 0 },
	{10, 0, T_THref, "10", "10", 0, 0, 0, 0, 0, 0, 0 },
	{11, 0, T_THref, "11", "11", 0, 0, 0, 0, 0, 0, 0 },
	{12, 0, T_THref, "12", "12", 0, 0, 0, 0, 0, 0, 0 },
	
};

int sloc[4] = { 1024, 1024, 1024, 1024};          //存放显示数据的坐标  默认1024  

static const int month_focusList[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

//焦点id布局
static const int WINSI_id_DataLists[] = { 8, 10, 12,14, 14, 14 };  //2行3列

const int v_ud_a[3] = { 8, 10, 12 };
	
static void _cbDialogsysinfo(WM_MESSAGE * pMsg) {
	
	WM_HWIN hItem;
	int NCode, x, y;
	int Id,MAX_LEN = 0;
	vu8 i;
	char *winName,*displayName;
	
	static struct SListData slistSYSinfo[] = {	
	
		{T_Button ,  "内阻测试", "I.R. TEST",  0x00B22222 , 0,  ID_BUTTON_36 },
		{T_Button ,  "程控电源", "POWER",      0x00B22222 , 0, 	ID_BUTTON_37 },
		{T_Button ,  "程控负载", "LOAD", 	   0x00B22222 , 0, 	ID_BUTTON_38 },
		{T_Button ,  "充放电",   "C&DC", 	   0x00B22222 , 0,  ID_BUTTON_39 },
		{T_Button ,  "系统设置", "SYSTEM",     0x00B22222 , 0, 	ID_BUTTON_40 },
		
	};
	
	switch (pMsg->MsgId) {
	  	
		case WM_PAINT: {
			
			GUI_SetColor(0x00FFD700);
			
			GUI_Clear();
			
			GUI_FillRoundedRect(0,0,480,33,5);
			
			GUI_SetColor(GUI_BLACK);
			
			GUI_SetFont(&GUI_FontSONG16);
			
			GUI_UC_SetEncodeUTF8();
			
			GUI_SetTextMode(GUI_TM_TRANS);//设置文本模式为底色透明
			
			winName = ( FLAG_LANG == 1) ? "System Information" : "系统信息";
		
			GUI_DispStringAt( winName, (480 - GUI_GetStringDistX( winName))/2, 9);
			
			if( FLAG_LANG == 1 ){  //英文过长需重新定位
			
				SGhListSYSinfo[1].d_x = 180;    SGhListSYSinfo[1].d_y = 45;
				
				SGhListSYSinfo[3].d_x = 180;    SGhListSYSinfo[3].d_y = 75;
				
				SGhListSYSinfo[5].d_x = 180;    SGhListSYSinfo[5].d_y = 75+30*1;
				
				SGhListSYSinfo[7].d_x = 180;    SGhListSYSinfo[7].d_y = 75+30*2;
				SGhListSYSinfo[7].d_x1 = 180+35;    SGhListSYSinfo[8].d_y1 = 75+30*2+17;
				
				SGhListSYSinfo[8].d_x = 180+40;    SGhListSYSinfo[8].d_y = 75+30*2;
				
				SGhListSYSinfo[9].d_x = 180+58;    SGhListSYSinfo[9].d_y = 75+30*2;
				SGhListSYSinfo[9].d_x1 = 180+78;    SGhListSYSinfo[9].d_y1 = 75+30*2+17;
				
				SGhListSYSinfo[10].d_x = 180+83;    SGhListSYSinfo[10].d_y = 75+30*2;
				
				SGhListSYSinfo[11].d_x = 180+101;    SGhListSYSinfo[11].d_y = 75+30*2;
				SGhListSYSinfo[11].d_x1 = 180+121;    SGhListSYSinfo[11].d_y1 = 75+30*2+17;
				
				SGhListSYSinfo[13].d_x = 180;    SGhListSYSinfo[13].d_y = 75+30*3;
				SGhListSYSinfo[13].d_x1 = 270;    SGhListSYSinfo[13].d_y1 = 75+30*3+17;
				
			}else{
				
				SGhListSYSinfo[1].d_x = 115;    SGhListSYSinfo[1].d_y = 45;
				
				SGhListSYSinfo[3].d_x = 115;    SGhListSYSinfo[3].d_y = 75;
				
				SGhListSYSinfo[5].d_x = 115;    SGhListSYSinfo[5].d_y = 75+30*1;
				
				SGhListSYSinfo[7].d_x = 115;    SGhListSYSinfo[7].d_y = 75+30*2;
				SGhListSYSinfo[7].d_x1 = 115+35;    SGhListSYSinfo[8].d_y1 = 75+30*2+17;
				
				SGhListSYSinfo[8].d_x = 115+40;    SGhListSYSinfo[8].d_y = 75+30*2;
				
				SGhListSYSinfo[9].d_x = 115+58;    SGhListSYSinfo[9].d_y = 75+30*2;
				SGhListSYSinfo[9].d_x1 = 115+78;    SGhListSYSinfo[9].d_y1 = 75+30*2+17;
				
				SGhListSYSinfo[10].d_x = 115+83;    SGhListSYSinfo[10].d_y = 75+30*2;
				
				SGhListSYSinfo[11].d_x = 115+101;    SGhListSYSinfo[11].d_y = 75+30*2;
				SGhListSYSinfo[11].d_x1 = 115+121;    SGhListSYSinfo[11].d_y1 = 75+30*2+17;
				
				SGhListSYSinfo[13].d_x = 115;    SGhListSYSinfo[13].d_y = 75+30*3;
				SGhListSYSinfo[13].d_x1 = 205;    SGhListSYSinfo[13].d_y1 = 75+30*3+17;
				
			}	
			for( i = 0; i < sizeof( SGhListSYSinfo )/sizeof( SGhListSYSinfo[0]) ; i++  ){
				
				bush_Graph_String( FLAG_LANG, SGhListSYSinfo[i]);
			}
			
			GUI_SetFont(&GUI_FontSONG12);
		
			GUI_UC_SetEncodeUTF8();
			
			GUI_SetTextMode(GUI_TM_TRANS);//设置文本模式为底色透明
			
			GUI_SetColor(0x00456654);
			
			displayName = ( !FLAG_PWD == 1 ) ? ( ( FLAG_LANG == 1) ? "Page Locking" : "页面锁定" ) : (( FLAG_LANG == 1) ? "" : "" );
				
			GUI_DispStringAt( displayName, 460 - GUI_GetStringDistX(displayName), 13);	
			
			if( ent_pwd ){  
			
				if( !FLAG_PWD ){
					
					x = 450 - GUI_GetStringDistX(( FLAG_LANG == 1 ) ? S_PWD[0].f_EName :  S_PWD[0].f_SName ) - 85;

					S_PWD[0].d_x = x - 20 ;    S_PWD[0].d_y = 212;
				
					S_PWD[1].d_x = x + GUI_GetStringDistX(( FLAG_LANG == 1 ) ? S_PWD[0].f_EName :  S_PWD[0].f_SName ) - 15;   		
					S_PWD[1].d_y = 210;
					S_PWD[1].d_x1 = x + GUI_GetStringDistX(( FLAG_LANG == 1 ) ? S_PWD[0].f_EName :  S_PWD[0].f_SName ) + 100;    
					S_PWD[1].d_y1 = 225; 
					
					for( i = 0; i < sizeof( S_PWD )/sizeof( S_PWD[0]) ; i++  ){
				
						  bush_Graph_String( FLAG_LANG, S_PWD[i]);
					}	
				}
			}
			if( FLAG_PWD ){
				
				//show_months_ListBox();
			}
			
		}
		
		break;

		case WM_TIMER:{
			
			if(WM_GetTimerId(pMsg->Data.v) == ID_TimerTimeSYSINFO){	
			
				WM_RestartTimer(pMsg->Data.v, 500);//复位定时器数值越大刷新时间越短	
				
			}	
			break;
		}
		break;	
		
		case WM_KEY:
		
		break;
			
		case WM_INIT_DIALOG:{
				
			hItem = pMsg->hWin;
			
			WINDOW_SetBkColor(hItem, 0x004A4655);
			
			WM_CreateTimer( hItem ,ID_TimerTimeSYSINFO , 20 , 0);//创建本窗口定时器
			
			MAX_LEN = sizeof(slistSYSinfo)/sizeof(slistSYSinfo[0]);
			
			bush_WinObj( FLAG_LANG, pMsg, slistSYSinfo, MAX_LEN);
  
		}
		break;
		default:
		
			WM_DefaultProc(pMsg);
			
		break;
	 }

}

WM_HWIN CreateSysinfo(void);
WM_HWIN CreateSysinfo(void){
	
	int dlen, flen, _n = 0;
	
	int *key_loc;
	
	if( FLAG_FIRSTWIN[6] ){
		
		if(  CLists.SYS_SYEARS 	      !=	0xffffff ){
			
			strcpy( SGhListSYSinfo[7].f_SName ,  Read_Data_Parse(1) ); 	
			
			strcpy( SGhListSYSinfo[7].f_EName ,  Read_Data_Parse(1) ); 
		}
		if(  CLists.SYS_SMONTHS 	  !=	0xffffff ){
			
			strcpy( SGhListSYSinfo[9].f_SName ,  Read_Data_Parse(2) ); 	
			
			strcpy( SGhListSYSinfo[9].f_EName ,  Read_Data_Parse(2) ); 	
		}
		if(  CLists.SYS_SDAYS 	      !=	0xffffff ){
			
			strcpy( SGhListSYSinfo[11].f_SName , Read_Data_Parse(3) ); 	
			
			strcpy( SGhListSYSinfo[11].f_EName , Read_Data_Parse(3) ); 
		}
		if(  CLists.SYS_SerialNO 	  !=	0xffffff ){
			
			strcpy( SGhListSYSinfo[13].f_SName , Read_Data_Parse(4) ); 	
			
			strcpy( SGhListSYSinfo[13].f_EName , Read_Data_Parse(4) ); 	
		}
		
		FLAG_FIRSTWIN[6] = 0;	
	} 	

	dlen = sizeof( SGhListSYSinfo ) / sizeof( SGhListSYSinfo[0] );
	
	key_loc = get_id_index( SGhListSYSinfo, dlen);
	
	memset( displayStr, '\0', sizeof(displayStr));
	
 	if( *key_loc != 8 ){

		SGhListSYSinfo[*(key_loc+1)].focusType = 0;	
		
		SGhListSYSinfo[7].focusType = 1;
	} 
	sysinfoWin = GUI_CreateDialogBox(_aDialogCreatesysinfo, GUI_COUNTOF(_aDialogCreatesysinfo), _cbDialogsysinfo, WM_HBKWIN, 0, 0);
	
	return sysinfoWin;	
}

void KEY_DOWN_SYSINFO( int handle)
{
		int dlen = 0, i = 0;
		
		vu8 _f = 0;
		
		int *key_loc;
		
		int get_nextid = 0, nowid = 0, nowid_index = 0;
		
		dlen = sizeof( SGhListSYSinfo ) / sizeof( SGhListSYSinfo[0] );
		
		key_loc = get_id_index( SGhListSYSinfo, dlen);
		
		nowid = *key_loc;
		
		nowid_index = *(key_loc + 1);
		
		if( nowid == 0x194 )
			
				SGhListSYSinfo[7].focusType = 1;
		
		else
		{
				memset( displayStr, '\0', sizeof(displayStr));
			
				if( FLAG_INPUTAUTH == 0 )
				{	
						
						get_nextid = get_nextindex( nowid, WINSI_id_DataLists, handle);
					
						if( get_nextid != 0xFF )
						{
								SGhListSYSinfo[nowid_index].focusType = 0;
							
								for	( i = 0; i < dlen; i++)
								{
										if( SGhListSYSinfo[i].d_ID == get_nextid && (SGhListSYSinfo[i].t_Type == T_THref || SGhListSYSinfo[i].t_Type == T_TLISTBOX) )
										{				
												SGhListSYSinfo[i].focusType = 1;	
												break;
										}
								}	
						
						}
						WM_InvalidateWindow(sysinfoWin);	
				}
				else
				{

				}			
			
		}
}

/*  输入  */
void KEY_SYSINFO_INPUT(char *strx){
	
	int *key_loc;
	
	int i, dlen, flen, temp;
	
	vu8 _error = 0, not_d = 0;
	
	if( FLAG_PWD )
	{
			dlen = sizeof( SGhListSYSinfo ) / sizeof( SGhListSYSinfo[0] );
		
			key_loc = get_id_index( SGhListSYSinfo, dlen);;
			
			if( *key_loc == 0x194  )
				
					SGhListSYSinfo[7].focusType = 1;
			
			else
			{
					if( *strx == 0x2e && (*key_loc == 8 || *key_loc == 10 ||  *key_loc == 12 || *key_loc == 14 )   )
					{
					
					}
					else
					{
							switch( *key_loc )
							{	
									case 8:    if( strlen(displayStr) < 4 )	   strcat( displayStr, strx);    break;
									case 10:	
									case 12:   if( strlen(displayStr) < 2 )	   strcat( displayStr, strx);    break;		
									case 14:   if( strlen(displayStr) < 8 )    strcat( displayStr, strx);    break;			
							}	
							if( SGhListSYSinfo[*(key_loc + 1)].focusType == 1 && ( SGhListSYSinfo[*(key_loc + 1)].t_Type == T_THref || SGhListSYSinfo[*(key_loc + 1)].t_Type == T_TLISTBOX )   )
							{
									
									strcpy( SGhListSYSinfo[*(key_loc + 1)].f_SName, displayStr );	
											 
									strcpy( SGhListSYSinfo[*(key_loc + 1)].f_EName, displayStr );
							}	
							WM_InvalidateWindow(sysinfoWin);	
					}					
			}
	}
	else
	{
			if( strlen( c_password) < MAX_PWD_LEN -1 )
			{    
					 strcat( displayStr, "*");  

					 strcat( c_password, strx); 

					 strcpy( S_PWD[1].f_SName, displayStr );	
									 
					 strcpy( S_PWD[1].f_EName, displayStr );	

					 WM_InvalidateWindow(sysinfoWin);				
			}		
	}
	
}

//确认键
void Sysinfo_ent(){
	
	int *key_loc;
	
	int dlen, flen, temp;
	
	vu8 i = 0, x_x = 0, y_y = 0;
	
	if( FLAG_PWD ){
		
		dlen = sizeof(SGhListSYSinfo) / sizeof( SGhListSYSinfo[0] );
		
		key_loc = get_id_index( SGhListSYSinfo, dlen);;
			
		if( *key_loc == 0x194  )
				
				SGhListSYSinfo[7].focusType = 1;
			
		else
		{		
				memset( displayStr, '\0', sizeof(displayStr));
		
				temp = *(key_loc + 1);

				switch( SGhListSYSinfo[temp].t_Type ){
			
						case T_TLISTBOX:  {
				
							if( !FLAG_INPUTAUTH )
							{
					
									FLAG_INPUTAUTH = 1;
								
									if( *key_loc == 10)
									{
											//初始化月份列表显示坐标
											for( i = 0; i < sizeof( m_List) / sizeof( m_List[0]); i++ )
											{
													if( m_List[i].d_x == 0|| m_List[i].d_y == 0 || m_List[i].d_x1 == 0 || m_List[i].d_y1 == 0 )
													{
															if( i % 4 == 0 && i != 0 ){    y_y++;    x_x = 0;    }
																
															m_List[i].d_x  = SGhListSYSinfo[temp].d_x + ( 20 * x_x );
																
															m_List[i].d_y  = SGhListSYSinfo[temp].d_y + ( 17 * y_y );
																
															m_List[i].d_x1 = SGhListSYSinfo[temp].d_x + ( 20 * x_x ) + 20 ;
																
															m_List[i].d_y1 = SGhListSYSinfo[temp].d_y + ( 17 * y_y ) + 17;	
													}		
													if( strcmp( SGhListSYSinfo[temp].f_EName, m_List[i].f_EName ) == 0 )	
													{
															m_List[i].focusType = 1;
																	
															m_List[i].d_BKColor = 0x00F0FFFF;
													}
													else
													{ 
															m_List[i].focusType = 0;
																	
															m_List[i].d_BKColor = 0x0;									
													}
													x_x++;
											}
									}									
							}
							else
							{
									FLAG_INPUTAUTH = 0;
									
									for( i = 0; i < sizeof( m_List) / sizeof( m_List[0]); i++ )
									{
											m_List[i].d_BKColor =  0x0;
										
											if( m_List[i].focusType == 1 && SGhListSYSinfo[temp].d_ID == 9 ){
																	
													strcmp( SGhListSYSinfo[temp].f_SName, m_List[i].f_SName );
											
													strcmp( SGhListSYSinfo[temp].f_EName, m_List[i].f_EName );	
											
													CLists.SYS_SMONTHS = atoi( m_List[i].f_EName );	
											}	
									}					
									Write_Flash_Bytes();					
							}
							WM_InvalidateWindow(sysinfoWin);				
				}
				
				break;
				
				case T_THref:{
					
					if( FLAG_INPUTAUTH ){
						
							FLAG_INPUTAUTH = 0;
							
							Sysinfo_value_correct();
						
					}else{
						
							FLAG_INPUTAUTH = 1;
							
							SGhListSYSinfo[temp].d_BKColor = 0x00F0FFFF;
				
							strcpy( SGhListSYSinfo[temp].f_SName, "");
				
							strcpy( SGhListSYSinfo[temp].f_EName, "");
				
							WM_InvalidateWindow(sysinfoWin);			
					}
				
				}
				
				break;
				
				case T_Text:
				
				break;
				
				case T_Button:
				
				break;
				
			}
		
	  }
	}
	else
	{
			if( ent_pwd )
			{
					FLAG_INPUTAUTH = 0;
					
					ent_pwd = 0;
			
					if( strcmp( ENTERPASSWORD , c_password) == 0 )
						
							FLAG_PWD = 1;	
						
					memset( c_password, '\0', sizeof(c_password));
					
					memset( displayStr, '\0', sizeof(displayStr));
					
					strcpy( S_PWD[1].f_SName, "" );	
									 
					strcpy( S_PWD[1].f_EName, "" );
			}
			else
			{			
					FLAG_INPUTAUTH = 1;
					
					ent_pwd = 1;
						
					memset( c_password, '\0', sizeof(c_password));
						
					strcpy( S_PWD[1].f_SName, "" );	
										 
					strcpy( S_PWD[1].f_EName, "" );			
			}	
			WM_InvalidateWindow(sysinfoWin);
	}
	
}

//删除输入值
void Sysinfo_delete_input(){
	
		int dlen = 0, flen = 0;
		
		int *key_loc;
		
		char temp_char[15];
		
		if( FLAG_INPUTAUTH ){	
			
				if( FLAG_PWD){
					
						dlen = sizeof(SGhListSYSinfo) / sizeof( SGhListSYSinfo[0] );
				
						key_loc = get_id_index( SGhListSYSinfo, dlen);;
							
						if( *key_loc == 0x194  )
								
								SGhListSYSinfo[7].focusType = 1;
							
						else
						{
								strcpy( displayStr , SGhListSYSinfo[*(key_loc+1)].f_EName );
								
								if( SGhListSYSinfo[*(key_loc+1)].t_Type == T_THref && ( strlen( displayStr) != 0 || displayStr[0] != '\0' ) ){
									
										displayStr [ strlen( displayStr) - 1 ] = '\0'; 
										
										strcpy( SGhListSYSinfo[*(key_loc+1)].f_SName, displayStr );	
										
										strcpy( SGhListSYSinfo[*(key_loc+1)].f_EName, displayStr );	
										
										WM_InvalidateWindow(sysinfoWin);	
								}								
						}	
				}
				else
				{
						strcpy( temp_char , S_PWD[1].f_EName );
						
						if( S_PWD[1].t_Type == T_THref && ( strlen( temp_char) != 0 || temp_char[0] != '\0' ) &&  ( strlen( c_password) != 0 || c_password[0] != '\0' ) )
						{
							
								temp_char [ strlen( temp_char) - 1 ] = '\0'; 
								
								c_password [ strlen(c_password) -1 ] = '\0';
									
								strcpy( S_PWD[1].f_SName, temp_char );	
								
								strcpy( S_PWD[1].f_EName, temp_char );	
								
								WM_InvalidateWindow(sysinfoWin);				
						}		
				}
		}

}

void Sysinfo_value_correct(){
	
	int dlen = 0, flen = 0, i = 0, temp_int = 0, dot_loc;
	
	int *key_loc; 
	
	char int_char[10], temp_char[10], float_char[10];
	
	float temp_float;
	
	int temp = 0, year = 0, month = 0;
	
	vu8 flag_null = 0, is_leep = 0, flag_dot = 0;

	if( !FLAG_INPUTAUTH ){
		
		dlen = sizeof( SGhListSYSinfo ) / sizeof( SGhListSYSinfo[0] );
		
		key_loc = get_id_index( SGhListSYSinfo, dlen);;
							
		if( *key_loc == 0x194  )
								
				SGhListSYSinfo[7].focusType = 1;
							
		else
		{
		
			temp = *key_loc;

			strcpy( displayStr , SGhListSYSinfo[temp].f_EName );

			if( (displayStr[0] == '\0' || strlen(displayStr) == 0) && SGhListSYSinfo[temp].t_Type == T_THref ){
				
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
			
			switch( SGhListSYSinfo[temp].d_ID ){
				
				case 7:   
				//====================================     年     ======================================
					if( flag_null ){
							
						strcpy( displayStr, "2017"  );	
						
						CLists.SYS_SYEARS = 2017;
						
					}else{	
						
						if( !flag_dot ){
							
							if( temp_int > 2200  ){
									
								strcpy( displayStr, "2200"  );	
								
								CLists.SYS_SYEARS = 2200;	
								
								break;		
							}
								
							if( temp_int <= 2017  ){  
								
								strcpy( displayStr, "2017"  );
								
								CLists.SYS_SYEARS = 2017;
								
								break;		
							}		
								
							if( strlen(int_char) < 4){		strcpy( int_char, "2017" );		}
							
							CLists.SYS_SYEARS = atoi( int_char);
							
							strcpy( displayStr, int_char );	
						}			
					}			
				
				break;
				
				case 9:
				//====================================     月     ======================================
					if( flag_null ){
							
						strcpy( displayStr, "01"  );
						
						CLists.SYS_SMONTHS = 1;
							
					}else{	
						
						if( !flag_dot ){
							
							if( temp_int > 12  ){
									
								strcpy( displayStr, "12"  );    CLists.SYS_SMONTHS = 12;	
								
								break;		
							}					
							if( temp_int <= 0  ){  
								
								strcpy( displayStr, "01"  );    CLists.SYS_SMONTHS = 1;	
								
								break;		
							}			
							if( strlen( int_char) < 2 ){

								strcpy( displayStr, "0"	);    strcat( displayStr, int_char );	
								
								CLists.SYS_SMONTHS = temp_int;
								
								break;
							}			
							CLists.SYS_SMONTHS = temp_int;
							
							strcpy( displayStr, int_char );	
						}			
					}
					
				break;	
				
				case 11:   
				//====================================     日     ======================================
					if( flag_null ){
							
						strcpy( displayStr, "01"  );	
						
						CLists.SYS_SDAYS = 1;
							
					}else{	
						
						if( !flag_dot ){
							
							year = atoi( SGhListSYSinfo[7].f_EName);
							
							month = atoi( SGhListSYSinfo[9].f_EName);
							
							//判断年份
							if( year%400 == 0 || ( year%100 != 0 && year%4 == 0 ) ){	is_leep	= 1;	}
							
							if( temp_int <= 0 ){	strcpy( displayStr, "01"  );    CLists.SYS_SDAYS = 1;    break;		}
							
							//判断月份
							switch( month ){
								
								case 1:
								case 3:
								case 5:
								case 7:
								case 8:
								case 10:
								case 12:
								
									if( temp_int > 31 ){
										
										strcpy( int_char, "31"  );    CLists.SYS_SDAYS = 31;	
										
										break;		
									}

								break;
								
								case 2:
									
									if( is_leep ){
										
										if( temp_int > 29 ){    strcpy( int_char, "29"  );    CLists.SYS_SDAYS = 29;    break;	}
										
									}else{
										
										if( temp_int > 28 ){    strcpy( int_char, "28"  );	  CLists.SYS_SDAYS = 28;    break;	}	
									}
						
								break;
								
								case 4:
								case 6:
								case 9:
								case 11:
								
									if( temp_int > 30 ){
										
										strcpy( int_char, "30"  );    CLists.SYS_SDAYS = 30;			
									
										break;		
									}
									
								break;
							}
							
							if( strlen( int_char) < 2 ){
								
								strcpy( displayStr, "0"	);    strcat( displayStr, int_char );	
								
								CLists.SYS_SDAYS = atoi( int_char);

								break;
								
							}	
							strcpy( displayStr, int_char );	
							
							CLists.SYS_SDAYS = temp_int;
						}			
					}
				
				break;
				
				case 13:   
				//====================================  仪器编号  ======================================					
					if( flag_null  )
					{
						
						strcpy( displayStr, SGhListSYSinfo[7].f_EName );
								
						strcat( displayStr, SGhListSYSinfo[9].f_EName );
								
						strcat( displayStr, SGhListSYSinfo[11].f_EName );							
							
						CLists.SYS_SerialNO = atoi( displayStr);		
						
					}
					else
					{	
						
						if( !flag_dot ){
							
							if( strlen(displayStr) < 8) {	
							
								strcpy( displayStr, SGhListSYSinfo[7].f_EName );
								
								strcat( displayStr, SGhListSYSinfo[9].f_EName );
								
								strcat( displayStr, SGhListSYSinfo[11].f_EName );

								CLists.SYS_SerialNO = atoi( displayStr);		
								
								break;
							}											
							CLists.SYS_SerialNO = temp_int;
						}			
					}	
				
				break;
			}
			Write_Flash_Bytes();	
			
			SGhListSYSinfo[temp].d_BKColor = 0;
			
			strcpy( SGhListSYSinfo[temp].f_SName, displayStr );
			
			strcpy( SGhListSYSinfo[temp].f_EName, displayStr );
		}
	}
	memset( displayStr, '\0', sizeof(displayStr));
	
	WM_InvalidateWindow(sysinfoWin);

}

/**************************************************
*
*                显示下拉列表
*
***************************************************
*
*  @ _data       显示的数据
*  @ _lines      显示的行数
*  @ _action     当前操作    1/向下   2/向上
*
***************************************************/
void display_ListBox( struct SGraphData _data, vu8 _lines, vu8 _haction );
void display_ListBox( struct SGraphData _data, vu8 _lines, vu8 _haction ){
	
	vu8 len = 0,  i = 0, j = 0;
		
	int LCD_MAX_HEIGHT = 272;     //当前屏幕的高度
	
	vu8 show_normal = 0;          //显示方向  0/不显示  1/向下   -1/向上
	
	vu8 f_size = 0;               //字体高度
	
	f_size = GUI_GetStringDistX( FLAG_LANG == 1 ? _data.f_EName : _data.f_SName );    //每行的高度
	
	if( _data.d_y - ( f_size + 2 ) * _lines + 20  > 0 ){     
		
		show_normal = -1;             
	}
	if( LCD_MAX_HEIGHT - _data.d_y >= ( f_size + 2 ) * _lines + 20 ){
		
		show_normal = 1;
	}
	
	switch( _data.d_ID ){
		
		case 9:
		
			len = sizeof( m_List) / sizeof( m_List[0]);
			
			if( sloc[0] == 1024 ){
				
				sloc[0] = 9;   //存放id
				sloc[1] = 0;   
				sloc[2] = 0;
				sloc[3] = 0;
			}
			
			if( show_normal != 0 ){
		
				if( show_normal == 1 ){
					
					for( i = 0; i < len; i++ ){
				
						//页面显示后   当前焦点坐标是记录坐标列表中的最后一位  则重新赋值
						
						
						
					}
					
					
				}else if( show_normal == -1 ){
					
					
					
					
				}	
			}
		
		break;
	}
		
}

//显示月份列表
void show_months_ListBox()
{
		int i = 0, num = 0, dlen = 0;
		
		int *key_loc;
		
		int key_id;
		
		dlen = sizeof(SGhListSYSinfo) / sizeof( SGhListSYSinfo[0] );
				
		key_loc = get_id_index( SGhListSYSinfo, dlen);;
								
		if( *key_loc == 0x194  )
									
				SGhListSYSinfo[7].focusType = 1;
								
		else
		{	
				if( FLAG_INPUTAUTH &&  *key_loc == 10)
				{
						for( i = 0; i < sizeof( m_List )/sizeof( m_List[0]) ; i++  )
							
								bush_Graph_String( FLAG_LANG , m_List[i]);		
				}
		}
	
}











































