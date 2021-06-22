/**
**
  系统设置页面
  
*/

#include "DIALOG.h"
#include  "gui.h"
#include "my_register.h" 
#include "sysflag.h"
#include "key.h"
#include "string.h"

WM_HWIN sysSettingWin;

extern GUI_CONST_STORAGE GUI_FONT GUI_FontHZ16;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontSONG16;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontSONG12;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontSONG32;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontSONGB16;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontHZ12;

#define ID_WINDOW_SYSSETTING (GUI_ID_USER + 0x93)

#define ID_BUTTON_31 (GUI_ID_USER + 0x95)
#define ID_BUTTON_32 (GUI_ID_USER + 0x96)
#define ID_BUTTON_33 (GUI_ID_USER + 0x97)
#define ID_BUTTON_34 (GUI_ID_USER + 0x98)
#define ID_BUTTON_35 (GUI_ID_USER + 0x99)

#define ID_TimerTimeSYS  6

void bush_boxList();

int *sum_box_corres( int key_id );

static const GUI_WIDGET_CREATE_INFO _aDialogCreatesys[] = {
	
  { WINDOW_CreateIndirect, "Window", ID_WINDOW_SYSSETTING, 	0, 0, 	 480, 272, 	0, 0x0, 0 },
  
  { BUTTON_CreateIndirect, "Button", ID_BUTTON_31, 	85,    232, 70, 45, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Button", ID_BUTTON_32, 	85+79*1, 	 232, 70, 45, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Button", ID_BUTTON_33, 	85+79*2, 	 232, 70, 45, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Button", ID_BUTTON_34, 	85+79*3, 	 232, 70, 45, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Button", ID_BUTTON_35, 	85+79*4, 	 232, 70, 45, 0, 0x0, 0 },
  
};

/*  数据列表    */
static struct SGraphData SGhListSYS[] = {
		
		{1,0,T_Text ,"分选开关","Comparator", 0 , 0 , 0, 			30,	45,      0,	0 },
		{2,1,T_TLISTBOX ,"ON",	"ON", 	0 , 0 ,0 ,  				115,45,	  165,  62  },
		
		{3,0,T_Text ,"过流测试","Overflow", 0 , 0 , 0,  			30, 70	, 0	,	0 },
		{4,0,T_TLISTBOX ,"ON",	"ON", 	0 , 0 , 0 , 				115,70 , 165,	87 }, 
		
		{5,0,T_Text ,"报警声音","Beep", 0 , 0 , 0, 					30,	70+25*1,	0,	0 },
		{6,0,T_TLISTBOX ,"ON",	"ON", 	0 , 0 , 0 ,					115,70+25*1,	165,70+25*1+17 }, 
		
		{7,0,T_Text ,"电压上限", "UL Voltage",0 , 0 ,  0, 			30,	70+25*2,	0,	0 },
		{8,0,T_THref ,"36.00","36.00", 	0 , 0 , 0 , 				115,70+25*2,	165,	70+25*2+17 },
		{9,0,T_Text ,"V",		"V", 0 , 0 ,  0, 					170,70+25*2,	0,	0 },
		
		{10,0,T_Text ,"电压下限","LL Voltage", 0 , 0 ,  0, 			30, 70+25*3,	0,	0 },
		{11,0,T_THref ,"0.00",	"0.00", 0 , 0 , 0 ,  				115,70+25*3,	165,	70+25*3+17 },
		{12,0,T_Text ,"V",		"V", 0 , 0 , 0,   					170,70+25*3,	0,	0 },	
		
		{13,0,T_Text ,	"内阻上限","UL I.I.", 0 , 0 , 0,  			30, 70+25*4,	0,	0 },
		{14,0,T_THref ,	"300",		"300",0 , 0 , 0 ,  				115,70+25*4,	165,	70+25*4+17 },			
		{15,0,T_Text ,	"m",		"m", 0 , 0 , 0,   				170,70+25*4,	0,	0 },	
		{16,0,T_Text ,	"Ω",		"Ω", 0 , 0 ,  0,  				178,72+25*4,	0,	0 },
		
		{17,0,T_Text ,	"内阻下限","LL I.R.", 0 , 0 ,  0,  			30, 70+25*5,	0,	0 },
		{18,0,T_THref ,	"0.00",		"0.00",0 , 0 , 	0 , 			115,70+25*5,	165,70+25*5+17},
		{19,0,T_Text ,	"m",		"m", 0 , 0 , 0,   				170,70+25*5,	0,	0 },	
		{20,0,T_Text ,	"Ω",		"Ω", 0 , 0 ,  0,  				178,72+25*5,	0,	0 },
		
		{21,0,T_Text ,	"过流上限","UL Overflow", 0 , 0 ,  0,  		250,45,	0,	0 },
		{22,0,T_THref ,	"1.00",	"1.00",0 , 0 , 0 ,  				355,45,	405, 62 },
		{23,0,T_Text ,	"A",		"A", 0 , 0 , 0,    				410,45,	0,	0 },
		
		{24,0,T_Text ,	"过流下限","LL Overflow", 0 , 0 ,  0,  		250,70,	0,	0 },
		{25,0,T_THref ,	"0.00",	"0.00",0 , 0 , 0 ,  				355,70,	405,	87 },
		{26,0,T_Text ,	"A",		"A", 0 , 0 , 	0, 				410,70,  0,	0 },
		
		{27,0,T_Text ,	"电池类型", "Battery Type",0 , 0 ,   0, 	250,95,	0,	0 },
		{28,0,T_TLISTBOX ,	"锂电池","Lion", 	0 , 0 , 0 , 		355,95,	455,	112 },
		
		{29,0,T_Text ,		"语言","Language",	 0 , 0 ,  0,  		250,120,  0,	0 },
		{30,0,T_TLISTBOX ,	"中文",	"Chinese", 0 , 0 , 0 ,  		355,120, 455,	137 }
		
	}; 

/*  下拉框列表  */	
static struct SGraphData BoxData_List[] = {
		
	{2,0,T_LBox_Text ,"ON",  "ON", 			0 , 0 , 0,   0, 0, 0, 0 },
	{2,0,T_LBox_Text ,"OFF", "OFF", 		0 , 0 , 0,   0, 0, 0, 0 },
	
	{4,0,T_LBox_Text ,"ON",	 "ON", 			0 , 0 , 0,   0, 0, 0, 0 },
	{4,0,T_LBox_Text ,"OFF", "OFF", 		0 , 0 , 0,   0, 0, 0, 0 }, 
	
	{6,0,T_LBox_Text ,"ON",  "ON", 			0 , 0 , 0,   0, 0, 0, 0 },
	{6,0,T_LBox_Text ,"OFF", "OFF", 		0 , 0 , 0,   0, 0, 0, 0 }, 
	
	{28,0,T_LBox_Text ,"锂电池",  "Lion", 		0 , 0 , 0,   0, 0, 0, 0 },
	{28,0,T_LBox_Text ,"镍氢电池",  "NiMH", 	0 , 0 , 0,   0, 0, 0, 0 },
	{28,0,T_LBox_Text ,"镍镉电池",  "NiCD", 	0 , 0 , 0,   0, 0, 0, 0 },
	{28,0,T_LBox_Text ,"小型铅酸电池", "SLA", 	0 , 0 , 0,   0, 0, 0, 0 },
	{28,0,T_LBox_Text ,"锂锰电池",  "LiMH", 	0 , 0 , 0,   0, 0, 0, 0 },
	
	{30,0,T_LBox_Text ,"中文", "Chinese", 	    0 , 0 , 0,   0, 0, 0, 0 }, 
	{30,0,T_LBox_Text ,"English", "English", 	0 , 0 , 0,   0, 0, 0, 0 }, 
	
};

//焦点id布局
static const int WINST_id_DataLists[] = { 

		2, 22,
		4, 25,
		6, 28, 
		8, 30, 
		11, 0,
		14, 0,
		18, 0,
};
	
static void _cbDialogsys(WM_MESSAGE * pMsg) {
	
	WM_HWIN hItem;
	int NCode;
	int Id,MAX_LEN = 0;
	vu8 i;
	char* winName;
	
	static struct SListData slistSYS[] = {		
		{T_Button ,"内阻测试","I.R. TEST",  0x00B22222 , 0, ID_BUTTON_31 },
		{T_Button , "程控电源", "POWER",  0x00B22222 , 0, ID_BUTTON_32 },
		{T_Button ,"程控负载", "LOAD", 0x00B22222 , 0, ID_BUTTON_33 },
		{T_Button ,"充放电",   "C&DC", 0x00B22222 , 0,  ID_BUTTON_34 },
		{T_Button ,"系统设置","SYSTEM",  0x00B22222 , 0, ID_BUTTON_35 },
	};
	 
	switch (pMsg->MsgId) {
	  	
		case WM_PAINT: {
			
			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_35);	
			
			BUTTON_SetPressed( hItem , 1 );	
			
			GUI_SetColor(0x00FDF5E6);
			
			GUI_Clear();
			
			GUI_FillRoundedRect(0,0,480,33,5);
			
			GUI_SetColor(GUI_BLACK);
			
			GUI_SetFont(&GUI_FontSONG16);
			
			GUI_UC_SetEncodeUTF8();
			
			GUI_SetTextMode(GUI_TM_TRANS);//设置文本模式为底色透明
			
			winName = ( FLAG_LANG == 1) ? "SYSTEM TEST" : "系统设置";
		
			GUI_DispStringAt( winName, (480 - GUI_GetStringDistX( winName))/2, 10);
	
			for( i = 0; i < sizeof( SGhListSYS )/sizeof( SGhListSYS[0]) ; i++  ){
				
				bush_Graph_String( FLAG_LANG, SGhListSYS[i]);
			}
			bush_boxList();	
		}
		
		break;

		case WM_TIMER:{
			
			if(WM_GetTimerId(pMsg->Data.v) == ID_TimerTimeSYS){	
			
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
			
			WM_CreateTimer( hItem ,ID_TimerTimeSYS , 20 , 0);//创建本窗口定时器
			
			MAX_LEN = sizeof(slistSYS)/sizeof(slistSYS[0]);
			
			bush_WinObj( FLAG_LANG,pMsg ,slistSYS, MAX_LEN);
  
		}
		break;
		default:
		
			WM_DefaultProc(pMsg);
			
		break;
	 }

}

WM_HWIN CreateSysSetting(void);
WM_HWIN CreateSysSetting(void){
	
	int dlen = 0, _n = 0;
	char _t[1];
	int *key_loc;
	
	if( FLAG_FIRSTWIN[5] ){
		
		if( CLists.SYS_Comparator 	 	!=	0xffffff ){	
			
			
			if( CLists.SYS_Comparator  == 1 ){
				
				strcpy( SGhListSYS[1].f_SName, BoxData_List[0].f_SName ); 		
				strcpy( SGhListSYS[1].f_EName, BoxData_List[0].f_SName ); 		
				
			}else{
				
				strcpy( SGhListSYS[1].f_SName, BoxData_List[1].f_SName ); 		
				strcpy( SGhListSYS[1].f_EName, BoxData_List[1].f_EName ); 
			}
				
		}
		
		if( CLists.SYS_Overflow 	  	!=	0xffffff ){	
		
			if( CLists.SYS_Overflow == 1 ) {
				
				strcpy( SGhListSYS[3].f_SName , BoxData_List[2].f_SName  ); 		
				strcpy( SGhListSYS[3].f_EName , BoxData_List[2].f_EName  ); 	
				
			}else{
				
				strcpy( SGhListSYS[3].f_SName , BoxData_List[3].f_SName  ); 		
				strcpy( SGhListSYS[3].f_EName , BoxData_List[3].f_EName ); 
			}
						
		}
		
		if( CLists.SYS_Beep 	  		!=	0xffffff ){	
		
			if( CLists.SYS_Beep == 1 ) {
				
				strcpy( SGhListSYS[5].f_SName , BoxData_List[4].f_SName ); 		
				strcpy( SGhListSYS[5].f_EName , BoxData_List[4].f_EName  ); 		
			}else{
				
				strcpy( SGhListSYS[5].f_SName , BoxData_List[5].f_SName ); 		
				strcpy( SGhListSYS[5].f_EName , BoxData_List[5].f_EName  ); 		
				
			}
				
		}
		
		if( CLists.SYS_UL_V 	  		!=	0xffffff ){	
		
			strcpy( SGhListSYS[7].f_SName , Read_Data_Parse(4) ); 		
			strcpy( SGhListSYS[7].f_EName , Read_Data_Parse(4) ); 			
		}
		
		if( CLists.SYS_LL_V 	  	  	!=	0xffffff ){	
		
			strcpy( SGhListSYS[10].f_SName , Read_Data_Parse(5) ); 		
			strcpy( SGhListSYS[10].f_EName , Read_Data_Parse(5) ); 			
		}
		
		if( CLists.SYS_UL_II 	  		!=	0xffffff ){	
		
			strcpy( SGhListSYS[13].f_SName , Read_Data_Parse(6) ); 		
			strcpy( SGhListSYS[13].f_EName , Read_Data_Parse(6) ); 			
		}
		
		if( CLists.SYS_LL_IR 	  		!=	0xffffff ){	
		
			strcpy( SGhListSYS[17].f_SName , Read_Data_Parse(7) ); 		
			strcpy( SGhListSYS[17].f_EName , Read_Data_Parse(7) ); 			
		}
		
		if( CLists.SYS_UL_Overflow 	  	!=	0xffffff ){	
		
			strcpy( SGhListSYS[21].f_SName , Read_Data_Parse(8) ); 		
			strcpy( SGhListSYS[21].f_EName , Read_Data_Parse(8) ); 			
		}
		
		if( CLists.SYS_LL_Overflow 	  	!=	0xffffff ){	
		
			strcpy( SGhListSYS[24].f_SName , Read_Data_Parse(9) ); 		
			strcpy( SGhListSYS[24].f_EName , Read_Data_Parse(9) ); 			
		}
		
		if( CLists.SYS_BatteryType 	 	!=	0xffffff ){	
		
			sprintf(_t, "%d", CLists.SYS_BatteryType);
			
			_n = Box_Data_Location( 25, _t, 1);
			
 			strcpy( SGhListSYS[27].f_SName , BoxData_List[_n].f_SName); 		
 			strcpy( SGhListSYS[27].f_EName , BoxData_List[_n].f_EName); 	
		}
		
		if( CLists.SYS_LANUGAGE 	  	!=	0xffffff ){	
		
			if( CLists.SYS_LANUGAGE == 1 ){
				
				strcpy( SGhListSYS[29].f_SName ,  BoxData_List[12].f_SName); 		
				strcpy( SGhListSYS[29].f_EName ,  BoxData_List[12].f_EName); 		
				
			}else{
				
				strcpy( SGhListSYS[29].f_SName ,  BoxData_List[11].f_SName); 		
				strcpy( SGhListSYS[29].f_EName ,  BoxData_List[11].f_EName); 
				
			}		
		}
		FLAG_FIRSTWIN[5] = 0;	
	} 	
	
	
	dlen = sizeof(SGhListSYS) / sizeof(SGhListSYS[0]);
	
	key_loc = get_id_index( SGhListSYS, dlen);
	
	memset( displayStr, '\0', sizeof(displayStr));
	
	if( *key_loc != 2 ){

		SGhListSYS[*key_loc].focusType = 0;	
		
		SGhListSYS[1].focusType = 1;
	}
	
	sysSettingWin = GUI_CreateDialogBox(_aDialogCreatesys, GUI_COUNTOF(_aDialogCreatesys), _cbDialogsys, WM_HBKWIN, 0, 0);
	
	return sysSettingWin;	
}


void KEY_DOWN_SYSSET( int handle){
	
	int i,temp = 0,t = 0;
	
	int *key_loc, *sumCorres;
	
	int get_nextid = 0, nowid = 0, nowid_index = 0, dlen = 0;  
	
	dlen = sizeof(SGhListSYS) / sizeof(SGhListSYS[0]);
	
	key_loc = get_id_index( SGhListSYS, dlen);
	
	nowid = *key_loc;
	
	nowid_index = *(key_loc + 1);
	
	if( nowid == 0x194 )
			
		SGhListSYS[1].focusType = 1;
	else
	{	
	
		if( FLAG_INPUTAUTH == 0)    //不可输入状态  
		{	
			get_nextid = get_nextindex( nowid, WINST_id_DataLists, handle);
			
			if( get_nextid != 0xFF )
			{
				SGhListSYS[nowid_index].focusType = 0;	
					
				for	( i = 0; i < dlen; i++)
				{
					if( SGhListSYS[i].d_ID == get_nextid && (SGhListSYS[i].t_Type == T_THref || SGhListSYS[i].t_Type == T_TLISTBOX) )
					{				
						  SGhListSYS[i].focusType = 1;	
						  break;
					}
				}	
			}
		}
		else if( FLAG_INPUTAUTH == 1 )  //可输入状态
		{
			if( SGhListSYS[nowid_index].t_Type == T_TLISTBOX )   //下拉框 
			{
				sumCorres = sum_box_corres( nowid );
				
				if( *sumCorres == *(sumCorres+1) )
						
					temp = 1;
				else
					
					temp = *(sumCorres+1) + 1;
				
				for( i = 0; i < sizeof( BoxData_List ) / sizeof( BoxData_List[0] ); i++)
				{		
					if( BoxData_List[i].d_ID == nowid )
					{							
						t++;
							
						if( BoxData_List[i].focusType == 1 ){
								
							BoxData_List[i].focusType = 0;
								
							BoxData_List[i].d_BKColor = 0x0;
						}
							
						if( t == temp ){
								
							BoxData_List[i].focusType = 1;
								
							BoxData_List[i].d_BKColor = 0x00F0FFFF;
						}				
					}
				}						
			}		
		}	
	}
	WM_InvalidateWindow(sysSettingWin);	
}
 


//返回下拉列表的个数  焦点所在的位置
int *sum_box_corres( int key_id ){
	
	int i, dlen = 0;
	int sum_focusloc[] = { 1024, 1024};
	int sum = 0; 
	
	dlen = sizeof( BoxData_List ) / sizeof( BoxData_List[0] );
	
	for ( i = 0; i < dlen; i++ )
	{	
		if( BoxData_List[i].d_ID == key_id )
		{	
			sum++;
			
			if( BoxData_List[i].focusType == 1 )
				
				sum_focusloc[1] = sum;		
		}	
	}
	sum_focusloc[0] = sum;
	
	return sum_focusloc;
}

void KEY_SYSSET_INPUT(char *strx){
	
	int *key_loc;
	int i, dlen, flen;
	int dot_loc = 1024;
	
	dlen = sizeof( SGhListSYS ) / sizeof( SGhListSYS[0] );
	
	key_loc = get_id_index( SGhListSYS, dlen);
	
	if( *key_loc == 0x194 )
			
		 SGhListSYS[1].focusType = 1;
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
					
					goto stmore_point;
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

		if( SGhListSYS[*(key_loc+1)].focusType == 1 && (SGhListSYS[*(key_loc+1)].t_Type == T_THref || SGhListSYS[*(key_loc+1)].t_Type == T_TLISTBOX ))
		{	
			strcpy( SGhListSYS[*(key_loc+1)].f_SName, displayStr );	
					 
			strcpy( SGhListSYS[*(key_loc+1)].f_EName, displayStr );
		}	
	stmore_point:	
		WM_InvalidateWindow(sysSettingWin);
	
	}
	
	
	
}

//根据 值/id 找位置
int Box_Data_Location( int _k_d, char _k_v[], vu8 _type)
{
	int _i = 0, _loc = 0;
	
	for( _i = 0; _i < sizeof( BoxData_List ) / sizeof( BoxData_List[0]); _i++ )
	{
		if( BoxData_List[_i].d_ID == _k_d )
		{
			if( ( _type == 0 && strcmp( _k_v, BoxData_List[_i].f_EName ) == 0 ) || ( _type == 1 && _i == atoi(_k_v) ) )
			{	
				_loc = _i;
				
				break;
			}
		}			
	}
	
	return _loc;
	
}

//确认键
void Sysset_ent( ){
	
	int i = 0, num = 0, dlen, flag = 0;
	int *key_loc;	
	int key_id = 0, key_index = 0;
	
	dlen = sizeof( SGhListSYS ) / sizeof( SGhListSYS[0] );
	
	key_loc = get_id_index( SGhListSYS, dlen);
	
	memset( displayStr, '\0', sizeof(displayStr));
	
	if( *key_loc == 0x194 )
			
		 SGhListSYS[1].focusType = 1;
	else
	{
		key_id = *key_loc;
		
		key_index = *(key_loc + 1);
		
		switch( SGhListSYS[key_index].t_Type )
		{
			case T_TLISTBOX:{    //下拉列表
				
				if( !FLAG_INPUTAUTH )
				{
					FLAG_INPUTAUTH = 1;	
					
					//初始化界面
					for( i = 0; i < sizeof( BoxData_List ) / sizeof( BoxData_List[0]); i++)
					{
						
						if( BoxData_List[i].d_ID == key_id  )
						{			
							//确认显示坐标
							if( BoxData_List[i].d_x == 0||BoxData_List[i].d_y == 0 || BoxData_List[i].d_x1 == 0 || BoxData_List[i].d_y1 == 0 ){
								
								BoxData_List[i].d_x  = SGhListSYS[key_index].d_x;
								
								BoxData_List[i].d_y  = SGhListSYS[key_index].d_y + ( 25 * num );
								
								BoxData_List[i].d_x1 = SGhListSYS[key_index].d_x + 110 ;
								
								BoxData_List[i].d_y1 = SGhListSYS[key_index].d_y + ( 25 * num ) + 17;	
							}	
							
							if( strcmp( SGhListSYS[key_index].f_EName ,BoxData_List[i].f_EName) == 0 )	{
								
									BoxData_List[i].focusType = 1;
									
									BoxData_List[i].d_BKColor = 0x00F0FFFF;
									
							}else{ 
							
									BoxData_List[i].focusType = 0;
									
									BoxData_List[i].d_BKColor = 0x0;									
							}
							num++ ;
						}	
					}	
					
				}
				else
				{
					FLAG_INPUTAUTH = 0;
						
					for( i = 0; i < sizeof( BoxData_List ) / sizeof( BoxData_List[0]); i++ )
					{	
						BoxData_List[i].d_BKColor =  0x0;
							
						if( BoxData_List[i].focusType == 1 && BoxData_List[i].d_ID == key_id  )
						{	
							strcpy( SGhListSYS[key_index].f_SName, BoxData_List[i].f_SName );
								
							strcpy( SGhListSYS[key_index].f_EName, BoxData_List[i].f_EName );
								
							//======================================分选开关========================================
							if( key_id == 2 )
							{	
								if( strcmp(SGhListSYS[key_index].f_EName, "ON") == 0 ) 		
										
										CLists.SYS_Comparator = 1;
		
								if( strcmp(SGhListSYS[key_index].f_EName, "OFF") == 0 ) 
										
										CLists.SYS_Comparator = 0;										
							}
								
							//======================================过流测试========================================
							if( key_id == 4 )
							{	
								if( strcmp(SGhListSYS[key_index].f_EName, "ON") == 0 )  {
					
									FLAG_OVTest = 1;
										
									CLists.SYS_Overflow = 1;
	
								}
								if( strcmp(SGhListSYS[key_index].f_EName, "OFF") == 0 )  {
										
									FLAG_OVTest = 0;	
										
									CLists.SYS_Overflow = 0;									
								}
							}
								
							//======================================报警声音========================================
							if( key_id == 6 )
							{	
								if( strcmp(SGhListSYS[key_index].f_EName, "ON") == 0 )  	
										
									CLists.SYS_Beep = 1;
									
								if( strcmp(SGhListSYS[key_index].f_EName, "OFF") == 0 )  
										
									CLists.SYS_Beep = 0;									
							}
								
							//======================================电池类型========================================
							if( key_id == 28 )
							{	
								num = Box_Data_Location( key_id, SGhListSYS[key_index].f_EName, 0);
									
								CLists.SYS_BatteryType = num ;		
							}
								
							//=======================================语言===========================================
							if( key_id == 30 )
							{
								if( strcmp( SGhListSYS[key_index].f_EName , "English") == 0 )
								{
									FLAG_LANG = 1;
										
									CLists.SYS_LANUGAGE = 1 ;
								}
								if( strcmp( SGhListSYS[key_index].f_EName , "Chinese") == 0 )
								{
										
									FLAG_LANG = 0;
										
									CLists.SYS_LANUGAGE = 0 ;
								}
															
								delteWindow();
									
								CreateSysSetting();
							}					
						}					
					}
					Write_Flash_Bytes();					
				}	
				WM_InvalidateWindow(sysSettingWin);
			}

			break;
			
			case T_THref:{
			
				if( FLAG_INPUTAUTH ){
		
					FLAG_INPUTAUTH = 0;
					
					Sysset_value_correct();
					
				}else{
					
					FLAG_INPUTAUTH = 1;
					
					SGhListSYS[key_index].d_BKColor = 0x00F0FFFF;
		
					strcpy( SGhListSYS[key_index].f_SName, "");
		
					strcpy( SGhListSYS[key_index].f_EName, "");
		
					WM_InvalidateWindow(sysSettingWin);			
				}	
			
			}
				
			break;
			
			default:  break;
		}
	}
	
}

//下拉列表显示
void bush_boxList(){
	
	int i = 0, dlen = 0;
	
	int *key_loc;
	
	if( FLAG_INPUTAUTH )
	{
		dlen = sizeof( SGhListSYS ) / sizeof( SGhListSYS[0] );
		
		key_loc = get_id_index( SGhListSYS, dlen);

		if( *key_loc == 0x194 )
				
			 SGhListSYS[1].focusType = 1;
		
		else
		{	
			for( i = 0; i < sizeof( BoxData_List )/sizeof( BoxData_List[0]) ; i++  )
			{
				if( BoxData_List[i].d_ID ==  *key_loc )
					
					bush_Graph_String( FLAG_LANG , BoxData_List[i]);		
					
			}		
		}
	}
	
}

/*  删除  */
void Sysset_delete_input(){
	
	int dlen = 0;
	
	int *key_loc;

	if( FLAG_INPUTAUTH  ){	
	
		dlen = sizeof( SGhListSYS ) / sizeof(SGhListSYS[0]);
		
		key_loc = get_id_index( SGhListSYS, dlen);
		
		if( *key_loc == 0x194 )
			
		   SGhListSYS[1].focusType = 1;
			
		else
		{		
			strcpy( displayStr , SGhListSYS[*(key_loc+1)].f_EName );
			
			if( SGhListSYS[*key_loc].t_Type == T_THref && ( strlen( displayStr) != 0 || displayStr[0] != '\0' ) )
			{
				displayStr[ strlen( displayStr) - 1 ] = '\0'; 
					
				strcpy( SGhListSYS[*(key_loc+1)].f_SName, displayStr );	
					
				strcpy( SGhListSYS[*(key_loc+1)].f_EName, displayStr );	
					
				WM_InvalidateWindow(sysSettingWin);				
			}
		}

	}

}

/*  数据格式  */
void Sysset_value_correct( )
	{
	
	int dlen = 0, flen = 0, i = 0, NUM = 0, flag_null = 0; //是否为最小值
	int *key_loc;
	int temp, dot_loc = 1024;  //是否具有小数点  1024---无   其余---小数点位置
	char temp_char[10], int_char[10], float_char[10];
	int temp_int, num_int;
	float temp_float, num_float;
	vu8 flag_dot = 0;
	
	if( !FLAG_INPUTAUTH ){
		
		dlen = sizeof( SGhListSYS ) / sizeof( SGhListSYS[0]);
		
		key_loc = get_id_index( SGhListSYS, dlen);
		
		if( *key_loc == 0x194 )
			
		   SGhListSYS[1].focusType = 1;
			
		else
		{	
		
			temp = *(key_loc+1);
			
			strcpy( displayStr , SGhListSYS[temp].f_EName );
			
			if( (displayStr[0] == '\0' || strlen(displayStr) == 0) && SGhListSYS[temp].t_Type == T_THref ){
				
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
			switch( SGhListSYS[temp].d_ID ){
				
				case 7:   
				//======================================07 电压上限========================================
					if( flag_null ){   
					
						strcpy( displayStr,"0.000"  );
						
						CLists.SYS_UL_V = 0 * 1000;
						
					}else{
						
						num_float = atof( SGhListSYS[10].f_EName );
						
						if( temp_float - num_float <= 0 ){
		
							strcpy( SGhListSYS[10].f_SName, "0.000"  );
							
							strcpy( SGhListSYS[10].f_EName, "0.000"  );
							
							CLists.SYS_LL_V = 0 * 1000;
						}
						
						if( !flag_dot ){      	
							
							if( temp_int > 60  ){   //最大值
							
								strcpy( displayStr, "60.000"  );
								
								CLists.SYS_UL_V = 60 * 1000;
								
								break;
							}
							if( temp_int <= 0 ){    //最小值
							
								strcpy( displayStr, "0.000"  );
								
								CLists.SYS_UL_V = 0 * 1000;
								
								break;
							}						
							if( strlen(int_char) > 2){
								
								strcat( int_char, ".0");
								
							}else{
								
								strcat( int_char, ".000");
							}
							strcpy( displayStr, int_char );
							
							CLists.SYS_UL_V = temp_int * 1000;
							
						}else{

							if( temp_float - 60 > 0 ){   //最大值
							
								strcpy( displayStr, "60.000"  );
								
								CLists.SYS_UL_V = 60 * 1000;
								
								break;
							}
							
							if( temp_float <= 0 ){    //最小值
							
								strcpy( displayStr, "0.000"  );
								
								CLists.SYS_UL_V = 0 * 1000;
								
								break;
							}
							NUM = strlen(float_char) - dot_loc - 1;
							
							for( i = 0; i < 3 - NUM; i++ ){
								
								strcat( float_char, "0");
							}	
							NUM = ( dot_loc > 1 ) ? 6 : 5;
							
							strncpy( int_char, float_char, NUM );
							
							strcpy( displayStr, int_char );	
							
							CLists.SYS_UL_V = temp_float * 1000;
						}
					}
				
				break;
				
				case 10:
				//======================================10 电压下限========================================
					if( flag_null ){   
					
						strcpy( displayStr,"0.000"  );
						
						CLists.SYS_LL_V = 0 * 1000;
						
					}else{

						num_float = atof( SGhListSYS[7].f_EName );
						
							if( num_float - temp_float <= 0 ){
							
								strcpy( displayStr, "0.000"  );						
									
								CLists.SYS_LL_V = 0 * 1000;					
								
								break;
						}
							
						if( !flag_dot ){      	
						
						
							if( temp_int > 60  ){   //最大值
							
								strcpy( displayStr, "60.000"  );
								
								CLists.SYS_LL_V = 60 * 1000;
								
								break;
							}
							if( temp_int <= 0 ){    //最小值
							
								strcpy( displayStr, "0.000"  );
								
								CLists.SYS_LL_V = 0 * 1000;
								
								break;
							}						
							if( strlen(int_char) > 2){
								
								strcat( int_char, ".0");
								
							}else{
								
								strcat( int_char, ".000");
							}
							strcpy( displayStr, int_char );
							
							CLists.SYS_LL_V = temp_int * 1000;
							
						}else{

							if( temp_float - 60 > 0 ){   //最大值
							
								strcpy( displayStr, "60.000"  );
								
								CLists.SYS_LL_V = 60 * 1000;
								
								break;
							}
							
							if( temp_float <= 0 ){    //最小值
							
								strcpy( displayStr, "0.000"  );
								
								CLists.SYS_LL_V = 0 * 1000;
								
								break;
							}
							NUM = strlen(float_char) - dot_loc - 1;
							
							for( i = 0; i < 3 - NUM; i++ ){
								
								strcat( float_char, "0");
							}	
							NUM = ( dot_loc > 1 ) ? 6 : 5;
							
							strncpy( int_char, float_char, NUM );
							
							strcpy( displayStr, int_char );	
							
							CLists.SYS_LL_V = temp_float * 1000;
							
						}
					}
				
				break;	
				
				case 13:
				//======================================13 内阻上限========================================
					if( flag_null ){   
					
						strcpy( displayStr,"0"  );
						
						CLists.SYS_UL_II = 0;
						
					}else{
						
						num_int = atoi( SGhListSYS[17].f_EName );
						
						if( !flag_dot ){      	
							
							if( temp_int > 15  ){   //最大值
							
								strcpy( displayStr, "15"  );
								
								CLists.SYS_UL_II = 15;
								
								if( 15 - num_int <= 0 ){
									
									strcpy( SGhListSYS[17].f_SName, "0"  );
							
									strcpy( SGhListSYS[17].f_EName, "0"  );
									
									CLists.SYS_LL_IR = 0;
								}
								
								break;
							}
							if( temp_int <= 0 ){    //最小值
							
								strcpy( displayStr, "0"  );
								
								CLists.SYS_UL_II = 0 ;
								
								if( num_int >= 0 ){
									
									strcpy( SGhListSYS[17].f_SName, "0"  );
							
									strcpy( SGhListSYS[17].f_EName, "0"  );
									
									CLists.SYS_LL_IR = 0;
								}
								
								break;
							}						
						/* 	if( strlen(int_char) > 3){
								
								strcat( int_char, ".0");
								
							}else{
								
								strcat( int_char, ".00");
							} */
							strcpy( displayStr, int_char );
							
							CLists.SYS_UL_II = temp_int;
							
						}else{

							if( temp_float - 15 > 0 ){   //最大值
							
								strcpy( displayStr, "15"  );
								
								CLists.SYS_UL_II = 15;
								
								break;
							}
							if( temp_float <= 0 ){    //最小值
							
								strcpy( displayStr, "0"  );
								
								CLists.SYS_UL_II = 0;
								
								break;
							}
							if( dot_loc + 1 == strlen(displayStr) ){							
										
							}else{
								
								temp_int++;
								
								sprintf( int_char , "%d" , temp_int);		
							}
							
							strcpy( displayStr, int_char ); 
							
						/* 	NUM = strlen(float_char) - dot_loc - 1;
							
							for( i = 0; i < 3 - NUM; i++ ){
								
								strcat( float_char, "0");
							}	
							NUM = ( dot_loc > 1 ) ? 6 : 5; */
							
							strncpy( displayStr, float_char, NUM );
							
							//CLists.SYS_UL_II = temp_float * 1000;
						}					
						
						if( temp_int - num_int <= 0 ){
		
							strcpy( SGhListSYS[17].f_SName, "0"  );
							
							strcpy( SGhListSYS[17].f_EName, "0"  );
							
							CLists.SYS_LL_IR = 0;
						}
					}
				
				break;	
				
				case 16:
				//======================================17 内阻下限========================================			
					if( flag_null ){
						
						strcpy( displayStr,"0"  );
						
						CLists.SYS_LL_IR = 0 * 1000;
						
					}else{	
					
						num_int = atoi( SGhListSYS[13].f_EName );
						
						if( !flag_dot ){      	
							
							if( temp_int > 15  ){   //最大值
							
								strcpy( displayStr, "15"  );
								
								CLists.SYS_LL_IR = 15;
								
								if( num_int - 15 <= 0 ){
		
									strcpy( displayStr, "0"  );
										
									CLists.SYS_LL_IR = 0 ;			
								}
								
								break;
							}
							if( temp_int <= 0 ){    //最小值
							
								strcpy( displayStr, "0"  );
								
								CLists.SYS_LL_IR = 0;			
								
								break;
							}						
							/* if( strlen(int_char) > 2){
								
								strcat( int_char, ".0");
								
							}else{
								
								strcat( int_char, ".000");
								
							} */
							strcpy( displayStr, int_char );
							
							//CLists.SYS_LL_IR = temp_int * 1000;
							
						}else{

							if( temp_float - 15 > 0 ){   //最大值
							
								strcpy( displayStr, "15"  );
								
								CLists.SYS_LL_IR = 15;
								
								break;
							}
							if( temp_float <= 0 ){    //最小值
							
								strcpy( displayStr, "0"  );
								
								CLists.SYS_LL_IR = 0;
								
								break;
							}	
							if( dot_loc + 1 == strlen(displayStr) ){							
										
							}else{
								
								temp_int++;
								
								sprintf( int_char , "%d" , temp_int);		
							}
							
							strcpy( displayStr, int_char ); 
							
						/* 	NUM = strlen(float_char) - dot_loc - 1;
							
							for( i = 0; i < 3 - NUM; i++ ){
								
								strcat( float_char, "0");
							}	
							NUM = ( dot_loc > 1 ) ? 6 : 5; 
							
							strncpy( displayStr, float_char, NUM );*/
						}
						
						if( num_int - temp_int <= 0 ){
		
							strcpy( displayStr, "0"  );
							
							CLists.SYS_LL_IR = 0 ;
							
							break;
						}
						CLists.SYS_LL_IR = temp_int;
					}
				
				break;	
				
				case 19:
				//======================================21 过流上限========================================
					if( flag_null ){   
					
						strcpy( displayStr,"0.000"  );
						
						CLists.SYS_UL_Overflow = 0 * 1000;
						
					}else{
						
						num_float = atof( SGhListSYS[24].f_EName );
						
						if( temp_float - num_float <= 0 ){
		
							strcpy( SGhListSYS[24].f_SName, "0.000"  );
							
							strcpy( SGhListSYS[24].f_EName, "0.000"  );
							
							CLists.SYS_LL_Overflow = 0 * 1000;
						}
						
						if( !flag_dot ){      	
							
							if( temp_int > 30  ){   //最大值
							
								strcpy( displayStr, "30.000"  );
								
								CLists.SYS_UL_Overflow = 30 * 1000;
								
								break;
							}
							if( temp_int <= 0 ){    //最小值
							
								strcpy( displayStr, "0.000"  );
								
								CLists.SYS_UL_Overflow = 0 * 1000;
								
								break;
							}						
							if( strlen(int_char) > 2){
								
								strcat( int_char, ".0");
								
							}else{
								
								strcat( int_char, ".000");
							}
							strcpy( displayStr, int_char );
							
							CLists.SYS_UL_Overflow = temp_int * 1000;
							
						}else{	
							
							if( temp_float - 30 > 0 ){   //最大值
							
								strcpy( displayStr, "30.000"  );
								
								CLists.SYS_UL_Overflow = 30 * 1000;
								
								break;
							}
							if( temp_float <= 0 ){    //最小值
							
								strcpy( displayStr, "0.000"  );
								
								CLists.SYS_UL_Overflow = 0 * 1000;
								
								break;
							}
							NUM = strlen(float_char) - dot_loc - 1;
							
							for( i = 0; i < 3 - NUM; i++ ){
								
								strcat( float_char, "0");
							}	
							NUM = ( dot_loc > 1 ) ? 6 : 5;
							
							strncpy( int_char, float_char, NUM );
							
							strcpy( displayStr, int_char );	
							
							CLists.SYS_UL_Overflow = temp_float * 1000;
						}
					}
				
				break;
				
				case 22:
				//======================================24 过流下限========================================
					if( flag_null ){  
					
						strcpy( displayStr,"0.000"  );
						
						CLists.SYS_LL_Overflow = 0 * 1000;
						
					}else{
						
						num_float = atof( SGhListSYS[21].f_EName );
						
						if( num_float - temp_float <= 0 ){
							
							strcpy( displayStr, "0.000"  );						
								
							CLists.SYS_LL_Overflow = 0 * 1000;					
							
							break;
						}
						
						if( !flag_dot ){      	
							
							if( temp_int > 30  ){   //最大值
							
								strcpy( displayStr, "30.000"  );
								
								CLists.SYS_LL_Overflow = 30 * 1000;			
								
								break;
							}
							if( temp_int <= 0 ){    //最小值
							
								strcpy( displayStr, "0.000"  );
								
								CLists.SYS_LL_Overflow = 0 * 1000;
								
								break;
							}						
							if( strlen(int_char) > 2){
								
								strcat( int_char, ".0");
								
							}else{
								
								strcat( int_char, ".000");
							}
							strcpy( displayStr, int_char );			
							
							CLists.SYS_LL_Overflow = temp_int * 1000;
							
						}else{
							
							if( temp_float - 30 > 0 ){   //最大值
							
								strcpy( displayStr, "30.000"  );
								
								CLists.SYS_LL_Overflow = 30 * 1000;
								
								break;
							}
							
							if( temp_float <= 0 ){    //最小值
							
								strcpy( displayStr, "0.000"  );
								
								CLists.SYS_LL_Overflow = 0 * 1000;
								
								break;
							}
							NUM = strlen(float_char) - dot_loc - 1;
							
							for( i = 0; i < 3 - NUM; i++ ){
								
								strcat( float_char, "0");
							}	
							NUM = ( dot_loc > 1 ) ? 6 : 5;
							
							strncpy( int_char, float_char, NUM );
							
							strcpy( displayStr, int_char );	
									
							CLists.SYS_LL_Overflow = temp_float * 1000;
						}
					}
				
				break;		
			}	
			Write_Flash_Bytes();	
			
			SGhListSYS[temp].d_BKColor = 0x0;
			
			strcpy( SGhListSYS[temp].f_SName, displayStr );
			
			strcpy( SGhListSYS[temp].f_EName, displayStr );
		}
	}
	memset( displayStr, '\0', sizeof(displayStr));
	
	WM_InvalidateWindow(sysSettingWin);

}































